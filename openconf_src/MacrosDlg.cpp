// MacrosDlg.cpp : implementation file
//

#include "stdafx.h"
#include "configsvcimpl.h"
#include "MacrosDlg.h"
#include "scriptmaneger.h"
#include "pluginmanager.h"

/////////////////////////////////////////////////////////////////////////////
// CMacrosDlg dialog
CString CMacrosDlg::m_LastSelPath;
bool CMacrosDlg::m_LastSelIsScript;


CMacrosDlg::CMacrosDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMacrosDlg::IDD, pParent)
{
	m_selDispatch=NULL;
	m_selMacros=0;
	//{{AFX_DATA_INIT(CMacrosDlg)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CMacrosDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CMacrosDlg)
	DDX_Control(pDX, IDC_TREE1, m_tree);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CMacrosDlg, CDialog)
	//{{AFX_MSG_MAP(CMacrosDlg)
	ON_BN_CLICKED(IDOK, OnPressOK)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(NM_RETURN, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMacrosDlg message handlers

void CMacrosDlg::OnPressOK() 
{
	HTREEITEM item=m_tree.GetSelectedItem();
	if(!item)
		return;
	Info* pInfo=(Info*)m_tree.GetItemData(item);
	if(!pInfo)
		return;
	m_selDispatch=pInfo->pDisp;
	m_selMacros=pInfo->dispID;
	m_LastSelIsScript=pInfo->IsScript;
	HTREEITEM parent=m_tree.GetParentItem(item);
	m_LastSelPath=m_tree.GetItemText(parent)+"::"+m_tree.GetItemText(item);
	EndDialog(IDOK);
}

void CMacrosDlg::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	OnPressOK();
	*pResult = 1;
}

void CMacrosDlg::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HTREEITEM item=m_tree.GetSelectedItem();
	if(!item)
		return;
	Info* pInfo=(Info*)m_tree.GetItemData(item);
	GetDlgItem(IDOK)->EnableWindow(pInfo!=NULL);
	*pResult = 1;
}

BOOL CMacrosDlg::OnInitDialog() 
{
	CDialog::OnInitDialog();
	CScriptManager* pSM=CScriptManager::GetScriptManager();
	long count=pSM->GetScriptCount();
	for(int i=0;i<count;i++)
	{
		CScripting* pScript=pSM->GetScriptByNum(i);
		IDispatchPtr pDisp;
		try{
			HRESULT hr=pScript->m_pEngine->GetScriptDispatch(NULL,&pDisp);
			if(S_OK==hr)
				InsertMacros(pDisp,pScript->m_ScriptName,true);
		}
		catch(...){}
	}
	CMapStringToPtr *map=&CPluginManager::GetPM()->m_Plugins;
	CString name;
	CPluginManager::Info* pInfo;
	for(POSITION pos=map->GetStartPosition();pos;)
	{
		map->GetNextAssoc(pos,name,(void*&)pInfo);
		try{
			if(pInfo->pPlugin!=NULL && pInfo->ShowMacros)
				InsertMacros(pInfo->pPlugin,pInfo->name,false);
		}
		catch(...){}
	}
	HTREEITEM hSelItem=NULL;
	if(!m_LastSelPath.IsEmpty())
	{
		CString lastSel=m_LastSelPath;
		lastSel.MakeLower();
		HTREEITEM item=m_tree.GetNextItem(TVI_ROOT,TVGN_CHILD);
		while(item)
		{
			CString path=m_tree.GetItemText(item)+"::";
			path.MakeLower();
			if(lastSel.Find(path)==0)
			{
				HTREEITEM child=m_tree.GetNextItem(item,TVGN_CHILD);
				while(child)
				{
					Info* pInfo=(Info*)m_tree.GetItemData(child);
					if(pInfo->IsScript==m_LastSelIsScript)
					{
						CString name=m_tree.GetItemText(child);
						name.MakeLower();
						if(path+name==lastSel)
						{
							hSelItem=child;
							goto fnd;
						}
					}
					child=m_tree.GetNextItem(child,TVGN_NEXT);
				}
			}
			item=m_tree.GetNextItem(item,TVGN_NEXT);
		}
	}
fnd:
	if(hSelItem)
		m_tree.SelectItem(hSelItem);
	return TRUE;
}

static void RemoveItem(CTreeCtrl& tree,HTREEITEM item)
{
	HTREEITEM child=tree.GetNextItem(item,TVGN_CHILD);
	while(child)
	{
		RemoveItem(tree,child);
		child=tree.GetNextItem(child,TVGN_NEXT);
	}
	if(item==TVI_ROOT)
		return;
	CMacrosDlg::Info* pInfo=(CMacrosDlg::Info*)tree.GetItemData(item);
	if(pInfo)
		delete pInfo;
}
void CMacrosDlg::OnDestroy() 
{
	RemoveItem(m_tree,TVI_ROOT);
	CDialog::OnDestroy();
}

void CMacrosDlg::InsertMacros(IDispatchPtr& pDisp, CString name,bool IsScript)
{
	CStringArray names;
	CDWordArray dispIDs;
	ExtractMacrosFromDispatch(pDisp,names,dispIDs);
	int size=names.GetSize();
	if(size)
	{
		HTREEITEM item=m_tree.InsertItem(name);
		for(int i=0;i<size;i++)
		{
			Info* pInfo=new Info;
			pInfo->pDisp=pDisp;
			pInfo->dispID=dispIDs[i];
			pInfo->IsScript=IsScript;
			HTREEITEM macros=m_tree.InsertItem(names[i],item);
			m_tree.SetItemData(macros,(DWORD)pInfo);
		}
	}

}

void CMacrosDlg::InvokeMacros()
{
	CMacrosDlg dlg;
	if(dlg.DoModal()!=IDOK)
		return;
	if(dlg.m_selDispatch==NULL)
		return;
	DISPPARAMS none={0,0,0,0};
	dlg.m_selDispatch->Invoke(dlg.m_selMacros,IID_NULL,0,DISPATCH_METHOD,&none,NULL,NULL,NULL);
}

int CMacrosDlg::DoModal() 
{
	AfxSetResourceHandle(g_hInst);
	int ret=CDialog::DoModal();
	AfxSetResourceHandle(h1CResource);
	return ret;
}

void CMacrosDlg::GetLastSel(bool &IsScript, CString &modul, CString &macros)
{
	IsScript=m_LastSelIsScript;
	int fnd=m_LastSelPath.Find("::");

	modul=m_LastSelPath.Left(fnd);
	macros=m_LastSelPath.Mid(fnd+2);
}
