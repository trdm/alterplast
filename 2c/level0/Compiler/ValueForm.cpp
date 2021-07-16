// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueForm.cpp: implementation of the CValueForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueForm.h"
#include "../FormUnit.h"
#include "../DialogUnit.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueForm,CValueControl);

CMethods CValueForm::Methods;
CValueForm::CValueForm()
{
	nType=100;
	bMicroForm=0;
	bIsView=0;
}

CValueForm::~CValueForm()
{

}
CString CValueForm::GetTypeString(void)const
{
	return "Форма";

}

#define PWINDOW ((CWnd *)pWnd)
void CValueForm::Attach(void *pObj)
{
	pWnd=(CObject*)pObj;

	if(!pObj)
		return;
	if(PWINDOW->IsKindOf(RUNTIME_CLASS(CView)))
		bIsView=1;
	else
		bIsView=0;
}
CValue CValueForm::GetValue(int)
{
	return ValueByRef(this);//агрегатный объект - создаем новую переменную-ссылку
}
int CValueForm::Modify(int nSet)
{
	int nRet=0;
	if(bIsView)
	{
		nRet=((CFormUnit*)pWnd)->bModified;
		if(nSet!=-1)
			((CFormUnit*)pWnd)->bModified=nSet;
	}
	else
	{
		nRet=((CDialogUnit*)pWnd)->bModified;
		if(nSet!=-1)
			((CDialogUnit*)pWnd)->bModified=nSet;
	}

	return  nRet;
}


CString CValueForm::Caption()
{
	if(bIsView)
		return ((CView*)pWnd)->GetDocument()->GetTitle();
	else
		return CValueControl::Caption();
};
void CValueForm::Caption(CString Str)
{
	if(bIsView)
		((CView *)pWnd)->GetDocument()->SetTitle(Str);
	else
		CValueControl::Caption(Str);
};
void CValueForm::Close()
{
	if(pWnd)
	{
		afxFormParam=vParam;
		PWINDOW->SendMessage(WM_CLOSEFORM);
	}
};



//******************************
//Работа как агрегатного объекта
//******************************
enum
{
	enVisible=0,
	enEnable,
	enColor,
	enCaption,
	enEnableEdit,
	enParam,
	enClose,
	enModif,
	enGetAttribut,
	enSetAttribut,
	enGetAttributCount,
	enGetAttributName,
	enRefresh,
	enRefresh2,

	enTabCtrlState,
	enUseLayer,

	enAddPropertyMicroForm,
	enGetPropertyMicroForm,

	enDefButton,

	enToolBar,
	enActiveControl,
	enModalMode,
	enChoiceMode,
	enMakeChoice,

	enFilePath,
	enModuleText,
	enFormText,
	enDescriptionText,
	enLineSelect,
	enCurrentValue,
	
	enLastMethod
};
void CValueForm::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"Visible","Видимость","Видимость(Флаг)"},
		{"Enable","Доступность","Доступность(Флаг)"},
		{"Color","Цвет"},
		{"Caption","Заголовок","Заголовок(Значение)"},
		{"EnableEdit","Редактирование","Редактирование(Флаг)"},
		{"Param","Параметр"},
		{"Close","Закрыть","Закрыть()"},
		{"Modify","Модифицированность","Модифицированность(Флаг=\"\")"},
		{"GetAttribut","ПолучитьАтрибут","ПолучитьАтрибут(ИмяАтрибута)"},
		{"SetAttribut","УстановитьАтрибут","УстановитьАтрибут(ИмяАтрибута,Значение)"},
		{"GetAttributCount","ПолучитьКоличествоАтрибутов","ПолучитьКоличествоАтрибутов()"},
		{"GetAttributName","ПолучитьИмяАтрибута","ПолучитьИмяАтрибута(Номер) - где: Номер от 1 до ПолучитьКоличествоАтрибутов()"},
		{"Refresh","Обновить","Обновить()"},
		{"RefreshExpr","ОбновитьВыражения","ОбновитьВыражения()"},

		{"TabCtrlState","ИспользоватьЗакладки","ИспользоватьЗакладки(Флаг)"},
		{"UseLayer","ИспользоватьСлой","ИспользоватьСлой(ИмяСлоя,Режим)"},


		{"AddPropertyMicroForm","ДобавитьСвойствоМикроформы","ДобавитьСвойствоМикроформы(ИмяСвойства,ЗначПоУмолч,СтрТип,Длина)"},
		{"GetPropertyMicroForm","ПолучитьСвойствоМикроформы","ПолучитьСвойствоМикроформы(ИмяСвойства)"},


		{"DefButton","КнопкаПоУмолчанию","КнопкаПоУмолчанию(Идентификатор)"},
		{"ToolBar","ПанельИнструментов"},//TODO

		{"ActiveControl","АктивныйЭлемент","АктивныйЭлемент(Идентификатор=\"\")"},
		{"ModalMode","МодальныйРежим","МодальныйРежим()"},
		{"ChoiceMode","РежимВыбора","РежимВыбора(Флаг=\"\")"},
		{"MakeChoice","ВыполнитьВыбор","ВыполнитьВыбор(Значение)"},

		{"FilePath","РасположениеФайла","РасположениеФайла()"},
		{"ModuleText","ТекстМодуля","ТекстМодуля()"},
		{"FormText","ТекстФормы","ТекстФормы()"},
		{"DescriptionText","ТекстОписания","ТекстОписания()"},
		
		{"ProcessSelectLine","ОбработкаВыбораСтроки","ОбработкаВыбораСтроки(Флаг=\"\")"},

		{"CurrentValue","ТекущийЭлемент","ТекущийЭлемент(Значение=\"\")"},
		
		
		


	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

CValue CValueForm::Method(int iName,CValue **p)
{
	if(!pWnd)
		Error("Не было привязки окна CWnd!");
	CValue Ret;
	switch(iName)
	{
		case enVisible:
			{
				Ret=Visible();
				if(p[0]->nType!=TYPE_EMPTY)
					Visible(p[0]->GetNumber());
				break;
			}
		case enEnable:
			{
				Ret=Enable();
				if(p[0]->nType!=TYPE_EMPTY)
					Enable(p[0]->GetNumber());
				break;
			}
		case enColor:
			{
				Ret=Color();
				if(p[0]->nType!=TYPE_EMPTY)
					Color(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetNumber());
				break;
			}
		case enCaption:
			{
				Ret=String(Caption());
				if(p[0]->nType!=TYPE_EMPTY)
					Caption(p[0]->GetString());
				break;
			}
		case enEnableEdit:
			{
				Ret=EnableEdit();
				if(p[0]->nType!=TYPE_EMPTY)
					EnableEdit(p[0]->GetNumber());
				break;
			}
		case enClose:
			{
				Close();
				break;
			}
		case enModif:
			{
				int nSet=-1;
				if(p[0]->nType!=TYPE_EMPTY)
					nSet=p[0]->GetNumber();
				return Modify(nSet);
			}
		case enGetAttribut:
			{
				int n=FindAttribute(p[0]->GetString());
				if(n<0)
					Error(CString("Атрибут \"")+p[0]->GetString()+"\" не найден");
				Ret=GetAttribute(n);
				break;
			}
		case enSetAttribut:
			{
				int n=FindAttribute(p[0]->GetString());
				if(n<0)
					Error(CString("Атрибут \"")+p[0]->GetString()+"\" не найден");
				SetAttribute(n,*p[1]);
				break;
			}
		case enGetAttributCount:
			{
				Ret=GetNAttributes();
				break;
			}
		case enGetAttributName:
			{
				Ret=String(GetAttributeName(p[0]->GetNumber()-1));
				break;
			}
		case enRefresh:
			{
				int nMode=p[0]->GetNumber();
				if(nMode)
				{
					Modify(1);
				}

				ASSERT(pWnd);
				((CWnd*)pWnd)->RedrawWindow();
				break;
			}
		case enRefresh2:
			{
				PWINDOW->PostMessage(WM_REFRESHEXPR);
/*
				if(bIsView)
				{
					((CFormUnit*)pWnd)->RefreshExpr();
				}
				else
				{
					((CDialogUnit*)pWnd)->RefreshExpr();
				}*/
				break;
			}
		case enTabCtrlState:
			{
				int nMode=p[0]->GetNumber();
				if(bIsView)
				{
					((CFormUnit*)pWnd)->UseTabCtrl(nMode);
				}
				else
				{
					((CDialogUnit*)pWnd)->UseTabCtrl(nMode);
				}
				vLayers.CallFunction("Видимость",p[0]);
				return 0;
			}
		case enUseLayer:
			{
				if(!p[1]->nType)
					*p[1]=2;
				return vLayers.CallFunction("ТекущаяСтрока",p[0],p[1]);
			}

		case enAddPropertyMicroForm:
			{
				CString csName=p[0]->GetString();
				CValue vDefaultValue=*p[1];
				CValue vType=*p[2];
				CValue vLength=*p[3];

				CValue vValue;
				if(bIsView)
				{
					vValue=((CFormUnit*)pWnd)->vCurrentValue;
				}
				else
				{
					vValue=((CDialogUnit*)pWnd)->vCurrentValue;
				}


				if(vValue.GetSizeStrArray()>0)
				{
					CValue vMicroForm=vValue.GetAt("Микроформа");
					if(vMicroForm.GetSizeStrArray()>0)
					{
						CValue vValue=vMicroForm.GetAt(csName);
						if(vValue.IsEmpty())
							vValue=vDefaultValue;
						CValue vElement;
						vElement.CreateObject("Структура");
						vElement.SetAt("Тип",vType);
						vElement.SetAt("Длина",vLength);
						vElement.SetAt("Значение",vValue);
						vMicroForm.SetAt(csName,vElement);
						return 1;
					}
				}
				return 0;
			}

		case enGetPropertyMicroForm:
			{
				CString csName=p[0]->GetString();

				CValue vValue;
				if(bIsView)
				{
					vValue=((CFormUnit*)pWnd)->vCurrentValue;
				}
				else
				{
					vValue=((CDialogUnit*)pWnd)->vCurrentValue;
				}

				if(vValue.GetSizeStrArray()>0)
				{
					CValue vMicroForm=vValue.GetAt("Микроформа");
					if(vMicroForm.GetSizeStrArray()>0)
					{
						CValue vProperty=vMicroForm.GetAt(csName);
						return vProperty;
					}
				}
				break;
			}

		case enDefButton:
			{
				if(bIsView)
				{
					return String(((CFormUnit*)pWnd)->DefButton(p[0]->GetString()));
				}
				else
				{
					return String(((CDialogUnit*)pWnd)->DefButton(p[0]->GetString()));
				}
				break;
			}

		case enToolBar://TODO
			{
				break;
			}


		case enActiveControl:
			{
				if(bIsView)
				{
					return String(((CFormUnit*)pWnd)->ActiveControl(p[0]->GetString()));
				}
				else
				{
					return String(((CDialogUnit*)pWnd)->ActiveControl(p[0]->GetString()));
				}
				break;
			}

		case enModalMode:
			{
				Ret=0;
				if(pWnd)
				 if((((CWnd*)pWnd)->GetExStyle()& (WS_EX_DLGMODALFRAME)) != 0)
					 Ret=1;

				break;
			}
		case enChoiceMode:
			{
				if(bIsView)
				{
					Ret=((CFormUnit*)pWnd)->nChoiceMode;
					if(p[0]->nType)
						((CFormUnit*)pWnd)->nChoiceMode=p[0]->GetNumber();
				}
				else
				{
					Ret=((CDialogUnit*)pWnd)->nChoiceMode;
					if(p[0]->nType)
						((CDialogUnit*)pWnd)->nChoiceMode=p[0]->GetNumber();
				}
				break;
			}


		case enMakeChoice:
			{
				if(bIsView)
				{
					return ((CFormUnit*)pWnd)->MakeChoice(*p[0]);
				}
				else
				{
					return ((CDialogUnit*)pWnd)->MakeChoice(*p[0]);
				}
				break;
			}

		case enFilePath:
			{
				CString csPath;
				if(bIsView)
				{
					csPath = ((CFormUnit*)pWnd)->csPath;
				}
				else
				{
					csPath = ((CDialogUnit*)pWnd)->csPath;
				}

				CString csDir;
				CString csFile=csPath;
				int nIndex=csPath.ReverseFind('\\');
				if(nIndex>=0)
				{
					csDir=csPath.Left(nIndex+1);
					csFile=csPath.Mid(nIndex+1);
				}
				p[0]->SetString(csDir);
				p[1]->SetString(csFile);
				return String(csPath);
			}

		case enModuleText:
			{
				if(bIsView)
				{
					return String(((CFormUnit*)pWnd)->csModule);
				}
				else
				{
					return String(((CDialogUnit*)pWnd)->csModule);
				}
				break;
			}

		case enFormText:
			{
				if(bIsView)
				{
					return String(((CFormUnit*)pWnd)->csForm);
				}
				else
				{
					return String(((CDialogUnit*)pWnd)->csForm);
				}
				break;
			}

		case enDescriptionText:
			{
				if(bIsView)
				{
					return String(((CFormUnit*)pWnd)->csDescription);
				}
				else
				{
					return String(((CDialogUnit*)pWnd)->csDescription);
				}
				break;
			}
		
		case enLineSelect:
			{
				if(bIsView)
				{
					Ret=((CFormUnit*)pWnd)->nLineSelect;
					if(p[0]->nType)
						((CFormUnit*)pWnd)->nLineSelect=p[0]->GetNumber();
				}
				else
				{
					Ret=((CDialogUnit*)pWnd)->nLineSelect;
					if(p[0]->nType)
						((CDialogUnit*)pWnd)->nLineSelect=p[0]->GetNumber();
				}
				break;
			}

		case enCurrentValue:
			{
				if(bIsView)
				{
					Ret=((CFormUnit*)pWnd)->vCurrentValue;
					if(p[0]->nType)
						((CFormUnit*)pWnd)->vCurrentValue=*p[0];
				}
				else
				{
					Ret=((CDialogUnit*)pWnd)->vCurrentValue;
					if(p[0]->nType)
						((CDialogUnit*)pWnd)->vCurrentValue=*p[0];
				}
				break;
			}


	}
	return Ret;
}

CValue CValueForm::GetAttribute(int iName)
{
	if(pWnd)
	{
		DefValueList *pValueList;

		if(bIsView)
		{
			pValueList=&((CFormUnit*)pWnd)->aValueList;
		}
		else
		{
			pValueList=&((CDialogUnit*)pWnd)->aValueList;
		}

		if(iName==pValueList->GetSize()-1)//Параметр
			return	vParam;
		else
		if(iName==pValueList->GetSize())//Закладки
			return vLayers;
		else
		if(iName>pValueList->GetSize())
			return CValue();
		else
		{
			CValue *p=pValueList->GetAt(iName);
			return *p;
		}
	}
	return CValue();
}
void CValueForm::SetAttribute(int iName,CValue &Val)
{
	if(pWnd)
	{
		DefValueList *pValueList;

		if(bIsView)
		{
			pValueList=&((CFormUnit*)pWnd)->aValueList;
		}
		else
		{
			pValueList=&((CDialogUnit*)pWnd)->aValueList;
		}

		if(iName<pValueList->GetSize()-1)
			pValueList->GetAt(iName)->SetValue(Val);
		if(iName=pValueList->GetSize()-1)
		{
			pValueList->GetAt(iName)->SetValue(Val);
			vParam=Val;
		}
	}
}

int  CValueForm::FindAttribute(CString csName)
{
	if(pWnd)
	{
		DefNameList *pNameList;

		if(bIsView)
		{
			pNameList=&((CFormUnit*)pWnd)->aNameList;
		}
		else
		{
			pNameList=&((CDialogUnit*)pWnd)->aNameList;
		}

		csName=mUpper(csName);
		if(csName=="ПАРАМЕТР"||csName=="PARAMETER")
			return pNameList->GetSize()-1;
		else
		if(csName=="ЗАКЛАДКИ"||csName=="LAYERS")
			return pNameList->GetSize();
		for(int i=0;i<pNameList->GetSize()-1;i++)
		{
			if(mUpper(pNameList->GetAt(i))==csName)
				return i;
		}
	}
	return -1;
}
CString CValueForm::GetAttributeName(int iName,int nAlias)
{
	if(pWnd)
	{
		DefNameList *pNameList;

		if(bIsView)
		{
			pNameList=&((CFormUnit*)pWnd)->aNameList;
		}
		else
		{
			pNameList=&((CDialogUnit*)pWnd)->aNameList;
		}

		if(iName==pNameList->GetSize()-1)//Параметр
		{
			if(nAlias==0)
				return	"Parameter";
			else
				return	"Параметр";
		}
		else
		if(iName==pNameList->GetSize())//Закладки
		{
			if(nAlias==0)
				return	"Layers";
			else
				return	"Закладки";
		}
		else
		{
			return pNameList->GetAt(iName);
		}
	}
	return "";
}
int  CValueForm::GetNAttributes(void)
{
	if(pWnd)
	{
		DefNameList *pNameList;

		if(bIsView)
		{
			pNameList=&((CFormUnit*)pWnd)->aNameList;
		}
		else
		{
			pNameList=&((CDialogUnit*)pWnd)->aNameList;
		}

		return pNameList->GetSize();
	}
	return 0;
}

