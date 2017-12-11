// EFTreeView.cpp : implementation file
//

#include "stdafx.h"
#include "efexplorer.h"
#include "EFTreeView.h"
#include "res/resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define EFFOLDERS_FILE_NAME "effolders.txt"

/////////////////////////////////////////////////////////////////////////////
// CEFTreeView

IMPLEMENT_DYNCREATE(CEFTreeView, CTreeView)

CEFTreeView::CEFTreeView()
{
}

CEFTreeView::~CEFTreeView()
{
}


BEGIN_MESSAGE_MAP(CEFTreeView, CTreeView)
	//{{AFX_MSG_MAP(CEFTreeView)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_DBLCLK, OnDblclk)
	ON_NOTIFY_REFLECT(NM_RETURN, OnReturn)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CEFTreeView drawing

void CEFTreeView::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();

}

/////////////////////////////////////////////////////////////////////////////
// CEFTreeView diagnostics

#ifdef _DEBUG
void CEFTreeView::AssertValid() const
{
	CTreeView::AssertValid();
}

void CEFTreeView::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

static int CALLBACK ItemCompareProc(LPARAM lParam1, LPARAM lParam2, LPARAM lParamSort)
{
  if (((CItemData*)lParam1)->m_IsFolder ^ ((CItemData*)lParam2)->m_IsFolder)
    return ((CItemData*)lParam1)->m_IsFolder ? -1 : 1;
  else
    return strcmp(((CItemData*)lParam1)->m_Path, ((CItemData*)lParam2)->m_Path);
}

BOOL isValidExt(CString ext, int& icoIndex, int& itemType)
{
  if (ext==".ert") {
    icoIndex=2; itemType=1;
  }
  else if (ext==".mxl") {
    icoIndex=3; itemType=2;
  }
  else if (ext==".txt") {
    icoIndex=4; itemType=3;
  }
  else if (ext==".mlg") {
    icoIndex=4; itemType=4;
  }
  else if (ext==".spl") {
    icoIndex=4; itemType=5;
  }
  else if (ext==".prm") {
    icoIndex=4; itemType=6;
  }
  else {
    icoIndex=0;
    return FALSE;
  }
  return TRUE;
}

HTREEITEM CEFTreeView::MakeBranchOfTree(HTREEITEM hParentItem, HTREEITEM hAfterItem, CString Path, CString Name, int& CntItem, void* ExceptPath)
{
  Path.MakeUpper();

  if (ExceptPath)
    if (((CStringList*)ExceptPath)->Find(Path)||
        ((CStringList*)ExceptPath)->Find(m_IBDir+Path))
      return hAfterItem;
 	
  CFileFind ff;
  BOOL res = ff.FindFile(Path+"*.*");
  if (!res)
    res = ff.FindFile(m_IBDir+Path+"*.*");
  if (!res)
    return hAfterItem;

  HTREEITEM hFolder = m_tree->InsertItem(Name, 1, 1, hParentItem, hAfterItem);
  m_tree->SetItemData(hFolder, (DWORD)new CItemData(Path,TRUE,0));

  BOOL ShowExt = 1;//((CButton*)GetDlgItem(IDC_CHECK3))->GetCheck();

  HTREEITEM hLastFolder = TVI_FIRST;
  int cnt = 0;
  while (res)
	{
    res = ff.FindNextFile();

    if (ff.IsDots())
      continue;
    
    if (ff.IsDirectory())
    {
      int cntInFolders = 0;
      hLastFolder = MakeBranchOfTree(hFolder, hLastFolder, ff.GetFilePath()+"\\", ff.GetFileTitle(), cntInFolders, ExceptPath);
      cnt += cntInFolders;
    }
    else
    {
      int icoIndex, itemType;
      if (!isValidExt(ff.GetFileName().Right(4),icoIndex, itemType))
        continue;
      
      HTREEITEM hItem = m_tree->InsertItem( ShowExt ? ff.GetFileName() : ff.GetFileTitle(),
                                             icoIndex, icoIndex, hFolder, TVI_LAST);
      m_tree->SetItemData(hItem, (DWORD)new CItemData(ff.GetFilePath(), FALSE, itemType));
      cnt++;
    };
	};

  if ( m_tree->ItemHasChildren(hFolder) /*|| ((CButton*)GetDlgItem(IDC_CHECK2))->GetCheck()*/)
  {
    //if (((CButton*)GetDlgItem(IDC_CHECK1))->GetCheck())
    {
      Name.Format("%s (%d)", Name, cnt);
      m_tree->SetItemText(hFolder, Name);
    }
  }
  else
  {
    m_tree->DeleteItem(hFolder);
  }

  TVSORTCB tvs;
  tvs.hParent = hFolder;
  tvs.lParam = (LPARAM)m_tree;
  tvs.lpfnCompare = ItemCompareProc;
  m_tree->SortChildrenCB(&tvs);

//  if (TRUE)
//    GroupByFirstSymbol(hFolder);

  CntItem = cnt;
  return hFolder;
}

void CEFTreeView::GroupByFirstSymbol(HTREEITEM hParentItem)
{
  HTREEITEM hItem = m_tree->GetNextItem(hParentItem, TVGN_CHILD);
  while (hItem)
  {
    //todo: move
    hItem = m_tree->GetNextItem(hItem,TVGN_NEXT);
  }
}

void CEFTreeView::LoadDescrFile(CString fileName)
{
  CStringList ExceptPath;
  TRY
  {
    CStdioFile file(fileName, CFile::modeRead | CFile::shareDenyNone );
    CString str;
    while(file.ReadString(str))
    {
      if (str.Left(1)=="-")
      {
        str = str.Mid(1);
        str.TrimLeft();
        str.TrimRight();
        str.MakeUpper();
        ExceptPath.AddTail(str);
        ExceptPath.AddTail(m_IBDir+str);
      }
    }

    CString name;
    file.SeekToBegin();
    while(file.ReadString(str))
    {
      if ((str.IsEmpty())||(str.Left(2)=="//")||(str.Left(1)=="-"))
        continue;
      int comma_pos = str.Find(',');
      name = str.Mid(comma_pos+1);
      name.TrimLeft();
      name.TrimRight();
      if (comma_pos<0)
        str = name;
      else
      {
        str = str.Left(comma_pos);
        str.TrimLeft();
        str.TrimRight();
      }
      if (str.IsEmpty())
        str = m_IBDir;
      int cnt = 0;
      MakeBranchOfTree(TVI_ROOT, TVI_LAST, str, name, cnt, &ExceptPath);
    }
    file.Close();
  }
  CATCH( CFileException, e )
  {
    #ifdef _DEBUG
      AfxMessageBox("Файл отсутствует: "+fileName);
    #endif
  }
  END_CATCH
}

void CEFTreeView::UpdateTree()
{
  m_tree->DeleteAllItems();

  LoadDescrFile(pSvc->IBDir()+EFFOLDERS_FILE_NAME);
  LoadDescrFile(pSvc->BinDir()+"config\\"+EFFOLDERS_FILE_NAME);

  if (!m_tree->GetCount())
  {
    int cnt = 0;
    MakeBranchOfTree(TVI_ROOT, TVI_LAST, m_IBDir+"ExtForms\\", "ExtForms", cnt, NULL);
  }

}

/////////////////////////////////////////////////////////////////////////////
// CEFTreeView message handlers

int CEFTreeView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{

	if (CTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
  m_IBDir = pSvc->IBDir();
  m_IBDir.MakeUpper();
	
	m_imageList.Create(16, 16, ILC_COLOR8|ILC_MASK, 8, 1);
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON1));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON2));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON3));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON4));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON5));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON5));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON5));
  m_imageList.Add(LoadIcon(hMyInst,(LPCTSTR)IDI_ICON5));

  m_tree = (CTreeCtrl*)&GetTreeCtrl();
  m_tree->SetImageList(&m_imageList,TVSIL_NORMAL);
  UpdateTree();

  return 0;
}

BOOL CEFTreeView::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style = cs.style
    |TVS_HASBUTTONS
    |TVS_HASLINES
    |TVS_LINESATROOT
    /*|TVS_EDITLABELS*/;
	return CTreeView::PreCreateWindow(cs);
}


long CEFTreeView::OpenDoc()
{
  CItemData* pItemData = (CItemData*)m_tree->GetItemData(m_tree->GetSelectedItem());
  if (!pItemData->m_IsFolder)
    if(CDocument* doc = pSvc->OpenFile(pItemData->m_Path))
    {
      /*
      POSITION pos = doc->GetFirstViewPosition();
      CView* pFirstView = doc->GetNextView(pos);
      pFirstView->SetActiveWindow();
      pFirstView->SetFocus();
      */
      return 1;
    }
  return 0;
  
}

void CEFTreeView::OnReturn(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = OpenDoc();
}

void CEFTreeView::OnDblclk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = OpenDoc();
}

void CEFTreeView::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
  switch (pTVKeyDown->wVKey)
  {
  case VK_F5:
    {
      UpdateTree();
      break;
    }
  };
	
	*pResult = 1;
}
