// Connect.cpp : Implementation of CConnect
#include "stdafx.h"
#include "ServerCOM.h"
#include "Connect.h"

/////////////////////////////////////////////////////////////////////////////
// CConnect



STDMETHODIMP CConnect::Test(int a)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	CValue Val;
	Val=a;
	AfxMessageBox(Val.GetString());

	return S_OK;
}

STDMETHODIMP CConnect::Test2()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CConnect::Test3(VARIANT A)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CValue Val;
//	Val=A.bstrVal;
	AfxMessageBox(Val.GetString());

	return S_OK;
}

STDMETHODIMP CConnect::put_Test4(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	AfxMessageBox(CString(newVal));

	return S_OK;
}

STDMETHODIMP CConnect::put_Test5(BSTR A, BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	AfxMessageBox(CString(A));
	AfxMessageBox(CString(newVal));

	return S_OK;
}

STDMETHODIMP CConnect::Test6()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CConnect::Test7(BSTR A)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	AfxMessageBox(CString(A));

	return S_OK;
}

STDMETHODIMP CConnect::EvalExpr(BSTR Str)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strRun=CString(Str);
	AfxMessageBox(strRun);
	Eval(strRun);

	return S_OK;
}

STDMETHODIMP CConnect::EvalExpr2(BSTR Str, VARIANT *Ret)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strRun=CString(Str); 
	SysFreeString(Str); 
	//AfxMessageBox(strRun);


	CString csRes=Eval(strRun);

	Ret->vt=VT_BSTR;
	Ret->bstrVal=csRes.AllocSysString();//CString("1234567890").AllocSysString();
	return S_OK;
}

STDMETHODIMP CConnect::get_Test8(BSTR **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	//**pVal=CString("0123456789").AllocSysString();

	return S_OK;
}

STDMETHODIMP CConnect::put_Test8(BSTR *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CConnect::get_Test9(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal=CString("0123456789").AllocSysString();

	return S_OK;
}

STDMETHODIMP CConnect::put_Test9(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CConnect::get_Test10(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	pVal->vt=VT_BSTR;
	pVal->bstrVal=CString("1234567890").AllocSysString();

	return S_OK;
}

STDMETHODIMP CConnect::put_Test10(VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CConnect::get_Test11(VARIANT **pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	static VARIANT var;

	return S_OK;
}

STDMETHODIMP CConnect::get_Array(VARIANT *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// Create a new 1-D array of Integers.
	SAFEARRAY * psaiNew; 
	SAFEARRAYBOUND aDim[1]; 
	aDim[0].lLbound = 1; 
	aDim[0].cElements = 8; 
	// Equivalent to: Dim aiNew(1 To 8) As Integer.
	psaiNew = SafeArrayCreate(VT_I2, 1, aDim);
	if (psaiNew == NULL) throw ERROR_NOT_ENOUGH_MEMORY;
	 // Use array.

	pVal->vt=VT_SAFEARRAY;
	pVal->parray=psaiNew;

	//if (hres = SafeArrayDestroy(psaiNew)) throw hres;


	return S_OK;
}

STDMETHODIMP CConnect::put_Array(VARIANT newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}
