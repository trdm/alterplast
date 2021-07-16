//Авторские права - VTOOLS.RU (info@vtools.ru)
// ChildFrm.cpp : implementation of the CChildFrame class
//

#include "stdafx.h"
#include "Enterprise.h"

#include "ChildFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "Resource.h"
 
/////////////////////////////////////////////////////////////////////////////
// CChildFrame
 
IMPLEMENT_DYNCREATE(CChildFrame, CMDIChildWnd)
IMPLEMENT_DYNCREATE(CMyCodeView, CFormEditor)
IMPLEMENT_DYNCREATE(CMyDescriptionView, PARENTFORDESCRIPTION)

BEGIN_MESSAGE_MAP(CChildFrame, CMDIChildWnd)
	//{{AFX_MSG_MAP(CChildFrame)
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_VIEW_DESCRIPTION, OnViewDescription)
	ON_COMMAND(ID_VIEW_FORM, OnViewForm)
	ON_COMMAND(ID_VIEW_MODULE, OnViewModule)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChildFrame construction/destruction

CChildFrame::CChildFrame()
{
	// TODO: add member initialization code here
	
}

CChildFrame::~CChildFrame()
{
}

BOOL CChildFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

//    cs.style &= ~(WS_THICKFRAME);
//    cs.style &= ~(WS_MAXIMIZEBOX);
//    cs.style |= WS_BORDER;


	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CChildFrame diagnostics

#ifdef _DEBUG
void CChildFrame::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CChildFrame::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CChildFrame message handlers

BOOL CChildFrame::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	return CMDIChildWnd::OnNotify(wParam, lParam, pResult);
}



int CChildFrame::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CChildFrame::OnCreateClient(LPCREATESTRUCT lpcs, CCreateContext* pContext) 
{
	UNREFERENCED_PARAMETER(lpcs);

	if(!m_TabViewContainer.Create(this))
		return FALSE;

	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CFormEditor),//pContext->m_pNewViewClass,
		pContext,_T("Диалог")))
		return FALSE;
	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CMyCodeView),
		pContext,_T("Модуль")))
		return FALSE;
	if(!m_TabViewContainer.AddPage(RUNTIME_CLASS(CMyDescriptionView),
		pContext,_T("Описание")))
		return FALSE;

	m_TabViewContainer.SetActivePageIndex(0);
	
	return TRUE;
//	return CMDIChildWnd::OnCreateClient(lpcs, pContext);
}

void CChildFrame::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	// TODO: Add your message handler code here
	
}
void CChildFrame::SetActivePage(CRuntimeClass *pSet)
{
	for(int i=0;i<m_TabViewContainer.GetPageCount();i++)
	{
		CWnd* pPage=m_TabViewContainer.GetPage(i);
		CRuntimeClass *pClass=pPage->GetRuntimeClass();
		if(pClass==pSet)
		{
			m_TabViewContainer.SetActivePage(pPage);
		}
	}
}

void CChildFrame::OnViewForm() 
{
	SetActivePage(RUNTIME_CLASS(CFormEditor));
}

void CChildFrame::OnViewModule() 
{
	SetActivePage(RUNTIME_CLASS(CMyCodeView));
}
void CChildFrame::OnViewDescription() 
{
	SetActivePage(RUNTIME_CLASS(CMyDescriptionView));
}



//CMyDescriptionView
void CMyDescriptionView::OnInitialUpdate() 
{
	COXTabViewPage<PARENTFORDESCRIPTION>::OnInitialUpdate();
	
	//CMetaObject *m_MetaObj=GetMeta(GetDocument());
	//CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->GetObj();
	CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->GetObj();
	if(m_MetaObj) 
	{
		//AfxMessageBox(m_MetaObj->csComment);
		SetWindowText(m_MetaObj->csComment);
		GetDocument()->SetModifiedFlag(0);
	}
};
