// FormCnt.cpp : implementation file
#include "stdafx.h"
#include "configsvcimpl.h"
#include "FormCnt.h"
#include "Formdoc.h"
#include "Formscript.h"
#include "OCView.h"
#include "FormHolder.h"
#include "oleclientiemhook.h"

IMPLEMENT_DYNCREATE(CFormCnt, CView)

CFormCnt::CFormCnt()
{
	m_pActiveObject=NULL;
	//{{AFX_DATA_INIT(CFormCnt)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CFormCnt::~CFormCnt()
{
}

BEGIN_MESSAGE_MAP(CFormCnt, CView)
	//{{AFX_MSG_MAP(CFormCnt)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_SETFOCUS()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormCnt message handlers

void CFormCnt::CreateForm()
{
}

int CFormCnt::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
/*
	CFormScript* pDoc=(CFormScript*)m_pDocument;
	if(!pDoc->m_pForm->LoadFile(pDoc->GetPathName(),this))
		return -1;
	pDoc->m_pForm->m_pForm->put_DesignMode(fmModeOn);
*/
	return 0;
}


void CFormCnt::OnDestroy() 
{

	CView::OnDestroy();
}


void CFormCnt::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	///*
	CRect rc;
	GetClientRect(rc);
	CFormScript* pDoc=(CFormScript*)m_pDocument;
	COleClientItem* pItem=(COleClientItem*)pDoc->GetInPlaceActiveItem(this);
	if(pItem)
	{
		IOleDocumentViewPtr pObj=pItem->m_lpObject;
		if(pObj!=NULL)
		{
			pObj->SetRect(rc);
		}
	}
	//*/
}

void CFormCnt::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
}

void CFormCnt::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	CFormScript* pDoc=(CFormScript*)m_pDocument;
	/*
	pDoc->m_pForm->LoadFile(pDoc->GetPathName(),this);
	//pDoc->m_pForm->m_pForm->put_DesignMode(fmModeOn);
	//pDoc->m_pForm->m_pForm->put_DesignMode(fmModeOff);
	//pDoc->m_pForm->m_pForm->put_DesignMode(fmModeOn);
	*/
	
	///*
	POSITION pos=pDoc->GetStartPosition();
	if(pos)
	{
		COleClientItem* pItem=(COleClientItem*)pDoc->GetNextItem(pos);
		pItem->DoVerb(OLEIVERB_SHOW,this);
	}
	//*/
}

int CFormCnt::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
