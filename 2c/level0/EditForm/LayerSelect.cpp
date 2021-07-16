// LayerSeclect.cpp : implementation file
//

#include "stdafx.h"
#include "LayerSelect.h"
#include "../Resource.h"
#include "../InputDialog.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CImageList *GetImageList();
#include "FormEditor.h"

/////////////////////////////////////////////////////////////////////////////
// CLayerSelect dialog


CLayerSelect::CLayerSelect(CWnd* pParent /*=NULL*/)
	: CDialog(CLayerSelect::IDD, pParent)
{
	//{{AFX_DATA_INIT(CLayerSelect)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT

	nCurrentEdit=0;
	pEditor=0;
}


void CLayerSelect::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CLayerSelect)
	DDX_Control(pDX, IDHELP, m_Help);
	DDX_Control(pDX, IDCANCEL, m_Cancel);
	DDX_Control(pDX, IDC_GRID, m_Grid);             // associate the grid window with a C++ object
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CLayerSelect, CDialog)
	//{{AFX_MSG_MAP(CLayerSelect)
	ON_WM_SIZE()
	ON_COMMAND(ID_NEW_LAYER, OnNewLayer)
	ON_COMMAND(ID_EDIT_LAYER, OnEditLayer)
	ON_COMMAND(ID_DEL_LAYER, OnDelLayer)
	ON_COMMAND(ID_UP_LAYER, OnUpLayer)
    ON_NOTIFY(NM_DBLCLK, IDC_GRID, OnGridDblClick)
	ON_COMMAND(ID_DOWN_LAYER, OnDownLayer)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CLayerSelect message handlers


void CLayerSelect::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	RECT r2;
	if(IsWindowVisible())
	{
		//Grid
		m_Grid.GetWindowRect(&r2);
		ScreenToClient(&r2);
		r2.bottom=cy-8;
		r2.right=cx-90-7;
		m_Grid.MoveWindow(&r2);
		int nWidth=r2.right-55;
		if(nWidth<50)
			nWidth=50;
		m_Grid.SetColumnWidth(2,nWidth);

		int x=cx-90+5;
		m_Cancel.SetWindowPos(NULL, x,37, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
		m_Help.SetWindowPos	 (NULL, x,60, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
	}

}

void CLayerSelect::OnCancel() 
{
	// TODO: Add extra cleanup here
	
	CDialog::OnCancel();
}

BOOL CLayerSelect::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
#ifdef MYDEBUG
	m_Grid.SetDoubleBuffering(0);
#else
	m_Grid.SetDoubleBuffering(1);
#endif
	m_Grid.SetImageList(GetImageList());
	
	//начальные параметры таблицы
	m_Grid.EnableDragAndDrop(false);
	m_Grid.SetGridLineColor(RGB(255,255,255));
	
	m_Grid.SetFixedColumnSelection(0);
	m_Grid.SetFixedRowSelection(0);
	m_Grid.SetDefCellHeight(16);
	m_Grid.SetSingleRowSelection(0);
	m_Grid.EnableSelection(0);
	
	m_Grid.SetGridBkColor(RGB(255,255,255));
	m_Grid.m_NotUseMyExtEdit=TRUE;

	//m_Grid.SetRowCount(1+aListName.GetSize());
	m_Grid.SetColumnCount(3);
	m_Grid.SetFixedRowCount(0);
	m_Grid.SetFixedColumnCount(0);
	m_Grid.SetEditable(0);
	m_Grid.SetColumnWidth(0,20);
	m_Grid.SetColumnWidth(1,20);
	m_Grid.SetColumnWidth(2,220);

	ReLoadGrid();
	
	m_ToolBar.Create(this, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_TOOLTIPS |
		CBRS_FLYBY | CBRS_BORDER_BOTTOM|TBSTYLE_DROPDOWN,AFX_IDW_TOOLBAR);//надо изменить AFX_IDW_TOOLBAR
	m_ToolBar.LoadToolBar(IDR_LAYER);

	
	// Положение панелей
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

	return FALSE;
}


void CLayerSelect::OnGridDblClick(NMHDR *pNotifyStruct, LRESULT* /*pResult*/)
{
    NM_GRIDVIEW* pItem = (NM_GRIDVIEW*) pNotifyStruct;
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row;
	if(nLine<0 || nLine>=aList.GetSize())
		return;

	if(0==curCell.col)
		aList[nLine].bVisible=!aList[nLine].bVisible;

	if(1==curCell.col)
		nCurrentEdit=nLine;

	if(2==curCell.col)
	{
		for(int i=0;i<aList.GetSize();i++)
		{
			aList[i].bVisible=0;
		}
		nCurrentEdit=nLine;
	}

	ReLoadGrid();

	ASSERT(pEditor);
	pEditor->SetModifiedFlag(1);
	if(pEditor->m_pDialog)
		pEditor->m_pDialog->SendMessage(WM_ENDSELECT);

}

void CLayerSelect::OnNewLayer() 
{
	CInputDialog dlg;
	dlg.csString="Слой1";
	dlg.csTitle="Идентификатор слоя:";
	if(dlg.DoModal()==IDOK)
	{
		CLayerItem data;
		data.csName=dlg.csString;
		data.bVisible=1;
		nCurrentEdit=aList.Add(data);

		ReLoadGrid();
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CLayerSelect::OnEditLayer() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row;
	if(nLine<0 || nLine>=aList.GetSize())
		return;

	CLayerItem data=aList[nLine];
	CInputDialog dlg;
	dlg.csString=data.csName;
	dlg.csTitle="Идентификатор слоя:";
	if(dlg.DoModal()==IDOK)
	{
		data.csName=dlg.csString;
		aList[nLine]=data;
		ReLoadGrid();
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CLayerSelect::OnDelLayer() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row;
	if(nLine<0 || nLine>=aList.GetSize())
		return;

	CString Str;
	Str.Format("Удалить слой %s ?",aList[nLine].csName);
	int nRes=MessageBox(Str,"Конфигуратор 2С",MB_ICONQUESTION | MB_YESNO);
	if(6==nRes)
	{
		aList.RemoveAt(nLine);
		ReLoadGrid();
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CLayerSelect::OnUpLayer() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row;
	if(nLine>0)
	{
		CLayerItem data=aList[nLine];
		aList.RemoveAt(nLine);
		aList.InsertAt(nLine-1,data);
		ReLoadGrid();
		curCell.row-=1;
		m_Grid.SetFocusCell(curCell);
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}

void CLayerSelect::OnDownLayer() 
{
	CCellID curCell=m_Grid.GetFocusCell();
	int nLine=curCell.row;
	if(nLine+1<aList.GetSize())
	{
		CLayerItem data=aList[nLine];
		aList.RemoveAt(nLine);
		aList.InsertAt(nLine+1,data);
		ReLoadGrid();
		curCell.row+=1;
		m_Grid.SetFocusCell(curCell);
		ASSERT(pEditor);
		pEditor->SetModifiedFlag(1);
	}
}


CString CLayerSelect::GetCurrentName()
{
	if(nCurrentEdit<0||nCurrentEdit>=aList.GetSize())
		nCurrentEdit=0;
	if(0==aList.GetSize())
	{
		CLayerItem data;
		data.csName=DEFAULT_LAYER_NAME;
		data.bVisible=1;
		aList.Add(data);
	}

	return aList[nCurrentEdit].csName;
}

void CLayerSelect::ReLoadGrid()
{
	GetCurrentName();
	CCellID curCell=m_Grid.GetFocusCell();
	m_Grid.SetRowCount(0);
	m_Grid.SetRowCount(aList.GetSize());

	aList[nCurrentEdit].bVisible=1;
	for(int i=0;i<aList.GetSize();i++)
	{
		m_Grid.SetItemText(i,2,aList[i].csName);
		if(aList[i].bVisible)
			m_Grid.SetItemImage(i,0,608);
	}

	m_Grid.SetItemImage(nCurrentEdit,1,609);
	m_Grid.SetFocusCell(curCell);
}

void CLayerSelect::AddLayer(CString csName0,BOOL bVisible,BOOL bActivate)
{
	CString csName=csName0;
	csName.MakeUpper();
	for(int i=0;i<aList.GetSize();i++)
	{
		CLayerItem data=aList[i];
		CString csCurName=data.csName;
		csCurName.MakeUpper();
		if(csCurName==csName)
		{
			data.bVisible=bVisible;
			aList[i]=data;
			if(bActivate)
				nCurrentEdit=i;
			return;
		}
	}

	//еще нет такого слоя - добавляем
	CLayerItem data;
	data.csName=csName0;
	data.bVisible=bVisible;
	i=aList.Add(data);
	if(bActivate)
		nCurrentEdit=i;
}

void CLayerSelect::AttachEditor(class CFormEditor *pSet)
{
	pEditor=pSet;
}

BOOL CLayerSelect::IsVisibleLayer(CString csName)
{
	csName.MakeUpper();
	for(int i=0;i<aList.GetSize();i++)
	{
		CLayerItem data=aList[i];
		CString csCurName=data.csName;
		csCurName.MakeUpper();
		if(csCurName==csName)
		{
			return data.bVisible;
		}
	}
	return 0;
}

CString CLayerSelect::GetStrLayers()
{
	CString Str;
	GetCurrentName();
	for(int i=0;i<aList.GetSize();i++)
	{
		Str=Str+","+aList[i].csName;
	}
	Str.TrimLeft(',');
	return Str;

}

