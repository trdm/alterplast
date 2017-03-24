// PanelSetupDlg.cpp : implementation file
//

#include "stdafx.h"
#include "CONFIGSVCIMPL.h"
#include "PanelSetupDlg.h"
#include "PluginManager.h"
#include "Option.h"

/////////////////////////////////////////////////////////////////////////////
// CPanelSetupDlg dialog

void LoadPanels(SPanelInfo* pInfo);

CPanelSetupDlg::CPanelSetupDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CPanelSetupDlg::IDD, pParent),m_isNameEdit(false),m_isModified(false)
{
	//{{AFX_DATA_INIT(CPanelSetupDlg)
	//}}AFX_DATA_INIT
}


void CPanelSetupDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPanelSetupDlg)
	DDX_Control(pDX, IDC_LIST1, m_list);
	DDX_Control(pDX, IDC_TREE1, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPanelSetupDlg, CDialog)
	//{{AFX_MSG_MAP(CPanelSetupDlg)
	ON_BN_CLICKED(IDC_ADD_PANEL, OnAddPanel)
	ON_BN_CLICKED(IDC_MOVE_LEFT, OnMoveLeft)
	ON_BN_CLICKED(IDC_MOVE_RIGHT, OnMoveRight)
	ON_BN_CLICKED(IDC_REMOVE_PANEL, OnRemovePanel)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, OnBeginlabeledit)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, OnEndlabeledit)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(NM_DBLCLK, IDC_LIST1, OnDblclkList1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPanelSetupDlg message handlers

void CPanelSetupDlg::OnMoveLeft() 
{
	int nSel=m_list.GetNextItem(-1,LVIS_SELECTED);
	if(nSel<0)
		return;
	HTREEITEM hParent,hItem=m_tree.GetSelectedItem();
	if(!hItem)
		return;
	hParent=m_tree.GetNextItem(hItem,TVGN_PARENT);
	if(hParent)
		hItem=hParent;
	CString name=m_list.GetItemText(nSel,0);

	LVITEM lvi;
	lvi.iItem=nSel;
	lvi.iSubItem=0;
	lvi.mask=LVIF_IMAGE;
	m_list.GetItem(&lvi);
	m_tree.InsertItem(name,lvi.iImage,lvi.iImage,hItem);
	m_tree.Expand(hItem,TVE_EXPAND);
	m_list.DeleteItem(nSel);
	m_isModified=true;
}

void CPanelSetupDlg::OnMoveRight() 
{
	HTREEITEM hItem=m_tree.GetSelectedItem();
	int iImage,iSelImage;
	if(m_tree.GetNextItem(hItem,TVGN_PARENT))
	{
		m_tree.GetItemImage(hItem,iImage,iSelImage);
		m_list.InsertItem(0,m_tree.GetItemText(hItem),iImage);
		m_tree.DeleteItem(hItem);
		m_isModified=true;
	}
	else
	{
		HTREEITEM hChild;
		while(hChild=m_tree.GetNextItem(hItem,TVGN_CHILD))
		{
			m_tree.GetItemImage(hChild,iImage,iSelImage);
			m_list.InsertItem(0,m_tree.GetItemText(hChild),iImage);
			m_tree.DeleteItem(hChild);
			m_isModified=true;
		}
	}
}

void CPanelSetupDlg::OnRemovePanel() 
{
	HTREEITEM hItem=m_tree.GetSelectedItem();
	if(m_tree.GetNextItem(hItem,TVGN_PARENT))
		return;
	CString strPaneName=m_tree.GetItemText(hItem);
	if(AfxMessageBox("Удалить панель "+strPaneName+" ?",MB_YESNO|MB_ICONQUESTION)==IDNO)
		return;
	OnMoveRight();
	m_tree.DeleteItem(hItem);
	m_iPaneCnt--;
	m_isModified=true;
}

void CPanelSetupDlg::OnAddPanel() 
{
	if(m_iPaneCnt==16)
	{
		AfxMessageBox("Нельзя создать более 16 панелей");
		return;
	}
	m_iPaneCnt++;
	m_isModified=true;
	HTREEITEM hItem=m_tree.InsertItem("",0,0);
	m_tree.Select(hItem,TVGN_CARET);
	m_tree.EditLabel(hItem);
}


HICON GetIcon(CPluginManager::Info* pInfo);

BOOL CPanelSetupDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	AfxSetResourceHandle(h1CResource);
	m_hImgList=ImageList_Create(16,16,ILC_MASK|ILC_COLOR32,0,1);
	m_tree.SendMessage(TVM_SETIMAGELIST,TVSIL_NORMAL,(LPARAM)m_hImgList);
	m_list.SendMessage(LVM_SETIMAGELIST,LVSIL_SMALL,(WPARAM)m_hImgList);
	HICON hImg=(HICON)LoadImage(g_hInst,(LPCTSTR)IDI_SCRIPT,IMAGE_ICON,16,16,0);
	ImageList_AddIcon(m_hImgList,hImg);
	DestroyIcon(hImg);
	
	LoadPanels(m_panels);

	CMapStringToPtr mapLoadedPlugins;
	CString name;
	CPluginManager* pPM=CPluginManager::GetPM();
	void* fnd;
	m_iPaneCnt=0;
	CPluginManager::Info* pInfo;
	STabInfo* pTabInfo;
	HICON hIcon=NULL;
	int iIcon;
	
	for(int i=0;i<16;i++)
	{
		if(m_panels[i].name.IsEmpty())
			break;
		m_iPaneCnt++;
		HTREEITEM hPane=m_tree.InsertItem(m_panels[i].name,0,0);
		m_tree.SetItemData(hPane,i);
		name=m_panels[i].name;
		name.MakeLower();
		m_mapPaneNames[name]=(void*)1;
		int tCnt=m_panels[i].tabs.GetSize();
		for(int k=0;k<tCnt;k++)
		{
			name=m_panels[i].tabs[k];
			iIcon=-1;
			pInfo=pPM->GetPluginByName(name);
			if(pInfo)
			{
				hIcon=::GetIcon(pInfo);
				iIcon=ImageList_AddIcon(m_hImgList,hIcon);
				DestroyIcon(hIcon);
			}
			else
			{
				if(CPanelManager::m_rtcPanels.Lookup(name,(void*&)pTabInfo))
				{
					if(pTabInfo->hIcon)
						iIcon=ImageList_AddIcon(m_hImgList,pTabInfo->hIcon);
				}
			}

			m_tree.InsertItem(name,iIcon,iIcon,hPane);
			name.MakeLower();
			mapLoadedPlugins[name]=(void*)1;
		}
		m_tree.Expand(hPane,TVE_EXPAND);
	}

	for(POSITION pos=pPM->m_Plugins.GetStartPosition();pos;)
	{
		pPM->m_Plugins.GetNextAssoc(pos,name,(void*&)pInfo);
		if(pInfo)
		{
			IOleControlPtr ctrl=pInfo->pPlugin;
			if(ctrl!=NULL)
			{
				if(!mapLoadedPlugins.Lookup(name,fnd))
				{
					iIcon=ImageList_AddIcon(m_hImgList,::GetIcon(pInfo));
					m_list.InsertItem(0,pInfo->name,iIcon);
					mapLoadedPlugins[name]=(void*)1;
				}
			}
		}
	}
	for(pos=CPanelManager::m_rtcPanels.GetStartPosition();pos;)
	{
		CPanelManager::m_rtcPanels.GetNextAssoc(pos,name,(void*&)pTabInfo);
		CString lname=name;
		lname.MakeLower();
		if(!mapLoadedPlugins.Lookup(lname,fnd))
		{
			iIcon=ImageList_AddIcon(m_hImgList,pTabInfo->hIcon);
			m_list.InsertItem(0,name,iIcon);
			mapLoadedPlugins[lname]=(void*)1;
		}
	}
	return TRUE;
}

void CPanelSetupDlg::OnBeginlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult=m_tree.GetNextItem(((TV_DISPINFO*)pNMHDR)->item.hItem,TVGN_PARENT)?TRUE:FALSE;
	m_isNameEdit=*pResult!=TRUE;
}

void CPanelSetupDlg::OnEndlabeledit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_isNameEdit=false;
	*pResult = FALSE;
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	if(pTVDispInfo->item.pszText)
	{
		CString newName=pTVDispInfo->item.pszText;
		if(!newName.IsEmpty())
		{
			CString lName=newName;
			lName.MakeLower();
			void* fnd;
			if(!m_mapPaneNames.Lookup(lName,fnd))
			{
				m_mapPaneNames[lName]=(void*)1;
				lName=m_tree.GetItemText(pTVDispInfo->item.hItem);
				lName.MakeLower();
				m_mapPaneNames.RemoveKey(lName);
				*pResult=1;
				m_isModified=true;
			}
		}
	}
}

void CPanelSetupDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	OnMoveRight();
}

BOOL CPanelSetupDlg::PreTranslateMessage(MSG* pMsg) 
{
	if(m_isNameEdit)
		return FALSE;
	return CDialog::PreTranslateMessage(pMsg);
}

void CPanelSetupDlg::OnOK() 
{
	if(!m_isModified)
	{
		EndDialog(IDOK);
		return;
	}
	COption& opt=COption::GetOptions();
	opt.Delete("Panels");
	HTREEITEM hPanels=m_tree.GetNextItem(TVI_ROOT,TVGN_CHILD);
	CString path;
	int i=0;
	while(hPanels)
	{
		path.Format("Panels\\%i\\",i);
		opt.SetString(path,m_tree.GetItemText(hPanels));
		int j=0;
		HTREEITEM hTab=m_tree.GetNextItem(hPanels,TVGN_CHILD);
		while(hTab)
		{
			path.Format("Panels\\%i\\%i\\",i,j++);
			opt.SetString(path,m_tree.GetItemText(hTab));
			hTab=m_tree.GetNextItem(hTab,TVGN_NEXT);
		}
		i++;
		hPanels=m_tree.GetNextItem(hPanels,TVGN_NEXT);
	}
	AfxMessageBox("Для вступления изменений в силу перезапустите Конфигуратор");
	EndDialog(IDOK);
}

void CPanelSetupDlg::OnCancel() 
{
	if(m_isModified)
	{
		if(AfxMessageBox("Выйти без сохранения изменений состава панелей?",MB_YESNO|MB_ICONQUESTION)==IDNO)
			return;
	}
	EndDialog(IDCANCEL);
}

void CPanelSetupDlg::OnDblclkList1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	OnMoveLeft();
}
