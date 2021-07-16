// ChangeOrder.cpp : implementation file
//

#include "stdafx.h"
#include "../Resource.h"
#include "ChangeOrder.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "FormEditor.h"
/////////////////////////////////////////////////////////////////////////////
// CChangeOrder dialog


CChangeOrder::CChangeOrder(CWnd* pParent /*=NULL*/)
	: CDialog(CChangeOrder::IDD, pParent)
{

	//{{AFX_DATA_INIT(CChangeOrder)
	//}}AFX_DATA_INIT
	pEditor=0;
	nCancel=0;
}

void CChangeOrder::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CChangeOrder)
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_GRID1, m_Grid); 
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CChangeOrder, CDialog)
	//{{AFX_MSG_MAP(CChangeOrder)
	ON_COMMAND(ID_UP, OnUpElement)
	ON_COMMAND(ID_DOWN, OnDownElement)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CChangeOrder message handlers

BOOL CChangeOrder::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
#ifdef MYDEBUG
	m_Grid.SetDoubleBuffering(0);
#else
	m_Grid.SetDoubleBuffering(1);
#endif

	//начальные параметры таблицы
	m_Grid.EnableDragAndDrop(false);
	m_Grid.SetGridLineColor(RGB(255,255,255));
	
	m_Grid.SetFixedColumnSelection(0);
	m_Grid.SetFixedRowSelection(1);
	m_Grid.SetDefCellHeight(16);
	m_Grid.SetSingleRowSelection(0);
	m_Grid.EnableSelection(0);
	
	m_Grid.SetGridBkColor(RGB(255,255,255));
	m_Grid.m_NotUseMyExtEdit=TRUE;
	m_Grid.SetFixedRowCount(1);
	//m_Grid.SetRowCount(1+aListName.GetSize());
	m_Grid.SetColumnCount(4);
	
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetEditable(0);
	m_Grid.SetColumnWidth(0,100);
	m_Grid.SetColumnWidth(1,100);
	m_Grid.SetColumnWidth(2,120);
	m_Grid.SetColumnWidth(3,120);
	m_Grid.SetFocusCell(1,2);

	ReLoadGrid();
	m_ToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS |
			CBRS_FLYBY | CBRS_BORDER_BOTTOM|TBSTYLE_DROPDOWN,AFX_IDW_TOOLBAR);//надо изменить AFX_IDW_TOOLBAR
	m_ToolBar.LoadToolBar(IDR_CHANGEORDER);
		// Положение панелей
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);
		
	return FALSE;
}

void CChangeOrder::AttachEditor(class CFormEditor *pSet)
{
	pEditor=pSet;
}

void CChangeOrder::OnUpElement() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row-1;
	if(nLine>0)
	{
		CChangeOrderItem data=aList[nLine];
		aList.RemoveAt(nLine);
		aList.InsertAt(nLine-1,data);
		ReLoadGrid();
		curCell.row-=1;
		m_Grid.SetFocusCell(curCell);
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CChangeOrder::OnDownElement() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row-1;
	if(nLine+1<aList.GetSize())
	{
		CChangeOrderItem data=aList[nLine];
		aList.RemoveAt(nLine);
		aList.InsertAt(nLine+1,data);
		ReLoadGrid();
		curCell.row+=1;
		m_Grid.SetFocusCell(curCell);
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CChangeOrder::ReLoadGrid()
{

	CCellID curCell=m_Grid.GetFocusCell();
	m_Grid.SetRowCount(1);
	m_Grid.SetRowCount(aList.GetSize()+1);
	m_Grid.SetItemText(0,0,"Слой");
	m_Grid.SetItemText(0,1,"Тип");
	m_Grid.SetItemText(0,2,"Заголовок");
	m_Grid.SetItemText(0,3,"Идентификатор");
	//m_Grid.SetFixedBkColor(COLOR_MENUTEXT);
	
	for(int i=0;i<aList.GetSize();i++)
	{
		m_Grid.SetItemText(i+1,0,aList[i].csLayer);
		m_Grid.SetItemText(i+1,1,GetType((int)Number(CValue(aList[i].csType)).GetNumber()));
		m_Grid.SetItemText(i+1,2,aList[i].csCapture);
		m_Grid.SetItemText(i+1,3,aList[i].csId);
	}

	m_Grid.SetFocusCell(curCell);
}

CString CChangeOrder::GetType(int nType)
{
	CString Ret;
	if(nType==ID_DYNBUTTON)
		Ret = "Кнопка";
	else if(nType==ID_DYNBUTTONGRID)
		Ret = "Grid";
	else if(nType==ID_DYNBUTTONTREE)
		Ret = "Дерево";
	else if(nType==ID_DYNBUTTONEDIT)
		Ret = "Поле ввода";
	else if(nType==ID_DYNBUTTONLABEL)
		Ret = "Текст";
	else if(nType==ID_DYNBUTTONCOMBO)
		Ret = "Поле со списком";
	else if(nType==ID_DYNBUTTONLISTBOX)
		Ret = "Список";
	else if(nType==ID_DYNBUTTONGROUP)
		Ret = "Рамка";
	else if(nType==ID_DYNBUTTONCHECK)
		Ret = "Флажок";
	else if(nType==ID_DYNBUTTONRADIO)
		Ret = "Радиокнопка";
	else if(nType==ID_DYNBUTTONPROGRESS)
		Ret = "Прогресс";
	else if(nType==ID_DYNBUTTONSLIDER)
		Ret = "Ползунок";
	else if(nType==ID_DYNBUTTONFORMOBJECT)
		Ret = "Микроформа";
	else
		Ret = CString(CValue(nType));
	return Ret;
}
