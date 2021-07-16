//
//	класс CFListCtrl		(файл FListCtrl.cpp)
//	дерево-список
//
//	версия:		2.0
//	дата:		12/04/2002
//	автор:		Денис Солоненков
//	e-mail:		2Los@land.ru
//
//
//
//

#include "stdafx.h"

#include "FListCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


//	УПРАВЛЕНИЕ ЦВЕТОМ
#define FL_ISCOLOR(x)		(!((x)&FL_NOCOLOR))		
#define FL_CONTROL_MASK (FL_COMBOBOX|FL_EDIT)

//	КОНСТАНТЫ
#define	FL_BORDER_SPACE	3						//GetFRect(FL_RC_TEXT)
#define FL_STEP			(m_nTextLineHeight+4)
#define FL_EV_SPACE		10						//EnsureVisible
#define FL_DROPDOWN_SIZE	10

// DRAW ELEMENT
#define FLDR_FITEM_BACKGROUND	401
#define FLDR_FITEM_TREEBUTTON	402
#define FLDR_FITEM_CHECKBOX		403
#define FLDR_FITEM_IMAGE		404
#define FLDR_FITEM_BUTTON		405
#define FLDR_FITEM_TEXT			406
#define FLDR_FITEM_FRAME		407
#define FLDR_BACKGROUND			408
#define FLDR_UNDERHEADER		409
#define FLDR_FCOLUMN_BACKGROUND	410

//
#define FL_ISOK(lr)			(!((lr)&FL_CANCEL))
#define FL_ISCONTINUE(lr)	((lr)&FL_CONTINUE)

//
#define FL_ISGRID() (GetExtendedStyle()&LVS_EX_GRIDLINES)
#define FL_ISFLAT() (GetExtendedStyle()&LVS_EX_FLATSB)

#define FL_ISEDIT(ctype) (((ctype)&FL_EDIT)==FL_EDIT && ((ctype)&FL_COMBOBOX)!=FL_COMBOBOX)
#define FL_ISBUTTON(ctype) (((ctype)&FL_BUTTON)==FL_BUTTON && ((ctype)&FL_COMBOBOX)!=FL_COMBOBOX)
#define FL_ISCOMBOLIST(ctype) (((ctype)&FL_COMBOBOX)==FL_COMBOBOX && ((ctype)&FL_EDIT)!=FL_EDIT)

#define FL_ISCONTROL() (m_pInplaceControl!=NULL && m_pInplaceControl->GetSafeHwnd()!=NULL)

//	сообщение от встраиваемых элементов управления
#define FLNM_INPLACE_KILLFOCUS	FLNM_USER_MSG(20)

// id для встраиваемых элементов управления
#define IDC_INPLACE_EDIT	501
#define IDC_INPLACE_COMBO	502



/////////////////////////////////////////////////////////////////////////////
// CFListCtrl
//
//
//

CFListCtrl::CFListCtrl()
{
	m_rootFRow.m_bCollapsed=FALSE;
	m_opt.m_iExpandButton=-1;
	m_opt.m_iCollapseButton=-1;
	//
	m_defaultControlProfile.m_nControlType=0;		
	m_pInplaceControl=NULL;
	m_hFRow=NULL;
	m_iFColumn=-1;
	m_nChar=0;
	//
	m_opt.m_nStyle=FL_READONLY;				
	m_opt.m_nTextLineCount=1;
	//
	m_opt.m_iTreeColumn=-1;				
	//
	m_nTextLineHeight=0;

	m_pCompareFRowsFunc=DefaultCompareFRows;
	m_pNotifyWnd=NULL;

	m_nRedrawCounter=0;

	m_pRefControls=&m_aControlProfiles;
}

CFListCtrl::~CFListCtrl()
{		
}

void CFListCtrl::OnDestroy() 
{
	DeleteAllFRows();
	RemoveAllControls();
	CListCtrl::OnDestroy();	
}

//
//
//					CONTROL PROFILE	
//
//


//	добавляет новый профиль контролов редактирования и возвращает его индекс
int CFListCtrl::AddControl(int nControlType)
{
	XFList_ControlProfile *pCP= new XFList_ControlProfile();
	pCP->m_nControlType=nControlType;
	return m_pRefControls->Add(pCP);
}

//	удаляет все профили
void CFListCtrl::RemoveAllControls()
{
	for(int i=0; i<GetControlCount(); i++)
		delete m_aControlProfiles[i];
	m_aControlProfiles.RemoveAll();//m_pRefControls
}

//	возвращает тип контролов редактирования для данного профиля
int CFListCtrl::GetControlType(int iControlProfile)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	return pCP->m_nControlType;
}

//	устанавливает тип контролов редактирования для данного профиля
void CFListCtrl::SetControlType(int iControlProfile, int nControlType)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	pCP->m_nControlType=nControlType;
}

//	возвращает картинки элемента checkbox указанного профиля
void CFListCtrl::GetControlCheckImages(int iControlProfile, int *piChecked, int *piUnchecked, int *piIndeterminate)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_CHECKBOX)==FL_CHECKBOX);	// для данного профиля не задан контрол checkbox
	if(piChecked!=NULL)
		*piChecked=pCP->m_checkImages.m_iChecked;
	if(piUnchecked!=NULL)
		*piUnchecked=pCP->m_checkImages.m_iUnchecked;
	if(piIndeterminate!=NULL)
		*piIndeterminate=pCP->m_checkImages.m_iIndeterminate;
}

//	устанавливает картинки элемента checkbox указанного профиля
void CFListCtrl::SetControlCheckImages(int iControlProfile, int iChecked, int iUnchecked, int iIndeterminate)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());	
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_CHECKBOX)==FL_CHECKBOX);	// для данного профиля не задан контрол checkbox
	pCP->m_checkImages.m_iChecked=iChecked;
	pCP->m_checkImages.m_iUnchecked=iUnchecked;
	pCP->m_checkImages.m_iIndeterminate=iIndeterminate;
}

//	добавляет элемент в выпадающий список combobox'а для  указанного профиля
void CFListCtrl::AddControlComboItem(int iControlProfile, CString strText, DWORD dwUserData)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());	
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_COMBOBOX)==FL_COMBOBOX);	// для данного профиля не задан элемент управления combobox
	FL_COMBOITEM ci;
	ci.strText=strText;
	ci.dwUserData=dwUserData;
	pCP->m_aComboItems.Add(ci);
}

//	возвращает массив элементов выпадающего списка combobox'а
FL_COMBOITEMS* CFListCtrl::GetControlCombo(int iControlProfile)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_COMBOBOX)==FL_COMBOBOX);	// для данного профиля не задан элемент управления combobox
	return &pCP->m_aComboItems;
}

//	возвращает выделенный элемент выпадающего списка combobox'а
FL_COMBOITEM CFListCtrl::GetControlComboSel(int iControlProfile)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_COMBOBOX)==FL_COMBOBOX);	// для данного профиля не задан элемент управления combobox
	return pCP->m_ciSelect;
}

//	устанавливает выделенный элемент выпадающего списка combobox'а
void CFListCtrl::SetControlComboSel(int iControlProfile, CString strText, DWORD dwUserData)
{
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	XFList_ControlProfile *pCP=GetCP(iControlProfile);
	ASSERT((pCP->m_nControlType&FL_COMBOBOX)==FL_COMBOBOX);	// для данного профиля не задан элемент управления combobox
	pCP->m_ciSelect.strText=strText;
	pCP->m_ciSelect.dwUserData=dwUserData;
}

//
//
//			COLUMN
//
//


//	втавляет новую колонку в дерево-список
int CFListCtrl::InsertFColumn(int iIndex, CString strText, int nWidth, int nFormat, int iDefaultControlProfile)
{		
	ModifyStyle(0, LVS_REPORT|LVS_OWNERDRAWFIXED);	
	if(iIndex==-1) iIndex=GetFColumnCount();

	XFList_Column FColumn;
	InitFColumn(FColumn);
	FColumn.m_iDefaultControlProfile=iDefaultControlProfile;
	FColumn.m_nFormat= nFormat==LVCFMT_RIGHT ? DT_RIGHT : (nFormat==LVCFMT_CENTER ? DT_CENTER : DT_LEFT);
	m_aFColumns.InsertAt(iIndex, FColumn);

	XFList_Row* pFRow=NULL;
	HFROW hFRow=m_lstFRows.GetHeadPosition();
	XFList_Row::XFList_Item item;			
	XFList_Row::InitFItem(item);		
		item.m_iControlProfile=FColumn.m_iDefaultControlProfile;

	while(hFRow!=NULL){
		pFRow=m_lstFRows.GetNext(hFRow);		
		pFRow->m_aFItems.InsertAt(iIndex, item);
	}		

	return CListCtrl::InsertColumn(iIndex, strText, nFormat, nWidth);
}

//	удаляет колонку
void CFListCtrl::DeleteFColumn(int iIndex)
{
	if(iIndex==-1) iIndex=GetFColumnCount();
	ASSERT(iIndex>=0 && iIndex<=GetFColumnCount());

	XFList_Row* pFRow=NULL;
	HFROW hFRow=GetNextFRow(FL_ROOT, FL_FIRST_CHILD);
	while(hFRow!=NULL){
		pFRow=m_lstFRows.GetNext(hFRow);		
		pFRow->m_aFItems.RemoveAt(iIndex);
	}

	if(!GetFColumnCount())
		DeleteAllFRows();
	
	m_aFColumns.RemoveAt(iIndex);
	DeleteColumn(iIndex);
}

//	возвращает индекс колонки с деревом
int CFListCtrl::GetTree()	
{
	//ASSERT(m_opt.m_iTreeColumn>=0 && m_opt.m_iTreeColumn<GetFColumnCount());
	return m_opt.m_iTreeColumn;
}

//	устанавлисает колонку, в которой будет отображаться дерево
void CFListCtrl::SetTree(int iFColumn)	
{
	ASSERT(iFColumn>=-1 && iFColumn<GetFColumnCount());
	m_opt.m_iTreeColumn=iFColumn;
	RedrawWindow();
}

//	возвращает имя колонки
CString CFListCtrl::GetFColumnCaption(int iFColumn)
{
	ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());	
	CString strCaption;
	LVCOLUMN lvColumn;
	lvColumn.mask=LVCF_TEXT;	
	lvColumn.pszText=strCaption.GetBufferSetLength(200);
	lvColumn.cchTextMax=200;	
	GetColumn(iFColumn, &lvColumn);
	strCaption.ReleaseBuffer();
	return strCaption;
}

//	устанавливает имя колонки
void CFListCtrl::SetFColumnCaption(int iFColumn, CString strText)
{
	ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());
	LVCOLUMN lvColumn;
	lvColumn.mask=LVCF_TEXT;
	lvColumn.pszText=strText.GetBuffer(strText.GetLength());
	SetColumn(iFColumn, &lvColumn);	
}


//
//
//			FROW
//
//

//	вставляет новую строку в дерево-список
HFROW CFListCtrl::InsertFRow(HFROW hParent, HFROW hPosition, CString strText, int iImageIndex, int iControlProfile,BOOL bFast)
{
	ASSERT(GetFColumnCount());
		//	create row
	XFList_Row* pFRow= new XFList_Row();
	XFList_Row::XFList_Item item;
	XFList_Row::InitFItem(item);
	for(int i=0; i<m_aFColumns.GetSize(); i++){		
		item.m_iControlProfile=GetFColumn(i).m_iDefaultControlProfile;
		pFRow->m_aFItems.Add(item);
	}
	int indx=GetOptions().m_iTreeColumn<0 ? 0 : GetOptions().m_iTreeColumn;		
	pFRow->m_aFItems[indx].m_strText=strText;
	pFRow->m_aFItems[indx].m_iImageIndex=iImageIndex;
	if(iControlProfile>=0)
		pFRow->m_aFItems[indx].m_iControlProfile=iControlProfile;

		//insert row	
	pFRow->m_hFRow=m_lstFRows.AddTail(pFRow);

	XFList_Row* pParent=GetFRow(hParent);
	pFRow->m_hParentFRow= hParent;
	pFRow->m_nDeep=pParent->m_nDeep+1;

	if(hPosition==FL_FIRST || hPosition==FL_SORTPOS)	
		pFRow->m_posParentList=pParent->m_lstChildFRows.AddHead(pFRow->m_hFRow);	
	else if(hPosition==FL_LAST)	
		pFRow->m_posParentList=pParent->m_lstChildFRows.AddTail(pFRow->m_hFRow);	
	else	
		pFRow->m_posParentList=pParent->m_lstChildFRows.
			InsertAfter(GetFRow(hPosition)->m_posParentList, pFRow->m_hFRow);		

	if(bFast)
		return pFRow->m_hFRow;
	
		//show row	
	ShowFRow(pFRow->m_hFRow, TRUE);
	if(hPosition==FL_SORTPOS){
		SetRedraw(FALSE);
		Sort(GetTree()>=0 ? GetTree() : 0, hParent, FALSE);
		SetRedraw(TRUE);
	}
	UpdateFRow(hParent);

	UpdateFColumnWidth(-1);
	return pFRow->m_hFRow;
}


//	удаляет строку
void CFListCtrl::DeleteFRow(HFROW hFRow)
{		
	ASSERT(hFRow!=NULL);	
	
	Collapse(hFRow);
	//
	if(hFRow!=FL_ROOT){
		FL_NOTIFY notify;
		NotifyInit(notify, hFRow, -1, FLNM_DELETE);
		NotifySend(notify);	
	}
	//			
	if(GetSelectedFRow()==hFRow)
	{
		HFROW hSelFRow=GetNextFRow(hFRow, FL_NEXT);
		if(hSelFRow==NULL)
			hSelFRow=GetNextFRow(hFRow, FL_PREV_VISIBLE);
		Select(hSelFRow, GetSelectedFColumn());
	}		
	//		
	HFROW hNextChild, hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
	for(;hChild!=NULL; hChild=hNextChild){				
		hNextChild=GetNextFRow(hChild, FL_NEXT);
		DeleteFRow(hChild);
	}		
	//
	if(hFRow!=FL_ROOT){
		ShowFRow(hFRow, FALSE, TRUE);
		XFList_Row* pFRow=GetFRow(hFRow);
		HFROW hParent=GetNextFRow(hFRow, FL_PARENT);
		GetFRow(hParent)->m_lstChildFRows.RemoveAt(pFRow->m_posParentList);
		UpdateFRow(hParent);
		m_lstFRows.RemoveAt(hFRow);
		delete pFRow;
	}		
	else
		Collapse(hFRow, FALSE);
}

// возвращает число строк
int CFListCtrl::GetFRowCount(HFROW hParent)
{
	XFList_Row *pFRow=GetFRow(hParent);
	return pFRow->m_lstChildFRows.GetCount();
}

//	возвращает описатель строки по её индексу
HFROW CFListCtrl::FindIndex(HFROW hParent, int iIndex)
{
	XFList_Row *pFRow=GetFRow(hParent);
	POSITION pos=pFRow->m_lstChildFRows.FindIndex(iIndex);
	return pos==NULL ? NULL : pFRow->m_lstChildFRows.GetAt(pos);
}

//	возвращает номер строки в листе по её описателю
int CFListCtrl::GetFRowIndex(HFROW hFRow)
{	
	XFList_Row *pFRow=GetFRow(hFRow);
	return pFRow->m_iListIndex;	
}


//	возвращает описатель "следующей" строки 
HFROW CFListCtrl::GetNextFRow(HFROW hFRow, int nfrCode)
{
	ASSERT(hFRow!=NULL || nfrCode==FL_TOP || nfrCode==FL_BOTTOM);
	HFROW hNextFRow=NULL;
	XFList_Row* pFRow=NULL, *pParentFRow=NULL;
	POSITION pos=NULL;

	switch(nfrCode){
	case FL_FIRST_CHILD:
		pFRow=GetFRow(hFRow);
		if(pFRow->m_lstChildFRows.IsEmpty())
			hNextFRow=NULL;
		else
			hNextFRow=pFRow->m_lstChildFRows.GetHead();
		break;

	case FL_LAST_CHILD:
		pFRow=GetFRow(hFRow);
		if(pFRow->m_lstChildFRows.IsEmpty())
			hNextFRow=NULL;
		else
			hNextFRow=pFRow->m_lstChildFRows.GetTail();
		break;

	case FL_NEXT:
			ASSERT(hFRow!=FL_ROOT);
		pFRow=GetFRow(hFRow);
		pParentFRow=GetFRow(GetNextFRow(hFRow, FL_PARENT));
			ASSERT(!pParentFRow->m_lstChildFRows.IsEmpty());
						
		pos=pFRow->m_posParentList;			
		pParentFRow->m_lstChildFRows.GetNext(pos);
		hNextFRow= pos==NULL ? NULL : pParentFRow->m_lstChildFRows.GetAt(pos);
		break;

	case FL_NEXT_VISIBLE:
		pFRow=GetFRow(hFRow);
		if(pFRow!=NULL && pFRow->m_iListIndex<(GetItemCount()-1))			
			hNextFRow=(HFROW)GetItemData(pFRow->m_iListIndex+1);			
		else
			hNextFRow=NULL;		
		break;

	case FL_PREV:
			ASSERT(hFRow!=FL_ROOT);
		pFRow=GetFRow(hFRow);
		pParentFRow=GetFRow(GetNextFRow(hFRow, FL_PARENT));
			ASSERT(!pParentFRow->m_lstChildFRows.IsEmpty());
						
		pos=pFRow->m_posParentList;			
		pParentFRow->m_lstChildFRows.GetPrev(pos);
		hNextFRow= pos==NULL ? NULL : pParentFRow->m_lstChildFRows.GetAt(pos);
		break;

	case FL_PREV_VISIBLE:		
		{
			ASSERT(IsFRowVisible(hFRow));
			hNextFRow=GetNextFRow(hFRow, FL_PREV);
			if(hNextFRow==NULL)
			{
				if(hNextFRow=GetNextFRow(hFRow, FL_PARENT), hNextFRow==FL_ROOT)
					hNextFRow=NULL;
			}
			else{			
				for(;!IsCollapsed(hNextFRow);)
					hNextFRow=GetNextFRow(hNextFRow, FL_LAST_CHILD);	
			}
		}
		//	ASSERT(hFRow==GetNextFRow(hFRow, FL_PARENT_VISIBLE));
		//hNextFRow=GetNextFRow(hFRow, FL_PREV);
		//if(hNextFRow==NULL){			
		//	if(hNextFRow=GetNextFRow(hFRow, FL_PARENT), hNextFRow==FL_ROOT)
		//		hNextFRow=NULL;
		//}
		//else{			
		//	for(;!IsCollapsed(hNextFRow);)
		//		hNextFRow=GetNextFRow(hNextFRow, FL_LAST_CHILD);	
		//}
		break;

	case FL_PARENT:
		hNextFRow=GetFRow(hFRow)->m_hParentFRow;
		break;

	case FL_PARENT_VISIBLE:
		hNextFRow=GetNextFRow(hFRow, FL_PARENT);
		if(!IsCollapsed(hNextFRow))
			hNextFRow=hFRow;
		else
			hNextFRow=GetNextFRow(hNextFRow, FL_PARENT_VISIBLE);			
		break;

	case FL_PARENT_ROOT:			
		hNextFRow=GetNextFRow(hFRow, FL_PARENT);
		if(hNextFRow==FL_ROOT)
			hNextFRow=hFRow;
		else
			hNextFRow=GetNextFRow(hNextFRow, FL_PARENT_ROOT);			
		break;

	case FL_TOP:
		{
			int indx=GetTopIndex();			
			hNextFRow=indx>=0 ? (HFROW)GetItemData(indx) : NULL;			
		}
		break;

	case FL_BOTTOM:
		{
			int indx=GetTopIndex()+GetCountPerPage();	
			if(indx>=GetItemCount())
				indx=GetItemCount()-1;
			hNextFRow=indx>=0 ? (HFROW)GetItemData(indx) : NULL;			
		}
		break;
	
	default:
		ASSERT(FALSE);
		break;
	}
	return hNextFRow;
}

//	закрывает/раскрывает узел дерева
void CFListCtrl::Collapse(HFROW hFRow, BOOL bCollapse)
{	
	ASSERT(hFRow!=NULL);
	XFList_Row* pFRow=GetFRow(hFRow);
	if(IsCollapsed(hFRow)==bCollapse)
		return;
	pFRow->m_bCollapsed=bCollapse;
	//	
	if(!IsCollapsed(GetNextFRow(hFRow, FL_PARENT))){
		SetRedraw(FALSE);
		HFROW hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
		for(;hChild!=NULL; hChild=GetNextFRow(hChild, FL_NEXT))			
			ShowFRow(hChild, !bCollapse, TRUE);
		SetRedraw(TRUE);
	}
	//
	if(hFRow!=FL_ROOT){
		FL_NOTIFY notify;
		NotifyInit(notify, hFRow, -1, FLNM_COLLAPSE);
		NotifySend(notify);
	}
	//
	HFROW hSelFRow=GetSelectedFRow();
	if(hSelFRow!=NULL && GetFRow(hSelFRow)->m_iListIndex<0)
		Select(hFRow, GetSelectedFColumn());
	else
		UpdateFRow(hFRow);

	UpdateFColumnWidth(-1);
}

//	возвращает TRUE если узел закрыт или не имеет дочерних строк
BOOL CFListCtrl::IsCollapsed(HFROW hFRow)
{
	XFList_Row* pFRow=GetFRow(hFRow);	
	return pFRow==NULL ? FALSE : (pFRow->m_bCollapsed || pFRow->m_lstChildFRows.IsEmpty());
}

//	обновляет строку
void CFListCtrl::UpdateFRow(HFROW hFRow)
{	
	XFList_Row* pFRow=GetFRow(hFRow);			
	if(pFRow!=NULL && pFRow->m_iListIndex>=0){			
		RedrawItems(pFRow->m_iListIndex, pFRow->m_iListIndex);
		UpdateWindow();
	}
}

//	меняет две строки местами
void CFListCtrl::ExchangeFRows(HFROW hFRow1, HFROW hFRow2)
{
	XFList_Row *pFRow1=GetFRow(hFRow1);
	XFList_Row *pFRow2=GetFRow(hFRow2);

	ASSERT(pFRow1!=NULL && pFRow2!=NULL);

	ASSERT(GetNextFRow(hFRow1, FL_PARENT)==GetNextFRow(hFRow2, FL_PARENT));
	XFList_Row *pParent=GetFRow(GetNextFRow(hFRow1, FL_PARENT));	

	BOOL bCollapse1=IsCollapsed(hFRow1);
	BOOL bCollapse2=IsCollapsed(hFRow2);
	Collapse(hFRow1, TRUE);
	Collapse(hFRow2, TRUE);

	POSITION pos1=pFRow1->m_posParentList;
	POSITION pos2=pFRow2->m_posParentList;

	pParent->m_lstChildFRows.SetAt(pos1, hFRow2);
	pFRow2->m_posParentList=pos1;
	pParent->m_lstChildFRows.SetAt(pos2, hFRow1);
	pFRow1->m_posParentList=pos2;

	if(pFRow1->m_iListIndex>=0)
	{			
		int indxExch=pFRow1->m_iListIndex;
		pFRow1->m_iListIndex=pFRow2->m_iListIndex;
		pFRow2->m_iListIndex=indxExch;
			
		SetItemData(pFRow1->m_iListIndex, (DWORD)pFRow1->m_hFRow);
		SetItemData(pFRow2->m_iListIndex, (DWORD)pFRow2->m_hFRow);
	}

	if(!bCollapse1)
		Collapse(hFRow1, FALSE);
	if(!bCollapse2)
		Collapse(hFRow2, FALSE);

	UpdateFRow(hFRow1);
	UpdateFRow(hFRow2);
}


//перемещает строку
void CFListCtrl::MoveFRows(HFROW hFRow1, HFROW hFNewParent)
{
	XFList_Row *pFRow1=GetFRow(hFRow1);

	ASSERT(pFRow1);

	XFList_Row *pParent=GetFRow(GetNextFRow(hFRow1, FL_PARENT));	
	XFList_Row *pNewParent=GetFRow(hFNewParent);	

	BOOL bCollapse1=IsCollapsed(hFRow1);
	Collapse(hFRow1, TRUE);

	//делаем строку невидимой
	ShowFRow(hFRow1, FALSE, TRUE);

	//удаляем в старом родителе
	pParent->m_lstChildFRows.RemoveAt(pFRow1->m_posParentList);
	//добавляем в новом
	pFRow1->m_posParentList=pNewParent->m_lstChildFRows.AddTail(hFRow1);
	pFRow1->m_hParentFRow= hFNewParent;
	
	//изменяем глубину для всех элементов
	SetAllDeep(hFRow1,pNewParent->m_nDeep+1);

	//делаем строку видимой
	ShowFRow(hFRow1, TRUE);

	if(!bCollapse1)
		Collapse(hFRow1, FALSE);
	UpdateFRow(hFRow1);
}
//изменяет уровень всех элементов
void CFListCtrl::SetAllDeep(HFROW hFRow,int nDeep)
{
	if(nDeep>100)//защита от вложенности в самого себя
		return;
	XFList_Row *pFRow=GetFRow(hFRow);
	pFRow->m_nDeep=nDeep;
	
	//устанавливаем для всех дочерних элементов
	HFROW pos=GetNextFRow(hFRow,FL_FIRST_CHILD);
	while(pos!=NULL)
	{
		SetAllDeep(pos,nDeep+1);
		pos=GetNextFRow(pos,FL_NEXT);
	}

/*	//делаем полосную раскраску
	COLORREF crColor1=RGB(255, 255, 255);
	if(nDeep%2==0)
		crColor1=RGB(243, 243, 243);
*/
/*	int nDeltaC=(nDeep%10)*5;
	COLORREF crColor1=RGB(255-nDeltaC, 255-nDeltaC, 255);
	SetColor(FL_FROW, hFRow, -1, FL_DONTSET, crColor1);
*/
}


//	возвращает TRUE, если для строки установлен стиль bold (жирный текст)
BOOL CFListCtrl::IsFRowBold(HFROW hFRow)
{
	XFList_Row *pFRow=GetFRow(hFRow);
	return pFRow->m_bBoldText;
}

//	устанавливает стиль текста bold для указанной строки
void CFListCtrl::SetFRowBold(HFROW hFRow, BOOL bBoldText)
{
	XFList_Row *pFRow=GetFRow(hFRow);
	pFRow->m_bBoldText=bBoldText;
}

//	возвращает уровень вложенности для строки начиная с единицы
int CFListCtrl::GetFRowDeep(HFROW hFRow)
{
	XFList_Row *pFRow=GetFRow(hFRow);
	return pFRow->m_nDeep;
}

//	устанавливает профиль элементов управления для каждого элемента строки
void CFListCtrl::SetFRowControls(HFROW hFRow, int iControlProfile1, ...)
{
	int iControl;
	va_list marker;
	va_start(marker, iControlProfile1);

	SetFItemControl(hFRow, 0, iControlProfile1);
	for(long i=1; i<GetFColumnCount(); i++)
	{
		iControl=va_arg(marker, int);
		SetFItemControl(hFRow, i, iControl);
	}
	va_end(marker);
}

//	поиск строки
HFROW CFListCtrl::FindFRow(HFROW hParent, HFROW hFirst, int iFColumn, CString text, BOOL bCompareNoCase)
{
	ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());
	HFROW hFRow= hFirst!=NULL ? hFirst : GetNextFRow(hParent, FL_FIRST_CHILD);
	if(bCompareNoCase){
		for(;hFRow!=NULL; hFRow=GetNextFRow(hFRow, FL_NEXT))
			if(!text.CompareNoCase(GetFItemText(hFRow, iFColumn)))
				break;
	}
	else{
		for(;hFRow!=NULL; hFRow=GetNextFRow(hFRow, FL_NEXT))
			if(GetFItemText(hFRow, iFColumn)==text)
				break;
	}
	return hFRow;
}

//	возвращает TRUE, если все родительские узлы раскрыты
BOOL CFListCtrl::IsFRowVisible(HFROW hFRow)
{
	//if(hFRow==FL_ROOT)
		//return TRUE;
	//else if(hFRow==NULL)
	//	return FALSE;
	//HFROW hParent=GetNextFRow(hFRow);
	//return hParent==FL_ROOT || IsFRowVisible(hParent) && !IsCollapsed(hParent);;
	if(hFRow==NULL || hFRow==FL_ROOT)
		return FALSE;
	HFROW hParent=GetNextFRow(hFRow, FL_PARENT);
	return hParent==FL_ROOT || ( GetFRow(hParent)->m_iListIndex>=0 && !IsCollapsed(hParent) );
}

//
//
//			FITEM
//
//

//	возвращает текст
CString CFListCtrl::GetFItemText(HFROW hFRow, int iFColumn) 
{	
	return GetFItem(hFRow, iFColumn).m_strText;
}

//	устанавливает текст 	
void CFListCtrl::SetFItemText(HFROW hFRow, int iFColumn, CString strText) 
{				
	GetFItem(hFRow, iFColumn).m_strText=strText;	
	UpdateFColumnWidth(-1);
	UpdateFRow(hFRow);
}


//	возвращает индекс картинки
int CFListCtrl::GetFItemImage(HFROW hFRow, int iFColumn) 
{	
	return GetFItem(hFRow, iFColumn).m_iImageIndex;
}

//	устанавливает картинку
void CFListCtrl::SetFItemImage(HFROW hFRow, int iFColumn, int iImageIndex)
{					
	GetFItem(hFRow, iFColumn).m_iImageIndex=iImageIndex;
	UpdateFRow(hFRow);
}

//	возвращает пользовательские данные
DWORD CFListCtrl::GetFItemData(HFROW hFRow, int iFColumn) 
{	
	return GetFItem(hFRow, iFColumn).m_dwUserData;
}
//	устанавливает пользовательские данные
void CFListCtrl::SetFItemData(HFROW hFRow, int iFColumn, DWORD dwUserData)
{					
	GetFItem(hFRow, iFColumn).m_dwUserData=dwUserData;	
}

//	прокручивает таблицу так, чтобы указанный элемент стал видимым
BOOL CFListCtrl::EnsureVisible(HFROW hFRow, int iFColumn)
{
	XFList_Row* pFRow=GetFRow(hFRow);
	if(pFRow!=NULL && pFRow->m_iListIndex>=0)
	{
		BOOL bResult=CListCtrl::EnsureVisible(pFRow->m_iListIndex, FALSE);
		if(bResult && iFColumn>=0){
			CRect rect, client;
			if(GetFRect(hFRow, iFColumn, FLRC_FITEM, rect))
			{
				GetClientRect(&client);				
				CSize scroll(0, 0);
				
				if(rect.left<client.left)
					scroll.cx = rect.left-client.left+(iFColumn ? -FL_EV_SPACE : 0);
				else if(rect.right>client.right){				
					scroll.cx = rect.right-client.right+(iFColumn<GetFColumnCount() ? FL_EV_SPACE : 0);
					if(rect.Width()>(client.Width()-2*FL_EV_SPACE))
						scroll.cx-=rect.Width()-client.Width()+2*FL_EV_SPACE;
				}
								
				Scroll(scroll);
				//
				DrawElement(FLDR_UNDERHEADER, NULL, NULL, 0, NULL);
				return TRUE;
			}
		}		
	}
	return FALSE;
}

//	возвращает состояние checkbox'а
int CFListCtrl::GetFItemCheck(HFROW hFRow, int iFColumn)
{
	ASSERT((GetCP(hFRow, iFColumn)->m_nControlType&FL_CHECKBOX)==FL_CHECKBOX);
	return GetFItem(hFRow, iFColumn).m_nCheckState;
}

//	устанавливает состояние checkbox'a
void CFListCtrl::SetFItemCheck(HFROW hFRow, int iFColumn, int nState)
{
	ASSERT((GetCP(hFRow, iFColumn)->m_nControlType&FL_CHECKBOX)==FL_CHECKBOX);
	GetFItem(hFRow, iFColumn).m_nCheckState=nState;
	UpdateFRow(hFRow);
}

//	возвращает индекс профиля контролов редактирования указанного элемента строки
int CFListCtrl::GetFItemControl(HFROW hFRow, int iFColumn)
{
	return GetFItem(hFRow, iFColumn).m_iControlProfile;
}	

//	устанавливает профиль контролов редактирования для указанного элемента строки
void CFListCtrl::SetFItemControl(HFROW hFRow, int iFColumn, int iControlProfile)
{
	GetFItem(hFRow, iFColumn).m_iControlProfile=iControlProfile;
	UpdateFRow(hFRow);
}


//
//
//				SELECTION
//
//


//	выделяет указанную строку/элемент дерева-списка
void CFListCtrl::Select(HFROW hFRow, int iFColumn, BOOL bEnsureVisible)
{
	if(FL_ISCONTROL())
		return;
	ASSERT(iFColumn>=-1 && iFColumn<GetFColumnCount());
	//ASSERT(m_pInplaceControl==NULL);//чтобы закрыть элемент управления, нужно установить фокус на другой элемент управления, например, на само дерово-список
	hFRow= hFRow==FL_ROOT ? NULL : hFRow;
	iFColumn =  IsReadOnly() ? -1 : iFColumn;
	HFROW hOldFRow=GetSel().m_hSelectedFRow;
	int iOldFColumn=GetSel().m_iSelectedFColumn;
	//
	GetSel().m_hSelectedFRow=hFRow;
	GetSel().m_iSelectedFColumn= IsReadOnly() ? -1 : (iFColumn<GetFColumnCount() ? iFColumn : GetFColumnCount()-1);
	//
	if(hOldFRow!=hFRow || iOldFColumn!=iFColumn)
	{					
		UpdateFRow(hOldFRow);
		//				
		UpdateFRow(hFRow);	
		if(bEnsureVisible)
			EnsureVisible(hFRow, iFColumn);
		//
		FL_NOTIFY notify;
		NotifyInit(notify, hFRow, iFColumn, FLNM_SELECT);
		NotifySend(notify);
	}	
}


//
//
//			PROPERTIES
//
//


	
//	модифицирует стиль дерева-списка
void CFListCtrl::SetFStyle(int nDeleteStyle, int nAddStyle, int nMask)
{	
	if(!nMask)
		nMask=~nMask;
	m_opt.m_nStyle&= ~(nDeleteStyle&nMask);
	m_opt.m_nStyle|= nAddStyle&nMask;
	UpdateWindow();
}

//	устанваливает стиль readonly
void CFListCtrl::SetReadOnly(BOOL bReadOnly)
{	
	m_opt.m_nStyle= bReadOnly ? m_opt.m_nStyle|FL_READONLY : m_opt.m_nStyle&(~FL_READONLY);
	HFROW hSelFRow=GetSelectedFRow();
	if(hSelFRow!=NULL)
		UpdateFRow(hSelFRow);
}


//	возвращает индекс изображения для кнопки раскрыть/закрыть узел
void CFListCtrl::GetTreeButton(int *piExpandImage, int *piCollapseImage)
{
	if(piExpandImage!=NULL)
		*piExpandImage=m_opt.m_iExpandButton;
	if(piCollapseImage!=NULL)
		*piCollapseImage=m_opt.m_iCollapseButton;
}

//	устанавливает изображение для кнопоки раскрыть/закрыть узел
void CFListCtrl::SetTreeButton(int iExpandImage, int iCollapseImage)
{
	m_opt.m_iExpandButton=iExpandImage;
	m_opt.m_iCollapseButton=iCollapseImage;
	UpdateWindow();
}

//	устанавливает число строк текста в одной строке таблицы	
void CFListCtrl::SetTextLineCount(int nTextLineCount)
{
	m_opt.m_nTextLineCount=nTextLineCount;
	//
	CRect rc;
	GetWindowRect( &rc );
	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );			
}	


//
//	
//			MISCELLANEOUS
//
//


//	определяет прямоугольник требуемой области
BOOL CFListCtrl::GetFRect(HFROW hFRow, int iFColumn, int nArea, CRect &rect, BOOL bFItemOrigin)
{	
	XFList_Row *pFRow=GetFRow(hFRow);	
	rect.SetRect(0, 0, 0, 0);

	switch(nArea){
	case FLRC_FITEM:
		{
			ASSERT(pFRow!=NULL);
			if(pFRow->m_iListIndex<0)
				return FALSE;
			GetSubItemRect(pFRow->m_iListIndex, iFColumn, LVIR_BOUNDS, rect);
			rect.right=rect.left+GetColumnWidth(iFColumn);
			if(bFItemOrigin)
				rect.OffsetRect(-rect.TopLeft());
		}
		break;

	case FLRC_FITEM_CLIENT:
		if(!GetFRect(hFRow, iFColumn, FLRC_FITEM, rect, bFItemOrigin))
			return FALSE;
		if(iFColumn==GetOptions().m_iTreeColumn && !(m_opt.m_nStyle&FL_LINEAR_TREE))		
			rect.left+=(pFRow->m_nDeep-1)*FL_STEP;		
		break;
		
	case FLRC_FITEM_TREEBUTTON:		
		if(iFColumn!=GetOptions().m_iTreeColumn || !GetFRect(hFRow, iFColumn, FLRC_FITEM_CLIENT, rect, bFItemOrigin))
			return FALSE;
		rect.right=rect.left+FL_STEP;
		rect.bottom=rect.top+rect.Width();		
		break;
	
	case FLRC_FITEM_CHECKBOX:
		if(!(GetCP(hFRow, iFColumn)->m_nControlType&FL_CHECKBOX) || !GetFRect(hFRow, iFColumn, FLRC_FITEM_CLIENT, rect, bFItemOrigin))
			return FALSE;						
		rect.left+= (iFColumn==GetOptions().m_iTreeColumn) ? FL_STEP : 0;
		rect.right=rect.left+FL_STEP;
		rect.bottom=rect.top+rect.Width();
		break;

	case FLRC_FITEM_IMAGE:
		if(GetImageList(LVSIL_SMALL)==NULL || GetFItem(hFRow, iFColumn).m_iImageIndex<0 || !GetFRect(hFRow, iFColumn, FLRC_FITEM_CLIENT, rect, bFItemOrigin))
			return FALSE;				
		rect.left+= (iFColumn==GetOptions().m_iTreeColumn) ? FL_STEP : 0;
		rect.left+= (GetCP(hFRow, iFColumn)->m_nControlType&FL_CHECKBOX) ? FL_STEP : 0;
		rect.right=rect.left+FL_STEP;
		rect.bottom=rect.top+rect.Width();
		break;

	case FLRC_FITEM_COMBOBOX:
		{
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_CLIENT, rect, bFItemOrigin))
				return FALSE;		
			rect.left+=(iFColumn==GetOptions().m_iTreeColumn) ? FL_STEP : 0;
			rect.left+=(GetCP(hFRow, iFColumn)->m_nControlType&FL_CHECKBOX) ? FL_STEP : 0;
			rect.left+=(GetFItem(hFRow, iFColumn).m_iImageIndex>=0) ? FL_STEP : 0;
			if(FL_ISGRID())
				rect.DeflateRect(1, 0, 0, 0);			
		}
		break;

	case FLRC_FITEM_BUTTON:
		{					
			if( !(GetCP(hFRow, iFColumn)->m_nControlType&FL_BUTTON)
				|| !(GetSelectedFRow()==hFRow && GetSelectedFColumn()==iFColumn)				
				|| !GetFRect(hFRow, iFColumn, FLRC_FITEM_COMBOBOX, rect, bFItemOrigin))
				return FALSE;		
			int left=rect.left;
			//			
				//rect.right+=-2;			
			rect.left=rect.right-FL_STEP+(FL_ISGRID() ? 1 : 0);						
			rect.bottom=rect.top+rect.Width();
			//			
			if(rect.left<=left) return FALSE;
		}
		break;

	case FLRC_FITEM_TEXT:
		{
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_COMBOBOX, rect, bFItemOrigin))
				return FALSE;			
			int left_space=( (GetCP(hFRow, iFColumn)->m_nControlType&FL_BUTTON) && GetSelectedFRow()==hFRow && GetSelectedFColumn()==iFColumn) 
					? -FL_STEP+(FL_ISGRID() ? 1 : 0) : 0;
			rect.right+= rect.Width()>(-left_space) ? left_space : 0;
			rect.bottom-= FL_ISGRID() ? 1 : 0;
		}
		break;

	case FLRC_FROW:
		ASSERT(pFRow!=NULL);

		if(pFRow->m_iListIndex<0)
			return FALSE;
		GetItemRect(pFRow->m_iListIndex, rect, LVIR_BOUNDS);
		break;

	case FLRC_FROW_TOP:		
		if(!GetItemCount() || !GetFRect((HFROW)GetItemData(GetTopIndex()), 0, FLRC_FROW, rect, bFItemOrigin))
			return FALSE;		
		break;

	case FLRC_VISIBLE_FROWS:
		{
			if(GetFRect(NULL, 0, FLRC_FROW_TOP, rect, bFItemOrigin)){
				int rows_count =GetItemCount()-GetTopIndex();
				rows_count = rows_count>GetCountPerPage() ? GetCountPerPage()+1 : rows_count;
				rect.bottom=rect.top+rect.Height()*rows_count;
				//if(GetExtendedStyle()&LVS_EX_GRIDLINES==LVS_EX_GRIDLINES)
				//	rect.right++;
			}
			//else
			//	rect.SetRect(0, 0, 0, 0);			
		}
		break;

	case FLRC_FCOLUMN:
		{
			ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());
			GetHeaderCtrl()->GetItemRect(iFColumn, rect);			
			CRect client;
			GetClientRect(client);			
			rect.top=rect.bottom;
			rect.bottom=client.bottom;			
			//
			CRect header;
			GetHeaderCtrl()->GetWindowRect(header);
			//rect.top=header.bottom+1;			
			ClientToScreen(&client);

			rect.OffsetRect(CPoint(header.left-client.left, 0));
			//
			
		}
		break;

	default:
		ASSERT(FALSE);
		break;
	}
	return TRUE;
}

//
#define get_color(cr, color_struct, type)\
	( (type)==FL_TEXT ? (FL_ISCOLOR((color_struct).crText) ? \
							(cr=(color_struct).crText, TRUE) : FALSE) : \
						(FL_ISCOLOR((color_struct).crBackground) ? \
							(cr=(color_struct).crBackground, TRUE) : FALSE) )\

#define getcolor(color_struct, type)\
	((type)==FL_TEXT ? (color_struct).crText : (color_struct).crBackground)

//	возвращает цвет текста/фона для элемента/стронки/колонки/фона
COLORREF CFListCtrl::GetColor(int nType, HFROW hFRow, int iFColumn)
{
	ASSERT(nType==FL_TEXT || nType==FL_BACKGROUND);
	COLORREF color=0;
	if(iFColumn==-1)
		iFColumn=-2;
	
	if(hFRow==GetSelectedFRow() && hFRow!=NULL)
	{
		if(iFColumn==GetSelectedFColumn())
		{
			if(FL_ISCONTROL() && nType==FL_BACKGROUND)
				return GetBkColor();

			color=getcolor(GetSel().m_colorFItem, nType);
			switch(color){
			case FL_DEFCOLOR:
				return nType==FL_TEXT ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_HIGHLIGHT);				
			case FL_NOCOLOR:
				break;
			default:				
				return color;
			}
		}
	
		if((IsReadOnly() && !iFColumn) || (GetExtendedStyle()&LVS_EX_FULLROWSELECT))
		{
			color=getcolor(GetSel().m_colorFRow, nType);
			switch(color){
			case FL_DEFCOLOR:
				return nType==FL_TEXT ? GetSysColor(COLOR_HIGHLIGHTTEXT) : GetSysColor(COLOR_HIGHLIGHT);			
			case FL_NOCOLOR:
				break;
			default:				
				return color;
			}
		}
	}

	if(hFRow!=NULL){
		color=getcolor(GetFRow(hFRow)->m_color, nType);
		if(color!=FL_DEFCOLOR && color!=FL_NOCOLOR)		
			return color;		
	}

	if(iFColumn>=0){
		color=getcolor(GetFColumn(iFColumn).m_color, nType);
		if(color!=FL_DEFCOLOR && color!=FL_NOCOLOR)		
			return color;
	}

	return nType==FL_TEXT ? GetTextColor() : GetBkColor();
}

//	устанавлинвает цвет для указанной области
void CFListCtrl::SetColor(int nArea, HFROW hFRow, int iFColumn, COLORREF crText, COLORREF crBackground)
{	
	if((nArea&FL_FULL)==FL_FULL){
		if(crText!=FL_DONTSET) SetTextColor(crText);
		if(crBackground!=FL_DONTSET) SetBkColor(crBackground);		
	}
	else if((nArea&FL_FCOLUMN)==FL_FCOLUMN){
		if(crText!=FL_DONTSET) GetFColumn(iFColumn).m_color.crText=crText;		
		if(crBackground!=FL_DONTSET)GetFColumn(iFColumn).m_color.crBackground=crBackground;		
	}
	else if((nArea&FL_FROW)==FL_FROW){
		if(crText!=FL_DONTSET) GetFRow(hFRow)->m_color.crText=crText;
		if(crBackground!=FL_DONTSET)GetFRow(hFRow)->m_color.crBackground=crBackground;
	}
	else if((nArea&FL_SELECTED_FROW)==FL_SELECTED_FROW){
		if(crText!=FL_DONTSET) GetSel().m_colorFRow.crText=crText;		
		if(crBackground!=FL_DONTSET)GetSel().m_colorFRow.crBackground=crBackground;
	}
	else if((nArea&FL_SELECTED_FITEM)==FL_SELECTED_FITEM){
		if(crText!=FL_DONTSET) GetSel().m_colorFItem.crText=crText;
		if(crBackground!=FL_DONTSET) GetSel().m_colorFItem.crBackground=crBackground;			
	}
}


//	сортирует строки по тексту в указанной колонке
void CFListCtrl::Sort(int iFColumn, BOOL bReverseSort)
{
	ASSERT(iFColumn>=0 && iFColumn<GetFColumnCount());
	
	GetFColumn(iFColumn).m_bReverseSort=bReverseSort;	
	SetRedraw(FALSE);
	Sort(iFColumn, FL_ROOT);	
	SetRedraw(TRUE, TRUE);
}

//	сортирует подстроки для указанной строки
void CFListCtrl::Sort(int iFColumn, HFROW hParentFRow, BOOL bSortChild)
{	
	return;//сортирует крайне медленно - поэтому опция отключена
	if(GetFRowCount(hParentFRow)>2)
	{
		BOOL bReverseSort=GetFColumn(iFColumn).m_bReverseSort;

		
		//сортировка дочерних строк hParentFRow
		HFROW hCurFRow, hNextFRow;		
		BOOL bExit;
		int iCmpResult;
		do{
			bExit=TRUE;

			hCurFRow=GetNextFRow(hParentFRow, FL_FIRST_CHILD);
			hNextFRow=GetNextFRow(hCurFRow, FL_NEXT);
			while(hNextFRow!=NULL){	

				//iCmpResult=GetFItemText(hCurFRow, iFColumn).CompareNoCase(GetFItemText(hNextFRow, iFColumn));

				iCmpResult=(*m_pCompareFRowsFunc)(this, iFColumn, hCurFRow, hNextFRow);
				if(bReverseSort ? iCmpResult<0 : iCmpResult>0)
				{
					ExchangeFRows(hCurFRow, hNextFRow);
					bExit=FALSE;
				}
				
				hCurFRow=GetNextFRow(hCurFRow, FL_NEXT);
				hCurFRow=hNextFRow;
				hNextFRow=GetNextFRow(hNextFRow, FL_NEXT);
			}
		}
		while(!bExit);	
	}

	if(bSortChild){
		//сортировка на нижних уровнях вложенности
		HFROW hCurFRow=GetNextFRow(hParentFRow, FL_FIRST_CHILD);
		while(hCurFRow!=NULL){		
			Sort(iFColumn, hCurFRow);
			hCurFRow=GetNextFRow(hCurFRow, FL_NEXT);
		}
	}
}


//
//
//				IMPLEMENTATION
//
//



//
// 
//
void CFListCtrl::OnHeaderClicked(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;

	if( phdn->iButton == 0 )
	{                			
		BOOL bReverseSort=GetFColumn(phdn->iItem).m_bReverseSort;
		Sort(phdn->iItem, bReverseSort);
		GetFColumn(phdn->iItem).m_bReverseSort=!bReverseSort;
	}
	*pResult = 0;
}

void CFListCtrl::OnDividerDblClk(NMHDR* pNMHDR, LRESULT* pResult) 
{
	HD_NOTIFY *phdn = (HD_NOTIFY *) pNMHDR;
	if(phdn->iItem>=0 && phdn->iItem<GetFColumnCount())
		UpdateFColumnWidth(phdn->iItem);
	*pResult=0;
}
//===================================================================================
CFListCtrl::XFList_ControlProfile* CFListCtrl::GetCP(int iControlProfile)
{
	if(iControlProfile==-1)
		return &m_defaultControlProfile;
#ifdef _DEBUG
	ASSERT(iControlProfile>=0 && iControlProfile<GetControlCount());
	if(iControlProfile>=0 && iControlProfile<GetControlCount())
		return (*m_pRefControls)[iControlProfile];
	return &m_defaultControlProfile;
#else
	return (*m_pRefControls)[iControlProfile];
#endif
}


//
//
//					ROW
//
//

void CFListCtrl::ShowFRow(HFROW hFRow, BOOL bShow, BOOL bUpdateIndexes)
{
	XFList_Row* pFRow=GetFRow(hFRow);
	int indxList=0;
	if(bShow)
	{
		if(!IsFRowVisible(hFRow))
			return;
		HFROW hPrevFRow=GetNextFRow(hFRow, FL_PREV_VISIBLE);			
		pFRow->m_iListIndex = hPrevFRow!=NULL ? GetFRow(hPrevFRow)->m_iListIndex+1 : 0;
		indxList = pFRow->m_iListIndex+1;

		InsertItem(LVIF_STATE|LVIF_TEXT, pFRow->m_iListIndex, _T(""), 0, 0, 0, 0);
		SetItemData(pFRow->m_iListIndex, (DWORD)hFRow);
		//
		if(!IsCollapsed(hFRow)){
			HFROW hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
			for(;hChild!=NULL; hChild=GetNextFRow(hChild, FL_NEXT))				
				ShowFRow(hChild, TRUE, TRUE);
		}
	}
	else{
		if(pFRow->m_iListIndex<0)
			return;
		HFROW hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
		for(;hChild!=NULL; hChild=GetNextFRow(hChild, FL_NEXT))
			ShowFRow(hChild, FALSE, TRUE);
		//
		indxList=pFRow->m_iListIndex;
		pFRow->m_iListIndex=-1;
		//
		DeleteItem(indxList);
	}
	//
	if(bUpdateIndexes)
	{
		int iCount=GetItemCount();
		for(indxList; indxList<iCount; indxList++)			
			GetFRow((HFROW)GetItemData(indxList))->m_iListIndex=indxList;
	}
}



//
//
//			DRAW
//
//


void CFListCtrl::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC dc;
	dc.Attach(lpDIS->hDC);

	if(FL_ISCONTROL())
	{
		CRect rect;
		if(m_pInplaceControl->GetWindowRect(rect), !rect.IsRectEmpty())
		{
			ScreenToClient(&rect);
			CRgn rgn;
			rgn.CreateRectRgnIndirect(&rect);
			dc.SelectClipRgn(&rgn, RGN_DIFF);
		}
	}
	
	for(int i=0; i<GetFColumnCount(); i++)
		DrawFItem(&dc, (HFROW)GetItemData(lpDIS->itemID), i);

	dc.Detach();	
}

//
LRESULT CFListCtrl::OnSetFont(WPARAM /*wParam*/, LPARAM /*lParam*/)
{
	LRESULT res =  Default();

	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
	return res;
}

void CFListCtrl::OnFontChange() 
{
	CListCtrl::OnFontChange();
	
	CRect rc;
	GetWindowRect( &rc );

	WINDOWPOS wp;
	wp.hwnd = m_hWnd;
	wp.cx = rc.Width();
	wp.cy = rc.Height();
	wp.flags = SWP_NOACTIVATE | SWP_NOMOVE | SWP_NOOWNERZORDER | SWP_NOZORDER;
	SendMessage( WM_WINDOWPOSCHANGED, 0, (LPARAM)&wp );
}

void CFListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	//
	int indx=GetTopIndex();
	if(m_nTextLineHeight>0 &&indx>=0)
	{	
		CRect row, header, window;
		GetItemRect(indx, &row, LVIR_BOUNDS);
		GetHeaderCtrl()->GetWindowRect(&header);
		GetWindowRect(&window);
		CSize sz(header.left-window.left, -indx*row.Height());
		Scroll(sz);
	}
	//
	CClientDC dc( this );
	CFont* pFont = GetFont();
	ASSERT( pFont );

	CFont* pOldFont = dc.SelectObject( pFont );
	ASSERT( pOldFont );

	TEXTMETRIC txtMetric;
	BOOL bRet = dc.GetTextMetrics( &txtMetric );
	ASSERT( bRet );
	
	m_nTextLineHeight=txtMetric.tmHeight + txtMetric.tmExternalLeading;
	ASSERT( 0 != m_nTextLineHeight );
	
	lpMeasureItemStruct->itemHeight = m_nTextLineHeight*GetOptions().m_nTextLineCount+4;		
}

void CFListCtrl::DrawFItem(CDC* pDC, HFROW hFRow, int iFColumn)
{
	ASSERT(hFRow!=NULL);		
	CRect rcFItem;
	GetFRect(hFRow, iFColumn, FLRC_FITEM, rcFItem);
	UpdateMemDC(pDC, rcFItem);	

	//			MEMDRAW	
	DrawElement(FLDR_FITEM_BACKGROUND, &m_dcFItem, hFRow, iFColumn, NULL);	
	DrawElement(FLDR_FITEM_TEXT, &m_dcFItem, hFRow, iFColumn, NULL);	
	DrawElement(FLDR_FITEM_TREEBUTTON, &m_dcFItem, hFRow, iFColumn, NULL);		
	DrawElement(FLDR_FITEM_CHECKBOX, &m_dcFItem, hFRow, iFColumn, NULL);		
	DrawElement(FLDR_FITEM_IMAGE, &m_dcFItem, hFRow, iFColumn, NULL);			
	DrawElement(FLDR_FITEM_BUTTON, &m_dcFItem, hFRow, iFColumn, NULL);
	DrawElement(FLDR_FITEM_FRAME, &m_dcFItem, hFRow, iFColumn, NULL);
	//
	pDC->BitBlt(rcFItem.left, rcFItem.top, rcFItem.Width(), rcFItem.Height(), &m_dcFItem, 0, 0, SRCCOPY);
}	
//===================================================================================
void CFListCtrl::UpdateMemDC(CDC *pDC, CRect rcFItem)
{
	if(m_dcFItem.m_hDC==NULL)
		m_dcFItem.CreateCompatibleDC(pDC);		
	
	if(m_bmpFItem.m_hObject==NULL || m_szFItem.cx<rcFItem.Width() || m_szFItem.cy<rcFItem.Height())
	{
		m_bmpFItem.DeleteObject();	
		m_szFItem=rcFItem.Size();			
		//
		BITMAPINFOHEADER RGB32BITSBITMAPINFO={	sizeof(BITMAPINFOHEADER),
												m_szFItem.cx, m_szFItem.cy,
												1, 32, BI_RGB, 0,0,0,0,0};		
		//
		VOID * pbits32=NULL; 
		m_bmpFItem.Attach(CreateDIBSection(m_dcFItem, (BITMAPINFO *)&RGB32BITSBITMAPINFO, DIB_RGB_COLORS, &pbits32, NULL, 0));
		HBITMAP m_holdBmp = (HBITMAP)SelectObject(m_dcFItem, m_bmpFItem);
		DeleteObject(m_holdBmp);
	}
	//
	CFont* pFont=pDC->GetCurrentFont();
	m_dcFItem.SelectObject(pFont);				
}

//	устанавливает ширину колонки так, чтобы она вмещала в себя текст в каждой строке таблицы
void CFListCtrl::UpdateFColumnWidth(int iFColumn)
{	
	if(GetTextLineCount()>1)
		return;	
	if(iFColumn==-1)	
		if((GetStyle()&LVS_NOCOLUMNHEADER) && GetFColumnCount()==1)
			iFColumn=0;	

	if(iFColumn>=0 && iFColumn<GetFColumnCount())
	{
		HFROW hFRow=m_lstFRows.GetHeadPosition();
		int max_width=0;
		CRect bounds, text, real;
		CClientDC dc(this);
	
			//creat font
		CFont* pFont=GetFont();
		CFont font;
		LOGFONT logFont;
		pFont->GetLogFont(&logFont);
	
		CFont fontNormal, fontBold;
		fontNormal.CreateFontIndirect(&logFont);
		logFont.lfWeight=FW_BOLD;
		fontBold.CreateFontIndirect(&logFont);		

		BOOL bBold;
		BOOL bBoldWeight=FALSE;
		CFont* pOldFont = dc.SelectObject(&fontNormal);
		
		while(hFRow!=NULL)
		{	
			if(GetFRow(hFRow)->m_iListIndex>=0)
			{
				GetFRect(hFRow, iFColumn, FLRC_FITEM, bounds);
				GetFRect(hFRow, iFColumn, FLRC_FITEM_TEXT, text);
				real.SetRect(0, 0, 0, 0);
				// set font
				bBold=IsFRowBold(hFRow);
				if(bBold!=bBoldWeight){
					dc.SelectObject(bBold ? &fontBold : &fontNormal);
					bBoldWeight=bBold;
				}
				//	calc width
				dc.DrawText(GetFItemText(hFRow, iFColumn), &real, DT_CALCRECT);
				bounds.right+=-text.Width()+real.Width()+2*FL_BORDER_SPACE+5;				
				if(bounds.Width()>max_width)
					max_width=bounds.Width();
			}
			m_lstFRows.GetNext(hFRow);
		}
		if(!(GetStyle()&LVS_NOCOLUMNHEADER))
		{
			if(bBoldWeight)
				dc.SelectObject(&fontNormal);
			bounds.SetRect(0, 0, 0, 0);
			dc.DrawText(GetFColumnCaption(iFColumn), &bounds, DT_CALCRECT);			
			bounds.right+=20;
			if(bounds.Width()>max_width)
				max_width=bounds.Width();
		}
		if(max_width)		
			SetColumnWidth(iFColumn, max_width>FL_MIN_FCOLUMN_WIDTH ? max_width : FL_MIN_FCOLUMN_WIDTH);
		dc.SelectObject(pOldFont);
	}
}

void CFListCtrl::SetRedraw(BOOL bRedraw, BOOL bClear)
{
	m_nRedrawCounter+=bRedraw ? -1 : 1;
	m_nRedrawCounter = m_nRedrawCounter<0 || bClear ? 0 :  m_nRedrawCounter;
	CListCtrl::SetRedraw(IsRedraw());
}

BOOL CFListCtrl::IsRedraw()
{
	return m_nRedrawCounter>0 ? FALSE : TRUE;
}

//===================================================================================
BOOL CFListCtrl::DrawElement(int nType, CDC *pDC, HFROW hFRow, int iFColumn, CRect *prcElement)
{	
	CRect rcElement;
	//
	switch(nType){
//
	case FLDR_FITEM_BACKGROUND:
		{			
			COLORREF crBackground=GetColor(FL_BACKGROUND, hFRow, iFColumn);
			COLORREF crText=GetColor(FL_TEXT, hFRow, iFColumn);					
			GetFRect(hFRow, iFColumn, FLRC_FITEM, rcElement, TRUE);			
			m_dcFItem.FillSolidRect(rcElement, crBackground);
			//
			m_dcFItem.SetBkColor (crBackground);
			m_dcFItem.SetTextColor (crText);
		}
		break;
//
	case FLDR_FITEM_TREEBUTTON:		
		{
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_TREEBUTTON, rcElement, TRUE))
				return FALSE;
			//
			CImageList* il=GetImageList(LVSIL_SMALL);
			if(GetNextFRow(hFRow, FL_FIRST_CHILD)!=NULL){
				if(GetOptions().m_iExpandButton<0 || il==NULL || GetOptions().m_iExpandButton >= il->GetImageCount())
				{	
					CPoint pt=rcElement.CenterPoint();
					rcElement.SetRect(pt.x-4, pt.y-4, pt.x+5, pt.y+5);
					pDC->FillSolidRect(&rcElement, RGB(255, 255, 255));
					pDC->Draw3dRect(&rcElement, RGB(172, 168, 153), RGB(172, 168, 153));
					pDC->MoveTo(rcElement.left+2, rcElement.top+rcElement.Height()/2);
					pDC->LineTo(rcElement.right-2, rcElement.top+rcElement.Height()/2);

					if(IsCollapsed(hFRow)){
						pDC->MoveTo(rcElement.left+rcElement.Width()/2, rcElement.top+2);
						pDC->LineTo(rcElement.left+rcElement.Width()/2, rcElement.bottom-2);
					}
				}
				else{
					il->Draw(pDC, IsCollapsed(hFRow) ? GetOptions().m_iExpandButton
						: GetOptions().m_iCollapseButton, 
						rcElement.TopLeft(), ILD_TRANSPARENT);
				}
			}			
		}
		break;
//
	case FLDR_FITEM_CHECKBOX:			
		{
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_CHECKBOX, rcElement, TRUE))
				return FALSE;

			XFList_ControlProfile &cp=*GetCP(hFRow, iFColumn);	
			CImageList* il=GetImageList(LVSIL_SMALL);
			int image_indx=-1;
			int button_state=DFCS_FLAT;
			//
			switch(GetFItem(hFRow, iFColumn).m_nCheckState){
			case 0: //unchecked
				image_indx=cp.m_checkImages.m_iUnchecked;
				if(image_indx<0)
					button_state|=DFCS_BUTTONCHECK;
				break;

			case 1://checked
				image_indx=cp.m_checkImages.m_iChecked;
				if(image_indx<0)
					button_state|=DFCS_CHECKED|DFCS_BUTTONCHECK;
				break;				

			case 2://indeterminate
				image_indx=cp.m_checkImages.m_iIndeterminate;
				if(image_indx<0)
					button_state=DFCS_INACTIVE|DFCS_CHECKED|DFCS_BUTTONCHECK;				
				break;			

			default:
				ASSERT(FALSE);
				break;
			}	
			//
			if(image_indx<0){
				rcElement.DeflateRect(2, 2, 2, 2);
				pDC->DrawFrameControl(rcElement, DFC_BUTTON, button_state);
			}
			else{
				IMAGEINFO ii;
				il->GetImageInfo(image_indx, &ii);
				il->Draw(pDC, image_indx, CPoint(rcElement.left,
					(rcElement.bottom+rcElement.top)/2 -
					(ii.rcImage.bottom-ii.rcImage.top)/2+1), ILD_TRANSPARENT);
			}
		}
		break;
//
	case FLDR_FITEM_IMAGE:
		{
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_IMAGE, rcElement, TRUE))
				return FALSE;
			//
			CImageList* il=GetImageList(LVSIL_SMALL);
			int indx=GetFItem(hFRow, iFColumn).m_iImageIndex;
			IMAGEINFO ii;
			il->GetImageInfo(indx, &ii);
			il->Draw(pDC, indx, CPoint(rcElement.left,
				(rcElement.bottom+rcElement.top)/2 -
				(ii.rcImage.bottom-ii.rcImage.top)/2+1), ILD_TRANSPARENT);
		}
		break;
//
	case FLDR_FITEM_BUTTON:
		{
			if(GetSelectedFRow()!=hFRow || GetSelectedFColumn()!=iFColumn ||
				!GetFRect(hFRow, iFColumn, FLRC_FITEM_BUTTON, rcElement, TRUE))
				return FALSE;
			//								
			//(FL_ISFLAT() ? DFCS_FLAT : 0)|
			pDC->DrawFrameControl(rcElement, DFC_SCROLL, DFCS_SCROLLDOWN);//DFCS_BUTTONPUSH);			
			if(GetCP(hFRow, iFColumn)->m_nControlType&FL_PLUSBUTTON)
			{
				COLORREF cr=GetSysColor(COLOR_BTNFACE);
				rcElement.DeflateRect(1, 1, 1, 1);				
				pDC->FillSolidRect(rcElement, cr);
				
				cr=GetSysColor(COLOR_BTNTEXT);
				rcElement.DeflateRect(3, 3, 3, 3);	
				rcElement.right-=rcElement.Width()%2 ? 1 : 0;
				rcElement.bottom=rcElement.top+rcElement.Width();
				CPoint center=rcElement.CenterPoint();
				CRect rc=rcElement;

				rc.top=center.y-1;
				rc.bottom=rc.top+2;				
				pDC->FillSolidRect(rc, cr);

				rc=rcElement;
				rc.left=center.x-1;
				rc.right=rc.left+2;
				pDC->FillSolidRect(rc, cr);
			}
			else if(FL_ISBUTTON(GetCP(hFRow, iFColumn)->m_nControlType))
			{
				rcElement.DeflateRect(2, 1, 1, 1);				
				pDC->SetBkColor(GetSysColor(COLOR_BTNFACE));
				pDC->SetTextColor(GetSysColor(COLOR_BTNTEXT));				
				pDC->DrawText(_T("..."), -1, rcElement, DT_CENTER);
			}			
		}
		break;
//
	case FLDR_FITEM_TEXT:		
		{	
			if(!GetFRect(hFRow, iFColumn, FLRC_FITEM_TEXT, rcElement, TRUE)
				)//|| (FL_ISCONTROL() && hFRow==GetSelectedFRow() && iFColumn==GetSelectedFColumn()))
				return FALSE;
			rcElement.DeflateRect(FL_BORDER_SPACE, 1, FL_BORDER_SPACE, 2);
			//
			CString text=GetFItem(hFRow, iFColumn).m_strText;						
			int nFormat=GetFColumn(iFColumn).m_nFormat| DT_EXTERNALLEADING | DT_NOPREFIX | DT_EXTERNALLEADING;
			nFormat|= GetOptions().m_nTextLineCount>1 ? DT_WORDBREAK : 0;
			if(GetFRow(hFRow)->m_bBoldText){
				CFont* pFont=pDC->GetCurrentFont();
				CFont font;
				LOGFONT logFont;
				pFont->GetLogFont(&logFont);
				logFont.lfWeight=FW_BOLD;
				font.CreateFontIndirect(&logFont);
				pFont=(CFont*)pDC->SelectObject(&font);			
				pDC->DrawText(text, -1, rcElement, nFormat);
				pDC->SelectObject(pFont)->DeleteObject();	
			}
			else								
				pDC->DrawText(text, -1, rcElement, nFormat);
		}	
		break;

	case FLDR_FITEM_FRAME:
		{
			COLORREF cr=RGB(0, 0, 0);
			GetFRect(hFRow, iFColumn, FLRC_FITEM, rcElement, TRUE);
			//
			if(FL_ISGRID()){
				cr=GetSysColor(COLOR_BTNFACE);
				rcElement.InflateRect(0, 1, 1, 0);
				pDC->Draw3dRect(rcElement, cr, cr);
				rcElement.DeflateRect(0, 1, 1, 0);
			}			
		}
		break;		

	case FLDR_BACKGROUND:
		{
			BOOL bGrid=FL_ISGRID();
			//ROWS
			CRect rcRows, rcCol, rcDraw, rcClient;
			GetFRect(NULL, 0, FLRC_VISIBLE_FROWS, rcRows);
			GetClientRect(&rcClient);
			int height=GetFRowHeight();
			
			COLORREF color;
			for(int i=0; i<GetFColumnCount(); i++)
			{	
				color=GetColor(FL_BACKGROUND, NULL, i);
				GetFRect(NULL, i, FLRC_FCOLUMN, rcCol);				
				//				
				if(bGrid){
					rcCol.left+= i ? 1 : 0;
					rcDraw=rcCol;
					rcDraw.bottom=rcRows.top-1;					
					pDC->FillSolidRect(rcDraw, color);
					//
					rcDraw=rcCol;
					rcDraw.top=rcRows.bottom;					
					rcDraw.bottom=rcDraw.top+height-1;
					while(rcDraw.top<rcClient.bottom)
					{
						pDC->FillSolidRect(rcDraw, color);
						rcDraw.top+=height;
						rcDraw.bottom=rcDraw.top+height-1;
					}
				}
				else{
					rcDraw=rcCol;
					rcDraw.bottom=rcRows.top;
					pDC->FillSolidRect(rcDraw, color);
					//
					rcDraw=rcCol;
					rcDraw.top=rcRows.bottom;					
					pDC->FillSolidRect(rcDraw, color);
				}									
				//
			}			
			color=GetColor(FL_BACKGROUND, NULL, -1);
			GetClientRect(rcDraw);
			rcDraw.left=rcRows.right;
			rcDraw.top=rcRows.top-2;
			if(bGrid)
			{			
				rcDraw.bottom=rcDraw.top+2;//rcRows.top-1;				
				pDC->FillSolidRect(rcDraw, color);
				//
				rcDraw.top+=2;
				rcDraw.bottom=rcDraw.top+height-1;
				while(rcDraw.top<rcClient.bottom)
				{
					pDC->FillSolidRect(rcDraw, color);
					rcDraw.top+=height;
					rcDraw.bottom=rcDraw.top+height-1;
				}
			}
			else
				pDC->FillSolidRect(rcDraw, color);		
		}
		break;

	case FLDR_UNDERHEADER:
		{
			CRect rcErase, rcHeader, rcRows;
			GetClientRect(rcErase);
			GetHeaderCtrl()->GetClientRect(rcHeader);
			rcErase.top=rcHeader.bottom;

			GetFRect(NULL, 0, FLRC_VISIBLE_FROWS, rcRows);
			rcErase.bottom=rcRows.top;
			//CRgn rgnErase, rgnRows;
			//rgnErase.CreateRectRgnIndirect(rcErase);
			//rgnRows.CreateRectRgnIndirect(rcRows);
			//rgnErase.CombineRgn(&rgnErase, &rgnRows, RGN_DIFF);

			InvalidateRect(rcErase);
			//InvalidateRgn(&rgnErase);	
		}
		break;

	case FLDR_FCOLUMN_BACKGROUND:
		{
			CRect rcErase, rcHeader, rcRows;
			GetClientRect(rcErase);
			GetFRect(NULL, iFColumn, FLRC_FCOLUMN, rcErase);			
			GetHeaderCtrl()->GetClientRect(rcHeader);
			rcErase.top=rcHeader.bottom;
			//
			GetFRect(NULL, 0, FLRC_VISIBLE_FROWS, rcRows);						
			CRgn rgnErase, rgnRows;
			rgnErase.CreateRectRgnIndirect(rcErase);
			rgnRows.CreateRectRgnIndirect(rcRows);
			rgnErase.CombineRgn(&rgnErase, &rgnRows, RGN_DIFF);
			//
			InvalidateRgn(&rgnErase);
		}
		break;

//
	default:
		ASSERT(FALSE);
		break;
	}
//
	if(prcElement!=NULL)
		*prcElement=rcElement;
	return TRUE;
}

//===================================================================================
void CFListCtrl::OnSysColorChange() 
{
	CListCtrl::OnSysColorChange();
	
	// TODO: Add your message handler code here
	
}

//===================================================================================
BOOL CFListCtrl::OnEraseBkgnd(CDC* pDC) 
{	
	return DrawElement(FLDR_BACKGROUND, pDC, NULL, 0, NULL);	
}
//===================================================================================
void CFListCtrl::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if(GetFocus()!=this) SetFocus();
	if(nSBCode==SB_THUMBTRACK || nSBCode==SB_ENDSCROLL)
		DrawElement(FLDR_UNDERHEADER, NULL, NULL, 0, NULL);
	CListCtrl::OnHScroll(nSBCode, nPos, pScrollBar);
}
//===================================================================================
void CFListCtrl::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{	
	if(GetFocus()!=this) SetFocus();
	CListCtrl::OnVScroll(nSBCode, nPos, pScrollBar);
}
//===================================================================================
BOOL CFListCtrl::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	HD_NOTIFY *pHD = (HD_NOTIFY*)lParam;	
	if((pHD->hdr.code == HDN_ITEMCHANGEDA 
		|| pHD->hdr.code == HDN_ITEMCHANGEDW 
		|| pHD->hdr.code == HDN_ITEMCHANGINGA 
		|| pHD->hdr.code == HDN_ITEMCHANGINGW) 
		&& (GetStyle() & LVS_TYPEMASK) == LVS_REPORT)
	{
		if(pHD->pitem->mask&HDI_WIDTH)
		{
			if(GetFocus()!=this) SetFocus();
			int width=LOWORD(((DWORD)pHD->pitem->cxy));			
			if(GetFColumn(pHD->iItem).m_nTmpWidth!=width)
			{
				GetFColumn(pHD->iItem).m_nTmpWidth=width;	
				SetColumnWidth(pHD->iItem, width);			
				DrawElement(FLDR_FCOLUMN_BACKGROUND, NULL, NULL, pHD->iItem, NULL);			
			}		
		}
	}	
	*pResult = 0;
	return CListCtrl::OnNotify(wParam, lParam, pResult);
}

//
//
//			INPUT TRANSLATE
//
//

BOOL CFListCtrl::HitTestMouseClick(CPoint point, HFROW &hFRow, int &iFColumn)
{	
	int count=GetItemCount();
	if(!count)
		return FALSE;
	//
	int row = GetTopIndex();
	if(row>=count)
		return FALSE;
	int bottom = row + GetCountPerPage()+1;
	if(bottom>count)
		bottom=count;
	//
	CRect rect;
	GetItemRect(row, &rect, LVIR_BOUNDS);	
	row+= (point.y-rect.top)/rect.Height();
	if(row>=bottom)	
		return FALSE;
	//	
	hFRow=(HFROW)GetItemData(row);	
	for(iFColumn=0; iFColumn<GetFColumnCount(); iFColumn++){
		GetFRect(hFRow, iFColumn, FLRC_FITEM, rect);
		if(rect.PtInRect(point))
			return TRUE;
	}
	//	
	return FALSE;
}

//
//
//		NOTIFY
//
//


void CFListCtrl::NotifyInit(FL_NOTIFY &notify, HFROW hFRow, int iFColumn, int code)
{
	OnEvent(notify, hFRow, iFColumn, code);
	//	
	notify.hdr.code=code;
	notify.hdr.hwndFrom=m_hWnd;
	notify.hdr.idFrom=GetDlgCtrlID();
	//
	notify.hFRow=hFRow;
	notify.iFColumn=iFColumn;	
	notify.bCollapsed=IsCollapsed(hFRow);
	if(hFRow!=NULL && iFColumn>=0){
		notify.strText= GetFItem(hFRow, iFColumn).m_strText;
		notify.iControlProfile=GetFItem(hFRow, iFColumn).m_iControlProfile;
		notify.dwUserData= GetFItem(hFRow, iFColumn).m_dwUserData;
		notify.nCheckState= GetFItem(hFRow, iFColumn).m_nCheckState;
	}
	else{
		notify.strText= "";
		notify.iControlProfile=-1;
		notify.dwUserData= 0;
		notify.nCheckState= 0;
	}
}

LRESULT CFListCtrl::NotifySend(FL_NOTIFY &notify)
{
	if(m_pNotifyWnd!=NULL)
		return m_pNotifyWnd->SendMessage(WM_NOTIFY, GetDlgCtrlID(),  (LPARAM) &notify);
	return GetParent()->SendMessage(WM_NOTIFY, GetDlgCtrlID(),  (LPARAM) &notify);
}

//===================================================================================

BEGIN_MESSAGE_MAP(CFListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CFListCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_KEYDOWN()
	ON_WM_DESTROY()			
	ON_WM_CONTEXTMENU()	
	ON_WM_SYSCOLORCHANGE()	
	ON_WM_HSCROLL()	
	ON_WM_LBUTTONDBLCLK()
	ON_WM_VSCROLL()
	ON_WM_CHAR()
	ON_NOTIFY(HDN_ITEMCLICKA, 0, OnHeaderClicked) 
	ON_NOTIFY(HDN_ITEMCLICKW, 0, OnHeaderClicked)
	ON_NOTIFY(HDN_DIVIDERDBLCLICKA, 0, OnDividerDblClk) 
	ON_NOTIFY(HDN_DIVIDERDBLCLICKW, 0, OnDividerDblClk)
	ON_WM_FONTCHANGE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_REFLECT(FLNM_INPLACE_KILLFOCUS, OnInplaceControlNotify)	
	ON_MESSAGE(WM_SETFONT, OnSetFont)
	ON_WM_MEASUREITEM_REFLECT()
END_MESSAGE_MAP()



/////////////////////////////////////////////////////////////////////////////
// CFListCtrl message handlers

BOOL CFListCtrl::PreCreateWindow(CREATESTRUCT& cs) 
{
	cs.style|=LVS_REPORT|LVS_SHOWSELALWAYS|LVS_SINGLESEL|LVS_OWNERDRAWFIXED;
	cs.style&= ~LVS_EDITLABELS|WS_HSCROLL;	
	return CListCtrl::PreCreateWindow(cs);
}

void CFListCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	TranslateMouse(nFlags, point, FALSE);	
}
void CFListCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	//AfxMessageBox("!!");
	int t=11;

//	TranslateMouse(nFlags, point, FALSE);	
}

void CFListCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	TranslateMouse(nFlags, point, TRUE);	
}

void CFListCtrl::TranslateMouse(UINT /*nFlags*/, CPoint point, BOOL bDoubleClick)
{	
	SetFocus();
	//
	HFROW hFRow;
	int iFColumn;
	if(HitTestMouseClick(point, hFRow, iFColumn))
	{
		CRect rect;
		GetFRect(hFRow, iFColumn, FLRC_FITEM_TREEBUTTON, rect);
		//tree button
		if(rect.PtInRect(point))		
			Collapse(hFRow, !IsCollapsed(hFRow));
		else{
			HFROW hOldFRow=GetSelectedFRow();
			int iOldFColumn=GetSelectedFColumn();
			Select(hFRow, iFColumn);
	
			if(!IsReadOnly())
			{
				if(GetFRect(hFRow, iFColumn, FLRC_FITEM_CHECKBOX, rect) && rect.PtInRect(point))
				{
					FL_NOTIFY notify;
					NotifyInit(notify, hFRow, iFColumn, FLNM_CHECK);
					int nState=notify.nCheckState;
					notify.nCheckState= GetCP(notify.iControlProfile)->m_nControlType&FL_AUTOCHECK ? !nState : nState;
					if(FL_ISOK(NotifySend(notify)))// && (GetCP(notify.iControlProfile)->m_nControlType&FL_AUTOCHECK))
					{
						SetFItemCheck(hFRow, iFColumn, notify.nCheckState);// !nState
					}
				}
				else if(hOldFRow==hFRow && iOldFColumn==iFColumn && GetFRect(hFRow, iFColumn, FLRC_FITEM_BUTTON, rect) && rect.PtInRect(point))
				{												
					InplaceControl(WM_LBUTTONDOWN, 0);
				}
				else if(bDoubleClick && (GetFRect(hFRow, iFColumn, FLRC_FITEM_TEXT, rect), rect.PtInRect(point)))
				{
					InplaceControl(WM_LBUTTONDBLCLK, 0);				
				}
			}
		}
	}			
}

void CFListCtrl::OnContextMenu(CWnd* /*pWnd*/, CPoint point) 
{	
	HFROW hFRow;
	int iFColumn;
	ScreenToClient(&point);
	if(HitTestMouseClick(point, hFRow, iFColumn))	
		Select(hFRow, iFColumn);	
	Default();	
}

BOOL CFListCtrl::PreTranslateMessage(MSG* pMsg) 
{	
	if(pMsg->message==WM_SYSKEYDOWN && (pMsg->wParam==VK_DOWN || pMsg->wParam==VK_UP) && !FL_ISCONTROL())
	{			
		InplaceControl(WM_KEYDOWN, pMsg->wParam);		
		return TRUE;
	}
	if(pMsg->message==WM_KEYDOWN)
	if(pMsg->wParam == VK_RETURN||pMsg->wParam == VK_F2)
	{			
		InplaceControl(WM_KEYDOWN, VK_RETURN);		
		return TRUE;
	}
	return CListCtrl::PreTranslateMessage(pMsg);
}

void CFListCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	HFROW hSelFRow=GetSelectedFRow();
	if(hSelFRow==NULL)
	{
		if(nChar==VK_DOWN || nChar==VK_NEXT || (nChar==VK_TAB && GetKeyState(VK_SHIFT)>=0))
		{
			hSelFRow=GetNextFRow(NULL, FL_TOP);
			Select(hSelFRow, 0);
		}
		else if(nChar==VK_UP || nChar==VK_PRIOR)
		{
			hSelFRow=GetNextFRow(NULL, FL_BOTTOM);
			Select(hSelFRow, 0);		
		}
		else if(nChar==VK_TAB && GetKeyState(VK_SHIFT)<0)
		{
			hSelFRow=GetNextFRow(NULL, FL_BOTTOM);
			Select(hSelFRow, GetFColumnCount()-1);		
		}
		return;
	}
	int iSelFItem=GetSelectedFColumn();
	XFList_Row *pFRow=GetFRow(hSelFRow);
	//XFList_Row::XFList_Item *pFItem= iSelFItem>=0 ? &GetFItem(hSelFRow, iSelFItem) : NULL;

	switch(nChar){	
	case VK_LEFT:
		if(0==iSelFItem)//расширенный режим реакции
		{
			if(!IsCollapsed(hSelFRow))
			{
				Collapse(hSelFRow, 1);
				break;
			}
			HFROW hParent=GetNextFRow(hSelFRow,FL_PARENT);
			if(hParent)
			{
				if(int(hParent)>int(FL_FIRST))
					Select(hParent, iSelFItem);
				break;
			}
		}
	case VK_HOME:
		if(iSelFItem>0)		
			Select(hSelFRow, nChar==VK_HOME ? 0 : iSelFItem-1);
		break;
	
	case VK_RIGHT:
		if(0==iSelFItem)//расширенный режим реакции
		{
			if(IsCollapsed(hSelFRow)&&(!pFRow->m_lstChildFRows.IsEmpty()))
			{
				Collapse(hSelFRow, 0);
				break;
			}
		}
	case VK_END:
		if(iSelFItem<(GetFColumnCount()-1))		
			Select(hSelFRow, nChar==VK_END ? GetFColumnCount()-1 : iSelFItem+1);					
		break;

	case VK_UP:		
		if(pFRow->m_iListIndex>0 && GetKeyState(VK_MENU)>=0)
			Select((HFROW)GetItemData(pFRow->m_iListIndex-1), iSelFItem);			
		break;

	case VK_PRIOR:
		{
			int iListIndex=pFRow->m_iListIndex-GetCountPerPage()+1;
			hSelFRow= (HFROW) GetItemData(iListIndex>=0 ? iListIndex : 0);			
			Select(hSelFRow, iSelFItem);
		}
		break;

	case VK_DOWN:		
		if(pFRow->m_iListIndex<(GetItemCount()-1) && GetKeyState(VK_MENU)>=0)
			Select((HFROW)GetItemData(pFRow->m_iListIndex+1), iSelFItem);			
		break;

	case VK_NEXT:
		{
			int iListIndex=pFRow->m_iListIndex+GetCountPerPage()-1;
			hSelFRow= (HFROW) GetItemData(iListIndex<GetItemCount() ? iListIndex : GetItemCount()-1);			
			Select(hSelFRow, iSelFItem);
		}
		break;	

	case VK_TAB:	
		if(GetKeyState(VK_SHIFT)>=0)
		{
			if(iSelFItem<(GetFColumnCount()-1))				
				Select(hSelFRow, nChar==VK_END ? GetFColumnCount()-1 : iSelFItem+1);							
			else if(pFRow->m_iListIndex<(GetItemCount()-1))		
				Select((HFROW)GetItemData(pFRow->m_iListIndex+1), 0);
		}
		else{
			if(iSelFItem>0)		
				Select(hSelFRow, nChar==VK_HOME ? 0 : iSelFItem-1);
			else if(pFRow->m_iListIndex>0)
				Select((HFROW)GetItemData(pFRow->m_iListIndex-1), GetFColumnCount()-1);
		}
		break;

	case VK_ADD:
	case VK_SUBTRACT:
		Collapse(hSelFRow, nChar==VK_SUBTRACT);		
		break;

	case VK_SPACE:
		break;

	case VK_DELETE:
		if(GetNextFRow(hSelFRow,FL_PARENT)==FL_ROOT)
		if(hSelFRow!=GetNextFRow(FL_ROOT,FL_LAST_CHILD))
			DeleteFRow(hSelFRow);	
		break;

	default:
		CListCtrl::OnKeyDown(nChar, nRepCnt, nFlags);
		break;
	}
	InplaceControl(WM_KEYDOWN, nChar);			
}

void CFListCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	CListCtrl::OnChar(nChar, nRepCnt, nFlags);		
	InplaceControl(WM_CHAR, nChar);
}

LRESULT CFListCtrl::InplaceControl(UINT code, UINT nChar, FL_INPLACE_NOTIFY* pInplaceNotify)
{
	LRESULT lr=S_OK;
	HFROW hSelFRow=GetSelectedFRow();
	int iSelFColumn=GetSelectedFColumn();	
	XFList_ControlProfile *pcp;
	//
	if(code==FLNM_INPLACE_KILLFOCUS && m_hFRow!=NULL && m_iFColumn>=0)
	{
		CWnd *pOldControl=m_pInplaceControl;
		m_pInplaceControl=NULL;

		pcp=GetCP(m_hFRow, m_iFColumn);		
		switch((pcp->m_nControlType)&FL_CONTROL_MASK){
		case FL_EDIT:
		case FL_EDIT|FL_BUTTON:
		case FL_COMBOBOX:
		case FL_COMBOBOX|FL_EDIT:			
			if(VK_RETURN==nChar || VK_TAB==nChar)
			{
				//подгонка для CDebugMessageBar
				HFROW hParent=GetNextFRow(pInplaceNotify->hFRow,FL_PARENT);
				if(((UINT)hParent)>50)
				{
					return FL_OK;
				}

				FL_NOTIFY notify;
				notify.strText=pInplaceNotify->strText;
				NotifyInit(notify, pInplaceNotify->hFRow, pInplaceNotify->iFColumn, FLNM_EDIT_END);
				notify.strText=pInplaceNotify->strText;
				if(pcp->m_nControlType&FL_COMBOBOX)
					notify.dwUserData=pInplaceNotify->dwUserData;
				lr=NotifySend(notify);
				if(FL_ISCONTINUE(lr))				
					m_pInplaceControl=pOldControl;
				if(FL_ISOK(lr))	
				{
					SetFItemText(pInplaceNotify->hFRow, pInplaceNotify->iFColumn, notify.strText);
					if(pcp->m_nControlType&FL_COMBOBOX)
						SetFItemData(pInplaceNotify->hFRow, pInplaceNotify->iFColumn, notify.dwUserData);
				}
				UpdateFRow(pInplaceNotify->hFRow);
				if(FL_ISCONTINUE(lr))									
					return lr;//продолжаем редактирование
				//
				if(nChar==VK_TAB){					
					m_nChar=VK_TAB;
					OnKeyDown(VK_TAB, 0, 0);					
					return FL_OK;
				}
			}
			else
				UpdateFRow(pInplaceNotify->hFRow);
			break;
		}		
		m_hFRow=NULL;
		m_iFColumn=-1;
		return lr;		
	}
	//
	//
	//
	if(hSelFRow==NULL || iSelFColumn<0 || FL_ISCONTROL())
		return FL_OK;
	m_hFRow=hSelFRow;
	m_iFColumn=iSelFColumn;	
	pcp=GetCP(m_hFRow, m_iFColumn);
	ASSERT(pcp!=NULL);

	switch(pcp->m_nControlType&FL_CONTROL_MASK){
	case FL_EDIT:
	case FL_EDIT|FL_BUTTON:
		{
			BOOL bShowEdit=FALSE;
			CString text;
			if(code==WM_CHAR)
			{
				if(::IsCharAlphaNumeric((TCHAR)nChar))
				{
					bShowEdit=TRUE;
					text=(TCHAR)nChar;
				}
			}
			else if(code==WM_LBUTTONDBLCLK || nChar==VK_RETURN || nChar==VK_F2 || (nChar==VK_TAB && m_nChar==VK_TAB))
			{
				bShowEdit=TRUE;
				text=GetFItemText(m_hFRow, m_iFColumn);
			}
			//
			if(bShowEdit)
			{		
				FL_NOTIFY notify;
				NotifyInit(notify, m_hFRow, m_iFColumn, FLNM_EDIT_BEGIN);
				if(FL_ISOK(NotifySend(notify)))
				{				
					CRect rect;
					GetFRect(m_hFRow, m_iFColumn, FLRC_FITEM_TEXT, rect);
					rect.DeflateRect(0, 1, 0, 0);
					CFList_Edit* pEdit = new CFList_Edit(m_hFRow, m_iFColumn);
					//
					DWORD dwStyle=(GetOptions().m_nTextLineCount>1 ? ES_AUTOVSCROLL|ES_MULTILINE|ES_WANTRETURN: ES_AUTOHSCROLL);
					pEdit->Create(WS_CHILD|WS_VISIBLE|dwStyle, rect, this, IDC_INPLACE_EDIT);
					m_pInplaceControl=pEdit;
					pEdit->ModifyStyleEx(WS_EX_CLIENTEDGE, 0);
					UpdateFRow(m_hFRow);
					
					pEdit->SetWindowText(text);
					pEdit->SetSel(text.GetLength(), -1);
					
					pEdit->SetFont(GetFont());				
					pEdit->SetFocus();
				}
			}
		}
		if(FL_ISCONTROL())
			break;

	case FL_BUTTON:
		{
			if(WM_LBUTTONDOWN==code || (VK_SPACE==nChar && WM_KEYDOWN==code))
			{			
				FL_NOTIFY notify;
				NotifyInit(notify, m_hFRow, m_iFColumn, FLNM_BUTTON_CLICK);
				if(FL_ISOK(NotifySend(notify)))
				{
					SetFocus();
					SetFItemText(m_hFRow, m_iFColumn, notify.strText);
					SetFItemData(m_hFRow, m_iFColumn, notify.dwUserData);
				}
			}
		}
		break;

	case FL_COMBOBOX:
		{
			if( (nChar==VK_DOWN || nChar==VK_UP) && GetKeyState(VK_MENU)<0 || nChar==VK_RETURN
				|| WM_LBUTTONDOWN==code || WM_LBUTTONDBLCLK==code || VK_F2==nChar || (nChar==VK_TAB && m_nChar==VK_TAB))
			{							
				FL_NOTIFY notify;
				NotifyInit(notify, m_hFRow, m_iFColumn, FLNM_COMBOBOX_EXPAND);
				pcp->m_ciSelect.strText=notify.strText;
				pcp->m_ciSelect.dwUserData=notify.dwUserData;
				if(FL_ISOK(NotifySend(notify)))
				{
					CRect rect;
					GetFRect(m_hFRow, m_iFColumn, FLRC_FITEM_COMBOBOX, rect);
					rect.InflateRect(0, 2, 0, 0);
					rect.bottom=rect.top+FL_STEP-1;
					//rect.InflateRect(FL_BORDER_SPACE, 1, FL_BORDER_SPACE, (FL_ISGRID() ? 2 : 3) );				
					int nHeight=rect.Height();
					//rect.top+=nHeight;
					rect.bottom = rect.top + FL_DROPDOWN_SIZE * nHeight;					

					CFList_ComboBox *pCombo= new CFList_ComboBox(this, m_hFRow, m_iFColumn, &(pcp->m_aComboItems), pcp->m_ciSelect);
					pCombo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWNLIST|WS_VSCROLL
						|(pcp->m_nControlType&FL_SORT ? CBS_SORT : 0), rect, this, IDC_INPLACE_COMBO);	
					m_pInplaceControl=pCombo;
					
					pCombo->SetItemHeight(-1, nHeight);
					UpdateFRow(m_hFRow);

					pCombo->SetFocus();
					pCombo->ShowDropDown();
				}
			}
		}
		break;

	case FL_COMBOBOX|FL_EDIT:
		{
			FL_NOTIFY notify;
			NotifyInit(notify, m_hFRow, m_iFColumn, FLNM_COMBOBOX_EXPAND);

			BOOL bShowEdit=FALSE;
			CString text;
			if(code==WM_CHAR)
			{
				if(::IsCharAlphaNumeric((TCHAR)nChar))
				{
					bShowEdit=TRUE;
					//text=nChar;
					pcp->m_ciSelect.strText=(TCHAR)nChar;
					pcp->m_ciSelect.dwUserData=0;
				}
			}
			else if( (nChar==VK_DOWN || nChar==VK_UP) && GetKeyState(VK_MENU)<0 || nChar==VK_RETURN
				|| WM_LBUTTONDOWN==code || WM_LBUTTONDBLCLK==code || VK_F2==nChar || (nChar==VK_TAB && m_nChar==VK_TAB))
			{
				bShowEdit=TRUE;	
				pcp->m_ciSelect.strText=notify.strText;
				pcp->m_ciSelect.dwUserData=notify.dwUserData;
			}
			//
			if(bShowEdit)
			{						
				if(FL_ISOK(NotifySend(notify)))
				{
					CRect rect;
					GetFRect(m_hFRow, m_iFColumn, FLRC_FITEM_COMBOBOX, rect);
					rect.InflateRect(0, 2, 0, 0);
					rect.bottom=rect.top+FL_STEP-1;
					//rect.InflateRect(FL_BORDER_SPACE, 1, FL_BORDER_SPACE, (FL_ISGRID() ? 2 : 3) );				
					int nHeight=rect.Height();
					//rect.top+=nHeight;
					rect.bottom = rect.top + FL_DROPDOWN_SIZE * nHeight;					

					CFList_ComboBox *pCombo= new CFList_ComboBox(this, m_hFRow, m_iFColumn, &(pcp->m_aComboItems), pcp->m_ciSelect);
					pCombo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN|WS_VSCROLL|CBS_AUTOHSCROLL
						|(pcp->m_nControlType&FL_SORT ? CBS_SORT : 0), rect, this, IDC_INPLACE_COMBO);	
					m_pInplaceControl=pCombo;
					
					pCombo->SetItemHeight(-1, nHeight);
					UpdateFRow(m_hFRow);

					//pCombo->SetFocus();
					pCombo->ShowDropDown();					

					CString text=pcp->m_ciSelect.strText;
					pCombo->SetEditSel(text.GetLength(), -1);
				}
			}
		}
		break;
	}
	
	m_nChar=0;
	return lr;
}


void CFListCtrl::OnInplaceControlNotify(NMHDR * pNotifyStruct, LRESULT * result)
{
	FL_INPLACE_NOTIFY *pNotify=(FL_INPLACE_NOTIFY*)pNotifyStruct;
	*result=FL_OK;
	if(pNotify->hdr.idFrom==IDC_INPLACE_EDIT || pNotify->hdr.idFrom==IDC_INPLACE_COMBO)
		*result=InplaceControl(FLNM_INPLACE_KILLFOCUS, pNotify->nChar, pNotify);	
}

//
//
//	
//			CFList_Edit
//
//
//


CFList_Edit::CFList_Edit(HFROW hFRow, int iFColumn)
{	
	m_nChar=VK_RETURN;
	m_hFRow=hFRow;
	m_iFColumn=iFColumn;
}

CFList_Edit::~CFList_Edit()
{
}


BEGIN_MESSAGE_MAP(CFList_Edit, CEdit)
	//{{AFX_MSG_MAP(CFList_Edit)
	ON_WM_KILLFOCUS()
	ON_WM_NCDESTROY()	
	//ON_WM_CREATE()	
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////

BOOL CFList_Edit::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| pMsg->wParam == VK_TAB
				|| GetKeyState( VK_CONTROL)				
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	return CEdit::PreTranslateMessage(pMsg);
}

void CFList_Edit::OnKillFocus(CWnd* pNewWnd)
{
	CEdit::OnKillFocus(pNewWnd);

	CFListCtrl::FL_INPLACE_NOTIFY notify;
	notify.hdr.code=FLNM_INPLACE_KILLFOCUS;
	notify.hdr.hwndFrom=GetParent()->m_hWnd;
	notify.hdr.idFrom=GetDlgCtrlID();
	GetWindowText(notify.strText);
	notify.hFRow=m_hFRow;
	notify.iFColumn=m_iFColumn;
	notify.nChar=m_nChar;

	LRESULT lr=GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(),  (LPARAM) &notify);
	if(FL_ISCONTINUE(lr))
		SetFocus();
	else
		DestroyWindow();	
}

void CFList_Edit::OnNcDestroy()
{		
	CEdit::OnNcDestroy();	
	delete this;	
}

void CFList_Edit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	m_nChar=nChar;
	switch(nChar){
	case VK_RETURN:
		if(GetKeyState(VK_CONTROL)<0 && (GetStyle()&ES_MULTILINE)==ES_MULTILINE)
			break;	
	case VK_TAB:
		GetParent()->SetFocus();
		return;

	case VK_ESCAPE:	
		GetParent()->SetFocus();
		return;			

	default:
		m_nChar=VK_RETURN;
		break;
	}
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);	
}

int CFList_Edit::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CEdit::OnCreate(lpCreateStruct) == -1)
		return -1;
	return 0;
}



//
//
//
//			CFList_ComboBox
//
//
//

CFList_ComboBox::CFList_ComboBox(CFListCtrl* pParent, HFROW hFRow, int iFColumn, FL_COMBOITEMS* paComboItems, FL_COMBOITEM ciSelect)
{
	m_hFRow=hFRow;
	m_iFColumn=iFColumn;	

	m_paComboItems=paComboItems;
	m_ciSelect=ciSelect;

	m_nChar=VK_RETURN;
	m_pParent=pParent;
}

CFList_ComboBox::~CFList_ComboBox()
{

}


BEGIN_MESSAGE_MAP(CFList_ComboBox, CComboBox)
	//{{AFX_MSG_MAP(CInPlaceList)
	ON_WM_CREATE()
	ON_CONTROL_REFLECT(CBN_KILLFOCUS,OnKillFocus)
	ON_WM_NCDESTROY()
	ON_WM_KEYDOWN()
	ON_WM_PAINT()	
	ON_CONTROL_REFLECT(CBN_CLOSEUP, OnCloseup)
	ON_CONTROL_REFLECT(CBN_EDITCHANGE, OnEditChange)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CInPlaceList message handlers

BOOL CFList_ComboBox::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message == WM_KEYDOWN )
	{
		if(pMsg->wParam == VK_RETURN
				|| pMsg->wParam == VK_DELETE
				|| pMsg->wParam == VK_ESCAPE
				|| pMsg->wParam == VK_TAB
				|| GetKeyState( VK_CONTROL)				
				)
		{
			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);
			return TRUE;		    	// DO NOT process further
		}
	}
	return CComboBox::PreTranslateMessage(pMsg);
}

int CFList_ComboBox::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CComboBox::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// Set the proper font
	CFont* font = GetParent()->GetFont();
	SetFont(font);

	int select=-1;
	FL_COMBOITEM *pci;
	for(int i=0; i<m_paComboItems->GetSize(); i++)
	{
		pci=&(m_paComboItems->ElementAt(i));
		AddString(pci->strText);
		SetItemData(i, pci->dwUserData);
		if(pci->strText==m_ciSelect.strText && pci->dwUserData==m_ciSelect.dwUserData)
			select=i;
	}
	if(select==-1)
	{
		InsertString(0, m_ciSelect.strText);
		SetItemData(0, m_ciSelect.dwUserData);		
	}
	//
	select=-1;
	CString text;
	for(i=0; i<GetCount(); i++)
	{
		GetLBText(i, text);
		if(text==m_ciSelect.strText && GetItemData(i)==m_ciSelect.dwUserData){
			select=i;
			break;
		}
	}
	ASSERT(select!=-1);
	SetCurSel(select);
	//
	SetFocus();	
	return 0;
}

void CFList_ComboBox::OnKillFocus()//CWnd* pNewWnd) 
{
	CComboBox::OnKillFocus(NULL);//pNewWnd);
	
	CFListCtrl::FL_INPLACE_NOTIFY notify;
	notify.hdr.code=FLNM_INPLACE_KILLFOCUS;
	notify.hdr.hwndFrom=GetParent()->m_hWnd;
	notify.hdr.idFrom=GetDlgCtrlID();
	
	int select=GetCurSel();	
	if(select!=CB_ERR && m_nChar!=VK_SPACE)
	{	
		GetLBText(select, notify.strText);
		notify.dwUserData=GetItemData(select);
	}
	else{
		GetWindowText(notify.strText);
		notify.dwUserData=0;
	}
	if(m_nChar==VK_SPACE)
		m_nChar=VK_RETURN;
	
	notify.hFRow=m_hFRow;
	notify.iFColumn=m_iFColumn;
	notify.nChar=m_nChar;

	LRESULT lr=GetParent()->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(),  (LPARAM) &notify);		
	if(FL_ISCONTINUE(lr))
		SetFocus();	
	else
		PostMessage( WM_CLOSE );
}

void CFList_ComboBox::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{	
	if(m_nChar!=VK_SPACE)
		m_nChar=nChar;
	switch(nChar){
	case VK_RETURN:	
	case VK_TAB:
		GetParent()->SetFocus();
		return;

	case VK_ESCAPE:	
		m_nChar=VK_ESCAPE;
		GetParent()->SetFocus();		
		return;		
			
	default:	
		m_nChar=VK_RETURN;
		break;
	}
	CComboBox::OnKeyDown(nChar, nRepCnt, nFlags);	
}

void CFList_ComboBox::OnCloseup()
{
	GetParent()->SetFocus();
}

void CFList_ComboBox::OnEditChange()
{	
	m_nChar=VK_SPACE;
}

void CFList_ComboBox::OnNcDestroy() 
{
	CComboBox::OnNcDestroy();	
	delete this;
}

void CFList_ComboBox::OnPaint() 
{				
	Default();	

	CRect rcItem, rcGrid;
	GetClientRect(&rcItem);
	rcGrid=rcItem;
	
	CDC* pDC = GetDC();
	COLORREF clr=GetSysColor(COLOR_WINDOW);
				
	pDC->Draw3dRect(rcItem, clr, clr);
	rcItem.DeflateRect(1,1);
	pDC->Draw3dRect(rcItem, clr, clr);
				
	CRect rcFill=rcItem;
	rcFill.DeflateRect(-1, 1, ::GetSystemMetrics(SM_CXHTHUMB)+1, 1);

	//		BUTTON
	CRect rcButton=rcItem;
	rcButton.left=rcButton.right-rcButton.Height();
	rcButton.DeflateRect(2, 1, -1, 1);
	pDC->Draw3dRect(rcButton, clr, clr);
	rcButton.DeflateRect(1, 0, 0, 0);
	pDC->Draw3dRect(rcButton, clr, clr);
	rcButton.DeflateRect(1, 0, 0, 1);
	if(m_pParent->GetExtendedStyle()&LVS_EX_GRIDLINES)
	{
		pDC->Draw3dRect(rcButton, clr, clr);
		rcButton.DeflateRect(1, 0, 0, 1);
	}
	pDC->DrawFrameControl(rcButton, DFC_SCROLL, DFCS_SCROLLDOWN);
	//
	if(m_pParent->GetCP(m_hFRow, m_iFColumn)->m_nControlType&FL_PLUSBUTTON)
	{
		CRect plus=rcButton;
		COLORREF cr=GetSysColor(COLOR_BTNFACE);
		plus.DeflateRect(1, 1, 1, 1);				
		pDC->FillSolidRect(plus, cr);
				
		cr=GetSysColor(COLOR_BTNTEXT);
		plus.DeflateRect(3, 3, 3, 3);	
		plus.right-=plus.Width()%2 ? 1 : 0;
		plus.bottom=plus.top+plus.Width();
		CPoint center=plus.CenterPoint();
		CRect rc=plus;

		rc.top=center.y-1;
		rc.bottom=rc.top+2;				
		pDC->FillSolidRect(rc, cr);

		rc=plus;
		rc.left=center.x-1;
		rc.right=rc.left+2;
		pDC->FillSolidRect(rc, cr);
	}
	//


	//		UP FROW	
	HFROW hUpFRow=m_pParent->GetNextFRow(m_hFRow, FL_PREV_VISIBLE);
	clr=m_pParent->GetColor(FL_BACKGROUND, hUpFRow, m_iFColumn);

	CRect rcUp=rcItem;
	rcUp.bottom=rcUp.top+2;
	rcUp.InflateRect(1, 1, 1, -1);
	pDC->Draw3dRect(rcUp, clr, clr);	
	//


	//		GRIDLINES
	if(m_pParent->GetExtendedStyle()&LVS_EX_GRIDLINES)
	{
		clr=GetSysColor(COLOR_BTNFACE);
		rcGrid.DeflateRect(0, 1, -1, 3);
		pDC->Draw3dRect(rcGrid, clr, clr);
		
		clr=GetSysColor(COLOR_WINDOW);
		rcGrid.right=rcGrid.left+1;
		rcGrid.DeflateRect(0, 1);
		pDC->Draw3dRect(rcGrid, clr, clr);
	}
	//*/


	//		DOWN FROW	
	HFROW hDownFRow=m_pParent->GetNextFRow(m_hFRow, FL_NEXT_VISIBLE);
	if(m_pParent->GetOptions().m_nTextLineCount==1)
		clr=m_pParent->GetColor(FL_BACKGROUND, hDownFRow, m_iFColumn);
	else
		clr=GetSysColor(COLOR_WINDOW);

	CRect rcDown=rcItem;
	rcDown.top=rcDown.bottom;
	rcDown.bottom=rcDown.top;	
	rcDown.InflateRect(1, 2, 1, 1);

	pDC->FillSolidRect(rcDown, clr);	
	//
	
	ReleaseDC(pDC);	
}

//						уф!!!  I did it!!!

