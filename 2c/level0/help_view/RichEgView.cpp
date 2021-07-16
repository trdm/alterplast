// RichEgView.cpp : implementation of the CRichEgView class
//

#include "stdafx.h"

#include "RichEgDoc.h"
#include "CntrItemEg.h"
#include "RichEgView.h"

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
// CRichEgView

IMPLEMENT_DYNCREATE(CRichEgView, CRichEditView)

BEGIN_MESSAGE_MAP(CRichEgView, CRichEditView)
	//{{AFX_MSG_MAP(CRichEgView)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CRichEditView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CRichEditView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CRichEgView construction/destruction

CRichEgView::CRichEgView()
{
	// TODO: add construction code here

}

CRichEgView::~CRichEgView()
{
}

BOOL CRichEgView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CRichEditView::PreCreateWindow(cs);
}

void CRichEgView::OnInitialUpdate()
{
	CRichEditView::OnInitialUpdate();


	// Set the printing margins (720 twips = 1/2 inch).
	SetMargins(CRect(720, 720, 720, 720));
}

/////////////////////////////////////////////////////////////////////////////
// CRichEgView printing

BOOL CRichEgView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}


void CRichEgView::OnDestroy()
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
}


/////////////////////////////////////////////////////////////////////////////
// CRichEgView diagnostics

#ifdef _DEBUG
void CRichEgView::AssertValid() const
{
	CRichEditView::AssertValid();
}

void CRichEgView::Dump(CDumpContext& dc) const
{
	CRichEditView::Dump(dc);
}

CRichEgDoc* CRichEgView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CRichEgDoc)));
	return (CRichEgDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CRichEgView message handlers

CString GetWindowText();

int CRichEgView::GetWindowText(CString &sReadText)
{
	EDITSTREAM es;

	es.dwCookie = (DWORD)&sReadText; // Pass a pointer to the CString to the callback function 
	es.pfnCallback = MEditStreamOutCallback; // Specify the pointer to the callback function.
											 // Function defined in RichEg.cpp

	GetRichEditCtrl().StreamOut(SF_RTF,es); // Perform the streaming
	return 1;
}

void CRichEgView::SetWindowText(CString sWriteText)
{
	//sWriteText="{\\rtf1\\ansi\\ansicpg1252\\deff0\\deflang2057{\\fonttbl{\\f0\froman\\fprq2\\fcharset0 Times New Roman;}{\\f1\\fswiss\\fprq2\\fcharset0 System;}}{\\colortbl ;\\red255\\green0\\blue0;\\red51\\green153\\blue102;\\red0\\green0\\blue255;}\\viewkind4\\uc1\\pard\\cf1\\i\\f0\\fs24 Inputted\\cf0\\i0  \\cf2\\b rich\\cf0\\b0  \\cf3 text\\cf0 !\\b\\f1\\fs20 \\par }";

	EDITSTREAM es;

	es.dwCookie = (DWORD)&sWriteText; // Pass a pointer to the CString to the callback function
	es.pfnCallback = MEditStreamInCallback; // Specify the pointer to the callback function
											// Function defined in RichEg.cpp

	GetRichEditCtrl().StreamIn(SF_RTF,es); // Perform the streaming
}




