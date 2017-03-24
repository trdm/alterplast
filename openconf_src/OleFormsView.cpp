// OleFormsView.cpp : implementation of the COleFormsView class
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "OleFormsView.h"
#include "formscript.h"
#include "activedoc.h"

/////////////////////////////////////////////////////////////////////////////
// COleFormsView

IMPLEMENT_DYNCREATE(COleFormsView, CView)

BEGIN_MESSAGE_MAP(COleFormsView, CView)
	//{{AFX_MSG_MAP(COleFormsView)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	ON_WM_DESTROY()
	ON_WM_SETFOCUS()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COleFormsView construction/destruction

COleFormsView::COleFormsView()
{
	m_pSelection = NULL;
	// TODO: add construction code here

}

COleFormsView::~COleFormsView()
{
}

BOOL COleFormsView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// COleFormsView drawing

void COleFormsView::OnDraw(CDC* pDC)
{
}

void COleFormsView::OnInitialUpdate()
{
	m_pSelection = NULL;    // initialize selection
	COleDocument* pDoc=(COleDocument*)GetDocument();
	POSITION pos = pDoc->GetStartPosition();
	if(pos)
	{
		m_pSelection = (COleDocObjectItem*)pDoc->GetNextClientItem(pos);
		if(m_pSelection)
			m_pSelection->DoVerb(OLEIVERB_SHOW, this);
	}
}

void COleFormsView::OnDestroy()
{
	// Deactivate the item on destruction; this is important
	// when a splitter view is being used.
	CView::OnDestroy();
	COleClientItem* pActiveItem = ((COleDocument*)GetDocument())->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL && pActiveItem->GetActiveView() == this)
	{
		pActiveItem->Deactivate();
		ASSERT(GetDocument()->GetInPlaceActiveItem(this) == NULL);
	}
}


/////////////////////////////////////////////////////////////////////////////
// OLE Client support and commands

BOOL COleFormsView::IsSelected(const CObject* pDocItem) const
{
	// The implementation below is adequate if your selection consists of
	//  only COleFormsCntrItem objects.  To handle different selection
	//  mechanisms, the implementation here should be replaced.

	// TODO: implement this function that tests for a selected OLE client item

	return pDocItem == m_pSelection;
}

// The following command handler provides the standard keyboard
//  user interface to cancel an in-place editing session.  Here,
//  the container (not the server) causes the deactivation.

// Special handling of OnSetFocus and OnSize are required for a container
//  when an object is being edited in-place.
void COleFormsView::OnSetFocus(CWnd* pOldWnd)
{
	COleClientItem* pActiveItem = ((COleDocument*)GetDocument())->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL &&
		pActiveItem->GetItemState() == COleClientItem::activeUIState)
	{
		// need to set focus to this item if it is in the same view
		CWnd* pWnd = pActiveItem->GetInPlaceWindow();
		if (pWnd != NULL)
		{
			pWnd->SetFocus();   // don't call the base class
			return;
		}
	}

	CView::OnSetFocus(pOldWnd);
}

void COleFormsView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);
	COleClientItem* pActiveItem = ((COleDocument*)GetDocument())->GetInPlaceActiveItem(this);
	if (pActiveItem != NULL)
	{
		CRect rc;
		GetClientRect(rc);
		pActiveItem->SetItemRects(rc);
	}
}

/////////////////////////////////////////////////////////////////////////////
// COleFormsView message handlers

void COleFormsView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}
