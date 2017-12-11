// MyView.cpp : implementation file
//

#include "stdafx.h"
#include "Resource.h"
#include "MyView.h"
#include "SubsView.h"
#include "MyADO.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMetaDataCont* pMD;
HTREEITEM pArrChancheParent[1];
HTREEITEM i_ch, m_root_doc, m_root_jrn, h_item_plan,m_root_vid_rasch, m_root; //, m_root
CMapStringToString pMapSSDocOsn;
char* ch_doc_form = "Форма";
char* ch_doc_mtrans = "Модуль";

extern CMyADO* pAdo;

/////////////////////////////////////////////////////////////////////////////
// CMyView CCmdUI 

//IMPLEMENT_DYNCREATE(CMyView, CFormView)
IMPLEMENT_DYNCREATE(CMyView, CColumnTreeView)
CMyView* CMyView::m_pView=NULL;


CMyView::CMyView()
{
	m_pView	= this;
	//{{AFX_DATA_INIT(CMyView)
	// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CMyView::~CMyView(){
}




//BEGIN_MESSAGE_MAP(CMyView, CFormView)
BEGIN_MESSAGE_MAP(CMyView, CColumnTreeView)
	//{{AFX_MSG_MAP(CMyView)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyView diagnostics

#ifdef _DEBUG
void CMyView::AssertValid() const
{
	CColumnTreeView::AssertValid();
}

void CMyView::Dump(CDumpContext& dc) const
{
	CColumnTreeView::Dump(dc);
}
#endif //_DEBUG


CString sGlobWndName, sGlobWndClassName;
HWND hFindWindow;
int iGlgNumberSearchChild, iGlgNumberSearchChildWindow = 0;
static BOOL CALLBACK ChildWindowProc(HWND hwnd, LPARAM lParam)
{
	TCHAR  cname[256];
	TCHAR  wname[256];
	
	//only display 1st generation (1-deep) children - 
	//(don't display child windows of child windows)
	GetClassName(hwnd, cname, sizeof(cname) / sizeof(TCHAR));
	GetWindowText(hwnd, wname, sizeof(wname) / sizeof(TCHAR));

	if (hFindWindow == NULL){ 
		if (!sGlobWndClassName.IsEmpty()){ 
			if (sGlobWndClassName.CompareNoCase(cname) == 0){ 
				hFindWindow = hwnd;
			}
		}
		if (!sGlobWndName.IsEmpty()){ 
			if (sGlobWndName.CompareNoCase(wname) == 0){ 
				hFindWindow = hwnd;
			}
		}
		if (iGlgNumberSearchChildWindow!=0){ 
			iGlgNumberSearchChild = iGlgNumberSearchChild + 1;
			if (iGlgNumberSearchChild == iGlgNumberSearchChildWindow){ 
				hFindWindow = hwnd;
			}

		}
	}
	return TRUE;
}
HWND FindChildWindow(HWND hParent, CString nClassName, CString nTitleName, int nNumbWnd = 0){
	sGlobWndName = nTitleName;
	sGlobWndClassName = nClassName;
	hFindWindow = NULL;
	iGlgNumberSearchChildWindow = nNumbWnd;
	iGlgNumberSearchChild = 0;
	EnumChildWindows(hParent, ChildWindowProc, (LONG)0);
	sGlobWndName = "";
	sGlobWndClassName = "";
	iGlgNumberSearchChildWindow = 0;
	return hFindWindow;

}

HWND FindSiblingWindow(HWND hParent, CString nClassName, CString nTitleName, int nNumbWnd = 0){
	return FindChildWindow(GetParent(hParent), nClassName, nTitleName, nNumbWnd);
}

void MyKillStyle(HWND nh_Wnd){
// Попробуем убрать стиль WS_EX_CLIENTEDGE
	HWND hParent = nh_Wnd;
	if (hParent != NULL){ 
	
		DWORD dwStyles = ::GetWindowLong(hParent,GWL_EXSTYLE);
		dwStyles&=~WS_EX_CLIENTEDGE;	
		dwStyles&=~WS_BORDER;
		dwStyles&=~WS_OVERLAPPED;
		//::SetClassLong(hParent,GCL_STYLE,dwStyles);
		SetWindowLong(hParent, GWL_EXSTYLE, dwStyles);
	}
	hParent = NULL;
}

void KillBorders(){ 
	// надо бы убить все бордюрчики 
	// на этих окнах, а то сильно раздражает...
	if (CMyView::m_pView == NULL){ 
		return;
	}
	HWND hWndKB = CMyView::m_pView->m_hWnd;
	int iIter = 1;
	while (hWndKB != NULL && iIter<4)	{
		hWndKB = ::GetParent(hWndKB);
		iIter++;
		if (iIter == 3){ 
			MyKillStyle(hWndKB);
		}
	}

	if (CSubsView::m_pView != NULL){ 
		hWndKB = CSubsView::m_pView->m_hWnd;
		MyKillStyle(hWndKB);
	}
	hWndKB = NULL;

}


/////////////////////////////////////////////////////////////////////////////
// CMyView message handlers


int CMyView::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CColumnTreeView::OnCreate(lpCreateStruct) == -1)
		return -1;
	pArrChancheParent[0] = NULL;

	// TODO: Add your specialized creation code here
	return 0;
}



void CMyView::OnInitialUpdate() 
{
	CColumnTreeView::OnInitialUpdate();
	CTreeCtrl& tree = GetTreeCtrl();
	DWORD treeStyle = GetWindowLong(tree.m_hWnd, GWL_STYLE);
	treeStyle |= TVS_SHOWSELALWAYS|TVS_NOTOOLTIPS;
	SetWindowLong(tree.m_hWnd,GWL_STYLE,treeStyle);
		
	pMapSSDocOsn.RemoveAll();

	CHeaderCtrl& header = GetHeaderCtrl();
	if (header.GetItemCount()==0){ 
		HDITEM hditem;
		hditem.mask = HDI_TEXT | HDI_WIDTH | HDI_FORMAT;
		hditem.fmt = HDF_LEFT | HDF_STRING;
		hditem.cxy = 200;
		hditem.pszText = "Идентификатор";
		header.InsertItem(0, &hditem);
		
		hditem.cxy = 100;
		hditem.pszText = "Тип";
		header.InsertItem(1, &hditem);
		hditem.cxy = 100;
		hditem.pszText = "Комментарий";
		header.InsertItem(2, &hditem);
		
	}

	UpdateColumns();

	m_root_doc = m_root_jrn = h_item_plan = m_root_vid_rasch = NULL;
	tree.DeleteAllItems();

	DWORD dwStyle = GetWindowLong(tree, GWL_STYLE);
	dwStyle |= TVS_HASBUTTONS | TVS_HASLINES | TVS_LINESATROOT;
	SetWindowLong(tree, GWL_STYLE, dwStyle);

	pMD=GetMetaData();
	if (pAdo->m_StateSubs == 1){ 
		pAdo->BuildMarkUpSubs();
	}
	CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetTaskDef();
	Insert(pObj,TVI_ROOT);
	m_root = tree.GetRootItem();
	m_root = tree.GetChildItem(m_root);

	CString sDocOsnName,sDocOsnNames, nText = "";
	m_root_doc = NULL;
	
	while (m_root != NULL){
		tree.SortChildren(m_root);
		nText = tree.GetItemText(m_root);
		if (CString("Документ").CompareNoCase(nText) == 0){ 
			m_root_doc = m_root; // добавим к документам основания....
		}
		if (tree.ItemHasChildren(m_root)){ 
			m_root = tree.GetNextSiblingItem(m_root);
		} else {
			i_ch = m_root;
			m_root = tree.GetNextSiblingItem(m_root);
			tree.DeleteItem(i_ch);
		}
	}
	int iPos = 0;
	m_root = tree.GetRootItem();
	tree.Expand(m_root, TVE_EXPAND);
	if (m_root_doc != NULL){ 
		m_root_doc = tree.GetChildItem(m_root_doc);
		while(m_root_doc != NULL){
			nText = tree.GetItemText(m_root_doc);
			iPos = nText.Find("\t");
			if (iPos != -1){ 
				nText = nText.Left(iPos);
			}
			sDocOsnNames = "";
			if (pMapSSDocOsn.Lookup(nText,sDocOsnNames)){ 
				HTREEITEM m_root_doc_sam = tree.InsertItem("Вводим на основании",0,0,m_root_doc);
				while(!sDocOsnNames.IsEmpty()){
					iPos = sDocOsnNames.Find(",");
					if (iPos!=-1){ 
						sDocOsnName = sDocOsnNames.Left(iPos);
						sDocOsnNames = sDocOsnNames.Mid(iPos+1);
					} else {
						sDocOsnName = sDocOsnNames;
						sDocOsnNames = "";
					}
					tree.InsertItem(sDocOsnName,0,0,m_root_doc_sam);
				}
			}

			m_root_doc = tree.GetNextSiblingItem(m_root_doc);
		}
	}
	
	KillBorders(); // убъемся 

	/*
	m_ImgList.Create(IDB_IMAGES, 16, 1, RGB(255,0,255));
	tree.SetImageList(&m_ImgList, TVSIL_NORMAL);
	*/

	// TODO: Add your specialized code here and/or call the base class

}

//	Извлекаем типы и синоним
//	надо использовать CMetaDataTypedObj
// int nPropType = 0 - н
CString GetPropsSpec(class CMetaDataObj* nObj, int nPropType = 0) {
	CString nPrp[4],nRet = "";
	nPrp[0] = "";
	nPrp[1] = "";
	nPrp[2] = "";
	nPrp[3] = "";

	CValue val;
	CMetaDataObj* pNew=NULL;
	
	const char* nPropName = NULL;
	int nNProps = nObj->GetNProps();
	
	if (nPropType == 0){ //  нужен тип и вид
		int WhatIsIt = nObj->WhatIsIt();
		switch(WhatIsIt)
		{
		case 1: // константа
		case 2: // атрибут справочника, графа отбора журнала расчетов
		case 4: // измерение регистра
		case 5: // ресурс регистра
		case 7: // реквизит шапки документа
		case 8: // реквизит ТЧ документа
		case 21: // реквизит журнала расчетов
		case 25: // ресурс регистра
		case 26: // ОРД
		case 28: // реквизит счета
		case 32: // реквизит проводки.....
		case 35: // нумератор, попробуем
		case 34: // графа отбора.
		case 36: // вид субконто
			{
				// 34 - графа отбора. тип 5 вид-6 ????? длина,точность  нет...
				// 10 - графа журнала. У нее можно брать 1-й реф и выводить его тип...
				int iPropLoc1 = 3; // тип
				int iPropLoc2 = 4; // вид
				int iPropLoc3 = 5; // длина
				int iPropLoc4 = 6; // точность
				if (WhatIsIt == 34){ 
					iPropLoc1 = 5; // тип
					iPropLoc2 = 6; // вид
					iPropLoc3 = 0;
					iPropLoc4 = 0;
				}
				nObj->GetPropVal(iPropLoc1,pMD,val,&pNew);// тип
				nPrp[0]=val.GetString();
				
				pNew=NULL;
				nObj->GetPropVal(iPropLoc2,pMD,val,&pNew);// вид
				nPrp[1]=val.GetString();

				pNew=NULL;
				nObj->GetPropVal(iPropLoc3,pMD,val,&pNew);// длина
				nPrp[2]=val.GetString();
				if (val.GetNumeric()!=0){ 
					pNew=NULL;
					nObj->GetPropVal(iPropLoc4,pMD,val,&pNew);// точность
					nPrp[3]=val.GetString();

					nRet.Format("%s %s,%s", nPrp[0], nPrp[2], nPrp[3]);
				} else {
					nRet.Format("%s.%s", nPrp[0], nPrp[1]);
				}
			}
			break;
		default:
		    break;
		}
	} else if (nPropType == 1){ //  комментарий
		nRet = nObj->GetDescr();
		if (nRet.IsEmpty()){ // нет комментария, выводим синоним...
			nObj->GetPropVal(1,pMD,val,&pNew);// тип
			nRet = val.GetString();
		}
	} else if (nPropType == -3/* || nPropType == -4*/){ //  вдаделец справочника/журнал документа/тип регистра/

		nObj->GetPropVal(3,pMD,val,&pNew);// тип
		nRet = val.GetString();
		if(pNew){
			long l;
			nRet = pMD->GetFullName(pNew->GetID(),&l,1);
		}		
	} else if (nPropType == 2){  // Специально для счета, возьмем 0 свойство(код счета) и 1-е (наименование)
		nObj->GetPropVal(0,pMD,val,&pNew);// тип
		nPrp[0]=val.GetString();
		
		pNew=NULL;
		nObj->GetPropVal(1,pMD,val,&pNew);// вид
		nPrp[1]=val.GetString();
		nRet.Format("%s\t%s", nPrp[0], nPrp[1]);
	}else if (nPropType == 3){  
		// для справочника, получим тип кода и длинну.
		//"Код", если длина кода >0 а тип его будет Длинакода/Тип/СерииКодов
		nObj->GetPropVal(5,pMD,val,&pNew);// длина кода
		if (val.GetNumeric()>0){ 
			nPrp[0]=val.GetString();

			pNew=NULL;
			nObj->GetPropVal(8,pMD,val,&pNew);// тип кода
			nPrp[1]=val.GetString();

			pNew=NULL;
			nObj->GetPropVal(7,pMD,val,&pNew);// серии кодов
			nPrp[2]=val.GetString();
			nRet.Format("Код\t%s,%s,%s", nPrp[0], nPrp[1], nPrp[2]);
		} 
	}else if (nPropType == 4){  
		// для справочника, получим наименование и длинну его
		//"Код", если длина кода >0 а тип его будет Длинакода/Тип/СерииКодов
		nObj->GetPropVal(6,pMD,val,&pNew);// длина наименования
		if (val.GetNumeric()>0){ 
			nPrp[0]=val.GetString();
			nRet.Format("Наименование\t%s,Строка", nPrp[0], nPrp[1], nPrp[2]);
		} 
	}

	return nRet;
}


HTREEITEM CMyView::Insert(CMetaDataObj *pObj, HTREEITEM parent)
{
	if(!pObj)
		return NULL;
	CTreeCtrl& ctrl=GetTreeCtrl();
	CString strID, name=pObj->GetCode();
	
	if (pAdo->m_Settings.Show_ID == 1) {
		strID.Format(" (%d)", pObj->GetID());
	}
	

	name += strID;
	CString sPropAll, sPropKomm, sPropType=  GetPropsSpec(pObj);
	sPropKomm = pObj->GetDescr();
	if (sPropKomm.IsEmpty()){ // Нет комментария, получим синоним
		sPropKomm = pObj->GetRealPresent();
	}
	bool bObjIsTru = true; // gправильный объект....
	
	int WhatIsIt = pObj->WhatIsIt();
	if (pAdo->m_StateSubs == 1){ //фильтрация.....
		if (pAdo->IsStoredObj(WhatIsIt)){ 
			// А вот тут засада с документами, если мы уберем документы, которые 
			// не попадают в фильтр, соответственно их не будет в ветках документов
			// "Вводим на основании" и "Является основанием для:"
			// что не есть хотрошо...
			bObjIsTru = false;
			CString sSearchID = "", sSearchSub = "";
			sSearchID.Format("%d", pObj->GetID());
			if (pAdo->m_ObjToSub.Lookup(sSearchID,sSearchSub)){ 
				if (pAdo->LineHaveMarkUpSubs (sSearchSub)){ 
					bObjIsTru = true;
				}
			}
		}
	}
	if(!bObjIsTru)
		return NULL;

	switch(WhatIsIt)	{
	case 3:	// справочник. получим владельца....
	case 6:	// регистр, тип
	case 9:	// документ. получим журнал
		{
			sPropType = GetPropsSpec(pObj,-3);
			sPropAll.Format("%s\t%s",name,sPropType,sPropKomm);
		}
		break;
	case 30:	
		{
			name = GetPropsSpec(pObj,2);
			sPropAll.Format("%s\t%s",name,sPropType,sPropKomm);
		}
		break;
	default:
		{
			sPropAll.Format("%s\t%s\t%s",name,sPropType,sPropKomm);
		}
	    break;
	}
	if (WhatIsIt == 30){  // счета...
		name = GetPropsSpec(pObj,2);
		sPropAll.Format("%s\t%s",name,sPropType,sPropKomm);
	} else {
		sPropAll.Format("%s\t%s\t%s",name,sPropType,sPropKomm);
	}


	HTREEITEM item=ctrl.InsertItem(/*name*/sPropAll,1,1,parent);
	ctrl.SetItemData(item,(DWORD)pObj);

	if (WhatIsIt == 36 ){  // вид субконто, нет нормальных пропсов
		return NULL;
	}
	// добавим некоторые свойства к объекту....
	// Например, к справочнику код и его длину
	InsertAdd(pObj, item);

	int nChilds=pObj->GetNChilds();
	for(int i=0;i<nChilds;i++)
	{
		name=pObj->GetChildName(i,1);
		HTREEITEM hChild;
		// Попробуем сменить папашу у некоторых итемов:
		// 
		// Документ<< ОбщийРеквизитДокумента,Нумератор,Последовательность, 
		// Журнал << ГрафаОтбора    
		// ПланСчетов <<  РеквизитСчета  ФормаСпискаСчетов ФормаЖурналаОпераций 
		// ПланСчетов <<  РеквизитПроводки  ФормаЖурналаПроводок  
		// ПредставлениеПроводки - это вообще не выводить   
		// ВидРасчета << ПравилоПерерасчета
		if (WhatIsIt == 17){ // таск деф, тут помудрим немного...
			CString sName_2 = "";
			// m_root_doc = m_root_jrn = h_item_plan = m_root_vid_rasch
			if (CString("Документ").CompareNoCase(name) == 0){ 
				hChild = ctrl.InsertItem(name,0,0,item);	
				m_root_doc = hChild;
			} else if (
				(CString("ОбщийРеквизитДокумента").CompareNoCase(name) == 0) || 
				(CString("Нумератор").CompareNoCase(name) == 0) ||
				(CString("Последовательность").CompareNoCase(name) == 0)
				)
			{ 
				sName_2 = "! " + name; // т.к. сортируется
				hChild = ctrl.InsertItem(sName_2,0,0,m_root_doc);	
			} else if (CString("Журнал").CompareNoCase(name) == 0){ 
				hChild = ctrl.InsertItem(name,0,0,item);	
				m_root_jrn = hChild;
			} else if (CString("ГрафаОтбора").CompareNoCase(name) == 0){ 
				hChild = ctrl.InsertItem(name,0,0,m_root_jrn);	
			} else if (CString("ПланСчетов").CompareNoCase(name) == 0){ 
				hChild = ctrl.InsertItem(name,0,0,item);	
				h_item_plan = hChild;
			} else if (
				(CString("РеквизитСчета").CompareNoCase(name) == 0) ||
				(CString("ФормаСпискаСчетов").CompareNoCase(name) == 0) ||
				(CString("ФормаЖурналаОпераций").CompareNoCase(name) == 0) ||
				(CString("РеквизитПроводки").CompareNoCase(name) == 0) ||
				(CString("РеквизитПроводки").CompareNoCase(name) == 0) ||
				(CString("ФормаЖурналаПроводок").CompareNoCase(name) == 0) ||
				(CString("ПредставлениеПроводки").CompareNoCase(name) == 0)
				){ 
				hChild = ctrl.InsertItem(name,0,0,h_item_plan);	
			} else if (CString("ВидРасчета").CompareNoCase(name) == 0){ 
				hChild = ctrl.InsertItem(name,0,0,item);	
				m_root_vid_rasch = hChild;
			} else if (CString("ПравилоПерерасчета").CompareNoCase(name) == 0){ 
				sName_2 = "! " + name; // т.к. сортируется
				hChild = ctrl.InsertItem(sName_2,0,0,m_root_vid_rasch);	
			} 
			else 
			{
				hChild = ctrl.InsertItem(name,0,0,item);	
			}

		} else { 
			// а тут вообще мудрить не будем.....
			hChild = ctrl.InsertItem(name,0,0,item);
		}


		
		CMetaDataObjArray* pArr=pObj->GetChild(i,pMD);
		if(!pArr)
			return NULL;
		int nObj=pArr->GetNItems();
		for(int j=0;j<nObj;j++)
		{
			CMetaDataObj* pObj1=pArr->GetAt(j);
			Insert(pObj1,hChild);
		}
	}
	CValue val;
	CMetaDataObj* pNew=NULL;

	switch(WhatIsIt)
	{
	case 9:	// документ. перечислим вводимые на основании
		m_root = item;
		// 18 - ввод на основании
		pObj->GetPropVal(18,pMD,val,&pNew);
		if(pNew)
		{
			if(pNew->IsRefObj())
			{
				CString sDocName =pObj->GetCode(),sDocOsn = "";
				HTREEITEM hAddBrancheOsn = NULL;
				name = "";
				CMetaDataRefObj* pRef=(CMetaDataRefObj*)pNew;
				for(int z=0;z<pRef->GetNRefDefs();z++)
				{
					if (hAddBrancheOsn == NULL){ 
						hAddBrancheOsn = ctrl.InsertItem("Является основанием для:",0,0,item);
					}
					name = pRef->GetRefDefAt(z)->GetString();
					name = name.Mid(9);
					ctrl.InsertItem(name,0,0,hAddBrancheOsn);
					// Надо найти документ из name и добавить в него в ветку "Вводим на основании"
					// name тут Документ.ТакойТо
					if (pMapSSDocOsn.Lookup(name,sDocOsn)){ 
						sDocOsn = sDocOsn+","+sDocName;
					} else {
						sDocOsn = sDocName;
					}
					pMapSSDocOsn.SetAt(name,sDocOsn);
				}
			}
		}
		break;
	default:
	    break;
	}
	return item;

}

HTREEITEM CMyView::InsertAdd(CMetaDataObj *pObj, HTREEITEM parent)
{
	HTREEITEM hRetItem = NULL;
	int nWhatIsIt = pObj->WhatIsIt();
	CString nStrToAdd = "";
	CTreeCtrl& ctrl=GetTreeCtrl();

	switch(nWhatIsIt)
	{
	case 3: 
		// Справочник. Добавим: "Код", если длина кода >0 а тип его будет Длинакода/Тип/СерииКодов
		{
			// Запрашиваем код и его данные...
			nStrToAdd = GetPropsSpec(pObj, 3);
			if (!nStrToAdd.IsEmpty()){ 
				ctrl.InsertItem(nStrToAdd,0,0,parent);
			} 
			// Запрашиваем наименование и его данные...
			nStrToAdd = GetPropsSpec(pObj, 4);
			if (!nStrToAdd.IsEmpty()){ 
				ctrl.InsertItem(nStrToAdd,0,0,parent);
			} 
			
		}
		break;
	case 9:	// документ. добавим форму и модуль...
		{

			ctrl.InsertItem(ch_doc_form,0,0,parent);
			ctrl.InsertItem(ch_doc_mtrans,0,0,parent);

		}
		break;
	default:
	    break;
	}

	return hRetItem;
}




LRESULT CMyView::DefWindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (message == WM_NOTIFY ){ 
		if (((LPNMHDR)lParam)->code == TVN_SELCHANGED ){ 
			if (CSubsView::m_pView != NULL){ 
				CSubsView::m_pView->OnSelectMetaData();
				//CSubsView::m_pView->SendNotifyMessage(message, wParam, lParam);
				//return true;
			}
		}
	} 
	
	return CColumnTreeView::DefWindowProc(message, wParam, lParam);
}

BOOL CMyView::PreTranslateMessage(MSG* pMsg) 
{
	// TODO: Add your specialized code here and/or call the base class
	if (pMsg->message == WM_KEYDOWN){ 
		if (pMsg->wParam == VK_F6 || pMsg->wParam == VK_TAB){ 
			if (CSubsView::m_pView != NULL){ 
				CListBox&	m_ListCtrl = CSubsView::m_pView->m_ListCtrl;
				::SetFocus(m_ListCtrl.m_hWnd);
				
			}
		} else if (pMsg->wParam == VK_RETURN){ 
			OnOpenMetaObj();
		} else if (pMsg->wParam == VK_ESCAPE){ 
			pAdo->ActivateStandartView();

		}	
	} if (pMsg->message == WM_LBUTTONDBLCLK){ 
		// тут надо посмотреть на дереве ли мы сликаем или на хейдере,
		// если на хейдере, тогда сортанем итемы.
		// только клик нужен одиночный......
		OnOpenMetaObj();
	} 
	
	return CColumnTreeView::PreTranslateMessage(pMsg);
}


void CMyView::OnOpenMetaObj()
{
	CTreeCtrl& ct=GetTreeCtrl();
	HTREEITEM item=ct.GetSelectedItem();
	CMetaDataObj* pObj=(CMetaDataObj*)ct.GetItemData(item);
	int nAction = 0;
	CString sItemText = "";
	sItemText = ct.GetItemText(item);
	int wis;

	if (pObj == NULL){ 
		item = ct.GetParentItem(item);
		pObj=(CMetaDataObj*)ct.GetItemData(item);
		if (pObj!=NULL){ 
			try
			{
				wis=pObj->WhatIsIt();
			}
			catch (...)
			{
				return;			
			}
			if (wis == 9){ 
				if (sItemText.CompareNoCase(ch_doc_form) == 0){ 
					nAction = 1; // откроем форму
				} else if (sItemText.CompareNoCase(ch_doc_mtrans) == 0){ 
					nAction = 2; // откроем форму
				}
			} else {
				return;
			}

		} else 
			return;


	}
	if(pObj)
	{
		
		try
		{
			wis=pObj->WhatIsIt();
		}
		catch (...)
		{
			return;			
		}
		
		CBkEndUI* pUI=GetBkEndUI();
		
		//pUI->DoMessageLine(pObj->GetCode(),mmNone);

		long l,id=pObj->GetID();
		CString sMetaName, txt=pMD->GetFullName(id,&l,1);
		
		//pUI->DoMessageLine(txt,mmNone);
		BOOL IsTyped=pObj->IsTypedObj();
		CTypedCont* pCont = NULL;
		CModuleCont* pMCont=NULL;

		CWnd* phWnd = AfxGetMainWnd();
		HWND nhWndMDTree = NULL;
		HWND nhWndMain = AfxGetMainWnd()->m_hWnd;
		HWND nhMDIWnd = NULL;
		nhMDIWnd = FindChildWindow(nhWndMain,"MDIClient","");
		if (hFindWindow == NULL){ 
			nhWndMDTree = NULL;
			nhWndMain = NULL;
			nhMDIWnd = NULL;
			return;					
		}
		HWND nhWndMain2, hWmdMF_MD, hWmdMF_Tab, hWmdMF_Tree; 
		// hWmdMF_MD - 1 первой дочернее окно конфигурации 
		// hWmdMF_Tab - закладки
		// hWmdMF_Tree - само дерево метаданных


		switch(wis)
		{
		case 17:
		case 3:
		case 15:
		case 23:
		case 16:
		case 9:
			{
				// открываем окно метаданых
				nhWndMain = FindChildWindow(nhMDIWnd,"","Конфигурация");
				nhWndMain2 = FindChildWindow(nhMDIWnd,"","Конфигурация *");
				if ((nhWndMain == NULL && nhWndMain2 == NULL)){ 
					phWnd->SendMessage(WM_COMMAND,(WPARAM)33188);
				}
			
			}
			break;
		default:
		    break;
		}

		//POSITION pos;

		switch(wis)
		{
		case 17:// Метаданные 
			{
			}
			break;
		case 3:// справочник
			{
				CConfigCont::IDToPath(pObj->GetID(),"Subconto",txt,&pCont,0);
			}
			break;
		case 15:// справочник, форма списка
			{
				CConfigCont::IDToPath(pObj->GetID(),"SubList",txt,&pCont,0);
			}
			break;
			
		case 9:// документ
			{
				if (nAction == 0){ 
					sMetaName.Format("%s",pObj->GetCode());
					OpenMetaObjSpec("Документ",sMetaName, nAction);
				} else {
					if (nAction == 1){  // форма
						CConfigCont::IDToPath(pObj->GetID(),"Document",txt,&pCont,0);
					} else if (nAction == 2){ 
						CConfigCont::IDToTextModule(pObj->GetID(), "Transact", txt, &pMCont,FALSE);
						if (pMCont)
							pMCont->ShowDocument("Transact");
					}

				}
			}	

			break;
		case 23:// обработка
			{
				CConfigCont::IDToPath(pObj->GetID(),"CalcVar",txt,&pCont,0);
			}
			break;
		case 16:// отчет...
			{
				CConfigCont::IDToPath(pObj->GetID(),"Report",txt,&pCont,0);
			}
			break;
		default:
			{
			}
		    break;
		}		
		nhWndMain2 = NULL;
		hWmdMF_MD = NULL;
		hWmdMF_Tab = NULL;
		hWmdMF_Tree = NULL;		
		nhWndMDTree = NULL;
		nhWndMain = NULL;
		nhMDIWnd = NULL;

		if (pCont) 			
			pCont->ShowDocument(TRUE);

	} else {
		// не нашли объекта

	}
}



void CMyView::OpenMetaObjSpec (CString nMetaObjType, CString nMetaObjName, int nAction)
{
	CWnd* phWnd = AfxGetMainWnd();
	HWND nhWndMDTree = NULL;
	HWND nhWndMain = AfxGetMainWnd()->m_hWnd;
	HWND nhMDIWnd = NULL;
	nhMDIWnd = FindChildWindow(nhWndMain,"MDIClient","");
	if (hFindWindow == NULL){ 
		nhWndMDTree = NULL;
		nhWndMain = NULL;
		nhMDIWnd = NULL;
		return;					
	}
	bool bMetaDataWndNeedOpen = true;

	HWND nhWndMain2, hWmdMF_MD, hWmdMF_Tab, hWmdMF_Tree; 
	// hWmdMF_MD - 1 первой дочернее окно конфигурации 
	// hWmdMF_Tab - закладки
	// hWmdMF_Tree - само дерево метаданных

	nhWndMain = FindChildWindow(nhMDIWnd,"","Конфигурация");
	nhWndMain2 = FindChildWindow(nhMDIWnd,"","Конфигурация *");
	if ((nhWndMain == NULL && nhWndMain2 == NULL) && bMetaDataWndNeedOpen){ 
		phWnd->SendMessage(WM_COMMAND,(WPARAM)33188);
		nhWndMain = FindChildWindow(nhMDIWnd,"","Конфигурация");
		nhWndMain2 = FindChildWindow(nhMDIWnd,"","Конфигурация *");
	}
	if (nhWndMain != NULL){ 
		hWmdMF_MD = FindChildWindow(nhWndMain,"Afx:400000:8","");
	} else {
		nhWndMain = nhWndMain2;
		nhWndMain2 = NULL;
		hWmdMF_MD = FindChildWindow(nhWndMain,"Afx:400000:8","");
	}
	//FindWindowExA
	if (hWmdMF_MD != NULL){ 
		// искали подфрейм..., почему-то находится сразу окно с закладками...
		// надо вывести на передний план окно конфигурации в любом случае....
		phWnd->SendMessage(WM_COMMAND,(WPARAM)33188);
		::SendMessage(hWmdMF_MD,WM_LBUTTONDOWN,0,MAKELPARAM(10,10));

		hWmdMF_Tree = FindSiblingWindow(hWmdMF_MD,"SysTreeView32","",0);
		if (hWmdMF_Tree != NULL){ 
			CString str_ch_name = "";
			HTREEITEM hItem = (HTREEITEM) ::SendMessage(hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_ROOT,NULL); // рутовый элемент
			::SendMessage(hWmdMF_Tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
			::SendMessage(hWmdMF_Tree,TVM_EXPAND,TVE_EXPAND,(LPARAM)hItem); // expand рутовый элемент
			hItem = (HTREEITEM) ::SendMessage (hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hItem); // встали на константты

			if (hItem != NULL){ 
				// спуститься вниз от рута
				int nCtnDown = 0; 
				if (nMetaObjType.CompareNoCase("Документ") == 0){ 
					nCtnDown = 2;
				}
				CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetTaskDef();
				int nChilds=pObj->GetNChilds();
				int i, find_i = -1;
				for(i=0;i<nChilds;i++){
					// спускаемся по чилдам.. анализируем имя....
					str_ch_name=pObj->GetChildName(i,1);
					if (nMetaObjType.CompareNoCase(str_ch_name) == 0){ 
						find_i = i;
					}
					if (find_i != -1){ 
						// нашли корешок, и уже стоим на нем...
						::SendMessage(hWmdMF_Tree,TVM_EXPAND,TVE_EXPAND,(LPARAM)hItem);// развернем....
						hItem = (HTREEITEM) ::SendMessage (hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_CHILD,(LPARAM)hItem); 
						// встали на первый чилдовый...
						
						for (int y = 0; y<nCtnDown; y++) {
							// надо пропустить всякие там нумераторы и графы отбора....
							hItem = (HTREEITEM) ::SendMessage (hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)hItem); 
						}
						// теперь размотаем "настоящие" документы, справочники....
						CMetaDataObjArray* pArr=pObj->GetChild(i,pMD);
						if(pArr != NULL) {							
							int nObj=pArr->GetNItems();
							for(int j=0;j<nObj;j++)
							{
								CMetaDataObj* pObj1=pArr->GetAt(j);
								hItem = (HTREEITEM) ::SendMessage (hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)hItem); 
								if (nMetaObjName.CompareNoCase(pObj1->GetCode()) == 0){ 
									::SendMessage(hWmdMF_Tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
									//::SendMessage(hWmdMF_Tree, TVM_SELECTITEM, TVGN_CARET, (LPARAM)hItem);
									phWnd->SendMessage(WM_COMMAND,(WPARAM)33711);//Редактировать элемент\nРедактировать
									
								}

							}
						}

						
						break; // Это в самом конце должно быть......
					}

					hItem = (HTREEITEM) ::SendMessage (hWmdMF_Tree,TVM_GETNEXTITEM,TVGN_NEXT,(LPARAM)hItem); // идем дальше, ведь еще не нашлии..
				}



			}
		}

	}

	nhWndMain2 = NULL;
	hWmdMF_MD = NULL;
	hWmdMF_Tab = NULL;
	hWmdMF_Tree = NULL;		
	nhWndMDTree = NULL;
	nhWndMain = NULL;
	nhMDIWnd = NULL;
}



