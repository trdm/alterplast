// IMDWindow.cpp: implementation of the CIMDWindow class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "configsvcimpl.h"
#include "IMDWindow.h"
#include "metadata.h"

ITypeInfo* CIMDWindow::m_pTypeInfo=NULL;
CIMDWindow* CIMDWindow::m_pMDWindow;

static WNDPROC oldMDViewProc	= NULL;
static CTreeView* pMDView		= NULL;
static HTREEITEM hCaretItem		= NULL;

LRESULT CALLBACK MDViewWindowProc(
  HWND hwnd,      // handle to window
  UINT uMsg,      // message identifier
  WPARAM wParam,  // first message parameter
  LPARAM lParam   // second message parameter
)
{
	if(uMsg==TVM_GETNEXTITEM && wParam==TVGN_CARET && hCaretItem)
	{
		return (LRESULT)hCaretItem;
	}
	else if(uMsg==WM_DESTROY)
	{
		pMDView=NULL;
		CFakeWnd::m_hTabWnd=NULL;
	}
	return CallWindowProcA(oldMDViewProc,hwnd,uMsg,wParam,lParam);
}

static void OnOpenMDWindow(CWnd*)
{
	for(POSITION pos=CIMetaData::m_pOneMetaData->m_pTemplateOfMetaData->GetFirstDocPosition();pos;)
	{
		CDocument* pDoc=CIMetaData::m_pOneMetaData->m_pTemplateOfMetaData->GetNextDoc(pos);
		if(pDoc)
		{
			pos=pDoc->GetFirstViewPosition();
			pMDView=(CTreeView*)pDoc->GetNextView(pos);
			if(pMDView)
			{
				oldMDViewProc=(WNDPROC)GetWindowLong(pMDView->m_hWnd,GWL_WNDPROC);
				SetWindowLong(pMDView->m_hWnd,GWL_WNDPROC,(DWORD)MDViewWindowProc);
			}
			break;
		}
	}
}

void CIMDWindow::Init()
{
	CConfigSvcImpl::m_pTheService->AddConfigEventHandler(CFG_ON_CREATE_CFG_WND,OnOpenMDWindow);
	m_pMDWindow=new CIMDWindow;
}

CIMDWindow::CIMDWindow()
{
}

CIMDWindow::~CIMDWindow()
{
	AfxMessageBox("Dest");
}

STDMETHODIMP CIMDWindow::Activate()
{
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	((CMDIFrameWnd*)pMainApp->m_pMainWnd)->MDIActivate(pMDView->GetParentFrame());
	return S_OK;
}

STDMETHODIMP CIMDWindow::get_TabCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	SECTabWndBase* pTab=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
	if(!pTab)
		return E_FAIL;
	*pVal=pTab->SECTabWndBase::GetTabCount();
	return S_OK;
}

STDMETHODIMP CIMDWindow::get_TabName(long idx, BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	SECTabWndBase* pTab=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
	if(!pTab)
		return E_FAIL;
	int tCnt=pTab->SECTabWndBase::GetTabCount();
	if(((DWORD)idx)>=tCnt)
		return SetError(E_INVALIDARG,"Неправильный номер закладки: %i",idx);
	LPCTSTR name;
	CWnd* pWnd;
	int p;
	void* pp;
	pTab->GetTabInfo(idx,name,p,pWnd,pp);
	CString strName=name;
	*pVal=strName.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIMDWindow::get_ActiveTab(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	SECTabWndBase* pTab=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
	if(!pTab)
		return E_FAIL;
	int act;
	pTab->GetActiveTab(act);
	*pVal=act;
	return S_OK;
}

STDMETHODIMP CIMDWindow::put_ActiveTab(long newVal)
{
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	SECTabWndBase* pTab=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
	if(!pTab)
		return E_FAIL;
	int tCnt=pTab->SECTabWndBase::GetTabCount();
	if(((DWORD)newVal)>=tCnt)
		return SetError(E_INVALIDARG,"Неправильный номер закладки: %i",newVal);
	pTab->SECTabWndBase::ActivateTab(newVal);
	return S_OK;
}

STDMETHODIMP CIMDWindow::DoAction(BSTR PathInTree, mdActions Action)
{
	if(!pMDView)
		return SetError(E_FAIL,"Окно конфигурации не открыто");
	CString path=PathInTree;
	CStringArray arr;
	SplitStr2Array(path,arr,'\\');
	CTreeCtrl& tree=pMDView->GetTreeCtrl();
	HTREEITEM item=tree.GetNextItem(TVI_ROOT,TVGN_CHILD);
	bool fnd=true;
	int cnt=arr.GetSize();
	CString name;
	for(int i=0;i<cnt && fnd;i++)
	{
		item=tree.GetNextItem(item,TVGN_CHILD);
		fnd=false;
		while(item)
		{
			name=tree.GetItemText(item);
			if(!name.CompareNoCase(arr[i]))
			{
				fnd=true;
				break;
			}
			item=tree.GetNextItem(item,TVGN_NEXT);
		}
	}
	if(!fnd)
		return SetError(E_FAIL,"Объект \"%s\" не найден в дереве метаданных",path);
	USHORT cmd=(USHORT)Action;
	if(cmd<2)
	{
		((CMDIFrameWnd*)pMainApp->m_pMainWnd)->MDIActivate(pMDView->GetParentFrame());
		SECTabWndBase* pWnd=(SECTabWndBase*)CWnd::FromHandlePermanent(CFakeWnd::m_hTabWnd);
		pWnd->SECTabWndBase::ActivateTab(0);

		tree.Select(item,TVGN_CARET);

		if(Action==mdaProps)
		{
			CPropertyBox* pBox=CPropertyBox::GetPropertyBox();
			if(pBox)
				pBox->Show(TRUE);
			else
				pMDView->SendMessage(WM_COMMAND,0x804C);
		}
	}
	else
	{
		hCaretItem=item;
		pMDView->SendMessage(WM_COMMAND,cmd);
		hCaretItem=NULL;
	}
	return S_OK;
}

STDMETHODIMP CIMDWindow::get_hWnd(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(!pMDView)
		*pVal=0;
	else
		*pVal=(long)pMDView->GetParentFrame()->m_hWnd;
	return S_OK;
}

STDMETHODIMP CIMDWindow::GetSelected(BSTR *Path)
{
	if(IsBadWritePtr(Path,sizeof(*Path)))
		return E_POINTER;
	if(!pMDView)
		return SetError(E_FAIL, "Окно конфигурации не открыто");
	CString path;
	CTreeCtrl& tree=pMDView->GetTreeCtrl();
	HTREEITEM hItem=tree.GetNextItem(NULL,TVGN_CARET);
	if(hItem)
	{
		HTREEITEM hFirst=tree.GetNextItem(TVI_ROOT,TVGN_CHILD);
		while(hItem!=hFirst)
		{
			path=tree.GetItemText(hItem)+'\\'+path;
			hItem=tree.GetNextItem(hItem,TVGN_PARENT);
		}
		path=path.Left(path.GetLength()-1);
	}
	else
		path="no sel";
	*Path=path.AllocSysString();
	return S_OK;
}
