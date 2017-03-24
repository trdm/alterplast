//////////////////////////////////////////////////////////////////////
// istrmap.h: interface for the CIStringMap class.
//            interface for the CCollection class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@ezmail.ru
// Version: 2.3
//
// CIStringMap: implementation of an associative array with a key as case insensitive string.
// Implemented on base of MFC source code. Completly save interface of MFC class СMap.
//
// CCollection: array of named item poiners.
//////////////////////////////////////////////////////////////////////
#if !defined _ISTRMAP_H_INCLUDED_
#define _ISTRMAP_H_INCLUDED_

#pragma once

#define DEFAULT_HASH_TABLE_SIZE 32
#define DEFAULT_BLOCK_SIZE_RATIO 4
#define DEFAULT_MAX_BLOCK_SIZE 128

template<class VALUE, class ARG_VALUE>
class CIStringMap : public CObject
{
protected:
	// Association
	struct CAssoc
	{
		CAssoc* pNext;
		// hash хранится полностью, так, как его возвращает HashKey()
		// используется для эффективного перестроения hash таблиц и предварительного сравнения
		UINT nHashValue;  // needed for efficient iteration
		LPSTR key;
		VALUE value;
	};
public:
// Construction
	CIStringMap(bool bUseExternalKeys = false);

// Attributes
	// number of elements
	int GetCount() const;
	BOOL IsEmpty() const;

	// Lookup
	BOOL Lookup(LPCSTR key, VALUE& rValue) const;
	BOOL LookupKey(LPCSTR key, LPCSTR& rKey) const;

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
	void GetNextAssoc(POSITION& rNextPosition, LPCSTR& rKey, VALUE& rValue) const;
	// rKey копируется в CString (оставлено для совместимости с существующим кодом) =kms
	void GetNextAssoc(POSITION& rNextPosition, CString& rKey, VALUE& rValue) const;

	// advanced features for derived classes
	UINT GetHashTableSize() const;

	// метод динамически и безопасно (без потери указателей) управляет размером hash table
	void InitHashTable(UINT new_nHashTableSize);

	// функция для отладки hash функции
	int GetCollisionCount(bool bShowMsg = false) const;

	static UINT m_CS1251_upper[256];

	template<typename UnaryOp>
	void ForEachValue(UnaryOp op);

	template<typename BinaryOp>
	void ForEachPair(BinaryOp op);

// Implementation
protected:
	CAssoc** m_pHashTable;
	UINT m_nHashTableSize;

	UINT m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;
	UINT m_nBlockSize;

	CAssoc* NewAssoc(size_t nKeySize);
	void FreeAssoc(CAssoc*);
	CAssoc* GetAssocAt(LPCSTR, UINT&) const;

	CAssoc* InnerGetNextAssoc(POSITION& rNextPosition) const;

	// управляет реализацией хранения ключей:
	// false: выделяет память, копирует ключ
	// truе:  использует ключ по ссылке
	bool m_bUseExternalKeys;
public:
	~CIStringMap();
	void Serialize(CArchive&);
#ifdef _DEBUG
	void Dump(CDumpContext&) const;
	void AssertValid() const;
#endif
};

class CIStringMapCMN
{
public:
	static inline UINT HashKey(LPCSTR key)
	{
		UINT ch;
		//	UINT nHash = 0;

		UINT x1=0x14f2affc;
		UINT x2=0xfac5bd8f;
		UINT x3=0xe5fc82a9;

		while(ch = static_cast<UINT>(static_cast<BYTE>(*key++)))
		{
			//		 nHash = (nHash<<5) + nHash + m_CS1251_upper[ch];

			// более равномерна на коротких строках
			x1 += _rotl(x2, 4) + x3;
			x2 += _rotl(x1, 4) + m_CS1251_upper[ch];
		};
		//	return nHash;
		return x1 + x2;
	}

	static inline int Compare(LPCSTR key1, LPCSTR key2)
	{
		int result;
		UINT n1, n2;
		UINT ch2;

		while (
			// именно в этом порядке
			ch2 = static_cast<UINT>(static_cast<BYTE>(*key2++)),
			n2 = m_CS1251_upper[ch2],
			n1 = m_CS1251_upper[static_cast<UINT>(static_cast<BYTE>(*key1++))],
			!( result = n1 - n2) && ch2);

		return result;
	}
private:
	static UINT m_CS1251_upper[256];
};

/*
template<class VALUE, class ARG_VALUE>
int CIStringMap<VALUE, ARG_VALUE>::GetCollisionCount(bool bShowMsg) const
{
	int nUnunique = 0, nMaxBucket = 0, nBuckets = 0, nNulls = 0;
	double nCollisions = 0;

	if (m_pHashTable != NULL)
	{
		for (UINT nHash = 0; nHash < m_nHashTableSize; ++nHash)
		{
			CAssoc* pAssoc;
			int nCurrent = 0;
			for ((pAssoc = m_pHashTable[nHash]) ? 0 : ++nNulls; pAssoc; pAssoc = pAssoc->pNext, ++nCurrent);

			if (nCurrent == 1)
				--nCurrent;

			if (nCurrent)
			{
				++nBuckets;

				// (nCurrent - 1)!
				double fact = 1;
				for (int i = 2; i < nCurrent; fact *=i, ++i);

				nCollisions += fact;
			}

			nUnunique += nCurrent;
			nMaxBucket = max(nMaxBucket, nCurrent);
		}
	}
	if (bShowMsg)
	{
		double nRatio = (m_nCount <2) ? 0 : ((double)nCollisions) / ((double)m_nCount * (m_nCount - 1) / (2 * m_nHashTableSize));
		Msg("Ununique hashes: %i, Collisions : %g, Buckets: %i, Max bucket size: %i, Nulls: %i, Associations count: %i, Hash table size: %i, Hash function quality: %g",
			nUnunique, nCollisions, nBuckets, nMaxBucket, nNulls, m_nCount, m_nHashTableSize, nRatio);
	}

	return nCollisions;
}
*/
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
CIStringMap<VALUE, ARG_VALUE>::CIStringMap(bool bUseExternalKeys)
{
//	ASSERT(nBlockSize > 0);

	m_pHashTable = NULL;
	m_nHashTableSize = DEFAULT_HASH_TABLE_SIZE;
	m_nCount = 0;
	m_pFreeList = NULL;
	m_pBlocks = NULL;

	//управление размером блока перенесено в InitHashTable()
	//m_nBlockSize = DEFAULT_BLOCK_SIZE;

	m_bUseExternalKeys = bUseExternalKeys;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::InitHashTable(UINT new_nHashTableSize)
{
	ASSERT_VALID(this);
	
	// возможно только увеличение или корректное уменьшение размера таблицы
	if ((new_nHashTableSize > m_nCount) || (new_nHashTableSize > m_nHashTableSize))
	{
		BOOL bNeedDelete = reinterpret_cast<BOOL>(m_pHashTable);
		
		{
			UINT b2;
			for (b2 = 1; b2 < __max(new_nHashTableSize, DEFAULT_HASH_TABLE_SIZE); b2 *= 2);
			// и все же хеш-функция неравномерна
			new_nHashTableSize = b2 + 1;
		}

		CAssoc **new_pHashTable = new CAssoc* [new_nHashTableSize];
		memset(new_pHashTable, NULL, sizeof(CAssoc*) * new_nHashTableSize);
		
		if ( UINT nCount = m_nCount )
		{
			// изменение размера и перенос ассоциаций
			CAssoc *pAssoc, *pAssocNext;
			
			for (UINT nHash = 0; nCount && (nHash < m_nHashTableSize); ++nHash)
			{
				for (pAssoc = m_pHashTable[nHash]; pAssoc; --nCount, pAssoc = pAssocNext)
				{
					UINT nNewHash = pAssoc->nHashValue % new_nHashTableSize;
					pAssocNext = pAssoc->pNext;
					
					pAssoc->pNext = new_pHashTable[nNewHash];
					new_pHashTable[nNewHash] = pAssoc;
				}
			}
		}
		
		if (bNeedDelete)
		{
			delete[] m_pHashTable;
		}
		m_pHashTable = new_pHashTable;
		m_nHashTableSize = new_nHashTableSize;

		// в общем случае влияние размера блока незначительно
		m_nBlockSize = __min(DEFAULT_MAX_BLOCK_SIZE, m_nHashTableSize / DEFAULT_BLOCK_SIZE_RATIO);
	}
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::RemoveAll()
{
	ASSERT_VALID(this);

	if (m_pHashTable != NULL)
	{
		// destroy elements (values and keys)
		for (UINT nHash = 0; nHash < m_nHashTableSize; ++nHash)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				DestructElements<VALUE>(&pAssoc->value, 1);
				if ( !m_bUseExternalKeys )
				{
					free( pAssoc->key );
				}
			}
		}
	}

	// free hash table
	delete[] m_pHashTable;
	m_pHashTable = NULL;
	m_nHashTableSize = DEFAULT_HASH_TABLE_SIZE;

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
typename CIStringMap<VALUE, ARG_VALUE>::CAssoc*
CIStringMap<VALUE, ARG_VALUE>::NewAssoc(size_t nKeySize)
{
	if (m_pFreeList == NULL)
	{
		// add another block
		CPlex* newBlock = CPlex::Create(m_pBlocks, m_nBlockSize, sizeof(CIStringMap::CAssoc));
		// chain them into free list
		CIStringMap::CAssoc* pAssoc = (CIStringMap::CAssoc*) newBlock->data();
		// free in reverse order to make it easier to debug
		pAssoc += m_nBlockSize - 1;
		for (int i = m_nBlockSize-1; i >= 0; --i, --pAssoc)
		{
			pAssoc->pNext = m_pFreeList;
			m_pFreeList = pAssoc;
		}
	}
	ASSERT(m_pFreeList != NULL);  // we must have something

	CIStringMap::CAssoc* pAssoc = m_pFreeList;
	m_pFreeList = m_pFreeList->pNext;
	++m_nCount;
	ASSERT(m_nCount > 0);  // make sure we don't overflow
	if ( !m_bUseExternalKeys )
	{
		pAssoc->key = static_cast<LPSTR>(malloc( nKeySize * sizeof(char) ));
	}
	ConstructElements<VALUE>(&pAssoc->value, 1);   // special construct values
	return pAssoc;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::FreeAssoc(typename CIStringMap::CAssoc* pAssoc)
{
	DestructElements<VALUE>(&pAssoc->value, 1);
	if ( !m_bUseExternalKeys )
	{
		free( (void *)pAssoc->key );
	}
	pAssoc->pNext = m_pFreeList;
	m_pFreeList = pAssoc;
	--m_nCount;
	ASSERT(m_nCount >= 0);  // make sure we don't underflow
}

template<class VALUE, class ARG_VALUE>
typename CIStringMap<VALUE, ARG_VALUE>::CAssoc*
CIStringMap<VALUE, ARG_VALUE>::GetAssocAt(LPCSTR key, UINT& nFullHash) const
// find association (or return NULL)
{
	nFullHash = CIStringMapCMN::HashKey(key);
	UINT nHash = nFullHash % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nFullHash)
		{
			if (!CIStringMapCMN::Compare(pAssoc->key, key))
				return pAssoc;
		}
	}
	return NULL;
}

template<class VALUE, class ARG_VALUE>
BOOL CIStringMap<VALUE, ARG_VALUE>::Lookup(LPCSTR key, VALUE& rValue) const
{
	ASSERT_VALID(this);

	UINT nFullHash;
	CAssoc* pAssoc = GetAssocAt(key, nFullHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rValue = pAssoc->value;
	return TRUE;
}

template<class VALUE, class ARG_VALUE>
BOOL CIStringMap<VALUE, ARG_VALUE>::LookupKey(LPCSTR key, LPCSTR& rKey) const
{
	ASSERT_VALID(this);

	UINT nFullHash;
	CAssoc* pAssoc = GetAssocAt(key, nFullHash);
	if (pAssoc == NULL)
		return FALSE;  // not in map

	rKey = pAssoc->key;
	return TRUE;
}

template<class VALUE, class ARG_VALUE>
VALUE& CIStringMap<VALUE, ARG_VALUE>::operator[](LPCSTR key)
{
	ASSERT_VALID(this);

	UINT nFullHash;
	CAssoc* pAssoc;
	if ((pAssoc = GetAssocAt(key, nFullHash)) == NULL)
	{
		if ( !m_pHashTable )
		{
			InitHashTable( m_nHashTableSize );
		}
		else if ( m_nCount * 2 >= m_nHashTableSize )
		{
			// увеличение размера hash table
			InitHashTable( m_nHashTableSize * 2 );
		}

		UINT nHash = nFullHash % m_nHashTableSize;

		// it doesn't exist, add a new Association
		if ( !m_bUseExternalKeys )
		{
			pAssoc = NewAssoc(strlen(key) + 1);
			strcpy (pAssoc->key, key);
		}
		else
		{
			// используем ключ по месту расположения
			// внимание! он не должен исчезнуть или измениться
			// до освобождения ассоциации
			pAssoc = NewAssoc(0);
			pAssoc->key = const_cast<LPSTR>(key);
		}
		pAssoc->nHashValue = nFullHash;
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

	UINT nFullHash = CIStringMapCMN::HashKey(key);
	UINT nHash = nFullHash % m_nHashTableSize;
	CAssoc** ppAssocPrev = &m_pHashTable[nHash];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nFullHash)
		{
			if (!CIStringMapCMN::Compare(pAssoc->key, key))
			{
				// remove it
				*ppAssocPrev = pAssoc->pNext;  // remove from list
				FreeAssoc(pAssoc);

				// уменьшение размера hash table
				if ( (m_nHashTableSize > DEFAULT_HASH_TABLE_SIZE) && (m_nCount * 8 < m_nHashTableSize) )
				{
					InitHashTable( m_nHashTableSize / 2 );
				}
				return TRUE;
			}
		}
		ppAssocPrev = &pAssoc->pNext;
	}
	return FALSE;  // not found
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	LPCSTR& rKey, VALUE& rValue) const
{
	CAssoc* pAssocRet = InnerGetNextAssoc(rNextPosition);

	// fill in return data
	rKey = pAssocRet->key;
	rValue = pAssocRet->value;
}

template<class VALUE, class ARG_VALUE>
void CIStringMap<VALUE, ARG_VALUE>::GetNextAssoc(POSITION& rNextPosition,
	CString& rKey, VALUE& rValue) const
{
	LPCSTR lpstrKey = NULL;

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
		for (UINT nHash = 0; nHash < m_nHashTableSize; ++nHash)
		{
			CAssoc* pAssoc;
			for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL;
			  pAssoc = pAssoc->pNext)
			{
				CString key(pAssoc->key);
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
			SetAt(reinterpret_cast<LPCSTR>(newKey.GetBuffer(0)), newValue);
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

 template<class VALUE, class ARG_VALUE>
 template<typename UnaryOp>
 void CIStringMap<VALUE, ARG_VALUE>::ForEachValue(UnaryOp op)
 {
 	for (POSITION pos = GetStartPosition(); pos != NULL;)
 	{
 		CAssoc* pAssoc = InnerGetNextAssoc( pos);
 		op(pAssoc->value);
 	}
 }
 
 template<class VALUE, class ARG_VALUE>
 template<typename BinaryOp>
 void CIStringMap<VALUE, ARG_VALUE>::ForEachPair(BinaryOp op)
 {
 	for (POSITION pos = GetStartPosition(); pos != NULL;)
 	{
 		CAssoc* pAssoc = InnerGetNextAssoc(pos);
 		op(pAssoc->key, pAssoc->value);
 	}
 }

template<class VALUE, class ARG_VALUE>
typename CIStringMap<VALUE, ARG_VALUE>::CAssoc* CIStringMap<VALUE, ARG_VALUE>::InnerGetNextAssoc(POSITION& rNextPosition) const
{
	ASSERT_VALID(this);
	ASSERT(m_pHashTable != NULL);  // never call on empty map

	CAssoc* pAssocRet = (CAssoc*)rNextPosition;
	ASSERT(pAssocRet != NULL);

	if (pAssocRet == (CAssoc*) BEFORE_START_POSITION)
	{
		// find the first association
		for (UINT nBucket = 0; nBucket < m_nHashTableSize; ++nBucket)
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
		for (UINT nBucket = pAssocRet->nHashValue % m_nHashTableSize + 1;
			nBucket < m_nHashTableSize; ++nBucket)
			if ((pAssocNext = m_pHashTable[nBucket]) != NULL)
				break;
	}

	rNextPosition = (POSITION) pAssocNext;

	return pAssocRet;
}

typedef CIStringMap<CString, CString&> CIStringMapToString;
typedef CIStringMap<CObject*, CObject*> CIStringMapToOb;
typedef CIStringMap<int, int> CIStringMapToInt;
typedef CIStringMap<void*, void*> CIStringMapToPtr;

//Класс для связи строк с _непрерывным_ индексом.
class CIStringMapToIndex : public CIStringMapToInt
{
private:
	void ShiftIndexes(int nStartIndex, int nDelta);

public:
	CIStringMapToIndex(bool bUseExternalKeys = false);
	int Add(LPCSTR szKey); //returns new index value. If szKey already in hash - returns -1
	void Insert(int nIndex, LPCSTR szKey);
	int Remove(LPCSTR szKey); //returns index of removed key
	void Remove(int nIndex);
	void Move(int nIndex, int nOffset);
};

class CCollection;

class CCollectionItem
{
    friend class CCollection;
public:
    CCollectionItem();
    ~CCollectionItem();
    LPCSTR GetName() const;
    void SetName(LPCSTR szName);
protected:
    inline CCollection* GetCollection() const { return m_pCollection; };
    inline void SetCollection( CCollection *pCollection ) { m_pCollection = pCollection; };
private:
	LPSTR m_Name;
    CCollection* m_pCollection;
};

class CCollection
{
	// ВАЖНО: удаление элементов коллекции при очистке коллекции не происходит,
	// т.к. коллекция не владеет этими элементами, клиент коллекции должен удалять эти элементы самостоятельно

    friend class CCollectionItem;
public:
	CCollection();
	~CCollection();

    int GetCount() const;
    int GetUpperBound() const;
    void Add(CCollectionItem* pItem);
    void Insert(int nIndex, CCollectionItem* pItem);
    void Remove(int nIndex);
    void Clear();
    CCollectionItem* operator [](int nIndex) const;
    CCollectionItem* operator [](LPCSTR szName) const;
    inline CCollectionItem* GetItem(int nIndex) const {return (*this)[nIndex];};
    inline CCollectionItem* GetItem(LPCSTR szName) const {return (*this)[szName];};
    int GetIndex(CCollectionItem* pItem) const;
    int GetIndex(LPCSTR szName) const;
    CCollectionItem** GetData() const;
    void Move(int nIndex, int nOffset);
protected:
    virtual void OnAdd(CCollectionItem* pItem);
    virtual void OnRemove(CCollectionItem* pItem, int nIndexWas); //Warning! When calling from Clear(), don't use
                                                                  //other members of this collection (they will
                                                                  //work wrong) and nIndexWas is equal -1.
    virtual void OnRename(CCollectionItem* pItem);
private:
    CTypedPtrArray<CPtrArray, CCollectionItem*> m_Items;
	CIStringMapToIndex m_ItemsHash;
};
#endif
