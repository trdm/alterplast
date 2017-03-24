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
#pragma once

//#include <afxtempl.h>
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

	// добавим возможность переопределения хеш-функции
	virtual UINT HashKey(LPCSTR key) const;
	virtual int Compare(LPCSTR key1, LPCSTR key2) const;

	// функция для отладки hash функции
	int GetCollisionCount(bool bShowMsg = false) const;

	static UINT m_CS1251_upper[256];

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

template<class VALUE, class ARG_VALUE>
UINT CIStringMap<VALUE, ARG_VALUE>::m_CS1251_upper[256] =
{
    0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
    0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
    0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
    0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
//     @    A
    0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
//														 Z						  _ 
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
//     `    a
    0x60,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
//														 z						  ~
    0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x7B,0x7C,0x7D,0x7E,0x7F,
    0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
    0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
//                                             Ё
    0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
//											   ё
    0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xA8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
//	   А
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
//																				  Я
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
//	   а
    0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
//																				  я
    0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
};

template<class VALUE, class ARG_VALUE>
UINT CIStringMap<VALUE, ARG_VALUE>::HashKey(LPCSTR key) const
{
	/*
	if (ch = 0xB8)
	ch = 0xA8;
	else if (ch >= 0xE0 || ch >= 0x61 && ch <= 0x7A)
	ch -= 0x20;
	*/

    UINT ch;
//	UINT nHash = 0;

    UINT x1=0x14f2affc;
    UINT x2=0xfac5bd8f;
	UINT x3=0xe5fc82a9;
	
	// неочевидно, но без приведения к BYTE используется movsx!
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

template<class VALUE, class ARG_VALUE>
int CIStringMap<VALUE, ARG_VALUE>::Compare(LPCSTR key1, LPCSTR key2) const
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
		
		for (UINT b2 = 1; b2 < __max(new_nHashTableSize, DEFAULT_HASH_TABLE_SIZE); b2 *= 2);
		// и все же хеш-функция неравномерна
		new_nHashTableSize = b2 + 1;

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
	nFullHash = HashKey(key);
	UINT nHash = nFullHash % m_nHashTableSize;

	if (m_pHashTable == NULL)
		return NULL;

	// see if it exists
	CAssoc* pAssoc;
	for (pAssoc = m_pHashTable[nHash]; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nFullHash)
		{
			if (!Compare(pAssoc->key, key))
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
		pAssoc = NewAssoc(strlen(key) + 1);
		pAssoc->nHashValue = nFullHash;

		if ( !m_bUseExternalKeys )
		{
			strcpy (pAssoc->key, key);
		}
		else
		{
			// используем ключ по месту расположения
			// внимание! он не должен исчезнуть или измениться
			// до освобождения ассоциации
			pAssoc->key = const_cast<LPSTR>(key);
		}
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

	UINT nFullHash = HashKey(key);
	UINT nHash = nFullHash % m_nHashTableSize;
	CAssoc** ppAssocPrev = &m_pHashTable[nHash];

	CAssoc* pAssoc;
	for (pAssoc = *ppAssocPrev; pAssoc != NULL; pAssoc = pAssoc->pNext)
	{
		if (pAssoc->nHashValue == nFullHash)
		{
			if (!Compare(pAssoc->key, key))
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
    friend CCollectionItem;
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
