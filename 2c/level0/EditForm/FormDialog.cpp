// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// FormDialog.cpp : implementation file
//

#include "stdafx.h"
#include "FormDialog.h"
#include "FormEditor.h"
#include "../MainFrm.h"
#include "../GridCtrl_src/GridCtrl.h"
#include "../Resource.h"
#include "../MetadataTree.h"
#include "../ModuleDoc.h"

#define IsPressed(x)  (( (GetKeyState(x) & (1 << (sizeof(SHORT)*8-1))) != 0 ))


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#include "DblSubclassWnd.h"

CValue ValCopyPaste;//буфер обмена
CGridCtrl *AfxCurrentGrid=0;
int AfxCurrentCol=-1;
CMap<CWnd*,CWnd*,CFormDialog *,CFormDialog *> aThisList;//список форм
extern HTREEITEM treeDragItem;
int IsWindows5();
extern int nUniqueID;

void LoadObjectsList();
extern char *aChooseList[];
extern int nTypeLayers;


CString GetNormalProcName(CString Str,CString csEventName,CString csIdName);
int GotoModuleProc(CString csIdName,CString Str);



//сравнение по номеру создания диалога
int CompareControl( const void *arg1, const void *arg2 )
{
 	CDynControl *pCtrl1=(*(CDynControl**)arg1);
	CDynControl *pCtrl2=(*(CDynControl**)arg2);
	return pCtrl1->nCreate-pCtrl2->nCreate;
}
void SortControlList(CArray <CDynControl*,CDynControl*> &List)
{
	if(List.GetSize())
		qsort(&List[0], List.GetSize(), sizeof(List[0]), CompareControl);
}

//сравнение по ID
int PosCompareControl( const void *arg1, const void *arg2 )
{
 	CDynControl *pCtrl1=(*(CDynControl**)arg1);
	CDynControl *pCtrl2=(*(CDynControl**)arg2);
	return (pCtrl1->m_Rect.top*1000+pCtrl1->m_Rect.left)-(pCtrl2->m_Rect.top*1000+pCtrl2->m_Rect.left);
}
void PosSortControlList(CArray <CDynControl*,CDynControl*> &List)
{
	if(List.GetSize())
		qsort(&List[0], List.GetSize(), sizeof(List[0]), PosCompareControl);
}

/////////////////////////////////////////////////////////////////////////////
// CFormDialog dialog
CFormDialog::CFormDialog(CWnd* pParent /*=NULL*/)
	: CDIALOG(CFormDialog::IDD, pParent)
//	: CDIALOG(pParent)
{
	//{{AFX_DATA_INIT(CFormDialog)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	// Note that LoadIcon does not require a subsequent DestroyIcon in Win32
	m_hIcon = AfxGetApp()->LoadIcon(IDR_FORM1C_TMPL);
	m_bGrid=0;
	m_bUseGrid=1;
	SetGridSize(5,5);
	m_pDoc=0;
	ptContextMenu.x=-1;

	nCurrentMode=0;
	//m_csName="Форма";
	m_csValue="Форма";
	bCanResize=0;
	m_nIcon=0;

	m_pTracker=0;
//////////////////////////////////////////////////////////////////////////
	// Set the Dblsubclassing function
	m_pEditedCtrl = NULL;
	CDblSubclassWnd::SubclassChildsRecurs(m_hWnd, AllowSubclassCallBack);
	aThisList[this]=this;
//////////////////////////////////////////////////////////////////////////


	bReadOnly=0;
}
CFormDialog::~CFormDialog()
{
	if(m_pTracker)
		delete m_pTracker;
	m_pTracker=0;
	ClearData();
	CDblSubclassWnd::UnSubclassChildsRecurs(m_hWnd);
}

void CFormDialog::DoDataExchange(CDataExchange* pDX)
{
	CDIALOG::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormDialog)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CFormDialog, CDIALOG)
	//{{AFX_MSG_MAP(CFormDialog)
	ON_WM_SIZE()
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_LBUTTONDOWN()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_KEYDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(IDR_REMOVE, OnRemove)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_COMMAND(WM_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_WM_CLOSE()
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_PROP_COLUMN, OnPropColumn)
	ON_COMMAND(ID_DEL_COLUMN, OnDelColumn)
	ON_COMMAND(ID_LEFT_COLUMN, OnLeftColumn)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_MESSAGE(WM_ENDSELECT,EndSelectMode)
	ON_MESSAGE(WM_ADDCONTROL,OnAddControl)
	ON_COMMAND(ID_ADD_COLUMN, OnAddColumn)
	ON_COMMAND(ID_RIGHT_COLUMN, OnRightColumn)
	ON_COMMAND(WM_POSTUPDATE, PostUpdate)
	//}}AFX_MSG_MAP

END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFormDialog message handlers

BOOL CFormDialog::OnInitDialog()
{
	CDIALOG::OnInitDialog();

	//SetIcon(m_hIcon, TRUE);			// Set big icon
	//SetIcon(m_hIcon, FALSE);		// Set small icon

	



	ReloadWindows();

	return TRUE;  // return TRUE  unless you set the focus to a control
}

void CFormDialog::OnSysCommand(UINT nID, LPARAM lParam)
{
	CDIALOG::OnSysCommand(nID, lParam);
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void CFormDialog::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CPaintDC pDC(this); 
			
		if(m_bGrid)
		{
			// Pen for grid
			CPen dashPen(PS_DASH, 1, 1);
			CPen* oldpen;
			oldpen = (CPen*)pDC.SelectObject(&dashPen);

			CRect Rect;
			GetClientRect(&Rect);
			for(int x=0;x<Rect.right;x+=m_nGridX)
			for(int y=0;y<Rect.bottom;y+=m_nGridY)
			{
				pDC.SetPixel(x,y, 0);
			}
			pDC.SelectObject(oldpen);
		}



		if(m_pTracker)
		{
			CDC *pDC = GetDC();
			m_pTracker->Draw(pDC);
			ReleaseDC(pDC);
		}

	}
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR CFormDialog::OnQueryDragIcon()
{
	return (HCURSOR) m_hIcon;
}








//////////////////////////////////////////////////////////////////////////
BOOL IsEditGrid(CWnd* pWnd,CPoint point,UINT nMsg)
{
	if(!pWnd)
		return 0;

	if(pWnd->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
	{
		CFormDialog *pThis=aThisList[pWnd->GetParent()];
		CGridCtrl *pGrid=(CGridCtrl *)pWnd;
		CCellID cell=pGrid->GetCellFromPt(point);

		if(nMsg == WM_LBUTTONUP||nMsg == WM_LBUTTONDOWN)
		{
			CRect Rect;
			pWnd->GetWindowRect(&Rect) ;
			pThis->ScreenToClient(&Rect) ;
			Rect.InflateRect(8,8);
			pThis->InvalidateRect(Rect);
		}

		if(cell.row==0)
		{
			if (nMsg == WM_LBUTTONUP||nMsg == WM_LBUTTONDBLCLK)
			{
				pThis->SelectItem(pWnd,2);
				if(nMsg == WM_LBUTTONDBLCLK)
				{
					pThis->ShowProperty(1);//показываем окно свойств без заполнения списка
				}
				else
					pThis->ShowProperty(0);
			}
			return 1;
		}

		if(nMsg == WM_MOUSEMOVE||nMsg == WM_LBUTTONUP)
		if(cell.row<4)
			return 1;
	}
	return 0;
}


//////////////////////////////////////////////////////////////////////////
//EDIT_FORM
//////////////////////////////////////////////////////////////////////////
// CDoubleSubclassDlg dialog

LRESULT CALLBACK CFormDialog::PREDblSubclassFunction(
	CWnd* pWnd, HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam, bool &bDoReturn)
{
	CPoint point;
	point.x=LOWORD(lParam);
	point.y=HIWORD(lParam);
	if(IsEditGrid(pWnd,point,nMsg))
		return 0;

	if (::GetCapture() != NULL)
		return 0;

	if(!pWnd)
		return 0;
	CFormDialog *pThis=aThisList[pWnd->GetParent()];
	if(!pThis)
		return 0;

	if (nMsg == WM_LBUTTONDOWN||nMsg == WM_MOUSEMOVE||nMsg == WM_CONTEXTMENU||nMsg == WM_RBUTTONDOWN)
	{
		pWnd->ClientToScreen(&point);
		pThis->ScreenToClient(&point);


		if (nMsg == WM_MOUSEMOVE)
		{
			pThis->OnMouseMove(wParam, point);
		}
		else
		if (nMsg == WM_LBUTTONDOWN)
		{
			pThis->OnLButtonDown(-1, point);
		}
		else
		if (nMsg == WM_RBUTTONDOWN)
		{
			point.x=LOWORD(lParam);
			point.y=HIWORD(lParam);
			pWnd->ClientToScreen(&point);
			pThis->OnContextMenu(pWnd,point);
		}
		else
		if (nMsg == WM_CONTEXTMENU)
		{
			point.x=LOWORD(lParam);
			point.y=HIWORD(lParam);
			pThis->OnContextMenu(pWnd,point);
		}
	}

	if (nMsg == WM_LBUTTONDBLCLK)
	{

		if(IsCTRLpressed())
		{
			BOOL bWasVisible=1;//AfxGetProperty()->IsWindowVisible();
			CDynControl* pElement=pThis->GetCurrenElement();
			if(pElement && bWasVisible)//переход на процедуру в модуле
			{
				CString csName;
				CString csFunction;
				if(pElement->GetFirstFunctionParams(csName,csFunction))
				{
					CString Str=GetNormalProcName(csFunction,csName,pElement->m_csName);
					csFunction.TrimRight();
					if(csFunction.IsEmpty())
					{
						pElement->SetFuncProperty(csName,Str);
					}

					GotoModuleProc(pElement->m_csName,Str);
					return 0;
				}
			}
		}
		pThis->ShowProperty(1);
		return 0;
	}
	if (nMsg == WM_PAINT)
	if(pThis->m_pTracker)
	{
		CDC *pDC = pThis->GetDC();
		pThis->m_pTracker->Draw(pDC);
		pThis->ReleaseDC(pDC);
	}


	if (nMsg == WM_SETFOCUS||
		nMsg == WM_CONTEXTMENU||
		nMsg == WM_LBUTTONDOWN||
		nMsg == WM_RBUTTONDOWN||
		nMsg == WM_LBUTTONUP||
		nMsg == WM_MOUSEMOVE||
		nMsg == WM_LBUTTONDBLCLK
		//nMsg == WM_SETCURSOR
		)
	{
		if(IsEditGrid(pWnd,point,nMsg))
			return 0;
		bDoReturn = true;
	}
	return 0;
}



bool CALLBACK CFormDialog::AllowSubclassCallBack(HWND hWnd)
{
	CWnd *pWnd=CWnd::FromHandle(hWnd);
	if(!pWnd)
		return 0;
	CFormDialog *pThis=aThisList[pWnd->GetParent()];
	if(!pThis)
		return 0;

	//корректируем видимость элементов в соответствии с их слоями
	for(int i=0;i<pThis->aControlList.GetSize();i++)
	{
		CDynControl *p=pThis->aControlList[i];
		if(p)
		if(p->GetWnd())
		if(p->GetWnd()->m_hWnd==hWnd)
		{
			int nShow=SW_HIDE;
			if(pThis->GetView()->IsVisibleLayer(p->m_csLayer))
				nShow=SW_NORMAL;
			p->GetWnd()->ShowWindow(nShow);

			if(SW_NORMAL==nShow)
			{
				pThis->aWindowsList.Add(hWnd);
				return true;
			}
		}
	}
	return false;
}

CWnd* CFormDialog::FindCurrentWindow(CPoint point) 
{
	CPoint cFindPoint=point;
	ClientToScreen(&cFindPoint);
	//поиск окна выделяемого объекта
	for(int i=0;i<aWindowsList.GetSize();i++)
	{
		CRect Rect;
		::GetWindowRect(aWindowsList[i],&Rect);
		if(Rect.PtInRect(cFindPoint))
		{
			HWND hWnd=aWindowsList[i];
			CWnd* pWnd=CWnd::FromHandle(hWnd);

			CString strClassName;
			GetClassName(hWnd, strClassName.GetBufferSetLength(255), 255);
			if(strClassName=="SysTabControl32")
			{
					//доп. проверка для слоев
					Rect.DeflateRect(10,10);
					Rect.top+=20;
					if(Rect.PtInRect(cFindPoint))
						continue;
			}
			else
			if(strClassName=="Button")
			{
				UINT uStyle = ((CButton*)pWnd)->GetButtonStyle();
				if (uStyle == BS_GROUPBOX)
				{
					//для рамок выполняем доп. проверку
					Rect.DeflateRect(10,10);
					if(Rect.PtInRect(cFindPoint))
						continue;

				}
			}
			return pWnd;
		}
	}
	return 0;
}	

void CFormDialog::UseGrid(CRect *lpRect)
{
	//if(IsCTRLpressed())//точное позиционирование
	if(m_bUseGrid)
	{
		if(m_nGridX>0)
		{
			lpRect->left=int(lpRect->left/m_nGridX)*m_nGridX;
			lpRect->right=int(lpRect->right/m_nGridX)*m_nGridX;
		}
		if(m_nGridY>0)
		{
			lpRect->top=int(lpRect->top/m_nGridY)*m_nGridY;
			lpRect->bottom=int(lpRect->bottom/m_nGridY)*m_nGridY;
		}
	}

	if(lpRect->Width()<MIN_SIZE)
		lpRect->right=lpRect->left+MIN_SIZE;
	if(lpRect->Height()<MIN_SIZE)
		lpRect->bottom=lpRect->top+MIN_SIZE;
}

int CFormDialog::GetCountSelectedList()
{
	if(m_pTracker)
		return m_pTracker->aList.GetSize();
	return 0;
}
void CFormDialog::GroupsSelectedList(int nMode)
{
	if(bReadOnly)
		return;
	if(m_pTracker)
	{
		m_pTracker->Invalidates(this);

		if(nMode==ID_CNTERINVIEW_HORIZ||nMode==ID_CNTERINVIEW_VERT)
		{
			CPoint point_min=m_pTracker->m_rect.TopLeft();
			CPoint point_max=m_pTracker->m_rect.BottomRight();

			//находим мин и макс точки
			for(int i=0;i<m_pTracker->aList.GetSize();i++)
			{
				CRect *pRect=&m_pTracker->aList[i];
				point_min.x=min(point_min.x,pRect->left);
				point_min.y=min(point_min.y,pRect->top);

				point_max.x=max(point_max.x,pRect->right);
				point_max.y=max(point_max.y,pRect->bottom);
			}
			//по ним определяем центр выделенной группировки
			CPoint point_center;
			point_center.x=(point_max.x-point_min.x)/2+point_min.x;
			point_center.y=(point_max.y-point_min.y)/2+point_min.y;

			//определяем настоящий центр формы и вычисляем дельта отклонения
			CRect RectC;
			GetClientRect(RectC);
			int dx=-point_center.x+RectC.Width()/2;
			int dy=-point_center.y+RectC.Height()/2;

			//выполняем перемещение

			for(	i=0;i<m_pTracker->aList.GetSize();i++)
			{
				CWnd *pWnd=m_pTracker->aWindows[i];
				if(pWnd)
				{
					CRect *pRect=&m_pTracker->aList[i];
					if(nMode==ID_CNTERINVIEW_HORIZ)
						pRect->OffsetRect(dx,0);
					else
					if(nMode==ID_CNTERINVIEW_VERT)
						pRect->OffsetRect(0,dy);

					if(m_pTracker->pCurWnd==pWnd)
						m_pTracker->m_rect=m_pTracker->aList[i];

					pWnd->MoveWindow(pRect->left,pRect->top, pRect->Width(), pRect->Height(),1);
				}
			}

		}
		else
		{
			CRect m_rect=m_pTracker->m_rect;
			for(int i=0;i<m_pTracker->aList.GetSize();i++)
			{
				CWnd *pWnd=m_pTracker->aWindows[i];
				if(pWnd)
				{
					CRect *pRect=&m_pTracker->aList[i];
					if(nMode==ID_DYNBUTTONLEFTALIGN)//Left
						pRect->OffsetRect(m_rect.left-pRect->left,0);
					else
					if(nMode==ID_DYNBUTTONRIGHTALIGN)//Right
						pRect->OffsetRect(m_rect.right-pRect->right,0);
					else
					if(nMode==ID_DYNBUTTONTOPALIGN)//Top
						pRect->OffsetRect(0,m_rect.top-pRect->top);
					else
					if(nMode==ID_DYNBUTTONBOTTOMALIGN)//Bottom
						pRect->OffsetRect(0,m_rect.bottom-pRect->bottom);
					else
					if(nMode==ID_DYNBUTTONMAKESAMEWIDTH)
					{
						pRect->right=pRect->left+m_rect.Width();
					}
					else
					if(nMode==ID_DYNBUTTONMAKESAMEHEIGHT)
					{
						pRect->bottom=pRect->top+m_rect.Height();
					}
					else
					if(nMode==ID_DYNBUTTONMAKESAMESIZE)
					{
						pRect->right=pRect->left+m_rect.Width();
						pRect->bottom=pRect->top+m_rect.Height();
					}

					pWnd->MoveWindow(pRect->left,pRect->top, pRect->Width(), pRect->Height(),1);
				}
			}
		}
		GetView()->SetModifiedFlag(1);
	}

}
void CFormDialog::MoveSelectedWindows(int dx,int dy,int bResize)
{
	if(bReadOnly)
		return;

	if(m_pTracker)
	{
		if(dx||dy)
			m_pTracker->Invalidates(this);
		for(int i=0;i<m_pTracker->aList.GetSize();i++)
		{
			CWnd *pWnd=m_pTracker->aWindows[i];
			if(pWnd)
			{
				if(m_pTracker->pCurWnd==pWnd)
				{
					m_pTracker->m_rect.NormalizeRect();
					m_pTracker->aList[i]=m_pTracker->m_rect;
				}
				UseGrid(&m_pTracker->aList[i]);
				
				//перемещение
				if(bResize)
				{
					m_pTracker->aList[i].right+=dx;
					m_pTracker->aList[i].bottom+=dy;
				}
				else
					m_pTracker->aList[i].OffsetRect(dx,dy);
				
				if(m_pTracker->pCurWnd==pWnd)
					m_pTracker->m_rect=m_pTracker->aList[i];

				CRect Rect=m_pTracker->aList[i];
				//pWnd->SetWindowPos(NULL, Rect.left,Rect.top, Rect.Width(), Rect.Height(), SWP_NOZORDER);
				pWnd->MoveWindow(Rect.left,Rect.top, Rect.Width(), Rect.Height(),1);
			}
		}
		GetView()->SetModifiedFlag(1);
	}
}
void CFormDialog::SelectItem(CWnd* pWnd,int nMode) 
{
	GetParent()->SendMessage(WM_ENDSELECT);//отменяем общее выделение формы
	if(!nMode)
		SetCursor(AfxGetApp()->LoadStandardCursor (IDC_SIZEALL));

	POINT CurPoint;
	GetCursorPos(&CurPoint);
	pWnd->ScreenToClient(&CurPoint);	

	if (m_pTracker)
		m_pTracker->Invalidates(this);
	pWnd->Invalidate();


	CRect Rect;
	pWnd->GetWindowRect(&Rect) ;
	ScreenToClient(&Rect) ;

	m_pEditedCtrl = pWnd;
	if(!m_pTracker)
		m_pTracker = new CMultiRectTracker();
	m_pTracker->Init(pWnd,Rect,-1,nMode);
	m_pTracker->bNotNegative=1;

	if(bReadOnly==0 && nMode==0)
	{
		CPoint point;
		pWnd->GetWindowRect(&Rect) ;
		ScreenToClient(&Rect) ;
		point.x=Rect.left+CurPoint.x;
		point.y=Rect.top+CurPoint.y;
		
		m_pTracker->SetGrid(m_bUseGrid,m_nGridX,m_nGridY);
		BOOL bRes=m_pTracker->Track(this, point, TRUE);
		if(bRes)
		{
			MoveSelectedWindows();
		}
	}
	if(m_pTracker)
		m_pTracker->Invalidates(this);
}

void CFormDialog::ReloadWindows()
{
	aWindowsList.RemoveAll();
	CDblSubclassWnd::SubclassChildsRecurs(::GetWindow(m_hWnd ,GW_CHILD), AllowSubclassCallBack);
	CDblSubclassWnd::SetDblSubclassWndProc(PREDblSubclassFunction, 0);
}
CDynControl *CFormDialog::AddControl(int nType,CRect &Rect,bool bGridAllign)
{
	if(nType!=ID_BUTTONFORMOBJECT)
		csCurrentObjectName="";
	CDynControl *p=new CDynControl(this, nType,1,csCurrentObjectName);
	p->m_csLayer=GetView()->GetLayerName();
	Rect.NormalizeRect();
	if(bGridAllign)
		UseGrid(&Rect);
	p->AddControl(Rect);
	aControlList.Add(p);
	return p;
}
CDynControl *CFormDialog::AddControl(int nType,CPoint point)
{
	if(nType!=ID_BUTTONFORMOBJECT)
		csCurrentObjectName="";
	CDynControl *p=new CDynControl(this, nType, 1,csCurrentObjectName);
	p->m_csLayer=GetView()->GetLayerName();
	
	CRect Rect;
	Rect.left=point.x;
	Rect.top=point.y;
	Rect.right=Rect.left+20;
	Rect.bottom=Rect.top+20;
	UseGrid(&Rect);
	point=Rect.TopLeft();

	p->AddControl(&point);
	aControlList.Add(p);
	return p;
}

//////////////////////////////////////////////////////////////////////////
void CFormDialog::OnLButtonDown(UINT nFlags, CPoint point) 
{
	GetParent()->SetFocus();
	bool bWasAddControl=0;
	if(nCurrentMode>0)//режим добавления нового элемента
	{
		if(bReadOnly)
			return;

		EndSelectMode(0,0);
		bWasAddControl=1;
		CRect Rect;
		Rect.left=Rect.right=point.x;
		Rect.top=Rect.bottom=point.y;
		CMultiRectTracker PosSelector(Rect,CRectTracker::solidLine  | CRectTracker::resizeOutside);
		PosSelector.SetGrid(m_bUseGrid,m_nGridX,m_nGridY);
		BOOL bRes=PosSelector.Track(this, point, TRUE);
		
		Rect=PosSelector.m_rect;
		if(bRes)
		if((abs(Rect.right-point.x)<=m_nGridX/2)&&(abs(Rect.bottom-point.y)<=m_nGridY/2))
			bRes=0;

		if(bRes)
		{
			CRect Rect2;
			Rect2.left=min(Rect.left,Rect.right);
			Rect2.right=max(Rect.left,Rect.right);
			Rect2.top=min(Rect.top,Rect.bottom);
			Rect2.bottom=max(Rect.top,Rect.bottom);

			AddControl(nCurrentMode,Rect2);
			point.x=Rect2.left;
			point.y=Rect2.top;
		}
		else
		{
			AddControl(nCurrentMode,point);
		}

		nCurrentMode=0;
		ReloadWindows();
		GetView()->SetModifiedFlag(1);

	}


	bool bSelectItem=1;
	if(nFlags>=0)
	if(m_pTracker)
	{
		int nHit=(CRectTracker::TrackerHit)m_pTracker->HitTestHandles(point);
		if(nHit>=0&&nHit<8)
		{
			bSelectItem=0;
		}
	}

	
	if(bSelectItem||IsCTRLpressed()||IsSHIFTpressed())//режим выделения элементов
	{
		CWnd* pWnd=FindCurrentWindow(point);
		if(pWnd)
		{
			if(pWnd->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
			{
				CGridCtrl *pGrid=(CGridCtrl *)pWnd;
				pGrid->ResetSelectedRange();

			}

			SelectItem(pWnd,bWasAddControl);
			ShowProperty(0);
			return;
		}
	}
	ShowProperty(0);

	bool bSendMessage=1;
	bool bDeleteTracker=0;
	if (bReadOnly==0 && m_pTracker)//режим изменения размеров
	{
		bSendMessage=0;
		m_pTracker->SetGrid(m_bUseGrid,m_nGridX,m_nGridY);
		BOOL bRes=m_pTracker->Track(this, point, TRUE);
		if(bRes)
		{
			Invalidate(FALSE);		

			m_pTracker->m_rect.NormalizeRect();
			UseGrid(&m_pTracker->m_rect);

			CDC* pDC = GetDC();
			m_pTracker->Draw(pDC);

			MoveSelectedWindows();
		}
		else
		if(bSelectItem)
		{
			bSendMessage=1;
			bDeleteTracker=1;
		}

	}

	if(bSendMessage&&nFlags!=-2)
	{
		GetParent()->PostMessage(WM_LBUTTONDOWN,0,MAKELPARAM(point.x,point.y));
	}
	else
	if(bDeleteTracker)
	{
		EndSelectMode(0,0);
	}

	CDIALOG::OnLButtonDown(nFlags, point);
}

BOOL CFormDialog::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(!bReadOnly)
	if (m_pTracker)
		if (pWnd == this && m_pTracker->SetCursor(this, nHitTest))
			return TRUE;
	
	if(nCurrentMode>0)
	{
		if(nCurrentMode==ID_DYNBUTTONEDIT)
			SetCursor(AfxGetApp()->LoadCursor (IDC_EDITCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONLISTBOX)
			SetCursor(AfxGetApp()->LoadCursor (IDC_LISTCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONLABEL)
			SetCursor(AfxGetApp()->LoadCursor (IDC_LABELCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONCOMBO)
			SetCursor(AfxGetApp()->LoadCursor (IDC_COMBOCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONCHECK)
			SetCursor(AfxGetApp()->LoadCursor (IDC_CHECKCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONRADIO)
			SetCursor(AfxGetApp()->LoadCursor (IDC_RADIOCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONDATE)
			SetCursor(AfxGetApp()->LoadCursor (IDC_DATECURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONGROUP)
			SetCursor(AfxGetApp()->LoadCursor (IDC_GROUPCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTON)
			SetCursor(AfxGetApp()->LoadCursor (IDC_BUTTONCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONGRID)
			SetCursor(AfxGetApp()->LoadCursor (IDC_GRIDCURSOR));
		else
		if(nCurrentMode==ID_DYNBUTTONTREE)
			SetCursor(AfxGetApp()->LoadCursor (IDC_TREECURSOR));
		else
		if(nCurrentMode==ID_BUTTONPROGRESS)
			SetCursor(AfxGetApp()->LoadCursor (IDC_PROGRESS));

		else
		if(nCurrentMode==ID_BUTTONSLIDER)
			SetCursor(AfxGetApp()->LoadCursor (IDC_SLIDER));
		else
		if(nCurrentMode==ID_BUTTONFORMOBJECT)
			SetCursor(AfxGetApp()->LoadCursor (IDC_FORMOBJECT));
		else
		if(nCurrentMode==ID_BUTTON_TAB)
			SetCursor(AfxGetApp()->LoadCursor (IDC_BUTTON_TAB));


		

		return TRUE;
	}
	//BUG - переполнение стека в W98:
//	else
//		return CDIALOG::OnSetCursor(pWnd, nHitTest, message);

	else
	if(IsWindows5())
	{
		return CDIALOG::OnSetCursor(pWnd, nHitTest, message);
	}


	return FALSE;
}





void CFormDialog::OnMouseMove(UINT nFlags, CPoint point) 
{
	ptContextMenu.x=-1;

	bool bSetCursor=1;
	if(m_pTracker)
	{
		int nHit=(CRectTracker::TrackerHit)m_pTracker->HitTestHandles(point);
		if(nHit>=0&&nHit<8)
		{
			bSetCursor=0;
		}
	}
	if(bSetCursor)
	if(FindCurrentWindow(point))
		SetCursor(AfxGetApp()->LoadStandardCursor (IDC_SIZEALL));

	GetParent()->SendMessage(WM_MOUSEMOVE,0,MAKELPARAM(point.x,point.y));
	CDIALOG::OnMouseMove(nFlags, point);



	if(treeDragItem)
	{
		if(GetFocus()!=this)
		{
			GetView()->GetParentFrame()->BringWindowToTop();
			SetFocus();
		}
		if(nFlags==0)
		{
			nCurrentMode=0;
			if(AfxGetMetadataTree())
				AfxGetMetadataTree()->EndDrag();
			return;
		}
		nCurrentMode=ID_DYNBUTTONEDIT;
	}

}

void CFormDialog::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(treeDragItem)
	{
		ptContextMenu=point;
		OnEditPaste();


		nCurrentMode=0;
		if(AfxGetMetadataTree())
			AfxGetMetadataTree()->EndDrag();
		return;
	}

	GetParent()->SendMessage(WM_LBUTTONUP,0,MAKELPARAM(point.x,point.y));
	CDIALOG::OnLButtonUp(nFlags, point);
}

void CFormDialog::EndSelectMode(WPARAM wParam, LPARAM lParam)
{
	if(m_pTracker)
	{
		AfxGetProperty()->Clear();

		m_pTracker->Invalidates(this);
		delete m_pTracker;
		m_pTracker=0;
		m_pEditedCtrl=0;
	}

	ReloadWindows();
}
void CFormDialog::OnAddControl(WPARAM wParam, LPARAM lParam)
{
	if(bReadOnly)
		return;

	nCurrentMode=0;
	if(wParam==ID_BUTTONFORMOBJECT)
	{
		csCurrentObjectName="";

	}
	nCurrentMode=wParam;
}

void CFormDialog::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(bReadOnly && nChar!=VK_TAB && nChar!=VK_ENTER)
		return;

	int nDX=m_nGridX;
	int nDY=m_nGridY;
	if(nDX==0)
		nDX=5;
	if(nDY==0)
		nDY=5;

	bool _m_bUseGrid=m_bUseGrid;
	if(IsCTRLpressed()||IsSHIFTpressed())//точное позиционирование
	{
		m_bUseGrid=0;
		nDX=1;
		nDY=1;
	}
	int bShift=0;
	if(IsSHIFTpressed())
		bShift=1;


	switch(nChar)
	{
		case VK_LEFT:
		MoveSelectedWindows(-nDX,0,bShift);
		break;
		case VK_RIGHT:
		MoveSelectedWindows(nDX,0,bShift);
		break;

		case VK_UP:
		MoveSelectedWindows(0,-nDY,bShift);
		break;

		case VK_DOWN:
		MoveSelectedWindows(0,nDY,bShift);
		break;

		case VK_ESCAPE:
		nCurrentMode=0;
		treeDragItem=0;
		break;

		case VK_DELETE:
		OnRemove();
		break;

		case VK_ENTER:
		OnProperty();
		break;

		case VK_TAB:
			{
				CDynControl* p=GetNextElement(bShift);
				if(p)
				{
					SelectItem(p->GetWnd(),-1);
					ShowProperty(0);
				}
			}
		break;
	}


	m_bUseGrid=_m_bUseGrid;

	
	CDIALOG::OnKeyDown(nChar, nRepCnt, nFlags);
	GetParent()->SetFocus();
}

void CFormDialog::OnRButtonDown(UINT nFlags, CPoint point) 
{
	nCurrentMode=0;
	EndSelectMode(0,0);
	CDIALOG::OnRButtonDown(nFlags, point);
}

void CFormDialog::OnContextMenu(CWnd* pWnd, CPoint point) 
{
	CMenu menu;
	CMenu* pPopup;
	AfxCurrentGrid=0;
	AfxCurrentCol=-1;

	VERIFY(menu.LoadMenu(IDR_ALL_EDIT_MENU));
	pPopup = menu.GetSubMenu(3);//Form - menu
	CWnd* pWndPopupOwner = this;//GetParent();
	if(pWnd->IsKindOf(RUNTIME_CLASS(CGridCtrl)))
	{
		SelectItem(pWnd,2);
		CGridCtrl *pGrid=(CGridCtrl *)pWnd;
		CPoint point2=point;
		//ClientToScreen(&point2);
		pGrid->ScreenToClient(&point2);
		CCellID cell=pGrid->GetCellFromPt(point2);
		if(cell.row<2)
		{
			if(cell.col>=0)
				pGrid->SelectColumns(cell);
			AfxCurrentGrid=pGrid;
			AfxCurrentCol=cell.col;
			pPopup = menu.GetSubMenu(1);//Grid Popup - menu

			SetMenuDefaultItem(pPopup->m_hMenu,ID_PROP_COLUMN,MF_BYCOMMAND);

			pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
				pWndPopupOwner);
			return;
		}
		pGrid->ResetSelectedRange();
	}



	nCurrentMode=0;

	CPoint point2=point;
	ScreenToClient(&point2);
	ptContextMenu=point2;
	CWnd* pItemWnd=FindCurrentWindow(point2);
	if(!pItemWnd && pWnd!=this)
	{
		ClientToScreen(&point);
		CDynControl* p=GetCurrenElement();
		if(p)
			pItemWnd=p->m_pWnd;
	}

	if(pItemWnd)
	{
		bool bFind=0;
		if(m_pTracker)
		{
			for(int i=0;i<m_pTracker->aList.GetSize();i++)
			{
				if(pItemWnd==m_pTracker->aWindows[i])
				{
					bFind=1;
					break;
				}
			}
		}
		if(!bFind)
		{
			if(m_pTracker)
			{
				m_pTracker->Invalidates(this);
				delete m_pTracker;
			}
			m_pTracker = new CMultiRectTracker();
		}
		SelectItem(pItemWnd,2);
		ShowProperty(0);

		pPopup->EnableMenuItem(ID_PROPERTY,MF_BYCOMMAND|MF_ENABLED );
		pPopup->EnableMenuItem(IDR_REMOVE,MF_BYCOMMAND|MF_ENABLED );
		pPopup->EnableMenuItem(ID_EDIT_CUT,MF_BYCOMMAND|MF_ENABLED );
		pPopup->EnableMenuItem(ID_EDIT_COPY,MF_BYCOMMAND|MF_ENABLED );
		pPopup->EnableMenuItem(ID_EDIT_PASTE,MF_BYCOMMAND|MF_ENABLED );

		SetMenuDefaultItem(pPopup->m_hMenu,ID_PROPERTY,MF_BYCOMMAND);
	}
	if(ValCopyPaste.GetSizeArray()>0)
		pPopup->EnableMenuItem(ID_EDIT_PASTE,MF_BYCOMMAND|MF_ENABLED );

	pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
		pWndPopupOwner);
	if(m_pTracker)
		m_pTracker->Invalidates(this);


}

void CFormDialog::ClearData() 
{
	AfxGetProperty()->ReleaseDoc(GetDocument());
	if(m_pTracker)
	{
		m_pTracker->Invalidates(this);
		delete m_pTracker;
		m_pTracker=0;
	}
	for(int i=0;i<aControlList.GetSize();i++)
	{
		delete aControlList[i];
	}
	aControlList.RemoveAll();
}

void CFormDialog::OnRemove() 
{
	if(bReadOnly)
		return;
	if(m_pTracker) 
	{
		AfxGetProperty()->ReleaseDoc(GetDocument());

		m_pTracker->Invalidates(this);
		for(int i=0;i<m_pTracker->aList.GetSize();i++)
		{
			for(int j=0;j<aControlList.GetSize();j++)
			if(aControlList[j]->GetWnd()==m_pTracker->aWindows[i])
			{
				delete aControlList[j];
				aControlList.RemoveAt(j);
				break;
			}
		}
		GetView()->SetModifiedFlag(1);


		delete m_pTracker;
		m_pTracker=0;
		m_pEditedCtrl=0;

		ReloadWindows();
	}
}

CDynControl* CFormDialog::GetCurrenElement()
{
	if(m_pTracker)
	for(int j=0;j<aControlList.GetSize();j++)
	if(aControlList[j]->GetWnd()==m_pTracker->pCurWnd)
	{
		return aControlList[j];
	}
	return 0;
}
CDynControl* CFormDialog::GetNextElement(int nPrev)
{
	int nCurrent=0;
	if(m_pTracker)
	for(int j=0;j<aControlList.GetSize();j++)
	if(aControlList[j]->GetWnd()==m_pTracker->pCurWnd)
	{
		nCurrent=j+1-2*nPrev;
	}
	if(nCurrent<0)
		nCurrent=aControlList.GetSize()-1;
	if(nCurrent>=aControlList.GetSize())
		nCurrent=0;

	if(nCurrent>=0 && nCurrent < aControlList.GetSize())
		return aControlList[nCurrent];

	return 0;
}


void CFormDialog::OnProperty() 
{
	ShowProperty(1);
}

void CFormDialog::ShowProperty(int nShow) 
{
	AfxGetProperty()->Clear();
	CDynControl *pControl=GetCurrenElement();
	if(pControl)
	{
		LoadObjectsList();

		//загружаем список слоев
		static CString Str;
		Str=GetView()->GetStrLayers();
		aChooseList[nTypeLayers]=Str.GetBuffer(0);
		


		AfxGetProperty()->SetDoc(GetDocument(),(LPARAM)pControl,(LPARAM)this,bReadOnly);
		if(AfxGetProperty()->IsWindowVisible()==0&&nShow)
			((CMainFrame*)AfxGetMainWnd())->ShowPropertyBar(TRUE);

		//подготавливаем имя модуля (для определения переменных метаданных)
		CModuleDoc *pDoc=(CModuleDoc *)GetDocument();
		CString csModuleName=pDoc->GetModulePath();

		//подготавливаем список переменных на форме
		CString csVarValues="...";
		for(int j=0;j<aControlList.GetSize();j++)
		if(aControlList[j]->GetWnd())
		{
			if(!aControlList[j]->m_csName.IsEmpty())
				csVarValues=csVarValues+","+aControlList[j]->m_csName;
		}

		pControl->AddShowProperty(AfxGetProperty(),csModuleName,csVarValues);
	}
	else
	{
		GetParent()->SendMessage(WM_SHOWPROPERTY,nShow);//ShowProperty
	}
}
void CFormDialog::OnUpdate() 
{
	PostMessage(WM_COMMAND,WM_POSTUPDATE,0);
}
void CFormDialog::PostUpdate()
{
	CDynControl *pControl=(CDynControl *)AfxGetProperty()->GetDocData();
	if(AfxGetProperty()->GetDocType()==(LPARAM)this)
	if(pControl)
	{
		CWnd *pPrevWnd=pControl->m_pWnd;
		pControl->OnUpdate();
		if(pPrevWnd!=pControl->m_pWnd)//объект поменял окно
		{
			AfxGetProperty()->Clear();
			if(m_pTracker)
			{
				SelectItem(pControl->m_pWnd,1);
			}
			//ShowProperty(0);
			ReloadWindows();
		}
	}
}

void CFormDialog::SaveAllToValue(CValue &AllElements)
{

	AutoSetDlgItem();

	//Сортируем по nCreate
	SortControlList(aControlList);
	for(int j=0;j<aControlList.GetSize();j++)
	if(aControlList[j]->GetWnd())
	{
		CDynControl *pControl=aControlList[j];
		CValue ObjectVal;
		ObjectVal.CreateObject("Структура");
		pControl->SaveToValue(ObjectVal);
		AllElements.SetAt(j+1,ObjectVal);
		CValue v=AllElements.GetAt(j+1);
	}
}


CDynControl *CFormDialog::AddControl(CValue ObjectVal)
{
	CDynControl *pControl=new CDynControl(this, 0, 1);
	pControl->AddControl(ObjectVal);
	aControlList.Add(pControl);
	return pControl;
}

void CFormDialog::OnEditCopy() 
{
	if(m_pTracker)
	{
		int n=0;
		ValCopyPaste.CreateObject("Структура");
		for(int i=0;i<m_pTracker->aList.GetSize();i++)
		{
			for(int j=0;j<aControlList.GetSize();j++)
			if(aControlList[j]->GetWnd()==m_pTracker->aWindows[i])
			{
				CDynControl *pControl=aControlList[j];
				CValue ObjectVal;
				ObjectVal.CreateObject("Структура");
				pControl->SaveToValue(ObjectVal);
				ValCopyPaste.SetAt(n+1,ObjectVal);
				n++;
				break;
			}
		}
	}
	GetParent()->SetFocus();
}

void CFormDialog::OnEditPaste() 
{ 
	if(bReadOnly)
		return;

	if(ValCopyPaste.GetSizeArray()==0)
		return;
	if(m_pTracker)
	{
		m_pTracker->Invalidates(this);
		delete m_pTracker;
	}
	m_pTracker=0;
	m_pEditedCtrl=0;

	CPoint point;

	if(ptContextMenu.x!=-1)
		point=ptContextMenu;
	else
	{
		GetCursorPos(&point);
		ScreenToClient(&point);	
	}


	int min_x=9999;
	int min_y=9999;

	m_pTracker = new CMultiRectTracker();
	GetParent()->SendMessage(WM_ENDSELECT);//отменяем общее выделение формы
	for(int i=0;i<ValCopyPaste.GetSizeArray();i++)
	{
		CDynControl *pControl=AddControl(ValCopyPaste.GetAt(i+1));
		CRect Rect;
		Rect.left=pControl->m_x1;
		Rect.right=pControl->m_x2;
		Rect.top=pControl->m_y1;
		Rect.bottom=pControl->m_y2;
		CWnd *pWnd=pControl->GetWnd();

		min_x=min(min_x,Rect.left);
		min_y=min(min_y,Rect.top);

		m_pTracker->Init(pWnd,Rect,-1,1);//0-Remove+Add+Move,1-Add+Remove,2-Add

		pControl->OnUpdate();

		//При нажатом CTRL вставляется только один элемент
//		if(IsPressed(VK_CONTROL))
		{
			//break;
		}

	}

	CRect Rect;
	GetClientRect(Rect);
	if(point.x<0||point.y<0||point.x+5>Rect.right||point.y+5>Rect.bottom)
	{
		point.x=min_x+10;
		point.y=min_y+10;
	}
	MoveSelectedWindows(point.x-min_x,point.y-min_y);
	
	ReloadWindows();

	ptContextMenu.x=-1;
	GetParent()->SetFocus();
}

void CFormDialog::OnEditCut() 
{
	if(bReadOnly)
		return;

	OnEditCopy();
	OnRemove();
	GetParent()->SetFocus();
}


void CFormDialog::OnCancel() 
{
//	CDialog::OnCancel();
	GetParent()->SetFocus();
}

void CFormDialog::OnClose() 
{
	CDialog::OnCancel();
}
void CFormDialog::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	if(bReadOnly)
		return;

	GetParent()->SendMessage(WM_LBUTTONDBLCLK,0,MAKELPARAM(point.x,point.y));
	CDIALOG::OnLButtonDblClk(nFlags, point);
	GetParent()->SetFocus();
}


void CFormDialog::OnAddColumn()
{
	if(bReadOnly)
		return;

	if(AfxCurrentGrid)
	{
		int nCol=AfxCurrentGrid->InsertColumn("",DT_LEFT|DT_VCENTER|DT_SINGLELINE,AfxCurrentCol);
		if(nCol==-1)
			nCol=AfxCurrentGrid->GetColumnCount()-1;
		AfxCurrentGrid->SetColumnWidth(nCol,130);
		AfxCurrentGrid->Invalidate();
		GetView()->SetModifiedFlag(1);
	}
}

void CFormDialog::OnPropColumn() 
{
	if(AfxCurrentGrid&&AfxCurrentCol>=0)
	{
		AfxCurrentGrid->SelectColumns(AfxCurrentCol);
		ShowProperty(1);
	}
}

void CFormDialog::OnDelColumn() 
{
	if(bReadOnly)
		return;

	if(AfxCurrentGrid&&AfxCurrentCol>=0)
	{
		AfxCurrentGrid->DeleteColumn(AfxCurrentCol);
		AfxCurrentGrid->Invalidate();
		GetView()->SetModifiedFlag(1);
	}
}

void CFormDialog::CopyColumn(int nCol1,int nCol2) 
{
	CGridCtrl *pGrid=AfxCurrentGrid;
	CGridCell* pCell1 = (CGridCell*)pGrid->GetCell(0,nCol1);
	CGridCell* pCell2 = (CGridCell*)pGrid->GetCell(0,nCol2);
	if (pCell1&&pCell2)
	{
		pGrid->aColumns[nCol1].csName=pGrid->aColumns[nCol2].csName;
		pCell1->TextData()=pCell2->TextData();
		pGrid->aColumns[nCol1].csType=pGrid->aColumns[nCol2].csType;
		pGrid->aColumns[nCol1].bVisible=pGrid->aColumns[nCol2].bVisible;
		pGrid->aColumns[nCol1].bEnable=pGrid->aColumns[nCol2].bEnable;
		pGrid->aColumns[nCol1].bTab=pGrid->aColumns[nCol2].bTab;
		pGrid->m_arColWidths[nCol1]=pGrid->m_arColWidths[nCol2];
	}
}
void CFormDialog::OnLeftColumn() 
{
	if(bReadOnly)
		return;

	if(AfxCurrentGrid&&AfxCurrentCol>0)
	{
		int nCol=AfxCurrentCol-1;
		AfxCurrentGrid->InsertColumn("",DT_LEFT|DT_VCENTER|DT_SINGLELINE,nCol);

		CopyColumn(nCol,nCol+2);
		AfxCurrentGrid->DeleteColumn(nCol+2);
		AfxCurrentGrid->Invalidate();
		GetView()->SetModifiedFlag(1);
	}
}

void CFormDialog::OnRightColumn() 
{
	if(bReadOnly)
		return;

	if(AfxCurrentGrid&&AfxCurrentCol>=0)
	if(AfxCurrentCol+1<AfxCurrentGrid->GetColumnCount())
	{
		int nCol=AfxCurrentCol+2;
		AfxCurrentGrid->InsertColumn("",DT_LEFT|DT_VCENTER|DT_SINGLELINE,nCol);
		CopyColumn(nCol,nCol-2);

		AfxCurrentGrid->DeleteColumn(nCol-2);
		AfxCurrentGrid->Invalidate();
		GetView()->SetModifiedFlag(1);
	}
}

void CFormDialog::BringToFront(int nMode) 
{
	if(bReadOnly)
		return;

	int k;
	if(m_pTracker)
	{
		m_pTracker->Invalidates(this);

		//Сортируем по ID
		SortControlList(aControlList);

		nUniqueID=FORM_START_ID+m_pTracker->aWindows.GetSize();
		for(k=0;k<aControlList.GetSize();k++)
		{
			CDynControl *pControl=aControlList[k];
			nUniqueID++;
			pControl->nCreate=nUniqueID;
		}


		for(k=0;k<aControlList.GetSize();k++)
		{
			CDynControl *pControl=aControlList[k];
			for(int i=0;i<m_pTracker->aWindows.GetSize();i++)
			{
				if(pControl->m_pWnd==m_pTracker->aWindows[i])
				{//выделенный элемент - перемещаем его вниз или вверх
					if(nMode)
					{
						nUniqueID++;
						pControl->nCreate=nUniqueID;//вверх
						::SetWindowPos(pControl->m_pWnd->m_hWnd,HWND_BOTTOM,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
					}
					else
					{
						pControl->nCreate=k;//вниз
						::SetWindowPos(pControl->m_pWnd->m_hWnd,HWND_TOP,0,0,0,0,SWP_NOSIZE | SWP_NOMOVE);
					}
				}

				
			}
			ReloadWindows();
		}
	}
}

void CFormDialog::AutoSetDlgItem() 
{
	int k;
	CArray <CDynControl*,CDynControl*> tempList;
	tempList.Copy(aControlList);
	for(k=0;k<aControlList.GetSize();k++)
	{
		CDynControl *pControl=aControlList[k];
		pControl->m_pWnd->GetWindowRect(pControl->m_Rect);
	}

	//Сортируем по расположению
	PosSortControlList(aControlList);

	nUniqueID=FORM_START_ID;
	for(k=0;k<aControlList.GetSize();k++)
	{
		CDynControl *pControl=aControlList[k];
		nUniqueID++;
		pControl->m_nID=nUniqueID;
		//pControl->m_pWnd->SetDlgCtrlID(nUniqueID);
	}
	ReloadWindows();
}


void CFormDialog::OnSize(UINT nType, int cx, int cy) 
{
/*	SIZE size;
	size.cx=cx-5;
	size.cy=cy-5;
	SetScrollSizes( MM_TEXT, size,size,size);
	GetParentFrame()->RecalcLayout();
*/

}