//////////////////////////////////////////////////////////////////////
// istrmap.h: interface for the CIStringMap class.
//            interface for the CCollection class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@ezmail.ru
// Version: 2.0
//
// CIStringMap: implementation of an associative array with a key as case insencitive string.
// Implemented on base of MFC source code. Completly save interface of MFC class СMap.
//
// CCollection: array of named item poiners.
//////////////////////////////////////////////////////////////////////

#pragma once

#include <afxtempl.h>

template<class VALUE, class ARG_VALUE>
class CIStringMap : public CObject
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		UINT nHashValue;  // needed for efficient iteration
//		CString key;
		LPSTR key;
		VALUE value;
	};
public:
// Construction
	CIStringMap(int nBlockSize = 10);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(LPCSTR key, VALUE& rValue) const;

// Operations
	// Lookup and add if not there
	VALUE& operator[](LPCSTR key);

	// add a new (key, value) pair
	void SetAt(LPCSTR key, ARG_VALUE newValue);

	// removing existing (key, ?) pair
	BOOL RemoveKey(LPCSTR key);
	void RemoveAll();

	// iterating all (key, value) pairs
	POSITION GetStartPosition() const;

	// rKey возвращается по ссылке, эта функция рекомендуется к использованию =kms
	void GetNextAssoc(POSITION& rNextPosition, LPSTR& rKey, VALUE& rValue) const;
	// rKey копируется в CString (оставлено для совместимости с существующим кодом) =kms
	void GetNextAssoc(POSITION& rNextPosition, CString& rKey, VALUE& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;
	void InitHashTable(UINT hashSize, BOOL bAllocNow = TRUE);

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;
	int m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	int m_nBlockSize;

	CAssoc* NewAssoc(int len);
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(LPCSTR, UINT&) const;
public:
	~CIStringMap();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

UINT HashKey_CS1251i(LPCSTR key);

/////////////////////////////////////////////////////////////////////////////
// CIStringMap<VALUE, ARG_VALUE> inline functions

template<class VALUE, class ARG_VALUE>
AFX_INLINE int CIStringMap<VALUE, ARG_VALUE>::GetCount() const
	{ return m_nCount; }
template<class VALUE, class ARG_VALUE>
AFX_INLINE BOOL CIStringMap<VALUE, ARG_VALUE>::IsEmpty() const
	{ return m_nCount == 0; }
template<class VALUE, class ARG_VALUE>
AFX_INLINE void CIStringMap<VALUE, ARG_VALUE>::SetAt(LPCSTR key, ARG_VALUE newValue)
	{ (*this)[key] = newValue; }
template<class VALUE, class ARG_VALUE>
AFX_INLINE POSITION CIStringMap<VALUE, ARG_VALUE>::GetStartPosition() const
	{ return (m_nCount == 0) ? NULL : BEFORE_START_POSITION; }
template<class VALUE, class ARG_VALUE>
AFX_INLINE UINT CIStringMap<VALUE, ARG_VALUE>::GetHashTableSize() const
	{ return m_nHashTableSize; }

/////////////////////////////////////////////////////////////////////////////
// CIStringMap<VALUE, ARG_VALUE> out-of-line functions

template<class VALUE, class ARG_VALUE>
CIStringMap<VALUE, ARG_VALUE>::CIStringMap(int nBlockSize)
{
	ASSERT(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = 17;  // default size
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;
	m_nBlockSize = nBlockSize;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::InitHashTable(
	UINT nHashSize, BOOL bAllocNow)
//
// Used to force allocation of a hash table or to override the default
//   hash table size of (which is fairly small)
{
	ASSERT_VALID(this);
	ASSERT(m_nCount == 0);
	ASSERT(nHashSize > 0);

	if (m_pHashTable != NULL)
	{
		// free hash table
		delete[] m_pHashTable;
		m_pHashTable = NULL;
	}

	if (bAllocNow)
	{
		m_pHashTable = new CAssoc* [nHashSize];
		memset(m_pHashTable, 0, sizeof(CAssoc*) * nHashSize);
	}
	m_nHashTableSize = nHashSize;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::RemoveAll()
{
	ASSERT_VALID(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElements<VALUE>(&pAssoc->value, 1);
				free( pAssoc->key );
//				DestructElements<CString>(&pAssoc->key, 1);
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;

	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks->FreeDataChain();
	m_pBlocks = NULL;
}

template<class VALUE, class ARG_VALUE>
CIStringMap<VALUE, ARG_VALUE>::~CIStringMap()
{
	RemoveAll();
	ASSERT(m_nCount == 0);
}

template<class VALUE, class ARG_VALUE>
CIStringMap<VALUE, ARG_VALUE>::CAssoc*
CIStringMap<VALUE, ARG_VALUE>::NewAssoc(int len)
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CIStringMap::CAssoc));
		// chain them into free list
		CIStringMap::CAssoc* pAssoc = (CIStringMap::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; i--, pAssoc--)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ASSERT(m_pFreeList != NULL);  // we must have something

	CIStringMap::CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	m_nCount++;
	ASSERT(m_nCount > 0);  // make sure we don't overflow
//	ConstructElements<CString>(&pAssoc->key, 1);
	pAssoc->key = static_cast<LPSTR>(malloc( len * sizeof(char) ));
	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::FreeAssoc(CIStringMap::CAssoc* pAssoc)
{
	DestructElements<VALUE>(&pAssoc->value, 1);
	free( (void *)pAssoc->key );
//	DestructElements<CString>(&pAssoc->key, 1);
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	m_nCount--;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow

	// if no more elements, cleanup completely
	if (m_nCount == 0)
		RemoveAll();
}

template<class VALUE, class ARG_VALUE>
CIStringMap<VALUE, ARG_VALUE>::CAssoc*
CIStringMap<VALUE, ARG_VALUE>::GetAssocAt(LPCSTR key, UINT& nHash) const
// find association (or return NULL)
{
	nHash = HashKey_CS1251i(key) % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (!stricmp(pAssoc->key, key))
			return pAssoc;
	}
	return NULL;
}

template<class VALUE, class ARG_VALUE>
BOOL CIStringMap<VALUE, ARG_VALUE>::Lookup(LPCSTR key, VALUE& rValue) const
{
	ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc = GetAssocAt(key, nHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class VALUE, class ARG_VALUE>
VALUE& CIStringMap<VALUE, ARG_VALUE>::operator[](LPCSTR key)
{
	ASSERT_VALID(this);

	UINT nHash;
	CAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nHash)) == NULL)
	{
		if (m_pHashTable == NULL)
			InitHashTable(m_nHashTableSize);

		// it doesn't exist, add a new Association
		pAssoc = NewAssoc(strlen(key) +1);
		pAssoc->nHashValue = nHash;

		strcpy (pAssoc->key, key);
		// 'pAssoc->value' is a constructed object, nothing more

		// put into hash table
		pAssoc->pNext = m_pHashTable[nHash];
		m_pHashTable[nHash] = pAssoc;
	}
	return pAssoc->value;  // return new reference
}

template<class VALUE, class ARG_VALUE>
BOOL CIStringMap<VALUE, ARG_VALUE>::RemoveKey(LPCSTR key)
// remove key - return TRUE if removed
{
	ASSERT_VALID(this);

	if (m_pHashTable == NULL)
		return FALSE;  // nothing in the table

	CAssoc** ppAssocPrev;
	ppAssocPrev = &m_pHashTable[HashKey_CS1251i(key) % m_nHashTableSize];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (!stricmp(pAssoc->key, key))
		{
			// remove it
			*ppAssocPrev = pAssoc->pNext;  // remove from list
			FreeAssoc(pAssoc);
			return TRUE;
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	LPSTR& rKey, VALUE& rValue) const
{
	ASSERT_VALID(this);
	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocRet = m_pHashTable[nBucket]) != NULL)
				break;
		ASSERT(pAssocRet != NULL);  // must find something
	}

	// find next association
	ASSERT(AfxIsValidAddress(pAssocRet, sizeof(CAssoc)));
	CAssoc* pAssocNext;
	if ((pAssocNext = pAssocRet->pNext) == NULL)
	{
		// go to next bucket
		for (UINT nBucket = pAssocRet->nHashValue + 1;
		  nBucket < m_nHashTableSize; nBucket++)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	CString& rKey, VALUE& rValue) const
{
	LPSTR lpstrKey = NULL;

	GetNextAssoc(rNextPosition, lpstrKey, rValue);
	rKey = lpstrKey;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::Serialize(CArchive& ar)
{
	ASSERT_VALID(this);

	CObject::Serialize(ar);

	if (ar.IsStoring())
	{
		ar.WriteCount(m_nCount);
		if (m_nCount == 0)
			return;  // nothing more to do

		ASSERT(m_pHashTable != NULL);
		for (UINT nHash = 0; nHash < m_nHashTableSize; nHash++)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				CString key = pAssoc->key;
//				SerializeElements<CString>(ar, &pAssoc->key, 1);
				SerializeElements<CString>(ar, &key, 1);
				SerializeElements<VALUE>(ar, &pAssoc->value, 1);
			}
		}
	}
	else
	{
		DWORD nNewCount = ar.ReadCount();
		while (nNewCount--)
		{
			CString newKey;
			VALUE newValue;
			SerializeElements<CString>(ar, &newKey, 1);
			SerializeElements<VALUE>(ar, &newValue, 1);
			SetAt(newKey, newValue);
		}
	}
}

#ifdef _DEBUG
template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);

	dc << "with " << m_nCount << " elements";
	if (dc.GetDepth() > 0)
	{
		// Dump in format "[key] -> value"
		CString key;
		VALUE val;

		POSITION pos = GetStartPosition();
		while (pos != NULL)
		{
			GetNextAssoc(pos, key, val);
			dc << "\n\t[";
			DumpElements<CString>(dc, &key, 1);
			dc << "] = ";
			DumpElements<VALUE>(dc, &val, 1);
		}
	}

	dc << "\n";
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::AssertValid() const
{
	CObject::AssertValid();

	ASSERT(m_nHashTableSize > 0);
	ASSERT(m_nCount == 0 || m_pHashTable != NULL);
		// non-empty map should have hash table
}
#endif //_DEBUG

typedef CIStringMap<CString, LPCSTR> CIStringMapToString;
typedef CIStringMap<void*, void*> CIStringMapToPtr;
