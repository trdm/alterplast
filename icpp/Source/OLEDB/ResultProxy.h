#ifndef OLE_DB_RESULT_PROXY_H
#define OLE_DB_RESULT_PROXY_H

#pragma once

namespace OleDb {

class CResult;

class DLLEXPORT CResultProxy
{
protected:
	boost::shared_ptr<CResult> m_pResult;
public:
	CResultProxy (CResult *pResult);
	bool IsValid () const;

	bool IsEmpty () const;
	void Reset ();

	bool GetFieldValue (ULONG Field, CValue &Value) const;
	
	ULONG GetNumFields () const;
	CString const &GetFieldName (ULONG Field) const;
	CType const &GetFieldType (ULONG Field) const;
};

class CResultSequentialAccess;

class DLLEXPORT CResultSequentialAccessProxy : public CResultProxy
{
public:
	CResultSequentialAccessProxy (CResultSequentialAccess *pResult);
	bool GetNextRow () const;
};

class CResultDirectAccess;

class DLLEXPORT CResultDirectAccessProxy : public CResultProxy
{
public:
	CResultDirectAccessProxy (CResultDirectAccess *pResult);
	bool GetRow (ULONG Row);
};

} // namespace OleDb

#endif // OLE_DB_RESULT_PROXY_H
