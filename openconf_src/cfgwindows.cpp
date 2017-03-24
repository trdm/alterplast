// cfgwindows.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "cfgwindows.h"
#include "icfgwindow.h"

ITypeInfo* CICfgWindows::m_pTypeInfo=NULL;
CICfgWindows* CICfgWindows::m_pCfgWindows;

void CICfgWindows::Init()
{
	m_pCfgWindows=new CICfgWindows;
}

CICfgWindows::CICfgWindows()
{
}

STDMETHODIMP CICfgWindows::get_MainWnd(ICfgWindow **pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	static CICfgWindow* pMainWnd=new CICfgWindow(AfxGetMainWnd()->m_hWnd);
	*pVal=pMainWnd;
	pMainWnd->AddRef();
	return S_OK;
}

STDMETHODIMP CICfgWindows::get_ActiveWnd(ICfgWindow **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)AfxGetMainWnd();
	if(pFrame)
	{
		CMDIChildWnd* pChild=pFrame->MDIGetActive();
		if(pChild)
			*pVal=new CICfgWindow(pChild->m_hWnd);
	}
	return S_OK;
}

STDMETHODIMP CICfgWindows::put_ActiveWnd(ICfgWindow *newVal)
{
	if(!newVal)
		return E_INVALIDARG;
	HWND hWnd;
	if(newVal->get_hWnd((long*)&hWnd)==S_OK)
	{
		CMDIFrameWnd* pFrame=(CMDIFrameWnd*)AfxGetMainWnd();
		if(pFrame)
		{
			pFrame->MDIActivate(CWnd::FromHandle(hWnd));
			::SetFocus(hWnd);
		}
	}
	return S_OK;
}

STDMETHODIMP CICfgWindows::get_FirstWnd(ICfgWindow **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=NULL;
	CMDIFrameWnd* pFrame=(CMDIFrameWnd*)AfxGetMainWnd();
	if(pFrame)
	{
		HWND hFirst=::GetWindow(pFrame->m_hWndMDIClient,GW_CHILD);
		if(hFirst)
			*pVal=new CICfgWindow(hFirst);
	}
	return S_OK;
}

STDMETHODIMP CICfgWindows::get_NextWnd(ICfgWindow *CurrentWnd,ICfgWindow **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!CurrentWnd)
		return E_INVALIDARG;
	HWND hWnd;
	if(CurrentWnd->get_hWnd((long*)&hWnd)==S_OK)
	{
		HWND hNext=GetWindow(hWnd,GW_HWNDNEXT);
		if(hNext)
			*pVal=new CICfgWindow(hNext);
	}
	return S_OK;
}

SECControlBar* FindMyPanel(LPCTSTR Caption)
{
	SECMDIFrameWnd* pFrame=(SECMDIFrameWnd*)pMainApp->m_pMainWnd;
	for(POSITION pos=pFrame->m_listControlBars.GetHeadPosition();pos;)
	{
		SECControlBar* pCB=(SECControlBar*)pFrame->m_listControlBars.GetNext(pos);
		CString txt;
		pCB->GetWindowText(txt);
		if(!txt.CompareNoCase(Caption))
		{
			if(strcmp("CDockBar7",pCB->GetRuntimeClass()->m_lpszClassName))
				return pCB;
		}
	}
	return NULL;
}

void SetPanelVisible(SECControlBar* pBar,bool visible)
{
	if(pBar)
	{
		SECMDIFrameWnd* pFrame=(SECMDIFrameWnd*)pMainApp->m_pMainWnd;
		if(pBar->IsMDIChild())
		{
			CMDIChildWnd* pHolder=(CMDIChildWnd*)pBar->GetParentFrame();
			if(visible)
			{
				pFrame->MDIActivate(pHolder);
				if(pBar->IsWindowVisible())
				{
					CWnd* pChild=pBar->GetWindow(GW_CHILD);
					while(pChild)
					{
						if(pChild->IsKindOf(RUNTIME_CLASS(CView)))
						{
							pChild->SetFocus();
							break;
						}
						pChild=pChild->GetWindow(GW_CHILD);
					}
					return;
				}
			}
			else
			{
				CMDIChildWnd* pChild=pFrame->MDIGetActive();
				if(pChild==pHolder)
					pFrame->MDINext();
				pHolder->SendMessage(WM_CLOSE);
				return;
			}
		}
		pFrame->ShowControlBar(pBar,visible,TRUE);
		if(visible)
		{
			CWnd* pChild=pBar->GetWindow(GW_CHILD);
			while(pChild)
			{
				if(pChild->IsKindOf(RUNTIME_CLASS(CView)))
				{
					pChild->SetFocus();
					break;
				}
				pChild=pChild->GetWindow(GW_CHILD);
			}
		}
		else
		{
			CMDIChildWnd* pChild=pFrame->MDIGetActive();
			if(pChild && pChild->IsWindowVisible())
			{
				pChild->ActivateFrame();
				CView* pView=pChild->GetActiveView();
				if(pView)
					pView->SetFocus();
			}
		}
	}
}

STDMETHODIMP CICfgWindows::get_PanelVisible(BSTR Caption,VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	_bstr_t capt=Caption;
	SECControlBar* pBar=FindMyPanel(capt);
	if(pBar)
		*pVal=(pBar->GetStyle() & WS_VISIBLE)?VARIANT_TRUE:VARIANT_FALSE;
	return S_OK;
}

STDMETHODIMP CICfgWindows::put_PanelVisible(BSTR Caption,VARIANT_BOOL newVal)
{
	_bstr_t capt=Caption;
	SECControlBar* pBar=FindMyPanel(capt);
	SetPanelVisible(pBar,newVal==VARIANT_TRUE);
	return S_OK;
}
