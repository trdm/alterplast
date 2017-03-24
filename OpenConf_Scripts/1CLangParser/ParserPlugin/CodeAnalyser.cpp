// CodeAnalyser.cpp : Implementation of CCodeAnalyser
#include "stdafx.h"

//#include "1cheaders.h"
//#include "1CPP\GlobalMethods.h"

#include "ParserPlugin.h"

#undef TypeOfMetaDataObj
#include "CodeAnalyser.h"
#include "..\CommonFunctions.h"

#include "..\Analyser.h"
#include "..\Types1C.h"

IConfigurator* CCodeAnalyser::pConfigurator;

void InitParser(char* aText);
int yyparse(CCode** parse_res, CLexAnalyzer_1C* Lexer);
extern int yydebug;
extern int ObjCount;

CTypes1C Types;



/////////////////////////////////////////////////////////////////////////////
// CCodeAnalyser

CCodeAnalyser::CCodeAnalyser()
{
	debug("constructor");

	pGlobalModule = NULL;
	pGlobalModuleCode = NULL;

	pLastModule = NULL;
	pLastType = NULL;
	pLastVar = NULL;

	pLocalContext = new CExecutionContext;

	GlobalContext.AddAutomaticVar("Перечисление", "Перечисление");
	GlobalContext.AddAutomaticVar("ФС", "ФС");
}

CCodeAnalyser::~CCodeAnalyser()
{
	if( pGlobalModule )
	{
		delete pGlobalModule;
		pGlobalModule = NULL;
	}
	if( pGlobalModuleCode )
	{
		delete pGlobalModuleCode;
		pGlobalModuleCode = NULL;
	}

	if( pLocalContext )
	{
		delete pLocalContext;
		pLocalContext = NULL;
	}

	if( pLastModule )
	{
		delete pLastModule;
		pLastModule = NULL;
	}

}

STDMETHODIMP CCodeAnalyser::Init(IConfigurator * pConf, BSTR * PluginName)
{
	setlocale(LC_ALL, ".ACP");
	pConfigurator = pConf;

	if (PluginName == NULL)
		return E_POINTER;
		
	CComBSTR  pStr = "CodeAnalyser";

	debug("Init, pConfigurator = %i", pConfigurator);

	//CMetaDataType1C mt(CString("Справочник"), CString("Номенклатура"), pConfigurator);

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::Done()
{
	debug("Done");
	return S_OK;//E_NOTIMPL;
}
STDMETHODIMP CCodeAnalyser::GetPluginCaps(PluginCaps capNum, VARIANT * pResult)
{
	if (pResult == NULL)
		return E_POINTER;
		
	return E_NOTIMPL;
}

STDMETHODIMP CCodeAnalyser::DoAction(PluginAction Action, VARIANT Param, VARIANT * pResult)
{
	if (pResult == NULL)
		return E_POINTER;
		
	return E_NOTIMPL;
}


STDMETHODIMP CCodeAnalyser::InterfaceSupportsErrorInfo(REFIID riid)
{
	static const IID* arr[] = 
	{
		&IID_ICodeAnalyser
	};
	for (int i=0; i < sizeof(arr) / sizeof(arr[0]); i++)
	{
		if (InlineIsEqualGUID(*arr[i],riid))
			return S_OK;
	}
	return S_FALSE;
}


//===================================================================================
STDMETHODIMP CCodeAnalyser::get_Line(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = nLine;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_Line(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	nLine = newVal;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_Column(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = nColumn;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_Column(long newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	nColumn = newVal;

	return S_OK;
}

ITextDoc* CCodeAnalyser::GetGlobalModuleDoc()
{
	ICfgDocuments* pDocs = NULL;
	ICfgDoc* pDoc = NULL;
	
	debug("GetGlobalModuleDoc 1, pConfigurator = %i", pConfigurator);
	pConfigurator->get_Documents(&pDocs);
	debug("GetGlobalModuleDoc 2  pDocs = %i", pDocs);

	CComBSTR ModName(L"Глобальный модуль");
	debug("GetGlobalModuleDoc 3");
	pDocs->get_item(ModName, &pDoc);
	debug("GetGlobalModuleDoc 4");

	return (ITextDoc*)pDoc;
}


CString* CCodeAnalyser::GetModuleText()
{
	/*
	CGetDoc7* pDoc = GetActiveDocument();
	debug("pDoc = %i", pDoc);
	if(pDoc)
	{
		CBLModule7 * pMod = pDoc->m_pBLModule;
		return &(pMod->GetInternalData()->mSource);
	}
	*/
	return NULL;
}

ITextDoc* CCodeAnalyser::GetTextDoc()
{
	ICfgWindows* pWindows = NULL;
	ICfgWindow* pActiveWnd = NULL;
	ICfgDoc* pDoc = NULL;

	pConfigurator->get_Windows(&pWindows);
	debug("GetTextDoc 1, pWindows = %i", pWindows);
	HRESULT res = pWindows->get_ActiveWnd(&pActiveWnd);

	debug("GetTextDoc 2  pActiveWnd = %i", pActiveWnd);
	pActiveWnd->get_Document(&pDoc);
	debug("GetTextDoc 3");

	enum DocType type;
	pDoc->get_Type((enum DocTypes*)&type);
	if( type == docWorkBook )
	{
		IWorkBook* pWorkBook = (IWorkBook*)pDoc;
		COleVariant page("Модуль");
		pWorkBook->get_Page(page, &pDoc);
		debug("GetTextDoc 4, pDoc = %i", pDoc);
		pDoc->get_Type((enum DocTypes*)&type);
		debug("GetTextDoc 5");
	}

	if( type != docText )
	{
		CComBSTR err("Это не текст!!!  ");
		BSTR Name;
		pDoc->get_Name(&Name);
		err.AppendBSTR(Name);
		pConfigurator->Message(err, mRedErr);
		err.Empty();
		return NULL;
	}

	debug("GetTextDoc 6");

	return (ITextDoc*)pDoc;
}

CCode* CCodeAnalyser::ParseModule(CString& strModule)
{
	CCode *parse_res = NULL;

	CLexAnalyzer_1C Lexer(strModule);
	//yydebug = 1;
	yyparse(&parse_res, &Lexer);

	return parse_res;
}

extern int ObjCount;
void CCodeAnalyser::AnalyseModule(CString& strModule, long line, long col)
{
	if( pLastModule )
		delete pLastModule;
	pLastModule = NULL;
	pLastVar = NULL;

	int oc = ObjCount;
	CCode *ModuleCode = ParseModule(strModule);

	if( ModuleCode != NULL )
	{
		pLastModule = new CModule(ModuleCode);
		pLastModule->SetParentContext(pLocalContext);
		
		if( pGlobalModule )
		{
			pGlobalModule->SetParentContext(&GlobalContext);
			pLocalContext->SetParentContext(pGlobalModule);
		}
		else
		{
			pLocalContext->SetParentContext(&GlobalContext);
		}

		if( line >= 0 )
		{
			//отработаем стандартные процедуры входа
			pLastModule->Analyse("ПриОткрытии");
			pLastModule->Analyse("ПослеСозданияФормы");
			pLastModule->Analyse("ПослеОткрытия");
		}

		pLastModule->DesiredLine = line + 1;
		pLastModule->DesiredCol = col;
		
		pLastModule->Analyse();
		strType = pLastModule->TypeFound;

		delete ModuleCode;

		//Msg("Not deleted: %i, ObjCount = %i", ObjCount - oc, ObjCount);
		debug("Not deleted: %i, ObjCount = %i", ObjCount - oc, ObjCount);
	}
}

void CCodeAnalyser::AnalyseGlobalModule(CString& strModule)
{
	if( pGlobalModule )
	{
		delete pGlobalModule;
		pGlobalModule = NULL;
	}
	if( pGlobalModuleCode )
	{
		delete pGlobalModuleCode;
		pGlobalModuleCode = NULL;
	}
	CMemCollector::Free();

	pGlobalModuleCode = ParseModule(strModule);

	if( pGlobalModuleCode != NULL )
	{
		pGlobalModule = new CModule(pGlobalModuleCode);
		
		pGlobalModule->SetParentContext(&GlobalContext);

		//анализируем объявлния и код в конце модуля
		pGlobalModule->DesiredLine = 1;
		pGlobalModule->Analyse();
		//анализируем ПриНачалеРаботыСистемы
		pGlobalModule->Analyse("ПриНачалеРаботыСистемы");
	}
}

STDMETHODIMP CCodeAnalyser::AnalyseModule(BSTR bstrModuleBody, long line, long col)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	clock_t StartTime = clock();


	//CString str(bstrModuleBody);

	//AnalyseModule(str, line, col);

	TimeSpent = clock() - StartTime;

	return S_OK;
}

//#define _USE_1CPP_HEADERS_
STDMETHODIMP CCodeAnalyser::GetTypeInPos()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	clock_t StartTime = clock();

#ifndef _USE_1CPP_HEADERS_
	ITextDoc* pTextDoc = GetTextDoc();
	if( pTextDoc == NULL )
		return S_OK;

	CComBSTR strModule;
	pTextDoc->get_text(&strModule);

	CString str(strModule);
	long line, col;
	pTextDoc->get_SelEndLine(&line);
	pTextDoc->get_SelEndCol(&col);

	AnalyseModule(str, line, col);
	strModule.Empty();

#else

	ITextDoc* pTextDoc = GetTextDoc();
	if( pTextDoc == NULL )
		return S_OK;

	CString* pstrModule = GetModuleText();
	if( pstrModule == NULL )
		return S_OK;

	long lines, line, col;
	pTextDoc->get_LineCount(&lines);
	if( lines > 3000 )
	{
		//Модуль слишком большой, так что заанализируем только нужный метод
		pTextDoc->get_SelEndLine(&line);
		pTextDoc->get_SelEndCol(&col);
		AnalyseModule(*pstrModule, line, col);
	}
	else
	{
		//Модуль маленький - не будем стесняться, заанализируем всё что есть
		AnalyseModule(*pstrModule, -1, -1);
	}

#endif

	TimeSpent = clock() - StartTime;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_Type(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = strType;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_TimeSpent(long *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pVal = TimeSpent;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_TypePropertyName(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = TypePropertyName;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_TypePropertyName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TypePropertyName = newVal;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_TypePropertyType(BSTR* pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = TypePropertyType;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_TypePropertyType(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TypePropertyType = newVal;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_TypeName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = TypeName;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_TypeName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	TypeName = newVal;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::AddType(BSTR TypeName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())


	CString Type(TypeName);
	pLastType = Types.GetType(Type);
	if( pLastType == NULL )
	{
		pLastType = new CType1C(Type);
		Types.AddType(Type, Type, pLastType);
	}

	return S_OK;
}


STDMETHODIMP CCodeAnalyser::AddBaseType(BSTR bstrBaseType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pLastType )
	{
		CString BaseType(bstrBaseType);
		CType1C* pBaseType = NULL;
		if( !BaseType.IsEmpty() )
		{
			pBaseType = Types.GetType(BaseType);
			if( pBaseType )
				pLastType->AddBaseType(pBaseType);
		}
	}

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::AddTypeProperty(BSTR PropName, BSTR PropType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())


	CString Name(PropName), Type(PropType);
	pLastType->AddProperty(Name, Name, Type);

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::FindType(BSTR bstrTypeName, BOOL* pFound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString sTypeName(bstrTypeName);
	pLastType = Types.GetType(sTypeName);

	if( pLastType )
	{
		*pFound = TRUE;
		strType = sTypeName;
	}
	else
	{
		*pFound = FALSE;
		strType = "";
	}


	return S_OK;
}

STDMETHODIMP CCodeAnalyser::RemoveAllTypes()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	Types.RemoveAll();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::RemoveType(BSTR bstrTypeName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strTypeName = bstrTypeName;
	Types.RemoveType(strTypeName);

	return S_OK;
}


STDMETHODIMP CCodeAnalyser::StartPropsIteration()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pLastType )
		pLastType->StartPropsIteration();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::NextProperty(BSTR *pPropName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR str("");
	if( pLastType )
	{
		CString strProp;
		if( pLastType->NextProp(strProp) )
			str = strProp;
	}

	str.CopyTo(pPropName);

	return S_OK;
}


STDMETHODIMP CCodeAnalyser::FindVar(BSTR bstrVarName, BOOL *pFound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pFound = FALSE;
	pLastType = NULL;
	strType = "";

	if( !pLastModule )
		return S_OK;

	CMethod* pMethod = pLastModule->GetMethod(pLastModule->MethodFound, false);
	if( !pMethod )
		return S_OK;

	CString strVarNameToFind(bstrVarName);
	pLastVar = pMethod->GetVar(strVarNameToFind);
	if( pLastVar )
	{
		pLastType = CTypes1C::GetType(pLastVar->Type);
		strType = pLastVar->Type;
		*pFound = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::FindTypeProp(BSTR bstrPropName, BOOL *pFound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pFound = FALSE;
	if( !pLastType )
		return S_OK;

	CString strPropName(bstrPropName);
	TypePropertyType = pLastType->GetPropType(strPropName);

	*pFound = !(TypePropertyType.IsEmpty());

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::FindVarProp(BSTR bstrPropName, BOOL *pFound)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	*pFound = FALSE;

	if( pLastVar )
	{
		VarPropName = bstrPropName;
		VarPropType = pLastVar->GetPropType(VarPropName);
		if( !VarPropType.IsEmpty() )
			*pFound = TRUE;
	}

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_VarPropName(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = VarPropName;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_VarPropName(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VarPropName = newVal;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::get_VarPropType(BSTR *pVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR Str = VarPropType;
	Str.CopyTo(pVal);
	Str.Empty();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::put_VarPropType(BSTR newVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	VarPropType = newVal;

	return S_OK;
}


STDMETHODIMP CCodeAnalyser::StartVarPropsIteration()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pLastVar )
		pLastVar->StartPropsIteration();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::NextVarProp(BSTR *pPropName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CComBSTR str("");
	if( pLastVar )
	{
		CString strProp;
		if( pLastVar->NextProp(strProp) )
			str = strProp;
	}

	str.CopyTo(pPropName);

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::AddExemplarModifyingMethod(BSTR bstrMethodName, long nParam_PropName, long nParam_PropType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pLastType )
	{
		CString MethodName = bstrMethodName;
		pLastType->AddExemplarModifyingMethod(MethodName, nParam_PropName, nParam_PropType);
	}

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::AnalyseGlobalModule()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	ITextDoc* pGM = GetGlobalModuleDoc();

	if( pGM == NULL )
		return S_OK;

	long nLines;
	pGM->get_LineCount(&nLines);
	if( nLines < 5000 ) //Ограничение макс. размера. Надо будет сделать настройкой
	{
		CComBSTR strModule;
		pGM->get_text(&strModule);

		CString str(strModule);
		int oc = ObjCount;

		AnalyseGlobalModule(str);

		debug("AnalyseGlobalModule Not deleted: %i, ObjCount = %i", ObjCount - oc, ObjCount);

		strModule.Empty();
	}

	/*
	debug("AnalyseGlobalModule 1");
	CModuleCont* pCont=NULL;
	CString path, src;
	debug("AnalyseGlobalModule 1");
	CConfigCont::IDToTextModule(1,"ModuleText", path, &pCont, 0);
	debug("AnalyseGlobalModule 2");
	if(pCont)
	{
	debug("AnalyseGlobalModule 3");
		CTextDocument* pDoc=static_cast<CTextDocument*>(pCont->GetTextDocument());
		pDoc->GetText(src);
	}
	debug("AnalyseGlobalModule 4");
	int oc = ObjCount;
	AnalyseGlobalModule(src);
	debug("AnalyseGlobalModule Not deleted: %i, ObjCount = %i", ObjCount - oc, ObjCount);
	*/


	return S_OK;
}


STDMETHODIMP CCodeAnalyser::ResetLocalContext()
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	if( pLocalContext )
	{
		delete pLocalContext;
	}
	pLocalContext = new CExecutionContext;

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::LC_AddVar(BSTR bstrVarName, BSTR bstrVarType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strVarName = bstrVarName, strVarType = bstrVarType;
	pLocalContext->AddDeclaredVar(strVarName, strVarType);

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::LC_AddFunction(BSTR bstrFuncName, BSTR bstrFuncType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strFuncName = bstrFuncName, strFuncType = bstrFuncType;
	//pLocalContext->AddMethod();

	return S_OK;
}

STDMETHODIMP CCodeAnalyser::LC_AddVarProperty(BSTR bstrVarName, BSTR bstrPropName, BSTR bstrPropType)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState())

	CString strVarName = bstrVarName,
		strPropName = bstrPropName,
		strPropType = bstrPropType;

	CVariable* pVar = pLocalContext->GetDeclaredVar(strVarName);
	if( pVar )
	{
		pVar->AddProperty(strPropName, strPropType);
	}

	return S_OK;
}


