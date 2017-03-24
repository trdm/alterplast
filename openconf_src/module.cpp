// module.cpp
#include "stdafx.h"
#include "configsvcimpl.h"
#include "dispimpl.h"
#include "iconfig.h"
#include "module.h"
#include "metadata.h"
#include "metadataobj.h"

ITypeInfo* CIModule::m_pTypeInfo=NULL;
char* CIModule::m_TypesOfModules[]={
	"ModuleText",
	"CalcAlg",
	"UserHelp",
	"Transact",
	"SubList",
	"SubFolder",
	"Subconto",
	"Document",	
	"Journal",	
	"CalcJournal",	
	"AccountChartList",	
	"AccountChart",	
	"OperationList",	
	"Operation",	
	"ProvList",	
	"Report",	
	"CalcVar",	
	"ERT",	
	"File",	
};

STDMETHODIMP CIModule::get_Text(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		CString text;
		pDoc->GetText(text);
		*pVal=text.AllocSysString();
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось получить текст модуля");
	/*
	CBuhDef* pDef=CIMetaData::m_pMetaData->GetBuhDef();
	CJournalDef* pJ=(CJournalDef*)pDef->GetOperJournalDef();
	CMetaDataObj* pObj=pJ->GetFormDefsArray()->GetMetaDataArray()->GetAt(0);
	DoMsgLine(pObj->GetCode());
	*/
}
STDMETHODIMP CIModule::put_Text(BSTR newVal)
{
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		CString text=newVal;
		pDoc->SetText(text);
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось установить текст модуля");
}
STDMETHODIMP CIModule::get_LineCount(long *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		*pVal=pDoc->GetLineCount();
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось получить текст модуля");
}
STDMETHODIMP CIModule::get_Line(long num,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		if(num<0 || num>=pDoc->GetLineCount())
			return SetError(E_FAIL,"Недопустимый номер строки модуля");
		CString text;
		pDoc->GetLine(num,text);
		*pVal=text.AllocSysString();
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось получить текст модуля");
}
STDMETHODIMP CIModule::put_Line(long num,BSTR newVal)
{
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		if(num<0 || num>=pDoc->GetLineCount())
			return SetError(E_FAIL,"Недопустимый номер строки модуля");
		CString text=newVal;
		CPoint pt(pDoc->GetLineLength(num)-1,num);
		while(--pt.x)
			pDoc->DeleteSymb(1,pt);
		pDoc->InsertStr(text,CPoint(0,num),TRUE);
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось установить текст модуля");
}
STDMETHODIMP CIModule::get_Name(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	CTextDocument* pDoc=GetDoc();
	if(pDoc)
	{
		CString path;
		path=GetModuleFullName(m_id,m_TypesOfModules[m_kind],0,CIMetaData::m_pMetaData);
		//path=pDoc->GetTitle();
		//CConfigCont::DocumentToID(pDoc,path);
		//CConfigCont::DocumentToPath(pDoc,path);
		*pVal=path.AllocSysString();
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось получить имя модуля");
}
STDMETHODIMP CIModule::Open(void)
{
	CDocument* pDoc=NULL;
	if(m_kind<modSubcList)
	{
		CModuleCont* pCont=NULL;
		CString path;
		CConfigCont::IDToTextModule(m_id,m_TypesOfModules[m_kind],path,&pCont,TRUE);
		if(pCont)
		{
			pCont->ShowDocument(m_TypesOfModules[m_kind]);
			pDoc=pCont->GetTextDocument();
		}
	}
	else if(m_kind<modERT)
	{
		CTypedCont* pCont=NULL;
		CString path;
		CConfigCont::IDToPath(m_id,m_TypesOfModules[m_kind],path,&pCont,TRUE);
		if(pCont)
		{
			pCont->ShowDocument(m_id);
			CWorkBookDoc* pWB=pCont->GetWorkBook();
			if(pWB)
				pWB->SwitchToPage(1);
			pDoc=pCont->GetTextDocument();
		}
	}
	if(pDoc)
	{
		POSITION pos=pDoc->GetFirstViewPosition();
		if(pos)
		{
			CView* pView=pDoc->GetNextView(pos);
			if(pView)
			{
				CFrameWnd* pFrame=pView->GetParentFrame();
				if(pFrame)
					pFrame->ActivateFrame();
				pView->SetFocus();
			}
		}
		return S_OK;
	}
	return SetError(E_FAIL,"Не удалось открыть модуль");
}
STDMETHODIMP CIModule::get_MetaDataObj(IMetaDataObj **pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	*pVal=CIMetaDataObj::GetObj(m_id);
	return S_OK;
}
STDMETHODIMP CIModule::get_Kind(ModuleKind *pVal)
{
	if(IsBadWritePtr(pVal,4))
		return E_POINTER;
	*pVal=m_kind;
	return S_OK;
}

CTextDocument* CIModule::GetDoc()
{
	if(m_kind<modSubcList)
	{
		CModuleCont* pCont=NULL;
		CString path;
		CConfigCont::IDToTextModule(m_id,m_TypesOfModules[m_kind],path,&pCont,FALSE);
		if(pCont)
			return (CTextDocument*)pCont->GetTextDocument();
	}
	else if(m_kind<modERT)
	{
		CTypedCont* pCont=NULL;
		CString path;
		CConfigCont::IDToPath(m_id,m_TypesOfModules[m_kind],path,&pCont,FALSE);
		if(pCont)
			return (CTextDocument*)pCont->GetTextDocument();
	}
	else if(m_kind==modERT)
	{
	}
	return NULL;
}
static const char* strNames[]={
	"ГлобальныйМодуль",
	"Справочник",	// <ИмяСпр>.<Форма | Форма Группы | ФормаСписка>
	"Документ",		// <ИмяДок>.<Форма | Модуль Документа>
	"Отчет",		// <Имя Отчета>
	"Обработка",	// <Имя обработки>
	"ВидРасчета",	// <Имя расчета>
	"Журнал",		// <Имя журнала документов>
	"ЖурналРасчетов",	// <Имя журнала расчетов>
	"ПланСчетов",		// <Форма>	<ФормаСписка>
	"Операция",		// <Форма> | <ФормаСписка.Имя>
	"Проводка",		// <ФормаСписка>.имя
	"Описание",		// <ID>
};

CIModule* CIModule::GetModule(CString name)
{
	if(name.IsEmpty())
		return NULL;
	ModuleKind kind=modGlobal;
	long id=0;

	CStringArray keys;
	LPCTSTR pStart=name,pRead=pStart;
	while(1)
	{
		while(*pRead && *pRead!='.')
			pRead++;
		keys.Add(CString(pStart,pRead-pStart));
		if(!*pRead)
			break;
		pStart=++pRead;

	}
	for(int wrd=0;wrd<sizeof(strNames)/sizeof(strNames[0]);wrd++)
	{
		if(!keys[0].CompareNoCase(strNames[wrd]))
			break;
	}
	if(wrd==sizeof(strNames)/sizeof(strNames[0]))
		return NULL;
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	int words=keys.GetSize();
	switch(wrd){
	case 0:		// Глобальный модуль
		id=1;
		break;
	case 1:		// Справочник
		if(words>1)
		{
			CSbCntTypeDef* pDef=pMD->GetSTypeDef(keys[1]);
			if(pDef)
			{
				if(words>2)
				{
					if(!keys[2].CompareNoCase("Форма"))
					{
						id=pDef->GetID();
						kind=modSubcItem;
					}
					else if(!keys[2].CompareNoCase("Форма Группы"))
					{
						id=pDef->GetID();
						kind=modSubcGroup;
					}
					else if(!keys[2].CompareNoCase("ФормаСписка"))
					{
						if(words>3)
						{
							CFormDefsArray* pfDef=pDef->GetFormDefsArray();
							if(pfDef)
							{
								CMetaDataObjArray* pArr=pfDef->GetMetaDataArray();
								if(pArr)
								{
									CMetaDataObj* pObj=pArr->GetItem(keys[3]);
									if(pObj)
									{
										id=pObj->GetID();
										kind=modSubcList;
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	case 2:		// Документ
		if(words>1)
		{
			CDocDef* pDef=pMD->GetDocDef(keys[1]);
			if(pDef)
			{
				if(words>2)
				{
					if(!keys[2].CompareNoCase("Форма"))
					{
						id=pDef->GetID();
						kind=modDocForm;
					}
					else if(!keys[2].CompareNoCase("Модуль Документа"))
					{
						id=pDef->GetID();
						kind=modTransact;
					}
				}
			}
		}
		break;
	case 3:		// Отчет
		if(words>1)
		{
			CReportDef* pDef=pMD->GetReportDef(keys[1]);
			if(pDef)
			{
				id=pDef->GetID();
				kind=modReport;
			}
		}
		break;
	case 4:		// Обработка
		if(words>1)
		{
			CCalcVarDef* pDef=pMD->GetCalcVarDef(keys[1]);
			if(pDef)
			{
				id=pDef->GetID();
				kind=modProcessing;
			}
		}
		break;
	case 5:		// ВидРасчета
		if(words>1)
		{
			CAlgorithmDef* pDef=pMD->GetAlgorithmDef(keys[1]);
			if(pDef)
			{
				id=pDef->GetID();
				kind=modCalcKind;
			}
		}
		break;
	case 6:		// Журнал
		if(words>1)
		{
			CJournalDef* pDef=pMD->GetJournalDef(keys[1]);
			if(pDef)
			{
				if(words>2)
				{
					CFormDefsArray* pForms=pDef->GetFormDefsArray();
					if(pForms)
					{
						CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
						if(pArr)
						{
							CMetaDataObj* pObj=pArr->GetItem(keys[2]);
							if(pObj)
							{
								id=pObj->GetID();
								kind=modJournal;
							}
						}
					}
				}
			}
		}
		break;
	case 7:		// ЖурналРасчетов
		if(words>1)
		{
			CCJDef* pDef=pMD->GetCJDef(keys[1]);
			if(pDef)
			{
				if(words>2)
				{
					CFormDefsArray* pForms=pDef->GetFormDefsArray();
					if(pForms)
					{
						CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
						if(pArr)
						{
							CMetaDataObj* pObj=pArr->GetItem(keys[2]);
							if(pObj)
							{
								id=pDef->GetID();
								kind=modCalcJournal;
							}
						}
					}
				}
			}
		}
		break;
	case 8:		// Счет
		{
			CBuhDef* pDef=pMD->GetBuhDef();
			if(pDef)
			{
				if(words>1)
				{
					if(!keys[1].CompareNoCase("Форма"))
					{
						id=pDef->GetID();
						kind=modAccount;
					}
					else if(!keys[1].CompareNoCase("ФормаСписка"))
					{
						if(words>2)
						{
							CFormDefsArray* pForms=pDef->GetAccFormDefsArray();
							if(pForms)
							{
								CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
								if(pArr)
								{
									CMetaDataObj* pObj=pArr->GetItem(keys[2]);
									if(pObj)
									{
										id=pObj->GetID();
										kind=modAccountList;
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	case 9:		// Операция
		{
			CBuhDef* pDef=pMD->GetBuhDef();
			if(pDef)
			{
				if(words>1)
				{
					if(!keys[1].CompareNoCase("Форма"))
					{
						id=pDef->GetID();
						kind=modOperation;
					}
					else if(!keys[1].CompareNoCase("ФормаСписка"))
					{
						if(words>2)
						{
							CJournalDef* pJ=pDef->GetOperJournalDef();
							if(pJ)
							{
								CFormDefsArray* pForms=pJ->GetFormDefsArray();
								if(pForms)
								{
									CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
									if(pArr)
									{
										CMetaDataObj* pObj=pArr->GetItem(keys[2]);
										if(pObj)
										{
											id=pObj->GetID();
											kind=modOperJournal;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		break;
	case 10:	// Проводка
		if(words>2)
		{
			if(!keys[1].CompareNoCase("ФормаСписка"))
			{
				CBuhDef* pDef=pMD->GetBuhDef();
				if(pDef)
				{
					CFormDefsArray* pForms=pDef->GetProvFormDefsArray();
					if(pForms)
					{
						CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
						if(pArr)
						{
							CMetaDataObj* pObj=pArr->GetItem(keys[2]);
							if(pObj)
							{
								id=pObj->GetID();
								kind=modProvJournal;
							}
						}
					}
				}
			}
		}
		break;
	case 11:	// Описание
		if(words>1)
		{
			id=atol(keys[1]);
			if(pMD->FindObject(id))
				kind=modUserHelp;
			else
				id=0;
		}
		break;
	};
	if(id)
		return new CIModule(kind,id);
	return NULL;
}
