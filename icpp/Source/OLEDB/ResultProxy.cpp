#include "StdAfx.h"
#include "ResultProxy.h"
#include "OleDbResult.h"

namespace OleDb {
	
CResultProxy::CResultProxy (CResult *pResult) :
m_pResult (pResult)
{
}

bool CResultProxy::GetFieldValue (ULONG Field, CValue &Value) const
{
	return m_pResult->GetFieldValue (Field, Value);
}

ULONG CResultProxy::GetNumFields () const
{
	return m_pResult->GetNumFields ();
}

CString const &CResultProxy::GetFieldName (ULONG Field) const
{
	return m_pResult->GetFieldName (Field);
}

CType const &CResultProxy::GetFieldType (ULONG Field) const
{
	return m_pResult->GetFieldType (Field);
}

bool CResultProxy::IsEmpty () const
{
	return m_pResult->IsEmpty ();
}

void CResultProxy::Reset()
{
	m_pResult.reset ();
}

bool CResultProxy::IsValid() const
{
	return !!m_pResult;
}

CResultSequentialAccessProxy::CResultSequentialAccessProxy (
	CResultSequentialAccess *pResult) : CResultProxy (pResult)
{
}

bool CResultSequentialAccessProxy::GetNextRow () const
{
	CResultSequentialAccess *pResult = static_cast<CResultSequentialAccess *> (
		m_pResult.get ());
	return pResult->GetNextRow ();
}


CResultDirectAccessProxy::CResultDirectAccessProxy (CResultDirectAccess *pResult) :
CResultProxy (pResult)
{
}

bool CResultDirectAccessProxy::GetRow (ULONG Row)
{
	CResultDirectAccess *pResult = static_cast<CResultDirectAccess *> (
		m_pResult.get ());
	return pResult->GetRow (Row);
}

} // namespace OleDb
