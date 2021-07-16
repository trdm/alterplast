//Авторские права - VTOOLS.RU (info@vtools.ru)
// FunctionList.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "FunctionList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CString csUserPath;//каталог пользователя для записи настроек
#define FILE_FLIST_NAME csUserPath+"\\func_list.lst"

extern CImageList *GetImageList();
CValue ValueFromFile(CString csFileName);
void ValueToFile(CValue *Val,CString csFileName);
/////////////////////////////////////////////////////////////////////////////
// CFunctionList dialog


CFunctionList::CFunctionList(CWnd* pParent /*=NULL*/)
	: CDialog(CFunctionList::IDD, pParent)
{
	//{{AFX_DATA_INIT(CFunctionList)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//aListName=0;
	//aListImage=0;
	nLine=0;
	nPrevSort=0;
}


void CFunctionList::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFunctionList)
	DDX_Control(pDX, IDC_CHECK_SORT, m_Sort);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDOK, m_OK);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CFunctionList, CDialog)
	//{{AFX_MSG_MAP(CFunctionList)
	ON_WM_SIZE()
	ON_BN_CLICKED(IDOK, OnOk)
	ON_BN_CLICKED(IDC_CHECK_SORT, OnCheckSort)
	ON_WM_CLOSE()
    ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFunctionList message handlers

void CFunctionList::OnSize(UINT nType, int cx, int cy) 
{
	RECT r2;
	if(IsWindowVisible())
	{
		//Grid
		m_Grid.GetWindowRect(&r2);
		ScreenToClient(&r2);
		r2.bottom=cy;
		r2.right=cx-90;
		m_Grid.MoveWindow(&r2);
		m_Grid.SetColumnWidth(0,r2.right-15);

		int x=cx-90+4;
		m_OK.SetWindowPos	 (NULL, x,14, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Cancel.SetWindowPos(NULL, x,44, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Sort.SetWindowPos	 (NULL, x,74, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	}

}

//поиск номера строки таблицы в исходном массиве (aListName)
//Возвращает номер индекса массива (начиная с 0)
int CFunctionList::GetNumberInArray(int nRow) //nRow - Номер строки с 1
{
	CString csText=m_Grid.GetItemText(nRow,0);
	for(int i=0;i<aListName.GetSize();i++)
	if(csText==aListName.GetAt(i))
	{
		return i;
	}
	return -1;
}
//поиск номера строки массива в таблице
//Возвращает номер строки таблицы (начиная с 1)
int CFunctionList::GetNumberInGrid(int nIndex) //nIndex - индекс в массиве с 0
{
	//поиск текущей строки в таблице
	if(nIndex>0&&nIndex<=aListName.GetSize())
	{
		CString csText=aListName.GetAt(nIndex);
		for(int i=1;i<m_Grid.GetRowCount();i++)
		if(csText==m_Grid.GetItemText(i,0))
		{
			return i;
		}
	}
	return -1;
}

void CFunctionList::OnOk() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	CString csText=m_Grid.GetItemText(curCell.row,curCell.col);
	
	nLine=GetNumberInArray(curCell.row);
	nPrevSort=m_Sort.GetCheck();
	SaveState();

	CDialog::OnOK();
}

BOOL CFunctionList::OnInitDialog() 
{
	CDialog::OnInitDialog();

	LoadState();
	m_Sort.SetCheck(nPrevSort);


#ifdef MYDEBUG
	m_Grid.SetDoubleBuffering(0);
#else
	m_Grid.SetDoubleBuffering(1);
#endif
	m_Grid.SetImageList(GetImageList());
	
	//начальные параметры таблицы
	m_Grid.EnableDragAndDrop(false);
	m_Grid.SetGridLineColor(RGB(255,255,255));
	
	m_Grid.SetFixedColumnSelection(1);
	m_Grid.SetFixedRowSelection(1);
	m_Grid.SetHeaderSort(1);
	m_Grid.SetDefCellHeight(16);
	m_Grid.SetDefCellWidth(210);
	m_Grid.SetSingleRowSelection(1);
	m_Grid.SetListMode(1);
	m_Grid.AutoSizeColumns(1);
	
	m_Grid.SetGridBkColor(RGB(255,255,255));
	m_Grid.m_NotUseMyExtEdit=TRUE;
	m_Grid.SetRowCount(1+aListName.GetSize());
	m_Grid.SetColumnCount(1);
	m_Grid.SetFixedRowCount(1);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetEditable(0);

	LoadGrid();
	
	if(nLine<0)
		nLine=0;
	m_Grid.SetCurrentCell(nLine+1,0);

	SortGrid();

	//поиск текущей строки в таблице
	int nRow=GetNumberInGrid(nLine);
	if(nRow<1)
		nRow=1;
	m_Grid.SetCurrentCell(nRow,0);

	return FALSE;
}

void CFunctionList::LoadGrid()
{
	for(int i=0;i<aListName.GetSize();i++)
	{
		m_Grid.SetItemText(i+1,0,aListName.GetAt(i));
		m_Grid.SetItemImage(i+1,0,aListImage.GetAt(i));
	}
}

void CFunctionList::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID curCell=m_Grid.GetFocusCell();
	if(curCell.row>0)
		OnOk();
}

void CFunctionList::SortGrid() 
{
	if(nPrevSort)
	{
		m_Grid.ResetSelectedRange();
		m_Grid.SortItems(CGridCtrl::pfnCellTextCompare, 0, 1, 0, 0, -1);
	}
}
void CFunctionList::OnCheckSort() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nIndex=GetNumberInArray(curCell.row);

	nPrevSort=m_Sort.GetCheck();
	if(nPrevSort)
		SortGrid();
	else
		LoadGrid();
	//поиск текущей строки в таблице
	int nRow=GetNumberInGrid(nIndex);
	if(nRow>0)
		m_Grid.SetCurrentCell(nRow,0);

	m_Grid.RedrawWindow();
}

void CFunctionList::OnClose() 
{
	// TODO: Add your message handler code here and/or call default
	SaveState();
	CDialog::OnClose();
}

void CFunctionList::OnCancel() 
{
	SaveState();
	CDialog::OnCancel();
}

void CFunctionList::LoadState()
{
	CValue Val=ValueFromFile(FILE_FLIST_NAME);
	nPrevSort=Val.GetNumber();
}
void CFunctionList::SaveState()
{
	CValue Val=nPrevSort;
	ValueToFile(&Val,FILE_FLIST_NAME);
}
