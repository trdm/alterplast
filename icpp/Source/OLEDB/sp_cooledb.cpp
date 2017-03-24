// Мирошниченко Кирилл aka spock
// #163860836
// e-mail: spock@km.ru
//
#include "stdafx.h"
#include "sp_cooledb.h"
#include "OleDbResult.h"
#include "../Utils/StringAlgo.h"
#include "../Utils/ErrorProcessing.h"
#include "../Utils/SystemInterface.h"
#include "../Utils/UserInterface.h"
#include "Utils.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

using namespace OleDb;
//_____________________________________________________________________________
//

extern CDataBase7* pDataBase7;
extern CMetaDataCont* pMetaDataCont;
extern CBkEndUI* pBkEndUI;

extern char szMNPErrorPrefix[];

BEGIN_BL_METH_MAP(COLEDBCommand)
    BL_METH_FUNC_DEF_PARAM("ExecuteStatement",	 "ВыполнитьИнструкцию", 3, &funcExecuteStatement, &defsExecuteStatement)
	BL_METH_FUNC_DEF_PARAM("Execute",			 "Выполнить", 1, &funcExecute, &defsExecute)
	
	BL_METH_PROC_DEF_PARAM("PutObjectList",		 "УложитьСписокОбъектов", 3, &procPutObjectList, &defsPutObjectList)

	BL_METH_PROC("Close",			 "Закрыть", 0, &procClose)
	BL_METH_PROC("Debug",			 "Отладка", 1, &procDebug)
	BL_METH_PROC("SetTextParam",	 "УстановитьТекстовыйПараметр", 2, &procSetTextParam)
	BL_METH_PROC("SetTempTablesDir", "УстановитьКаталогВремТаблиц", 1, &procSetTempTablesDir)
	BL_METH_PROC("SetExecTimeOut",	 "УстановитьТаймаутВыполнения", 1, &procSetExecTimeOut)
	BL_METH_PROC("AddParam",		 "ДобПараметр", 4, &procAddParam)
	BL_METH_PROC("SetParam",		 "УстановитьПараметр", 2, &procSetParam)
	BL_METH_PROC("DeleteParam",		 "УдалитьПараметр", 1, &procDeleteParam)
	BL_METH_PROC("DeleteParams",	 "УдалитьПараметры", 0, &procDeleteParams)
	BL_METH_PROC("Prepare",			 "Подготовить", 1, &procPrepare)
	BL_METH_PROC("Unprepare",		 "ОтменитьПодготовку", 0, &procUnprepare)
	
	BL_METH_FUNC("ParamCnt",		 "КолвоПараметров", 0, &funcParamCnt)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(COLEDBCommand)
END_BL_PROP_MAP()

IMPLEMENT_MY_NONCREATE_CONTEXT(COLEDBCommand, "OLEDBCommand", "OLEDBCommand");

//_____________________________________________________________________________
//
COLEDBCommand::COLEDBCommand(CComPtr<IDBCreateCommand> pIDBCreateCommand) :
	CMyContextBase(), m_IsDebugMode(false), m_nRowsChunk (DEFAULT_ROWS_CHUNK)
{
	HRESULT hr;
	CString strErr;

	m_bCreated	 = TRUE;
	::UuidCreateNil(&m_GUID);
	m_TempTablesDir.Empty();

	m_ExecTimeout = -1;
	m_bPrepared = FALSE;

	hr = pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, reinterpret_cast<IUnknown**>(&m_pICommandText));
	if(FAILED(hr))
	{
		strErr = "FAILED! IDBCreateCommand::CreateCommand()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return;
	}

	m_TmpTblsList.RemoveAll();
}

//_____________________________________________________________________________
//
COLEDBCommand::~COLEDBCommand()
{
	RemoveAllTmpTables();

	if(m_pICommandText)
	{
		m_pICommandText.Release();
	}

	m_bCreated = FALSE;
	m_bPrepared = FALSE;

	m_ParamSet.DeleteParams();
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsExecute(int nParam, CValue* param) const
{
	*param = "";
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsExecuteStatement(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0: *param = "";
		break;
	case 1: param->Reset();
		break;
	case 2: *param = CNumeric(1);
	};
	
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsPutObjectList(int nParam, CValue* param) const
{
	if(nParam == 2)
		*param = "";
	
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcExecuteStatement(CValue& rValue, CValue** ppValue)
{
	try {
		CString strErr;

		CValueTable* pVT	 = NULL;
		CPtrArray* pVL		 = NULL;
		CVTExtended* pVTExt	 = NULL;

		// второй параметр не передан - создаем ТЗ
		//
		if(ppValue[1]->GetTypeCode() == UNDEFINE_TYPE_1C)
		{
			rValue.CreateObject("ТаблицаЗначений");
			pVT = CValue2VT(rValue);
			if (!pVT) {
				Utils::CSystemInterface::Instance ().RuntimeError (
					"Не удалось создать таблицу значений для результата запроса");
				return FALSE;
			}
		}
		else
		{
			if(!(pVT = CValue2VT(*ppValue[1])))
			{
				pVTExt = CValue2VTExt (*ppValue[1]);
			}
			rValue = *ppValue[1];
		}

		if(!pVT && !pVL && !pVTExt)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Не удалось получить возвращаемый объект");
			return FALSE;
		}

		if(!m_bCreated)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Использование метода без создания команды");
			return FALSE;
		}

		DBPARAMS *pParams = NULL;
		CString strSQL;

		std::vector<DBPARAMBINDINFO> PrmBindInfo;
		std::vector<DBBINDING> PrmDBBinding;
		std::vector<ULONG> PrmOrdinals;
		std::vector<BYTE> Buffer;
		
		if(!m_bPrepared)
		{
			strSQL = ppValue[0]->GetString();
		}
		else
		{
			pParams = m_ParamSet.GetDBPARAMS();
		}

		long nRowsAffected = 0;
		CResultSequentialAccessProxy Result = ExecuteStatementExternalSequential (
			strSQL, nRowsAffected, pParams);
		if (Result.IsEmpty ()) return TRUE;

		bool ClearContainer = ((long)ppValue[2]->GetNumeric()) != 0;

		if (pVT) ResultToValueTable (Result, *pVT, ClearContainer);
		else if (pVTExt) ResultToIndexedTable (Result, *pVTExt, ClearContainer);

		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно выполнить запрос");
	}

	return FALSE;
} // COLEDBCommand::funcExecuteStatement

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcExecute(CValue& rValue, CValue** ppValue)
{
	try {
		CString strErr;
		HRESULT hr;

		if(!m_bCreated)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Использование метода без создания команды!");
			return FALSE;
		}
		
		DBPARAMS *pParams = NULL;
		CString strSQL;
		
		std::vector<DBPARAMBINDINFO> PrmBindInfo;
		std::vector<DBBINDING> PrmDBBinding;
		std::vector<ULONG> PrmOrdinals;
		std::vector<BYTE> Buffer;

		if(!m_bPrepared)
		{
			strSQL = ppValue[0]->GetString();

			if(!ParseQuery(strSQL))
			{
				rValue = 0L;
				return FALSE;
			}
		}
		else
		{
			pParams = m_ParamSet.GetDBPARAMS();
		}

		long nRowsAffected = 0;

		hr = Execute(strSQL, pParams, &nRowsAffected, NULL, false, m_bPrepared);
		if(!hr)
		{
			rValue = 0L;
			return FALSE;
		}
		
		rValue	 = nRowsAffected;
		
		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно выполнить команду");
	}

	return FALSE;
} // COLEDBCommand::funcExecute

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procPutObjectList(CValue** ppValue)
{
	try {
		HRESULT hr = PutObjectsList(ppValue);
		if(!hr)
			return FALSE;
		
		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно создать список объектов");
	}

	return FALSE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procClose(CValue** ppValue)
{
	RemoveAllTmpTables();

	if(m_pICommandText)
		m_pICommandText.Release();

	m_bCreated = FALSE;
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDebug(CValue** ppValue)
{
	SetDebugMode (0 == ppValue[0]->GetNumeric() ? false : true);
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetTextParam(CValue** ppValue)
{
	try {
		SetTextParam (ppValue[0]->GetString(), *ppValue[1]);
		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно установить параметр");
	}

	return FALSE;
} // COLEDBCommand::procSetTextParam

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetTempTablesDir(CValue** ppValue)
{
	try {
		CString strErr;

		CString TempDir = ppValue[0]->GetString();
		TempDir.TrimRight ();

		if(TempDir.IsEmpty())
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Путь к каталогу временных таблиц не может быть пустой строкой");
			return FALSE;
		}

		CString LastChar = TempDir.Right (1);

		if (LastChar != "\\" && LastChar != "/") TempDir += "\\";

		if (TempDir.Find (' ') > 0)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Путь к каталогу временных таблиц не может включать пробелы");
			return FALSE;
		}
		
		m_TempTablesDir = TempDir;
		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно установить каталог временных таблиц");
	}

	return FALSE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetExecTimeOut(CValue** ppValue)
{
	m_ExecTimeout = ppValue[0]->GetNumeric();
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procAddParam(CValue** ppValue)
{
	try {
		CString strErr;

		if(m_bPrepared)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"После выполнения операции подготовки "
				"добавление новых параметров не допускается");
			return FALSE;
		}

		DWORD dwParamIO = 0;
		switch(ppValue[0]->GetNumeric())
		{
		case 1: dwParamIO = DBPARAMIO_INPUT;
			break;
		case 2: dwParamIO = DBPARAMIO_OUTPUT;
			break;
		case 3: dwParamIO = DBPARAMIO_INPUT|DBPARAMIO_OUTPUT;
			break;
		default:
			{
				Utils::CSystemInterface::Instance ().RuntimeError (
					"Неверный параметр (1)!");
				return FALSE;
			}
		}

		WORD nType = ppValue[1]->GetNumeric();
		if(0 == COLEDBParamHlpr::GetTypeSize(nType))
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Неверный тип параметра (2)");
			return FALSE;
		}

		m_ParamSet.AddParam(dwParamIO, nType, ppValue[2]->GetNumeric(), ppValue[3]->GetNumeric());

		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно добавить параметр");
	}

	return FALSE;
} // COLEDBCommand::procAddParam

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetParam(CValue** ppValue)
{
	try {
		CString strErr;

		if(!m_ParamSet.SetParamValue(ppValue[0]->GetNumeric(), ppValue[1], strErr))
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				(LPCSTR)strErr);
			return FALSE;
		}

		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно установить параметр");
	}

	return FALSE;
} // COLEDBCommand::procSetParam
	
//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDeleteParam(CValue** ppValue)
{
	try {
		int nParamNum = ppValue[0]->GetNumeric();
		
		if(!m_ParamSet.DeleteParam(nParamNum))
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Неверный номер параметра");
			return FALSE;
		}

		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно удалить параметр");
	}

	return FALSE;
} // COLEDBCommand::procDeleteParam

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDeleteParams(CValue** ppValue)
{
	m_ParamSet.DeleteParams();

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procPrepare(CValue** ppValue)
{
	try {
		CString strErr;
		
		m_bPrepared = FALSE;
		
		CString strSQL(ppValue[0]->GetString());

		if(!ParseQuery(strSQL))
			return FALSE;
		
		if(!Prepare(strSQL, strErr))
		{
			Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
			return FALSE;
		}

		m_bPrepared = TRUE;

		return TRUE;
	}
	catch (...) {
		Utils::ProcessErrors ("Невозможно выполнить подготовку");
	}

	return FALSE;
} // COLEDBCommand::procPrepare

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procUnprepare(CValue** ppValue)
{
	CString strErr;
	HRESULT hr;
	
	hr = Unprepare(strErr);
	if(!hr)
	{
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		return FALSE;
	}

	m_bPrepared = FALSE;

	return TRUE;
} // COLEDBCommand::procUnprepare

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcParamCnt(CValue& rValue, CValue** ppValue)
{
	rValue = m_ParamSet.GetCount();
	return TRUE;
}

//_____________________________________________________________________________
//
void COLEDBCommand::GetErrorDescription(CString& strError) const
{
	CComPtr<IErrorInfo> spIErrorInfo;

	if(SUCCEEDED(::GetErrorInfo(0, &spIErrorInfo)))
	{
		BSTR errDescr;
		spIErrorInfo->GetDescription(&errDescr);

		try {
			strError += ": ";
			strError += errDescr;

			::SysFreeString(errDescr);
		}
		catch (...) {
			::SysFreeString(errDescr);
			throw;
		}
	}
} // COLEDBCommand::GetErrorDescription

//_____________________________________________________________________________
//
BOOL COLEDBCommand::ParseQuery(CString& strQuery)
{
	// OUT: [S_OK, S_FALSE]
	//

	CString strErr;

	strQuery.TrimLeft();
	strQuery.TrimRight();

	if(strQuery.IsEmpty())
	{
		strErr = "A SQL statement is empty!";
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		
		return FALSE;
	}

	m_MetaNameParser.SetQueryText(strQuery);
	
	try
	{
		m_MetaNameParser.Parse();
	}
	catch(CMNPException* MNPException)
	{
		strErr = szMNPErrorPrefix + MNPException->GetErrorDescr();
		MNPException->Delete();

		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);

		return FALSE;
	}

	strQuery = m_MetaNameParser.GetQueryText();

	if(m_IsDebugMode) {
		Utils::CUserInterface::Instance ().AddMessage ((LPCSTR)strQuery);
	}
	
	return TRUE;
} // COLEDBCommand::ParseQuery

//_____________________________________________________________________________
//
bool COLEDBCommand::Execute (const CString& strSQL, DBPARAMS* pParams,
	long* pnRowsAffected, IUnknown** pIRowset, bool bQuiet /*= false*/,
	bool bPrepared /*= false*/, bool DirectAccess /*= false*/) const
{
	HRESULT hr;
	CString strErr;

	if(!bPrepared)
	{
		Utils::StringToUnicodeConverter_t Converter;
		hr = m_pICommandText->SetCommandText(DBGUID_DEFAULT,
			Converter.Convert (strSQL));
		if(FAILED(hr))
		{
			if(bQuiet)
				return false;
			
			strErr = "FAILED! ICommandText::SetCommandText()";
			GetErrorDescription(strErr);
			Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
			return false;
		}
	}

	if(m_ExecTimeout >= 0)
	{
		CComPtr<ICommandProperties> spCommProperties;

		hr = m_pICommandText->QueryInterface(&spCommProperties);
		if(FAILED(hr))
		{
			strErr = "FAILED! ICommandProperties::QueryInterface()";
			GetErrorDescription(strErr);
			Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
			return false;
		}

		CDBPropSet PropSet(DBPROPSET_ROWSET);
		PropSet.AddProperty(DBPROP_COMMANDTIMEOUT, m_ExecTimeout);
		
		hr = spCommProperties->SetProperties(1, &PropSet);
		if(FAILED(hr))
		{
			strErr = "FAILED! ICommandProperties::SetProperties(): Не удалось установить property";
			Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
			
			return false;
		}
	}

	if(!pIRowset) {
		hr = m_pICommandText->Execute(NULL, IID_NULL, pParams, pnRowsAffected,
			NULL);
	}
	else {
		if (!DirectAccess) {
			hr = m_pICommandText->Execute(NULL, IID_IRowset, pParams,
				pnRowsAffected,	pIRowset);
		}
		else {
			hr = m_pICommandText->Execute(NULL, IID_IRowsetLocate, pParams,
				pnRowsAffected, pIRowset);
		}
	}

	if(FAILED(hr))
	{
		if(bQuiet)
			return false;

		strErr = "FAILED! ICommandText::Execute()";
		GetErrorDescription(strErr);
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		return false;
	}

	return true;
} // COLEDBCommand::Execute

//_____________________________________________________________________________
//
bool COLEDBCommand::CreateAccessor(DBACCESSORFLAGS eDBACCFLAGS,
	ULONG cbRowSize, IAccessor_& pIAccessor, IUnknown* pIUnknown,
	HACCESSOR* phAccessor, ULONG& cCntItems,
	std::vector<DBBINDING>& DBBindings) const
{
	// OUT: [S_FALSE, S_OK]
	//
	HRESULT hr;
	CString strErr;
	
	hr = pIUnknown->QueryInterface(IID_IAccessor, (void**)&pIAccessor);
	if(FAILED(hr))
	{
		strErr = "FAILED! IUnknown::QueryInterface()";
		GetErrorDescription(strErr);
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		return false;
	}
	
	std::vector<DBBINDSTATUS> DBBindStatus;
	
	DBBindStatus.resize (cCntItems);
	
	hr = pIAccessor->CreateAccessor(eDBACCFLAGS, cCntItems,
		&DBBindings [0], cbRowSize, phAccessor, &DBBindStatus [0]);
	if(FAILED(hr))
	{
		strErr = "FAILED! IAccessor::CreateAccessor()";
		
		switch(hr)
		{
		case DB_E_BADACCESSORFLAGS:
			strErr += " [One or more accessor flags were invalid]";
			break;
			
		case DB_E_BYREFACCESSORNOTSUPPORTED:
			strErr += " [Reference accessors are not supported by this provider]";
			break;
			
		case DB_E_ERRORSOCCURRED:
			strErr += " [Multiple-step OLE DB operation generated errors]";
			break;
			
		case DB_E_NOTREENTRANT:
			strErr += " [Consumer's event handler called a non-reentrant method in the provider]";
			break;
			
		case DB_E_NULLACCESSORNOTSUPPORTED:
			strErr += " [Null accessors are not supported by this provider]";
			break;
			
		default:
			break;
		}
		
		GetErrorDescription(strErr);
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		
		return false;
	}
	
	return true;
} // COLEDBCommand::CreateAccessor

//_____________________________________________________________________________
//
BOOL COLEDBCommand::PutObjectsList(CValue** ppValue)
{
	CString strErr;
	HRESULT hr;

	// Сразу позаботимся, чтобы параметр был пустым
	//
	CValue* pTableName = ppValue[1];
	pTableName->Reset();

	// Сгенерируем новый GUID
	//
	::UuidCreate(&m_GUID);
	
	unsigned char* strGUID;
	::UuidToString(&m_GUID, &strGUID);

	CString TableName;

	try {
		// Получим текстовое значение GUID
		//
		TableName = strGUID;
		TableName = m_TempTablesDir + TableName;
		::RpcStringFree(&strGUID);
	}
	catch (...) {
		::RpcStringFree(&strGUID);
		throw;
	}

	CString strSQL;
	
	// Создаем временную таблицу
	//
	strSQL.Format("CREATE TABLE %s (VAL C(9) NOT NULL, ISFOLDER N(1,0) NOT NULL)",
		(LPCSTR)TableName);

	long nRowsAffected = 0;
	
	hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
	if(!hr)
	{
		strErr.Format("Не удалось создать временную таблицу!");
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);

		return FALSE;
	}

	// Заполняем список: временные таблицы
	//
	m_TmpTblsList.AddTail(TableName);

	CString RefName = ppValue[2]->GetString();

	BOOL ToDeleteFldr = FALSE;

	if(ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
	{
		// Если передан список значений
		//
		CBLContext* pValCont = ppValue[0]->GetContext();

		if(pValCont && !strcmp(pValCont->GetRuntimeClass()->m_lpszClassName, "CValueListContext"))
		{
			// TODO: Кривовато
			CPtrArray* pVL = *(reinterpret_cast<CPtrArray**>((reinterpret_cast<char*>(pValCont)) + 0x30));
			
			int nlistSize = pVL->GetSize();
			for(int i = 0; i < nlistSize; i++)
			{
				CValue* pValItem = reinterpret_cast<CValue*>(pVL->GetAt(i));

				hr = PutAggregateObjects(pValItem, &TableName, &RefName);
				if(!hr) return FALSE;
			}

			if(nlistSize > 0) ToDeleteFldr = TRUE;
		}
		else
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Недопустимое значение первого аргумента метода!");
			return FALSE;
		}
	}
	else if(ppValue[0]->GetTypeCode() > DATE_TYPE_1C)
	{
		// Если передан агрегатный объект
		//
		CValue* pAggregate	 = ppValue[0];

		hr = PutAggregateObjects(pAggregate, &TableName, &RefName);
		if(!hr) return FALSE;

		ToDeleteFldr = TRUE;
	}

	if(ToDeleteFldr)
	{
		// Теперь нужно из временной таблицы удалить строки - группы (isfolder=1)
		//
		strSQL = "DELETE FROM %tmptblnm WHERE ISFOLDER = 1";
		strSQL.Replace("%tmptblnm", TableName);

		nRowsAffected = 0;
		
		hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
		if(!hr)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Не удалось удалить группы из временной таблицы!");
			
			return FALSE;
		}

		strSQL = "USE";

		nRowsAffected = 0;

		hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
		if(!hr)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Не удалось сменить контекст базы данных!");
			
			return FALSE;
		}
	}

	// Все, теперь можно вернуть имя временной таблицы
	//
	*pTableName	 = TableName;
	
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::PutAggregateObjects (CValue* pValue,
	CString const * pTableName, CString const * pRefName)
{
	CString strErr, strSQL;

	CSbCntTypeDef* pSbCntTypeDef = NULL;

	if(!pRefName->IsEmpty())
	{
		pSbCntTypeDef = pMetaDataCont->GetSTypeDef(*pRefName);
		
		if(!pSbCntTypeDef)
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Недопустимое значение третьего пераметра метода!");
			
			return FALSE;
		}
	}

	int nLevelsCount = 1;
	int nIsFldr		 = 2;
	
	if(!pValue->IsEmpty() && !pRefName->IsEmpty())
	{
		CValue arValue;
		arValue.Reset();

		pValue->LinkContext(FALSE);
		CBLContext* pValCont = pValue->GetContext();
		pValCont->CallAsFunc(pValCont->FindMethod("IsGroup"), arValue, NULL);
		nIsFldr = 0 == arValue.GetNumeric() ? 2 : 1;

		nLevelsCount = pSbCntTypeDef->GetLevelsLimit();
	}

	CString ObjStr = "'";
	ObjStr += CMetaDataWork::GetObjDBString(*pValue, CMetaDataWork::ShortString);
	ObjStr += "'";

	strSQL.Empty();
	strSQL.Format("INSERT INTO %s (VAL, ISFOLDER) VALUES (%s, %d)",
		(LPCSTR)(*pTableName), (LPCSTR)ObjStr, nIsFldr);

	long nRowsAffected = 0;
	
	if(!Execute(strSQL, NULL, &nRowsAffected, NULL))
	{
		Utils::CSystemInterface::Instance ().RuntimeError (
			"Не удалось добавить корневую строку!");

		return FALSE;
	}

	// Если не передали вид справочника, то дальше не выполняем
	//
	if(pRefName->IsEmpty() || nLevelsCount < 2) return TRUE;

	strSQL.Empty();

	strSQL = "INSERT INTO %tmptblnm (VAL, ISFOLDER)\r\n\
			SELECT SC.ID as VAL, SC.ISFOLDER as ISFOLDER\r\n\
			FROM %sctbl as SC\r\n\
			WHERE\r\n\
			(SC.PARENTID IN (SELECT T.VAL FROM %tmptblnm as T))\r\n\
			AND (SC.ID NOT IN (SELECT TT.VAL FROM %tmptblnm as TT))";

	strSQL.Replace("%tmptblnm", (*pTableName));
	
	CString RefTable;
	RefTable.Format("sc%d", pSbCntTypeDef->GetID());
	strSQL.Replace("%sctbl", (LPCSTR)RefTable);

	if(!Prepare(strSQL, strErr))
	{
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		return FALSE;
	}

	// Нужно заполнять временную таблицу итеративно, проход - спуск ниже уровнем
	//
	do
	{
		nRowsAffected = 0;

		if(!Execute(strSQL, NULL, &nRowsAffected, NULL, FALSE, TRUE))
		{
			Utils::CSystemInterface::Instance ().RuntimeError (
				"Не удалось добавить строки!");
			
			return FALSE;
		}
	} while(nRowsAffected > 0);

	if(!Unprepare(strErr))
	{
		Utils::CSystemInterface::Instance ().RuntimeError ((LPCSTR)strErr);
		return FALSE;
	}

	return TRUE;
} // COLEDBCommand::PutAggregateObjects

//_____________________________________________________________________________
//
void COLEDBCommand::RemoveTmpTable(CString& strTmpTblName)
{
	// Значит уже закрыли команду, вообще такого не должно быть
	// но лучше перебздеть, чем недобздеть
	//
	if(!m_pICommandText) return;

	CString strErr;
	
	CString strSQL("DROP TABLE ");
	strSQL += strTmpTblName;

	long nRowsAffected = 0;

	HRESULT hr;
	hr = Execute(strSQL, NULL, &nRowsAffected, NULL, TRUE); // будет ошибка? это не важно.
} // COLEDBCommand::RemoveTmpTable

//_____________________________________________________________________________
//
void COLEDBCommand::RemoveAllTmpTables()
{
	int nCountTmps = m_TmpTblsList.GetCount();
	if(nCountTmps > 0)
	{
		for(int i = 0; i < nCountTmps; ++i)
			RemoveTmpTable(m_TmpTblsList.GetAt(m_TmpTblsList.FindIndex(i)));

		m_TmpTblsList.RemoveAll();
	}
}

//_____________________________________________________________________________
//
LPOLESTR COLEDBCommand::wGetTypeByStr(const WORD DBType) const
{
	switch(DBType)
	{
		case DBTYPE_I1:			 return L"DBTYPE_I1";
		case DBTYPE_I2:			 return L"DBTYPE_I2";
		case DBTYPE_I4:			 return L"DBTYPE_I4";
		case DBTYPE_I8:			 return L"DBTYPE_I8";
		case DBTYPE_UI1:		 return L"DBTYPE_UI1";
		case DBTYPE_UI2:		 return L"DBTYPE_UI2";
		case DBTYPE_UI4:		 return L"DBTYPE_UI4";
		case DBTYPE_UI8:		 return L"DBTYPE_UI8";
		case DBTYPE_R4:			 return L"DBTYPE_R4";
		case DBTYPE_R8:			 return L"DBTYPE_R8";
		case DBTYPE_CY:			 return L"DBTYPE_CY";
		case DBTYPE_DECIMAL:	 return L"DBTYPE_DECIMAL";
		case DBTYPE_NUMERIC:	 return L"DBTYPE_NUMERIC";
		case DBTYPE_BOOL:		 return L"DBTYPE_BOOL";
		case DBTYPE_ERROR:		 return L"DBTYPE_ERROR";
		case DBTYPE_UDT:		 return L"DBTYPE_UDT";
		case DBTYPE_VARIANT:	 return L"DBTYPE_VARIANT";
		case DBTYPE_IDISPATCH:	 return L"DBTYPE_IDISPATCH";
		case DBTYPE_IUNKNOWN:	 return L"DBTYPE_IUNKNOWN";
		case DBTYPE_GUID:		 return L"DBTYPE_GUID";
		case DBTYPE_DATE:		 return L"DBTYPE_DATE";
		case DBTYPE_DBDATE:		 return L"DBTYPE_DBDATE";
		case DBTYPE_DBTIME:		 return L"DBTYPE_DBTIME";
		case DBTYPE_DBTIMESTAMP: return L"DBTYPE_DBTIMESTAMP";
		case DBTYPE_BSTR:		 return L"DBTYPE_BSTR";
		case DBTYPE_WSTR:		 return L"DBTYPE_WCHAR";
		case DBTYPE_BYTES:		 return L"DBTYPE_BINARY";
		case DBTYPE_FILETIME:	 return L"DBTYPE_FILETIME";
		case DBTYPE_VARNUMERIC:	 return L"DBTYPE_VARNUMERIC";
		case DBTYPE_PROPVARIANT: return L"DBTYPE_PROPVARIANT";
		default:				 return L"DBTYPE_CHAR";
	}
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::BindWithParams (CString& strErr,
	ICommandWithParameters_& pICmdParams)
{
	HRESULT hr;

	ULONG nDataOffset = 0;
	ULONG nLengthOffset = 0;

	int nParamCnt = m_ParamSet.GetCount();

	for(int nItem = 0; nItem < nParamCnt; nItem++)
	{
		WORD ParamType		 = 0;
		BYTE ParamLen		 = 0;
		DWORD ParamIO		 = 0;
		BYTE ParamPrec		 = 0;
		ULONG ParamOrdinal	 = 0;
		long nSize			 = 0;
		CValue val;

		if(!m_ParamSet.GetParamProps (nItem, NULL, ParamLen, ParamType, ParamIO,
			ParamPrec, ParamOrdinal, nSize))
			return FALSE;

		m_ParamSet.m_ParamBindInfo[nItem].pwszDataSourceType	 = wGetTypeByStr(ParamType);
		m_ParamSet.m_ParamBindInfo[nItem].pwszName				 = NULL;
		m_ParamSet.m_ParamBindInfo[nItem].dwFlags				 = ParamIO;
		m_ParamSet.m_ParamBindInfo[nItem].ulParamSize			 = nSize;
		m_ParamSet.m_ParamBindInfo[nItem].bPrecision			 = ParamPrec;
		m_ParamSet.m_ParamBindInfo[nItem].bScale				 = 0;

		m_ParamSet.m_DBBinding[nItem].wType	 = ParamType;
		m_ParamSet.m_DBBinding[nItem].cbMaxLen	 = nSize;

		nLengthOffset = nDataOffset + nSize;

		m_ParamSet.m_DBBinding[nItem].iOrdinal		 = ParamOrdinal;
		m_ParamSet.m_DBBinding[nItem].obValue		 = nDataOffset;
		m_ParamSet.m_DBBinding[nItem].obLength		 = nLengthOffset;
		m_ParamSet.m_DBBinding[nItem].obStatus		 = 0;//nStatusOffset;
		m_ParamSet.m_DBBinding[nItem].pTypeInfo		 = NULL;
		m_ParamSet.m_DBBinding[nItem].pObject		 = NULL;
		m_ParamSet.m_DBBinding[nItem].pBindExt		 = NULL;
		m_ParamSet.m_DBBinding[nItem].dwPart		 = DBPART_VALUE | DBPART_LENGTH;
		m_ParamSet.m_DBBinding[nItem].dwMemOwner	 = DBMEMOWNER_CLIENTOWNED;
		m_ParamSet.m_DBBinding[nItem].dwFlags		 = 0;
		m_ParamSet.m_DBBinding[nItem].eParamIO		 = ParamIO;
		m_ParamSet.m_DBBinding[nItem].bPrecision	 = ParamPrec;
		m_ParamSet.m_DBBinding[nItem].bScale		 = 0;

		nDataOffset = nLengthOffset + sizeof(long);

		m_ParamSet.m_ParamOrdinals[nItem] = ParamOrdinal;
	}

	hr = m_pICommandText->QueryInterface(IID_ICommandWithParameters,
		(void**)&pICmdParams);
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandWithParameters)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	hr = pICmdParams->SetParameterInfo(nParamCnt,
		&m_ParamSet.m_ParamOrdinals[0], &m_ParamSet.m_ParamBindInfo [0]);
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandWithParameters::SetParameterInfo()";
		
		switch(hr)
		{
			case E_INVALIDARG: strErr += " (INVALIDARG)";
				break;

			case DB_E_BADORDINAL: strErr += " (BADORDINAL)";
				break;

			case DB_E_BADPARAMETERNAME: strErr += " (BADPARAMETERNAME)";
				break;

			case DB_E_BADTYPENAME: strErr += " (BADTYPENAME)";
				break;

			case DB_E_OBJECTOPEN: strErr += " (OBJECTOPEN)";
				break;

			default:
				break;
		}

		GetErrorDescription(strErr);
		return FALSE;
	}
	
	return TRUE;
} // COLEDBCommand::BindWithParams

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Unprepare(CString& strErr)
{
	HRESULT hr;

	m_ParamSet.UnprepareParamsBuf();

	CComPtr<ICommandPrepare> pICommandPrepare;

	hr = m_pICommandText->QueryInterface(IID_ICommandPrepare, reinterpret_cast<void**>(&pICommandPrepare));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandPrepare)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	if(FAILED(pICommandPrepare->Unprepare()))
	{
		strErr = "FAILED! ICommandPrepare::Unprepare(0)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	return TRUE;
} // COLEDBCommand::Unprepare

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Prepare (CString const & strSQL, CString& strErr)
{
	HRESULT hr;

	Utils::StringToUnicodeConverter_t Converter;

	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL,
		Converter.Convert (strSQL));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::SetCommandText()";
		GetErrorDescription(strErr);
		return FALSE;
	}
	
	CComPtr<ICommandPrepare> spICommandPrepare;
	
	hr = m_pICommandText->QueryInterface(IID_ICommandPrepare, (void**)&spICommandPrepare);
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandPrepare)";
		GetErrorDescription(strErr);
		return FALSE;
	}
	
	if(FAILED(spICommandPrepare->Prepare(0)))
	{
		strErr = "FAILED! ICommandPrepare::Prepare(0)";
		GetErrorDescription(strErr);
		return FALSE;
	}
	
	if(m_ParamSet.PrepareParamsBuf(strErr) > 0)
	{
		if(!BindWithParams(strErr, m_ParamSet.m_pICmdWithParams))
			return FALSE;
		
		int nParamCnt = m_ParamSet.GetCount();
		
		if(!CreateAccessor(DBACCESSOR_PARAMETERDATA, 0, m_ParamSet.m_pIAccessor,
			m_ParamSet.m_pICmdWithParams, &(m_ParamSet.m_hPrmAccessor),
			(ULONG &)nParamCnt, m_ParamSet.m_DBBinding))
			return FALSE;
	}
	
	return TRUE;
} // COLEDBCommand::Prepare

OleDb::CResultSequentialAccessProxy
COLEDBCommand::ExecuteStatementExternalSequential (CString &strQuery,
	long &nRowsAffected, DBPARAMS *pParams /*= NULL*/)
{
	HRESULT hr;
	if (!m_bPrepared) {
		hr = COLEDBCommand::ParseQuery (strQuery);
		if(!hr)	return CResultSequentialAccessProxy (NULL);
	}

	nRowsAffected = 0;
	CComPtr<IRowset> pIRowset;
	
	hr = COLEDBCommand::Execute(strQuery, pParams, &nRowsAffected,
		(IUnknown**)&pIRowset, false, m_bPrepared, false);
	if(!hr) return CResultSequentialAccessProxy (NULL);
	
	return CResultSequentialAccessProxy (new CResultSequentialAccess (pIRowset,
		m_nRowsChunk));
}

void COLEDBCommand::SetTextParam (CString const &Name, CValue const &Value)
{
	m_MetaNameParser.SetParameter(Name, Value);
}

void COLEDBCommand::SetDebugMode (bool fDebug)
{
	m_IsDebugMode = fDebug;
}

OleDb::CResultDirectAccessProxy COLEDBCommand::ExecuteStatementExternalDirect (
	CString &strQuery, long &nRowsAffected, DBPARAMS *pParams /*= NULL*/)
{
	HRESULT hr = COLEDBCommand::ParseQuery (strQuery);
	if(!hr)	return CResultDirectAccessProxy (NULL);

	nRowsAffected = 0;
	CComPtr<IRowset> pIRowset;
	
	hr = COLEDBCommand::Execute(strQuery, pParams, &nRowsAffected,
		(IUnknown**)&pIRowset, false, m_bPrepared, false);
	if(!hr) return CResultDirectAccessProxy (NULL);
	
	return CResultDirectAccessProxy (new CResultDirectAccess (
		(IRowsetLocate *)(IRowset *) pIRowset));
}
