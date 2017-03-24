// OCView.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "OCView.h"
#include "scriptmaneger.h"
#include "treeiteminfo.h"
#include "itemscripts.h"
#include "pluginsitem.h"
#include "pluginitem.h"
#include "resource.h"
#include "activedoc.h"

/////////////////////////////////////////////////////////////////////////////
// COCView
COCView* COCView::m_pView=NULL;
CImageList COCView::m_ImageList;

IMPLEMENT_DYNCREATE(COCView, CView)

COCView::COCView()
{
	m_pView=this;
	m_isMove=false;
	m_movePoint.x=-1;
	if(!m_ImageList.GetImageCount())
	{
		OSVERSIONINFO os;
		os.dwOSVersionInfoSize=sizeof(os);
		GetVersionEx(&os);
		if(os.dwMajorVersion>4)
		{
			SHFILEINFO fi;
			DWORD ret=SHGetFileInfo("c:\\",0,&fi,sizeof(fi),SHGFI_SYSICONINDEX|SHGFI_SMALLICON|SHGFI_USEFILEATTRIBUTES);
			HIMAGELIST imgList=ImageList_Duplicate((HIMAGELIST)ret);
			ImageList_RemoveAll(imgList);
			m_ImageList.Attach(imgList);
		}
		else
			m_ImageList.Create(16,16,ILC_MASK|ILC_COLOR32,0,1);
		static const DWORD iconIDs[]={IDI_SCRIPTS,IDI_SCRIPT,IDI_MACROS,IDI_PLUGINS};
		for(int i=0;i<sizeof(iconIDs)/sizeof(iconIDs[0]);i++)
		{
			HICON hIcon=LoadIcon(g_hInst,(LPCTSTR)iconIDs[i]);
			m_ImageList.Add(hIcon);
			DestroyIcon(hIcon);
		}
		char buf[MAX_PATH];
		GetModuleFileName(g_hInst,buf,MAX_PATH);
		SHFILEINFO  shfi;
		if(SHGetFileInfo(buf,0,&shfi,sizeof (SHFILEINFO),SHGFI_ICON|SHGFI_SMALLICON))
		{
			m_ImageList.Add(shfi.hIcon);
			DestroyIcon(shfi.hIcon);
		}
	}
	m_focus=NULL;
	m_pActiveObject=NULL;
	m_pLastRightInfo=NULL;
}

COCView::~COCView()
{
	m_pView=NULL;
	m_ImageList.DeleteImageList();
}

BEGIN_MESSAGE_MAP(COCView, CView)
	//{{AFX_MSG_MAP(COCView)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SETFOCUS()
	//}}AFX_MSG_MAP
	ON_NOTIFY(NM_DBLCLK,100,OnDblclkTree)
	ON_NOTIFY(NM_RETURN,100,OnReturnTree)
	ON_NOTIFY(TVN_SELCHANGED,100,OnSelChanged)
	ON_NOTIFY_RANGE(NM_SETFOCUS,100,101,OnChildSetFocus)
	ON_NOTIFY_RANGE(NM_KILLFOCUS,100,101,OnChildKillFocus)
	//ON_EN_SETFOCUS(101, OnSetfocusRight)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// COCView message handlers
BOOL COCView::PreCreateWindow(CREATESTRUCT& cs) 
{
	HINSTANCE hInst=AfxFindResourceHandle(
				MAKEINTRESOURCE(AFX_IDC_HSPLITBAR), RT_GROUP_CURSOR);
	cs.lpszClass=AfxRegisterWndClass(CS_HREDRAW|CS_VREDRAW,::LoadCursor(hInst, MAKEINTRESOURCE(AFX_IDC_HSPLITBAR)),
		(HBRUSH)GetStockObject(LTGRAY_BRUSH));
	cs.style&=~WS_CLIPCHILDREN;
	return TRUE;
}

int COCView::OnCreate(LPCREATESTRUCT lpc) 
{
	if (CView::OnCreate(lpc) == -1)
		return -1;
	m_tree.CreateEx(WS_EX_CLIENTEDGE,WC_TREEVIEW,NULL,
		WS_TABSTOP|WS_CHILD|WS_VISIBLE|TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS|TVS_SHOWSELALWAYS,
		CRect(0,0,0,0),this,100);
	m_tree.SetImageList(&m_ImageList,TVSIL_NORMAL);
	m_list.CreateEx(WS_EX_CLIENTEDGE,WC_LISTVIEW,NULL,
		WS_TABSTOP|WS_VISIBLE|WS_CHILD|LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL,
		0,0,0,0,m_hWnd,(HMENU)101);
	m_list.SendMessage(LVM_SETEXTENDEDLISTVIEWSTYLE,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES,
		LVS_EX_FULLROWSELECT|LVS_EX_GRIDLINES);
	m_right=m_list;
	m_focus=m_tree;
	FillScripts();
	return 0;
}

void COCView::OnSize(UINT nType, int cx, int cy) 
{
	CRect rc;
	GetClientRect(rc);
	int w=m_movePoint.x;
	if(w<0)
		w=rc.right/3;
	::MoveWindow(m_tree,-2,-2,w,rc.bottom+4,TRUE);
	if(m_pActiveObject)
	{
		if(m_pActiveObject->m_pInPlaceObject)
		{
			CRect rr(w+1,-2,rc.right+2,rc.bottom+2);
			m_pActiveObject->m_rect=rr;
			m_pActiveObject->m_pInPlaceObject->SetObjectRects(rr,rr);
		}
	}
	else
		::MoveWindow(m_right,w+1,-2,rc.right-w,rc.bottom+4,TRUE);
}

void COCView::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(!m_isMove)
		return;
	InvertSplit();
	m_movePoint.x=point.x;
	InvertSplit();
}

void COCView::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_isMove=true;
	SetCapture();
	CRect rc;
	GetClientRect(rc);
	m_movePoint.x=point.x;
	m_movePoint.y=rc.bottom;
	ClientToScreen(rc);
	rc.DeflateRect(10,0,10,0);
	ClipCursor(rc);
	InvertSplit();
}

void COCView::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_isMove=false;
	ReleaseCapture();
	InvertSplit();
	ClipCursor(NULL);
	OnSize(0,0,0);
}

void COCView::InvertSplit()
{
	CDC* pDC = GetDC();
	CBrush* pBrush = CDC::GetHalftoneBrush();
	HBRUSH hOldBrush = NULL;
	if (pBrush != NULL)
		hOldBrush = (HBRUSH)SelectObject(pDC->m_hDC, pBrush->m_hObject);
	pDC->PatBlt(m_movePoint.x,0,3,m_movePoint.y, PATINVERT);
	if (hOldBrush != NULL)
		SelectObject(pDC->m_hDC, hOldBrush);
	ReleaseDC(pDC);
}

void COCView::FillScripts()
{
	CItemScripts* pScripts=new CItemScripts;
	pScripts->InsertInTree(TVI_ROOT);
	CPluginsItem* pPlugins=new CPluginsItem;
	pPlugins->InsertInTree(TVI_ROOT);
}

void COCView::OnDblclkTree(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult=1;
	keybd_event(VK_RETURN,MapVirtualKey(0,VK_RETURN),0,0);
    keybd_event(VK_RETURN,MapVirtualKey(0,VK_RETURN),KEYEVENTF_KEYUP,0);
}

void COCView::OnSelChanged(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult=1;
	CTreeItemInfo* pItem=CTreeItemInfo::GetCurItem();
	pItem->OnSelect();
}

void COCView::OnSetFocus(CWnd* pOldWnd) 
{
	if(m_focus)
		::SetFocus(m_focus);
	else
		::SetFocus(m_right);
}

void COCView::OnChildSetFocus(UINT id,NMHDR* pNMHDR, LRESULT* pResult)
{
	m_focus=pNMHDR->hwndFrom;
	*pResult=1;
}

void COCView::OnChildKillFocus(UINT id,NMHDR* pNMHDR, LRESULT* pResult)
{
	HWND h=::GetFocus();
	if(::IsChild(m_hWnd,h))
		m_focus=h;
	*pResult=1;
}

void COCView::OnReturnTree(NMHDR* pNMHDR, LRESULT* pResult)
{
	*pResult=1;
	CTreeItemInfo* pItem=CTreeItemInfo::GetCurItem();
	pItem->OnAction();
}

void COCView::CreatePluginControl(CPluginItem* pItem)
{
	if(pItem==m_pLastRightInfo)
		return;
	DestroyRightPane();
	PluginInfo* pInfo=pItem->m_pInfo;
	IOleControlPtr pCtrl=pInfo->pPlugin;
	if(pInfo->pPlugin==NULL || pCtrl==NULL || (pInfo->pPlugin!=NULL && pInfo->OpenIn))
	{
		CStringArray titles;
		CStringArray data;
		titles.Add("—войство");
		titles.Add("«начение");
		char fname[MAX_PATH];
		GetModuleFileName((HINSTANCE)pInfo->handle,fname,MAX_PATH);
		data.Add("ѕуть");
		data.Add(fname);
		DWORD z,size;
		size=GetFileVersionInfoSize(fname,&z);
		if(size)
		{
			static const char* EngNames[]={
				"FileVersion",
				"ProductVersion",
				"CompanyName",
				"ProductName",
				"FileDescription",
				"Comments",
				"LegalCopyright",
				"LegalTrademarks",
				"InternalName",
				"OriginalFilename",
				"PrivateBuild",
				"SpecialBuild"
			};
			char* pVersion=new char[size];
			GetFileVersionInfo(fname,0,size,pVersion);
			DWORD* translate;
			UINT len;
			VerQueryValue(pVersion,"\\VarFileInfo\\Translation",(void**)&translate,&len);
			CString blockName;
			blockName.Format("\\StringFileInfo\\%04x%04x\\",LOWORD(translate[0]),HIWORD(translate[0]));
			LPCTSTR valuebuf;
			for(int i=0;i<sizeof(EngNames)/sizeof(EngNames[0]);i++)
			{
				CString fdName=blockName+EngNames[i];
				VerQueryValue(pVersion,(char*)(LPCTSTR)(fdName),(void**)&valuebuf,&len);
				if(len)
				{
					data.Add(EngNames[i]);
					data.Add(valuebuf);
				}
			}
			char lname[MAX_PATH];
			VerLanguageName(translate[0],lname,MAX_PATH);
			data.Add("язык");
			data.Add(lname);
			delete pVersion;
		}
		ShowTab(pItem,titles,data);
		return;
	}
	COleControlSiteWrap* pSite = NULL;
	InitControlContainer();
	TRY
	{
		pSite = (COleControlSiteWrap*)afxOccManager->CreateSite(m_pCtrlCont);
	}
	END_TRY
	CFakeWnd* pWnd=(CFakeWnd*)new CWnd;
	pSite->m_pWndCtrl=pWnd;
	pWnd->SetSite(pSite);
	pInfo->pPlugin->QueryInterface(IID_IOleObject,(void**)&pSite->m_pObject);
	pSite->m_pObject->SetClientSite(&pSite->m_xOleClientSite);
	pSite->m_pObject->QueryInterface(IID_IOleInPlaceObject,(LPVOID*)&pSite->m_pInPlaceObject);
	pSite->DoVerb(OLEIVERB_INPLACEACTIVATE);
	pSite->AttachWnd();
	m_pCtrlCont->m_siteMap.SetAt(pSite->m_hWnd, pSite);
	m_right=pWnd->m_hWnd;
	m_pActiveObject=pSite;
	pWnd->ModifyStyle(0,WS_TABSTOP);
	pWnd->ModifyStyleEx(0,WS_EX_CLIENTEDGE);
	OnSize(0,0,0);
	m_pLastRightInfo=pItem;
}

void COCView::ShowTab(CTreeItemInfo* pInfo,CStringArray &titles, CStringArray &data)
{
	if(pInfo==m_pLastRightInfo)
		return;
	DestroyRightPane();
	m_pLastRightInfo=pInfo;
	HDITEM hdi;
	hdi.mask=HDI_TEXT;
	int Cols=titles.GetSize();
	CHeaderCtrl* pHeader=m_list.GetHeaderCtrl();
	int cols=pHeader->GetItemCount();
	for(int i=0;i<cols && i<Cols;i++)
	{
		hdi.pszText=(LPTSTR)(LPCTSTR)titles[i];
		pHeader->SetItem(i,&hdi);
	}
	if(cols>Cols)
	{
		for(;i<cols;i++)
			m_list.DeleteColumn(i);
	}
	else
	{
		for(;i<Cols;i++)
			m_list.InsertColumn(i,titles[i],LVCFMT_LEFT,200,i);
	}
	m_list.DeleteAllItems();
	for(int row=0;row<data.GetSize()/Cols;row++)
	{
		CString name=data[row*Cols];
		LVITEM lvi;
		lvi.mask=LVIF_TEXT;
		lvi.iItem=row;
		lvi.iSubItem=0;
		lvi.pszText=(LPTSTR)(LPCTSTR)name;
		m_list.InsertItem(&lvi);
		for(int i=1;i<Cols;i++)
		{
			name=data[row*Cols+i];
			lvi.mask=LVIF_TEXT;
			lvi.iItem=row;
			lvi.iSubItem=i;
			lvi.pszText=(LPTSTR)(LPCTSTR)name;
			m_list.SetItem(&lvi);
		}
	}
	m_right=m_list;
	OnSize(0,0,0);
	m_list.ShowWindow(SW_SHOW);
}

void COCView::DestroyRightPane()
{
	if(m_pActiveObject)
	{
		m_pCtrlCont->m_siteMap.RemoveKey((void*&)m_pActiveObject->m_hWnd);
		delete m_pActiveObject;
		m_pActiveObject=NULL;
	}
	else
	{
		if(::IsWindow(m_right))
			::ShowWindow(m_right,SW_HIDE);
	}
	m_right=NULL;
	m_pLastRightInfo=NULL;
}

BOOL COCView::PreTranslateMessage(MSG* pMsg) 
{
	if(CView::PreTranslateMessage(pMsg))
		return TRUE;
	if(GetParentFrame()->PreTranslateMessage(pMsg))
		return TRUE;
	if(AfxGetMainWnd()->PreTranslateMessage(pMsg))
		return TRUE;
	if(pMsg->message==WM_KEYDOWN && pMsg->wParam==VK_RETURN && ::GetFocus()==m_tree)
		return FALSE;
	return IsDialogMessage(pMsg);
	//return FALSE;
}
