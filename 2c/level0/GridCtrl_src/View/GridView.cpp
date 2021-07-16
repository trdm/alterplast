// GridView.cpp : implementation of the CGridView class
//

#include "stdafx.h"
#include "GridView.h"

#include "GridDoc.h"
#include "GridView.h"

#include "../../MainFrm.h"
#include "../../MetadataTree.h"
#include "../../InputNumeric.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CGridView

IMPLEMENT_DYNCREATE(CGridView, CView)

BEGIN_MESSAGE_MAP(CGridView, CView)
	//{{AFX_MSG_MAP(CGridView)
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_SETFOCUS()
	ON_WM_DESTROY()
	ON_COMMAND(ID_GRID_COVERED, OnGridCovered)
	ON_UPDATE_COMMAND_UI(ID_GRID_COVERED, OnUpdateGridCovered)
	ON_COMMAND(ID_GRID_ADD_SECTION, OnGridAddSection)
	ON_COMMAND(ID_GRID_HEADER, OnGridHeader)
	ON_UPDATE_COMMAND_UI(ID_GRID_HEADER, OnUpdateGridHeader)
	ON_COMMAND(ID_GRID_READONLY, OnGridReadonly)
	ON_UPDATE_COMMAND_UI(ID_GRID_READONLY, OnUpdateGridReadonly)
	ON_COMMAND(ID_GRID_SECTION, OnGridSection)
	ON_COMMAND(ID_GRID_SECTION_REMOVE, OnGridSectionRemove)
	ON_COMMAND(ID_GRID_VIEW_SECTION, OnGridViewSection)
	ON_UPDATE_COMMAND_UI(ID_GRID_VIEW_SECTION, OnUpdateGridViewSection)
	ON_COMMAND(ID_GRID_SETKA, OnGridSetka)
	ON_UPDATE_COMMAND_UI(ID_GRID_SETKA, OnUpdateGridSetka)
	ON_COMMAND(ID_ROW_HEIGHT, OnChangingRowHeight)
	//ON_UPDATE_COMMAND_UI(ID_ROW_HEIGHT, OnUpdateChangedRowHeight)
	ON_COMMAND(ID_COL_WIDTH, OnChangingColWidth)
	//ON_UPDATE_COMMAND_UI(ID_COL_WIDTH, OnUpdateChangedColWidth)
	ON_COMMAND(ID_EDIT_ADD, OnEditAdd)
	ON_COMMAND(ID_EDIT_DEL, OnEditDel)
	ON_COMMAND(ID_FORMAT_CELLS, OnFormatCells)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)

    ON_NOTIFY(GVN_BEGINDELETEITEMS, IDC_GRID, OnGridBeginDelete)
    ON_NOTIFY(GVN_ENDDELETEITEMS, IDC_GRID, OnGridEndDelete)
    ON_NOTIFY(GVN_BEGINPASTE, IDC_GRID, OnGridBeginPaste)
    ON_NOTIFY(GVN_ENDPASTE, IDC_GRID, OnGridEndPaste)

	ON_MESSAGE(UPDATE_PROPERTY, OnPropertyUpdateMessage)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CGridView construction/destruction

CGridView::CGridView()
{
	m_pGridCtrl = NULL;
}

CGridView::~CGridView()
{
	if(AfxGetProperty())
		AfxGetProperty()->ReleaseDoc(GetDocument());
}

BOOL CGridView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView drawing

void CGridView::OnDraw(CDC* pDC)
{
	CGridDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	// TODO: add draw code for native data here
}

/////////////////////////////////////////////////////////////////////////////
// CGridView printing

BOOL CGridView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGridView::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnBeginPrinting(pDC, pInfo);
}

void CGridView::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnPrint(pDC, pInfo);
}

void CGridView::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	if (m_pGridCtrl)
		m_pGridCtrl->OnEndPrinting(pDC, pInfo);
}

/////////////////////////////////////////////////////////////////////////////
// CGridView diagnostics

#ifdef _DEBUG
void CGridView::AssertValid() const
{
	CView::AssertValid();
}

void CGridView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGridDoc* CGridView::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGridDoc)));
	return (CGridDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CGridView message handlers

void CGridView::OnInitialUpdate() 
{
	CView::OnInitialUpdate();
	
	if (m_pGridCtrl == NULL)
	{
		// Create the Gridctrl object
		m_pGridCtrl = GetDocument()->GetGrid();
		if (!m_pGridCtrl) 
			return;

		m_pGridCtrl->m_bHasModified=0;
		// Create the Gridctrl window
		CRect rect;
		GetClientRect(rect);
		if(m_pGridCtrl->bWasCreate)
		{
			m_pGridCtrl->SetParent(this);
			m_pGridCtrl->ShowWindow(1);
		}
		else
		{
			m_pGridCtrl->Create(rect, this, 100);

			if(GetDocument()->GetTree())//это режим в составе конфигурации
			{
				GetDocument()->Buffer=GetDocument()->GetObj()->csFile;
			}
			// fill it up with stuff
			m_pGridCtrl->LoadMXL(GetDocument()->Buffer);
			m_pGridCtrl->ViewSection(1);
		}
	}

	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	CMetadataTree *m_MetaTree=GetDocument()->GetTree();
	CString csObjTypeName=GetDocument()->GetObjName();
	if(m_MetaTree)
	{
		CString csTitleName;
		csTitleName=m_MetaTree->GetPath(m_MetaObj->nTempID);
		csTitleName=csTitleName.Mid(csTitleName.Find("\\")+1);
		csTitleName.Replace("\\"," : ");
		GetDocument()->SetTitle(csTitleName);
		
	}
	if(m_MetaObj)
	{
		if(!m_MetaObj->bCanSave)
		{
			m_pGridCtrl->SetEditable(0);
		}

		GetDocument()->SetModifiedFlag(0);
	}



	m_pGridCtrl->m_bHasModified=1;

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_ENTERPTYPE );
}

void CGridView::OnSize(UINT nType, int cx, int cy) 
{
	CView::OnSize(nType, cx, cy);
	
	if (m_pGridCtrl->GetSafeHwnd())
	{
		CRect RectGrid;
		m_pGridCtrl->GetWindowRect(RectGrid);
		ScreenToClient(RectGrid);

		RectGrid.right=cx;
		RectGrid.bottom=cy;

		m_pGridCtrl->MoveWindow(RectGrid);
	}
}

BOOL CGridView::OnCmdMsg(UINT nID, int nCode, void* pExtra, AFX_CMDHANDLERINFO* pHandlerInfo) 
{
    if (m_pGridCtrl && IsWindow(m_pGridCtrl->m_hWnd))
        if (m_pGridCtrl->OnCmdMsg(nID, nCode, pExtra, pHandlerInfo))
            return TRUE;

	return CView::OnCmdMsg(nID, nCode, pExtra, pHandlerInfo);
}



BOOL CGridView::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
	//return CView::OnEraseBkgnd(pDC);
}


void CGridView::OnGridBeginDelete(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    *pResult = true;
 //   CDocument *pDoc = GetDocument();
 //   pDoc->DisableCheckPoint(); // don't checkpoint individual changes
}

void CGridView::OnGridEndDelete(NMHDR *pNotifyStruct, LRESULT *pResult)
{
    *pResult = true;
    CGridDoc *pDoc = GetDocument();
    NM_GRIDVIEW *pItem = (NM_GRIDVIEW *)pNotifyStruct;
    pDoc->EnableCheckPoint();
  //  CString  cs;
   // cs.Format("Delete cells from row %d,column %d", pItem->iRow, pItem->iColumn);
    pDoc->CheckPoint();

}

void CGridView::OnGridBeginPaste(NMHDR *pNotifyStruct, LRESULT*  pResult)
{
    *pResult = true;
    CGridDoc *pDoc = GetDocument();
    pDoc->DisableCheckPoint(); // don't checkpoint individual changes

}

void CGridView::OnGridEndPaste(NMHDR *pNotifyStruct, LRESULT* pResult)
{
    *pResult = true;
    CGridDoc *pDoc = GetDocument();
    NM_GRIDVIEW *pItem = (NM_GRIDVIEW *)pNotifyStruct;
    pDoc->EnableCheckPoint();
 //  CString cs;
  //  cs.Format("Paste cells to row %d,column %d", pItem->iRow, pItem->iColumn);
    pDoc->CheckPoint();
}

void CGridView::OnSetFocus(CWnd* pOldWnd) 
{
	CView::OnSetFocus(pOldWnd);
	if(m_pGridCtrl)
		m_pGridCtrl->SetFocus();
	
	// TODO: Add your message handler code here
	
}

void CGridView::OnDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
	CView::OnDestroy();
}

void CGridView::OnGridCovered() 
{
	if(!m_pGridCtrl)
		return;

	CCellRange Selection=m_pGridCtrl->GetSelectedCellRange();
	CGridCellBase *pCell = m_pGridCtrl->GetCell(Selection.GetMinRow(),Selection.GetMinCol());
	if(pCell)
	if(pCell->IsCovered())
		m_pGridCtrl->SetUnCoveredCells();
	else
		m_pGridCtrl->SetCoveredCells();

	
}

void CGridView::OnUpdateGridCovered(CCmdUI* pCmdUI) 
{
	if(!m_pGridCtrl)
		return;

	if(!m_pGridCtrl->IsEditable())
	{
		pCmdUI->Enable(0);
		return;
	}

	CCellRange Selection=m_pGridCtrl->GetSelectedCellRange();
	CGridCellBase *pCell = m_pGridCtrl->GetCell(Selection.GetMinRow(),Selection.GetMinCol());
	if(!pCell)
	{
		pCmdUI->Enable(0);
	}
	else
	{
		if(!pCell->IsCovered()&&Selection.Count()<2)
			pCmdUI->Enable(0);
		else
			pCmdUI->SetCheck(pCell->IsCovered());
	}
}



void CGridView::OnGridReadonly() 
{
	if(!m_pGridCtrl)
		return;

	m_pGridCtrl->SetEditable(!m_pGridCtrl->IsEditable());
	
}

void CGridView::OnUpdateGridReadonly(CCmdUI* pCmdUI) 
{
	if(!m_pGridCtrl)
		return;
	pCmdUI->SetCheck(!m_pGridCtrl->IsEditable());

	CMetaObject *m_MetaObj=GetDocument()->GetObj();
	if(m_MetaObj)
	if(!m_MetaObj->bCanSave)
		pCmdUI->Enable(0);

}

void CGridView::OnGridHeader() 
{
	if(!m_pGridCtrl)
		return;
	
	m_pGridCtrl->ViewHeader(!m_pGridCtrl->IsViewHeader());
}

void CGridView::OnUpdateGridHeader(CCmdUI* pCmdUI) 
{
	if(!m_pGridCtrl)
		return;
	
	pCmdUI->SetCheck(m_pGridCtrl->IsViewHeader());
}

void CGridView::OnGridViewSection() 
{
	if(!m_pGridCtrl)
		return;
	
	m_pGridCtrl->ViewSection(!m_pGridCtrl->IsViewSection());
}

void CGridView::OnUpdateGridViewSection(CCmdUI* pCmdUI) 
{
	if(!m_pGridCtrl)
		return;
	
	pCmdUI->SetCheck(m_pGridCtrl->IsViewSection());
}

void CGridView::OnGridSetka() 
{
	if(!m_pGridCtrl)
		return;
	
	m_pGridCtrl->ViewGridLines(!m_pGridCtrl->IsViewGridLines());
}

void CGridView::OnUpdateGridSetka(CCmdUI* pCmdUI) 
{
	if(!m_pGridCtrl)
		return;
	
	pCmdUI->SetCheck(m_pGridCtrl->IsViewGridLines());
}


void CGridView::OnGridAddSection() 
{
	if(!m_pGridCtrl)
		return;
	
	m_pGridCtrl->AddSection();
}

void CGridView::OnGridSectionRemove() 
{
	if(!m_pGridCtrl)
		return;

	m_pGridCtrl->RemoveSection();
	
}

void CGridView::OnGridSection() 
{
	if(!m_pGridCtrl)
		return;
}

void CGridView::OnChangingRowHeight() 
{
	if(!m_pGridCtrl)
		return;
#ifndef SMALL_TRANSLATE
	CInputNumeric dlg;
	dlg.csValue="0";
	dlg.csTitle="¬ведите высоту строки:";
	dlg.nLength=4;
	dlg.nPrec=0;
	dlg.nTimer=0;
	if(dlg.DoModal()==IDOK)
	{
		int nHeight=int(String(dlg.csValue).GetNumber());
		CCellRange Range=m_pGridCtrl->GetSelectedCellRange();
		for(int i=Range.GetMinRow();i<=Range.GetMaxRow();i++)
		{
			m_pGridCtrl->SetRowHeight(i,nHeight);
			Invalidate();
			m_pGridCtrl->SetModified(TRUE, i,0);
		}
	}
	
#endif
}

//DEL void CGridView::OnUpdateChangedRowHeight(CCmdUI* pCmdUI) 
//DEL {
//DEL 	if(!m_pGridCtrl)
//DEL 		return;
//DEL 	if(!m_pGridCtrl->IsEditable())
//DEL 	{
//DEL 		pCmdUI->Enable(0);
//DEL 		return;
//DEL 	}
//DEL 	Invalidate();
//DEL }

void CGridView::OnChangingColWidth() 
{
	if(!m_pGridCtrl)
		return;
#ifndef SMALL_TRANSLATE
	CInputNumeric dlg;
	dlg.csValue="0";
	dlg.csTitle="¬ведите ширину колонки:";
	dlg.nLength=4;
	dlg.nPrec=0;
	dlg.nTimer=0;
	if(dlg.DoModal()==IDOK)
	{
		int nWidth=int(String(dlg.csValue).GetNumber());
		CCellRange Range=m_pGridCtrl->GetSelectedCellRange();
		for(int i=Range.GetMinCol();i<=Range.GetMaxCol();i++)
		{
			m_pGridCtrl->SetColumnWidth(i,nWidth);
			Invalidate();
			m_pGridCtrl->SetModified(TRUE,0,i);
		}
			
	}
	
#endif
}

//DEL void CGridView::OnUpdateChangedColWidth(CCmdUI* pCmdUI) 
//DEL {
//DEL 	if(!m_pGridCtrl)
//DEL 		return;
//DEL 	if(!m_pGridCtrl->IsEditable())
//DEL 	{
//DEL 		pCmdUI->Enable(0);
//DEL 		return;
//DEL 	}
//DEL 	Invalidate();
//DEL }

void CGridView::OnEditAdd() 
{
	if(!m_pGridCtrl)
		return;


	CCellRange Selection=m_pGridCtrl->GetSelectedCellRange();
	if(Selection.Count()>0)
	{
		if(Selection.GetRowSpan()<=Selection.GetColSpan())//строки
		{
			for(int n=Selection.GetMinRow();n<=Selection.GetMaxRow();n++)
				m_pGridCtrl->InsertRow("",Selection.GetMinRow());
		}
		else//колонки
		{
			for(int n=Selection.GetMinCol();n<=Selection.GetMaxCol();n++)
				m_pGridCtrl->InsertColumn("",DT_CENTER|DT_VCENTER|DT_SINGLELINE,Selection.GetMinCol());
		}
		Invalidate();
	}

}

void CGridView::OnEditDel() 
{
	if(!m_pGridCtrl)
		return;


	CCellRange Selection=m_pGridCtrl->GetSelectedCellRange();
	if(Selection.Count()>0)
	{
		if(Selection.GetRowSpan()<=Selection.GetColSpan())//строки
		{
			for(int n=Selection.GetMinRow();n<=Selection.GetMaxRow();n++)
				m_pGridCtrl->DeleteRow(Selection.GetMinRow());
		}
		else//колонки
		{
			for(int n=Selection.GetMinCol();n<=Selection.GetMaxCol();n++)
				m_pGridCtrl->DeleteColumn(Selection.GetMinCol());
		}
		Invalidate();
	}

	
}

void CGridView::OnFormatCells() 
{
	if(m_pGridCtrl)
		m_pGridCtrl->ShowProperty(1);
}

BOOL CGridView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if(m_pGridCtrl)
	if(pMsg->message == WM_KEYDOWN)
	{
		int nChar= (int) pMsg->wParam;
		if(VK_F2==nChar)
		{
			m_pGridCtrl->SendMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return 1;
		}
	}
	return CView::PreTranslateMessage(pMsg);
}

#define SetVal(x) if(pOrigCell->x!=pCel->x){pItemCel->x=pCel->x;}

int CGridView::OnPropertyUpdateMessage(WPARAM wParam, LPARAM lParam)
{
	if(m_pGridCtrl)
	{
		//смотрим если есть выделенна€ область, то изменени€ выполн€ем дл€ всех
		CCellRange Selection=m_pGridCtrl->GetSelectedCellRange();
		CCellID cell=m_pGridCtrl->GetFocusCell();
		if(Selection.Count()>1)
		{
			CCell *pCel=m_pGridCtrl->GetItemCell(cell.row,cell.col);
			if(pCel)
			{
				CCell *pOrigCell=m_pGridCtrl->GetOrigCell();

				for(int nRow=Selection.GetMinRow();nRow<=Selection.GetMaxRow();nRow++)
				for(int nCol=Selection.GetMinCol();nCol<=Selection.GetMaxCol();nCol++)
				{
					CCell *pItemCel=m_pGridCtrl->GetItemCell(nRow,nCol);
					if(!pItemCel)
					{
						CCell cel;
						m_pGridCtrl->SetItemCell(nRow,nCol,&cel);
						pItemCel=m_pGridCtrl->GetItemCell(nRow,nCol);
					}
					SetVal(csText);
					SetVal(csFormula);
					SetVal(csFormat);
					SetVal(csMaska);
					SetVal(bProtect);
					SetVal(nMode);
					SetVal(nControl);
					SetVal(nHPosition);
					SetVal(nVPosition);
					SetVal(nBold);
					SetVal(nItalic);
					SetVal(nUnderLine);
					SetVal(nFontHeight);
					SetVal(nFontColor);
					SetVal(nBackgroundColor);
					SetVal(nRamkaColor);
					SetVal(nRamkaL);
					SetVal(nRamkaR);
					SetVal(nRamkaU);
					SetVal(nRamkaD);

					m_pGridCtrl->InvalidateCellRect(nRow,nCol);
				}
				m_pGridCtrl->SetOrigCell(pCel);
			}
		}
		m_pGridCtrl->InvalidateCellRect(cell.row,cell.col);
	}
	return 1;
}
