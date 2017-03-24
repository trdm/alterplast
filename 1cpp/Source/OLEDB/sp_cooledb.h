// ћирошниченко  ирилл aka spock
// #163860836
// e-mail: spock@km.ru
//
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

#ifndef _MSADERR_H_
#include <oledberr.h>
#endif // _MSADERR_H_

#include "..\\ODBC\\MetaNameParser.h"
#include "..\\SQL\\MetaDataWork.h"
#include "..\\GUID.h"

#include "DBPropSet.h"
#include "sp_oledb.h"
#include "sp_oledbparam.h"

#include "sp_oledbptr.h"

#include "sp_ConvHlpr.h"
#include "ValueTo.h"
#include "sp_oledbparamhlpr.h"

#define NUMROWS_CHUNK 50
#define BLOCK_SIZE 1024

enum ExtTp1C {eToOther, eToDoc, eToSb};

//_____________________________________________________________________________
//
struct COLEDBFldsInf
{
public:
	COLEDBFldsInf();

	ULONG	 m_Index;
	CString	 m_strName; 
	CType	 m_nType1C;
	DBTYPE	 m_wType;
	bool	 m_bIsType1C;
	ExtTp1C	 m_eExtType1C;
	bool	 m_bIsLong;
};

typedef CComPtr<ICommandWithParameters> ICommandWithParameters_;
typedef CComPtr<IAccessor> IAccessor_;
typedef CComPtr<IRowset> IRowset_;

typedef COLEDBMemPtr<COLEDBFldsInf> COLEDBFldsInf_;
typedef COLEDBMemPtr<DBBINDSTATUS> DBBINDSTATUS_;
typedef COLEDBMemPtr<DBBINDING> DBBINDING_;
//typedef COLEDBMemPtr<DBOBJECT> DBOBJECT_;
typedef COLEDBMemPtr<BYTE> BYTE_;

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

	BOOL funcExecuteStatement(CValue& RetVal, CValue** ppValue);
	BOOL defsExecuteStatement(int nParam, CValue* param) const;

	BOOL funcExecute(CValue& RetVal, CValue** ppValue);
	BOOL defsExecute(int nParam, CValue* param) const;

	BOOL procPutObjectList(CValue** ppValue);
	BOOL defsPutObjectList(int nParam, CValue* param) const;

	BOOL procClose(CValue** ppValue);
	BOOL procDebug(CValue** ppValue);
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
	BOOL Binding(IRowset_& pIRowset, ULONG* pnCols, DBBINDING_& pDBBindings, COLEDBFldsInf_& pColInfo1C, ULONG* pcMaxRowSize, BOOL& HasKindField) const;
	CType GetType1C(COLEDBFldsInf* fInfo) const;
	BOOL ParseQuery(CString& strQuery);
	BOOL Execute(const CString& strSQL, DBPARAMS* pParams, long* pnRowsAffected, IUnknown** pIRowset, BOOL bQuiet = false, BOOL bPrepared = false) const;
	BOOL CreateAccessor(DBACCESSORFLAGS eDBACCFLAGS, ULONG cbRowSize, IAccessor_& pIAccessor, IUnknown* pIUnknown, HACCESSOR* hAccessor, ULONG& cCntItems, DBBINDING_& pDBBindings) const;
	BOOL PutObjectsList(CValue** ppValue);
	BOOL PutAggregateObjects(CValue* pValue, const CString* pszTableName, const CString* RefName);
	void RemoveTmpTable(CString& strTmpTblName);
	void RemoveAllTmpTables(void);
	ULONG FindKindCol(const COLEDBFldsInf* pColumnsInfo, const ULONG& cMaxCols, const CString& strColName) const;

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
	CString	 m_szTempTablesDir;

	DECLARE_MY_CONTEXT()
};