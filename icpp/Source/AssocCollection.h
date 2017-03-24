#ifndef ASSOC_COLLECTION_H_INCLUDED
#define ASSOC_COLLECTION_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CAssocCollection class (c) kms 2008
//////////////////////////////////////////////////////////////////////////

/*
The COLLTYPE class provides interface:
class COLLTYPE
{
public:
// scf_Readonly
	int GetIndexOf(KEYREF key, BOOL bQuiet = FALSE) const;
	ASSOC* GetAssocAt(size_t nIndex, BOOL bQuiet = FALSE) const;
	ASSOC* GetAssocAt(KEYREF key, BOOL bQuiet = FALSE) const;
	size_t GetSize() const; // number of items
	size_t GetAssocSize() const; // number of items with associations
	VALREF GetAt(size_t nIndex) const;
	VALREF GetAt(KEYREF key) const;

// scf_Modify
	void SetAt(size_t nIndex, VALREF val);
	void SetAt(KEYREF key, VALREF val);
	void Add(VALREF val, KEYREF key);
	void Remove(size_t nIndex);
	void Remove(KEYREF key);
	void Clear();

// scf_Insert
	void Insert(size_t nIndex, VALREF val, KEYREF key);

// scf_Move
	void Move(size_t nIndex, int nOffset);
}

COLLTYPE provides definitions:
	KEY
	KEYREF
	VAL
	VALREF
	ASSOC
 	ITEM
 	ITEMREF

ASSOC class provides interface:
	KEYREF GetKey() const;
	VALREF GetVal() const;
	void SetVal(VALREF val);

Any interface function can throw sc_exception()
*/

#include "SimpleContext.h"
#include "SimpleCMN.h"
#include <boost/scoped_array.hpp>

template <typename COLLTYPE, DWORD dwFLAGS = scf_Readonly>
class CAssocCollection : public CSimpleContext, public COLLTYPE, public IResultLoader, public IResultPublisher
{
public:
	CAssocCollection(LPCSTR szTypeString = NULL)
		: CSimpleContext(&m_StaticDef, m_bStaticDefValid, IsNULL(szTypeString, TypeString()))
	{
		if (!m_bStaticDefValid)
		{
			InitDef();
			m_bStaticDefValid = TRUE;
		}
	}

	CAssocCollection(CSimpleDef* pDef, BOOL bDefValid, LPCSTR szTypeString = NULL)
		: CSimpleContext(pDef, bDefValid, szTypeString ? szTypeString : TypeString())
	{
		if (!bDefValid)
			InitDef();
	}

	virtual ~CAssocCollection() {}

private:
	void InitDef()
	{
		AddDef("Count",		"Количество",		0,		&CAssocCollection::funcCount);
		AddDef("UBound",	"ВГраница",			0,		&CAssocCollection::funcUBound);
		AddDef("Get",		"Получить",			1,		&CAssocCollection::funcGet,		&CAssocCollection::defsFALSE);
		AddDef("FindKey",	"НайтиКлюч",		2,		&CAssocCollection::funcFindKey,	&CAssocCollection::defsFindKey);
		AddDef("GetKey",	"ПолучитьКлюч",		1,		&CAssocCollection::funcGetKey,	&CAssocCollection::defsFALSE);
		AddDef("Unload",	"Выгрузить",		1,		&CAssocCollection::procUnload,	&CAssocCollection::defsFALSE);
		if (dwFLAGS & scf_Modify)
		{
			AddDef("Set",		"Установить",		2,		&CAssocCollection::procSet,		&CAssocCollection::defsFALSE);
			AddDef("Add",		"Добавить",			2,		&CAssocCollection::procAdd,		&CAssocCollection::defsAdd);
			AddDef("Remove",	"Удалить",			1,		&CAssocCollection::procRemove,	&CAssocCollection::defsFALSE);
			AddDef("Clear",		"Очистить",			0,		&CAssocCollection::procClear);
			AddDef("Load",		"Загрузить",		1,		&CAssocCollection::procLoad,	&CAssocCollection::defsFALSE);
			if (dwFLAGS & scf_Insert)
				AddDef("Insert",	"Вставить",			3,		&CAssocCollection::procInsert,	&CAssocCollection::defsInsert);

			if (dwFLAGS & scf_Move)
				AddDef("Move",		"Сдвинуть",			2,		&CAssocCollection::procMove,	&CAssocCollection::defsFALSE);
		}
	}

	LPCSTR TypeString()
	{
		return scf_Readonly ? "FixedAssocCollection" : "AssocCollection";
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// CAssocCollection bridge interface
	//////////////////////////////////////////////////////////////////////////

	BOOL defsFALSE(int nIndex, CValue* pValue) const
	{
		return FALSE;
	}

	BOOL funcCount(CValue& RetVal, CValue** ppVal)
	{
		RetVal = sc_GetSize();
		return TRUE;
	}

	BOOL funcUBound(CValue& RetVal, CValue** ppVal)
	{
		RetVal = sc_GetSize() - 1;
		return TRUE;
	}

	BOOL funcGet(CValue& RetVal, CValue** ppVal)
	{
		sc_GetAt(RetVal, *ppVal[0]);
		return TRUE;
	}

	BOOL defsFindKey(int nIndex, CValue* pValue) const
	{
		if (nIndex == 1)
		{
			*pValue = 0L;
			return TRUE;
		}
		return FALSE;
	}

	BOOL funcFindKey(CValue& RetVal, CValue** ppVal)
	{
		sc_FindKey(RetVal, *ppVal[0], *ppVal[1]);
		return TRUE;
	}

	BOOL funcGetKey(CValue& RetVal, CValue** ppVal)
	{
		sc_GetKey(RetVal, *ppVal[0]);
		return TRUE;
	}

	BOOL procUnload(CValue** ppVal)
	{
		sc_Unload(*ppVal[0]);
		return TRUE;
	}

	BOOL procSet(CValue** ppVal)
	{
		sc_SetAt(*ppVal[0], *ppVal[1]);
		return TRUE;
	}

	BOOL defsAdd(int nIndex, CValue* pValue) const
	{
		switch(nIndex)
		{
		case 1:
			*pValue = "";
			return TRUE;
		}
		return FALSE;
	}

	BOOL procAdd(CValue** ppVal)
	{
		sc_Add(*ppVal[0], *ppVal[1]);
		return TRUE;
	}

	BOOL procRemove(CValue** ppVal)
	{
		sc_Remove(*ppVal[0]);
		return TRUE;
	}

	BOOL procClear(CValue** ppVal)
	{
		sc_Clear();
		return TRUE;
	}

	BOOL defsInsert(int nIndex, CValue* pValue) const
	{
		switch(nIndex)
		{
		case 2:
			*pValue = "";
			return TRUE;
		}
		return FALSE;
	}

	BOOL procLoad(CValue** ppVal)
	{
		sc_Load(*ppVal[0]);
		return TRUE;
	}

	BOOL procInsert(CValue** ppVal)
	{
		sc_Insert(*ppVal[0], *ppVal[1], *ppVal[2]);
		return TRUE;
	}

	BOOL procMove(CValue** ppVal)
	{
		sc_Move(*ppVal[0], *ppVal[1]);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	CAssocCollection "virtual" interface
	//////////////////////////////////////////////////////////////////////////

	int sc_GetIndex(CValue const& val) const
	{
		if (val.GetTypeCode() == NUMBER_TYPE_1C)
		{
			int nIndex = val.GetNumeric();
			return nIndex == -1 ? GetSize() - 1 : nIndex;
		}
		else
			return -1;
	}

	size_t sc_GetSize() const
	{
		return GetSize();
	}

	BOOL f_GetAt(CValue& vRetVal, int nIndex) const
	{
		if (nIndex == -1)
			return FALSE;
		CSC_CMN::AssignItem(vRetVal, GetAt(nIndex));
		return TRUE;
	}

	void sc_GetAt(CValue& RetVal, CValue const& vKey) const
	{
		if (!f_GetAt(RetVal, sc_GetIndex(vKey)))
			CSC_CMN::AssignItem(RetVal, GetAt(CSC_CMN::GetItem<KEY>(vKey)));
	}

	void sc_FindKey(CValue& RetVal, CValue const& vKey, CValue const& vGetIndex) const
	{
		BOOL bGetIndex = vGetIndex.GetNumeric();
		if (!bGetIndex)
			RetVal = GetAssocAt(CSC_CMN::GetItem<KEY>(vKey), TRUE) ? 1 : -1;
		else
			RetVal = GetIndexOf(CSC_CMN::GetItem<KEY>(vKey), TRUE);
	}

	void sc_GetKey(CValue& RetVal, CValue const& vIndex)
	{
		CSC_CMN::AssignItem(RetVal, GetAssocAt(sc_GetIndex(vIndex))->GetKey());
	}

	void sc_Unload(CValue& vContext)
	{
		boost::shared_ptr<IResultLoader> pLoader(CValue2ResultLoader(vContext));
		IResultPublisher* pPublisher = this;
		CSC_CMN::DataExchange(pPublisher, pLoader.get());
	}

	template<bool B_SEL>
	void t_SetAt(CValue const& vKey, CValue const& vVal)
	{
	}

	template<>
	void t_SetAt<true>(CValue const& vKey, CValue const& vVal)
	{
		if (!f_SetAt(sc_GetIndex(vKey), vVal))
			SetAt(CSC_CMN::GetItem<KEY>(vKey), CSC_CMN::GetItem<VAL>(vVal));
	}

	BOOL f_SetAt(int nIndex, CValue const& vVal)
	{
		if (nIndex == -1)
			return FALSE;
		SetAt(nIndex, CSC_CMN::GetItem<VAL>(vVal));
		return TRUE;
	}

	void sc_SetAt(CValue const& vKey, CValue const& vVal)
	{
		t_SetAt<dwFLAGS & scf_Modify>(vKey, vVal);
	}

	template<bool B_SEL>
	void t_Add(CValue const& vVal, CValue const& vKey)
	{
	}

	template <>
	void t_Add<true>(CValue const& vVal, CValue const& vKey)
	{
 		Add(CSC_CMN::GetItem<VAL>(vVal), CSC_CMN::GetItem<KEY>(vKey));
	}

	void sc_Add(CValue const& vVal, CValue const& vKey)
	{
		t_Add<(bool)(dwFLAGS & scf_Modify)>(vVal, vKey);
	}

	template<bool B_SEL>
	void t_Remove(CValue const& vIndex)
	{
	}

	template<>
	void t_Remove<true>(CValue const& vKey)
	{
		int nIndex = sc_GetIndex(vKey);
		if (nIndex != -1)
			Remove(nIndex);
		else
			Remove(CSC_CMN::GetItem<KEY>(vKey));
	}

	void sc_Remove(CValue const& vKey)
	{
		t_Remove<(bool)(dwFLAGS & scf_Modify)>(vKey);
	}

	template<bool B_SEL>
	void t_Clear()
	{
	}

	template<>
	void t_Clear<true>()
	{
		Clear();
	}

	void sc_Clear()
	{
		t_Clear<(bool)(dwFLAGS & scf_Modify)>();
	}

	template<bool B_SEL>
	void t_Insert(CValue const& vIndex, CValue const& vVal, CValue const& vKey)
	{
	}

	template<>
	void t_Insert<true>(CValue const& vIndex, CValue const& vVal, CValue const& vKey)
	{
		Insert(sc_GetIndex(vIndex), CSC_CMN::GetItem<VAL>(vVal), CSC_CMN::GetItem<KEY>(vKey));
	}

	void sc_Insert(CValue const& vIndex, CValue const& vVal, CValue const& vKey)
	{
		t_Insert<(bool)(dwFLAGS & scf_Insert)>(vIndex, vVal, vKey);
	}

	void sc_Load(CValue& vContext)
	{
		IResultLoader* pLoader = this;
		IResultPublisher* pPublisher = safe_dynamic_cast<IResultPublisher*>(vContext.GetContext());
		CSC_CMN::DataExchange(pPublisher, pLoader);
	}

	template<bool B_SEL>
	void t_Move(CValue const& vIndex, CValue const& vShift)
	{
	}

	template<>
	void t_Move<true>(CValue const& vIndex, CValue const& vShift)
	{
		Move(sc_GetIndex(vIndex), vShift.GetNumeric());
	}

	void sc_Move(CValue const& vIndex, CValue const& vShift)
	{
		t_Move<(bool)(dwFLAGS & scf_Move)>(vIndex, vShift);
	}

	//////////////////////////////////////////////////////////////////////////
	//	CBLContext interface
	//////////////////////////////////////////////////////////////////////////

// 	virtual char const* GetTypeString(void) const
// 	{
// 		return dwFLAGS == scf_Readonly ? "FixedSimpleCollection" : "SimpleCollection";
// 	}

	//////////////////////////////////////////////////////////////////////////
	//	IErrorDescription interface
	//////////////////////////////////////////////////////////////////////////

	virtual LPCSTR GetLastError() throw()
	{
		return m_csLastError;
	}

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
	{
		m_nColumns = min(nColCount, 2);
		nColCount = m_nColumns;
		return NO_ERROR;
	}

	virtual HRESULT ldrDone() throw()
	{
		return NO_ERROR;
	}

	virtual HRESULT ldrAddColumn(LPCSTR szColName, CType const* pType) throw()
	{
		return NO_ERROR;
	}

	virtual HRESULT ldrAddRow(CValue const* const* ppValues) throw()
	{
		static CValue vEmpty;
		CSC_CMN::AssignEmpty<KEY>(vEmpty);

		CValue const& vVal = *ppValues[0];
		CValue const* pKey = m_nColumns > 1 ? ppValues[1] : &vEmpty;
		HRESULT nLastError = NO_ERROR;
		try
		{
			sc_Add(vVal, *pKey);
		}
		catch (sc_exception const& e)
		{
			CString csVal = vVal.Format();
			CString csKey = pKey->Format();
			m_csLastError.Format("%s (value: '%s', key: '%s')", e.what(), (LPCSTR)csVal, (LPCSTR)csKey);
			nLastError = E_INVALIDARG;
		}
		return nLastError;
	}

	//////////////////////////////////////////////////////////////////////////
	//	IResultPublisher interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw()
	{
		if (!m_Values)
			m_Values.reset(new CValue[2]);
		if (!m_pValues)
		{
			m_pValues.reset(new CValue*[2]);
			m_pValues[0] = m_Values.get();
			m_pValues[1] = m_Values.get() + 1;
		}

		m_nColumns = nColCount ? min(nColCount, 2) : 2;
		m_nRowIndex = 0;

		nColCount = m_nColumns;
		nRowCount = 0;
		return NO_ERROR;
	}

	virtual HRESULT pubDone() throw()
	{
		return NO_ERROR;
	}

	virtual HRESULT pubGetColumn(DWORD nIndex, LPCSTR& szColName, CType*& pType) throw()
	{
		szColName = NULL;
		pType = NULL;
		return NO_ERROR;
	}

	virtual HRESULT pubGetRow(CValue const* const*& ppValues) throw()
	{
		if (m_nRowIndex == sc_GetSize())
			return S_FALSE;
		ASSOC* pAssoc = GetAssocAt(m_nRowIndex++);
		CSC_CMN::AssignItem(m_Values[0], pAssoc->GetVal());
		if (m_nColumns > 1)
			CSC_CMN::AssignItem(m_Values[1], pAssoc->GetKey());
		ppValues = m_pValues.get();
		return NO_ERROR;
	}

private:
	static CSimpleDef m_StaticDef;
	static BOOL m_bStaticDefValid;

	// loader & publisher
	CString m_csLastError;
	size_t m_nColumns;
	size_t m_nRowIndex;
	boost::scoped_array<CValue> m_Values;
	boost::scoped_array<CValue*> m_pValues;
};

template <typename COLLTYPE, DWORD dwFLAGS>
CSimpleDef CAssocCollection<COLLTYPE, dwFLAGS>::m_StaticDef;

template <typename COLLTYPE, DWORD dwFLAGS>
BOOL CAssocCollection<COLLTYPE, dwFLAGS>::m_bStaticDefValid = FALSE;

#endif
