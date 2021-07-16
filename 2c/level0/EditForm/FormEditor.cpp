// Авторские права: VTOOLS.RU (info@vtools.ru) 2002-2004г.
// FormEditor.cpp : implementation file
//

#include "stdafx.h"
#include "FormEditor.h"
#include "../MainFrm.h"
#include "../DocList.h"
#include "../ModuleDoc.h"
#include "../MetadataTree.h"
#include "../Resource.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFormEditor
#define m_MetaObj ((CMetaDocument*)GetDocument())->GetObj()
#define m_MetaTree ((CMetaDocument*)GetDocument())->GetTree()

IMPLEMENT_DYNCREATE(CFormEditor, CBaseClass)

CFormEditor::CFormEditor()
//	: CBaseClass(CFormEditor::IDD)
{
	m_pToolBox=0;
	m_FormSelector=0;
	m_pDialog=0;
	nCurPosUndo=-1;
//	nSelectItem=0;
}

CFormEditor::~CFormEditor()
{
	AfxGetProperty()->ReleaseDoc(GetDocument());
	if(m_pToolBox)
		delete m_pToolBox;
	if(m_pDialog)
		delete m_pDialog;
}


BEGIN_MESSAGE_MAP(CFormEditor, CBaseClass)
	//{{AFX_MSG_MAP(CFormEditor)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_DESTROY()
	ON_COMMAND(ID_DYNBUTTONEDIT, OnDynbuttonedit)
	ON_COMMAND(ID_DYNBUTTONLISTBOX, OnDynbuttoneditmulti)
	ON_COMMAND(ID_DYNBUTTONLABEL, OnDynbuttonlabel)
	ON_COMMAND(ID_DYNBUTTONGROUP, OnDynbuttongroup)
	ON_COMMAND(ID_DYNBUTTONDATE, OnDynbuttondate)
	ON_COMMAND(ID_DYNBUTTONCOMBO, OnDynbuttoncombo)
	ON_COMMAND(ID_DYNBUTTONCHECK, OnDynbuttoncheck)
	ON_COMMAND(ID_DYNBUTTONRADIO, OnDynbuttonradio)
	ON_COMMAND(ID_DYNBUTTON, OnDynbutton)
	ON_WM_KEYDOWN()
	ON_COMMAND(ID_SELECT_ALL, OnSelectAll)
	ON_COMMAND(ID_EDIT_GRID, OnEditGrid)
	ON_COMMAND(ID_EDIT_ALLIGN_GRID, OnEditAllignGrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_GRID, OnUpdateEditGrid)
	ON_UPDATE_COMMAND_UI(ID_EDIT_ALLIGN_GRID, OnUpdateEditAllignGrid)
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(WM_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_UNDO, OnEditUndo)
	ON_COMMAND(ID_EDIT_REDO, OnEditRedo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_UNDO, OnUpdateEditUndo)
	ON_UPDATE_COMMAND_UI(ID_EDIT_REDO, OnUpdateEditRedo)
	ON_WM_SIZE()
	ON_COMMAND(ID_DYNBUTTONGRID, OnDynbuttongrid)
	ON_COMMAND(ID_DYNBUTTONTREE, OnDynbuttontree)
	ON_COMMAND(ID_DYNBUTTONLEFTALIGN, OnDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONLEFTALIGN, OnUpdateDynbuttonleftalign)
	ON_COMMAND(ID_DYNBUTTONRIGHTALIGN, OnDynbuttonrightalign)
	ON_COMMAND(ID_DYNBUTTONTOPALIGN, OnDynbuttontopalign)
	ON_COMMAND(ID_DYNBUTTONBOTTOMALIGN, OnDynbuttonbottomalign)
	ON_COMMAND(ID_DYNBUTTONMAKESAMESIZE, OnDynbuttonmakesamesize)
	ON_COMMAND(ID_BUTTONPROGRESS, OnButtonprogress)
	ON_COMMAND(ID_BUTTONSLIDER, OnButtonslider)
	ON_COMMAND(ID_DYNBUTTONMAKESAMEWIDTH, OnDynbuttonmakesamewidth)
	ON_COMMAND(ID_DYNBUTTONMAKESAMEHEIGHT, OnDynbuttonmakesameheight)
	ON_COMMAND(ID_CNTERINVIEW_HORIZ, OnCnterinviewHoriz)
	ON_UPDATE_COMMAND_UI(ID_CNTERINVIEW_HORIZ, OnUpdateCnterinviewHoriz)
	ON_COMMAND(ID_CNTERINVIEW_VERT, OnCnterinviewVert)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_BUTTONFORMOBJECT, OnButtonformobject)
	ON_COMMAND(ID_BUTTON_TAB, OnButtonTab)
	ON_COMMAND(ID_BRING_TO_FRONT, OnBringToFront)
	ON_UPDATE_COMMAND_UI(ID_BRING_TO_FRONT, OnUpdateBringToFront)
	ON_COMMAND(ID_SEND_TO_BACK, OnSendToBack)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONMAKESAMEWIDTH, OnUpdateDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONMAKESAMEHEIGHT, OnUpdateDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONRIGHTALIGN, OnUpdateDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONTOPALIGN, OnUpdateDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONBOTTOMALIGN, OnUpdateDynbuttonleftalign)
	ON_UPDATE_COMMAND_UI(ID_DYNBUTTONMAKESAMESIZE, OnUpdateDynbuttonleftalign)
	ON_MESSAGE(WM_SHOWPROPERTY,MessageShowProperty)
	ON_MESSAGE(WM_ENDSELECT,EndSelectMode)
	ON_UPDATE_COMMAND_UI(ID_CNTERINVIEW_VERT, OnUpdateCnterinviewHoriz)
	ON_UPDATE_COMMAND_UI(ID_SEND_TO_BACK, OnUpdateBringToFront)
	ON_COMMAND(ID_LAYERS, OnLayers)
	ON_COMMAND(ID_ORDER_ELEMENTS, OnChangeOrder)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormEditor drawing

void CFormEditor::OnDraw(CDC* pDC)
{
	CDocument* pDoc = GetDocument();
	
	if(m_FormSelector&&m_pDialog)
	{
/*		CRect Rect=m_FormSelector->m_rect;
		m_pDialog->GetWindowRect(&Rect) ;
		ScreenToClient(&Rect) ;
		delete m_FormSelector;
		m_FormSelector = new CMultiRectTracker(Rect,CRectTracker::dottedLine | CRectTracker::resizeInside  | CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
		m_FormSelector->bNotMove=1;
*/
		m_FormSelector->Draw(pDC)  ;
	}
}

/////////////////////////////////////////////////////////////////////////////
// CFormEditor diagnostics

#ifdef _DEBUG
void CFormEditor::AssertValid() const
{
	CBaseClass::AssertValid();
}

void CFormEditor::Dump(CDumpContext& dc) const
{
	CBaseClass::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFormEditor message handlers

void CFormEditor::OnInitialUpdate() 
{
	CBaseClass::OnInitialUpdate();
	CRect rect(10,5,60,19);

	m_pDialog=new CFormDialog(this);
	m_pDialog->Create(IDD_EDITDIALOG_FORM,this);
	m_pDialog->SetParent(this);
	m_pDialog->SetWindowPos(NULL, 0,0, 300, 200, SWP_NOSIZE | SWP_NOZORDER);
	m_pDialog->ShowWindow(1);


	m_pDialog->m_pDoc=GetDocument();
	m_pDialog->m_pView=this;


	m_Layers.Create(IDD_LAYERSELECT_FORM,this);
	m_Layers.AttachEditor(this);
	
	//CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->m_MetaObj;
	if(m_MetaObj)
	{
		AllElements.LoadFromString(m_MetaObj->csForm);
		if(!m_MetaObj->bCanSave)
			m_pDialog->bReadOnly=1;
	}
	LoadData(1);

	CRect Rect;
	m_pDialog->GetWindowRect(&Rect) ;
	m_pDialog->ScreenToClient(&Rect) ;
	m_FormSelector = new CMultiRectTracker(Rect,CRectTracker::dottedLine  | CRectTracker::resizeInside | CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
	m_FormSelector->bNotMove=1;
	ShowProperty(0);


	SetModifiedFlag(0);//подготовка к Undo
	OnUpdate(0,0,0);

    //GetParentFrame()->RecalcLayout();
    //ResizeParentToFit(TRUE);


	GetDocument()->SetTitle(m_pDialog->m_csValue);
//	GetDocument()->SetPathName(m_pDialog->m_csValue);
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_FORM1C_TMPL );

}

void CFormEditor::OnMouseMove(UINT nFlags, CPoint point) 
{
	CBaseClass::OnMouseMove(nFlags, point);
}

void CFormEditor::OnLButtonUp(UINT nFlags, CPoint point) 
{
	::ReleaseCapture();
	
	CBaseClass::OnLButtonUp(nFlags, point);
}

void CFormEditor::InvalidateSelector()
{
	if(m_FormSelector)
	{
		CRect Rect;
		Rect=m_FormSelector->m_rect;
		Rect.top=Rect.bottom;
		Rect.InflateRect(5,5);
		InvalidateRect(Rect);

		Rect=m_FormSelector->m_rect;
		Rect.left=Rect.right;
		Rect.InflateRect(5,5);
		InvalidateRect(Rect);

	}
}

void CFormEditor::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(!m_pDialog)
		return;
	if(!IsCTRLpressed())
		m_pDialog->SendMessage(WM_ENDSELECT);

	CRect Rect;
	bool bSelect=1;
	if (m_FormSelector)
	{
		if(m_pDialog->bReadOnly == 0)
		if(abs(m_FormSelector->m_rect.bottom-point.y)<5||abs(m_FormSelector->m_rect.right-point.x)<5)
		{
			bSelect=!m_FormSelector->Track(this, point, TRUE);
			if(!bSelect)
			{
				m_pDialog->MoveWindow(m_FormSelector->m_rect,1);
/*
				SIZE size;
				size.cx=m_FormSelector->m_rect.Width()-5;
				size.cy=m_FormSelector->m_rect.Height()-5;
				SetScrollSizes( MM_TEXT, size,size,size);
				GetParentFrame()->RecalcLayout();
*/
				Invalidate();
				SetModifiedFlag(1);
			}
		}
	}

	m_pDialog->GetWindowRect(&Rect) ;
	m_pDialog->ScreenToClient(&Rect) ;
	if(m_FormSelector)
		delete m_FormSelector;
	m_FormSelector = new CMultiRectTracker(Rect,CRectTracker::dottedLine | CRectTracker::resizeInside  | CRectTracker::resizeOutside | CRectTracker::hatchedBorder );
	m_FormSelector->bNotMove=1;


	ShowProperty(0);
	InvalidateSelector();


	if(bSelect)
	{
		Rect.left=Rect.right=point.x;
		Rect.top=Rect.bottom=point.y;
		CMultiRectTracker ItemSelector(Rect,CRectTracker::solidLine  | CRectTracker::resizeOutside);
		BOOL bRes=ItemSelector.Track(this, point, TRUE);
		if(bRes)
		{
			for(int i=0;i<m_pDialog->aWindowsList.GetSize();i++)
			{
				CRect newrect=ItemSelector.m_rect;
				newrect.NormalizeRect();

				CRect Rect;
				::GetWindowRect(m_pDialog->aWindowsList[i],&Rect);
				ScreenToClient(&Rect);
				CPoint point1;
				point1.x=Rect.left;
				point1.y=Rect.top;
				CPoint point2;
				point2.x=Rect.right;
				point2.y=Rect.bottom;
				if(newrect.PtInRect(point1)&&newrect.PtInRect(point2))
				{
					m_pDialog->SelectItem(CWnd::FromHandle(m_pDialog->aWindowsList[i]),1);
				}
			}
		}
	}



	CBaseClass::OnLButtonDown(nFlags, point);
}





BOOL CFormEditor::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(m_pDialog)
	if(m_pDialog->bReadOnly==0)
	if(m_FormSelector)
	if (pWnd == this && m_FormSelector->SetCursor(this, nHitTest))
		return TRUE;
	

	if(m_pDialog)
	{
		m_pDialog->OnSetCursor(pWnd, nHitTest, message);
		return TRUE;
	}

	return CBaseClass::OnSetCursor(pWnd, nHitTest, message);
}

void CFormEditor::EndSelectMode(WPARAM wParam, LPARAM lParam)
{
//	Invalidate();
	if(m_FormSelector)
	{
		InvalidateSelector();
		delete m_FormSelector;
		m_FormSelector=0;
	}

}
void CFormEditor::MessageShowProperty(WPARAM wParam, LPARAM lParam)
{
	ShowProperty(int(wParam));
}


void CFormEditor::OnDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
	CBaseClass::OnDestroy();
}

void CFormEditor::OnDynbuttonedit() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONEDIT);
}

void CFormEditor::OnDynbuttoneditmulti() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONLISTBOX);
}

void CFormEditor::OnDynbuttonlabel() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONLABEL);
}


void CFormEditor::OnDynbuttongroup() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONGROUP);
}

void CFormEditor::OnDynbuttondate() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONDATE);
}

void CFormEditor::OnDynbuttoncombo() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONCOMBO);
}

void CFormEditor::OnDynbuttoncheck() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONCHECK);
}

void CFormEditor::OnDynbuttonradio() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONRADIO);
}

void CFormEditor::OnDynbutton() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTON);
}
void CFormEditor::OnDynbuttongrid() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONGRID);
}
void CFormEditor::OnDynbuttontree() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_DYNBUTTONTREE);
}

void CFormEditor::OnButtonprogress() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_BUTTONPROGRESS);
}


void CFormEditor::OnButtonslider() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_BUTTONSLIDER);

}




void CFormEditor::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_pDialog->OnKeyDown(nChar, nRepCnt, nFlags);
}

void CFormEditor::OnSelectAll() 
{
	if(!m_pDialog)
		return;
	m_pDialog->SendMessage(WM_ENDSELECT);
	for(int i=0;i<m_pDialog->aWindowsList.GetSize();i++)
	{
		m_pDialog->SelectItem(CWnd::FromHandle(m_pDialog->aWindowsList[i]),1);
	}
	
}

void CFormEditor::OnEditGrid() 
{
	if(m_pDialog)
	{
		m_pDialog->m_bGrid=!m_pDialog->m_bGrid;
		m_pDialog->Invalidate();
	}
}

void CFormEditor::OnEditAllignGrid() 
{
	if(m_pDialog)
	{
		m_pDialog->m_bUseGrid=!m_pDialog->m_bUseGrid;
		m_pDialog->Invalidate();
	}
}

void CFormEditor::OnUpdateEditGrid(CCmdUI* pCmdUI) 
{
	if(m_pDialog)
		pCmdUI->SetCheck(m_pDialog->m_bGrid);
}

void CFormEditor::OnUpdateEditAllignGrid(CCmdUI* pCmdUI) 
{
	if(m_pDialog)
		pCmdUI->SetCheck(m_pDialog->m_bUseGrid);
}

void CFormEditor::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	if(m_pDialog)
		m_pDialog->OnContextMenu(pWnd,point);
}

void CFormEditor::OnEditCut() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_CUT);
		return;
	}
	if(m_pDialog)
		m_pDialog->OnEditCut();
}

void CFormEditor::OnEditCopy() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_COPY);
		return;
	}
	if(m_pDialog)
		m_pDialog->OnEditCopy();
}

void CFormEditor::OnEditPaste() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_PASTE);
		return;
	}
	if(m_pDialog)
		m_pDialog->OnEditPaste();
}

void CFormEditor::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	
	if(m_pDialog)
	{
		if(AfxGetProperty()->GetDocType()==(LPARAM)this)
		{
			//GetDocument()->SetTitle(m_pDialog->m_csValue);
		}
		else
		{
			m_pDialog->OnUpdate();
		}
/*
		CRect Rect;
		m_pDialog->GetClientRect(Rect);
		
		SIZE size;
		size.cx=Rect.Width()+150;
		size.cy=Rect.Height()+150;


		GetClientRect(Rect);
		SIZE size2;
		size2.cx=Rect.Width();
		size2.cy=Rect.Height();

		//SetScaleToFitSize(size);

		SetScrollSizes( MM_TEXT, size,size,size);
		GetParentFrame()->RecalcLayout();
		//ResizeParentToFit(FALSE);
*/
	}
}

void CFormEditor::ShowProperty(bool nShow) 
{
	AfxGetProperty()->Clear();
	if(m_pDialog)
	{
		AfxGetProperty()->SetDoc(GetDocument(),(LPARAM)m_pDialog,(LPARAM)this,m_pDialog->bReadOnly);
		//AfxGetProperty()->AddProperty("Форма","Идентификатор",m_pDialog->m_csName);
		AfxGetProperty()->AddProperty("Форма","Заголовок",m_pDialog->m_csValue);
		AfxGetProperty()->AddProperty("Форма","Синоним",m_pDialog->csAlias);
		AfxGetProperty()->AddProperty("Форма","Комментарий",m_pDialog->csComment);
		AfxGetProperty()->AddProperty("Форма","Не изменять размер",m_pDialog->bCanResize);
		//AfxGetProperty()->AddProperty("Форма","Иконка",m_pDialog->m_nIcon);



		if(AfxGetProperty()->IsWindowVisible()==0&&nShow)
			((CMainFrame*)AfxGetMainWnd())->ShowPropertyBar(TRUE);

	}

}

void CFormEditor::LoadData(int bCheckNew) 
{
	if(m_pDialog)
	{
		m_pDialog->ClearData();
		if(AllElements.GetTypeString()=="Массив"||AllElements.GetTypeString()=="Структура")
		{
			CValue ObjectVal=AllElements.GetAt("Форма");
			if(ObjectVal.GetSizeStrArray()>0)
			{
				m_pDialog->m_csName=ObjectVal.GetAt("Идентификатор");
				m_pDialog->m_csValue=ObjectVal.GetAt("Заголовок");
				m_pDialog->csAlias=ObjectVal.GetAt("Синоним");
				m_pDialog->csComment=ObjectVal.GetAt("Комментарий");
				m_pDialog->bCanResize=ObjectVal.GetAt("НеИзменятьРазмер");
				m_pDialog->m_nIcon=ObjectVal.GetAt("Иконка");
		
				CRect Rect;
				Rect.left=ObjectVal.GetAt("x1");
				Rect.top=ObjectVal.GetAt("y1");
				Rect.right=ObjectVal.GetAt("x2");
				Rect.bottom=ObjectVal.GetAt("y2");

				m_pDialog->MoveWindow(Rect.left, Rect.top, Rect.right-Rect.left, Rect.bottom-Rect.top,1);
			}

			//слои (доп. информация)
			ObjectVal=AllElements.GetAt("Слои");
			for(int l=0;l<ObjectVal.GetSizeArray();l++)
			{
				CValue vLayer=ObjectVal.GetAt(l+1);
				if(vLayer.GetSizeStrArray()>0)
				{
					CString csName=vLayer.GetAt("Name");
					int bVisible=vLayer.GetAt("Visible");
					int bActivate=vLayer.GetAt("Activate");
					m_Layers.AddLayer(csName,bVisible,bActivate);
				}
			}

			for(int i=0;i<AllElements.GetSizeArray();i++)
			{
				ObjectVal=AllElements.GetAt(i+1);
				CDynControl *pControl=m_pDialog->AddControl(ObjectVal);
				CChangeOrderItem data;
				data.csCapture=ObjectVal.GetAt("Заголовок");
				data.csType=ObjectVal.GetAt("ТипЭлемента");
				data.csId=ObjectVal.GetAt("Идентификатор");
				data.csLayer=ObjectVal.GetAt("Слой");
				data.pControl=pControl;
				m_aChangeOrder.Add(data);
				pControl->OnUpdate();

				//корректируем список слоев
				m_Layers.AddLayer(pControl->m_csLayer,0,0);

			}
			m_pDialog->ReloadWindows();
			m_Layers.ReLoadGrid();
			
		}
		else
		if(bCheckNew)
		{
			((CModuleDoc*)GetDocument())->nCanDefualtType=DEFAULT_FORM_MODULE;//форма

			CDynControl *pControl;
			pControl=m_pDialog->AddControl(ID_DYNBUTTON,CPoint(240,255));
			pControl->m_csValue="Выполнить";
			pControl->m_csName="Выполнить";
			pControl->SetFuncProperty("Нажатие","Выполнить");
			pControl->OnUpdate();

			pControl=m_pDialog->AddControl(ID_DYNBUTTON,CPoint(340,255));
			pControl->m_csValue="Закрыть";
			pControl->m_csName="Закрыть";
			pControl->SetFuncProperty("Нажатие","ЗакрытьФорму");
			pControl->SetFuncProperty("Кнопка по умолчанию",1);
			
			pControl->OnUpdate();
			m_pDialog->ReloadWindows();
		}
	}
		
}

//запись содержимого формы в строку
void CFormEditor::GetWindowText(CString &csText)
{
	if(m_pDialog)
	{
		//AllElements.Reset();
		AllElements.CreateObject("Структура");
		CValue ObjectVal;

		ObjectVal.CreateObject("Структура");
		ObjectVal.SetAt("Идентификатор",String(m_pDialog->m_csName));
		ObjectVal.SetAt("Заголовок",String(m_pDialog->m_csValue));
		ObjectVal.SetAt("Синоним",String(m_pDialog->csAlias));
		ObjectVal.SetAt("Комментарий",String(m_pDialog->csComment));
		ObjectVal.SetAt("НеИзменятьРазмер",m_pDialog->bCanResize);
		ObjectVal.SetAt("Иконка",m_pDialog->m_nIcon);

		CRect Rect;
		m_pDialog->GetWindowRect(&Rect);
		ScreenToClient(&Rect);

		ObjectVal.SetAt("x1",Rect.left);
		ObjectVal.SetAt("y1",Rect.top);
		ObjectVal.SetAt("x2",Rect.right);
		ObjectVal.SetAt("y2",Rect.bottom);


		m_pDialog->SaveAllToValue(AllElements);
		AllElements.SetAt("Форма",ObjectVal);

		//слои
		ObjectVal.CreateObject("Массив");
		for(int l=0;l<m_Layers.aList.GetSize();l++)
		{
			CValue vLayer;
			vLayer.CreateObject("Структура");
			vLayer.SetAt("Name",m_Layers.aList[l].csName);
			if(m_Layers.aList[l].bVisible)
				vLayer.SetAt("Visible",m_Layers.aList[l].bVisible);
			if(l==m_Layers.nCurrentEdit)
				vLayer.SetAt("Activate",1);
			ObjectVal.SetAt(l+1,vLayer);
		}
		AllElements.SetAt("Слои",ObjectVal);

		AllElements.SaveToString(csText);
	}
}


//Поддержка Undo+Redo
void CFormEditor::SetModifiedFlag(int bSet)
{
	CString csText;
	GetWindowText(csText);
	nCurPosUndo++;
	if(nCurPosUndo>0)
		listStrUndo.SetSize(nCurPosUndo);

	listStrUndo.SetAtGrow(nCurPosUndo,csText);
	if(bSet)
	if(m_pDialog)
	if(m_pDialog->bReadOnly==0)
		GetDocument()->SetModifiedFlag(bSet);
}

void CFormEditor::OnEditUndo() 
{
	nCurPosUndo--;
	if(nCurPosUndo>=0)
	{
		AfxGetProperty()->ReleaseDoc(GetDocument());

		CString csText=listStrUndo[nCurPosUndo];
		AllElements.LoadFromString(csText);
		EndSelectMode(0,0);
		LoadData(0);
		//m_pDialog->ReloadWindows();
		if(m_pDialog)
			m_pDialog->SendMessage(WM_ENDSELECT);

	}
	SetFocus();
}

void CFormEditor::OnEditRedo() 
{
	if(nCurPosUndo+1<listStrUndo.GetSize())
	{
		nCurPosUndo+=2;
		OnEditUndo();
	}
	SetFocus();
}

void CFormEditor::OnUpdateEditUndo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(nCurPosUndo>0);
}

void CFormEditor::OnUpdateEditRedo(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(nCurPosUndo+1<listStrUndo.GetSize());
}

void CFormEditor::OnSize(UINT nType, int cx, int cy) 
{
	CBaseClass::OnSize(nType, cx, cy);
}

void CFormEditor::OnUpdateDynbuttonleftalign(CCmdUI* pCmdUI) 
{
	if(m_pDialog)
	{
		pCmdUI->Enable(m_pDialog->GetCountSelectedList()>1);
		return;
	}
	pCmdUI->Enable(0);
}

void CFormEditor::OnDynbuttonleftalign() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONLEFTALIGN);
}

void CFormEditor::OnDynbuttonrightalign() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONRIGHTALIGN);
}

void CFormEditor::OnDynbuttontopalign() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONTOPALIGN);
}

void CFormEditor::OnDynbuttonbottomalign() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONBOTTOMALIGN);
}

void CFormEditor::OnDynbuttonmakesamesize() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONMAKESAMESIZE);
}
/*
void CFormEditor::OnUpdateFileSaveAs(CCmdUI* pCmdUI) 
{
	if(m_MetaTree)
		pCmdUI->Enable(0);
}
*/

void CFormEditor::OnDynbuttonmakesamewidth() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONMAKESAMEWIDTH);
}


void CFormEditor::OnDynbuttonmakesameheight() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_DYNBUTTONMAKESAMEHEIGHT);
}



void CFormEditor::OnUpdateCnterinviewHoriz(CCmdUI* pCmdUI) 
{
	if(m_pDialog)
	{
		pCmdUI->Enable(m_pDialog->GetCountSelectedList()>0);
		return;
	}
	pCmdUI->Enable(0);
}

void CFormEditor::OnCnterinviewHoriz() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_CNTERINVIEW_HORIZ);
}
void CFormEditor::OnCnterinviewVert() 
{
	if(m_pDialog)
		m_pDialog->GroupsSelectedList(ID_CNTERINVIEW_VERT);
}


void CFormEditor::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	ShowProperty(1);
	
	CBaseClass::OnLButtonDblClk(nFlags, point);
}

void CFormEditor::OnButtonformobject() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_BUTTONFORMOBJECT);
}

void CFormEditor::OnButtonTab() 
{
	if(m_pDialog)
		m_pDialog->SendMessage(WM_ADDCONTROL,ID_BUTTON_TAB);
}


void CFormEditor::OnUpdateBringToFront(CCmdUI* pCmdUI) 
{
	if(m_pDialog)
	{
		pCmdUI->Enable(m_pDialog->GetCountSelectedList()>=1);
		return;
	}
	pCmdUI->Enable(0);
}

void CFormEditor::OnBringToFront() 
{
	if(m_pDialog)
		m_pDialog->BringToFront(1);
}
void CFormEditor::OnSendToBack() 
{
	if(m_pDialog)
		m_pDialog->BringToFront(0);
}


void CFormEditor::OnLayers() 
{
	m_Layers.ShowWindow(SW_NORMAL);
	
}
void CFormEditor::OnChangeOrder() 
{
	CChangeOrder dlg; 
	dlg.AttachEditor(this);
	for(int i=0;i<m_aChangeOrder.GetSize();i++)
		dlg.aList.Add(m_aChangeOrder[i]);
	if(dlg.DoModal()==IDOK)
	{
		for(int i=0;i<dlg.aList.GetSize();i++)
		{
			CDynControl *pControl=dlg.aList[i].pControl;
			if(pControl)
				pControl->nCreate=FORM_START_ID+i;
		}
	}
}

CString CFormEditor::GetLayerName()
{
	return m_Layers.GetCurrentName();
}

BOOL CFormEditor::IsVisibleLayer(CString csName)
{
	return m_Layers.IsVisibleLayer(csName);
}

CString CFormEditor::GetStrLayers()
{
	return m_Layers.GetStrLayers();
}
