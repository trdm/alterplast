// јвторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueTreeCtrl.cpp: implementation of the CValueTreeCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueTreeCtrl.h"
#include "../MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CMethods CValueTreeCtrl::Methods;

#define pTree ((CTreeCtrl *)pWnd)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueTreeCtrl,CValue);

CValueTreeCtrl::CValueTreeCtrl()
{
	pWnd=0;
	pMetaTree=0;
	m_bUseCheck=0;
	hSelectParentItem=0;
	hSelectCurrentItem=0;
	nType=100;
}

CValueTreeCtrl::~CValueTreeCtrl()
{

}

//******************************
//–абота как агрегатного объекта
//******************************
enum
{
	enInsertItem=0,
	enDeleteItem,
	enSetItemText,
	enGetItemText,
	enSetItemData,
	enGetItemData,
	enSetItemImage,
	enGetItemImage,
	enEnableCheckBox,
	enSetCheck,
	enGetCheck,
	enGetRootItem,
	enGetNextItem,
	enGetParentItem,
	enGetChildItem,
	enExpand,
	enSelectChildItem,
	enGetItem,
	enCurrentItem,
	enSelItem,
	enBold,
	enDeleteAllItems,
	enIsGroup,
	enEditText,
	enAttachTree,
	enSetTypeObject,
	enSetItemData2,
	enSetItemImage2,
	enRedrawWindow,
	enRefresh,

	enSetMetaEdit,
	enSetMetaGroup,
	enSetMetaText,
	enSetMetaAtribut,
	enSetMetaModule,
	enSetMetaForm,
	enSetMetaMaket,
	enSetMetaFormule,
	enSetMetaPicture,
//	enSetMetaParam,

	enSelectItem,

	enVisible,
	enEnable,
	enFocus,
	enSetControlRect,
	enGetControlRect,

};

void CValueTreeCtrl::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"InsertItem","¬ставитьЁлемент","¬ставитьЁлемент(—тр“екст,Ќом артинки,Ёлемент–одитель)"},
		{"DeleteItem","”далитьЁлемент","”далитьЁлемент(Ёлемент)"},
		{"SetItemText","”становить“екст","”становить“екст(Ёлемент,—тр“екст)"},
		{"GetItemText","ѕолучить“екст"},
		{"SetItemData","«адатьƒанные"},
		{"GetItemData","ѕолучитьƒанные"},
		{"SetItemImage","«адать артинку"},
		{"GetItemImage","ѕолучить артинку"},
		{"EnableCheckBox","»спользовать‘лаги"},
		{"SetCheck","”становить‘лаг"},
		{"GetCheck","ѕолучить‘лаг"},
		{"GetRootItem"," орневойЁлемент"},
		{"GetNextItem","ѕолучить—ледующий"},
		{"GetParentItem","ѕолучить–одител€"},
		{"GetChildItem","ѕолучитьѕодчиненный"},
		{"Expand","–азворот"},
		{"SelectChildItem","¬ыбратьѕодчиненныеЁлементы"},
		{"GetItem","ѕолучитьЁлемент"},
		{"CurrentItem","“екущийЁлемент"},
		{"SelItem","¬ыделенныйЁлемент"},
		{"Bold","∆ирность"},
		{"DeleteAllItems","”далить¬се"},
		{"IsGroup","Ёто√руппа"},
		{"EditText","–едактировать“екст"},
		{"AttachTree","ѕодключитьƒерево"},
		{"SetTypeObject","”становить“ипќбъекта"},
		{"SetItemData","”становитьƒанные"},
		{"SetItemImage","”становить артинку"},
		{"RedrawWindow","ѕерерисоватьќкно"},
		{"Refresh","ќбновить"},

		{"MetaEdit","ћета–едактирование","ћета–едактирование(Ёлемент,‘лаг)"},
		{"MetaGroup","ћета√руппа","ћета√руппа(Ёлемент,‘лаг,Ќом артинкиѕодчЁлементов)"},
		{"MetaText","ћета“екст","ћета“екст(Ёлемент)"},
		{"MetaAttribute","ћетајтрибут","ћетајтрибут(Ёлемент)"},
		{"MetaModule","ћетаћодуль","ћетаћодуль(Ёлемент)"},
		{"MetaForm","ћета‘орма","ћета‘орма(Ёлемент)"},
		{"MetaTable","ћета“аблица","ћета“аблица(Ёлемент)"},
		{"MetaFormule","ћета‘ормула","ћета‘ормула(Ёлемент,—тр»м€‘ункции,ѕараметр)"},
		{"MetaItemPicture","ћета артинкаЁлемента","ћета артинкаЁлемента(Ёлемент,Ќом артинки)"},
//		{"MetaParam","ћетаѕараметр","ћетаѕараметр(Ёлемент,«начение)"},
		
		{"SelectItem","¬ыбратьЁлементы"},

		{"Visible","¬идимость"},
		{"Enable","ƒоступность"},
		{"Activate","јктивизировать"},
		{"SetControlRect","”становить оординаты"},
		{"GetControlRect","ѕолучить оординаты"},

	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}


CValue CValueTreeCtrl::Method(int iName,CValue **p)
{


	if(iName!=enAttachTree)
	if(!pWnd)
		Error("ќбъект не инициализирован!");

	HTREEITEM htree=(HTREEITEM)(int)p[0]->GetNumber();
	if(!htree)
		htree=hSelectCurrentItem;

	CValue Ret;
	switch(iName)
	{
		case enRedrawWindow:
		case enRefresh:
			{
				pTree->RedrawWindow();
				break;
			}
		case enSetTypeObject:
			{
				if(!pMetaTree)
					Error("ƒанный экземпл€р не поддерживает работу в режиме задани€ типа объекта");
				HTREEITEM CurItem = htree;
				CObjectData data;

				data.bEdit=p[1]->GetNumber();
				data.bGroup=p[2]->GetNumber();
				data.nType=p[3]->GetNumber();
				data.nChildImage=p[4]->GetNumber();
				if(p[3]->GetType()==TYPE_STRING)
				{
					data.nType=OBJ_RUN;
					data.csFunction=p[3]->GetString();
					data.pRun=afxCurrentRunModule;
					data.vData=*p[5];
				}

				pMetaTree->dataMetaObj[CurItem]=data;

				break;
			}

//то же самое, но отдельными методами
#define DEF_META_BEGIN()\
{\
if(!pMetaTree)\
	Error("ƒанный экземпл€р не поддерживает работу в режиме задани€ типа объекта");\
HTREEITEM CurItem = htree;\
CObjectData data=pMetaTree->dataMetaObj[CurItem];\

#define DEF_META_END()\
pMetaTree->dataMetaObj[CurItem]=data;\
break;\
}

		case enSetMetaEdit:
			DEF_META_BEGIN()
			data.bEdit=TYPE_EMPTY==p[1]->nType?1:p[1]->GetNumber();
			DEF_META_END()

		case enSetMetaGroup:
			DEF_META_BEGIN()
				data.bGroup=TYPE_EMPTY==p[1]->nType?1:p[1]->GetNumber();
				data.nChildImage=p[2]->GetNumber();
			DEF_META_END()
		case enSetMetaText:
			DEF_META_BEGIN()
			data.nType=OBJ_TEXT;
			DEF_META_END()
		case enSetMetaAtribut:
			DEF_META_BEGIN()
			data.nType=OBJ_ATTR;
			DEF_META_END()
		case enSetMetaModule:
			DEF_META_BEGIN()
			data.nType=OBJ_MODULE;
			DEF_META_END()
		case enSetMetaForm:
			DEF_META_BEGIN()
			data.nType=OBJ_FORM;
			DEF_META_END()
		case enSetMetaMaket:
			DEF_META_BEGIN()
			data.nType=OBJ_MAKET;
			DEF_META_END()
		case enSetMetaFormule:
			DEF_META_BEGIN()
			data.nType=OBJ_RUN;
			data.csFunction=p[1]->GetString();
			data.pRun=afxCurrentRunModule;
			data.vData=*p[2];
			DEF_META_END()
		case enSetMetaPicture:
			DEF_META_BEGIN()
			data.nChildImage=p[1]->GetNumber();
			DEF_META_END()
/*		case enSetMetaParam:
			DEF_META_BEGIN()
			data.vData=*p[1];
			DEF_META_END()

*/



		case enInsertItem:
			{
				HTREEITEM CurItem= pTree->InsertItem(p[0]->GetString(),p[1]->GetNumber(),p[1]->GetNumber(),(HTREEITEM)(int)p[2]->GetNumber());
				Ret=(int)CurItem;
				if(!m_bUseCheck)
					SetNonChecked((HTREEITEM)CurItem);
				break;
			}
		case enDeleteItem:
			{
				if(hSelectCurrentItem==htree)//удал€етс€ текущий элемент
				{
					hSelectParentItem=(HTREEITEM)1;//признак что текущий элемент уже выбран (т.е. его нужно только возвратить пользователю)
					hSelectCurrentItem=pTree->GetNextSiblingItem(hSelectCurrentItem);
				}
				Ret=(int) pTree->DeleteItem(htree);
				break;
			}
		case enSetItemText:
			{
				Ret=(int) pTree->SetItemText(htree,p[1]->GetString());
				break;
			}
		case enGetItemText:
			{
				Ret=String(pTree->GetItemText(htree));
				break;
			}
		case enSetItemData:
		case enSetItemData2:
			{
				if(!htree)
					htree=hSelectCurrentItem;
				Ret=(int) pTree->SetItemData(htree,(int)p[1]->GetNumber());
				break;
			}
		case enGetItemData:
			{
				Ret=(int) pTree->GetItemData(htree);
				break;
			}
		case enSetItemImage:
		case enSetItemImage2:
			{
				Ret=(int) pTree->SetItemImage(htree,(int)p[1]->GetNumber(),(int)p[1]->GetNumber());
				break;
			}
		case enGetItemImage:
			{
				int n1,n2;
				pTree->GetItemImage(htree,n1,n2);
				Ret=(int) n1;
				break;
			}

		case enEnableCheckBox:
			{
				Ret=(int)m_bUseCheck;
				m_bUseCheck=(int)Ret.GetNumber();
				break;
			}
		case enSetCheck:
			{
				pTree->SetCheck(htree,(int)p[1]->GetNumber()); 
				break;
			}
		case enGetCheck:
			{
				Ret=(int)pTree->GetCheck(htree); 
				break;
			}
		case enGetRootItem:
			{
				Ret=(int) pTree->GetRootItem();
				break;
			}
		case enGetNextItem:
			{
				Ret=(int) pTree->GetNextSiblingItem(htree);
				break;
			}
		case enGetParentItem:
			{
				Ret=(int) pTree->GetParentItem(htree);
				break;
			}
		case enGetChildItem:
			{
				Ret=(int) pTree->GetChildItem(htree);
				break;
			}
		case enIsGroup:
			{
				int n=(int) pTree->GetChildItem(htree);
				if(n)
					Ret=1;
				else
					Ret=0;
				break;
			}
		
		case enExpand:
			{
				UINT nCode=TVE_COLLAPSE;
				if(p[0]->GetNumber())
					nCode=TVE_EXPAND;
				Ret=(int) pTree->Expand(htree,nCode);
				break;
			}
		case enSelectItem://¬ыбратьЁлементы
			{
				if(!htree)//иначе = ¬ыбратьѕодчиненныеЁлементы
				{
					hSelectParentItem=(HTREEITEM)1;//признак что текущий элемент уже выбран (т.е. его нужно только возвратить пользователю)
					hSelectCurrentItem=pTree->GetRootItem();
					if(hSelectCurrentItem)
						Ret=1;
					else
						Ret=0;
					break;
				}
			}
		case enSelectChildItem://¬ыбратьѕодчиненныеЁлементы
			{
				hSelectParentItem=htree;
				hSelectCurrentItem=0;
				int n=(int) pTree->GetChildItem(htree);
				if(n)
					Ret=1;
				else
					Ret=0;
				break;
			}
		case enGetItem:
			{
				if(!hSelectParentItem)
					Error("Ќе было выбора родительского элемента дерева");
				if(!hSelectCurrentItem)
					hSelectCurrentItem=pTree->GetChildItem(hSelectParentItem);
				else
				{
					if(hSelectParentItem==(HTREEITEM)1)
						hSelectParentItem=(HTREEITEM)2;//тек. элемент и так уже установлен
					else
						hSelectCurrentItem=pTree->GetNextSiblingItem(hSelectCurrentItem);
				}
				if(hSelectCurrentItem)
					Ret=1;
				else
					Ret=0;
				break;
			}
		case enCurrentItem:
			{
				Ret=(int)hSelectCurrentItem;
				break;
			}
		case enSelItem:
			{
				Ret=(int)pTree->GetSelectedItem();
				if(p[0]->nType!=TYPE_EMPTY)
				{
					pTree->SelectItem((HTREEITEM)(int)p[0]->GetNumber()); 
					pTree->SetFocus();
				}
				break;
			}
		case enBold:
			{
				Ret=(int)GetBold(htree);
				if(p[0]->nType!=TYPE_EMPTY)
				{
					SetBold(htree,(int)p[1]->GetNumber()); 
				}
				break;
			}
		case enDeleteAllItems:
			{
				Ret=(int)pTree->DeleteAllItems();
				break;
			}
		case enEditText:
			{
				HTREEITEM CurItem=pTree->GetSelectedItem();
				if(CurItem)
				{
					Ret=1;
					pTree->EditLabel(CurItem);
				}
				else
				{
					Ret=0;
				}
				break;
			}
		case enAttachTree:
			{
				if(p[0]->GetTypeString()==GetTypeString())
					pWnd=((CValueTreeCtrl*)p[0]->pRef)->pWnd;
				break;
			}

		case enVisible:
			{
				if(!pWnd)
					return 0;
				Ret=Visible();
				if(p[0]->nType!=TYPE_EMPTY)
					Visible(p[0]->GetNumber());
				return Ret;
			}
		case enEnable:
			{
				if(!pWnd)
					return 0;
				Ret=Enable();
				if(p[0]->nType!=TYPE_EMPTY)
					Enable(p[0]->GetNumber());
				return Ret;
			}
		case enFocus:
			{
				if(!pWnd)
					return 0;
				((CWnd*)pWnd)->SetFocus();
			}
		case enSetControlRect:
			{
				int x, y, nWidth, nHeight;
				x=p[0]->GetNumber();
				y=p[1]->GetNumber();
				nWidth=p[2]->GetNumber();
				nHeight=p[3]->GetNumber();
				ASSERT(pWnd);
				((CWnd*)pWnd)->MoveWindow(x, y, nWidth, nHeight,1);
				((CWnd*)pWnd)->RedrawWindow();
				break;
			}
		case enGetControlRect:
			{
				CRect mRect(0,0,0,0);
				ASSERT(pWnd);
				((CWnd*)pWnd)->GetWindowRect(&mRect);
				CWnd* pParentWnd=((CWnd*)pWnd)->GetParent();
				pParentWnd->ScreenToClient(&mRect);
				*p[0] = CValue(mRect.left);
				*p[1] = CValue(mRect.top);
				*p[2] = CValue(mRect.right - mRect.left);
				*p[3] = CValue(mRect.bottom - mRect.top);
				break;
			}	
	}
	return Ret;
}

void CValueTreeCtrl::SetNonChecked(HTREEITEM CurItem)
{
    if(CurItem)
    {
        pTree->SetCheck(CurItem,FALSE);
        pTree->SetItemState(CurItem,1,0x1000);
    }
}

bool CValueTreeCtrl::IsCheckable(HTREEITEM CurItem)
{
    if(CurItem)
		return pTree->GetItemState(CurItem,0x1000)>4000;
	else
		return false;
}

void CValueTreeCtrl::SetBold(HTREEITEM CurItem,int bState)
{
	if(bState)
		pTree->SetItemState(CurItem,TVIS_BOLD,TVIS_BOLD);
	else
		pTree->SetItemState(CurItem,0,TVIS_BOLD);
}

#define GETITEMSTATE(x,y) int(pTree->GetItemState(x,0xFFFFFFFF)&y)
int CValueTreeCtrl::GetBold(HTREEITEM CurItem)
{
	return (GETITEMSTATE(CurItem,TVIS_BOLD)==TVIS_BOLD);
}

