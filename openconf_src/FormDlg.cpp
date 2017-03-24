// FormDlg.cpp : implementation file
//

#include "stdafx.h"
#include "config.h"
#include "FormDlg.h"

/////////////////////////////////////////////////////////////////////////////
// CFormDlg dialog


CFormDlg::CFormDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CFormDlg::IDD, pParent)
{
	EnableAutomation();

	//{{AFX_DATA_INIT(CFormDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CFormDlg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CDialog::OnFinalRelease();
}

void CFormDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormDlg)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFormDlg, CDialog)
	//{{AFX_MSG_MAP(CFormDlg)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CFormDlg, CDialog)
	//{{AFX_DISPATCH_MAP(CFormDlg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IFormDlg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {930A3085-F138-11D7-8D5E-B340FDE9FB02}
static const IID IID_IFormDlg =
{ 0x930a3085, 0xf138, 0x11d7, { 0x8d, 0x5e, 0xb3, 0x40, 0xfd, 0xe9, 0xfb, 0x2 } };

BEGIN_INTERFACE_MAP(CFormDlg, CDialog)
	INTERFACE_PART(CFormDlg, IID_IFormDlg, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDlg message handlers
