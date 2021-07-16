// HTMLHelpViewDoc.cpp : implementation file
//

#include "stdafx.h"
#include "HTMLHelpViewDoc.h"
#include "HTMLHelpView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpViewDoc

IMPLEMENT_DYNCREATE(CHTMLHelpViewDoc, CDocument)

CHTMLHelpViewDoc::CHTMLHelpViewDoc()
{
}

BOOL CHTMLHelpViewDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;
	return TRUE;
}

CHTMLHelpViewDoc::~CHTMLHelpViewDoc()
{
}


BEGIN_MESSAGE_MAP(CHTMLHelpViewDoc, CDocument)
	//{{AFX_MSG_MAP(CHTMLHelpViewDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpViewDoc diagnostics

#ifdef _DEBUG
void CHTMLHelpViewDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CHTMLHelpViewDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpViewDoc serialization

void CHTMLHelpViewDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: add storing code here
	}
	else
	{
		// TODO: add loading code here
	}
}

/////////////////////////////////////////////////////////////////////////////
// CHTMLHelpViewDoc commands

void CHTMLHelpViewDoc::OnCloseDocument() 
{
	//destroy the help window inside manually...	
	//then just duplicate the MFC code......
	BOOL bAutoDelete = m_bAutoDelete;
	m_bAutoDelete = FALSE;  // don't destroy document while closing views
	while (!m_viewList.IsEmpty())
	{
		// get frame attached to the view
		CView* pView = (CView*)m_viewList.GetHead();
		ASSERT_VALID(pView);
		CFrameWnd* pFrame = pView->GetParentFrame();
		ASSERT_VALID(pFrame);

		// and close it
		((CHTMLHelpView *)pView)->DestroyHelpWindow();
		PreCloseFrame(pFrame);
		pFrame->DestroyWindow();
			// will destroy the view as well
	}
	m_bAutoDelete = bAutoDelete;

	// clean up contents of document before destroying the document itself
	DeleteContents();

	// delete the document if necessary
	if (m_bAutoDelete)
		delete this;
}
