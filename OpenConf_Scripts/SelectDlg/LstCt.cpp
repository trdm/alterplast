// LstCt.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "LstCt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CLstCt

CLstCt::CLstCt()
{
	EnableAutomation();
}

CLstCt::~CLstCt()
{
}

void CLstCt::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CListCtrl::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CLstCt, CListCtrl)
	//{{AFX_MSG_MAP(CLstCt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CLstCt, CListCtrl)
	//{{AFX_DISPATCH_MAP(CLstCt)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_ILstCt to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {FDE3872D-1DBB-41FB-8A2D-8DA44FD99F5A}
static const IID IID_ILstCt =
{ 0xfde3872d, 0x1dbb, 0x41fb, { 0x8a, 0x2d, 0x8d, 0xa4, 0x4f, 0xd9, 0x9f, 0x5a } };

BEGIN_INTERFACE_MAP(CLstCt, CListCtrl)
	INTERFACE_PART(CLstCt, IID_ILstCt, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLstCt message handlers
