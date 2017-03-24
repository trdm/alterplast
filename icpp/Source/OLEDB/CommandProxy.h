#ifndef OLE_DB_COMMAND_PROXY_H
#define OLE_DB_COMMAND_PROXY_H

#pragma once

#include "ResultProxy.h"

class COLEDBCommand;

namespace OleDb {

class DLLEXPORT CCommandProxy
{
	boost::shared_ptr<COLEDBCommand> m_pCommand;
public:
	CCommandProxy (COLEDBCommand *pCommand);
	bool IsValid () const;
	void Reset ();

	OleDb::CResultSequentialAccessProxy ExecuteStatementExternalSequential (
		CString &strQuery, long &nRowsAffected);
	OleDb::CResultDirectAccessProxy ExecuteStatementExternalDirect (
		CString &strQuery, long &nRowsAffected);
	void SetTextParam (CString const &Name, CValue const &Value);
	void SetDebugMode (bool fDebug);
};

} // namespace OleDb

#endif // OLE_DB_COMMAND_PROXY_H
