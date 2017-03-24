#ifndef ASSOC_VECTOR_H_INCLUDED
#define ASSOC_VECTOR_H_INCLUDED

#include "AssocCollection.h"
#include "boost/shared_ptr.hpp"
#include <functional>

//////////////////////////////////////////////////////////////////////////
// CSimpleAssociator class (c) kms 2008
//////////////////////////////////////////////////////////////////////////

template <typename KEY, typename KEYREF, typename ASSOC, typename ASSOCREF>
class CSimpleAssociator
{
public:
	size_t GetSize() const;
	ASSOC* GetAt(KEYREF key) const;
	void SetAt(KEYREF, ASSOCREF);
	BOOL Remove(KEYREF);
	void Clear();
};

template <typename ASSOC, typename ASSOCREF>
class CSimpleAssociator<LPCSTR, LPCSTR, typename ASSOC, typename ASSOCREF>
{
private:
	typedef CIStringMap<ASSOC, ASSOCREF> MapType;
	typedef boost::shared_ptr<CIStringMap<ASSOC, ASSOCREF> > MapPtr;
	MapPtr m_map;
public:
	CSimpleAssociator() : m_map(new MapType(true)) {}

	size_t GetSize() const
	{
		return m_map->GetCount();
	}
	ASSOCREF GetAt(LPCSTR key) const
	{
		ASSOCREF pAssoc = NULL;
		if (!IsEmptyKey(key))
			m_map->Lookup(key, pAssoc);
		return pAssoc;
	}
	void SetAt(LPCSTR key, ASSOCREF assoc)
	{
		if (!IsEmptyKey(key))
			m_map->SetAt(key, assoc);
	}
	BOOL Remove(LPCSTR key)
	{
		if (!IsEmptyKey(key))
			return m_map->RemoveKey(key);
		else
			return FALSE;
	}
	void Clear()
	{
		m_map->RemoveAll();
	}
private:
	static BOOL IsEmptyKey(LPCSTR key)
	{
		return !key || *key == 0;
	}
};

//////////////////////////////////////////////////////////////////////////
// CSimpleAssoc class (c) kms 2008
//////////////////////////////////////////////////////////////////////////
template <typename KEY, typename KEYREF, typename VAL, typename VALREF>
class CSimpleAssoc
{
public:
	CSimpleAssoc();
	CSimpleAssoc(KEYREF key, VALREF val);
	~CSimpleAssoc();
	KEYREF GetKey() const;
	VALREF GetVal() const;
	void SetVal(VALREF val);
};

template <typename VAL, typename VALREF>
class CSimpleAssoc<LPCSTR, LPCSTR, VAL, VALREF>
{
private:
	LPSTR m_key;
	VAL m_val;

public:
	CSimpleAssoc() : m_key(NULL) {}
	CSimpleAssoc(LPCSTR key, VALREF val) : m_key(NULL)
	{
		if (key && *key != 0)
		{
			size_t nLen = strlen(key) + 1;
			m_key = new char[nLen];
			strcpy(m_key, key);
		}
		SetVal(val);
	}
	~CSimpleAssoc() { delete[] m_key; }
	LPCSTR GetKey() const { return m_key; }
	VALREF GetVal() const { return m_val; }
	void SetVal(VALREF val) { m_val = val; }
};

//////////////////////////////////////////////////////////////////////////
// CAssocVector class (c) kms 2008
//////////////////////////////////////////////////////////////////////////

template <typename KEY, typename KEYREF, typename VAL, typename VALREF>
class CAssocVector : public std::vector<boost::shared_ptr<CSimpleAssoc<KEY, KEYREF, VAL, VALREF> > >
{
public:
	typedef KEY KEY;
	typedef KEYREF KEYREF;
	typedef VAL VAL;
	typedef VALREF VALREF;

	typedef CSimpleAssoc<KEY, KEYREF, VAL, VALREF> ASSOC;
 	typedef boost::shared_ptr<ASSOC> ITEM;
 	typedef ITEM const& ITEMREF;

	typedef CSimpleAssociator<KEY, KEYREF, ASSOC*, ASSOC*> CAssociator;
private:
	CAssociator m_Associator;

public:
	virtual ~CAssocVector() {}

private:
	void BadIndex() const
	{
		throw sc_exception("Неверное значение индекса!");
	}

	void BadKey() const
	{
		throw sc_exception("Неверное значение ключа!");
	}

	void CheckIndex(size_t nIndex, size_t nSize) const
	{
		if (nIndex >= nSize)
			BadIndex();
	}

	void CheckKey(KEYREF key) const
	{
		GetAssocAt(key);
	}

public:
	// complexity O(N)
	int GetIndexOf(KEYREF key, BOOL bQuiet = FALSE) const
	{
		ASSOC* pAssoc = GetAssocAt(key, bQuiet);
		if (!pAssoc)
			return -1;
		const_iterator ib = begin(), ie = end();
		for (const_iterator i = ib; i != ie; ++i)
			if (i->get() == pAssoc)
				return std::distance(ib, i);
		// never occurs
		throw sc_exception("Internal error!");
	}

	// complexity const
	ASSOC* GetAssocAt(size_t nIndex, BOOL bQuiet = FALSE) const
	{
		if (nIndex >= GetSize())
			if (!bQuiet)
				BadIndex();
			else
				return NULL;
		
		return operator[](nIndex).get();
	}

	// complexity const
	ASSOC* GetAssocAt(KEYREF key, BOOL bQuiet = FALSE) const
	{
		ASSOC* pAssoc = m_Associator.GetAt(key);
		if (!pAssoc && !bQuiet)
			BadKey();

		return pAssoc;
	}
	size_t GetSize() const
	{
		return size();
	}

	size_t GetAssocSize() const
	{
		return m_Associator.GetSize();
	}

	VALREF GetAt(size_t nIndex) const
	{
		return GetAssocAt(nIndex)->GetVal();
	}

	VALREF GetAt(KEYREF key) const
	{
		return GetAssocAt(key)->GetVal();
	}

	void SetAt(size_t nIndex, VALREF val)
	{
		GetAssocAt(nIndex)->SetVal(val);
	}

	void SetAt(KEYREF key, VALREF val)
	{
		GetAssocAt(key)->SetVal(val);
	}

	void Add(VALREF val, KEYREF key)
	{
		if (GetAssocAt(key, TRUE))
			BadKey();

		ASSOC* pAssoc = new ASSOC(key, val);
		push_back(ITEM(pAssoc));
		m_Associator.SetAt(pAssoc->GetKey(), pAssoc);
	}

	void Remove(size_t nIndex)
	{
		m_Associator.Remove(GetAssocAt(nIndex)->GetKey());
		erase(begin() + nIndex);
	}

	void Remove(KEYREF key)
	{
		CheckKey(key);
		erase(begin() + GetIndexOf(key));
		m_Associator.Remove(key);
	}

	void Clear()
	{
		m_Associator.Clear();
		clear();
	}

	void Insert(size_t nIndex, VALREF val, KEYREF key)
	{
		if (nIndex == GetSize())
			Add(val, key);
		else
		{
			if (GetAssocAt(key, TRUE))
				BadKey();

			CheckIndex(nIndex, GetSize());
			ASSOC* pAssoc = new ASSOC(key, val);
			insert(begin() + nIndex, ITEM(pAssoc));
			m_Associator.SetAt(pAssoc->GetKey(), pAssoc);

		}
	}

	void Move(size_t nIndex, int nOffset)
	{
		if (nOffset)
		{
			CheckIndex(nIndex, GetSize());
			int nInsertPos = (int)nIndex + nOffset;
			if (nInsertPos < 0 || nInsertPos >= (int)GetSize())
				throw sc_exception("Неверное смещение!");

			iterator pb = begin();
			if (nOffset > 0)
				std::rotate(pb + nIndex, pb + nIndex + 1, pb + nInsertPos + 1);
			else
				std::rotate(pb + nInsertPos, pb + nIndex, pb + nIndex + 1);
		}
	}
};
#endif
