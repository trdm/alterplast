// ICfgDocuments.cpp: implementation of the CICfgDocuments class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ICfgDocuments.h"
#include "ICfgDoc.h"
#include "metadata.h"
#include "ialltypedobj.h"

ITypeInfo* CICfgDocuments::m_pTypeInfo=NULL;
CICfgDocuments* CICfgDocuments::m_pDocuments=NULL;
CRuntimeClass* CICfgDocuments::m_pUsefulRTC[10];

const char* CICfgDocuments::TypesOfModules[]={
	"ModuleText",	// 0
	"CalcAlg",		// 1
	"UserHelp",		// 2
	"Transact",		// 3
	"SubList",		// 4
	"SubFolder",	// 5
	"Subconto",		// 6
	"Document",		// 7
	"Journal",		// 8
	"CalcJournal",	// 9
	"AccountChartList",	// 10	
	"AccountChart",		// 11
	"OperationList",	// 12
	"Operation",		// 13
	"ProvList",			// 14
	"Report",			// 15
	"CalcVar",			// 16
	"GlobalData",		// 17	
};

static const char* strNames[]={
	"Глобальный Модуль",	// Добавим правильное имя
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
	"Общие Таблицы",		//
};

void CICfgDocuments::Init()
{
	static const char *usefulRtc[]={
		"CWorkBookDoc",		// seven		0
		"CSheetDoc",		// moxel		1
		"CEditDoc",			// editr		2
		"CTextDocument",	// txtedt		3
		"CBodyTextDoc",		// txtedt		4
		"CChildFrame7",		// frame		5
	};
	static const char *modulesRtc[]={
		"seven.dll",
		"moxel.dll",
		"editr.dll",
		"txtedt.dll",
		"frame.dll",
	};
	static const int rtcInMod[]={
		0,
		1,
		2,
		3,
		3,
		4,
	};
	for(int i=0;i<sizeof(usefulRtc)/sizeof(usefulRtc[0]);i++)
	{
		HINSTANCE hMod=GetModuleHandle(modulesRtc[rtcInMod[i]]);
		CString rtc;
		rtc.Format("?class%s@%s@@2UCRuntimeClass@@B",usefulRtc[i],usefulRtc[i]);
		m_pUsefulRTC[i]=(CRuntimeClass*)GetProcAddress(hMod,rtc);
	}
}

CICfgDocuments::CICfgDocuments()
{
	m_pDocuments=this;
}

CICfgDocuments::~CICfgDocuments()
{
}

int CICfgDocuments::NameToType(LPCTSTR name)
{
	for(int wrd=0;wrd<sizeof(TypesOfModules)/sizeof(TypesOfModules[0]);wrd++)
	{
		if(!lstrcmpi(name,TypesOfModules[wrd]))
			return wrd;
	}
	return -1;
}

static long GetIDFromFormDefs(CFormDefsArray* pForms,CString& name)
{
	if(pForms)
	{
		CMetaDataObjArray* pArr=pForms->GetMetaDataArray();
		if(pArr)
		{
			CMetaDataObj* pObj=pArr->GetItem(name);
			if(pObj)
				return pObj->GetID();
		}
	}
	return 0;
}

STDMETHODIMP CICfgDocuments::get_item(BSTR bname,ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	*pVal=NULL;
	CString name=(LPCTSTR)_bstr_t(bname);
	name.TrimLeft(" \t\r\n");
	name.TrimRight(" \t\r\n");
	if(name.IsEmpty())
		return S_OK;
	int kind;
	long id=0;

	CStringArray keys;
	SplitStr2Array(name,keys);
	for(int wrd=0;wrd<sizeof(strNames)/sizeof(strNames[0]);wrd++)
	{
		if(!keys[0].CompareNoCase(strNames[wrd]))
			break;
	}
	if(wrd==sizeof(strNames)/sizeof(strNames[0]))
		return SetError(E_FAIL,"Неправильное имя: %s",keys[0]);
	wrd--;	// Из-за добавленного Глобальный Модуль
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	int words=keys.GetSize();
	int readed=0;
	switch(wrd){
	case -1:
	case 0:		// Глобальный модуль
		id=1;
		kind=0;
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
						kind=6;
						readed=3;
					}
					else if(!keys[2].CompareNoCase("Форма Группы"))
					{
						id=pDef->GetID();
						kind=5;
						readed=3;
					}
					else if(!keys[2].CompareNoCase("ФормаСписка"))
					{
						if(words>3)
						{
							id=GetIDFromFormDefs(pDef->GetFormDefsArray(),keys[3]);
							kind=4;
							readed=4;
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
						kind=7;
						readed=3;
					}
					else if(!keys[2].CompareNoCase("Модуль Документа"))
					{
						id=pDef->GetID();
						kind=3;
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
				kind=15;
				readed=3;
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
				kind=16;
				readed=3;
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
				kind=1;
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
					if(!keys[2].CompareNoCase("Форма"))
					{
						if(words>3)
						{
							id=GetIDFromFormDefs(pDef->GetFormDefsArray(),keys[3]);
							kind=8;
							readed=4;
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
					if(!keys[2].CompareNoCase("Форма"))
					{
						if(words>3)
						{
							id=GetIDFromFormDefs(pDef->GetFormDefsArray(),keys[3]);
							kind=9;
							readed=5;
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
						kind=11;
						readed=2;
					}
					else if(!keys[1].CompareNoCase("ФормаСписка"))
					{
						if(words>2)
						{
							id=GetIDFromFormDefs(pDef->GetAccFormDefsArray(),keys[2]);
							kind=10;
							readed=4;
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
						kind=13;
						readed=2;
					}
					else if(!keys[1].CompareNoCase("ФормаСписка"))
					{
						if(words>2)
						{
							CJournalDef* pJ=pDef->GetOperJournalDef();
							if(pJ)
							{
								id=GetIDFromFormDefs(pJ->GetFormDefsArray(),keys[2]);
								kind=12;
								readed=4;
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
					id=GetIDFromFormDefs(pDef->GetProvFormDefsArray(),keys[2]);
					kind=14;
					readed=4;
				}
			}
		}
		break;
	case 11:	// Описание
		if(words>1)
		{
			id=atol(keys[1]);
			if(pMD->FindObject(id))
				kind=2;
			else
				id=0;
		}
		break;
	case 12:	// Общие Таблицы
		id=1;
		kind=17;
		readed=1;
	};
	CDocument* pDoc=NULL;
	if(id)
	{
		CString path;
		if(kind<4)
		{
			CModuleCont* pCont=NULL;
			CConfigCont::IDToTextModule(id,TypesOfModules[kind],path,&pCont,0);
			if(pCont)
				pDoc=pCont->GetTextDocument();
		}
		else
		{
			CTypedCont* pCont=NULL;
			CConfigCont::IDToPath(id,TypesOfModules[kind],path,&pCont,0);
			if(pCont)
				pDoc=pCont->GetWorkBook();
			if(readed<words)
			{
				if(!keys[readed].CompareNoCase("Диалог"))
					pDoc=((CWorkBookDoc*)pDoc)->GetDEditDocument();
				else if(!keys[readed].CompareNoCase("Модуль"))
					pDoc=((CWorkBookDoc*)pDoc)->GetTextDocument();
				else
				{
					int posdoc;
					pDoc=((CWorkBookDoc*)pDoc)->FindDocument(keys[readed],&posdoc);
				}
			}
		}
	}
	if(!pDoc)
		return SetError(E_FAIL,"Не найден объект с именем \"%s\"",name);
	*pVal=CICfgDoc::CreateDoc(pDoc);
	return S_OK;

}

STDMETHODIMP CICfgDocuments::Open(BSTR Path,ICfgDoc **pDoc)
{
	if(IsBadWritePtr(pDoc,sizeof(*pDoc)))
		return E_POINTER;
	_bstr_t bPath=Path;
	CString path=(LPCTSTR)bPath;
	*pDoc=NULL;
	if(!path.IsEmpty())
	{
		CDocument* pDoc1C=CConfigSvcImpl::m_pTheService->OpenFile(path);
		*pDoc=CICfgDoc::CreateDoc(pDoc1C);
	}
	return S_OK;
}

STDMETHODIMP CICfgDocuments::DocFromID(long id,DocTypes type,BSTR Path,BSTR kind,ICfgDoc **pRetDoc)
{
	if(IsBadWritePtr(pRetDoc,sizeof(*pRetDoc)))
		return E_POINTER;
	*pRetDoc=NULL;
	if(id<1)
		return SetError(E_FAIL,"Метод нельзя использовать для внешних файлов");
	CMetaDataCont* pMD=CIMetaData::m_pMetaData;
	CMetaDataObj* pObj=pMD->FindObject(id);
	if(!pObj)
		return SetError(E_FAIL,"Объект с ID=%i не найден",id);
	CString sKind=(LPCTSTR)_bstr_t(kind);
	CString sPath=CICfgDoc::m_tempPath+(LPCTSTR)_bstr_t(Path);
	for(int iKind=0;iKind<18;iKind++)
	{
		if(sKind==TypesOfModules[iKind])
			break;
	}
	if(iKind==18)
		return SetError(E_FAIL,"Неизвестный вид документа: \"%s\"",sKind);
	CString existPath;
	CDocument* pDoc=NULL;
	if(iKind<4)
	{
		CModuleCont* pCont=NULL;
		if(CConfigCont::IDToTextModule(id,sKind,existPath,&pCont,FALSE))
		{
			if(pCont)
				pDoc=pCont->GetTextDocument();
		}
	}
	else
	{
		CTypedCont* pCont=NULL;
		if(CConfigCont::IDToPath(id,sKind,existPath,&pCont,FALSE))
		{
			if(pCont)
			{
				CWorkBookDoc* pWB=pCont->GetWorkBook();
				if(pWB)
				{
					if(type==docText)
						pDoc=pWB->GetTextDocument();
					else if(type==docDEdit)
						pDoc=pWB->GetDEditDocument();
					else if(type==docTable)
					{
						int minPage=iKind==17?0:2;
						CStringArray tables;
						pWB->GetTablesName(tables);
						int Pages=tables.GetSize()+minPage;
						for(int i=minPage;i<Pages;i++)
						{
							if(!sPath.CompareNoCase(pWB->GetPagePathName(i)))
								break;
						}
						if(i<Pages)
							pDoc=pWB->FindDocument(tables[i-minPage],&Pages);
					}
					else
						pDoc=pWB;
				}
			}
		}
	}
	if(!pDoc)
		return SetError(E_FAIL,"Документ не найден");
	*pRetDoc=CICfgDoc::CreateDoc(pDoc);
	return S_OK;
}

STDMETHODIMP CICfgDocuments::New(DocTypes type,ICfgDoc **pDoc)
{
	if(IsBadWritePtr(pDoc,sizeof(*pDoc)))
		return E_POINTER;
	int rtc=-1;
	switch(type)
	{
	case docWorkBook:
		rtc=0;
		break;
	case docText:
		rtc=4;
		break;
	case docTable:
		rtc=1;
	}
	if(rtc<0)
		return SetError(E_FAIL,"Создание документов такого типа не поддерживается.");
	*pDoc=NULL;
	try{
		for(POSITION pos=pMainApp->GetFirstDocTemplatePosition();pos;)
		{
			CMyDocTemp* pTemp=(CMyDocTemp*)pMainApp->GetNextDocTemplate(pos);
			if(m_pUsefulRTC[rtc]==pTemp->GetDocClass())
			{
				*pDoc=CICfgDoc::CreateDoc(pTemp->OpenDocumentFile(NULL));
				break;
			}
		}
	}catch(...)
	{
		return SetError(E_FAIL,"Неизвестная ошибка");
	}
	return S_OK;
}

STDMETHODIMP CICfgDocuments::EnumAllTypedObj(IAllTypedObj **pRet)
{
	if(IsBadWritePtr(pRet,sizeof(*pRet)))
		return E_POINTER;
	*pRet=new CIAllTypedObj;
	return S_OK;
}

/*
STDMETHODIMP CICfgDocuments::DocFromIDKind(long id,BSTR kind,ICfgDoc **pDoc)
{
	if(IsBadWritePtr(pDoc,sizeof(*pDoc)))
		return E_POINTER;
	_bstr_t bType=kind;
	CString sType=(LPCTSTR)bType;
	return E_NOTIMPL;
}

STDMETHODIMP CICfgDocuments::DocFromPath(BSTR Path,ICfgDoc **ppDoc)
{
	if(IsBadWritePtr(ppDoc,sizeof(*ppDoc)))
		return E_POINTER;
	_bstr_t bPath=Path;
	CString sPath=(LPCTSTR)bPath;
	sPath=CICfgDoc::m_tempPath+sPath;
	CDocument* pDoc=CConfigCont::PathToDocument(sPath);
	*ppDoc=CICfgDoc::CreateDoc(pDoc);
	return S_OK;
}
*/