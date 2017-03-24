#ifndef SIMPLE_COLLECTION_H_INCLUDED
#define SIMPLE_COLLECTION_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CSimpleCollection class (c) kms 2008
//////////////////////////////////////////////////////////////////////////

/*
The COLLTYPE class must have this interface:
class COLLTYPE
{
public:
// scf_Readonly
	size_t GetSize() const;
	ITEMREF GetAt(int nIndex) const;

// scf_Modify
	void SetAt(size_t nIndex, ITEMREF val);
	void Add(ITEMREF val);
	void Remove(size_t nIndex);
	void Clear();

// scf_Insert
	void Insert(size_t nIndex, ITEMREF val);

// scf_Move
	void Move(size_t nIndex, int nOffset);
}

COLLTYPE must provide definitions:
	ITEM
	ITEMREF

Any interface function can throw sc_exception()
*/

#include "SimpleContext.h"
#include "SimpleCMN.h"

#include "../Interfaces/VLResultLoader.h"

template <typename COLLTYPE, DWORD dwFLAGS = scf_Readonly>
class CSimpleCollection : public CSimpleContext, public COLLTYPE, public IResultLoader, public IResultPublisher
{
public:
	CSimpleCollection(LPCSTR szTypeString = NULL)
		: CSimpleContext(&m_StaticDef, m_bStaticDefValid, IsNULL(szTypeString, TypeString()))
	{
		if (!m_bStaticDefValid)
		{
			InitDef();
			m_bStaticDefValid = TRUE;
		}
	}

	CSimpleCollection(CSimpleDef* pDef, BOOL bDefValid, LPCSTR szTypeString = NULL)
		: CSimpleContext(pDef, bDefValid, szTypeString ? szTypeString : TypeString())
	{
		if (!bDefValid)
			InitDef();
	}

	virtual ~CSimpleCollection() {}

private:
	void InitDef()
	{
		AddDef("Count",		"Количество",		0,		&CSimpleCollection::funcCount);
		AddDef("UBound",	"ВГраница",			0,		&CSimpleCollection::funcUBound);
		AddDef("Get",		"Получить",			1,		&CSimpleCollection::funcGet,	&CSimpleCollection::defsFALSE);
		AddDef("Unload",	"Выгрузить",		1,		&CSimpleCollection::procUnload,	&CSimpleCollection::defsFALSE);
		if (dwFLAGS & scf_Modify)
		{
			AddDef("Set",		"Установить",		2,		&CSimpleCollection::procSet,	&CSimpleCollection::defsFALSE);
			AddDef("Add",		"Добавить",			1,		&CSimpleCollection::procAdd,	&CSimpleCollection::defsFALSE);
			AddDef("Remove",	"Удалить",			1,		&CSimpleCollection::procRemove,	&CSimpleCollection::defsFALSE);
			AddDef("Clear",		"Очистить",			0,		&CSimpleCollection::procClear);
			AddDef("Load",		"Загрузить",		1,		&CSimpleCollection::procLoad,	&CSimpleCollection::defsFALSE);

			if (dwFLAGS & scf_Insert)
				AddDef("Insert",	"Вставить",			2,		&CSimpleCollection::procInsert,	&CSimpleCollection::defsFALSE);

			if (dwFLAGS & scf_Move)
				AddDef("Move",		"Сдвинуть",			2,		&CSimpleCollection::procMove,	&CSimpleCollection::defsFALSE);
		}
	}

	LPCSTR TypeString()
	{
		return scf_Readonly ? "FixedSimpleCollection" : "SimpleCollection";
	}

public:
	//////////////////////////////////////////////////////////////////////////
	// CSimpleCollection bridge interface
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

	BOOL procAdd(CValue** ppVal)
	{
		sc_Add(*ppVal[0]);
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

	BOOL procLoad(CValue** ppVal)
	{
		sc_Load(*ppVal[0]);
		return TRUE;
	}

	BOOL procInsert(CValue** ppVal)
	{
		sc_Insert(*ppVal[0], *ppVal[1]);
		return TRUE;
	}

	BOOL procMove(CValue** ppVal)
	{
		sc_Move(*ppVal[0], *ppVal[1]);
		return TRUE;
	}

	//////////////////////////////////////////////////////////////////////////
	//	CSimpleCollection "virtual" interface
	//////////////////////////////////////////////////////////////////////////

	int sc_GetIndex(CValue const& val) const
	{
		if (val.GetTypeCode() == NUMBER_TYPE_1C)
		{
			int nIndex = val.GetNumeric();
			return nIndex == -1 ? GetSize() - 1 : nIndex;
		}
		else
			throw sc_exception("Неверный тип индекса!");
	}

	size_t sc_GetSize() const
	{
		return GetSize();
	}

	void f_GetAt(CValue& vRetVal, int nIndex) const
	{
		CSC_CMN::AssignItem(vRetVal, GetAt(nIndex));
	}

	void sc_GetAt(CValue& RetVal, CValue const& vIndex) const
	{
		f_GetAt(RetVal, sc_GetIndex(vIndex));
	}

	void sc_Unload(CValue& vContext)
	{
		boost::shared_ptr<IResultLoader> pLoader(CValue2ResultLoader(vContext));
		IResultPublisher* pPublisher = this;
		CSC_CMN::DataExchange(pPublisher, pLoader.get());
	}

	template<bool B_SEL>
	void t_SetAt(CValue const& vIndex, CValue const& vItem)
	{
	}

	template<>
	void t_SetAt<true>(CValue const& vIndex, CValue const& vItem)
	{
		SetAt(sc_GetIndex(vIndex), CSC_CMN::GetItem<ITEM>(vItem));
	}

	void sc_SetAt(CValue const& vIndex, CValue const& vItem)
	{
		t_SetAt<dwFLAGS & scf_Modify>(vIndex, vItem);
	}

	template<bool B_SEL>
	void t_Add(CValue const& vItem)
	{
	}

	template <>
	void t_Add<true>(CValue const& vItem)
	{
		Add(CSC_CMN::GetItem<ITEM>(vItem));
	}

	void sc_Add(CValue const& vItem)
	{
		t_Add<(bool)(dwFLAGS & scf_Modify)>(vItem);
	}

	template<bool B_SEL>
	void t_Remove(CValue const& vIndex)
	{
	}

	template<>
	void t_Remove<true>(CValue const& vIndex)
	{
		Remove(sc_GetIndex(vIndex));
	}

	void sc_Remove(CValue const& vIndex)
	{
		t_Remove<(bool)(dwFLAGS & scf_Modify)>(vIndex);
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

	void sc_Load(CValue& vContext)
	{
		IResultLoader* pLoader = this;
		boost::shared_ptr<IResultPublisher> pPublisher (CValue2ResultPublisher(vContext)); //IResultPublisher* pPublisher = safe_dynamic_cast<IResultPublisher*>(vContext.GetContext());
		CSC_CMN::DataExchange(pPublisher.get(), pLoader);
	}

	template<bool B_SEL>
	void t_Insert(CValue const& vIndex, CValue const& vItem)
	{
	}

	template<>
	void t_Insert<true>(CValue const& vIndex, CValue const& vItem)
	{
		Insert(sc_GetIndex(vIndex), CSC_CMN::GetItem<ITEM>(vItem));
	}

	void sc_Insert(CValue const& vIndex, CValue const& vItem)
	{
		t_Insert<(bool)(dwFLAGS & scf_Insert)>(vIndex, vItem);
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
		return "";
	}

	//////////////////////////////////////////////////////////////////////////
	//	IResultLoader interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT ldrInit(DWORD& nColCount, DWORD& nRowCount) throw()
	{
		nColCount = 1;
		nRowCount = 0;
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
		sc_Add(*ppValues[0]);
		return NO_ERROR;
	}

	//////////////////////////////////////////////////////////////////////////
	//	IResultPublisher interface
	//////////////////////////////////////////////////////////////////////////

	virtual HRESULT pubInit(DWORD& nColCount, DWORD& nRowCount) throw()
	{
		if (!m_Value)
		{
			m_Value.reset(new CValue);
			m_pValue = m_Value.get();
		}
		m_nRowIndex = 0;

		nColCount = 1;
		nRowCount = sc_GetSize();
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
		f_GetAt(*m_Value, m_nRowIndex++);
		ppValues = &m_pValue;
		return NO_ERROR;
	}

private:
	static CSimpleDef m_StaticDef;
	static BOOL m_bStaticDefValid;

	// loader & publisher
	size_t m_nRowIndex;
	boost::scoped_ptr<CValue> m_Value;
	CValue const* m_pValue;
};

template <typename COLLTYPE, DWORD dwFLAGS>
CSimpleDef CSimpleCollection<COLLTYPE, dwFLAGS>::m_StaticDef;

template <typename COLLTYPE, DWORD dwFLAGS>
BOOL CSimpleCollection<COLLTYPE, dwFLAGS>::m_bStaticDefValid = FALSE;

#endif
