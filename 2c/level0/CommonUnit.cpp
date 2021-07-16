//Авторские права - VTOOLS.RU (info@vtools.ru)
#include "stdafx.h"
#include "resource.h"
#include "Compiler/ValueGrid.h"
#include "Compiler/ValueContext.h"
#include "Compiler/Definition.h"
#include "Compiler/ValueListBox.h"
//#include "IconListBox.h"
#include "Compiler/ValueListLayers.h"
#include "MainFrm.h"
//#include "MicroForm.h"

 
//BOOL GrantCheck(CString csPathName);
int RegisterEvent(CString csEventName);
int NotifyEvent(CString csEventName, CValue **p);

#ifdef _DEBUG
#undef THIS_FILE
#define new DEBUG_NEW 
#endif

#include "Compiler/ValueObject.h"

extern int afxEventMessage;
extern CString afxFormID;//уникальный ид открываемой формы
extern CMapStringToPtr aOpenForm;
extern BOOL afxSimpleErrorMessage;
extern CValue afxChoiceContext;//контекст, из которого открыт подбор
extern CValue afxCurrentValue;//текущее значение (для списка формы)


extern int afxChoiceMode;
extern CValue afxFormContext;//контекст открытой формы


extern BOOL bDebugLoop;
extern BOOL bDebugStopLine;
extern int nCurrentNumberStopContext;
extern BOOL bUseDebug;

extern CRect afxCurrentRect;//текущий координаты диалогового окна, который инициирует выборку из списка значений (CButton,CEdit)
extern CWnd	*afxCurrentWnd;
extern CImageList *GetImageList();


BOOL IsIdentifier(CString BUFFER);
#define TIME_DEBUG

#ifdef TIME_DEBUG
void OutTime(int n);
void InitTime();
#else
#define OutTime(x)
#define InitTime(x)
#endif


CMapStringToPtr CFormUnit::aOpenForm;

#ifdef CFormUnit
//определяет - целое это слово или нет
BOOL IsIdentifier(CString BUFFER)
{
	BOOL bRes=0;
	BUFFER.TrimRight();
	BUFFER.TrimLeft();
	BUFFER.MakeUpper();
	for(int i=0;i<BUFFER.GetLength();i++)
	{
		if(
		(BUFFER[i]=='_')||
		(BUFFER[i]>='A'&&BUFFER[i]<='Z')||
		(BUFFER[i]>='А'&&BUFFER[i]<='Я')||
		(BUFFER[i]>='0'&&BUFFER[i]<='9')
		)
			bRes=1;
		else
			return 0;
	}
	return bRes;
}

#ifdef TIME_DEBUG
UINT GetProcessorTimeMySQL(UINT nDiv);
int nFirstTime=0;
BOOL bUseDebugTime=0;
void InitTime()
{
	nFirstTime=GetProcessorTimeMySQL(10);
}
int GetDelta()
{
	int nDelata=GetProcessorTimeMySQL(10)-nFirstTime;
	InitTime();
	return nDelata;
}
void OutTime(int n)
{
	OutToDebug(" (CPU %d : %d)\n",n,GetDelta());
	InitTime();
}
#endif
#endif






/////////////////////////////////////////////////////////////////////////////
// CFormUnit

IMPLEMENT_DYNCREATE(CFormUnit, CPARENT)

void CFormUnit::Init()
{
	pRunModule=0;
	nModeCloseWindow=0;
	pModule=0;
	bDeleteModule=0;
	form_dx=100;
	form_dy=50;
	bCanOpen=1;
	bCanClose=0;
	m_bUseTabCtrl=0;
	nNumberTabCtrl=-1;
	bCanResize=1;
	pDefButton=0;
	nChoiceMode=0;
	pSetFocus=0;
	m_hIcon=0;
	nLineSelect=0;
	nRunCount=0;

}

CFormUnit::~CFormUnit()
{
	if(bDeleteModule&&pModule)
		delete pModule;
	nModeCloseWindow=2;
	pModule=0;

	for(int i=0;i<aElements.GetSize();i++)
	{
		CValue *pVal=&aElements[i];
		if(pVal->FindMethod("ДеИнициализацияОбъекта")>=0)
			pVal->CallFunction("ДеИнициализацияОбъекта");
		pVal->Detach();
	}
	ClearData();

	aOpenForm[csFormID]=0;
	vParentContext.Reset();
}

void CFormUnit::DoDataExchange(CDataExchange* pDX)
{
	CPARENT::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CFormUnit)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}

#define OnNotify(x)\
ON_CONTROL_RANGE(x, FORM_START_ID, FORM_FINISH_ID, OnMessage##x)

#define OnRangeNotify(x)\
ON_NOTIFY_RANGE(x, FORM_START_ID,  FORM_FINISH_ID, OnMessage##x)


BEGIN_MESSAGE_MAP(CFormUnit, CPARENT)
	//{{AFX_MSG_MAP(CFormUnit)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_MESSAGE(WM_CLOSEFORM,CloseForm)
	ON_MESSAGE(WM_CANDESTROY,CanDestroy)
	ON_MESSAGE(WM_DEFBUTTON,CallDefButton)
	ON_MESSAGE(WM_REFRESHEXPR,RefreshExpr)

	
	//}}AFX_MSG_MAP

	// Standard printing commands
#ifndef CFormUnit
	ON_COMMAND(ID_FILE_PRINT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CView::OnFilePrintPreview)
#endif

	OnNotify(BN_CLICKED)//Button,Check,Radio
	OnNotify(EN_CHANGE)//Edit
	OnNotify(CBN_SELENDOK)//ComboBox
	OnNotify(LBN_DBLCLK)//ListBox
	OnNotify(LBN_SELCHANGE)//ListBox
//	OnNotify(WM_DRAWITEM)

	OnRangeNotify(NM_CLICK)//CTreeCtrl

	OnRangeNotify(NM_RCLICK)//Grid
	
	OnRangeNotify(NM_DBLCLKGRID)//Grid
	OnRangeNotify(GVN_BEGINLABELEDIT)//Grid
	OnRangeNotify(GVN_ENDLABELEDIT)//Grid
	OnRangeNotify(GVN_SELCHANGING)//Grid
	OnRangeNotify(GVN_SELCHANGED)//Grid
	OnRangeNotify(GVN_BEGINDRAG)//Grid

	OnRangeNotify(NM_CUSTOMDRAW)//Slider
	OnRangeNotify(NM_RELEASEDCAPTURE)//Slider
	

	OnRangeNotify(WM_ONSELECT)//Выбор кнопки диалогового элемента

	OnRangeNotify(TVN_SELCHANGED)

	OnRangeNotify(TCN_SELCHANGE)//TabCtrl
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CFormUnit message handlers
void CFormUnit::CallFunction(CString csFunction,CValue vParam1,CValue vParam2,CValue vParam3)
{
	if(!pRunModule)
		return;
	try
	{
		pRunModule->CallFunction(csFunction,vParam1,vParam2,vParam3);
	}
	catch(CTranslateError *)
	{
		DoError();
		return;
	};
}

void CFormUnit::CallFunction2(CString csFunction,CValue &vParam1,CValue &vParam2,CValue &vParam3)
{
	if(!pRunModule)
		return;
	try
	{
		pRunModule->CallFunction(csFunction,vParam1,vParam2,vParam3);
	}
	catch(CTranslateError *)
	{
		DoError();
		return;
	};
}

void CFormUnit::OnDestroy() 
{


	CPARENT::OnDestroy();
#ifndef CFormUnit
	((CMainFrame*)AfxGetMainWnd())->m_wndDocSelector.RemoveButton( this );
#endif
}
void CFormUnit::ClearData() 
{
	int i;
	//if(pRunModule)
	//	delete pRunModule;
	pRunModule=0;
	for(i=0;i<aControlList.GetSize();i++)
	{
		CDynControl *pControl=aControlList[i];
		if(pControl->m_nControlType==ID_DYNBUTTONGRID)
			((CGridCtrl*)pControl->GetWnd())->SetCallbackFunc(0,0);

		delete pControl;
	}
	aControlList.RemoveAll();
	aControlListById.RemoveAll();

	aElements.RemoveAll();

	aNameList.RemoveAll();
	aValueList.RemoveAll();
}

//"Обратный вызов виртуального режима"
BOOL CALLBACK VIRUALFUNCTION(GV_DISPINFO *dispinfo, LPARAM lParam)
{
	if(bUseDebug)
	if(bDebugLoop)//||bDebugStopLine||nCurrentNumberStopContext)
		return 0;

	CCallBackInfo *pCallBack=(CCallBackInfo *)lParam;
	CFormUnit *pForm=pCallBack->pForm;
	if(!pForm)
		return 0;
	if(pForm->nModeCloseWindow==2)//форма закрывается поэтому события не обрабатываются
		return 0;
	pForm->nModeCloseWindow=1;

	CValue Val[10];
	Val[0]=dispinfo->item.row;
	Val[1]=dispinfo->item.col;
	Val[2]=dispinfo->item.Value;
	Val[3]=dispinfo->item.iImage;
	Val[4]=dispinfo->item.nFormat;
	Val[5]=dispinfo->item.crFgClr;
	Val[6]=dispinfo->item.crBkClr;

/*	static int nCount=0;
	nCount++;
	if(nCount%100==0)
		OutToDebug("nCount=%d\n",nCount);*/
	try
	{
		BOOL bCall=1;
		afxEventMessage=1;
		if(pCallBack->pControl->eventVal.nType!=TYPE_EMPTY)
		{
			if(pCallBack->pControl->eventVal.FindMethod("ОбработкаСобытия")>=0)
			{
				int nRes=pCallBack->pControl->eventVal.CallFunction("ОбработкаСобытия", &(CValue("Виртуальный режим")) ,&Val[0],&Val[1],&Val[2],&Val[3],&Val[4],&Val[5],&Val[6]);
				if(nRes)
					bCall=0;
			}
		}

		if(bCall && pForm->pRunModule)
			if(!pCallBack->csFunction.IsEmpty())
			{
				pForm->pRunModule->CallFunction(pCallBack->csFunction,Val[0],Val[1],Val[2],Val[3],Val[4],Val[5],Val[6]);
			}
		afxEventMessage=0;
	}
	catch(CTranslateError *)
	{
		DoError();
		afxEventMessage=0;
	};


	//dispinfo->item.row=Val[0].GetNumber();
	//dispinfo->item.col=Val[1].GetNumber();
	dispinfo->item.Value=Val[2];
	dispinfo->item.iImage=Val[3].GetNumber();
	dispinfo->item.nFormat=Val[4].GetNumber();
	dispinfo->item.crFgClr=Val[5].GetNumber();
	dispinfo->item.crBkClr=Val[6].GetNumber();

	if(pForm->nModeCloseWindow==2)
		pForm->CloseForm();
	else
		pForm->nModeCloseWindow=0;
	return TRUE;
}

#define FORM_DELTA_X	form_dx+10
#define FORM_DELTA_Y	form_dy+30



void CFormUnit::SetFormSize() 
{
#ifndef CFormUnit
	::SetWindowPos(GetParentFrame()->m_hWnd,0,0, 0, FORM_DELTA_X, FORM_DELTA_Y,SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOREDRAW);

	SIZE size;
	size.cx=form_dx-10;
	size.cy=form_dy-10;
	SetScrollSizes( MM_TEXT, size,size,size);
	GetParentFrame()->RecalcLayout();
#else
	::SetWindowPos(m_hWnd,0,0, 0, FORM_DELTA_X, FORM_DELTA_Y,SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOREDRAW);
#endif
}

BOOL CFormUnit::CreateForm() 
{
	vParentChoiceContext=afxChoiceContext;
	nChoiceMode=afxChoiceMode;
	vCurrentValue=afxCurrentValue;

	ClearData();
	pFormVal=new CValueForm();
	CValue AllElements;
	AllElements.LoadFromString(csForm);
	if(AllElements.GetSizeArray()>0)
	{
		CValue ObjectVal=AllElements.GetAt("Форма");
		if(ObjectVal.GetSizeStrArray()>0)
		{
			csFormTitle=ObjectVal.GetAt("Заголовок").GetString();
			bCanResize=1-ObjectVal.GetAt("НеИзменятьРазмер").GetNumber();

			//csAlias=ObjectVal.GetAt("Синоним");
			//csComment=ObjectVal.GetAt("Комментарий");

			//запоминаем начальную ширину и высоту
			form_dx=ObjectVal.GetAt("x2");
			form_dy=ObjectVal.GetAt("y2");


			CImageList *pImageList=GetImageList();
			int nImage=ObjectVal.GetAt("Иконка").GetNumber();
			if(nImage>0)
			{
				m_hIcon = pImageList->ExtractIcon( nImage );
			}

		}

		int nSize=AllElements.GetSizeArray();
		for(int i=0;i<nSize;i++)
		{
			CValue *pDialogData=&AllElements.GetAt(i+1);
			CDynControl *pControl=new CDynControl(this, 0,0);
			pControl->AddControl(*pDialogData);
			aControlList.Add(pControl);
			aControlListById[pControl->m_nID]=pControl;

			if(pDialogData->GetSizeStrArray()==0)
			{
				return 0;
			}

			CString csTypeObj=pDialogData->GetAt("Тип значения").GetString();
			csTypeObj.TrimLeft();
			csTypeObj.TrimRight();

			//if(pControl->GetWnd())
			//	pControl->GetWnd()->SetRedraw(0);


			CValue *pRefVal=0;
			if(pControl->m_nControlType==ID_DYNBUTTONLISTBOX)
			{
				CValueListBox *pVal=new CValueListBox();
				pVal->Attach(pControl->GetWnd());
				pVal->Init("СписокЗначений",CValue());
				pRefVal=pVal;
			}
			else
			if(pControl->m_nControlType==ID_DYNBUTTONCOMBO)
			{
				CValueComboBox *pVal=new CValueComboBox();
				pVal->Attach(pControl->GetWnd());
				pVal->Init("СписокЗначений",CValue());
				pRefVal=pVal;
			}
			else
			if(pControl->m_nControlType==ID_DYNBUTTONTREE)
			{
				CValueTreeCtrl *pVal=new CValueTreeCtrl();
				pVal->Attach((CTreeCtrl*)pControl->GetWnd());
				pVal->Init("Дерево",CValue());
				pRefVal=pVal;
			}
			else
			if(pControl->m_nControlType==ID_DYNBUTTONRADIO)
			{
				CValueRadio *pVal=new CValueRadio();
				pVal->Attach(pControl->GetWnd());
				pRefVal=pVal;
			}
			else
			if(pControl->m_nControlType==ID_DYNBUTTONGRID)
			{
				CValueGrid *pVal=new CValueGrid();
				CGridCtrl *p=(CGridCtrl*)pControl->GetWnd();
				pVal->Attach(p);
				pVal->Init("Grid",CValue());

				aGridCtrl.Add(p);

				CString csFormule=pDialogData->GetAt("Обратный вызов виртуального режима").GetString();
				int nIndex=csFormule.Find('(');
				if(nIndex>0)
					csFormule=csFormule.Left(nIndex); 
				csFormule.TrimRight();

				CCallBackInfo *pCallBack=new CCallBackInfo();
				pCallBack->pForm=this;
				pCallBack->pGrid=pVal;
				pCallBack->csFunction=csFormule;
				pCallBack->pControl=pControl;
				pCallBack->pFormValue=pFormVal;
				((CGridCtrl*)pVal->pWnd)->SetCallbackFunc(&VIRUALFUNCTION,LPARAM(pCallBack));
				((CGridCtrl*)pVal->pWnd)->pRunContext=&cRunModule.cCurContext;
				pRefVal=pVal;
			}
			else
			if(pControl->m_nControlType==ID_DYNBUTTONCHECK||pControl->m_nControlType==ID_BUTTONPROGRESS||pControl->m_nControlType==ID_BUTTONSLIDER)
			{
				CValueInt *pVal=new CValueInt();
				pVal->Attach(pControl->GetWnd());
				pRefVal=pVal;
			}
			else 
			if(pControl->m_nControlType==ID_DYNBUTTONEDIT)
			{
				if(csTypeObj=="Число")
				{
					CValueNumber *pVal=new CValueNumber();
					pVal->Attach(pControl->GetWnd());
					pRefVal=pVal;
				}
				else 
				if(csTypeObj=="Дата")
				{
					CValueControl *pVal=new CValueControl();
					pVal->Attach(pControl->GetWnd());
					pRefVal=pVal;
				}
				else
				{
					CValueEdit *pVal=new CValueEdit();
					pVal->Attach(pControl->GetWnd());
					pRefVal=pVal;
				}

			}
			else
			if(pControl->m_nControlType==ID_BUTTONFORMOBJECT)
			{
				aElements.Add(pControl->eventVal);
				continue;
			}
			else
			if(pControl->m_nControlType==ID_BUTTON_TAB)
			{
				CValueListLayers *pVal=new CValueListLayers();
				pVal->Attach(pControl->GetWnd());
				pRefVal=pVal;
			}
			else
			{
				CValueControl *pVal=new CValueControl();
				pVal->Attach(pControl->GetWnd());
				pRefVal=pVal;
			}

			//запоминаем первую кнопку по умолчанию
			if(!pDefButton)
			if(pDialogData->GetAt("Кнопка по умолчанию").GetNumber()==1)
			{
				pDefButton=pControl->GetWnd();
			}

			extern CString csBaseType;
			CString csBase=csBaseType;
			csBase=CString(",")+csBase+",";
			if(csBase.Find(","+csTypeObj+",")>=0)
				csTypeObj="";

			CValue ObjVal;
			if(csTypeObj.IsEmpty())
			{
				ObjVal=ValueByRef(pRefVal);
			}
			else
			{
				try
				{
					int _nType=pRefVal->nType;
					pRefVal->nType=100;
					CValue cVal=ValueByRef(pRefVal);//сначала копируем во временную переменную, потом вставляем в функцию... (иначе вылетает...)
					ObjVal=::CreateObject(csTypeObj,cVal);
					ObjVal.Attach(pControl->GetWnd());
					pRefVal->nType=_nType;
				}
				catch(CTranslateError *) 
				{
					if(ENTERPRISE_MODE==afxAppRunMode)
					{
						DoError();
						Message(String(CString("Ошибка при создании объекта ")+csTypeObj));
					}
					continue;
				}
			}
			if(!pControl->eventVal.nType)
				pControl->eventVal=ObjVal;
			aElements.Add(ObjVal);
		}

		pFormVal->Attach(this);
		pFormVal->vParam=afxFormParam;
		{
			//создаем дефолтный Таб-Контрол
			CValue ObjectVal;
			ObjectVal.CreateObject("Структура");
			ObjectVal.SetAt("ТипЭлемента",ID_BUTTON_TAB);
			ObjectVal.SetAt("Тип значения","Закладки");
			ObjectVal.SetAt("Вниз",1);
			ObjectVal.SetAt("Вправо",1);

			ObjectVal.SetAt("Видимость",0);
			ObjectVal.SetAt("Доступность",1);
			ObjectVal.SetAt("y1",0);
			ObjectVal.SetAt("x1",0);
			ObjectVal.SetAt("y2",form_dy);
			ObjectVal.SetAt("x2",form_dx);

			ObjectVal.SetAt("Выбор","ПриВыбореЗакладки");


			//проецируем его на форму
			CDynControl *pControl=new CDynControl(this, 0,0);
			pControl->AddControl(ObjectVal);
			nNumberTabCtrl=aControlList.Add(pControl);
			aControlListById[pControl->m_nID]=pControl;

			//связываем его с соответствующим агрегатным объектом
			CValueListLayers *pVal=new CValueListLayers();
			pVal->Attach(pControl->GetWnd());
			pFormVal->vLayers=ValueByRef(pVal);
		}
		//afxFormParam=CValue();

		aElements.Add(ValueByRef(pFormVal));



		for(i=0;i<aControlList.GetSize();i++)//заполняем массив соответствия слоев и элементов для всех закладок
		{
			CDynControl *pTabCtrl=aControlList[i];
			if(pTabCtrl->m_nControlType==ID_BUTTON_TAB)
			for(int j=0;j<aControlList.GetSize();j++)
			{
				CDynControl *pControl=aControlList[j];
				if(pTabCtrl!=pControl)//пропускаем самого себя
				{
					((CMyTabCtrl*)pTabCtrl->GetWnd())->Add(pControl->m_csLayer,pControl->m_nID);
				}
			}
		}

		//SetFormSize();

		//return TRUE;
	}
	return TRUE;
	//return FALSE;
}



void CFormUnit::GetNameValueList()
{
	CMapStringToPtr ListName;
	aNameList.RemoveAll();
	aValueList.RemoveAll();

	for(int i=0;i<min(aElements.GetSize(),aControlList.GetSize());i++)
	{
		CDynControl *pControl=aControlList[i];
		CString csName=pControl->m_csName;
		csName.TrimLeft();
		csName.TrimRight();
		if(csName.IsEmpty())
			csName.Format("Элемент_%d",i);
		if(!csName.IsEmpty())
		{
			if(!ListName[mUpper(csName)])//нет дубля идентификатора
			{
				ListName[mUpper(csName)]=(void*)1;

				aNameList.Add(csName);
				aValueList.Add(&aElements[i]);
			}
		}
	}



	//Форма
	int n=aElements.GetSize()-1;
	if(n>=0)
	{
		aNameList.Add(FORM_NAME);
		aValueList.Add(&aElements[n]);
		ListName[FORM_NAME]=(void*)1;
	}
}


int CFormUnit::Run()
{
	nReturnStatus=1;
	nModeCloseWindow=1;
	try
	{
		GetNameValueList();


		if(!pModule)
		{
			pModule=new CCompileModule();
			bDeleteModule=1;
			
			for(int i=0;i<aNameList.GetSize();i++)
			{
				CString csName=aNameList[i];
				pModule->AddVariable(aNameList[i],0);//объявление диалоговых элементов как внешних переменных
			}
				
			pModule->csModuleName=CString("")+csPath;
			if(AfxGetModuleManager())
			{
				CCompileModule *pStopParent;
				CCompileModule *pContinueParent;
				CCompileModule *pParent=AfxGetModuleManager()->GetParent(pModule->csModuleName,pStopParent,pContinueParent);
				pModule->SetParent(pParent,pStopParent,pContinueParent);
				AfxGetModuleManager()->SetAttrVariable(pModule);

			}
			pModule->Compile(csModule);
		}
		aOpenForm[csFormID]=this;
		
		if(pModule->cByteCode.aExternValue.GetSize()<aValueList.GetSize())
		{
			Error("Системная ошибка - форма изменила количество атрибутов");
		}

		for(int i=0;i<aValueList.GetSize();i++)
		{
			pModule->cByteCode.aExternValue[i+1]=aValueList[i];//связывание переменных (начинаем с 1, т.к. самая первая внешняя переменная это Контекст)
			aValueList[i]->bReadOnly=1;
		}
			
		

		pRunModule=&cRunModule;

		if(pRunRunModule&&mUpper(csPath.Left(METADATANAME.GetLength()+1))==mUpper(METADATANAME+"\\"))
		{
			CValueObject *pRef=(CValueObject *)pRunRunModule->cCurContext.pRefLocVars[0];
			if(pRef->GetRuntimeClass()==&CValueObject::classCValueObject)
			{
				pRunModule->SetParent(pRef->pRun);
				vParentContext=ValueByRef(pRef);
			}
		}
		if(!pRunModule->GetParent())
		if(AfxGetModuleManager()) 
		{
			pRunModule->SetParent(AfxGetModuleManager()->GetRunParent(pModule));
		}


		pRunModule->Exec(pModule->cByteCode,0);//проецирование в памяти
		vContext=pRunModule->GetAttribute(0);//контекстная переменная
		pRunModule->cCurContext.pCompileContext=&pRunModule->pByteCode->pModule->cContext;//контекст локальной компиляции


		if(vParentContext.FindMethod("ОбработкаОткрытияФормы")>=0)
			vParentContext.CallFunction("ОбработкаОткрытияФормы",&vContext);

		//активация всех микроформ
		for(i=0;i<min(aElements.GetSize(),aControlList.GetSize());i++)
		{
			CValue *pVal=&aElements[i];
			if(pVal->FindMethod("ИнициализацияОбъекта")>=0)
				pVal->CallFunction("ИнициализацияОбъекта",&vContext,&String(aControlList[i]->m_csName));
		}


		SetFormSize();

		pRunModule->Exec(&pRunModule->cCurContext,1);//запуск со строки, указанной в байт-коде
		CallFunction("ПриОткрытии",0,0,0);
		CallFunction("OnOpen",0,0,0);
		
		afxFormContext=vContext;
	}
	catch(CTranslateError *)
	{
		bCanOpen=0;
		aOpenForm[csFormID]=0;
		vParentContext.Reset();
		DoError();
	};
	if(nReturnStatus==0)//нельзя открывать форму
	{
		bCanOpen=0;
		return 0;
	}
	if(nModeCloseWindow==2)//требуется закрытие окна
	{
		bCanOpen=0;
		return 0;
	}

	if(nModeCloseWindow==1)//переход в нормальный режим
		nModeCloseWindow=0;

	bModified=0;
	return 1;
}


void CFormUnit::OnButton(UINT nID,int nMessage)
{
	if(nModeCloseWindow==0)
		nModeCloseWindow=1;//режим выполнения программы

	nRunCount++;
	OnButton2(nID,nMessage);
	nRunCount--;

	if(nModeCloseWindow==2)//требуется закрытие окна
	{
		CloseForm();
		return;
	}
	else
	{
		nModeCloseWindow=0;//переход в нормальный режим
	}
}
void CFormUnit::OnButton2(UINT nID,int nMessage)
{
	if(bUseDebug)
	if(bDebugLoop)
		return;

	if(!pRunModule) 
		return; 

	static BOOL bWasEnter=0;//защита от переполнения стека в W98
	if(bWasEnter)
		return;

	if(nMessage!=NM_CUSTOMDRAW)
	{
		RefreshExpr();
	}

	CDynControl *pControl=aControlListById[nID];
	if(pControl)
	{
		if(nMessage==EN_CHANGE)
		{
			bModified=1;
		}
		for(int i=0;i<pControl->aDataList.GetSize();i++)
		{
			int nData=pControl->aDataList[i].nData;
			CString csName=pControl->aDataList[i].csName;
			if(pControl->aDataList[i].nType==FUNCTION_DATA)
			if(nData==nMessage)
			{
				CWnd *pWnd=pControl->GetWnd();
				try
				{
					CValue vParam1,vParam2;
					if(TCN_SELCHANGE==nMessage)
					{
						CMyTabCtrl *pTabCtrl=(CMyTabCtrl*)pControl->GetWnd();
						int nIndex=pTabCtrl->GetCurSel();//выбранная закладка
						vParam1=nIndex+1;
						if(nIndex>=0)
							vParam2=String(pTabCtrl->aLayer[nIndex].Name);
					}

					if(WM_ONSELECT==nMessage)
					{
						afxCurrentWnd=pWnd;

						//При начале выбора значения
						CValue vFlag=1;
						CallFunction2("ПриНачалеВыбораЗначения",String(pControl->m_csName),vFlag,CValue(0));
						CallFunction2("OnStartValueChoice",String(pControl->m_csName),vFlag,CValue(0));
						if(!vFlag.GetNumber())
							return;
					}

					CString csPrevVal;
					if(pControl->eventVal.nType!=TYPE_EMPTY)
					{
						if(WM_ONSELECT==nMessage)
							pControl->eventVal.SaveToString(csPrevVal);

						int nRes=0;
						if(pControl->eventVal.FindMethod("ОбработкаСобытия")>=0)
							nRes=pControl->eventVal.CallFunction("ОбработкаСобытия",&(CValue(csName)),&vParam1,&vParam2,&CValue(),&CValue(),&CValue(),&CValue(),&CValue()).GetNumber();
						if(nRes)//отмена выбора или отмена дальнейшей обработки
						{
							nReturnStatus=0;
							return;
						}
					} 

					if(WM_ONSELECT==nMessage)
					{
						
						//При окончании выбора значения
						CValue vFlag=1;
						CallFunction2("ОбработкаВыбораЗначения",pControl->eventVal,String(pControl->m_csName),vFlag);
						CallFunction2("ProcessValueChoice",pControl->eventVal,String(pControl->m_csName),vFlag);

						if(!vFlag.GetNumber())
						{
							pControl->eventVal.LoadFromString(csPrevVal);
							return;
						}

						if(pWnd)
							((CWnd *)pWnd)->SetWindowText(pControl->eventVal.GetString());
						

					}

					CString csFormule=pControl->aDataList[i].sData;

					if(!csFormule.IsEmpty())
					{
							if(csName=="Формула надписи")
							{
								CString csValue=CProcUnit::Eval(csFormule,&pRunModule->cCurContext);
								CWnd *pWnd=pControl->GetWnd();
									
								if(pWnd)
								{
									bWasEnter=1;
									#define PWINDOW ((CWnd*)pWnd)
									#define PWINDOW2 ((CXColorStatic*)pWnd)
									PWINDOW->SetWindowText(csValue);
									if(pWnd->GetRuntimeClass()==(RUNTIME_CLASS(CXColorStatic)))	
									{
										PWINDOW2->SetPlainBorder(TRUE);
										PWINDOW2->SetPlainBorder(FALSE);
									}	
									bWasEnter=0;
								}
							}
							else//набор команд 
							{
								afxCurrentWnd=pWnd;
								
								//определяем - целое это слово или нет
								if(IsIdentifier(csFormule))
								{
									pRunModule->FindFunction(csFormule,1);
									CallFunction(csFormule,vParam1,vParam2,0);
								}
								else
								{
									CCompileModule cModule;
									cModule.SetParent(pModule);
									cModule.cContext.nFindLocalInParent=2;
									cModule.Compile(csFormule+";");
									
									CProcUnit cRunModule;
									cRunModule.SetParent(pRunModule);
									cRunModule.Exec(cModule.cByteCode);//проецирование в памяти
								}
							}
					}
				}
				catch(CTranslateError *)
				{
					afxEventMessage=0;
					DoError();
					return;
				};
			}
		}
	}
}

int CFormUnit::CallDefButton(WPARAM , LPARAM )
{
	if(pDefButton)
		OnButton(pDefButton->GetDlgCtrlID(),BN_CLICKED);
	return 1;
}

int CFormUnit::CanDestroy(WPARAM , LPARAM )
{
	IsCanDestroy();
	return 1;
}
int CFormUnit::IsCanDestroy()
{
	if(nRunCount>0)//программа еще выполняется
	{
		return 0;
	}

	if(bCanClose)
		return 1;


	nReturnStatus=1;
	afxEventMessage=1;

	nRunCount++;
	if(vParentContext.FindMethod("ОбработкаЗакрытияФормы")>=0)
		vParentContext.CallFunction("ОбработкаЗакрытияФормы",&vContext,&CValue(1));

	bCanClose=nReturnStatus;
	if(bCanClose)
	{
		CallFunction("ПриЗакрытии",0,0,0);
		CallFunction("OnClose",0,0,0);
		

		//проверка закрытия микроформ (вдруг там выполняется программа)
		CWnd *pCurWnd=GetWindow(GW_CHILD);
		if(nReturnStatus)
		while(pCurWnd)
		{
			pCurWnd->SendMessage(WM_CANDESTROY);//проверка разрешения у подчиненных объектов
			if(nReturnStatus==0)
				break;

			pCurWnd=pCurWnd->GetWindow(GW_HWNDNEXT);
		}
	}

	if(vParentContext.FindMethod("ОбработкаЗакрытияФормы")>=0)
		vParentContext.CallFunction("ОбработкаЗакрытияФормы",&vContext,&CValue(2),0);
	bCanClose=nReturnStatus;
	nRunCount--;


	afxEventMessage=0;
	return nReturnStatus;
}

int CFormUnit::CloseForm(WPARAM wParam, LPARAM lParam)
{
	nModeCloseWindow=2; 

	if(nRunCount>0)//программа еще выполняется
	{
		return 0;
	}

	if(nModeCloseWindow==2)//закрываем окна сейчас
	{
		CWnd *pParent=GetParent();
		if(pParent)
		{
			CString csName=GetRuntimeClass()->m_lpszClassName;
			if("CMicroForm"==csName)//микроформу закрывать нельзя
			{ 
				{
					MSG Msg;
					Msg.message=WM_KEYDOWN;
					Msg.wParam=VK_ESCAPE;
	#ifdef CFormUnit
					if(((CMainFrame*)AfxGetMainWnd())->DoCloseMessageBox(&Msg))
						return 0;
	#endif
					pParent->PostMessage(WM_CLOSEFORM);
				}

				nModeCloseWindow=0;
				return 0;
			}
		}

		if(!IsCanDestroy())//пользователь запретил закрывать
		{
			nModeCloseWindow=0;
			return 0;
		}



#ifndef CFormUnit
		if(GetParent())
			GetParent()->PostMessage(WM_CLOSE);
#else
			PostMessage(WM_CLOSE);
#endif
	}
	else
	{
		nModeCloseWindow=2;//закроем потом
	}
	return 1;
}

void CFormUnit::UseTabCtrl(BOOL bMode) 
{
	if(m_bUseTabCtrl==bMode)
		return;
	m_bUseTabCtrl=bMode;
	int nDelta=40;
	if(!m_bUseTabCtrl)
	{
		nDelta=-nDelta;
	}

	ASSERT(nNumberTabCtrl>=0);
	if(nNumberTabCtrl<0)
		return;
	CDynControl *pTab=aControlList[nNumberTabCtrl];
	if(!pTab)
		return;

	//смещаем все подчиненные элементы по вертикали
	for(int i=0;i<aControlList.GetSize();i++)
	{
		if(i==nNumberTabCtrl)
			continue;

		CDynControl *pControl=aControlList[i];
		pControl->m_y1+=nDelta;
		pControl->m_y2+=nDelta;

		int dx=pControl->m_x2-pControl->m_x1;
		int dy=pControl->m_y2-pControl->m_y1;

		pControl->GetWnd()->MoveWindow(pControl->m_x1, pControl->m_y1, dx, dy,1);
		if(!m_bUseTabCtrl)
			if(pControl->m_bVisible)
				pControl->GetWnd()->ShowWindow(SW_NORMAL);

	}
	pTab->m_y2+=nDelta;
	form_dy+=nDelta;



#ifndef CFormUnit
	CRect Rect;
	GetParentFrame()->GetWindowRect(Rect);

	if(GetParentFrame()->GetStyle() & WS_MAXIMIZE)
	{
		OnSize(0,Rect.right,Rect.bottom);
		GetParentFrame()->RecalcLayout();
		return;
	}

	Rect.bottom+=nDelta;
	::SetWindowPos(GetParentFrame()->m_hWnd,0,0, 0, Rect.Width(), Rect.Height(),SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_DRAWFRAME | SWP_NOMOVE);
#else
	::SetWindowPos(m_hWnd,0,0, 0, form_dx+10, form_dy+25,SWP_NOREPOSITION | SWP_NOSENDCHANGING | SWP_DRAWFRAME | SWP_NOMOVE);
#endif

}
 
void CFormUnit::OnSize(UINT nType, int cx2, int cy2) 
{

	if(0)
	{
		CRect rect;
		rect.left=form_dx;
		rect.right=rect.left+2000;
		rect.top=form_dy;
		rect.bottom=rect.top+2000;

		ClientToScreen(rect);
		ClipCursor(rect);
	}


	if(!bCanResize)
	{
		return;
	}

	CPARENT::OnSize(nType, cx2, cy2);

	int delta_x=cx2-form_dx;
	int delta_y=cy2-form_dy;

	//изменяем размер у подч элементов
	if(delta_x<=0)
		delta_x=1;
	if(delta_y<=0)
		delta_y=1;

	//сбрасываем значения новых начальных координат
	for(int i=0;i<aControlList.GetSize();i++)
	{
		aControlList[i]->m_x3=aControlList[i]->m_x1;
		aControlList[i]->m_y3=aControlList[i]->m_y1;
	}

	for (int nMode=1;nMode<=2;nMode++)
	{
	CWnd *pCurWnd=GetWindow(GW_CHILD);
	while(pCurWnd)
	{
		CDynControl *pControl=aControlListById[pCurWnd->GetDlgCtrlID()];
		if(pControl)
		if(pControl->m_bAutoSizeDown||pControl->m_bAutoSizeRight)
		{
			CRect Rect;
			pCurWnd->GetWindowRect(Rect);
			ScreenToClient(Rect);

			CRect CurrentWindow;//текущее положение окна
			CurrentWindow.left=pControl->m_x3;
			CurrentWindow.right=pControl->m_x3+pControl->m_x2-pControl->m_x1;
			CurrentWindow.top=pControl->m_y3;
			CurrentWindow.bottom=pControl->m_y3+pControl->m_y2-pControl->m_y1;
			CurrentWindow.NormalizeRect();

			int dx=Rect.Width();
			int dy=Rect.Height();

			int delta_x2=0;
			int delta_y2=0;
			if(pControl->m_bAutoSizeRight)//вправо
			{
				dx=CurrentWindow.Width()+delta_x;
				delta_x2=delta_x;
			}
			if(pControl->m_bAutoSizeDown)//вниз
			{
				dy=CurrentWindow.Height()+delta_y;
				delta_y2=delta_y;
			}

			if(1==nMode)
				MoveChildWindows(pControl,delta_x2,delta_y2);
			else
				pCurWnd->MoveWindow(Rect.left, Rect.top, dx, dy,1);
		}
		pCurWnd=pCurWnd->GetWindow(GW_HWNDNEXT);
	}
	}

	
}
void CFormUnit::MoveChildWindows(CDynControl *pControl,int delta_x,int delta_y)
{
	CRect ResizeWindow;//начальное положение окна
	ResizeWindow.left=pControl->m_x1;
	ResizeWindow.top=pControl->m_y1;
	ResizeWindow.right=pControl->m_x2;
	ResizeWindow.bottom=pControl->m_y2;
	for(int i=0;i<aControlList.GetSize();i++)
	{
		CDynControl *pChild=aControlList[i];
		if(pControl!=pChild)
		{
			CRect ChildRect; 
			ChildRect.left=pChild->m_x1;
			ChildRect.top=pChild->m_y1;
			ChildRect.right=pChild->m_x2;
			ChildRect.bottom=pChild->m_y2;

			int cur_delta_y=0;
			int cur_delta_x=0;
			if(ResizeWindow.right<ChildRect.left)
				cur_delta_x=delta_x;
			if(ResizeWindow.bottom<ChildRect.top)
				cur_delta_y=delta_y;

			//может уже было перемещение другим элементом?
			if(pChild->m_x3!=pChild->m_x1)
				cur_delta_x=0;
			if(pChild->m_y3!=pChild->m_y1)
				cur_delta_y=0;

			
			if(cur_delta_x>0 || cur_delta_y>0)
			{
				pChild->m_x3+=cur_delta_x;
				pChild->m_y3+=cur_delta_y;

				pChild->GetWnd()->MoveWindow(pChild->m_x3,pChild->m_y3,pChild->m_x2-pChild->m_x1,pChild->m_y2-pChild->m_y1,1);
			}
		}
	}
}



BOOL CFormUnit::Load() 
{
	bModified=0;
	pFormVal=0;
	csFormID=mUpper(afxFormID);
	pRunRunModule=afxCurrentRunModule;//модуль из которого открывается форма

	if(!afxFormPath.IsEmpty())
		csPath=afxFormPath;
	if(!afxMetaObj)
	{
		CZipArchive m_zip;
		try
		{
			m_zip.Open(csPath, CZipArchive::openReadOnly);
		}
		catch(...)
		{
			AfxMessageBox(CString("Ошибка открытия файла: ")+csPath);
			bCanOpen=0;
			return FALSE;
		}
			
		CString csAlias;
		CString csComment;
		//Чтение текста модуля
		csModule=ReadFileToString(m_zip,MODULENAME,csAlias,csComment);
		//Чтение текста описания
		csDescription=ReadFileToString(m_zip,DESCRIPNAME,csAlias,csComment);
		//Чтение содержимого формы
		csForm=ReadFileToString(m_zip,FORMNAME,csAlias,csComment);

		m_zip.Close();	
	}
	else
	{
		csPath=afxMetaObj->csPrevFileName;
		csModule=afxMetaObj->csFile;
		csForm=afxMetaObj->csForm;
		csDescription=afxMetaObj->csComment;
	}
	afxFormPath="";

	CString csName=GetRuntimeClass()->m_lpszClassName;
	if("CMicroForm"!=csName)//микроформу закрывать нельзя
	{ 
		//Поддержка проверки прав доступа.
		//if(!GrantCheck(csPath))
		CValue *aP[7];
		for(int i=0;i<7;i++)
			aP[i]=new CValue();
		try
		{
			aP[0]->SetString(csPath);
			if(NotifyEvent("ПриОткрытииФормы",aP)!=0)
				return FALSE;
		}
		catch(...){}
	}

  
  return TRUE;  // возвращает TRUE  до тех пор, пока Вы не установите
                // фокус на элемент управления


}

extern int glVirtKey;
BOOL CFormUnit::PreTranslateMessage(MSG* pMsg) 
{
/*	if(pMsg->message == WM_KEYDOWN)
	{
		CString Str;
		Str.Format("1 WM_KEYDOWN = %d",pMsg->wParam);

		Message(Str);
	}*/

	CWnd *pWnd=GetFocus();
	if(pMsg->message == WM_KEYDOWN)
	if(pWnd)
	{ 
		int nVirtKey = (int) pMsg->wParam;
		glVirtKey=nVirtKey;
		if (nVirtKey==VK_F4)
		if(IsCTRLpressed())
		{
			CloseForm(0,0);
			return TRUE;
		}

		int _nReturnStatus=nReturnStatus;
		nReturnStatus=1;


		OnButton(pWnd->GetDlgCtrlID(),WM_KEYDOWN);
		if(!nReturnStatus)
			return TRUE;
		nReturnStatus=_nReturnStatus;



		if (nVirtKey==VK_ESCAPE)
		if(pWnd->GetParent()==this)
		{
#ifndef CFormUnit
			if(((CMainFrame*)AfxGetMainWnd())->DoCloseMessageBox(pMsg))
				return TRUE;
#endif

			nModeCloseWindow=2;
			CloseForm();
			return TRUE;
		}
		if (nVirtKey==VK_RETURN)
		if(!IsCTRLpressed())
		{
			CRuntimeClass* prt = pWnd->GetRuntimeClass();
			CString csName=prt->m_lpszClassName;

			if(csName=="CListBox"||csName=="CTreeCtrl")
			{
				OnButton(pWnd->GetDlgCtrlID(),LBN_DBLCLK);
				return TRUE;
			}

			int nTab=nVirtKey;
			if(	
				csName=="CDateTimeCtrl"||
				csName=="CRadio"
				)
			{
				nTab=9;
			}
			else
			if(csName.Find("Edit")>-1&&pWnd->GetParent()==this)
			{
				if(!(::GetWindowLong(pWnd->m_hWnd, GWL_STYLE) & ES_WANTRETURN))
					nTab=9;
			}

			pMsg->wParam=nTab;
			if(nTab!=VK_RETURN)
			{
				return CPARENT::PreTranslateMessage(pMsg);
			}

			if(csName.Find("Button")>0) 
			{
				OnButton(pWnd->GetDlgCtrlID(),BN_CLICKED);
				return TRUE;
			}
			return pWnd->PreTranslateMessage(pMsg);
		}

		if (nVirtKey==VK_F2)
		if(pWnd->GetParent()==this)
		{
			pMsg->wParam=VK_RETURN;
			return pWnd->PreTranslateMessage(pMsg);
		}


		//обработка кнопки по умолчанию
		if (nVirtKey==VK_ENTER)
		if(IsCTRLpressed() || (GetExStyle()& (WS_EX_DLGMODALFRAME)))
		{
			if(pDefButton)
			{
				CallDefButton();
				return TRUE;
			}
			else
			{
				//ищем кнопки по умолчанию в подчиненных объектах (окнах)
				CWnd *pCurWnd=GetWindow(GW_CHILD);
				while(pCurWnd)
				{
					int nRes=pCurWnd->SendMessage(WM_DEFBUTTON);
					if(nRes==1)
						return TRUE;

					pCurWnd=pCurWnd->GetWindow(GW_HWNDNEXT);
				}
			}
		}

	}


	return CPARENT::PreTranslateMessage(pMsg);
}



BOOL CFormUnit::FindOpen(CString Str)
{
	CFormUnit *pForm=(CFormUnit *)aOpenForm[mUpper(Str)];
	if(pForm)
	{
		if(pForm->GetParent())
			pForm->GetParent()->BringWindowToTop();
		pForm->BringWindowToTop();
		pForm->CallFunction("ПриПовторномОткрытии",0,0,0);
		pForm->CallFunction("OnReopen",0,0,0);
		afxFormContext=pForm->vContext;
		return 1;
	}

	return 0;
}


#ifndef CFormUnit
BOOL CFormUnit::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CFormUnit::OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	for(int i=0;i<aGridCtrl.GetSize();i++)
	{
		CWnd *pGridCtrl=GetFocus();
		HWND h=aGridCtrl[i]->m_hWnd;

		CWnd *Focus=aGridCtrl[i]->GetFocus();
		if(Focus)
		{
			aGridCtrl[i]->OnBeginPrinting(pDC, pInfo);
			break;
		}
	}
}

void CFormUnit::OnPrint(CDC* pDC, CPrintInfo* pInfo) 
{
	for(int i=0;i<aGridCtrl.GetSize();i++)
	{
		CWnd *Focus=aGridCtrl[i]->GetFocus();
		if(Focus)
		{
			aGridCtrl[i]->OnPrint(pDC, pInfo);
			break;
		}
	}
}

void CFormUnit::OnEndPrinting(CDC* pDC, CPrintInfo* pInfo)
{
	for(int i=0;i<aGridCtrl.GetSize();i++)
	{
		CWnd *Focus=aGridCtrl[i]->GetFocus();
		if(Focus)
		{
			aGridCtrl[i]->OnEndPrinting(pDC, pInfo);
			break;
		}
	}
}
#endif



CString CFormUnit::DefButton(CString Str0)
{
	//получаем старое значение
	CString csRes;
	if(pDefButton)
	{
		CDynControl *pControl=aControlListById[pDefButton->GetDlgCtrlID()];
		if(pControl)
			csRes=pControl->m_csName;
	}

	CString Str=mUpper(Str0);
	if(!Str.IsEmpty())
	{
		//устанавливаем новое значение
		for(int i=0;i<aControlList.GetSize();i++)
		{
			CDynControl *pControl=aControlList[i];
			if(mUpper(pControl->m_csName)==Str)
			{
				pDefButton=pControl->GetWnd();
				return csRes;

			}
		}
		Error("Идентификатор \"%s\" не найден",Str0);
	}
	return csRes;
}
CString CFormUnit::ActiveControl(CString Str0)
{
	//получаем старое значение
	CString csRes;
	CWnd *pWnd=GetFocus();
	if(pWnd)
	{
		CDynControl *pControl=aControlListById[pWnd->GetDlgCtrlID()];
		if(pControl)
			csRes=pControl->m_csName;
	}

	CString Str=mUpper(Str0);
	if(!Str.IsEmpty())
	{
		//устанавливаем новое значение
		for(int i=0;i<aControlList.GetSize();i++)
		{
			CDynControl *pControl=aControlList[i];
			if(mUpper(pControl->m_csName)==Str)
			{
				pSetFocus=pControl->GetWnd();
				if(pSetFocus)
					pSetFocus->SetFocus();
				return csRes;

			}
		}
		Error("Идентификатор \"%s\" не найден",Str0);
	}
	return csRes;
}


//ВыполнитьВыбор
CValue CFormUnit::MakeChoice(CValue Val)
{
	if(nChoiceMode)
	if(vParentChoiceContext.nType)
	{
		if(1==nChoiceMode)
			PostMessage(WM_CLOSEFORM);

		CValue vRet;
		if(vParentChoiceContext.FindMethod("ОбработкаПодбора")>=0)
			vRet=vParentChoiceContext.CallFunction("ОбработкаПодбора",&Val,&vContext);
		else
		if(vParentChoiceContext.FindMethod("ProcessPermanentChoice")>=0)
			vRet=vParentChoiceContext.CallFunction("ProcessPermanentChoice",&Val,&vContext);

		return vRet;
	}
	return CValue();
}


BOOL CFormUnit::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CPARENT::PreCreateWindow(cs) )
		return FALSE;

	return 1;
}

int CFormUnit::RefreshExpr(WPARAM wParam, LPARAM lParam)
{
	//делаем invalidate, элементы у которых есть св-во NM_CUSTOMDRAW ("Формула надписи")
	for(int n=0;n<aControlList.GetSize();n++)
	{
		CDynControl *pControl=aControlList[n];
		for(int i=0;i<pControl->aDataList.GetSize();i++)
		{
			if( pControl->aDataList[i].nData==NM_CUSTOMDRAW)
			//if(!pControl->aDataList[i].sData.IsEmpty())
			{
				CWnd *pWnd=pControl->GetWnd();
				pWnd->RedrawWindow();
				#define PWINDOW2 ((CXColorStatic*)pWnd)
				if(pWnd->GetRuntimeClass()==(RUNTIME_CLASS(CXColorStatic)))	
				{
					PWINDOW2->SetPlainBorder(TRUE);
					PWINDOW2->SetPlainBorder(FALSE);
				}	
				//pControl->GetWnd()->PostMessage(NM_CUSTOMDRAW);
				//OnButton(pControl->m_nID,NM_CUSTOMDRAW);
				break;
			}
		}
	}
	return 1;
}
