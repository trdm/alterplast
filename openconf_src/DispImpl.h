// DispImpl.h: interface for the CDispImpl class.
#ifndef AFX_DISPIMPL_H
#define AFX_DISPIMPL_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template<class T,class C=T>
class CDispImpl:public T,ISupportErrorInfo,IProvideClassInfo
{
public:
	typedef T TypeOfInterface;
	typedef C TypeOfClass;
	CDispImpl():m_dwRef(1)
	{
		if(!m_pTypeInfo)
		{
			CConfigSvcImpl::m_pTheService->m_pTypeLib->GetTypeInfoOfGuid(__uuidof(T),&m_pTypeInfo);
		}
	}
	virtual ~CDispImpl(){};
	// IDispatch
    STDMETHOD(GetTypeInfoCount)(UINT *pctinfo)
	{
		*pctinfo = 1;
		return S_OK;
	}
    STDMETHOD(GetTypeInfo)(UINT iTInfo,LCID lcid,ITypeInfo **ppTInfo)
	{
		*ppTInfo = NULL;
		if(iTInfo != 0)
			return DISP_E_BADINDEX;
		m_pTypeInfo->AddRef();
		*ppTInfo = m_pTypeInfo;
		return S_OK;
	}
    STDMETHOD(GetIDsOfNames)(REFIID riid,LPOLESTR *rgszNames,UINT cNames,
        LCID lcid,DISPID *rgDispId)
	{
		return DispGetIDsOfNames(m_pTypeInfo,rgszNames,cNames,rgDispId);
	}

    STDMETHOD(Invoke)(DISPID dispIdMember,REFIID riid,LCID lcid,
        WORD wFlags,DISPPARAMS *pDispParams,VARIANT *pVarResult,
        EXCEPINFO *pExcepInfo,UINT *puArgErr)
	{
		return DispInvoke(this,m_pTypeInfo,dispIdMember,wFlags,pDispParams,pVarResult,
			pExcepInfo,puArgErr);
	}
	// ISupportErrorInfo
	STDMETHOD(InterfaceSupportsErrorInfo)(REFIID riid)
	{
		if(riid==__uuidof(T))
			return S_OK;
		return S_FALSE;
	}
	// IProvideClassInfo
	STDMETHOD(GetClassInfo)(ITypeInfo** ppTI)
	{
		if(IsBadWritePtr(ppTI,sizeof(*ppTI)))
			return E_POINTER;
		return CConfigSvcImpl::m_pTheService->m_pTypeLib->GetTypeInfoOfGuid(__uuidof(C),ppTI);
	}
	// IUnknown
	STDMETHOD(QueryInterface)(REFIID riid,void **ppvObject)
	{
		if(!ppvObject)
			return E_INVALIDARG;
		*ppvObject=NULL;
		if(riid==IID_IDispatch)
			*ppvObject=(IDispatch*)this;
		else if(riid==IID_IUnknown)
			*ppvObject=this;
		else if(riid==IID_ISupportErrorInfo)
			*ppvObject=(ISupportErrorInfo*)this;
		else if(riid==__uuidof(T))
			*ppvObject=(T*)this;
		else if(riid==IID_IProvideClassInfo)
		{
			if(__uuidof(T)==__uuidof(C))
				return E_NOINTERFACE;
			*ppvObject=(IProvideClassInfo*)this;
		}
		else if(AdditionalQI(riid,ppvObject))
			return E_NOINTERFACE;
		m_dwRef++;
		return S_OK;
	}
	virtual HRESULT AdditionalQI(REFIID riid,void **ppvObject)
	{
		return E_NOINTERFACE;
	}
	virtual ULONG STDMETHODCALLTYPE AddRef(void){return ++m_dwRef;}
	virtual ULONG STDMETHODCALLTYPE Release(void)
	{
		if(!--m_dwRef)
			delete this;
		return m_dwRef;
	}
	HRESULT SetError(HRESULT err,LPCTSTR format,...)
	{
		ICreateErrorInfo* pError=NULL;
		CreateErrorInfo(&pError);
		if(pError)
		{
			va_list arg;
			va_start(arg,format);
			CString txt;
			txt.FormatV(format,arg);
			_bstr_t descr=txt;
			pError->SetDescription(descr);
			pError->SetGUID(__uuidof(T));
			pError->SetSource(L"Конфигуратор");
			SetErrorInfo(0,(IErrorInfo*)pError);
			va_end(arg);
		}
		return err;
	}
	long m_dwRef;
	static ITypeInfo *m_pTypeInfo;
};
/*
Для того, чтобы в VC++ было удобно работать с реализацией интерфейсов,
придется его обмануть. Как выяснилось, VC++ привязывает интерфейс к
реализации по ATL макросам, перечисленным ниже.
Так как я не использую ATL для создания объектов, то в своей реализации
IDispatch я переопределю эти макросы на пустые, и буду вставлять в свои
классы.
*/
#define BEGIN_COM_MAP(par)
#define COM_INTERFACE_ENTRY(par)
#define END_COM_MAP()

#define DISPIMPL_STD() \
STDMETHOD(QueryInterface)(REFIID iid,void** pObj){return CDispImpl<TypeOfInterface,TypeOfClass>::QueryInterface(iid,pObj);} \
ULONG STDMETHODCALLTYPE AddRef(){return CDispImpl<TypeOfInterface,TypeOfClass>::AddRef();} \
ULONG STDMETHODCALLTYPE Release(){return CDispImpl<TypeOfInterface,TypeOfClass>::Release();}

#endif
