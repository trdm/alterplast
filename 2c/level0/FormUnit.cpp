//Авторские права - VTOOLS.RU (info@vtools.ru)
// FormUnit.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FormUnit.h"
#include "MainFrm.h"
#include "DocUnit.h"
#include "FormFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CPARENT CFormView 
#define VIRUALFUNCTION GridVirtualModeFunc2
#include "CommonUnit.cpp"
#undef VIRUALFUNCTION

void OutTime(int n);
void InitTime();

CFormUnit::CFormUnit()
	: CPARENT(CFormUnit::IDD)
{
	//{{AFX_DATA_INIT(CFormUnit)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	Init();
}

void CFormUnit::OnInitialUpdate() 
{
	CPARENT::OnInitialUpdate();
	

	csPath=GetDocument()->GetPathName();
	if(!Load())
	{
		CloseForm();
		return;
	}
	if(!CreateForm())
	{
		CloseForm();
		return;
	}

	((CFormFrame*)GetParentFrame())->pForm=this;
	GetDocument()->SetTitle(csFormTitle);
	int nRes=Run();
	if(!nRes)
	{
		CloseForm();
		return;
	}
	
	//if(!m_hIcon)
	m_hIcon=AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	if(GetParent())
    SetClassLong(GetParent()->m_hWnd,GCL_HICON,
        (LONG)m_hIcon);

    SetClassLong(m_hWnd,GCL_HICON,
        (LONG)m_hIcon);

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_FORM1C_TMPL);

	if(pSetFocus)
		pSetFocus->SetFocus();
}

#undef CPARENT
