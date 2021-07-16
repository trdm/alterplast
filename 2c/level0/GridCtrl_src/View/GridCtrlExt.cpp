// GridCtrlExt.cpp : implementation file
//

#include "stdafx.h"
#include "GridCtrlExt.h"
#include "../../InputDialog.h"
#include "../../resource.h"
#include "../../MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


#define ROW_HEIGHT		15
#define FIX_COL_WIDTH	40

int IsWindows5();

/////////////////////////////////////////////////////////////////////////////
// CSectionCtrl

CSectionCtrl::CSectionCtrl()
{
	pGrid=0;
}

CSectionCtrl::~CSectionCtrl()
{
}


BEGIN_MESSAGE_MAP(CSectionCtrl, CWnd)
	//{{AFX_MSG_MAP(CSectionCtrl)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CSectionCtrl message handlers
void CSectionCtrl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnDraw(&dc);
}

int CSectionCtrl::FindInSection(int nCurNumber,CString &csStr)
{
	CSectionArray *aSections=GetSectionsArray();

	for(int n=0;n<aSections->GetSize();n++)
	{
		int nRes=0;
		CSection Section=aSections->GetAt(n);
		if(nCurNumber>=Section.nRange1&&nCurNumber<=Section.nRange2)
		{
			nRes +=1;
		}
		if(nCurNumber==Section.nRange1)
		{
			nRes +=2;
		}
		if(nCurNumber==Section.nRange2)
		{
			nRes +=4;
		}
		if(nRes)
		{
			csStr=Section.csName;
			return nRes;
		}
	}
	return 0;
}
void CSectionCtrl::OnDraw(CDC* pDC)
{
	if(!pGrid)
		return;

	if(!pGrid->IsWindowVisible())
	{
		return;
	}

	CCellRange Range=pGrid->GetVisibleNonFixedCellRange();
	if(Range.Count()>0)
	{
		CRect rect;
		GetClientRect(rect);

		CBrush brush(::GetSysColor(COLOR_3DFACE));
		pDC->FillRect(rect, &brush);

		CBrush brush2(::GetSysColor(COLOR_3DHIGHLIGHT));
		pDC->FrameRect(rect, &brush2);
		
	    pDC->SetBkMode(TRANSPARENT);

		CFont Font;
        pDC->SelectObject(pGrid->GetFont());


		CPen *pOldPen = pDC->GetCurrentPen();
		static CPen pen(PS_SOLID, 2, RGB(0,0,0));
		pDC->SelectObject(&pen);

		if(LEFT_MODE==nMode)//LEFT_MODE
		{
			int nCount=min(Range.GetMaxRow()+1,pGrid->GetRowCount());

			rect.bottom=pGrid->GetFixedRowHeight()+1;
			for(int i=Range.GetMinRow();i<nCount;i++)
			{
				rect.top=rect.bottom;
				rect.bottom+=pGrid->GetRowHeight(i);

				CString csStr;
				int nFind=FindInSection(i,csStr);
				if(nFind)
				{
					CRect rect2=rect;
					rect2.left=+3;
					//rect2.top=+1;
					rect2.right-=1;
					rect2.OffsetRect(0,2);

					//рисуем рамку секции
					if(nFind&2)
					{
						DrawText(pDC->m_hDC, csStr, -1, rect2, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX | DT_END_ELLIPSIS | DT_NOPREFIX);
						pDC->MoveTo(rect2.left-1, rect2.top);
						pDC->LineTo(rect2.right, rect2.top);
					}

					pDC->MoveTo(rect2.left, rect2.top);
					pDC->LineTo(rect2.left, rect2.bottom);

					if(nFind&4)
					{
						pDC->MoveTo(rect2.left, rect2.bottom);
						pDC->LineTo(rect2.right, rect2.bottom);
					}
				}
			}
		}
		else
		if(UPPER_MODE==nMode)
		{
			int nCount=min(Range.GetMaxCol()+1,pGrid->GetColumnCount());

			rect.right=pGrid->GetFixedColumnWidth()+1;
			for(int i=Range.GetMinCol();i<nCount;i++)
			{
				rect.left=rect.right;
				rect.right+=pGrid->GetColumnWidth(i);

				CString csStr;
				int nFind=FindInSection(i,csStr);
				if(nFind)
				{
					CRect rect2=rect;
					rect2.top+=2;
					rect2.bottom-=1;

					//рисуем рамку секции
					if(nFind&2)
					{
						DrawText(pDC->m_hDC, csStr, -1, rect2, DT_RIGHT|DT_VCENTER|DT_SINGLELINE|DT_NOPREFIX | DT_END_ELLIPSIS | DT_NOPREFIX);
						pDC->MoveTo(rect2.left, rect2.bottom);
						pDC->LineTo(rect2.left, rect2.top);
					}

					pDC->MoveTo(rect2.left, rect2.top);
					pDC->LineTo(rect2.right, rect2.top);

					if(nFind&4)
					{
						pDC->MoveTo(rect2.right, rect2.top);
						pDC->LineTo(rect2.right, rect2.bottom);
					}
				}
			}
		}
		pDC->SelectObject(pOldPen);
	}
	else
	{
		pGrid->SetColumnCount(20);
		pGrid->SetRowCount(50);
	}
}


BOOL CSectionCtrl::Create(CGridCtrlExt *pSetGrid, CWnd* parent, UINT nID,int nSetMode)
{
    DWORD dwStyle = WS_CHILD | WS_VISIBLE;
	CRect rect(0,0,0,0);
	pGrid=pSetGrid;
	nMode=nSetMode;

	BOOL bRes=CWnd::Create( "STATIC", "", dwStyle|SS_NOTIFY, rect, parent, nID);
	return bRes;
}

int CSectionCtrl::GetSize()
{
	int nCount=0;

	if(nMode==LEFT_MODE)
	{
		nCount=cSection.GetSize();
		return nCount>0?80:0;
	}
	else
	{
		nCount=cSection.GetSize();
		return nCount>0?20:0;
	}

	return 0;
}



int CSectionCtrl::GetNSectionFromPt(CPoint point) 
{
	if(pGrid)
	{
		CCellID idCurrentCell = pGrid->GetCellFromPt(point);
		if(pGrid->IsValid(idCurrentCell))
		{
			CSectionArray *aSections=GetSectionsArray();

			int nCurRow;
			if(LEFT_MODE==nMode)
				nCurRow=idCurrentCell.row;
			else
			if(UPPER_MODE==nMode)
				nCurRow=idCurrentCell.col;

			for(int n=0;n<aSections->GetSize();n++)
			{
				int nRes=0;
				CSection &Section=aSections->GetAt(n);
				if(nCurRow>=Section.nRange1&&nCurRow<=Section.nRange2)
				{
					return n;
				}
			}
		}
	}
	return -1;
}
void CSectionCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(pGrid)
	{
		//pGrid->SetFocus();

		int n=GetNSectionFromPt(point);
		if(n<0)
			return;

		CSectionArray *aSections=GetSectionsArray();
		CSection Section=aSections->GetAt(n);
		if(nMode==LEFT_MODE)
			pGrid->SetSelectedRange(Section.nRange1, 0,
							 Section.nRange2, pGrid->GetColumnCount()-1);
		else
			pGrid->SetSelectedRange(0, Section.nRange1,
						 pGrid->GetRowCount()-1,Section.nRange2);
	}
	//CWnd::OnLButtonDown(nFlags, point);
}

BOOL CSectionCtrl::EditName(int n)
{
	if(n>=0)
	{
		CSectionArray *aSections=GetSectionsArray();
		CSection Section=aSections->GetAt(n);

		CInputDialog dlg;
		dlg.csString=Section.csName;
		dlg.csTitle="Идентификатор секции";
		if(dlg.DoModal()==IDOK)
		{

			Section.csName=dlg.csString;
			aSections->SetAt(n,Section);
			Invalidate();
			return 1;
		}
	}
	return 0;

}
//Редактирование имени идентификатора секции
void CSectionCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(!pGrid)
		return;
	if(!pGrid->IsEditable())
		return;

	int n=GetNSectionFromPt(point);
	EditName(n);
}





void CSectionCtrl::Add(int nRange1,int nRange2)
{
	CSectionArray *aSections=GetSectionsArray();

	for(int n=0;n<aSections->GetSize();n++)
	{
		CSection Section=aSections->GetAt(n);

		if(nRange1>=Section.nRange1&&nRange1<=Section.nRange2)
		if(nRange2>Section.nRange2)//расширение нижней границы
		{
			Section.nRange2=nRange2;
			aSections->SetAt(n,Section);
			return;
		}
		if(nRange2>=Section.nRange1&&nRange2<=Section.nRange2)
		if(nRange1<Section.nRange1)//расширение верхней границы
		{
			Section.nRange1=nRange1;
			aSections->SetAt(n,Section);
			return;
		}

		if(nRange1>=Section.nRange1&&nRange1<=Section.nRange2)
			return;
		if(nRange2>=Section.nRange1&&nRange2<=Section.nRange2)
			return;
	}


	//добавление новой секции
	CSection Section;
	Section.csName="Секция_";
	Section.nRange1=nRange1;
	Section.nRange2=nRange2;
	n=aSections->Add(Section);
	if(!EditName(n))
		aSections->RemoveAt(n);

}
void CSectionCtrl::Remove(int nRange1,int nRange2)
{
	CSectionArray *aSections=GetSectionsArray();

	for(int n=0;n<aSections->GetSize();n++)
	{
		CSection Section=aSections->GetAt(n);
		if(nRange1==Section.nRange1&&nRange2==Section.nRange2)
		{
			//удаление секции
			aSections->RemoveAt(n);
			return;
		}

		if(nRange1>=Section.nRange1&&nRange1<=Section.nRange2)
		if(nRange2>=Section.nRange2)//удаление нижней границы
		{
			Section.nRange2=nRange1-1;
			aSections->SetAt(n,Section);
			return;
		}
		if(nRange2>=Section.nRange1&&nRange2<=Section.nRange2)
		if(nRange1<=Section.nRange1)//удаление верхней границы
		{
			Section.nRange1=nRange2+1;
			aSections->SetAt(n,Section);
			return;
		}

	}

}
void CSectionCtrl::InsertRow(int nRow)
{
	CSectionArray *aSections=GetSectionsArray();
	if(nRow>0)
	{
		for(int n=0;n<aSections->GetSize();n++)
		{
			CSection Section=aSections->GetAt(n);
			if(nRow<Section.nRange1)
				Section.nRange1++;
			if(nRow<=Section.nRange2)
				Section.nRange2++;
			aSections->SetAt(n,Section);
		}

	}
}
void CSectionCtrl::RemoveRow(int nRow)
{
	CSectionArray *aSections=GetSectionsArray();
	if(nRow>0)
	{
		for(int n=0;n<aSections->GetSize();n++)
		{
			CSection Section=aSections->GetAt(n);
			if(nRow<Section.nRange1)
				Section.nRange1--;
			if(nRow<=Section.nRange2)
				Section.nRange2--;
			if(Section.nRange2<Section.nRange1)
			{
				aSections->RemoveAt(n);
				n--;

			}
			else
			{
				aSections->SetAt(n,Section);
			}
		}

	}
}



CPoint CSectionCtrl::GetRange(CString csName)
{
	csName=mUpper(csName);

	CSectionArray *aSections=GetSectionsArray();
	for(int n=0;n<aSections->GetSize();n++)
	{
		CSection Section=aSections->GetAt(n);
		if(csName==mUpper(Section.csName))
			return CPoint(Section.nRange1,Section.nRange2);
	}

	Error(CString("Неправильно задана секция с именем \"")+csName+"\"");

	return CPoint(0,0);
}










/////////////////////////////////////////////////////////////////////////////
// CGridCtrlExt

CGridCtrlExt::CGridCtrlExt()
{
	m_bViewSection=0;
	IsExtMode=1;
	m_NotUseMyExtEdit=1;
	bWasCreate=0;
	pDoc=0;
	m_bHasModified=1;
	EnableWysiwygPrinting(TRUE);
}

CGridCtrlExt::~CGridCtrlExt()
{
}


BEGIN_MESSAGE_MAP(CGridCtrlExt, CGridCtrl)
	//{{AFX_MSG_MAP(CGridCtrlExt)
    ON_WM_SIZE()
	ON_WM_CONTEXTMENU()
    ON_WM_LBUTTONDOWN()
	//ON_WM_RBUTTONDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridCtrlExt message handlers



void CGridCtrlExt::OnDraw(CDC* pDC)
{
    if (!m_bAllowDraw)
        return;

	CCellRange Range=GetVisibleNonFixedCellRange();
	CRect rect;
	GetClientRect(rect);

	m_bHasModified=0;
	if(Range.GetMaxRow()+5>GetRowCount())//подгонка видимых строк
		SetRowCount(GetRowCount()+10);
	if(Range.GetMaxCol()+3>GetColumnCount())//подгонка видимых колонок
		SetColumnCount(GetColumnCount()+6);
	m_bHasModified=1;


	m_Left.SetFont(GetFont(),0);
	m_Upper.SetFont(GetFont(),0);

	CGridCtrl::OnDraw(pDC);
	m_Left.RedrawWindow();
	m_Upper.RedrawWindow();

	CPen *pOldPen = pDC->GetCurrentPen();

	//рисуем красные линии
	if(Range.Count()>0)
	if(m_bViewSection)
	{
		static CPen pen(PS_SOLID, 1, RGB(250,0,0));
		pDC->SelectObject(&pen);

		//горизонтальные
		if(m_Left.GetSize()>0)
		{
			int dt=GetFixedRowHeight()-1;
			int nCount=min(Range.GetMaxRow()+1,GetRowCount());
			for(int i=Range.GetMinRow();i<nCount;i++)
			{
				CString csStr;
				int nFind=m_Left.FindInSection(i,csStr);
				if(nFind&2)
				{
					pDC->MoveTo(GetFixedColumnWidth(), dt);
					pDC->LineTo(rect.right, dt);
				}
				dt+=GetRowHeight(i);
				if(nFind&4)
				{
					pDC->MoveTo(GetFixedColumnWidth(), dt);
					pDC->LineTo(rect.right, dt);
				}
			}
		}
		//вертикальные
		if(m_Upper.GetSize()>0)
		{
			int dt=GetFixedColumnWidth()-1;
			int nCount=min(Range.GetMaxCol()+1,GetColumnCount());
			for(int i=Range.GetMinCol();i<nCount;i++)
			{
				CString csStr;
				int nFind=m_Upper.FindInSection(i,csStr);
				if(nFind&2)
				{
					pDC->MoveTo(dt,GetFixedRowHeight());
					pDC->LineTo(dt,rect.bottom);
				}
				dt+=GetColumnWidth(i);
				if(nFind&4)
				{
					pDC->MoveTo(dt,GetFixedRowHeight());
					pDC->LineTo(dt,rect.bottom);
				}
			}
		}
	}

	//рисуем видимые страницы для принтера
	if(IsExtMode)
	{
		static CPen pen(PS_DOT, 0, RGB(0,0,0));
		pDC->SelectObject(&pen);

		//горизонтальная линия
		int nRow=GetLastRow();
		if(nRow>0)
		if(nRow>=Range.GetMinRow()&&nRow<=Range.GetMaxRow())
		{
			int dt=GetFixedRowHeight()-1;
			for(int i=Range.GetMinRow();i<=nRow;i++)
			{
				dt+=GetRowHeight(i);
			}
			pDC->MoveTo(GetFixedColumnWidth(), dt);
			pDC->LineTo(rect.right, dt);
		}
		//верртикальная линия
		int nCol=GetLastCol();
		if(nCol>0)
		if(nCol>=Range.GetMinCol()&&nCol<=Range.GetMaxCol())
		{
			int dt=GetFixedColumnWidth()-1;
			for(int i=Range.GetMinCol();i<=nCol;i++)
			{
				dt+=GetColumnWidth(i);
			}
			pDC->MoveTo(dt,GetFixedRowHeight());
			pDC->LineTo(dt,rect.bottom);
		}
	}

	pDC->SelectObject(pOldPen);
}

BOOL CGridCtrlExt::Create(const RECT& rect, CWnd* pParentWnd, UINT nID, DWORD dwStyle)
{
	m_Left.Create(this,pParentWnd, nID+1,LEFT_MODE);
	m_Upper.Create(this,pParentWnd, nID+2,UPPER_MODE);
	m_TopLeft.Create(this,pParentWnd, nID+3,NONE_MODE);

	BOOL bRes=CGridCtrl::Create(rect,pParentWnd, nID, dwStyle);
	SetParent(pParentWnd);
	SetRowHeight(0,ROW_HEIGHT);
	SetColumnWidth(0,FIX_COL_WIDTH);
	bWasCreate=1;
	return bRes;
}

BOOL CGridCtrlExt::LoadMXL(CString csBuffer)
{
	if(!m_Data.Load(csBuffer))
		return 0;
	BOOL bRes=LoadMXL(m_Data);

	m_Data.Clear();
	return bRes;
}

void CGridCtrlExt::Clear()
{
	SetColumnCount(1);
	SetRowCount(1); 
	m_Left.cSection.RemoveAll();
	m_Upper.cSection.RemoveAll();
}

BOOL CGridCtrlExt::LoadMXL(CMoxcel &cMxl)
{
	m_bHasModified=0;
	Clear();

	SetColumnCount(cMxl.nAllColumnCount+1);
	SetRowCount(cMxl.nAllRowCount+1);
	if(!cMxl.nAllColumnCount || !cMxl.nAllRowCount)
	{
		m_bHasModified=1;
		return 0;
	}

	//секции
	m_Upper.cSection.Copy(cMxl.VertSection);
	m_Left.cSection.Copy(cMxl.HorizSection);

	int i;
	//ширина колонок
	for(i=0;i<cMxl.nWidthCount;i++)
	{
		int curCol=cMxl.aWidthNumber[i];
		int nWidth=cMxl.aWidth[i];

		SetColumnWidth(curCol,nWidth);
	}
	//объединяем необходимые ячейки
	for(i=0;i<cMxl.nMergeCells*4;i=i+4)
	{
		int curRow1=cMxl.aMergeCells[i+1];
		int curCol1=cMxl.aMergeCells[i];
		int curRow2=cMxl.aMergeCells[i+3];
		int curCol2=cMxl.aMergeCells[i+2];
		SetCoveredCells(curRow1,curCol1,curRow2-curRow1+1,curCol2-curCol1+1);

	}

	//данные
	for(int nRow=0;nRow<cMxl.nRowCount;nRow++)
	{
		int curRow=cMxl.aRowNumber[nRow];
		//if(cMxl.aRow[nRow].nRowHeight)
		//	SetRowHeight(curRow,cMxl.aRow[nRow].nRowHeight);
		for(int nCol=0;nCol<cMxl.aRow[nRow].nColumnCount;nCol++)
		{
			CCell &cell=cMxl.aRow[nRow].aCell[nCol];
			int curCol=cell.nColumn;
			CGridCellBase* pCell = GetCell(curRow, curCol);
			ASSERT(pCell);
			if (!pCell)
				continue;

			if(!pCell->IsCovered())
			{
				int nEnter=cell.csText.Replace("\n","\n")+1;
				if(nEnter>1)
				{//выполняем подгонку размера строки
					CString Str=cell.csText;
					Str.TrimRight();
					nEnter=Str.Replace("\n","\n")+1;

					if(nEnter>1)
					{
						int nFontHeight=14;
						if(cell.nFontHeight)
							nFontHeight=14*cell.nFontHeight/10;

						cMxl.aRow[nRow].nRowHeight=max(nFontHeight*nEnter,cMxl.aRow[nRow].nRowHeight);
					}
				}
			}


			pCell->SetCell(&cell);
		}
		if(cMxl.aRow[nRow].nRowHeight)
			SetRowHeight(curRow,cMxl.aRow[nRow].nRowHeight);
		//else
		//	AutoSizeRow(curRow);

	}

	

	SetLastRowCol(cMxl.nAllRowCount,cMxl.nAllColumnCount,2);
	m_bHasModified=1;
	return 1;
}

BOOL CGridCtrlExt::SaveMXL(CString &csBuffer)
{
	if(!SaveMXL())
		return 0;

	BOOL bRes=m_Data.Save(csBuffer);
	m_Data.Clear();
	return bRes;
}
BOOL CGridCtrlExt::SaveMXL()
{
	m_Data.Clear();

	m_nLastRow=min(m_nLastRow,GetRowCount()-1);
	m_nLastColumn=min(m_nLastColumn,GetColumnCount()-1);
	if(m_nLastRow<1||m_nLastColumn<1)
		return 1;

	//секции
	m_Data.VertSection.Copy(m_Upper.cSection);
	m_Data.HorizSection.Copy(m_Left.cSection);

	m_Data.nAllRowCount=0;
	m_Data.nAllColumnCount=0;

	//объединенные ячейки
	CArray <int,int> aMergeCells;
	for(int nRow=1;nRow <= m_nLastRow;nRow++)
    {
		for(int nCol=1;nCol <= m_nLastColumn;nCol++)
		{
			CGridCellBase* pCell=GetCell(nRow,nCol);
			int nCoveredCol=pCell->GetCoveredCol();
			int nCoveredRow=pCell->GetCoveredRow();
			if(nCoveredCol>0&&nCoveredRow>0)
			{
				aMergeCells.Add(nCol);
				aMergeCells.Add(nRow);
				aMergeCells.Add(nCol+nCoveredCol-1);
				aMergeCells.Add(nRow+nCoveredRow-1);
			}
		}
   }
	m_Data.nMergeCells=aMergeCells.GetSize()/4;
	if(m_Data.nMergeCells)
	{
		m_Data.aMergeCells=new int[aMergeCells.GetSize()];
		memcpy(m_Data.aMergeCells,aMergeCells.GetData(),aMergeCells.GetSize()*sizeof(int));
	}

	//данные
	int nRowCount=0;
	m_Data.nRowCount=GetRowCount();
	m_Data.aRow=new CRow[m_Data.nRowCount];
	m_Data.aRowNumber=new int[m_Data.nRowCount];
	for(nRow=1;nRow < m_Data.nRowCount;nRow++)
	{
		CRow &cRow=m_Data.aRow[nRowCount];

		GRID_ROW* pRow = m_RowData[nRow];
		if (!pRow)
			return 0;

		cRow.nRowHeight=GetRowHeight(nRow);
		int nColCount=0;
		cRow.aCell=new CCell[GetColumnCount()];
		for(int nCol=1;nCol < GetColumnCount();nCol++)
		{
			CGridCellBase* pCell=pRow->GetAt(nCol);
			ASSERT(pCell);
			CCell &cCel=cRow.aCell[nColCount];
			CCell *pData=pCell->GetCell();
			CString csText=pCell->GetText();
			if(pData)
			{
				cCel=*pData;
			}
			else
			if(csText.IsEmpty())
			{
				continue;
			}
			cCel.csText=csText;
			cCel.nColumn=nCol;
			
			int nDeltaRow=0;
			if(pCell->GetCoveredRow())
				nDeltaRow=pCell->GetCoveredRow()-1;
			int nDeltaCol=0;
			if(pCell->GetCoveredCol())
				nDeltaCol=pCell->GetCoveredCol()-1;

			m_Data.nAllRowCount=max(m_Data.nAllRowCount,nRow+nDeltaRow);
			m_Data.nAllColumnCount=max(m_Data.nAllColumnCount,nCol+nDeltaCol);

			nColCount++;
		}

		cRow.nColumnCount=nColCount;
		if(nColCount>0)
		{
			m_Data.aRowNumber[nRowCount]=nRow;

			nRowCount++;
		}
	}
	m_Data.nRowCount=nRowCount;


	//ширина колонок
	m_Data.nWidthCount=m_Data.nAllColumnCount;
	m_Data.aWidth=new int[m_Data.nWidthCount];
	m_Data.aWidthNumber=new int[m_Data.nWidthCount];

	for(int nCol=1;nCol <= m_Data.nWidthCount;nCol++)
	{
		int nWidth=GetColumnWidth(nCol);

		m_Data.aWidthNumber[nCol-1]=nCol;
		m_Data.aWidth[nCol-1]=nWidth;
	}

	return 1;
}


void CGridCtrlExt::ViewSection(int nSet)
{
	if(-1==nSet)
		nSet=m_bViewSection;

	CRect Rect;
	CWnd *pParent=GetParent();
	ASSERT(pParent);
	if(!pParent)
		return;


	pParent->GetClientRect(Rect);

	m_bViewSection=nSet;
	if(m_bViewSection)
	{
		Rect.left+=m_Left.GetSize();
		Rect.top+=m_Upper.GetSize();
	}
	MoveWindow(Rect);

	CRect Rect1(0,0,0,0);
	CRect Rect2(0,0,0,0);
	CRect Rect3(0,0,0,0);
	if(m_bViewSection)
	{
		Rect1.bottom=m_Upper.GetSize();
		Rect1.right=m_Left.GetSize()+2;

		Rect2.top=Rect1.bottom;
		Rect2.right=Rect1.right;
		Rect2.bottom=Rect.bottom;

		Rect3.left=Rect1.right;
		Rect3.right=Rect.right;
		Rect3.bottom=Rect1.bottom;
	}
	m_TopLeft.MoveWindow(Rect1);
	m_Left.MoveWindow(Rect2);
	m_Upper.MoveWindow(Rect3);

	m_TopLeft.ShowWindow(Rect1.Width()>2);
	m_Left.ShowWindow(Rect2.Width()>2);
	m_Upper.ShowWindow(Rect3.Width()>2);
}

void CGridCtrlExt::OnPrint(CDC *pDC, CPrintInfo *pInfo)
{
	int nSize1=GetColumnWidth(0);
	int nSize2=GetRowHeight(0);
	int nStyle=GetGridLines();
	SetColumnWidth(0,0);
	SetRowHeight(0,0);
	SetGridLines(0);
	CGridCtrl::OnPrint(pDC, pInfo);
	SetColumnWidth(0,nSize1);
	SetRowHeight(0,nSize2);
	SetGridLines(nStyle);
}

void CGridCtrlExt::OnSize(UINT nType, int cx, int cy)
{  
	ViewSection();
	CGridCtrl::OnSize(nType, cx, cy);
}


void CGridCtrlExt::ViewHeader(BOOL bSet)
{
	SetFixedRowCount(1);
	SetFixedColumnCount(1);
	if(bSet)
	{
		SetRowHeight(0,ROW_HEIGHT);
		SetColumnWidth(0,FIX_COL_WIDTH);
	}
	else
	{
		SetColumnWidth(0,0);
		SetRowHeight(0,0);
	}
	Invalidate();

}
BOOL CGridCtrlExt::IsViewHeader()
{
	return GetColumnWidth(0)!=0;
}

void CGridCtrlExt::ViewGridLines(BOOL bSet)
{
	if(bSet)
		SetGridLines(GVL_BOTH);
	else
		SetGridLines(0);
}
BOOL CGridCtrlExt::IsViewGridLines()
{
	return GetGridLines()!=0;
}

BOOL CGridCtrlExt::GetWorkSection(int &nRange1,int &nRange2,CSectionCtrl *&pSectionCtrl)
{
	CCellRange Selection=GetSelectedCellRange();
	if(Selection.Count()>0)
	{
		if(Selection.GetRowSpan()<Selection.GetColSpan())//гориз. секции
		{
			pSectionCtrl=&m_Left;
			nRange1=Selection.GetMinRow();
			nRange2=Selection.GetMaxRow();
		}
		else//верт. секции
		{
			pSectionCtrl=&m_Upper;
			nRange1=Selection.GetMinCol();
			nRange2=Selection.GetMaxCol();
		}
		return TRUE;
	}
	return FALSE;
}

void CGridCtrlExt::AddSection()
{
	if(!IsEditable())
		return;
	int nRange1,nRange2;
	CSectionCtrl *pSectionCtrl;
	if(!GetWorkSection(nRange1,nRange2,pSectionCtrl))
		return;


	pSectionCtrl->Add(nRange1,nRange2);
    SetModified(TRUE);

	ViewSection();
	Invalidate();
}
void CGridCtrlExt::RemoveSection()
{
	if(!IsEditable())
		return;

	int nRange1,nRange2;
	CSectionCtrl *pSectionCtrl;
	if(!GetWorkSection(nRange1,nRange2,pSectionCtrl))
		return;


	pSectionCtrl->Remove(nRange1,nRange2);
    SetModified(TRUE);

	ViewSection();
	Invalidate();
}
//установка картинки меню
#define SetMBitmap(x,y)\
{\
static CBitmap *pBitmap##x=NULL;\
if(!pBitmap##x)\
{\
	pBitmap##x=new CBitmap();\
	pBitmap##x->LoadBitmap(y);\
}\
pPopup->SetMenuItemBitmaps(x,MF_BYCOMMAND,pBitmap##x,pBitmap##x);\
} 

void CGridCtrlExt::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	CWnd* pWndPopupOwner = GetParent();
	VERIFY(menu.LoadMenu(IDR_GRID_CONTEXT));
	CMenu* pPopup = menu.GetSubMenu(0);
	ASSERT(pPopup != NULL); 

	SetMBitmap(ID_EDIT_COPY,IDB_EDIT_COPY)
	SetMBitmap(ID_EDIT_CUT,IDB_EDIT_CUT)
	SetMBitmap(ID_EDIT_PASTE,IDB_EDIT_PASTE)
	SetMBitmap(ID_GRID_SECTION,IDB_EDIT_SECTIONS) 

//устанавливаем галочки
	if(IsViewGridLines())
		pPopup->CheckMenuItem(ID_GRID_SETKA,MF_BYCOMMAND|MF_CHECKED);
	if(IsViewHeader())
		pPopup->CheckMenuItem(ID_GRID_HEADER,MF_BYCOMMAND|MF_CHECKED);
	if(!IsEditable())
		pPopup->CheckMenuItem(ID_GRID_READONLY,MF_BYCOMMAND|MF_CHECKED);

	int nRes=pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON , point.x, point.y,
		pWndPopupOwner);
	pPopup->DestroyMenu(); 

}


int CGridCtrlExt::InsertColumn(LPCTSTR strHeading, UINT nFormat,int nColumn)
{
	m_Upper.InsertRow(nColumn);
	return CGridCtrl::InsertColumn(strHeading, nFormat,nColumn);
}
int  CGridCtrlExt::InsertRow(LPCTSTR strHeading, int nRow)
{
	m_Left.InsertRow(nRow);
	return CGridCtrl::InsertRow(strHeading, nRow);
}
BOOL CGridCtrlExt::DeleteColumn(int nColumn)
{
	m_Upper.RemoveRow(nColumn);
	return CGridCtrl::DeleteColumn(nColumn);
}
BOOL CGridCtrlExt::DeleteRow(int nRow)
{
	m_Left.RemoveRow(nRow);
	return CGridCtrl::DeleteRow(nRow);
}









#define ID_GRID_BUFFER	0x1234567
// Copies text from the selected cells to the clipboard
COleDataSource* CGridCtrlExt::CopyTextFromGrid()
{
//	return CGridCtrl::CopyTextFromGrid();

    USES_CONVERSION; 

    CCellRange Selection = GetSelectedCellRange();
    if (!IsValid(Selection))
        return NULL;

    if (GetVirtualMode())
        SendCacheHintToParent(Selection);

    // Write to shared file (REMEBER: CF_TEXT is ANSI, not UNICODE, so we need to convert)
    CSharedFile sf(GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);

    // Get a tab delimited string to copy to cache
    CString str;
    CGridCellBase *pCell;
	CArray <CCell*,CCell*> aCells;
    for (int row = Selection.GetMinRow(); row <= Selection.GetMaxRow(); row++)
    {
        // don't copy hidden cells
        if( m_arRowHeights[row] <= 0 )
            continue;

        str.Empty();
        for (int col = Selection.GetMinCol(); col <= Selection.GetMaxCol(); col++)
        {
            // don't copy hidden cells
            if( m_arColWidths[col] <= 0 )
                continue;

            pCell = GetCell(row, col);
            if (pCell &&(pCell->GetState() & GVIS_SELECTED))
            {
                // if (!pCell->GetText())
                //    str += _T(" ");
                // else 
                str += pCell->GetText();

				//*****************************************************
				CCell *p=pCell->GetCell();
				if(!p)
				{
					CCell c;
					c.csText=pCell->GetText();
					pCell->SetCell(&c);
					p=pCell->GetCell();
				}
				p->nColumn=col-Selection.GetMinCol();
				p->nRow=row-Selection.GetMinRow();
				aCells.Add(p);

				p->nCoveredRow=pCell->GetCoveredRow();
				p->nCoveredCol=pCell->GetCoveredCol();
				//*****************************************************
            }
            if (col != Selection.GetMaxCol()) 
                str += _T("\t");
        }
        if (row != Selection.GetMaxRow()) 
            str += _T("\n");
        
        sf.Write(T2A(str.GetBuffer(1)), str.GetLength());
        str.ReleaseBuffer();
    }
    
    char c = '\0';
    sf.Write(&c, 1);


	int nCount=aCells.GetSize();
	if(nCount>0)
	{
		//*****************************************************
		int nID=ID_GRID_BUFFER;
		sf.Write(&nID,sizeof(int));
		//*****************************************************
		//дополнительная информация по ячейкам (цвет, рамка...) - Копирование
		int nCount1=0;
		if(1==Selection.GetMinCol()&&Selection.GetMaxCol()==GetColumnCount()-1)
			nCount1=Selection.GetRowSpan();
		int nCount2=0;
		if(1==Selection.GetMinRow()&&Selection.GetMaxRow()==GetRowCount()-1)
			nCount2=Selection.GetColSpan();
		int nAllRowCol=0;
		if(nCount1&&nCount2)
		{
			nAllRowCol=1;//признак полного копирования таблицы
		}
		sf.Write(&nAllRowCol,sizeof(int));
		//информация о секциях (при полном копировании таблицы)
		if(nAllRowCol)
		{
			m_Left.save(sf);
			m_Upper.save(sf);
		}

		sf.Write(&nCount,sizeof(int));
		if(nCount)//заранее выделяем примерно необходимое количество памяти
		{
			int n=sf.GetPosition();
			sf.SetLength(n+nCount*(sizeof(CCell)+50));
			sf.Seek(n,CFile::begin);
		}
		for(int i=0;i<nCount;i++)
		{
			aCells[i]->save(sf); 
		}

		//информация о высоте строк
		sf.Write(&nCount1,sizeof(int));
		if(nCount1)
		for(int i=Selection.GetMinRow();i<=Selection.GetMaxRow();i++)
		{
			int nRow=i-Selection.GetMinRow();
			int nHeight=GetRowHeight(i);
			sf.Write(&nRow,sizeof(int));
			sf.Write(&nHeight,sizeof(int));
		}
		//информация о ширине колонок
		sf.Write(&nCount2,sizeof(int));
		if(nCount2)
		for(int i=Selection.GetMinRow();i<=Selection.GetMaxRow();i++)
		{
			int nCol=i-Selection.GetMinCol();
			int nWidth=GetColumnWidth(i);
			sf.Write(&nCol,sizeof(int));
			sf.Write(&nWidth,sizeof(int));
		}
		int nPos=sf.GetPosition();
		sf.SetLength(nPos);//обрезаем лишнюю длину
		//*****************************************************
	}

    if (GetVirtualMode())
        SendCacheHintToParent(CCellRange(-1,-1,-1,-1));

    DWORD dwLen = (DWORD) sf.GetLength();
    HGLOBAL hMem = sf.Detach();
    if (!hMem)
        return NULL;

    hMem = ::GlobalReAlloc(hMem, dwLen, GMEM_MOVEABLE | GMEM_DDESHARE | GMEM_ZEROINIT);
    if (!hMem)
        return NULL;

    // Cache data
    COleDataSource* pSource = new COleDataSource();
    pSource->CacheGlobalData(CF_TEXT, hMem);

    return pSource;
}


// Pastes text from the clipboard to the selected cells
BOOL CGridCtrlExt::PasteTextToGrid(CCellID cell, COleDataObject* pDataObject)
{
    if (!IsValid(cell) || !IsCellEditable(cell) || !pDataObject->IsDataAvailable(CF_TEXT))
        return FALSE;

    // Get the text from the COleDataObject
    HGLOBAL hmem = pDataObject->GetGlobalData(CF_TEXT);
    CMemFile sf((BYTE*) ::GlobalLock(hmem), ::GlobalSize(hmem));

    // CF_TEXT is ANSI text, so we need to allocate a char* buffer
    // to hold this.
    LPSTR szBuffer = new char[::GlobalSize(hmem)];
    if (!szBuffer)
        return FALSE;

	int nGlobalSize=::GlobalSize(hmem);
    sf.Read(szBuffer, nGlobalSize);
    ::GlobalUnlock(hmem);

    // Now store in generic TCHAR form so we no longer have to deal with
    // ANSI/UNICODE problems
    CString strText = szBuffer;

	//*****************************************************
	//дополнительная информация по ячейкам (цвет, рамка...) - Вставка
	int nIndex=strText.GetLength();
	BOOL bMyCopy=0;
	if(nGlobalSize>nIndex+5)
	{
		bMyCopy=1;
	}
//	sf.Seek(nIndex+1,CFile::begin);
//	char buf[100];
//	sf.Read(&buf,sizeof(buf));


	strText.LockBuffer();
	sf.Seek(nIndex+1,CFile::begin);

	int nID;
	sf.Read(&nID,sizeof(int));
	if(nID!=ID_GRID_BUFFER)
	{
		bMyCopy=0;
	}


	if(!bMyCopy)
	{
		delete szBuffer;
		strText.UnlockBuffer();
		return CGridCtrl::PasteTextToGrid(cell,pDataObject);
	}


    SendMessageToParent(cell.row, cell.col, GVN_BEGINPASTE);    // pjp

	int nAllRowCol=0;//признак полного копирования таблицы
	sf.Read(&nAllRowCol,sizeof(int));
	if(nAllRowCol)
	{
		cell.col=1;
		cell.row=1;
		
		Clear();
		SetColumnCount(20);
		SetRowCount(50);
		
		//информация о секциях
		m_Left.load(sf);
		m_Upper.load(sf);
	}
	
	int nCount=0;//количество скопированных ячеек
	sf.Read(&nCount,sizeof(int));
	for(int i=0;i<nCount;i++)
	{
		CCell cCell;
		cCell.load(sf);
		int nRow=cell.row+cCell.nRow;
		int nCol=cell.col+cCell.nColumn;
		
		if(nRow+cCell.nCoveredRow>=GetRowCount())
			SetRowCount(nRow+cCell.nCoveredRow+1);
		if(nCol+cCell.nCoveredCol>=GetColumnCount())
			SetColumnCount(nCol+cCell.nCoveredCol+1);
		
		SetItemCell(nRow,nCol,&cCell);
		
		if(cCell.nCoveredRow)
		{
			SetCoveredCells(nRow,nCol,cCell.nCoveredRow,cCell.nCoveredCol);
		}
		SetItemText(nRow,nCol,cCell.csText);
		SetLastRowCol(nRow+cCell.nCoveredRow-1,nCol+cCell.nCoveredCol-1);
	}
	//информация о высоте строк
	sf.Read(&nCount,sizeof(int));
	for(i=0;i<nCount;i++)
	{
		int nRow;
		int nHeight;
		sf.Read(&nRow,sizeof(int));
		sf.Read(&nHeight,sizeof(int));
		nRow+=cell.row;
		if(nRow<GetRowCount())
			SetRowHeight(nRow,nHeight);
	}
	//информация о ширине колонок
	sf.Read(&nCount,sizeof(int));
	for(i=0;i<nCount;i++)
	{
		int nCol;
		int nWidth;
		sf.Read(&nCol,sizeof(int));
		sf.Read(&nWidth,sizeof(int));
		nCol+=cell.col;
		if(nCol<GetColumnCount())
			SetColumnWidth(nCol,nWidth);
	}
	//*****************************************************


	delete szBuffer;
	strText.UnlockBuffer();
	ViewSection();
	Refresh();


    SendMessageToParent(cell.row, cell.col, GVN_ENDPASTE);      // pjp

    return TRUE;
}

CWnd* CGridCtrlExt::SetParent(CWnd *pWnd )
{
	m_Left.SetParent(pWnd);
	m_Upper.SetParent(pWnd);
	m_TopLeft.SetParent(pWnd);

	CWnd* pRes=CGridCtrl::SetParent(pWnd);
	ViewSection(1);


	SetEditable(TRUE);
	EnableDragAndDrop(TRUE);

	SetDefCellHeight(ROW_HEIGHT);
	//SetDefCellWidth(FIX_COL_WIDTH);
	//SetRowCount(50);
	//SetColumnCount(8);
	SetFixedRowCount(1);
	SetFixedColumnCount(1);
	//SetColumnWidth(0,FIX_COL_WIDTH);

	return pRes;
}
void CGridCtrlExt::ShowProperty(bool bShow) 
{
	if((!AfxGetProperty()) || (afxAppRunMode!=CONFIG_MODE))
		return;


	if(bShow==0&&AfxGetProperty()->IsWindowVisible()==0)
		return;

	if(AfxGetProperty()->IsWindowVisible()==0&&bShow==1)
	{
		((CMainFrame*)AfxGetMainWnd())->ShowPropertyBar();
	}

/*	if(!IsEditable())
	{
		AfxGetProperty()->ReleaseDoc(pDoc);
		return;
	}
*/
	if(IsValid(m_idCurrentCell))
	{
		CGridCellBase *p=GetCell(m_idCurrentCell.row,m_idCurrentCell.col);
		CCell *pObj=p->GetCell();
		if(!pObj)
		{	
			CCell cel;
			p->SetCell(&cel);
			pObj=p->GetCell();
		}
		SetOrigCell(pObj);//для последующего сравнения

		AfxGetProperty()->Clear();
		AfxGetProperty()->SetDoc(pDoc,(LPARAM)pObj,(LPARAM)this,!IsEditable());
		AfxGetProperty()->AddProperty("Основные","Текст",pObj->csText);
		AfxGetProperty()->AddProperty("Основные","Расшифровка",pObj->csFormula);
		AfxGetProperty()->AddProperty("Основные","Формат",pObj->csFormat);
		AfxGetProperty()->AddProperty("Основные","Маска",pObj->csMaska);

		AfxGetProperty()->AddProperty("Основные","Защита",pObj->bProtect);
		AfxGetProperty()->AddProperty("Основные","Тип",pObj->nMode,"Текст,Выражение,Шаблон,Фикс.шаблон");
		AfxGetProperty()->AddProperty("Основные","Контроль",pObj->nControl,"Авто,Обрезать,Забивать,Переносить,Красный,Забивать+Красный");

		AfxGetProperty()->AddProperty("Положение","Гориз. полож",pObj->nHPosition,"Лево,Право,По ширине,Центр");
		AfxGetProperty()->AddProperty("Положение","Верт. полож",pObj->nVPosition,"Верх,Низ,Центр");

		AfxGetProperty()->AddProperty("Шрифт","Жирность",pObj->nBold);
		AfxGetProperty()->AddProperty("Шрифт","Наклонность",pObj->nItalic);
		AfxGetProperty()->AddProperty("Шрифт","Подчеркивание",pObj->nUnderLine);
		AfxGetProperty()->AddProperty("Шрифт","Размер шрифта",pObj->nFontHeight);
		AfxGetProperty()->AddProperty("Шрифт","Цвет",pObj->nFontColor);
		AfxGetProperty()->AddProperty("Шрифт","Цвет фона",pObj->nBackgroundColor);
		

		AfxGetProperty()->AddProperty("Рамка","Цвет",pObj->nRamkaColor);
		AfxGetProperty()->AddProperty("Рамка","Слева",pObj->nRamkaL,"Нет,Точки,Тонкая,Толстая");
		AfxGetProperty()->AddProperty("Рамка","Справа",pObj->nRamkaR,"Нет,Точки,Тонкая,Толстая");
		AfxGetProperty()->AddProperty("Рамка","Сверху",pObj->nRamkaU,"Нет,Точки,Тонкая,Толстая");
		AfxGetProperty()->AddProperty("Рамка","Снизу",pObj->nRamkaD,"Нет,Точки,Тонкая,Толстая");
	}

}
CCellID CGridCtrlExt::SetFocusCell(CCellID cell)
{
	CCellID ret=CGridCtrl::SetFocusCell(cell);
	ShowProperty(0);
/* 
	CRect Rect1(0,0,0,0);
	CRect Rect2(0,0,0,0);
	if(cell.row>1&&cell.col>1)
		GetCellRect(cell.row-1,cell.col-1,Rect1);
	if(cell.row+1<GetRowCount()&&cell.col+1<GetColumnCount())
		GetCellRect(cell.row+1,cell.col+1,Rect2);

	Rect1.left=min(Rect1.left,Rect2.left);
	Rect1.top=min(Rect1.top,Rect2.top);
	Rect1.right=max(Rect1.right,Rect2.right);
	Rect1.bottom=max(Rect1.bottom,Rect2.bottom);
	InvalidateRect(Rect1);
*/	
	return ret;
}

void CGridCtrlExt::SetModified(BOOL bModified /*=TRUE*/, int nRow /*=-1*/, int nCol /*=-1*/)
{
	CGridCtrl::SetModified(bModified,nRow,nCol);
	if(pDoc&&m_bHasModified)
		pDoc->SetModifiedFlag(bModified);
}

void CGridCtrlExt::OnLButtonDown(UINT nFlags, CPoint point)
{
	if(AfxGetProperty())
		AfxGetProperty()->EndEdit();
	CGridCtrl::OnLButtonDown(nFlags, point);
}