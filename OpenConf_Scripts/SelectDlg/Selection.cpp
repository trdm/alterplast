// Selection.cpp : Implementation of CSelection
#include "stdafx.h"
#include "SelectDialog.h"
#include "Selection.h"
#include "SelectFrm.h"

/////////////////////////////////////////////////////////////////////////////
// CSelection


STDMETHODIMP CSelection::put_ListToChoose(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	CString AllStrLines = newVal;

	int Pos = AllStrLines.Find(char(13));
	while(Pos >= 0)
	{
		AllStrings.Add(AllStrLines.Left(Pos));
		AllStrLines = AllStrLines.Mid(Pos + 2);
		Pos = AllStrLines.Find(char(13));
	}

	AllStrings.Add(AllStrLines);
	return S_OK;
}


STDMETHODIMP CSelection::get_DoSelection(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	
	CWnd* ModuleWnd = CWnd::FromHandle(HWND(ModuleDescription));
	CWnd* Conf = ModuleWnd->GetParent()->GetParent();
	
	SelectFrm SelDlg(Conf);
	SelDlg.ModuleDescription = ModuleDescription;
	SelDlg.SetInitialText(InitialText);
	CString SelectedItem = SelDlg.DoChoice(&AllStrings);
	*pVal = SelectedItem.AllocSysString();
	
	Conf->SetFocus();

	return S_OK;
}

STDMETHODIMP CSelection::get_WndDescrsSet(long ConfDescr, long ModuleDescr, long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here
	ModuleDescription = ModuleDescr;

	return S_OK;
}

STDMETHODIMP CSelection::Try()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	return S_OK;
}

STDMETHODIMP CSelection::put_InitialText(BSTR *newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	// TODO: Add your implementation code here

	InitialText = *newVal;

	return S_OK;
}
