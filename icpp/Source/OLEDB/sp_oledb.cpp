// Мирошниченко Кирилл aka spock.
// #163860836
// e-mail: spock@km.ru
//
#include "stdafx.h"
#include "sp_oledb.h"
#include "sp_cooledb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CDataBase7* pDataBase7;
extern CMetaDataCont* pMetaDataCont;
extern CBkEndUI* pBkEndUI;

BEGIN_BL_METH_MAP(COLEDBData)
	BL_METH_FUNC("Connect", "Соединение", 1, &funcConnect)
	BL_METH_FUNC_DEF_PARAM("AttachIB", "ПрисоединитьИБ", 2, &funcAttachIB, &defsAttachIB)

	BL_METH_PROC("Close", "Закрыть", 0, &procClose)
	BL_METH_FUNC("CreateCommand", "СоздатьКоманду", 0, &funcCreateCommand)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(COLEDBData)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(COLEDBData, "OLEDBData", "OLEDBData");

//_____________________________________________________________________________
//
COLEDBData::COLEDBData() : CMyContextBase()
{
	m_bConnected = false;
	m_pInit		 = NULL;

	// trad
	//
	m_pMetaDataCont = pMetaDataCont;
	//
}

//_____________________________________________________________________________
//
COLEDBData::~COLEDBData()
{
	m_bConnected = false;
	
	// trad
	//
	CMetaDataWork::UnloadMD(m_pMetaDataCont);
	//
}

//_____________________________________________________________________________
//
BOOL COLEDBData::defsAttachIB(int nParam, CValue* param) const
{
	if (1 == nParam){
		*param = "";
		return TRUE;
	}
	return FALSE;
}

//_____________________________________________________________________________
//
BOOL COLEDBData::funcConnect(CValue& rValue, CValue** ppValue)
{
	CString strErr;

	CComPtr<IDataInitialize> pIDataInit;

	HRESULT hr = CoCreateInstance(CLSID_MSDAINITIALIZE, NULL, CLSCTX_INPROC_SERVER, IID_IDataInitialize, (void**)&pIDataInit);
	if(FAILED(hr))
	{
		strErr = "FAILED! CoCreateInstance()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return FALSE;
	}

	CString strConn(ppValue[0]->GetString());

	hr = pIDataInit->GetDataSource(NULL, CLSCTX_INPROC_SERVER, CComBSTR(strConn), IID_IDBInitialize, (IUnknown**)&m_pInit);
	if(FAILED(hr))
	{
		strErr = "FAILED! IDataInitialize::GetDataSource()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return FALSE;
	}

	hr = m_pInit->Initialize();
	if(FAILED(hr))
	{
		strErr = "FAILED! IDBInitialize::Initialize()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return FALSE;
	}

	CComPtr<IDBCreateSession> pIDBCreateSession;

	hr = m_pInit->QueryInterface(IID_IDBCreateSession, (void**)&pIDBCreateSession);
	if(FAILED(hr))
	{
		strErr = "FAILED! IDBCreateSession::QueryInterface()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return FALSE;
	}
	
	hr = pIDBCreateSession->CreateSession(NULL, IID_IDBCreateCommand, (IUnknown**)&m_pIDBCreateCommand);
	if(FAILED(hr))
	{
		strErr = "FAILED! IDBCreateSession::CreateSession()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return FALSE;
	}
	
	m_bConnected = true;
	rValue		 = 1L;
	
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBData::funcAttachIB(CValue& rValue, CValue** ppValue)
{
	if(STRING_TYPE_1C != ppValue[0]->GetTypeCode() || 
		STRING_TYPE_1C != ppValue[1]->GetTypeCode())
	{
		RuntimeErrorRes(4712); // Неверный тип параметра
	}
	
	CString strFileName(ppValue[0]->GetString());
	strFileName.Format("%s%s", strFileName.operator LPCTSTR(), "1cv7.md");

	if(!CMetaDataWork::LoadMD(strFileName, m_pMetaDataCont))
		RuntimeErrorRes(28771); // Ошибка загрузки метаданных

	if(ppValue[1]->GetString().IsEmpty())
		*ppValue[1] = "Provider=VFPOLEDB.1;Mode=ReadWrite;Collating Sequence=MACHINE;Data Source=" + ppValue[0]->GetString();

	return funcConnect(rValue, &ppValue[1]);
}

//_____________________________________________________________________________
//
BOOL COLEDBData::procClose(CValue** ppValue)
{
	m_bConnected = false;
	m_pIDBCreateCommand.Release();
	m_pInit.Release();
	
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBData::funcCreateCommand(CValue& rValue, CValue** ppValue)
{
	COLEDBCommand* pCoOLEDB = CreateCommand();
	rValue.AssignContext(pCoOLEDB);
	pCoOLEDB->DecrRef();
	return TRUE;
}

//_____________________________________________________________________________
//
void COLEDBData::GetErrorDescription(CString& strError) const
{
	HRESULT hr;
	
	IErrorInfo* pIErrorInfo;

	try
	{
		hr = ::GetErrorInfo(0, &pIErrorInfo);
		if(SUCCEEDED(hr))
		{
			BSTR errDescr;
			pIErrorInfo->GetDescription(&errDescr);

			strError += ": ";
			strError += errDescr;

			::SysFreeString(errDescr);
		}
		pIErrorInfo->Release();
	}
	catch(...)
	{
		strError += ": Undefined critical error!";
	}
}

// trad
//_____________________________________________________________________________
//
CDate CIBSetOLEDB::GetDateTimeTA(CEventTime& TimeTA)
{
	return CIBSet::GetDateTimeTA(TimeTA);
}

//_____________________________________________________________________________
//
int CIBSetOLEDB::GetDataSourceType()
{
	return DATA_SOURCE_TYPE_DBF;
}

//_____________________________________________________________________________
//
enum PeriodType CIBSetOLEDB::GetRestsSnapShotPeriod()
{
	return CIBSet::GetRestsSnapShotPeriod();
}
//

DLLEXPORT COLEDBCommand * COLEDBData::CreateCommand()
{
	CString strErr;
	
	if(!m_bConnected)
	{
		strErr = "Попытка создать команду без подключения к источнику данных!";
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return NULL;
	}
	
	COLEDBCommand* pCoOLEDB = NULL;
	try
	{
		pCoOLEDB = new COLEDBCommand(m_pIDBCreateCommand);
		
		// trad
		//
		pCoOLEDB->m_pOLEDBData = this;
		pCoOLEDB->SetIBProp(m_pMetaDataCont, &m_OLEDBIBSet);
		//
		
	}
	catch(...)
	{
		if (pCoOLEDB) 
			pCoOLEDB->DecrRef();
		
		strErr = "FAILED! COLEDBCommand::COLEDBCommand(): Не удалось создать команду!";
		CBLModule::RaiseExtRuntimeError(strErr, 0);
		return NULL;
	}

	return pCoOLEDB;
}
