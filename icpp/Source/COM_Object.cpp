
#include "stdafx.h"
#include "afxpriv2.h"
#include "COM_Object.h"

extern CBkEndUI * pBkEndUI;

BEGIN_BL_METH_MAP(CCOM_Object)
    BL_METH("CreateObject", "СоздатьОбъект", 3, NULL, &methCreateObject, NULL)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CCOM_Object)
    BL_PROP("Object", "Объект", &GetPropObject, NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CCOM_Object, "COM_Object", "COM_Object", 1, NULL, NULL, -1);


CCOM_Object::~CCOM_Object()
{
}

BOOL CCOM_Object::methCreateObject(CValue &RetVal, CValue **params)
{
	CBLContext* pBLCont = params[0]->GetContext();
	CString strEventPrefix = params[1]->GetString();
	CString strClassName = params[2]->GetString();

	if( pBLCont == NULL )
		RuntimeError("Не указан обработчик событий");

	if( IS_KINDOF_CGroupContext(pBLCont) || IS_KINDOF_CComponentClass(pBLCont) )
	{
		Container.CreateObject(strClassName, pBLCont, strEventPrefix);
	}
	else
	{
		RuntimeError("Неверно указан обработчик событий");
	}

	return TRUE;
}

BOOL CCOM_Object::GetPropObject(CValue &Value) const
{
	if( Container.m_pUnknown )
	{
		DispatchToValue(Container.m_pUnknown, &Value);
	}
	return TRUE;
}



BEGIN_INTERFACE_MAP( CCOM_ObjectContainer, CCmdTarget )
	INTERFACE_PART( CCOM_ObjectContainer, IID_IDispatch, EventHandler )
END_INTERFACE_MAP()


CCOM_ObjectContainer::~CCOM_ObjectContainer()
{
	if( m_pUnknown )
	{
		CloseSink();
		m_pUnknown->Release();
		m_pUnknown->Release();
		delete m_pUnknown;
	}
	m_pUnknown = NULL;
}

BOOL CCOM_ObjectContainer::CreateObject(CString strClassName, CBLContext* pBLCont, CString strEventPrefix)
{
	if( IS_KINDOF_CComponentClass(pBLCont) )
	{
		CComponentClass* pClass = (CComponentClass*)pBLCont;
		m_pHostModule = pClass->GetModule();
	}
	else
	{
		CGroupContext* pCont = (CGroupContext*)pBLCont;
		m_pHostModule = GetModuleByContext(pCont);
	}

	m_ClassName = strClassName;
	m_EventPrefix = strEventPrefix + "_";


	BSTR bStr = m_ClassName.AllocSysString();

	HRESULT res1, res2;
	CLSID cls_id;
	LPUNKNOWN pUnk;
	LPCLASSFACTORY pFact;

	res1 = CLSIDFromProgID(bStr, &cls_id);
	delete bStr;

	if( res1 == CO_E_CLASSSTRING )
		RuntimeError("Invalid class name");
	else if( res1 == REGDB_E_WRITEREGDB )
		RuntimeError("REGDB_E_WRITEREGDB");

	res2 = ::CoGetClassObject(cls_id, CLSCTX_INPROC_SERVER, NULL, IID_IClassFactory, (void**)&pFact);

	pFact->CreateInstance(NULL, IID_IUnknown, (void**)&pUnk);

	pFact->Release();
	LPDISPATCH pDisp;
	HRESULT res3 = pUnk->QueryInterface(IID_IDispatch, (void**)(&pDisp));

	if( res2 == 0 )
	{
		m_pUnknown = pUnk;
		SetupEventHandlers();
		EstablishSink();
	}

	return TRUE;
}

void CCOM_ObjectContainer::SetupEventHandlers()
{
	IProvideClassInfoPtr pPCI;	
	HRESULT hResult = m_pUnknown->QueryInterface( IID_IProvideClassInfo, (void**)&pPCI );

	if( FAILED( hResult ) )
		return;

	ITypeInfoPtr pClassInfo;
	hResult = pPCI->GetClassInfo(&pClassInfo);
	if( FAILED( hResult ) )
		return;

	TYPEATTR *pTypeAttr;
	hResult = pClassInfo->GetTypeAttr(&pTypeAttr);
	if( FAILED( hResult ) )
		return;

	for(UINT iType = 0; (iType < pTypeAttr->cImplTypes) ; iType++ )
	{
		int iFlags;
		if(S_OK == pClassInfo->GetImplTypeFlags( iType, &iFlags ) )
		{
			if((iFlags & IMPLTYPE_MASK) == IMPLTYPE_DEFAULTSOURCE )
			{
				ITypeInfoPtr pTypeInfo;
				HREFTYPE hRefType;
				if(S_OK == pClassInfo->GetRefTypeOfImplType( iType, &hRefType ))
				{
					if(S_OK == pClassInfo->GetRefTypeInfo(hRefType, &pTypeInfo) && pTypeInfo!=NULL)
					{
						int iMethod;
						UINT nNames;
						TYPEATTR* pta;
						if(S_OK == pTypeInfo->GetTypeAttr( &pta ))
						{
							m_iid = pta->guid;

							for( iMethod = 0; iMethod < pta->cFuncs; iMethod++ )
							{
								FUNCDESC* pfd;
								if(S_OK == pTypeInfo->GetFuncDesc( iMethod, &pfd ))
								{
									if( !(pfd->wFuncFlags&FUNCFLAG_FRESTRICTED) &&
										(pfd->funckind == FUNC_DISPATCH) )
									{
										CComBSTR bstrNames=NULL;
										if(S_OK == pTypeInfo->GetNames( pfd->memid, &bstrNames, 1,&nNames ))
										{
											CString funcName(bstrNames);
											CString nameProc;
											int nHandlerMethNo = -1;

											if( m_pHostModule )
											{
												nameProc = m_EventPrefix + funcName;
												nHandlerMethNo = m_pHostModule->FindProc(nameProc, 0);
												if( nHandlerMethNo < 0 )
													nHandlerMethNo = m_pHostModule->FindFunc(nameProc);
											}

											if( nHandlerMethNo >= 0 )
											{
												EventInfo ei = {nHandlerMethNo, pfd->cParams};
												m_EventHandlers->SetAt(pfd->memid, ei);
											}
										}
									}
									pTypeInfo->ReleaseFuncDesc(pfd);
								}
							}
							pTypeInfo->ReleaseTypeAttr(pta);
						}
					}
				}
				break;
			}
		}
	}
	pClassInfo->ReleaseTypeAttr(pTypeAttr);
}

void CCOM_ObjectContainer::CallEventHandler(EventInfo ei, CValue** lArray, CBLModule7* pModule)
{
	if(ei.procid>0)
	{
		CBLProcInfo cblProcInfo;
		if( pModule->GetProcInfo(ei.procid, cblProcInfo) )
		{
			int newArgNum = cblProcInfo.GetNParams();
			if(newArgNum > ei.argNum) newArgNum = ei.argNum;
			int MethType = cblProcInfo.GetType();

			//if( m_pDoc ) m_pDoc->OnStartBatch();

			if( MethType == 1 )
			{
				pModule->CallAsProc(ei.procid, newArgNum, lArray);
			}
			else
			{
				CValue arValue;
				pModule->CallAsFunc(ei.procid, arValue, newArgNum, lArray);
			}
			
			//if( m_pDoc ) m_pDoc->OnEndBatch(1);
		}
	}

}



///////////////////////////////////////////////////////////////////////////////
// CCOM_ObjectContainer::XEventHandler
///////////////////////////////////////////////////////////////////////////////

STDMETHODIMP_( ULONG ) CCOM_ObjectContainer::XEventHandler::AddRef()
{
	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )

	return( E_NOTIMPL );
	//return( pThis->ExternalAddRef() );
}

STDMETHODIMP_( ULONG ) CCOM_ObjectContainer::XEventHandler::Release()
{
	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )

	return( E_NOTIMPL );
	//return( pThis->ExternalRelease() );
}

STDMETHODIMP CCOM_ObjectContainer::XEventHandler::QueryInterface( REFIID iid, LPVOID* ppvObj)
{
	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )

	if( IsEqualIID( iid, IID_IDispatch ) )
	{
		// QI'ing for IID_IDispatch will get you the default dispinterface on an
		// object.  In the case of the control site, that would be the ambient
		// properties dispinterface.  Since the control's event sink connection
		// point is exposed not as IID_IDispatch, but rather as the IID of the
		// control's event dispinterface,
	}

	HRESULT hr = pThis->ExternalQueryInterface( &iid, ppvObj );
	return hr;
}

STDMETHODIMP CCOM_ObjectContainer::XEventHandler::GetIDsOfNames( REFIID iid, LPOLESTR* ppszNames, UINT nNames, LCID lcid, DISPID* pDispIDs )
{
	(void)iid;
	(void)ppszNames;
	(void)nNames;
	(void)lcid;
	(void)pDispIDs;

	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )
		ASSERT_VALID( pThis );

	return( E_NOTIMPL );
}

STDMETHODIMP CCOM_ObjectContainer::XEventHandler::GetTypeInfo( UINT iTypeInfo, LCID lcid, ITypeInfo** ppTypeInfo )
{
	(void)iTypeInfo;
	(void)lcid;
	(void)ppTypeInfo;

	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )
		ASSERT_VALID( pThis );

	return( E_NOTIMPL );
}

STDMETHODIMP CCOM_ObjectContainer::XEventHandler::GetTypeInfoCount( UINT* pnInfoCount )
{
	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )
		ASSERT_VALID( pThis );

	*pnInfoCount = 0;

	return( S_OK );
}

STDMETHODIMP CCOM_ObjectContainer::XEventHandler::Invoke( DISPID dispidMember,
													 REFIID iid, LCID lcid, USHORT wFlags, DISPPARAMS* pdpParams,
													 VARIANT* pvarResult, EXCEPINFO* pExceptionInfo, UINT* piArgErr )
{
	(void)iid;
	(void)lcid;
	(void)wFlags;
	(void)pvarResult;
	(void)pExceptionInfo;
	(void)piArgErr;

	METHOD_PROLOGUE( CCOM_ObjectContainer, EventHandler )
		ASSERT_VALID( pThis );

	pThis->OnEvent( dispidMember, pdpParams );  // Call the event handler, who
	// also cleans up.

	return( S_OK );
}

HRESULT TraceSafeArray(SAFEARRAY * psa);

void CCOM_ObjectContainer::OnEvent( DISPID dispid, DISPPARAMS* pdpParams )
{
	if( m_pHostModule->IsValidObject() )
	{
		EventInfo ei;
		BOOL foundModuleHandler = m_EventHandlers->Lookup(dispid, ei);

		if(foundModuleHandler )
		{
			CValue p1, p2, p3, p4, p5, p6, p7, p8, p9, p10;
			CValue *lArray[10]={&p1, &p2, &p3, &p4, &p5, &p6, &p7, &p8, &p9, &p10};
			UINT ui;
			((CBLMyEx*)NULL)->DispParamToValueEx(lArray, pdpParams,&ui);

			CallEventHandler(ei,lArray, (CBLModule7*)m_pHostModule);

			for(int i=0;i<pdpParams->cArgs;i++)
			{
				VARIANTARG v=pdpParams->rgvarg[i];
				if(v.vt & VT_BYREF)
				{
					CValue RetValue=*(lArray[pdpParams->cArgs-i-1]);
					VARTYPE ordType = v.vt ^ VT_BYREF;
					switch (RetValue.type)
					{
					case 1:
						switch (ordType)
						{
						case VT_UI1: *(v.pbVal)		=RetValue.m_Number; break;
						case VT_UI2: *(v.puiVal)	=RetValue.m_Number; break;
						case VT_UI4: *(v.pulVal)	=RetValue.m_Number; break;
						case VT_I1: *(v.pcVal)		=RetValue.m_Number; break;
						case VT_I2: *(v.piVal)		=RetValue.m_Number; break;
						case VT_I4: *(v.plVal)		=RetValue.m_Number; break;
						case VT_INT: *(v.pintVal)	=RetValue.m_Number; break;
						case VT_UINT: *(v.puintVal)	=RetValue.m_Number; break;
						case VT_BOOL: *(v.piVal)	=RetValue.m_Number; break;
						case VT_R4: *(v.pfltVal)	=RetValue.m_Number; break;
						default:
							((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal); break;
						}
						break;
					case 2:
						switch (ordType)
						{
						case VT_BSTR:
							*(v.pbstrVal) = RetValue.m_String.AllocSysString(); break;
						}
						break;
					case 3:
						((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal);
						break;
					default:
						if((ordType !=VT_DISPATCH) && (ordType !=VT_UNKNOWN))
							((CBLMyEx*)NULL)->ValueToVariantEx(RetValue,v.pvarVal);
					}
				}
			}
		}
	}

}

IUnknown* CCOM_ObjectContainer::GetInterfaceHook( const void* pv )
{
	const IID* piid;

	// #ifdef _DEBUG
	// 	// The standard MFC implementation of QueryInterface doesn't switch module
	// 	// states, since most objects just use static data to implement QI.  Since
	// 	// we're calling GetDocument(), though, we wind up calling ASSERT_VALID on
	// 	// a bunch of MFC objects, and that requires the correct module state.
	// 	// Because ASSERT_VALID doesn't do anything in release builds, we only need
	// 	// to switch module states in debug builds.
	AFX_MANAGE_STATE( m_pModuleState );
	// #endif  // _DEBUG

	piid = (const IID*)pv;

	if( *piid == m_iid )
	{
		return( &m_xEventHandler );
	}

	return( NULL );
}

BOOL CCOM_ObjectContainer::EstablishSink()
{
	assert(NULL != m_pUnknown);

	CloseSink();

	IConnectionPointContainerPtr pConnPtContainer;

	HRESULT hr = m_pUnknown->QueryInterface(IID_IConnectionPointContainer, (void**)&pConnPtContainer);
	if ( FAILED(hr) )
		return FALSE;

	hr = pConnPtContainer->FindConnectionPoint(m_iid, &m_pIConnectionPoint);
	if ( FAILED(hr) )
		return FALSE;

	assert(m_pIConnectionPoint != NULL);
	assert(!IsSinkToEvent());

	if( !IsSinkToEvent() ){
		hr = m_pIConnectionPoint->Advise(&m_xEventHandler, &m_dwEventCookie);
		if( CONNECT_E_CANNOTCONNECT == hr)
			return FALSE;
		if( CONNECT_E_ADVISELIMIT == hr)
			return FALSE;

		assert(SUCCEEDED(hr));
		assert(IsSinkToEvent());
	}

	//iUnkControl->Release();
	return TRUE;
}

void CCOM_ObjectContainer::CloseSink()
{
	if( !m_pUnknown )
	{
		m_dwEventCookie = 0;
		m_pIConnectionPoint = NULL;
		return;
	}

	if (IsSinkToEvent())
	{
		assert(m_pIConnectionPoint != NULL);

		HRESULT hr = m_pIConnectionPoint->Unadvise(m_dwEventCookie);
		assert(SUCCEEDED(hr));
		m_dwEventCookie = 0;
		m_pIConnectionPoint = NULL;
	}
}