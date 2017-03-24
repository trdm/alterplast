#include "StdAfx.h"
#include "CommandProxy.h"
#include "sp_cooledb.h"

namespace OleDb {
	
CCommandProxy::CCommandProxy (COLEDBCommand *pCommand) : m_pCommand (pCommand)
{
}

OleDb::CResultSequentialAccessProxy DLLEXPORT
CCommandProxy::ExecuteStatementExternalSequential (CString &strQuery,
	long &nRowsAffected)
{
	return m_pCommand->ExecuteStatementExternalSequential (strQuery, nRowsAffected);
}

OleDb::CResultDirectAccessProxy DLLEXPORT
CCommandProxy::ExecuteStatementExternalDirect (CString &strQuery,
	long &nRowsAffected)
{
	return m_pCommand->ExecuteStatementExternalDirect (strQuery, nRowsAffected);
}

bool CCommandProxy::IsValid () const
{
	return !!m_pCommand;
}

void CCommandProxy::Reset ()
{
	m_pCommand.reset ();
}

void CCommandProxy::SetTextParam (CString const &Name, CValue const &Value)
{
	m_pCommand->SetTextParam (Name, Value);
}

void CCommandProxy::SetDebugMode (bool fDebug)
{
	m_pCommand->SetDebugMode (fDebug);
}

} // namespace OleDb
