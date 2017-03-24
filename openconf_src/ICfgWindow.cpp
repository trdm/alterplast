// ICfgWindow.cpp: implementation of the CICfgWindow class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ICfgWindow.h"
#include "ICfgDoc.h"
#include "metadataobj.h"

ITypeInfo* CICfgWindow::m_pTypeInfo=NULL;

CICfgWindow::~CICfgWindow(){}

STDMETHODIMP CICfgWindow::get_Document(ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!IsValid())
		return E_FAIL;
	CWnd* pWnd=CWnd::FromHandlePermanent(m_hWnd);
	CDocument* pDoc=NULL;
	if(pWnd)
	{
		if(pWnd->IsKindOf(RUNTIME_CLASS(CView)))
			pDoc=((CView*)pWnd)->GetDocument();
		else if(pWnd->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
			pDoc=((CMDIChildWnd*)pWnd)->GetActiveDocument();
	}
	*pVal=CICfgDoc::CreateDoc(pDoc);
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_hWnd(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!IsValid())
		return E_FAIL;
	*pVal=(long)m_hWnd;
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Caption(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!IsValid())
		return E_FAIL;
	CFrameWnd* pWnd=(CFrameWnd*)CWnd::FromHandlePermanent(m_hWnd);
	CString caption;
	if(pWnd)
	{
		if(!pWnd->IsFrameWnd())
			pWnd=pWnd->GetParentFrame();
		if(pWnd)
			pWnd->GetWindowText(caption);
	}
	*pVal=caption.AllocSysString();
	return S_OK;
}

STDMETHODIMP CICfgWindow::put_Caption(BSTR newVal)
{
	if(!IsValid())
		return E_FAIL;

	CFrameWnd* pWnd=(CFrameWnd*)CWnd::FromHandlePermanent(m_hWnd);
	if(pWnd)
	{
		if(!pWnd->IsFrameWnd())
			pWnd=pWnd->GetParentFrame();
	}
	if(pWnd)
	{
		CString cap=(LPCTSTR)_bstr_t(newVal);
		if(pWnd==AfxGetMainWnd())
		{
			free((void*)pMainApp->m_pszAppName);
			pMainApp->m_pszAppName=_strdup(cap);
			pWnd->SetTitle(cap);
			pWnd->SetWindowText(cap);
		}
		else
		{
			CDocument* pDoc=pWnd->GetActiveDocument();
			if(pDoc)
				pDoc->SetTitle(cap);
			else
				pWnd->SetWindowText(cap);
		}
	}
	return S_OK;
}

STDMETHODIMP CICfgWindow::Close()
{
	if(!IsValid())
		return E_FAIL;
	SendMessage(m_hWnd,WM_CLOSE,0,0);
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Maximized(VARIANT_BOOL *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=IsZoomed(m_hWnd)?-1:0;
	return S_OK;
}

STDMETHODIMP CICfgWindow::put_Maximized(VARIANT_BOOL newVal)
{
	if(!IsValid())
		return E_FAIL;
	if(newVal)
		ShowWindow(m_hWnd,SW_MAXIMIZE);
	else if(IsZoomed(m_hWnd))
		ShowWindow(m_hWnd,SW_RESTORE);
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Minimized(VARIANT_BOOL *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=IsIconic(m_hWnd)?-1:0;
	return S_OK;
}

STDMETHODIMP CICfgWindow::put_Minimized(VARIANT_BOOL newVal)
{
	if(!IsValid())
		return E_FAIL;
	if(newVal)
		ShowWindow(m_hWnd,SW_MINIMIZE);
	else if(IsIconic(m_hWnd))
		ShowWindow(m_hWnd,SW_RESTORE);
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Top(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT pt;
	pt.y=rc.top;
	pt.x=0;
	ScreenToClient(GetParent(m_hWnd),&pt);
	*pVal=pt.y;
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Left(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT pt;
	pt.x=rc.left;
	pt.y=0;
	ScreenToClient(GetParent(m_hWnd),&pt);
	*pVal=pt.x;
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Bottom(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT pt;
	pt.y=rc.bottom;
	pt.x=0;
	ScreenToClient(GetParent(m_hWnd),&pt);
	*pVal=pt.y;
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_Right(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	GetWindowRect(m_hWnd,&rc);
	POINT pt;
	pt.x=rc.right;
	pt.y=0;
	ScreenToClient(GetParent(m_hWnd),&pt);
	*pVal=pt.x;
	return S_OK;
}

STDMETHODIMP CICfgWindow::Move(long left,long top,long right,long bottom)
{
	if(!IsValid())
		return E_FAIL;
	if(IsIconic(m_hWnd) || IsZoomed(m_hWnd))
		ShowWindow(m_hWnd,SW_RESTORE);
	MoveWindow(m_hWnd,left,top,right-left,bottom-top,TRUE);
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_ClientWidth(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	HWND hWnd=m_hWnd;
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)AfxGetMainWnd();
	if(m_hWnd==pFrame->m_hWnd)
		hWnd=pFrame->m_hWndMDIClient;
	GetClientRect(hWnd,&rc);
	*pVal=rc.right-rc.left;
	return S_OK;
}

STDMETHODIMP CICfgWindow::get_ClientHeight(long *pVal)
{
	if(!IsValid())
		return E_FAIL;
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	RECT rc;
	GetClientRect(m_hWnd,&rc);
	HWND hWnd=m_hWnd;
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)AfxGetMainWnd();
	if(m_hWnd==pFrame->m_hWnd)
		hWnd=pFrame->m_hWndMDIClient;
	GetClientRect(hWnd,&rc);
	*pVal=rc.bottom-rc.top;
	return S_OK;
}

