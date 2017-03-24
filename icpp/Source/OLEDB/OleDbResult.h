#ifndef OLE_DB_RESULT_H
#define OLE_DB_RESULT_H

#pragma once

#include <oledb.h>
#include <oledberr.h>
#include "../Utils/StringAlgo.h"

enum ExtTp1C {eToOther, eToDoc, eToSb};

struct COLEDBFldsInf
{
	UINT m_Flags;

	enum Flags {
		IS_TYPE_1S = 1,
		IS_LONG = 2
	};

	void SetFlagValue (Flags Flag, bool Value)
	{
		if (Value) {
			m_Flags |= Flag;
		}
		else {
			m_Flags &= ((UINT)-1) & Flag;
		}
	}

	bool GetFlagValue (Flags Flag) const
	{
		return (m_Flags & Flag) != 0;
	}
public:
	COLEDBFldsInf();
	
	ULONG	 m_Index;
	CString	 m_strName; 
	CType	 m_nType1C;
	DBTYPE	 m_wType;
	ExtTp1C	 m_eExtType1C;

	void SetIsType1s (bool Value) {SetFlagValue (IS_TYPE_1S, Value);}
	bool GetIsType1s () const {return GetFlagValue (IS_TYPE_1S);}

	void SetIsLong (bool Value) {SetFlagValue (IS_LONG, Value);}
	bool GetIsLong () const {return GetFlagValue (IS_LONG);}
};

namespace OleDb {

UINT const DEFAULT_ROWS_CHUNK = 50;

class CResult
{
protected:
	class RowsHolder_t
	{
		std::vector<HROW> m_Rows;
		CComPtr<IRowset> m_pRowset;
		UINT m_nObtainedRows;
		UINT m_nCurRow;

		RowsHolder_t (RowsHolder_t const &);
		RowsHolder_t const &operator= (RowsHolder_t const &);
	public:
		RowsHolder_t (IRowset *pRowset, ULONG NumRows) :
			m_Rows (NumRows), m_pRowset (pRowset), m_nObtainedRows (0),
			m_nCurRow (0) {}

		HROW *Get ()
		{
			Reset ();
			return &m_Rows [0];
		}

		HROW GetRow ()
		{
			if (!m_nObtainedRows) return NULL;
			if (m_nCurRow == m_nObtainedRows) return NULL;
			return m_Rows [m_nCurRow++];
		}

		void SetObtainedRows (UINT Rows) {m_nObtainedRows = Rows;}

		void Reset ()
		{
			if (m_nObtainedRows) {
				m_pRowset->ReleaseRows(m_nObtainedRows, &m_Rows [0], NULL,
						NULL, NULL);
			}

			m_nObtainedRows = 0;
			m_nCurRow = 0;
		}

		~RowsHolder_t () {Reset ();}
	};

	class ScopedAccessor_t
	{
		HACCESSOR m_hAccessor;
		CComPtr<IAccessor> m_pAccessor;

		ScopedAccessor_t (ScopedAccessor_t const &);
		ScopedAccessor_t const &operator= (ScopedAccessor_t const &);
	public:
		ScopedAccessor_t () : m_hAccessor (0) {}
		ScopedAccessor_t (IAccessor *pAccessor, HACCESSOR hAccessor) :
			m_hAccessor (hAccessor), m_pAccessor (pAccessor) {}

		void Reset (IAccessor *pAccessor, HACCESSOR hAccessor)
		{
			if (m_hAccessor) m_pAccessor->ReleaseAccessor(m_hAccessor, NULL);

			m_pAccessor = pAccessor;
			m_hAccessor = hAccessor;
		}

		HACCESSOR Get () const {return m_hAccessor;}

		~ScopedAccessor_t () {Reset (NULL, NULL);}
	};

	typedef std::vector<DBBINDING> DbBindingsArr_t;
	typedef std::vector<COLEDBFldsInf> ColumnsInfoArr_t;
	typedef std::vector<DBOBJECT> DbObjectsArr_t;

	CComPtr<IRowset> m_pRowset;

	ULONG m_cCols;
	ULONG m_cbMaxRowSize;
	/* флаг - если в выборке есть хоть одно поле вида "ХХХ_вид" или "XXX_kind",
	то выставляем в значение true*/
	bool m_HasKindField;
	DbObjectsArr_t m_DbObjects;
	DbBindingsArr_t m_DBBindings;
	ColumnsInfoArr_t m_ColumnsInfo;

	CComPtr<IAccessor> m_pAccessor;
	ScopedAccessor_t m_Accessor;

	std::vector<BYTE> m_RowBuffer;

	std::vector<BYTE> m_TempBuffer;
	CNumeric m_TempNumeric;
	CString m_TempString;
	Utils::UnicodeToStringConverter_t m_StringConverter;

	void Binding ();
	void CreateAccessor ();
	CType GetType1C (COLEDBFldsInf* fInfo) const;
	ULONG FindKindCol (const CString& strColName) const;

	void ReadLongField (ULONG Field, CValue &Value);
	void ReadNumericField (ULONG Field, CValue &Value);
	void ReadBoolField (ULONG Field, CValue &Value);
	void ReadDateField (ULONG Field, CValue &Value);
	void ReadTimestampField (ULONG Field, CValue &Value);
	void ReadWideStringField (ULONG Field, CValue &Value);
	void Read1SField (ULONG Field, CValue &Value);

	static ULONG AddOffset(ULONG nCurrent, ULONG nAdd);
	static void GetErrorDescription (CString& strError);
public:
	CResult (IRowset *pRowset);
	virtual ~CResult () = 0;

	bool IsEmpty () const;
	bool GetFieldValue (ULONG Field, CValue &Value);

	ULONG GetNumFields () const;
	CString const &GetFieldName (ULONG Field) const;
	CType const &GetFieldType (ULONG Field) const;
};

class CResultSequentialAccess : public CResult
{
	RowsHolder_t m_RowsHolder;
	UINT m_RowsChunk;
public:
	CResultSequentialAccess (IRowset *pRowset, UINT RowsChunk = DEFAULT_ROWS_CHUNK);
	bool GetNextRow ();
};

class CResultDirectAccess : public CResult
{
public:
	CResultDirectAccess (IRowset *pRowset);
	bool GetRow (ULONG Row);
};

} // namespace OleDb

#endif // OLE_DB_RESULT_H
