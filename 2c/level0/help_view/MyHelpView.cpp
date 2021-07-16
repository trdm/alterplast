// MyHelpView.cpp : implementation of the CMyHelpView class
//

#include "stdafx.h"
//#include "TestForMyHelpDoc.h"

#include "MyHelpDoc.h"
#include "CntrItem.h"
#include "MyHelpView.h"
#include "../MetadataTree.h"
#include "../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



DWORD __stdcall MEditStreamInCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString *psBuffer = (CString *)dwCookie;

	if (cb > psBuffer->GetLength()) cb = psBuffer->GetLength();

	for (int i=0;i<cb;i++) {
		*(pbBuff+i) = psBuffer->GetAt(i);
	}

	*pcb = cb;

	*psBuffer = psBuffer->Mid(cb);

	return 0;
}

DWORD __stdcall MEditStreamOutCallback(DWORD dwCookie, LPBYTE pbBuff, LONG cb, LONG *pcb)
{
	CString sThisWrite;
	sThisWrite.GetBufferSetLength(cb);

	CString *psBuffer = (CString *)dwCookie;
	
	for (int i=0;i<cb;i++) {
		sThisWrite.SetAt(i,*(pbBuff+i));
	}

	*psBuffer += sThisWrite;

	*pcb = sThisWrite.GetLength();
	sThisWrite.ReleaseBuffer();
	return 0;
}

/////////////////////////////////////////////////////////////////////////////
// CMyHelpView

IMPLEMENT_DYNCREATE(CMyHelpView, CRichEditView)

BEGIN_MESSAGE_MAP(CMyHelpView, CRichEditView)
	//{{AFX_MSG_MAP(CMyHelpView)
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
    ON_WM_CHAR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)

	ON_COMMAND(ID_FORMAT_STYLE_BOLD, OnCharBold)
	ON_COMMAND(ID_FORMAT_STYLE_ITALIC, OnCharItalic)
	ON_COMMAND(ID_FORMAT_STYLE_UNDERLINE, OnCharUnderline)

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyHelpView construction/destruction

CMyHelpView::CMyHelpView()
{
	// TODO: add construction code here

}

CMyHelpView::~CMyHelpView()
{
}

BOOL CMyHelpView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CMyHelpView::OnInitialUpdate()
{
//	CRichEditView::OnInitialUpdate();
	m_bSyncCharFormat = m_bSyncParaFormat = TRUE;


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));





	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	CMetadataTree *m_MetaTree=GetDocument()->GetTree();
	CString csObjTypeName=GetDocument()->GetObjName();
	if(m_MetaTree)
	{
		CString csTitleName;
		csTitleName=m_MetaTree->GetPath(m_MetaObj->nTempID);
		csTitleName=csTitleName.Mid(csTitleName.Find("\\")+1);
		csTitleName.Replace("\\"," : ");
		GetDocument()->SetTitle(csTitleName);
		
	}
	if(m_MetaObj)
	{
		SetText(m_MetaObj->csFile);
		GetDocument()->SetModifiedFlag(0);
	}

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDS_RTF_TYPE);
}

/////////////////////////////////////////////////////////////////////////////
// CMyHelpView printing

BOOL CMyHelpView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CMyHelpView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
   CRichEditView::OnDestroy();
   COleClientItem* pActiveItem = GetDocument()->GetInPlaceActiveItem(this);
   if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
   {
      pActiveItem->Deactivate();
      ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
   }

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );

}


/////////////////////////////////////////////////////////////////////////////
// CMyHelpView diagnostics

#ifdef _DEBUG
void CMyHelpView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CMyHelpView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CMyHelpDoc* CMyHelpView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMyHelpDoc)));
	return (CMyHelpDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMyHelpView message handlers
CString GetWindowText();

int CMyHelpView::GetText(CString &sReadText)
{
	EDITSTREAM es;

	es.dwCookie = (DWORD)&sReadText; // Pass a pointer to the CString to the callback function 
	es.pfnCallback = MEditStreamOutCallback; // Specify the pointer to the callback function.
											 // Function defined in RichEg.cpp

	GetRichEditCtrl().StreamOut(SF_RTF,es); // Perform the streaming
	return 1;
}

void CMyHelpView::SetText(CString sWriteText)
{
	//sWriteText="{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\froman\\fprq2\\fcharset0 Times New Roman;}{\\f1\\fswiss\\fprq2\\fcharset0 System;}}{\\colortbl ;\\red255\\green0\\blue0;\\red51\\green153\\blue102;\\red0\\green0\\blue255;}\\viewkind4\\uc1\\pard\\cf1\\i\\f0\\fs24 Inputted\\cf0\\i0  \\cf2\\b rich\\cf0\\b0  \\cf3 text\\cf0 !\\b\\f1\\fs20 \\par }";

	EDITSTREAM es;

	es.dwCookie = (DWORD)&sWriteText; // Pass a pointer to the CString to the callback function
	es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function
											// Function defined in RichEg.cpp

	GetRichEditCtrl().StreamIn(SF_RTF,es); // Perform the streaming
}


void CMyHelpView::OnPaint() 
{
//	CPaintDC dc(this); // device context for painting

	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	if(m_MetaObj)
	if(m_MetaObj->bCanSave)
	if(GetDocument()->IsModified())
		GetDocument()->SetModifiedFlag(1);
	
	CRichEditView::OnPaint();
}

void CMyHelpView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
/*	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	if(m_MetaObj)
	if(!m_MetaObj->bCanSave)
		return;
*/

	CRichEditView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMyHelpView::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	if(m_MetaObj)
	if(!m_MetaObj->bCanSave)
		return;

	CRichEditView::OnChar(nChar, nRepCnt, nFlags);
}
