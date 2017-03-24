// ћирошниченко  ирилл aka spock
// #163860836
// e-mail: spock@km.ru
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <oledb.h>
#include <oledberr.h>

#include "../ODBC/MetaNameParser.h"
#include "../SQL/MetaDataWork.h"
#include "../GUID.h"

#include "DBPropSet.h"
#include "sp_oledb.h"
#include "sp_oledbparam.h"

#include "sp_ConvHlpr.h"
#include "ValueTo.h"
#include "sp_oledbparamhlpr.h"
#include "ResultProxy.h"

namespace OleDb {
	std::size_t const NUMROWS_CHUNK = 50;
	std::size_t const BLOCK_SIZE = 1024;
} // namespace OleDb

//_____________________________________________________________________________
//
typedef CComPtr<ICommandWithParameters> ICommandWithParameters_;
typedef CComPtr<IAccessor> IAccessor_;
typedef CComPtr<IRowset> IRowset_;

//_____________________________________________________________________________
//
class COLEDBCommand : public CMyContextBase
{
	friend class COLEDBData;
	
	DECLARE_DYNCREATE(COLEDBCommand);
	COLEDBCommand() {}; // запрещено использовать !!

public:
	// конструктор по-умолчанию не используем!
	// только с параметром, в конструкторе член (m_bCreated) инициализируетс€
	//
	COLEDBCommand(CComPtr<IDBCreateCommand> pIDBCreateCommand);
	virtual ~COLEDBCommand();

	OleDb::CResultSequentialAccessProxy ExecuteStatementExternalSequential (
		CString &strQuery, long &nRowsAffected, DBPARAMS *pParams = NULL);
	OleDb::CResultDirectAccessProxy ExecuteStatementExternalDirect (
		CString &strQuery, long &nRowsAffected, DBPARAMS *pParams = NULL);

	BOOL funcExecuteStatement(CValue& RetVal, CValue** ppValue);
	BOOL defsExecuteStatement(int nParam, CValue* param) const;

	BOOL funcExecute(CValue& RetVal, CValue** ppValue);
	BOOL defsExecute(int nParam, CValue* param) const;

	BOOL procPutObjectList(CValue** ppValue);
	BOOL defsPutObjectList(int nParam, CValue* param) const;

	BOOL procClose(CValue** ppValue);

	void SetDebugMode (bool fDebug);
	BOOL procDebug(CValue** ppValue);

	void SetTextParam (CString const &Name, CValue const &Value);
	BOOL procSetTextParam(CValue** ppValue);

	BOOL procSetTempTablesDir(CValue** ppValue);
	BOOL procSetExecTimeOut(CValue** ppValue);
	BOOL procAddParam(CValue** ppValue);
	BOOL procSetParam(CValue** ppValue);
	BOOL procDeleteParam(CValue** ppValue);
	BOOL procDeleteParams(CValue** ppValue);
	BOOL procPrepare(CValue** ppValue);
	BOOL procUnprepare(CValue** ppValue);
	BOOL funcParamCnt(CValue& RetVal, CValue** ppValue);

private:
	void SetIBProp(CMetaDataCont *pMDC, CIBSet *pIBSet) {m_MetaNameParser.SetIBProp(pMDC, pIBSet);};

	void GetErrorDescription(CString& strError) const;
	BOOL ParseQuery(CString& strQuery);
	bool Execute(const CString& strSQL, DBPARAMS* pParams, long* pnRowsAffected,
		IUnknown** pIRowset, bool bQuiet = false, bool bPrepared = false,
		bool DirectAccess = false) const;
	bool COLEDBCommand::CreateAccessor(DBACCESSORFLAGS eDBACCFLAGS,
		ULONG cbRowSize, IAccessor_& pIAccessor, IUnknown* pIUnknown,
		HACCESSOR* phAccessor, ULONG& cCntItems,
		std::vector<DBBINDING>& DBBindings) const;
	BOOL PutObjectsList(CValue** ppValue);
	BOOL PutAggregateObjects(CValue* pValue, const CString* pszTableName, const CString* RefName);
	void RemoveTmpTable(CString& strTmpTblName);
	void RemoveAllTmpTables(void);

	LPOLESTR wGetTypeByStr(const WORD) const;

	BOOL Prepare(const CString& strSQL, CString& strErr);
	BOOL Unprepare(CString& strErr);
	BOOL BindWithParams(CString& strErr, ICommandWithParameters_& pICmdParams);

	CBLPtr<COLEDBData> m_pOLEDBData;
	CMetaNameParser m_MetaNameParser;
	CComPtr<ICommandText> m_pICommandText;
	CList<CString, CString&> m_TmpTblsList;
	COLEDBParamHlpr m_ParamSet;

	BOOL	 m_bCreated;
	BOOL	 m_IsDebugMode;
	BOOL	 m_bPrepared;
	long	 m_ExecTimeout;
	GUID	 m_GUID;
	CString	 m_TempTablesDir;
	UINT	 m_nRowsChunk;

	DECLARE_MY_CONTEXT()
};