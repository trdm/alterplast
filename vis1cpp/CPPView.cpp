// CPPView.cpp : implementation file

#include "stdafx.h"

#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "Vis1cppPlugin.h"

#include "CPPdoc.h"
#include "InfoWrcSpace.h"
#include "mytextdoc.h"
#include "syntax.h"

#include "CoDefFiles.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCPPView

IMPLEMENT_DYNCREATE(CCPPView, CTreeView)
CCPPView* CCPPView::m_pView=NULL;
CImageList CCPPView::m_ImageList;
HACCEL CCPPView::m_HotKeys;

void CCPPView::LoadTypeIcon()
{
	typedef std::pair<InfoType, int> InfoTypeAndIconResIDPair;
	static InfoTypeAndIconResIDPair arrayOfInfoTypeAndResIconPair_OwnResources[] = 
	{
		InfoTypeAndIconResIDPair(TYPE_WRCSPACE_FOLDER, IDI_WRCCPACE),
		InfoTypeAndIconResIDPair(TYPE_DEF_FILE_FOLDER, IDI_DEFFILE),
		InfoTypeAndIconResIDPair(TYPE_CLASS_FOLDER, IDI_CLASS),
		
		InfoTypeAndIconResIDPair(TYPE_BASE_CLASS_FOLDER, IDI_BASECLASSES),
		InfoTypeAndIconResIDPair(TYPE_DECLARED_PROC, IDI_DECLPROC),
		InfoTypeAndIconResIDPair(TYPE_MODULE_PROC_EXPORT, IDI_OPENMET),
		InfoTypeAndIconResIDPair(TYPE_MODULE_PROC_PRIVATE, IDI_PRIVMET),
		InfoTypeAndIconResIDPair(TYPE_MEMBER_EXPORT, IDI_OPENMEMB),
		InfoTypeAndIconResIDPair(TYPE_MEMBER_PRIVATE, IDI_PRIVMEMB),
	};
	
	static InfoTypeAndIconResIDPair arrayOfInfoTypeAndResIconPair_1CResources[] = 
	{
		//	нижеследующие ресурсы из "1crcrus.dll"
		InfoTypeAndIconResIDPair(TYPE_FROM_ALS_CLASS, 206),
		InfoTypeAndIconResIDPair(TYPE_FROM_MD_CLASS, 213),
		InfoTypeAndIconResIDPair(TYPE_HARD_PROC, 324),
		InfoTypeAndIconResIDPair(TYPE_HARD_MEMBER, 325),
		InfoTypeAndIconResIDPair(TYPE_FILE_FOLDER, 363),
	};

	CImageList& pList = CCPPView::m_ImageList;
	pList.Create(16,16,ILC_COLOR|ILC_MASK,14,1);
	HINSTANCE hRes = GetModuleHandle("1crcrus.dll");
	
	const int count = sizeof(arrayOfInfoTypeAndResIconPair_OwnResources) / sizeof(arrayOfInfoTypeAndResIconPair_OwnResources[0]);
	for (int i = 0; i < count ;i++){
		int IconResID = arrayOfInfoTypeAndResIconPair_OwnResources[i].second;
		pList.Add(LoadIcon(hMyInst, reinterpret_cast<LPCTSTR>( IconResID )));
	}
	
	{
		const int count2 = sizeof(arrayOfInfoTypeAndResIconPair_1CResources) / sizeof(arrayOfInfoTypeAndResIconPair_1CResources[0]);
		for (int i = 0; i < count ;i++){
			int IconResID = arrayOfInfoTypeAndResIconPair_1CResources[i].second;
			pList.Add(LoadIcon(hRes, reinterpret_cast<LPCTSTR>( IconResID )));
		}
	}
}

CCPPView::CCPPView()
{
	m_pView = this;
	m_click = CPoint(0,0);
	//m_pRootData=NULL;

	static bool first = true;

	if(first)
	{
		LoadTypeIcon();
		
		CCPPView::m_HotKeys = LoadAccelerators(hMyInst, LPCTSTR(IDR_ACCELERATOR1));
		first=false;
	}
}

CCPPView::~CCPPView()
{
	m_pRootData.reset();
	m_pView = NULL;

	CSyntax::Clear();
}

BEGIN_MESSAGE_MAP(CCPPView, CTreeView)
	ON_COMMAND(ID_EDIT_COPY,OnCopy)
	ON_COMMAND(ID_FILE_SAVE,OnSave)
	ON_UPDATE_COMMAND_UI(ID_FILE_SAVE, OnUpdateFileSave)
	ON_COMMAND(100,OnProp)
	//{{AFX_MSG_MAP(CCPPView)
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONDOWN()
	ON_NOTIFY_REFLECT(NM_SETFOCUS, OnSetfocus)
	ON_WM_KEYDOWN()
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCPPView drawing


CBodyTextView* CCPPView::FindTextView(CDocument* pDoc)
{
	if(!pDoc)
	{
		Log("Не удалось получить документ.");
		return NULL;
	}
	CView *pTextView=NULL;
	CString kind=pDoc->GetRuntimeClass()->m_lpszClassName;
	POSITION pos=pDoc->GetFirstViewPosition();
	pTextView=pDoc->GetNextView(pos);
	if(kind=="CWorkBookDoc")
	{
		((CWorkBookDoc*)pDoc)->SwitchToPage(1);
		CWnd* pChild=pTextView->GetWindow(GW_CHILD);
		if(pChild)
			pChild=pChild->GetNextWindow();
		if(pChild)
			pTextView=(CBodyTextView*)pChild->GetWindow(GW_CHILD);
	}
	if(strstr(pTextView->GetRuntimeClass()->m_lpszClassName,"CBodyTextView"))
		return (CBodyTextView*)pTextView;
	else
		return NULL;
}

void CCPPView::OnDraw(CDC* pDC)
{
}
/////////////////////////////////////////////////////////////////////////////
// CCPPView message handlers
void CCPPView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult=1;
	keybd_event(VK_RETURN,MapVirtualKey(0,VK_RETURN),0,0);
    keybd_event(VK_RETURN,MapVirtualKey(0,VK_RETURN),KEYEVENTF_KEYUP,0);
}

int CCPPView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if(CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	if(!m_pDocument)
	{
		m_pDocument = static_cast<CDocument*>( RUNTIME_CLASS(CCPPDoc)->CreateObject() );
		assert(m_pDocument);
		m_pDocument->AddView(this);
	}
	GetTreeCtrl().SetImageList(&m_ImageList,TVSIL_NORMAL);
	FillList();

//AfxMessageBox("1");
//	CSyntax::SetHook();
//AfxMessageBox("2");

	return 0;
}

BOOL CCPPView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style|=TVS_HASLINES|TVS_LINESATROOT|TVS_HASBUTTONS;
	return CTreeView::PreCreateWindow(cs);
}

void CCPPView::FillList()
{
	m_pRootData.reset(new CInfoWrcSpace);
	assert(m_pRootData);

	m_pRootData->InsertInTree(TVI_ROOT);

	CDefFileInfo::UpdateLinksForEachDefFile();

	Sort(TVI_ROOT);
	UpdateModified();

	CVis1cppPlugin::Get()->Fire_OnRefreshAll();
}

void CCPPView::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	CInfo* pInfo = CInfo::GetData(GetTreeCtrl().GetSelectedItem());
	if(pInfo)
		pInfo->OnSelect();
}

void CCPPView::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	HTREEITEM item=GetTreeCtrl().GetSelectedItem();
// 	if (!this)
// 		return;
	assert(this);

	CInfo* pInfo=CInfo::GetData(item);
	if(!pInfo)
		return;
	if(!m_click.x && !m_click.y)
	{
		CRect rc;
		if(GetTreeCtrl().GetItemRect(item,rc,TRUE))
		{
			if(rc.bottom>0 && rc.left>0)
				m_click.x=rc.left,m_click.y=rc.bottom;
		}
	}
	ClientToScreen(&m_click);
	pInfo->ShowContextMenu(m_click);
	m_click=CPoint(0,0);
}

void CCPPView::OnRButtonDown(UINT nFlags, CPoint point) 
{
	UINT fl;
	HTREEITEM item=GetTreeCtrl().HitTest(point,&fl);
	if(item)
	{
		m_click=point;
		GetTreeCtrl().SelectItem(item);
	}
}

void CCPPView::Sort(HTREEITEM hParent)
{
	HTREEITEM hItem=GetTreeCtrl().GetChildItem(hParent);
	while(hItem)
	{
		Sort(hItem);
		hItem=GetTreeCtrl().GetNextItem(hItem,TVGN_NEXT);
	}
	TVSORTCB sort;
	sort.hParent = hParent;
	sort.lParam=0;
	sort.lpfnCompare = CInfo::GetCallbackCompareFunction();
	GetTreeCtrl().SortChildrenCB(&sort);
}

void CCPPView::Refresh()
{
    GetTreeCtrl().DeleteAllItems();

	m_pRootData.reset();

	static_cast<CCPPDoc*>(GetDocument())->UpdateMetadata();

	CClassInfo::RemoveAllFileNameInMap();
	CDefFileInfo::RemoveAllDefFiles();

	FillList();

	CCoClasses::Get()->FillAllElements();
	CCoDefFiles::Get()->FillAllElements();
}

void CCPPView::OnCopy()
{
	CInfo* pInfo=CInfo::GetData(GetTreeCtrl().GetSelectedItem());
	if(!pInfo)
		return;
	CString txt=pInfo->GetDisplayName();
	HGLOBAL hglbCopy = GlobalAlloc(GMEM_DDESHARE,txt.GetLength()+1);
	if(!hglbCopy)
		return;
	LPSTR lpStr=(LPSTR)GlobalLock(hglbCopy);
	if(!lpStr)
		return;
	memcpy(lpStr,(LPCTSTR)txt,txt.GetLength()+1);
	GlobalUnlock(hglbCopy);
	OpenClipboard();
	EmptyClipboard();
	SetClipboardData(CF_TEXT,hglbCopy);
	CloseClipboard();
}

void CCPPView::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	CTreeView::OnActivateView(bActivate, pActivateView, pDeactiveView);
	if(!bActivate)
		return;
	MiniRefresh();
}

BOOL CCPPView::PreTranslateMessage(MSG* pMsg) 
{
	if(TranslateAccelerator(m_hWnd,m_HotKeys,pMsg))
		return TRUE;
	return CTreeView::PreTranslateMessage(pMsg);
}

void CCPPView::SortOneNode(HTREEITEM item)
{
	TVSORTCB sort;
	sort.hParent=item;
	sort.lParam=0;
	sort.lpfnCompare = CInfo::GetCallbackCompareFunction();
	GetTreeCtrl().SortChildrenCB(&sort);
}

void CCPPView::OnProp()
{
	CInfo* pInfo=CInfo::GetData(GetTreeCtrl().GetSelectedItem());
	if(pInfo)
		pInfo->ShowProperties();
}

void CCPPView::UpdateClass(CDocument *pDoc)
{
	CString cl = pDoc->GetTitle();
	CClassInfo::CPtr pClassInfo = CClassInfo::FindElem(cl);
	if (pClassInfo)
	{
		pClassInfo->PrepareKeys();
		POSITION pos=pClassInfo->GetNodesList().GetHeadPosition();
		while(pos)
		{
			HTREEITEM item = reinterpret_cast<HTREEITEM>( pClassInfo->GetNodesList().GetNext(pos) );
			pClassInfo->InsertKeys(item);
			SortOneNode(item);
		}
		pClassInfo->ClearRemoved();
	}
}

struct CFunctor_UpdateModifiedForClassInfo
{
 	bool& m_AllMod;
 	
 	CFunctor_UpdateModifiedForClassInfo(bool AllMod) : m_AllMod(AllMod) {};
	void operator() (const CClassInfo::CPtr& pClassInfo) const
	{
		bool bModified = false;
		
		CDocument* pDoc = CDocumentInterceptor::LookupDoc(pClassInfo->GetPath());
		if(pDoc)
			bModified = pDoc->IsModified() == TRUE;
		
		if(bModified)
			m_AllMod = true;
		POSITION pos1=pClassInfo->GetNodesList().GetHeadPosition();
		while(pos1)
		{
			HTREEITEM item = reinterpret_cast<HTREEITEM>( pClassInfo->GetNodesList().GetNext(pos1) );
			CCPPView::GetMyTreeCtrl().SetItemText(item,pClassInfo->GetDisplayName()+(bModified?" *":""));
		}
	}
};

void CCPPView::UpdateModified()
{
	bool AllMod = false;
	CClassInfo::ForEachClassInfo( CFunctor_UpdateModifiedForClassInfo(AllMod) );

	CTreeCtrl& ctrl=GetTreeCtrl();
	HTREEITEM first=ctrl.GetChildItem(TVI_ROOT),item=ctrl.GetChildItem(first);
	while(item)
	{
		CDefFileInfo* pInfo = static_cast<CDefFileInfo*>( CInfo::GetData(item) );
		if(pInfo)
		{
			bool bModified = false;
			
			CDocument* pDoc = CDocumentInterceptor::LookupDoc(pInfo->GetPath());
			if(pDoc)
				bModified = pDoc->IsModified() == TRUE;
			if(bModified)
				AllMod = true;
			ctrl.SetItemText(item, pInfo->GetDisplayName()+(bModified?" *":""));
		}
		item=ctrl.GetNextItem(item, TVGN_NEXT);
	}

	if(m_pRootData)
		ctrl.SetItemText(first, m_pRootData->GetDisplayName()+(AllMod?" *":""));
}

void CCPPView::OnUpdateFileSave(CCmdUI *pcm)
{
	CInfo* pInfo=CInfo::GetData(GetTreeCtrl().GetSelectedItem());
	if(!pInfo)
	{
		pcm->Enable(FALSE);
		return;
	}
	if(pInfo->GetType() > TYPE_CLASS_FOLDER)
	{
		pcm->Enable(FALSE);
		return;
	}
	CString txt = GetTreeCtrl().GetItemText(CInfo::GetCurItem());
	pcm->Enable(txt.ReverseFind('*')==txt.GetLength()-1);
}

void CCPPView::OnSave()
{
	CInfo* pInfo = CInfo::GetData(GetTreeCtrl().GetSelectedItem());
	if(!pInfo)
		return;
	if(pInfo->GetType() > TYPE_CLASS_FOLDER)
		return;

	if(pInfo->GetType() == TYPE_CLASS_FOLDER)
	{
		CDocument* pDoc = CDocumentInterceptor::LookupDoc(((CClassInfo*)pInfo)->GetPath());
		if(pDoc)
			pDoc->OnCmdMsg(ID_FILE_SAVE, (WM_COMMAND<<16)|CN_COMMAND, NULL, NULL);
	}

	else if(pInfo->GetType() == TYPE_DEF_FILE_FOLDER)
	{
		CDocument* pDoc = CDocumentInterceptor::LookupDoc(((CDefFileInfo*)pInfo)->GetPath());
		if(pDoc)
			pDoc->OnCmdMsg(ID_FILE_SAVE, (WM_COMMAND<<16)|CN_COMMAND, NULL, NULL);
	}

	else
		CDocumentInterceptor::SendSaveCommandToAllDocuments();

	CCPPView::m_pView->UpdateModified();
}

void CCPPView::OnSetfocus(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	MiniRefresh();
}

void CCPPView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar==VK_ESCAPE)
	{
		SECMDIFrameWnd* pFrame=(SECMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pChild=pFrame->MDIGetActive();
		if(pChild && pChild->IsWindowVisible())
		{
			pChild->ActivateFrame();
			CView* pView=pChild->GetActiveView();
			if(pView)
			{
				pView->SetFocus();
				return;
			}
		}
	}
	CTreeView::OnKeyDown(nChar, nRepCnt, nFlags);
}

struct CFunctor_PrepareKeysForClassInfo
{
	MAP_Str_Info& m_changed;

	CFunctor_PrepareKeysForClassInfo(MAP_Str_Info& changed) : m_changed(changed) {};
	void operator() (CDocument* pDoc) const
	{
		CString cl = pDoc->GetTitle();

		CClassInfo::CPtr pClassInfo = CClassInfo::FindElem(cl);
		if (pClassInfo)
		{
			m_changed[cl] = pClassInfo.get();
			pClassInfo->PrepareKeys();
		}
	}
};

void CCPPView::MiniRefresh()
{
	MAP_Str_Info changed;
	CDocumentInterceptor::ForEachDocument( CFunctor_PrepareKeysForClassInfo(changed) );

	for(Str_Info_ITER it=changed.begin();it!=changed.end();it++)
	{
		CClassInfo* pClassInfo = static_cast<CClassInfo*>(it->second);
		POSITION pos=pClassInfo->GetNodesList().GetHeadPosition();
		while(pos)
		{
			HTREEITEM item=(HTREEITEM)pClassInfo->GetNodesList().GetNext(pos);
			pClassInfo->InsertKeys(item);
			SortOneNode(item);
		}
		pClassInfo->ClearRemoved();
	}
	UpdateModified();
}

void CCPPView::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	// TODO: Add your control notification handler code here
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	CTreeCtrl& pTreeCtl = GetTreeCtrl();
	HTREEITEM nItem = pTreeCtl.GetSelectedItem();
	if ((pNMTreeView) != NULL){ 
		if (pTreeCtl.GetItem(&(pNMTreeView->itemNew))){ 
			CString strItem;
			strItem = pTreeCtl.GetItemText(pNMTreeView->itemNew.hItem);
			HGLOBAL hGlobal = GlobalAlloc(GMEM_SHARE,
						  strItem.GetLength() + 1);
			LPSTR   pszGlobal = (LPSTR)GlobalLock(hGlobal);
			ASSERT(pszGlobal);
			lstrcpy(pszGlobal, strItem);
			GlobalUnlock(hGlobal);
			COleDataSource srcItem;

			srcItem.CacheGlobalData(CF_TEXT, hGlobal);

			srcItem.DoDragDrop(DROPEFFECT_COPY|DROPEFFECT_MOVE);
		}
	}		
	*pResult = 0;
}
