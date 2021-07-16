// MetadataTree.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "MetadataTree.h"
#include "MainFrm.h"
#include "MetaTreeCtrl.h"
#include "ModuleDoc.h"
#include "Resource.h"
#include "FindTextDialog.h"
#include "OpenForm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif
#include "MetedataDocument.h"
//#include "FormObject.h"
//#include "FormObjectDoc.h"
#include "Metadata.h"
#include "ChildFrm.h"
#include "help_view/MyHelpDoc.h"

CString csBaseType="Число,Строка,Дата,СписокЗначений,ДеревоЗначений,Grid";
extern int nTypeValue;
extern int nTypeObject;
extern char *aChooseList[];
extern CMessageBar *pMessageWindow;

extern CValue ValCopyPaste;//буфер обмена

#define FILE_LIST_NAME csUserPath+"\\module_list.lst"
extern CString csUserPath;//каталог пользователя для записи настроек
void ValueToFile(CValue *Val,CString csFileName);
CValue ValueFromFile(CString csFileName);
extern BOOL bViewLibObjects;//показывать в дереве метаданных объекты из системных библиотек


#define MENU_START_ID2		(MENU_FINISH_ID+1)
#define MENU_FINISH_ID2		(MENU_START_ID2+100)


#define SETITEMTYPE(hItem,xbEdit,xbGroup,xnType,xnChildImage)\
{\
CObjectData data;\
data.bEdit=xbEdit;\
data.bGroup=xbGroup;\
data.nType=xnType;\
data.nChildImage=xnChildImage;\
dataMetaObj[hItem]=data;\
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

//Поддержка операций копирования
CCopyElement cCopy;

//Поддержка Drag&Drop
CObjectData cDataForDrag;
HTREEITEM treeDragItem=0;
HCURSOR	  hDragCursor;
HCURSOR	  hDragCursorMove;

#define m_pMetadata	m_RunModuleManager.pMetadata

/////////////////////////////////////////////////////////////////////////////
// CMetadataTree

IMPLEMENT_DYNCREATE(CMetadataTree, CTreeView)
CMetadataTree* afx_MetadataTree=0;
CMetadataTree* AfxGetMetadataTree()
{
	return afx_MetadataTree;
};

CMetadataTree::CMetadataTree()
{
	hDragCursor=AfxGetApp()->LoadCursor (IDC_POINTER_DRAG2);
	hDragCursorMove=AfxGetApp()->LoadCursor (IDC_POINTER_DRAG3);
	treeDragItem=0;
	bIsClose=0;
	afx_MetadataTree=this;

	treeMODULES=0;
	treeFORMS=0;
	treeMAKETS=0;
	treeOBJECTS=0;
	TreeMetadata=0;
	TreeGlobalModule=0;
	hTrash=0;
	hTrashDelete=0;
	hDocuments=0;
}

CMetadataTree::~CMetadataTree()
{
	afx_MetadataTree=0;
	AfxGetProperty()->ReleaseDoc(GetDocument());
	bIsClose=1;

	if(AfxGetModuleManager())
		AfxGetModuleManager()->pTree=0;

}

BEGIN_MESSAGE_MAP(CMetadataTree, CTreeView)
	//{{AFX_MSG_MAP(CMetadataTree)
	ON_COMMAND(IDR_NEW, OnNew)
	ON_WM_LBUTTONDBLCLK()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_NOTIFY_REFLECT(NM_RCLICK, OnRclick)
	ON_COMMAND(IDR_EDIT, OnEdit)
	ON_NOTIFY_REFLECT(TVN_KEYDOWN, OnKeydown)
	ON_COMMAND(IDR_REMOVE, OnRemove)
	ON_COMMAND(ID_PROPERTY, OnProperty)
	ON_WM_LBUTTONDOWN()
	ON_WM_KEYUP()
	ON_WM_DESTROY()
	ON_COMMAND(ID_EDIT_CUT, OnEditCut)
	ON_COMMAND(ID_EDIT_COPY, OnEditCopy)
	ON_COMMAND(ID_EDIT_PASTE, OnEditPaste)
	ON_COMMAND(ID_INSERT_FORM, OnInsertForm)
	ON_COMMAND(ID_SAVEFORM, OnSaveform)
	ON_COMMAND(ID_REPLACEFORM, OnReplaceform)
	ON_UPDATE_COMMAND_UI(ID_EDIT_CUT, OnUpdateEditCut)
	ON_UPDATE_COMMAND_UI(ID_EDIT_COPY, OnUpdateEditCopy)
	ON_UPDATE_COMMAND_UI(ID_EDIT_PASTE, OnUpdateEditPaste)
	ON_NOTIFY_REFLECT(TVN_BEGINDRAG, OnBegindrag)
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_MOUSEMOVE()
	ON_WM_CONTEXTMENU()
	ON_COMMAND(ID_MODULE_CONFIG, OnModuleConfig)
	ON_COMMAND(ID_MODULE_RUN, OnModuleRun)
	ON_COMMAND(ID_OPENCONFIG, OnOpenconfig)
	ON_COMMAND(IDR_NEW_FOLDER, OnNewFolder)
	ON_COMMAND_RANGE(MENU_START_ID2, MENU_FINISH_ID2, OnSelectMenu)
	ON_WM_RBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMetadataTree drawing

void CMetadataTree::OnDraw(CDC* pDC)
{
	//CDocument* pDoc = GetDocument();
}

/////////////////////////////////////////////////////////////////////////////
// CMetadataTree diagnostics

#ifdef _DEBUG
void CMetadataTree::AssertValid() const
{
	CTreeView::AssertValid();
}

void CMetadataTree::Dump(CDumpContext& dc) const
{
	CTreeView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMetadataTree message handlers

CImageList *GetImageList();

BOOL CMetadataTree::Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID, CCreateContext* pContext) 
{
	CWnd::Create(lpszClassName, lpszWindowName, dwStyle, rect, pParentWnd, nID, pContext);
	CMetaTreeCtrl *pCtrl=(CMetaTreeCtrl *)&GetTreeCtrl();
	pCtrl->SetImageList(GetImageList(),TVSIL_NORMAL);

	DWORD dwStyle2; 
	dwStyle2 = WS_VISIBLE | WS_CHILD | TVS_HASLINES | TVS_LINESATROOT | 
			  TVS_HASBUTTONS;
	SetWindowLong(pCtrl->m_hWnd,GWL_STYLE ,dwStyle2);
	
	if(GetParentFrame())
	if(GetParentFrame()->GetParent())
	{
		CRect Rect;
		GetParentFrame()->GetParent()->GetClientRect(Rect);
		GetParentFrame()->MoveWindow(0,0,300,Rect.bottom);
	}


	InsertSystemElements();

	return TRUE;
}


void CMetadataTree::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	//CTreeCtrl *pCtrl=&GetTreeCtrl();
	//HTREEITEM CurItem=pCtrl->GetSelectedItem();
	//if(pCtrl->GetParentItem(CurItem))
	if(Edit())
		return;

	CTreeView::OnLButtonDblClk(nFlags, point);
}

void CMetadataTree::OnRButtonDown(UINT nFlags, CPoint point) 
{
	EndDrag();

	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->HitTest(point, 0);
	if(CurItem)
	{
		pCtrl->SetFocus();
		pCtrl->SelectItem(CurItem);
	}
	
	ShowProperty(0);

	//CTreeView::OnRButtonDown(nFlags, point);
}

void CMetadataTree::AddMenuInfo(CMenu* pPopup,HTREEITEM hParentItem,int &nPos)
{
	if(nPos>10)
		return;
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetChildItem(hParentItem);
	while(CurItem)
	{
		CObjectData data=dataMetaObj[CurItem];
		if(data.bGroup==0)
		if(data.nType==OBJ_FORM || data.nType==OBJ_MAKET || data.nType==OBJ_MODULE || data.nType==OBJ_DOC)
		{
			nPos++;
			aMenuItem.SetAtGrow(nPos,CurItem);
			CString csRes=CString("Открыть: \"")+pCtrl->GetItemText(CurItem)+"\"";
			int nID=MENU_START_ID2+nPos;
		    pPopup->InsertMenu(nPos, MF_BYPOSITION, nID, csRes);

			if(data.nType==OBJ_FORM)
				SetMBitmap(nID,IDB_EDIT_FORM)
			else
			if(data.nType==OBJ_MAKET)
				SetMBitmap(nID,IDB_EDIT_MAKET)
			else
			if(data.nType==OBJ_MODULE)
				SetMBitmap(nID,IDB_EDIT_MODULE)
			else
			if(data.nType==OBJ_DOC)
				SetMBitmap(nID,IDB_EDIT_DOC)

		}
		AddMenuInfo(pPopup,CurItem,nPos);
		CurItem=pCtrl->GetNextSiblingItem(CurItem);
	}
}
void CMetadataTree::GetFormArray(CStringArray &aList,HTREEITEM hParentItem)
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	if(hParentItem==0)
	{
		HTREEITEM CurItem=pCtrl->GetRootItem();
		while(CurItem)
		{
			GetFormArray(aList,CurItem);
			CurItem=pCtrl->GetNextSiblingItem(CurItem);
		}
		return;
	}
	HTREEITEM CurItem=pCtrl->GetChildItem(hParentItem);
	while(CurItem)
	{
		CObjectData data=dataMetaObj[CurItem];
		if(data.bGroup==0)
		if(data.nType==OBJ_FORM)
		{
			if(aList.GetSize()>100)
				return;
			aList.Add(GetPath(CurItem));
		}
		GetFormArray(aList,CurItem);
		CurItem=pCtrl->GetNextSiblingItem(CurItem);
	}
}
void CMetadataTree::OnSelectMenu(UINT nID)
{
	int nPos=nID-MENU_START_ID2;
	if(nPos>=0 && nPos<aMenuItem.GetSize())
	{
		HTREEITEM CurItem=aMenuItem[nPos];
		CTreeCtrl *pCtrl=&GetTreeCtrl();
		pCtrl->SelectItem(CurItem);
		Edit();
		//CObjectData data=dataMetaObj[CurItem];
	}
}

void CMetadataTree::OnRButtonUp(UINT nFlags, CPoint point) 
{
	EndDrag();

	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->HitTest(point, 0);

	if(CurItem)
	{
		CRect rect;
		GetClientRect(rect);
		ClientToScreen(rect);
		CPoint point2 = rect.TopLeft();
		point.Offset(point2);


		CMenu menu;
		VERIFY(menu.LoadMenu(IDR_METADATATREE));
		CMenu* pPopup;
		pPopup = menu.GetSubMenu(0);

		SetMBitmap(IDR_REMOVE,IDB_EDIT_CUT)
		SetMBitmap(IDR_NEW,IDB_EDIT_NEW)
		SetMBitmap(IDR_EDIT,IDB_EDIT)
		


		CTreeCtrl *pCtrl=&GetTreeCtrl();
		HTREEITEM CurItem=pCtrl->GetSelectedItem();
		CObjectData data=dataMetaObj[CurItem];


		//проверяем ввод нового элемента
		HTREEITEM hParent=GetParentForNew();
		CObjectData data2=dataMetaObj[hParent];
		if( data2.bGroup)
		{
			pPopup->EnableMenuItem(IDR_NEW,MF_BYCOMMAND|MF_ENABLED );
		}
		

		//проверяем на доступность редактирования
		//проверяем на доступность удаления
		//проверяем на наличие окна свойств
		if(data.bEdit)
		{
			pPopup->EnableMenuItem(IDR_EDIT,MF_BYCOMMAND|MF_ENABLED );
			pPopup->EnableMenuItem(IDR_REMOVE,MF_BYCOMMAND|MF_ENABLED);
		}
		if(data.bEdit||data.nType==OBJ_RUN)
		{
			pPopup->EnableMenuItem(ID_PROPERTY,MF_BYCOMMAND|MF_ENABLED);
			SetMenuDefaultItem(pPopup->m_hMenu,ID_PROPERTY,MF_BYCOMMAND);
		}


		//сохранение и загрузка форм
		if(data.nType==OBJ_FORM&&data.bGroup==0)
		{
			pPopup->EnableMenuItem(ID_SAVEFORM,MF_BYCOMMAND|MF_ENABLED );
			pPopup->EnableMenuItem(ID_REPLACEFORM,MF_BYCOMMAND|MF_ENABLED );
		}
		if(data.bEdit)
		{
			if(data.nType==OBJ_FORM)
				pPopup->EnableMenuItem(ID_INSERT_FORM,MF_BYCOMMAND|MF_ENABLED );
		}

		//ищем модули и формы, которые можно открыть на редактирование
		int nPos=4;
		aMenuItem.RemoveAll();
		AddMenuInfo(pPopup,CurItem,nPos);
		if(nPos>4)
			pPopup->InsertMenu(nPos+1, MF_BYPOSITION | MF_SEPARATOR, 0, "");

		

		CWnd* pWndPopupOwner = this;//GetParent();
		pPopup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y,
			pWndPopupOwner);

	}
	
	
	CTreeView::OnRButtonUp(nFlags, point);
}


void CMetadataTree::OnRclick(NMHDR* pNMHDR, LRESULT* pResult) 
{
	CPoint point;
	GetCursorPos(&point);

	CRect rect;
	GetClientRect(rect);
	ClientToScreen(rect);
	CPoint point2 = rect.TopLeft();
	point.x-=rect.left;
	point.y-=rect.top;

	OnRButtonUp(0,point);
	
	*pResult = 0;
}

CMetaObject *CMetadataTree::GetMetaObject(HTREEITEM CurItem) 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	if(!CurItem)
		CurItem=pCtrl->GetSelectedItem();
	if(!CurItem)
		return 0;
	CMetaObject *pObj=buferMetaObj[CurItem];//смотрим может мы уже открывали элемент для данного дерева (если да, то возможно изменяли наименование)
	CString csObjName=pCtrl->GetItemText(CurItem);
	if(!pObj)
	{
		CString csFileName=GetPath(CurItem);

		pObj=m_pMetadata->GetMetaObject(csFileName,csObjName);
		if(!pObj)
			return 0;

		CObjectData *pData=&dataMetaObj[CurItem];
		pObj->csName=csObjName;
		pObj->nTypeObject=pData->nType;
		if(IsType(OBJ_ATTR,CurItem))
		{
			CValue Val;
			Val.LoadFromString(pObj->csAttr);
			pData->LoadData(Val);
		}


		pObj->nTempID=CurItem;
		pObj->pTree=this;


		buferMetaObj[CurItem]=pObj;//запоминаем чтобы следующий раз не искать по имени


		//код для сохранения данных по типу объекта внутри метаданных (при записи)
		//в т.ч. для корректной работы копирования
		{
			CObjectData *pData=&dataMetaObj[CurItem];
			int nImage;
			pCtrl->GetItemImage(CurItem,nImage,nImage);

			pObj->vExtVal.SetAt("ChildImage",pData->nChildImage);
			pObj->vExtVal.SetAt("Edit",pData->bEdit);
			pObj->vExtVal.SetAt("Group",pData->bGroup);
			pObj->vExtVal.SetAt("Type",pData->nType);
			pObj->vExtVal.SetAt("Image",nImage);
		}

		//делаем цикл по подчиненным элементам для того чтобы забрать объект полностью (из файла в контейнер объектов) - например актуально для переименовывания!
		if(pCtrl->GetParentItem(CurItem))//если это не корневые элементы
		{
			CurItem=pCtrl->GetChildItem(CurItem);
			while(CurItem)
			{
				GetMetaObject(CurItem);
				CurItem=pCtrl->GetNextSiblingItem(CurItem);
			}
		}
	}
	return pObj;
}

void CMetadataTree::RemoveFromList(CMetaObject *pObj,CString csName) 
{
	if(!bIsClose)
	if(pObj)
	{
		CDocList* pDocList=metaDocList[pObj->nTempID];
		if(pDocList)
		{
			pDocList->aList[csName]=0;
		}
	}
}

//открытие формы
CDocument *CMetadataTree::OpenForm(CMetaObject *pObj,CString csClassName,CString csName) 
{
	if(!pObj) 
		pObj=GetMetaObject();
	if(pObj)
	{
		CDocList* pDocList=metaDocList[pObj->nTempID];
		if(pDocList)
		{//находим нужный объект
			CDocument* pa=(CDocument*)pDocList->aList[csName];
			if(pa)//нашли
			{
				ActivateObj(pa);
				return pa;
			}
		}
		else
		{
			pDocList=new CDocList();
			metaDocList[pObj->nTempID]=pDocList;
		}

		CDocument* pDoc=OpenFormMDI(csClassName,1,this,pObj);

		if(!pDoc)
			return 0;

		GetObjInfo(pDoc)->SetObjName(csName);

		pDocList->aList[csName]=pDoc;//запоминаем объект в списке
		return pDoc;
	}
	return 0;
}
CString CMetadataTree::GetPath(HTREEITEM CurItem)
{
	CString csRes;
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	while(CurItem)
	{
		csRes=pCtrl->GetItemText(CurItem)+"\\"+csRes;
		CurItem=pCtrl->GetParentItem(CurItem);
		if(CurItem==TreeReadOnly)
			break;
	}
	csRes.TrimRight('\\');

	return csRes;
}

CString CMetadataTree::GetNewName(HTREEITEM hParent,CString csPrefix)
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	int count=0;
	if(csPrefix.IsEmpty())
		csPrefix="Новый";
	CString csName;
	csName=csPrefix;
	HTREEITEM hItem=pCtrl->GetChildItem(hParent);
	while(hItem)
	{
		if(csName==pCtrl->GetItemText(hItem))
		{
			count++;
			csName.Format("%s%d",csPrefix,count);
			//начинаем цикл заново
			hItem=pCtrl->GetChildItem(hParent);
			continue;
		}
		hItem=pCtrl->GetNextSiblingItem(hItem);
	}
	return csName;
}

BOOL CMetadataTree::IsTrash(HTREEITEM hItem)
{
	if(hItem==hTrash||hItem==hTrashDelete)
		return 1;
	else
		return 0;
}
HTREEITEM CMetadataTree::GetParentForNew() 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetSelectedItem();
	if(!CurItem)
		return 0;
	if(IsTrash(CurItem))
		return CurItem;
	if(IsTrash(pCtrl->GetParentItem(CurItem)))
		return pCtrl->GetParentItem(CurItem);

	CObjectData data=dataMetaObj[CurItem];
	if(data.bGroup)
		return CurItem;
	return pCtrl->GetParentItem(CurItem);
}
void CMetadataTree::OnNew() 
{
	NewItem(1);
}

void CMetadataTree::NewItem(int bShowItem,CString csNewName,HTREEITEM hParent,int nMode)
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();

	if(!hParent)
		hParent=GetParentForNew();
	CString csName=GetNewName(hParent,csNewName);

	HTREEITEM NewItem=0;

	
	AfxGetProperty()->ReleaseDoc(GetDocument());
	if(IsGroup(hParent))
	{
		if(nMode==0)
		{
			if(IsType(OBJ_CLASS,hParent,1))
			{
				NewItem=AddObjectItem(csName,hParent,1);
			}
			else
			{
				int nImage=dataMetaObj[hParent].nChildImage;
				NewItem=pCtrl->InsertItem(csName,nImage,nImage,hParent);
				dataMetaObj[NewItem]=dataMetaObj[hParent];
				dataMetaObj[NewItem].bGroup=0;
				dataMetaObj[NewItem].bEdit=1;

				if(nImage==ICON_MODULE)
				{
					CMetaObject *pObj=GetMetaObject(NewItem);
					if(pObj->csFile.IsEmpty())
						pObj->csFile=CModuleDoc::GetDefaultText(DEFAULT_OBJ_COMMON_MODULE);
				}
			}
		}
		else
		{
			int nImage=1;
			NewItem=pCtrl->InsertItem(csName,nImage,nImage,hParent);
			dataMetaObj[NewItem]=dataMetaObj[hParent];
			dataMetaObj[NewItem].bGroup=1;
			dataMetaObj[NewItem].bEdit=1;
		}
	}

	

	if(!NewItem)
		return;

	pCtrl->Expand(NewItem,TVE_EXPAND);
	pCtrl->Invalidate();
	pCtrl->SelectItem(NewItem);
	GetDocument()->SetModifiedFlag(1);


	if(bShowItem)
	{
		OnEdit();
		ShowProperty(1);
	}
}
void CMetadataTree::OnEdit() 
{
	Edit();
}
CDocument* CMetadataTree::Edit() 
{
	//смотрим что за объект в дереве
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetSelectedItem();
	if(IsType(OBJ_FORM,CurItem))
	{
		return OpenForm(0,"Форма",FORMDIALOGNAME);
	}
	else
	if(IsType(OBJ_MODULE,CurItem))
	{
		CString csName=pCtrl->GetItemText(CurItem);
		return OpenForm(0,"Модуль",csName);
	}
	else
	if(IsType(OBJ_RUN,CurItem,1))
	{
		CObjectData data=dataMetaObj[CurItem];
		//dataMetaObj[CurItem].csFunction;
		try
		{
			if(data.pRun)
				data.pRun->CallFunction(data.csFunction,data.vData);
			return (CDocument*)1;
		}
		catch(CTranslateError *)
		{
			DoError();
			return 0;
		}
	}
	else
	if(IsType(OBJ_DOC,CurItem))
	{
		CString csName=pCtrl->GetItemText(CurItem);
		return OpenForm(0,"Документация",csName);
	}
	else
	if(IsType(OBJ_MAKET,CurItem))
	{
		CString csName=pCtrl->GetItemText(CurItem);
		return OpenForm(0,"Таблица",csName);
	}
	else
	if(IsEditable(CurItem))//есть только окно свойств
	{
		ShowProperty(1);
		return (CDocument*)1;
	}
	return 0;
}

void CMetadataTree::OnKeydown(NMHDR* pNMHDR, LRESULT* pResult) 
{
	TV_KEYDOWN* pTVKeyDown = (TV_KEYDOWN*)pNMHDR;
	if(pTVKeyDown->wVKey==13)
		OnEdit();
	else
	if(pTVKeyDown->wVKey==VK_INSERT)
		OnNew();
	else
	if(pTVKeyDown->wVKey==VK_DELETE)
		OnRemove();

	if(treeDragItem)
		OnSetCursor(this,0,0);

	*pResult = 0;
}
void CMetadataTree::OnRemove() 
{
	Remove();
}
void CMetadataTree::Remove(HTREEITEM CurItem) 
{

	BOOL bFirst=0;
	AfxGetProperty()->ReleaseDoc(GetDocument());
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	if(!CurItem)
	{
		CurItem=pCtrl->GetSelectedItem();
		bFirst=1;
	}
	if(!IsEditable(CurItem))
		return;

	CMetaObject *pObject=GetMetaObject(CurItem);
	if(!pObject)
		return;

	if(!pObject->bCanSave)
		return;


	//перед удалением - помещаем в TrashDelete
	if(bFirst)
	if(!treeDragItem)
	if(pCtrl->GetParentItem(CurItem)!=hTrashDelete)
	{

		CCopyElement cCopy;
		cCopy.data=dataMetaObj[CurItem];
		cCopy.pObj=GetMetaObject(CurItem);
		cCopy.aChildList.RemoveAll();

		AddToCopyList(cCopy.aChildList,CurItem);
		InsertFromCopyList(cCopy,hTrashDelete);
	}


	if(pObject)
	{
		m_pMetadata->RemoveMetaObject(pObject);
		CDocList*p=metaDocList[pObject->nTempID];
		metaDocList[pObject->nTempID]=0;
		buferMetaObj[pObject->nTempID]=0;
		if(p)
			delete p;//закрываем все открытые окна


		//делаем цикл по подчиненным элементам
		HTREEITEM hItem=pCtrl->GetChildItem(CurItem);
		while(hItem)
		{
			HTREEITEM hNextItem=pCtrl->GetNextSiblingItem(hItem);
			SETITEMTYPE(hItem,1,0,0,0);//разрешаем удаление
			Remove(hItem);
			hItem=hNextItem;
		}
		pCtrl->DeleteItem(CurItem);
		GetDocument()->SetModifiedFlag(1);
	}
}




void CMetadataTree::OnModuleOpen() 
{
}





HTREEITEM CMetadataTree::AddObjectItem(CString csName,HTREEITEM hParent,BOOL bNew)
{
	HTREEITEM hItem;
    CTreeCtrl *pCtrl=&GetTreeCtrl();
	ObjectDescription *pObject;

	pObject=(ObjectDescription *)m_pMetadata->OList[mUpper(csName)];
	if(pObject)
	if(!bViewLibObjects && pObject->bReadOnly)
	{
		return 0;
	}


	//Имя самого объека
	HTREEITEM hCurItem=pCtrl->InsertItem(csName,ICON_OBJECT,ICON_OBJECT,hParent);
	SETITEMTYPE(hCurItem,1,0,OBJ_CLASS,0);


	//Модули
	HTREEITEM hModule;
	hModule=pCtrl->InsertItem(OBJMODULE, ICON_MODULEGROUP,ICON_MODULEGROUP, hCurItem);
	SETITEMTYPE(hModule,0,1,OBJ_MODULE,ICON_MODULE);
	//Общие модули объекта
	if(pObject)
	for(int i=0;i<pObject->aModule.GetSize();i++)
	{
		hItem=pCtrl->InsertItem(pObject->aModule[i], ICON_MODULE,ICON_MODULE, hModule);
		SETITEMTYPE(hItem,1,0,OBJ_MODULE,0);
	}

	//Системные модули
	hItem=pCtrl->InsertItem(CONFIGMODULENAME, ICON_CONFMODULE,ICON_CONFMODULE, hModule);
	SETITEMTYPE(hItem,0,0,OBJ_MODULE,0);
	if(bNew)
	{
		CMetaObject *pObj=GetMetaObject(hItem);
		pObj->csFile=CModuleDoc::GetDefaultText(DEFAULT_OBJ_CONF_MODULE);
	}

	hItem=pCtrl->InsertItem(ENTERPRISEMODULENAME, ICON_RUNMODULE,ICON_RUNMODULE, hModule);
	SETITEMTYPE(hItem,0,0,OBJ_MODULE,0);
	if(bNew)
	{
		CMetaObject *pObj=GetMetaObject(hItem);
		pObj->csFile=CModuleDoc::GetDefaultText(DEFAULT_OBJ_RUN_MODULE);
	}


	//Формы
	HTREEITEM hForm;
	hForm=pCtrl->InsertItem(OBJFORM, ICON_FORMGROUP,ICON_FORMGROUP, hCurItem);
	SETITEMTYPE(hForm,0,1,OBJ_FORM,ICON_FORM);
	//Общие формы объекта
	if(pObject)
	for(int i=0;i<pObject->aForms.GetSize();i++)
	{
		hItem=pCtrl->InsertItem(pObject->aForms[i], ICON_FORM,ICON_FORM, hForm);
		SETITEMTYPE(hItem,1,0,OBJ_FORM,0);
	}

	HTREEITEM hMaket;
	hMaket=pCtrl->InsertItem(OBJMAKET, ICON_MAKETGROUP,ICON_MAKETGROUP, hCurItem);
	SETITEMTYPE(hMaket,0,1,OBJ_MAKET,ICON_MAKET);
	//Общие таблицы
	if(pObject)
	for(int i=0;i<pObject->aMakets.GetSize();i++)
	{
		hItem=pCtrl->InsertItem(pObject->aMakets[i], ICON_MAKET,ICON_MAKET, hMaket);
		SETITEMTYPE(hItem,1,0,OBJ_MAKET,0);
	}

	HTREEITEM hConst;
	hConst=pCtrl->InsertItem(OBJCONST, ICON_CONSTGROUP,ICON_CONSTGROUP, hCurItem);
	SETITEMTYPE(hConst,0,1,OBJ_TEXT,ICON_CONST);
	//Список констант - глобальных переменных программы доступных только на чтение
	if(pObject)
	for(int i=0;i<pObject->aConst.GetSize();i++)
	{
		hItem=pCtrl->InsertItem(pObject->aConst[i], ICON_CONST,ICON_CONST, hConst);
		SETITEMTYPE(hItem,1,0,OBJ_TEXT,0);
	}

	return hCurItem;
}

void CMetadataTree::InsertSystemElements()
{
    CTreeCtrl *pCtrl=&GetTreeCtrl();

	treeMODULES=pCtrl->InsertItem(MODULESNAME, ICON_MODULEGROUP,ICON_MODULEGROUP,0);
	SETITEMTYPE(treeMODULES,0,1,OBJ_MODULE,ICON_MODULE);

	HTREEITEM hItem;
	hItem=pCtrl->InsertItem(INITMODULENAME, ICON_INITMODULE,ICON_INITMODULE,treeMODULES);
	SETITEMTYPE(hItem,0,0,OBJ_MODULE,0);

	CMetaObject *pObj=GetMetaObject(hItem);
	if(pObj->csFile.IsEmpty())
		pObj->csFile=CModuleDoc::GetDefaultText(DEFAULT_OBJ_COMMON_MODULE);
	

	treeFORMS=pCtrl->InsertItem(FORMSNAME, ICON_FORMGROUP,ICON_FORMGROUP);
	SETITEMTYPE(treeFORMS,0,1,OBJ_FORM,ICON_FORM);

	treeMAKETS=pCtrl->InsertItem(MAKETSNAME, ICON_MAKETGROUP,ICON_MAKETGROUP,0);
	SETITEMTYPE(treeMAKETS,0,1,OBJ_MAKET,ICON_MAKET);

	treeOBJECTS=pCtrl->InsertItem(OBJECTSNAME, ICON_OBJECTGROUP,ICON_OBJECTGROUP, 0);
	SETITEMTYPE(treeOBJECTS,0,1,OBJ_CLASS,ICON_OBJECT);

	TreeMetadata=pCtrl->InsertItem(METADATANAME, 225,225, 0);
}

BOOL CMetadataTree::IsCanView(CString csName)
{
	if(bViewLibObjects==0)
	{
		CMetaObject *pObj=m_pMetadata->GetMetaObject(csName,"");
		if(pObj)
			if(pObj->bCanSave==0)
				return 0;
	}
	return 1;
}

//Заполнение визуального дерева метаданных списком доступных объектов
BOOL CMetadataTree::Load(int nReload)
{
	int i;

    CTreeCtrl *pCtrl=&GetTreeCtrl();
	pCtrl->SetRedraw(0);

	HTREEITEM hItem;

	if(nReload==0)
	{
		for(i=0;i<m_pMetadata->ListModuleName.GetSize();i++)
		{
			if(!IsCanView(MODULESNAME+"\\"+m_pMetadata->ListModuleName[i]))
				continue;

			hItem=pCtrl->InsertItem(m_pMetadata->ListModuleName[i], ICON_MODULE,ICON_MODULE,treeMODULES);
			SETITEMTYPE(hItem,1,0,OBJ_MODULE,0);
		}
		for(i=0;i<m_pMetadata->ListFormName.GetSize();i++)
		{
			if(!IsCanView(FORMSNAME+"\\"+m_pMetadata->ListFormName[i]))
				continue;

			hItem=pCtrl->InsertItem(m_pMetadata->ListFormName[i], ICON_FORM,ICON_FORM,treeFORMS);
			SETITEMTYPE(hItem,1,0,OBJ_FORM,0);
		}
		for(i=0;i<m_pMetadata->ListMaketName.GetSize();i++)
		{
			if(!IsCanView(MAKETSNAME+"\\"+m_pMetadata->ListMaketName[i]))
				continue;

			hItem=pCtrl->InsertItem(m_pMetadata->ListMaketName[i], ICON_MAKET,ICON_MAKET,treeMAKETS);
			SETITEMTYPE(hItem,1,0,OBJ_MAKET,0);
		}
	}

	if(nReload)
	{
		pCtrl->DeleteItem(TreeMetadata);
		TreeMetadata=pCtrl->InsertItem(METADATANAME, 225,225, 0);
	}

	for(i=0;i<m_pMetadata->ListObjectName.GetSize();i++)
	{
		if(nReload==0)
		{
			AddObjectItem(m_pMetadata->ListObjectName[i],treeOBJECTS);
		}

		try
		{
			CProcUnit *pRun=m_RunModuleManager.GetRunModule(OBJECTSNAME+"\\"+m_pMetadata->ListObjectName[i]+"\\"+OBJMODULE+"\\"+CONFIGMODULENAME);
			CValue vRes1,vRes2,vRes3;


			vRes2.CreateObject("ДеревоЗначений");
			((CValueTreeCtrl*)vRes2.pRef)->SetTree(pCtrl);
			((CValueTreeCtrl*)vRes2.pRef)->pMetaTree=this;

			vRes3=(int)TreeMetadata;
			pRun->CallFunction("ПриЗагрузкеКонфигурации",vRes2,vRes3);

		}
		catch(CTranslateError *)
		{
			DoError();
			//return 0;
		};
	}
	pCtrl->SetRedraw(1);


	LoadObjectList();

	if(nReload)
	{
		if(hTrash)
			pCtrl->DeleteItem(hTrash);
		if(hTrashDelete)
			pCtrl->DeleteItem(hTrashDelete);
	}

	hTrash=pCtrl->InsertItem("Репозитарий", 377,377, 0);
	LoadMetaFolder(m_pMetadata->vMeta,hTrash);

	hDocuments=pCtrl->InsertItem("Документация", 394,394, 0);
	LoadMetaFolder(m_pMetadata->vMeta,hDocuments);
	
	CObjectData data;
	data.bEdit=0;
	data.bGroup=1;
	data.nChildImage=396;
	data.nType=OBJ_DOC;
	dataMetaObj[hDocuments]=data;

	

	hTrashDelete=pCtrl->InsertItem("Корзина", 393,393, 0);
	LoadMetaFolder(m_pMetadata->vMeta,hTrashDelete);

	
	TreeReadOnly=0;
	if(!bViewLibObjects)
		TreeReadOnly=pCtrl->InsertItem("Только просмотр", 1,1, 0);

	//pCtrl->Expand(treeMODULES,TVE_EXPAND);
	//pCtrl->Expand(treeFORMS,TVE_EXPAND);
	//pCtrl->Expand(treeMAKETS,TVE_EXPAND);
	//pCtrl->Expand(treeOBJECTS,TVE_EXPAND);
	pCtrl->Expand(TreeMetadata,TVE_EXPAND);
	return TRUE;
}

void LoadObjectsList()
{
	static CString Str1;
	static CString Str2;

	CString Str0;
	CString Str;
	CMetadataTree *pMetaTree=AfxGetMetadataTree();
	if(!pMetaTree)
		return;

	CTreeCtrl *pCtrl=&pMetaTree->GetTreeCtrl();
	for(int i=0;i<m_pMetadata->ListObjectName.GetSize();i++)
	{
		//CString csName=pCtrl->GetItemText(CurItem);
		CString csName=m_pMetadata->ListObjectName[i];
		Str=Str+","+csName;
		Str0=Str0+","+csName;
		//ищем в папке метаданных
		HTREEITEM CurTypeItem=pCtrl->GetChildItem(pMetaTree->TreeMetadata);
		while(CurTypeItem)
		{
			CString csName2=pCtrl->GetItemText(CurTypeItem);
			//ищем нужный тип объекта
			if(mUpper(csName)==mUpper(csName2))
			{
				//получаем виды
				HTREEITEM CurItemKind=pCtrl->GetChildItem(CurTypeItem);
				while(CurItemKind)
				{
					CString csKindName=pCtrl->GetItemText(CurItemKind);
					Str=Str+","+csName+"."+csKindName;
					CurItemKind=pCtrl->GetNextSiblingItem(CurItemKind);
				}
				break;
			}
			CurTypeItem=pCtrl->GetNextSiblingItem(CurTypeItem);
		}
	}

	//новый список объектов
	HTREEITEM CurItem;
	CurItem=pCtrl->GetChildItem(pMetaTree->treeOBJECTS);
	while(CurItem)
	{
		CString csName=pCtrl->GetItemText(CurItem);
		if(!m_pMetadata->OList[mUpper(csName)])
		{
			Str=Str+","+csName;
			Str0=Str0+","+csName;
		}
		CurItem=pCtrl->GetNextSiblingItem(CurItem);
	}




	Str1=CString(",")+csBaseType+","+Str+",";
	Str2=Str0;

//	pObject=(ObjectDescription *)m_pMetadata->OList[mUpper(csName)];
	//if(pObject)
	//	m_pMetadata->ListObjectName


	aChooseList[nTypeValue]=Str1.GetBuffer(0);
	aChooseList[nTypeObject]=Str2.GetBuffer(0);
}

void CMetadataTree::LoadObjectList()
{
	::LoadObjectsList();
}

//загрузка элементов папки
void CMetadataTree::LoadMetaFolder(CValue Val,HTREEITEM hParentItem)
{
//	return;
    CTreeCtrl *pCtrl=&GetTreeCtrl();

	if(Val.GetType()!=TYPE_ARRAY)
		return;
	CString csName=pCtrl->GetItemText(hParentItem);
	CValue vData=Val.GetAt(csName);
	if(vData.GetSizeStrArray())
	if(!vData.GetAt(String("#")))//это не атрибут
	for(int i=1;i<=vData.GetSizeStrArray();i++)
	{
		CString csName=vData.CallFunction("ИдентификаторПоНомеру",&(CValue(i)));
		CString csPath=GetPath(hParentItem)+"\\"+csName;
		CMetaObject *pObj=m_pMetadata->GetMetaObject(csPath,csName,1);
		if(pObj)
		{
			int nImage=pObj->vExtVal.GetAt("Image");
			HTREEITEM CurItem=pCtrl->InsertItem(csName,nImage,nImage,hParentItem);

			CObjectData *pData=&dataMetaObj[CurItem];
			pData->nChildImage=pObj->vExtVal.GetAt("ChildImage");
			pData->bEdit=pObj->vExtVal.GetAt("Edit");
			pData->bGroup=pObj->vExtVal.GetAt("Group");
			pData->nType=pObj->vExtVal.GetAt("Type");

			LoadMetaFolder(vData,CurItem);
		}
	}

}


BOOL CMetadataTree::SaveMetadata()
{
    CTreeCtrl *pCtrl=&GetTreeCtrl();
	for(int i=0;i<m_pMetadata->ListObjectName.GetSize();i++)
	{
		try
		{
			CProcUnit *pRun=m_RunModuleManager.GetRunModule(OBJECTSNAME+"\\"+m_pMetadata->ListObjectName[i]+"\\"+OBJMODULE+"\\"+CONFIGMODULENAME);
			CValue vRes1,vRes2;
//			int nVisible=(int)pRun->CallFunction("ВидимыйВДереве").GetNumber();
//			if(nVisible)
			{
				vRes2.CreateObject("ДеревоЗначений");
				((CValueTreeCtrl*)vRes2.pRef)->SetTree(pCtrl);
				pRun->CallFunction("ПриСохраненииКонфигурации",vRes2);
			}

		}
		catch(CTranslateError *)
		{
			DoError();
			//return FALSE;
		};
	}
	return TRUE;
}

BOOL CMetadataTree::Save()
{
	AfxGetProperty()->EndEdit();
	//SetFocus();
	cCopy.pObj=0;

    CTreeCtrl *pCtrl=&GetTreeCtrl();
	//получаем все интерактивно обновленные данные обратно в контейнер
	GetAllData();

	//заносим информацию о типе объекта в контейнер
	CObjectData data;
	POSITION	pos;
	HTREEITEM	Key;
	for( pos = dataMetaObj.GetStartPosition(); pos != NULL; )
	{
		dataMetaObj.GetNextAssoc( pos, Key, data);
		CMetaObject *pObj=buferMetaObj[Key];
		if(pObj)
		{
			pObj->csName=pCtrl->GetItemText(pObj->nTempID);
			if(data.bEdit&&data.bGroup==0&&data.nType==OBJ_ATTR)
			{
				CValue Val;
				data.SaveData(Val);
				Val.SaveToString(pObj->csAttr);
			}
			else
			if(data.bEdit&&data.bGroup==0&&data.nType==OBJ_MODULE)
			{
				if(pObj->csFile.IsEmpty())
					pObj->csFile.Format("//%s\n",pObj->csName);
			}
/*			else
			if(data.bEdit&&data.nType==OBJ_CLASS)
			{
				pObj->csInfo=pObj->csName;
			}
			else
			if(data.bEdit&&data.bGroup==0&&data.nType==OBJ_TEXT)
			{
				pObj->csInfo=pObj->csName;
			}
			else
*/
			if(data.bEdit)
			{
				pObj->SaveToInfo();
			}
		}
	}


	if(m_pMetadata->Save())
	{
		SaveMetadata();
		return TRUE;
	}
	else
	{
		//GetDocument()->SetModifiedFlag(1);
		return FALSE;
	}
}
//забираем данные у всех объектов в контейнерный объект
void CMetadataTree::GetAllData()
{
	POSITION pos;
	HTREEITEM key;
	CDocList* pa;
	for( pos = metaDocList.GetStartPosition(); pos != NULL; )
	{
		metaDocList.GetNextAssoc( pos, key, pa);
		if(pa)
			pa->SaveModified();
	}
}
//проверка изменений у всех объектов
void CMetadataTree::CheckModified()
{
	POSITION pos;
	HTREEITEM key;
	CDocList* pa;
	for( pos = metaDocList.GetStartPosition(); pos != NULL; )
	{
		metaDocList.GetNextAssoc( pos, key, pa);
		if(pa)
			if(pa->IsModified())
			{
				GetDocument()->SetModifiedFlag(1);
				break;
			}
	}
}

BOOL CMetadataTree::IsEditable(HTREEITEM CurItem) 
{
	if(!CurItem)
		CurItem=GetTreeCtrl().GetSelectedItem();

	return dataMetaObj[CurItem].bEdit;
}
BOOL CMetadataTree::IsGroup(HTREEITEM CurItem) 
{
	if(IsTrash(CurItem))
		return TRUE;

	if(!CurItem)
		CurItem=GetTreeCtrl().GetSelectedItem();

	return dataMetaObj[CurItem].bGroup;
}

BOOL CMetadataTree::IsType(UINT nType,HTREEITEM CurItem,BOOL bNotCheckGroup) 
{
	if(!CurItem)
		CurItem=GetTreeCtrl().GetSelectedItem();

	if(bNotCheckGroup)
	{
		if(IsTrash(CurItem))
			return TRUE;
		if(IsTrash(GetTreeCtrl().GetParentItem(CurItem)))
			return TRUE;
	}

	CObjectData data=dataMetaObj[CurItem];
	if(bNotCheckGroup==0)
	if(data.bGroup)
		return FALSE;
	if(data.nType==nType)
		return TRUE;
	else
		return FALSE;
}

void CMetadataTree::ShowProperty(bool bShow) 
{
	if(afxAppRunMode!=CONFIG_MODE)
		return;

	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM hSel=pCtrl->GetSelectedItem();
	CMetaObject *pObj=GetMetaObject();

/*	if(pObj)
	if(!pObj->bEnable)
	{
		   pCtrl->SetItemState(hSel, TVIS_CUT, TVIS_CUT);
	}
*/

	if(bShow==0&&AfxGetProperty()->IsWindowVisible()==0)
		return;

	if(AfxGetProperty()->IsWindowVisible()==0&&bShow==1)
	{
		((CMainFrame*)AfxGetMainWnd())->ShowPropertyBar();
	}

	if(!IsEditable())
	{
		AfxGetProperty()->ReleaseDoc(GetDocument());
		return;
	}

	if(pObj)
	{
		AfxGetProperty()->Clear();
		AfxGetProperty()->SetDoc(GetDocument(),(LPARAM)pObj,(LPARAM)this,!pObj->bCanSave);
		AfxGetProperty()->AddProperty("Основные","Идентификатор",pObj->csName);
		AfxGetProperty()->AddProperty("Основные","Синоним",pObj->csAlias);
		AfxGetProperty()->AddProperty("Основные","Комментарий",pObj->csComment);
		AfxGetProperty()->AddProperty("Основные","Использование",pObj->bEnable);
		AfxGetProperty()->AddProperty("Основные","ИдентификаторБД",pObj->csIdDB);


		//TVIS_CUT 
		if(IsType(OBJ_ATTR,0,1))
		{
			LoadObjectList();
			CString Str=aChooseList[nTypeValue];
			CObjectData *pData=&dataMetaObj[hSel];

			AfxGetProperty()->AddProperty("Основные","Параметр",pObj->csParam1);
			AfxGetProperty()->AddProperty("Основные","Тип элемента",pData->sType,Str);
			AfxGetProperty()->AddProperty("Основные","Длина",pData->nLength);
			AfxGetProperty()->AddProperty("Основные","Точность",pData->nPrec,0,10);
		}
		else
		if(pCtrl->GetParentItem(hSel)==treeOBJECTS)
		{
		
			LoadObjectsList();
			CString Str=aChooseList[nTypeObject];
			AfxGetProperty()->AddProperty("Наследование","Родитель",pObj->csParam1,Str);
		}
	}

}
	

void CMetadataTree::OnProperty() 
{
	if(!IsEditable()&&IsType(OBJ_RUN))
		Edit();
	ShowProperty(1);
}

void CMetadataTree::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CTreeView::OnLButtonDown(nFlags, point);

	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->HitTest(point);
	pCtrl->SelectItem(CurItem);
	ShowProperty(0);
}

void CMetadataTree::Clear()
{
	POSITION pos;
	HTREEITEM key;
	CDocList* pa;
	for( pos = metaDocList.GetStartPosition(); pos != NULL; )
	{
		metaDocList.GetNextAssoc( pos, key, pa);
		if(pa)
		{
			CDocList *p=metaDocList[key];
			metaDocList[key]=0;
			if(p)
				delete p;//закрываем все подчиненные окна
		}
	}
	metaDocList.RemoveAll();
}

void CMetadataTree::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags) 
{

	if(nChar==VK_SHIFT||nChar==VK_CONTROL)
	{
		OnSetCursor(this,0,0);
		return;
	}

	CTreeView::OnKeyUp(nChar, nRepCnt, nFlags);
	ShowProperty(0);


	EndDrag();
}

void CMetadataTree::OnInitialUpdate() 
{
	GetDocument()->SetPathName(CONFIG_NAME);

	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.AddButton( this, IDR_METADATATREE );

	CTreeView::OnInitialUpdate();

	LoadOpenModulesState();
}

void CMetadataTree::OnDestroy() 
{
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
	CTreeView::OnDestroy();
}


void AddControl(CValue &ObjectVal,int nType,CString csName,CString csAlias,CString sType)
{
	ObjectVal.CreateObject("Структура");
	ObjectVal.SetAt("ТипЭлемента",nType);
	ObjectVal.SetAt("Тип значения",sType);
	ObjectVal.SetAt("Идентификатор",csName);
	ObjectVal.SetAt("Заголовок",csAlias); 
	ObjectVal.SetAt("Положение верт","Центр");
	ObjectVal.SetAt("Положение гориз","Лево");

	ObjectVal.SetAt("Вниз",0);
	ObjectVal.SetAt("Вправо",0);

	ObjectVal.SetAt("Видимость",1);
	ObjectVal.SetAt("Доступность",1);
	if(!sType.IsEmpty())
		if(sType!="Строка")
			ObjectVal.SetAt("Кнопка выбора",1);

	ObjectVal.SetAt("y1",0);
	ObjectVal.SetAt("y2",22);
}
void CMetadataTree::OnEditCopy() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_COPY);
		return;
	}


	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetSelectedItem();
	cCopy.data=dataMetaObj[CurItem];
	cCopy.pObj=GetMetaObject(CurItem);
	//запоминаем подчиненные элементы
	cCopy.aChildList.RemoveAll();
	AddToCopyList(cCopy.aChildList,CurItem);


	//**************************************
	//поддержка вставки диалоговых элементов

	ValCopyPaste.CreateObject("Структура");
	CValue ObjectVal;
	//надпись
	CString csAlias=cCopy.pObj->csAlias;
	if(csAlias.IsEmpty())
		csAlias=cCopy.pObj->csName;
	AddControl(ObjectVal,ID_DYNBUTTONLABEL,CString("Т")+cCopy.pObj->csName+cCopy.pObj->csName,csAlias,"");
	ObjectVal.SetAt("x1",0);
	ObjectVal.SetAt("x2",80);
	ValCopyPaste.SetAt(2,ObjectVal);

	//элемент редактирования
	CString sType=cCopy.data.sType;

	if(sType.IsEmpty()) 
	{
		HTREEITEM hParent=pCtrl->GetParentItem(CurItem);
		if(hParent)
		{
			if(hParent==TreeMetadata)
				sType=cCopy.pObj->csName;
			else
			if(pCtrl->GetParentItem(hParent)==TreeMetadata)
				sType=pCtrl->GetItemText(hParent)+"."+cCopy.pObj->csName;
		}
	}

	AddControl(ObjectVal,ID_DYNBUTTONEDIT,cCopy.pObj->csName,"",sType);
	ObjectVal.SetAt("x1",80);
	ObjectVal.SetAt("x2",240);
	ValCopyPaste.SetAt(1,ObjectVal);

	int x1=245;
	int n=3;
	//кнопка-просмотр
	if(!sType.IsEmpty())
	if(sType!="Число"&&sType!="Строка"&&sType!="Дата")
	{
		AddControl(ObjectVal,ID_DYNBUTTON,CString("П")+cCopy.pObj->csName," ","");
		ObjectVal.SetAt("x1",x1);x1+=20;
		ObjectVal.SetAt("x2",x1);
		ObjectVal.SetAt("Картинка",596);
		ObjectVal.SetAt("Картинка гориз","Центр");
		ObjectVal.SetAt("Картинка верт","Центр");
		ObjectVal.SetAt("Нажатие",CString("ОткрытьФорму(")+cCopy.pObj->csName+")");
		ValCopyPaste.SetAt(n,ObjectVal);n++;

		//кнопка-очистка
		AddControl(ObjectVal,ID_DYNBUTTON,CString("О")+cCopy.pObj->csName," ","");
		ObjectVal.SetAt("x1",x1);x1+=20;
		ObjectVal.SetAt("x2",x1);
		ObjectVal.SetAt("Картинка",595);
		ObjectVal.SetAt("Картинка гориз","Центр");
		ObjectVal.SetAt("Картинка верт","Центр");
		ObjectVal.SetAt("Нажатие",cCopy.pObj->csName+"=\"\"");
		ValCopyPaste.SetAt(n,ObjectVal);n++;
	}

}
void CMetadataTree::AddToCopyList(CCopyArray &aChildList,HTREEITEM hParent) 
{
	//TODO
/*	// Create a shared memory file
	CSharedFile sf(GMEM_MOVEABLE | GMEM_SHARE | GMEM_ZEROINIT);
	CString buffer = "{\\rtf1 {1 \\tab 2 \\tab 3 \\par 4 \\tab {\\b\\i 5} \\tab 6}}";
	sf.Write(buffer, buffer.GetLength()); 
*/


	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetChildItem(hParent);
	while(CurItem)
	{
		int i=aChildList.GetSize();
		aChildList.SetSize(i+1);
		CCopyElement *pElement=&aChildList[i];
		pElement->data=dataMetaObj[CurItem];
		pElement->pObj=GetMetaObject(CurItem);
		AddToCopyList(pElement->aChildList,CurItem);
		CurItem=pCtrl->GetNextSiblingItem(CurItem);
	}
}


HTREEITEM CMetadataTree::InsertFromCopyList(CCopyElement &cElement,HTREEITEM hParent,HTREEITEM hInsertAfter) 
{
	CString csName=GetNewName(hParent,cElement.pObj->csName);
	HTREEITEM NewItem=0;
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	int nImage=cElement.pObj->vExtVal.GetAt("Image");
	NewItem=pCtrl->InsertItem(csName,nImage,nImage,hParent,hInsertAfter);
	dataMetaObj[NewItem]=cElement.data;
	CMetaObject *pObj=GetMetaObject(NewItem);
	if(pObj)
	{
		pObj->csAlias=cElement.pObj->csAlias;
		pObj->csAttr=cElement.pObj->csAttr;
		pObj->csComment=cElement.pObj->csComment;
		pObj->csFile=cElement.pObj->csFile;
		pObj->csForm=cElement.pObj->csForm;
		pObj->csInfo=cElement.pObj->csInfo;
	}
	if(NewItem)
	for(int i=0;i<cElement.aChildList.GetSize();i++)
		InsertFromCopyList(cElement.aChildList[i],NewItem);

	return NewItem;
}

void CMetadataTree::OnEditPaste() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_PASTE);
		return;
	}

	if(cCopy.pObj)
	{
		CTreeCtrl *pCtrl=&GetTreeCtrl();
		HTREEITEM hCurItem=pCtrl->GetSelectedItem();
		HTREEITEM hParent=GetParentForNew();
		AfxGetProperty()->ReleaseDoc(GetDocument());
		if(IsGroup(hParent))
		{
			hCurItem=pCtrl->GetPrevSiblingItem(hCurItem);
			if(!hCurItem)
				hCurItem=TVI_FIRST;

			HTREEITEM NewItem=InsertFromCopyList(cCopy,hParent,hCurItem);
			pCtrl->Invalidate();
			pCtrl->SelectItem(NewItem);
			GetDocument()->SetModifiedFlag(1);
			ShowProperty(0);
		}
		else
		{
			//замена
			if(dataMetaObj[hCurItem].nType==cCopy.data.nType)
			{
				CMetaObject *pObj=GetMetaObject(hCurItem);
				if(pObj)
				{
					pObj->csAlias=cCopy.pObj->csAlias;
					pObj->csAttr=cCopy.pObj->csAttr;
					pObj->csComment=cCopy.pObj->csComment;
					pObj->csFile=cCopy.pObj->csFile;
					pObj->csForm=cCopy.pObj->csForm;
					pObj->csInfo=cCopy.pObj->csInfo;
				}

				GetDocument()->SetModifiedFlag(1);
				ShowProperty(0);
			}
		}

	}
}

void CMetadataTree::OnEditCut() 
{
	CWnd *pW=GetFocus();
	if(pW!=this)
	{
		pW->SendMessage(WM_CUT);
		return;
	}
	OnEditCopy();
	OnRemove();
}

void CMetadataTree::OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint) 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	CMetaObject *pObj=(CMetaObject *)AfxGetProperty()->GetDocData();
	if(AfxGetProperty()->GetDocType()==(LPARAM)this)
	if(pObj)
	{
		pCtrl->SetItemText(pObj->nTempID,pObj->csName);

/*		CDocList* pDocList=metaDocList[pObj->nTempID];
		if(pDocList)
			pDocList->Update();//обновляем открытые формы
*/		
	}
}
void CMetadataTree::SaveReplaceForm(int nMode,CString csFile)
{

	BOOL bAskOnSave=0;
	if(csFile.IsEmpty())
	{
		bAskOnSave=1;
		CValue vFile;
		try
		{
			CValue FC;
			FC.CreateObject("Файловая Система");
			CValue **p=new CValue*[5];
			CValue vDir;

			CValue vMode=0;
			if(nMode==MODE_SAVE)
			{
				vMode=1;
				CMetaObject *pObj=GetMetaObject();
				vFile=String(pObj->csName);
			}
			CValue tCaption("Файл внешней формы")
				,tExt("Формы 2С|*.fd|Все файлы|*.*");
			CValue Ret=FC.CallFunction("ВыбратьФайл",&vMode,&vFile,&vDir,&tCaption,&tExt);
			if(Ret.GetNumber()!=1)
				return;
		}
		catch(CTranslateError *)
		{
			DoError();
			return;
		};
		csFile=vFile.GetString();
	}

	CZipArchive m_zip;
	try
	{
		if(nMode==MODE_SAVE)
		{
			if(bAskOnSave)
			if(FileExist(csFile))
			{
				int nRet=MessageBox("Файл уже существует!\nПерезаписать?","Внимание!",MB_OKCANCEL|MB_ICONQUESTION);
				if(nRet!=1)
					return;
			}
			m_zip.Open(csFile, CZipArchive::create,0);
		}
		else
			m_zip.Open(csFile, CZipArchive::openReadOnly);
	}
	catch(...)
	{
		AfxMessageBox(CString("Ошибка открытия файла ")+csFile);
		return;
	}

	if(nMode==MODE_INSERT)
		NewItem(0);
	else
	if(nMode==MODE_REPLACE)
	{
		CMetaObject *pObj=GetMetaObject();
		CDocList* pDocList=metaDocList[pObj->nTempID];
		if(pDocList)
		{
			delete pDocList;
			metaDocList[pObj->nTempID]=0;
			buferMetaObj[pObj->nTempID]=0;
		}
	}


	CMetaObject *pObj=GetMetaObject();
	CString csAlias;
	CString csComment;
	if(nMode==MODE_SAVE)
	{
		WriteFileFromString(pObj->csFile,m_zip,MODULENAME,"","");
		WriteFileFromString(pObj->csForm,m_zip,FORMNAME,"","");
		WriteFileFromString(pObj->csComment,m_zip,DESCRIPNAME,"","");
	}
	else
	{
		pObj->csFile=ReadFileToString(m_zip,MODULENAME,csAlias,csComment);
		pObj->csComment=ReadFileToString(m_zip,DESCRIPNAME,csAlias,csComment);
		pObj->csForm=ReadFileToString(m_zip,FORMNAME,csAlias,csComment);
	}
	m_zip.Close();	
	//GetMetaObject();


	if(nMode==MODE_INSERT)
	{
		int nIndex1=csFile.ReverseFind('\\');
		CString csName=csFile.Mid(nIndex1+1);
		int nIndex2=csName.ReverseFind('.');
		csName=csName.Left(nIndex2);

		CTreeCtrl *pCtrl=&GetTreeCtrl();
		HTREEITEM hParent=pCtrl->GetParentItem(pObj->nTempID);

		pObj->csName=GetNewName(hParent,csName);
		pCtrl->SetItemText(pObj->nTempID,pObj->csName);
	}
	if(nMode!=MODE_SAVE)
	{
		GetDocument()->SetModifiedFlag(1);
		ShowProperty(0);
	}

}

void CMetadataTree::OnInsertForm() 
{
	SaveReplaceForm(MODE_INSERT);
}

void CMetadataTree::OnSaveform() 
{
	SaveReplaceForm(MODE_SAVE);
}

void CMetadataTree::OnReplaceform() 
{
	SaveReplaceForm(MODE_REPLACE);
}

void CMetadataTree::OnUpdateEditCut(CCmdUI* pCmdUI) 
{
	if(GetFocus()!=this)
	{
		pCmdUI->Enable(1);
		return;
	}
	pCmdUI->Enable(IsEditable());
	
}

void CMetadataTree::OnUpdateEditCopy(CCmdUI* pCmdUI) 
{
	if(GetFocus()!=this)
	{
		pCmdUI->Enable(1);
		return;
	}

//копировать можно все...	pCmdUI->Enable(IsEditable());
}

void CMetadataTree::OnUpdateEditPaste(CCmdUI* pCmdUI) 
{
	if(GetFocus()!=this)
	{
		pCmdUI->Enable(1);
		return;
	}
	if(IsType(cCopy.data.nType,0,1))//&&IsEditable())
		pCmdUI->Enable(1);
	else
		pCmdUI->Enable(0);
}

void CMetadataTree::OnBegindrag(NMHDR* pNMHDR, LRESULT* pResult) 
{
	NM_TREEVIEW* pNMTreeView = (NM_TREEVIEW*)pNMHDR;
	*pResult = 0;

	
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->HitTest(pNMTreeView->ptDrag);
	if(IsEditable(CurItem))
	{
		pCtrl->SelectItem(CurItem);
		cDataForDrag=dataMetaObj[CurItem];
		treeDragItem=CurItem;
		ShowProperty(0);
		OnEditCopy();
	}
	else
		pCtrl->SelectItem(CurItem);
}

void CMetadataTree::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();

	if(treeDragItem)
	{
		HTREEITEM CurItem=pCtrl->GetSelectedItem();
		if(CurItem==pCtrl->HitTest(point))
		if(CurItem!=treeDragItem)
		{
			pCtrl->SelectItem(treeDragItem);
			if(IsCTRLpressed()||IsSHIFTpressed())//режим копирования объектов
				OnEditCopy();
			else//режим перемещения объектов
				OnEditCut();
			pCtrl->SelectItem(CurItem);
			OnEditPaste();
		}
		EndDrag();
	}
	CTreeView::OnLButtonUp(nFlags, point);
}

BOOL CMetadataTree::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if(treeDragItem)
	{
		if(IsCTRLpressed()||IsSHIFTpressed())//режим копирования объектов
			SetCursor(hDragCursor);
		else
			SetCursor(hDragCursorMove);
		return 1;
	}
	
	return CTreeView::OnSetCursor(pWnd, nHitTest, message);
}

void CMetadataTree::EndDrag() 
{
	if(treeDragItem)
	{
		treeDragItem=0;
		SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	}
}

void CMetadataTree::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(treeDragItem)
	{
		if(GetFocus()!=this)
		{
			GetParentFrame()->BringWindowToTop();
			SetFocus();
		}

		if(nFlags==0)
		{
			EndDrag();
			return;
		}
		CTreeCtrl *pCtrl=&GetTreeCtrl();
		HTREEITEM CurItem=pCtrl->HitTest(point);
		if(CurItem)
		{
			if(IsType(cDataForDrag.nType,CurItem,1))
			{
				pCtrl->SelectItem(CurItem);
				//ShowProperty(0);
			}
		}
	}
	
	//CTreeView::OnMouseMove(nFlags, point);
}

void CMetadataTree::OnContextMenu(CWnd* pWnd, CPoint point) 
{
}

void CMetadataTree::EditModule(CString csObjName) 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetSelectedItem();
	CurItem=pCtrl->GetChildItem(CurItem);
	CurItem=pCtrl->GetChildItem(CurItem);
	while(CurItem)
	{
		CString csName=pCtrl->GetItemText(CurItem);
		if(csName==csObjName)
		{
			pCtrl->SelectItem(CurItem);
			Edit();
			return;
		}
		CurItem=pCtrl->GetNextSiblingItem(CurItem);
	}
}
int CMetadataTree::EditModule(CString csPath,int nLine,BOOL bSetRunLine) 
{
	CDocument* pCurrentDoc=0;
	if(FileExist(csPath))
	{
		pCurrentDoc=CEnterpriseApp::m_pSystemManager->OpenDocumentFile(csPath);
	}
	else
	{
		CTreeCtrl *pCtrl=&GetTreeCtrl();
		HTREEITEM CurItem=pCtrl->GetRootItem();

		csPath+="\\";
		int nIndexPrev=-1;
		int nIndex=csPath.Find("\\");
		while(nIndex>=0)
		{
			//ищем текущее имя подобъекта
			CString csObjName=csPath.Mid(nIndexPrev+1,nIndex-nIndexPrev-1);

			if(nIndexPrev!=-1)
				CurItem=pCtrl->GetChildItem(CurItem);

			//ищем текущий элемент в дереве
			while(CurItem)
			{
				CString csName=pCtrl->GetItemText(CurItem);
				if(mUpper(csName)==mUpper(csObjName))
					break;
				CurItem=pCtrl->GetNextSiblingItem(CurItem);
			}
			if(!CurItem)
				break;

			nIndexPrev=nIndex;
			nIndex=csPath.Find("\\",nIndex+1);
		}

		//может это объекты только для чтения
		if(!CurItem)
		{
			CurItem=pCtrl->GetChildItem(TreeReadOnly);
			while(CurItem)
			{
				CString csName=pCtrl->GetItemText(CurItem);
				if(mUpper(csName)==mUpper(csPath))
					break;
				CurItem=pCtrl->GetNextSiblingItem(CurItem);
			}
		}
		if(!CurItem)
		{
			//открываем новое окно
			CObjectData data;
			CurItem=pCtrl->InsertItem(csPath,ICON_MODULE,ICON_MODULE,TreeReadOnly);
			SETITEMTYPE(CurItem,0,0,OBJ_MODULE,0);
		}

		if(CurItem)
		{
			pCtrl->SelectItem(CurItem);
			pCurrentDoc=Edit();
		}
	}

	if(pCurrentDoc)
	if(((int)pCurrentDoc)!=1)
	{
		CRuntimeClass *Run=pCurrentDoc->GetRuntimeClass();
		if(CString(Run->m_lpszClassName)==CString("CModuleDoc"))
		{
			CModuleDoc *pMDoc=(CModuleDoc *)pCurrentDoc;
			CMyCodeView *pModule=pMDoc->GetModule();
			if(pModule)
			{
				CChildFrame *pFrame=(CChildFrame *)pModule->GetParentFrame();
				if(pFrame->GetRuntimeClass())
					if(CString(pFrame->GetRuntimeClass()->m_lpszClassName)=="CChildFrame")
						pFrame->m_TabViewContainer.SetActivePageIndex(1);

				pModule->GotoLine(nLine-1,bSetRunLine);
				return 1;
			}
		}
	}
	return 0;
}

void CMetadataTree::OnModuleConfig() 
{
	EditModule(CONFIGMODULENAME); 
}

void CMetadataTree::OnModuleRun() 
{
	EditModule(ENTERPRISEMODULENAME);
	
}


void CMetadataTree::OnOpenconfig() 
{
//	AfxGetProperty()->EndEdit();
//	GetAllData();
/*
	CModuleManager* p=new CModuleManager();
	AfxSetModuleManager(p);

	csIBDir.TrimRight("\\");
	CString csConfigPath=csIBDir+"\\"+METADATA_FILENAME;
	m_RunModuleManager.Load(csConfigPath);//конфигуратор


	//Load(1);
*/
}

void CMetadataTree::OnNewFolder() 
{
	CTreeCtrl *pCtrl=&GetTreeCtrl();
	HTREEITEM CurItem=pCtrl->GetSelectedItem();
	NewItem(1,"Группа",CurItem,1);
}
 

void CMetadataTree::SaveOpenModulesState(int nMode) 
{
	int nCount=0;
	CValue cValList;
	cValList.CreateObject("Массив");

	POSITION pos;
	HTREEITEM key;
	CDocList* pa;
	if(nMode)
	for( pos = metaDocList.GetStartPosition(); pos != NULL; )
	{
		metaDocList.GetNextAssoc( pos, key, pa);
		if(pa)
		{
			CMapStringToPtr &aList=pa->aList;
			POSITION pos2;
			CString csKey; 
			CDocument* pCurrentDoc;
			for( pos2 = aList.GetStartPosition(); pos2 != NULL; )
			{
				aList.GetNextAssoc( pos2, csKey, (void*&)pCurrentDoc);
				if(pCurrentDoc)
				{
					CRuntimeClass *Run=pCurrentDoc->GetRuntimeClass();
					if(CString(Run->m_lpszClassName)==CString("CModuleDoc"))
					{
						CModuleDoc *pMDoc=(CModuleDoc *)pCurrentDoc;
						CMetaObject *m_MetaObj=pMDoc->GetObj();
						CModuleView *pModule=pMDoc->GetModule();
						if(pModule)
						{
							nCount++;
							CString csModuleName=GetPath(m_MetaObj->nTempID);
							CValue Val;
							Val.CreateObject("Структура");
							Val.SetAt("Module",String(csModuleName));
							Val.SetAt("Line",pModule->GetCursorPos().y);
							//Val.SetAt("Активный",int(pModule==pModule->GetFocus()));
							cValList.SetAt(nCount,Val);
						}
					}
				}
			}
		}
	}

	ValueToFile(&cValList,FILE_LIST_NAME);

}
void CMetadataTree::LoadOpenModulesState() 
{
	CValue cValList=ValueFromFile(FILE_LIST_NAME);
	for(int i=1;i<=cValList.GetSizeArray();i++)
	{
		CValue Val=cValList.GetAt(i);
		if(Val.GetSizeStrArray())
		{
			CString csModuleName=Val.GetAt("Module");
			int nLine=Val.GetAt("Line");
			EditModule(csModuleName,nLine+1,0);
		}
	}
}

void CMetadataTree::OnAllFind() 
{
	CFindTextDialog dlg;
	int nRes=dlg.DoModal();
	if(IDOK==nRes)
	{
		AllFind(dlg.m_csFind,dlg.m_bMatchCase,0);
	}	
}

void CMetadataTree::AllFind(CString csFind,int nCase,int nWorld) 
{
	if(AfxGetModuleManager())
	{
		CMetadata *pMetadata=AfxGetModuleManager()->pMetadata;
		if(pMetadata)
		{
			GetAllData();
			pMetadata->AllFind(csFind,nCase,nWorld);
			pMessageWindow->SetFocus();
		}
	}
}
