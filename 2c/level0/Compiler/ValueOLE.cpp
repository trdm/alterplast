// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueOLE.cpp: implementation of the CValueOLE class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "enterprise.h"
#include "ValueOLE.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CString GetGetLastStringError();


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define CASE_NUMBER(x,y)\
		case x:\
			Val.nType=TYPE_NUMBER;\
			Val.fData=data.y;\
			break;\

CValue FromVariantArray(SAFEARRAY *pArray);
CValue FromVariant(VARIANT &data)
{
	CValue Val;
	switch(data.vt)
	{
		CASE_NUMBER(VT_UI1,bVal)
		CASE_NUMBER(VT_I2,iVal)
		CASE_NUMBER(VT_I4,lVal)
		CASE_NUMBER(VT_BOOL,boolVal)
		CASE_NUMBER(VT_R4,fltVal)
		CASE_NUMBER(VT_R8,dblVal)
		CASE_NUMBER(VT_I1,cVal)
		CASE_NUMBER(VT_UI2,uiVal)
		CASE_NUMBER(VT_UI4,ulVal)
		CASE_NUMBER(VT_INT,intVal)
		CASE_NUMBER(VT_UINT,uintVal)
		case VT_BSTR:
			Val.nType=TYPE_STRING;
			Val.sData=data.bstrVal;
			SysFreeString(data.bstrVal); 
			break;
		case VT_DATE:
			Val.nType=TYPE_DATE;
			Val.dData=data.date;//TODO
			break;
		case VT_DISPATCH:
			{
				if(data.pdispVal)
				{
					CValueOLE *pVal=new CValueOLE(data.pdispVal);
					data.pdispVal->AddRef();
					Val=ValueByRef(pVal);
				}
			}
			break;
		case VT_SAFEARRAY:
			if(data.parray)
			{
				Val=FromVariantArray(data.parray);
				//SafeArrayDestroy(data.parray);
				Error("В данной версии массивы не поддерживаются.");
			}
			break;

	}
	return Val;
}
void AddFromArray(CValue &Ret,long *aPos,SAFEARRAY *pArray,SAFEARRAYBOUND *aDims,int nLastDim);
CValue FromVariantArray(SAFEARRAY *pArray)
{
	HRESULT hres;
	CValue Ret;
	Ret.CreateObject("Массив");
	long nDim = SafeArrayGetDim(pArray); 
	long *aPos = new long[nDim];
	SAFEARRAYBOUND * aDims = new SAFEARRAYBOUND[nDim];

	for (int i = 0; i < nDim; i++) 
	{
		long nMin,nMax;
		hres = SafeArrayGetLBound(pArray, i + 1, &nMin);
		if (hres) 
			throw hres; 
		if (hres = SafeArrayGetUBound(pArray, i + 1, &nMax)) throw hres;

		aPos[i]=nMin;//начальное положение
		aDims[i].lLbound=nMin;
		aDims[i].cElements=nMax;//-nMin+1;
	}
	AddFromArray(Ret,aPos,pArray,aDims,nDim-1);

	delete []aPos;
	delete []aDims;
	return Ret;
}

void AddFromArray(CValue &Ret,long *aPos,SAFEARRAY *pArray,SAFEARRAYBOUND *aDims,int nLastDim)
{
	HRESULT hres;
	VARIANT Val;
    if (hres = SafeArrayGetElement(pArray, aPos, &Val)) throw hres;

	aPos[nLastDim]++;
	if(aPos[nLastDim]>aDims[nLastDim].cElements)
	{
		aPos[nLastDim]=aDims[nLastDim].lLbound;
		//AddFromArray(CValue &Ret,long *aPos,SAFEARRAY *pArray,SAFEARRAYBOUND *aDims,int nLastDim)
	}


/*
		AddFromArray(pArray,nMin
		if(i+1<nDim)
		{
			CValue Val;
			Val.CreateObject("Массив");
			Ret
		}

*/
}


void FreeValue(VARIANT &Val)
{
	switch(Val.vt)
	{
		case VT_BSTR:
			SysFreeString(Val.bstrVal);
			break;
	}
}  

VARIANT FromValue(CValue &Val)
{
	VARIANT data;
	int nType=Val.GetType();
	switch(nType)
	{
		case TYPE_NUMBER:
			{
				double fData=Val.GetNumber();
				if(double(int(fData))==fData)
				{
					data.vt=VT_I4;
					data.lVal=fData;
				}
				else
				{
					data.vt=VT_R8;
					data.dblVal=fData;
				}
				break;
			}
		case TYPE_STRING:
			data.vt=VT_BSTR;
			data.bstrVal=SysAllocString(CComBSTR(Val.GetString()));
			break;
		case TYPE_DATE:
			data.vt=VT_DATE;
			data.date=Val.GetDate();//TODO
			break;
		case TYPE_OLE:
			data.vt=VT_DISPATCH;
			data.pdispVal=((CValueOLE*)Val.pRef)->disp.m_lpDispatch;
			data.pdispVal->AddRef();//TODO
			break;
	}
	return data;
}

class CAutoComInit
{
public:
	CAutoComInit()
	{
		CoInitialize(0);
	};
}ComInit;

CValueOLE::CValueOLE(IDispatch *p)
{
	disp.m_bAutoRelease=1;
	nType=TYPE_OLE;
//	if(p)
//		disp.AttachDispatch(p,FALSE);
	disp=p;
	csObjectName="OLE";
}

CValueOLE::~CValueOLE()
{
//	if(disp.m_lpDispatch)
//		disp.m_lpDispatch->Release();
}

BOOL CValueOLE::Create(CString csName)
{
   csObjectName=csName;

   COleException *e = new COleException;
	try 
	{
		// Create instance by using ProgID.
		if (disp.CreateDispatch(csName, e))
		{
			disp.m_bAutoRelease=1;
		    e->Delete();
			return TRUE;
		}
	     e->Delete();
	}
    catch (COleDispatchException * e) 
	{
      CString cStr;

      if (!e->m_strSource.IsEmpty())
         cStr = e->m_strSource + " - ";
      if (!e->m_strDescription.IsEmpty())
         cStr += e->m_strDescription;
      else
         cStr += "unknown error";

      AfxMessageBox(cStr, MB_OK, 
         (e->m_strHelpFile.IsEmpty())? 0:e->m_dwHelpContext);

      e->Delete();
	}
	return FALSE;
}


int CValueOLE::FindAttribute(CString csName)
{
	CComBSTR cbstrName(csName);
	HRESULT hresult;
	IDispatch FAR* pdisp = disp.m_lpDispatch;
	DISPID dispid=-1;
	OLECHAR FAR* szMember = cbstrName;
	hresult = pdisp->GetIDsOfNames(
		IID_NULL,
		&szMember,
		1, LOCALE_SYSTEM_DEFAULT,
		&dispid);

//	if(!SUCCEEDED(hresult)||dispid==1000)
	if(hresult!=S_OK||dispid==1000)
	{
		//Error(CString("Не найден атрибут или метод ")+csName);
		return -1;
	}

	return dispid;
}

CValue CValueOLE::GetAttribute(int dispid)
{
	VARIANT Res;
	disp.InvokeHelper(dispid, DISPATCH_PROPERTYGET, VT_VARIANT, 
		&Res, NULL);             
	return FromVariant(Res);
}
void CValueOLE::SetAttribute(int dispid,CValue &Val)
{
	VARIANT Var=FromValue(Val);
	CExtOleDispatchDriver::PutProperty(disp,dispid,&Var);
	FreeValue(Var);

}
int CValueOLE::FindMethod(CString csName)
{
 	return FindAttribute(csName);
}



#define MAX_PARAMS_COUNT	10
CValue CValueOLE::Method(int dispid,CValue **aParams)
{
	COleVariant varRet;
	varRet.Clear();
	int nParamCount=0;
	while(aParams[nParamCount])
	{
		if(aParams[nParamCount]->nType==TYPE_EMPTY)
			break;
		nParamCount++;
	}

	//переводим параметры в тип VARIANT
	COleVariant *pvarArgs=new COleVariant [nParamCount];
	for(int i=0;i<nParamCount;i++)
	{
		pvarArgs[nParamCount-i-1]=FromValue(*aParams[i]);
	}


	EXCEPINFO FAR  ExcepInfo;
	DISPPARAMS dispparams = { &pvarArgs[0], NULL, nParamCount, 0};
	HRESULT hresult=((LPDISPATCH)(disp))->Invoke(dispid, IID_NULL, LOCALE_USER_DEFAULT, DISPATCH_METHOD | DISPATCH_PROPERTYGET, &dispparams, &varRet, &ExcepInfo, NULL);

	for(i=0;i<nParamCount;i++)
	{
		FreeValue(pvarArgs[i]);
	}
	delete []pvarArgs;

	if(hresult!=S_OK)
	{
		switch(hresult)
		{
		case DISP_E_EXCEPTION:
			Error("%s:\n%s",ExcepInfo.bstrSource,ExcepInfo.bstrDescription);
			break;
		case DISP_E_BADPARAMCOUNT:
			Error("The number of elements provided to DISPPARAMS is different from the number of arguments accepted by the method or property.");
			break;

		case DISP_E_BADVARTYPE:
			Error("One of the arguments in rgvarg is not a valid variant type. ");
			break;
		case DISP_E_MEMBERNOTFOUND:
			Error("The requested member does not exist, or the call to Invoke tried to set the value of a read-only property.");
			break;

		case DISP_E_NONAMEDARGS:
			Error("This implementation of IDispatch does not support named arguments.");
			break;
		case DISP_E_OVERFLOW:
			Error("One of the arguments in rgvarg could not be coerced to the specified type.");
			break;
		case DISP_E_PARAMNOTFOUND:
			Error("One of the parameter DISPIDs does not correspond to a parameter on the method. In this case, puArgErr should be set to the first argument that contains the error.");
			break;
		case DISP_E_TYPEMISMATCH:
			Error("One or more of the arguments could not be coerced. The index within rgvarg of the first parameter with the incorrect type is returned in the puArgErr parameter.");
			break;
		case DISP_E_UNKNOWNINTERFACE:
			Error("The interface identifier passed in riid is not IID_NULL.");
			break;
		case DISP_E_UNKNOWNLCID:
			Error("The member being invoked interprets string arguments according to the LCID, and the LCID is not recognized. If the LCID is not needed to interpret arguments, this error should not be returned.");
			break;
		case DISP_E_PARAMNOTOPTIONAL:
			Error("A required parameter was omitted.");
			break;

		default:
			Error("Ошибка");
			break;
		}
	}

	return FromVariant(varRet);
}
