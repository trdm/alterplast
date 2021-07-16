// Sections.cpp : implementation file
//

#include "stdafx.h"
//#include "resource.h"
#include "Sections.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CSections dialog


CSections::CSections(CWnd* pParent /*=NULL*/)
	: CDialog(CSections::IDD, pParent)
{
	//{{AFX_DATA_INIT(CSections)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	pGrid=NULL;
	hHorizSection=NULL;
	hVertSection=NULL;

}


void CSections::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CSections)
	DDX_Control(pDX, IDC_TREE1, m_TreeList);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CSections, CDialog)
	//{{AFX_MSG_MAP(CSections)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDMYCANCEL, OnMycancel)
	ON_NOTIFY(TVN_ENDLABELEDIT, IDC_TREE1, OnEndlabeleditTree1)
	ON_NOTIFY(LVN_ENDLABELEDIT, IDC_TREE1, OnEndSubItemLabelEdit)
	ON_BN_CLICKED(ID_ADD, OnAdd)
	ON_BN_CLICKED(ID_DEL, OnDel)
	ON_NOTIFY(NM_DBLCLK, IDC_TREE1, OnDblclkTree1)
	ON_NOTIFY(TVN_BEGINLABELEDIT, IDC_TREE1, OnBeginlabeleditTree1)
	ON_NOTIFY(LVN_BEGINLABELEDIT, IDC_TREE1, OnBeginSubItemLabelEdit)
	ON_NOTIFY(TVN_SELCHANGED, IDC_TREE1, OnSelchangedTree1)
	ON_NOTIFY(NM_CLICK, IDC_TREE1, OnClickTree1)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSections message handlers



CSections::Create()
{
	CDialog::Create(IDD);
}



void CSections::Load(CGridCtrlExt *pGrid2)
{
	pGrid=pGrid2;


	m_TreeList.DeleteAllItems();

	HTREEITEM hParent =0;
	HTREEITEM hNew = 0;

	CListCtrl	m_TreeList;
	hParent = m_TreeList.InsertItem(0,"Горизонтальные:");
	m_TreeList.SetItemState(hParent,TVIS_BOLD,TVIS_BOLD);
	//if(pHorizSection)
	for(int i=0;i<pGrid->m_Data.HorizSection.GetSize();i++)
	{
		//AddItem
		CString Str=pGrid->m_Data.HorizSection[i].csName;
		hNew=m_TreeList.InsertItem(Str,hParent,hNew);
/*		Str.Format("%d",pGrid->m_Data.HorizSection[i].nRange1);
		m_TreeList.SetItemText( hNew, 1, Str);
		Str.Format("%d",pGrid->m_Data.HorizSection[i].nRange2);
		m_TreeList.SetItemText( hNew, 2, Str);
*/
	}
	m_TreeList.Expand(hParent,TVE_EXPAND);
	hHorizSection=hParent;


	hParent = m_TreeList.InsertItem("Вертикальные:",0,0);
	m_TreeList.SetItemState(hParent,TVIS_BOLD,TVIS_BOLD);
	//if(pVertSection)
	for(i=0;i<pGrid->m_Data.VertSection.GetSize();i++)
	{
		CString Str=pGrid->m_Data.VertSection[i].csName;
		hNew=m_TreeList.InsertItem(Str,hParent,hNew);
/*		Str.Format("%d",pGrid->m_Data.VertSection[i].nRange1);
		m_TreeList.SetItemText( hNew, 1, Str);
		Str.Format("%d",pGrid->m_Data.VertSection[i].nRange2);
		m_TreeList.SetItemText( hNew, 2, Str);
*/
	}
	m_TreeList.Expand(hParent,TVE_EXPAND);
	hVertSection=hParent;




}
BOOL CSections::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
/*
	CSize sz(112, 11);
	CFont* pFont = GetFont();
	if(pFont)
		sz = SECConvertDBU(sz.cx, sz.cy, (HFONT) pFont->m_hObject);

	VERIFY(m_edit.SetBitmap(IDB_CALCBTN));
	VERIFY(m_edit.AttachEdit(IDC_EDIT1, this));
	
*/





	int numColors = GetDC()->GetDeviceCaps( NUMCOLORS );
	COLORREF clrBack =  (numColors == -1 ) ? RGB( 192, 220, 192) : RGB( 255, 255, 255);

	// init the columns
	//m_TreeList.EnableHeaderCtrl( TRUE );
	//m_TreeList.SetColumnHeading(0, _T("Секция") );
	//m_TreeList.SetColumnWidth(0, 160);
	//m_TreeList.InsertColumn( 1, _T("От"), LVCFMT_LEFT,40);
	//m_TreeList.InsertColumn( 2, _T("До"), LVCFMT_LEFT,40);

	//m_TreeList.StoreSubItemText( TRUE );


	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CSections::OnCancel() 
{
	//CDialog::OnCancel();
}

void CSections::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	
	CDialog::OnCancel();
	//CDialog::OnClose();
}

void CSections::OnMycancel() 
{
	CDialog::OnCancel();
	
}

void CSections::OnEndlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;
	// TODO: Add your control notification handler code here
	
	*pResult = 1;	// must return 1 to commit the edit
}

void CSections::OnEndSubItemLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvdi; 
	plvdi = (LV_DISPINFO FAR *) pNMHDR;
	
	// get the handle, based on the list index
	//HTREEITEM hti = m_TreeList.GetItemAt( plvdi->item.iItem );

	*pResult = 1;	// must return 1 to commit the edit
	
}

void CSections::OnBeginSubItemLabelEdit(NMHDR* pNMHDR, LRESULT* pResult) 
{
	*pResult = 1;
	HTREEITEM hTree=m_TreeList.GetFirstSelectedItem();
	if(hTree)
		if(m_TreeList.GetParentItem(hTree))
		{
			*pResult = 0;
		};
}


void CSections::OnAdd() 
{
	CString csRange1="1";
	CString csRange2="1";

	HTREEITEM hTree=m_TreeList.GetFirstSelectedItem();
	HTREEITEM hParent=0;
	if(hTree)
	{
		hParent=m_TreeList.GetParentItem(hTree);
		if(!hParent)
		{
			hParent=hTree;
			hTree=0;
		}
	}

	//определяем параметры по таблице
	if(pGrid)
	{
		CRowColArray awCols,awCols2,awRows,awRows2;
		int nCols=0,nRows=0;
		if (pGrid->GetSelectedCols(awCols))
			nCols=awCols.GetSize();
		if (pGrid->GetSelectedRows(awRows))
			nRows=awRows.GetSize();


		if(nCols<nRows)
		{
			if (pGrid->GetSelectedCols(awCols,awCols2))
			{
				hTree=0;
				hParent=hVertSection;
				csRange1.Format("%d",awCols[0]);
				csRange2.Format("%d",awCols2[0]);
			}
		}
		else
		if(nRows<nCols)
		{
			if (pGrid->GetSelectedRows(awRows,awRows2))
			{
				hTree=0;
				hParent=hHorizSection;
				csRange1.Format("%d",awRows[0]);
				csRange2.Format("%d",awRows2[0]);
			}
		}
	}


	CString Str="Секция_N";
	HTREEITEM hNew=m_TreeList.InsertItem(Str,hParent,hTree);
	m_TreeList.SetItemText( hNew, 1, csRange1);
	m_TreeList.SetItemText( hNew, 2, csRange2);

	m_TreeList.RedrawWindow();
	m_TreeList.SetFocus();
}

void CSections::OnDel() 
{
	HTREEITEM hTree=m_TreeList.GetFirstSelectedItem();
	if(hTree)
	{
		HTREEITEM hParent=m_TreeList.GetParentItem(hTree);
		if(hParent)
		{
			m_TreeList.DeleteItem(hTree);
		}
	}
	
	m_TreeList.RedrawWindow();
	m_TreeList.SetFocus();
}

static UINT bEditOK=0;
void CSections::OnDblclkTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{

	LV_DISPINFO *plvdi; 
	plvdi = (LV_DISPINFO FAR *) pNMHDR;


	UINT nFlags;
	CPoint point;
	point.x=10;//plvdi->item.iImage;
	point.y=plvdi->item.lParam;
	HTREEITEM hTree=m_TreeList.HitTest(point,&nFlags);
	if(hTree)
		m_TreeList.SelectItem(hTree);

	bEditOK=1;
	m_TreeList.SendMessage(WM_KEYDOWN,VK_F2,0);
	bEditOK=0;
	

	*pResult = 1;
}
void CSections::OnBeginlabeleditTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_DISPINFO* pTVDispInfo = (TV_DISPINFO*)pNMHDR;

	*pResult = 1;
	HTREEITEM hTree=m_TreeList.GetFirstSelectedItem();
	if(hTree)
		if(m_TreeList.GetParentItem(hTree))
		{
			*pResult = 0;
		};
	if(!bEditOK)
		*pResult = 1;
}

void CSections::OnSelchangedTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;

	HTREEITEM hTree=m_TreeList.GetFirstSelectedItem();
	if(hTree)
	{
		HTREEITEM hParent=m_TreeList.GetParentItem(hTree);
		if(hParent)
		{
			int nRange1=atoi(m_TreeList.GetItemText(hTree,1));
			int nRange2=atoi(m_TreeList.GetItemText(hTree,2));
			if(pGrid)
			if(nRange1<=nRange2)
			{
			// Deselect whole table
			pGrid->SelectRange(CGXRange().SetTable(), FALSE);
			if(hParent==hHorizSection)
			{
				pGrid->SelectRange(CGXRange().SetRows(nRange1,nRange2), TRUE);
			}
			else
			if(hParent==hVertSection)
			{
				pGrid->SelectRange(CGXRange().SetCols(nRange1,nRange2), TRUE);
			}
			}
		}
	}

	*pResult = 0;
}

void CSections::OnClickTree1(NMHDR* pNMHDR, LRESULT* pResult) 
{
	LV_DISPINFO *plvdi; 
	plvdi = (LV_DISPINFO FAR *) pNMHDR;



	UINT nFlags;
	TC_HITTESTINFO htinfo;
	GetCursorPos(&htinfo.pt);   // mouse pos
	m_TreeList.ScreenToClient(&htinfo.pt); // to client coords
	htinfo.pt.x=10;
	HTREEITEM hTree=m_TreeList.HitTest(htinfo.pt,&nFlags);
	if(hTree)
		m_TreeList.SelectItem(hTree);
	
	*pResult = 0;
}
void StoreSections(SECTreeCtrl &m_TreeList, HTREEITEM hParent,CArray <CSection,CSection> &cSections)
{
	cSections.RemoveAll();
	HTREEITEM hItem=NULL;
	hItem=m_TreeList.GetChildItem(hParent);
	while(hItem)
	{
		CSection data;
		data.csName=m_TreeList.GetItemText(hItem,0);
		data.nRange1=atoi(m_TreeList.GetItemText(hItem,1));
		data.nRange2=atoi(m_TreeList.GetItemText(hItem,2));
		cSections.Add(data);

		hItem=m_TreeList.GetNextSiblingItem(hItem);
	}
}
void CSections::OnOK() 
{
	HTREEITEM hItemH=m_TreeList.GetRootItem();
	HTREEITEM hItemV=m_TreeList.GetNextSiblingItem(hItemH);
	StoreSections(m_TreeList,hItemH,pGrid->m_Data.HorizSection);
	StoreSections(m_TreeList,hItemV,pGrid->m_Data.VertSection);
	pGrid->SetModifiedFlag(); 


	
	CDialog::OnOK();
}
