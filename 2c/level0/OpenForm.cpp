#include "stdafx.h"
#include "DialogUnit.h"
#include "FormUnit.h"

#include "OpenForm.h"
#include "MicroForm.h"


CRITICAL_SECTION m_criticalSection;
BOOL bInitCriticalSection=0;



/////////////////////////////////////////////////////////////////////////////
CMetadataTree *afxMetaTree;//текущее дерево метаданных
CMetaObject *afxMetaObj;	//текущий объект метаданных


/////////////////////////////////////////////////////////////////////////////
CDocument* OpenFormMDI(CString csClassName,int bVisible,CMetadataTree *pMetaTree,CMetaObject *pMetaObj)
{
	CLock cs(m_criticalSection, "OpenFormMDI",bInitCriticalSection);

	afxMetaTree=pMetaTree;
	afxMetaObj=pMetaObj;
	CDocument* pDoc=CEnterpriseApp::m_pSystemManager->OpenForm(csClassName,bVisible);
	afxMetaTree=0;
	afxMetaObj=0;
	return pDoc;
}

/////////////////////////////////////////////////////////////////////////////
//ОТКРЫТИЕ ПРОИЗВОЛЬНОЙ ФОРМЫ 2С
//Все параметры передаются чрез глобальные переменные!!!
/////////////////////////////////////////////////////////////////////////////
CValue afxFormParam;//параметр для формы
CValue afxFormContext;//контекст открытой формы
CString afxFormPath;//путь внешней формы
CString afxFormID;//уникальный ид открываемой формы
CValue afxChoiceContext;//контекст, из которого открыт подбор
CValue afxCurrentValue;//текущее значение (для списка формы)
int afxChoiceMode=0;


////////////////////////////////////////////////////////////////////////////
//nMode:
#define NORMAL_MODE		0
#define MODAL_MODE		1
#define EDITABLE_MODE	2
#define MICROFORM_MODE	3// (or pControl!=0)
int OpenFormExt2(CString csFormName,CValue &vContext, CValue Param,CString csFileName,CString csFormID,int nMode,  CValue vChoiceContext,int nChoiceMode,CValue vCurrentValue, CMicroForm* pControl,CWnd *pParent)
{

	afxFormPath="";
	afxCurrentValue=vCurrentValue;
	if(!pControl)
		afxChoiceContext=vChoiceContext;
	afxChoiceMode=nChoiceMode;
	
	if(csFormID.IsEmpty() && (!csFileName.IsEmpty()))
		csFormID=csFileName;

	if(!pControl)
	{
		if(!csFormID.IsEmpty())
		{
			if(CDialogUnit::FindOpen(csFormID))
			{
				vContext=afxFormContext;
				return 1;
			}
			if(CFormUnit::FindOpen(csFormID))
			{
				vContext=afxFormContext;
				return 1;
			}
		}
		
	}
	try
	{
		int Ret=AfxGetModuleManager()->FindForm(csFormName,csFileName,afxMetaObj); 
		if(!Ret)
		{
			if(!pControl)
				return 0;
		}
	}
	catch(...)
	{
		afxMetaObj=new CMetaObject();

	}
	afxFormParam=Param;
	afxFormID=csFormID;

	if(pControl)//микроформа
	{
		try
		{
			pControl->Create((LPCTSTR )IDD_FORMUNIT,pParent);
		}
		catch(...)
		{
		}
		vContext=afxFormContext;
		return 1;
	}


	if(MODAL_MODE==nMode || START_MODE==afxAppRunMode)
	{
		CDialogUnit dlg;
		dlg.DoModal();
		vContext=afxFormParam;
	}
	else
	if(NORMAL_MODE==nMode && CONFIG_MODE==afxAppRunMode)
	{
		OpenFormMDI("ИсполняемаяФорма",1,0,afxMetaObj);
		vContext=afxFormContext;
	}
	else
	if(EDITABLE_MODE==nMode || ENTERPRISE_MODE==afxAppRunMode)
	{
		OpenFormMDI("Форма",1,0,afxMetaObj);
		vContext=afxFormContext;
	}
	
	return 1;
}

//функция вызова открытия формы с сохранением гл. переменных в стеке
int OpenFormExt(CString csFormName,CValue &vContext, CValue Param,CString csFileName,CString csFormID,int nMode,  CValue vChoiceContext,int nChoiceMode,CValue vCurrentValue, CMicroForm* pControl,CWnd *pParent)
{
	CLock cs(m_criticalSection, "OpenFormExt",bInitCriticalSection);

	//запоминаем глоб. переменные
	CValue _afxFormParam=afxFormParam;
	CValue _afxFormContext=afxFormContext;
	CString _afxFormPath=afxFormPath;
	CString _afxFormID=afxFormID;
	CValue _afxChoiceContext=afxChoiceContext;
	int _afxChoiceMode=afxChoiceMode;
	CMetaObject *_afxMetaObj=afxMetaObj;
	CValue _afxCurrentValue=afxCurrentValue;

	//вызов критической процедуры
	int nReturn=OpenFormExt2(csFormName,vContext,Param,csFileName,csFormID,nMode,vChoiceContext,nChoiceMode,vCurrentValue,pControl,pParent);

	//восстанавливаем глоб. переменные
	afxFormParam=_afxFormParam;
	afxFormContext=_afxFormContext;
	afxFormPath=_afxFormPath;
	afxFormID=_afxFormID;
	afxChoiceContext=_afxChoiceContext;
	afxChoiceMode=_afxChoiceMode;
	afxMetaObj=_afxMetaObj;
	afxCurrentValue=_afxCurrentValue;
	return nReturn;
}

