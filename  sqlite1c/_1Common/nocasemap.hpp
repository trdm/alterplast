/*
nocasemap.hpp
(c) Александр Орефков
Шаблон хеш-таблицы для строковых ключей без учета регистра
*/

#pragma once
#pragma warning(disable: 4786)	// Предупреждения о длинных именах

namespace ncm_symb{
__forceinline DWORD symbol(LPCSTR ptr){return static_cast<DWORD>(static_cast<BYTE>(*ptr));}
}

// Базовый класс, реализующий общую функциональность хеш-таблицы
class CNoCaseMapBase
{
public:
	int GetCount() const
		{return m_nCount;}
	BOOL IsEmpty() const
		{return m_nCount!=0;}
	POSITION GetStartPosition() const
		{return m_nCount ? BEFORE_START_POSITION : NULL;}
	UINT GetHashTableSize() const
		{return m_nHashTableSize;}

	void PrepeareHashTable(DWORD nCount);

	struct CAssoc
	{
		CAssoc(DWORD nHash, CAssoc*& p):nHashValue(nHash), pNext(p){p=this;}
		CAssoc* pNext;
		DWORD nHashValue;
	};
	static DWORD m_lotable[256];
protected:
	CAssoc** m_pHashTable;
	DWORD m_nHashTableSize;

	DWORD m_nCount;
	CAssoc* m_pFreeList;
	struct CPlex* m_pBlocks;

	struct init
	{
		init()
		{
			for(DWORD i=0;i<256;i++)
				m_lotable[i]=(DWORD)CharLower((LPSTR)i);
		}
	};
	static init m_lotableinit;
	friend struct init;

	CNoCaseMapBase():
		m_pHashTable(NULL), m_nCount(0), m_nHashTableSize(0),
		m_pFreeList(NULL), m_pBlocks(NULL){}

	void DeleteAll()
	{
		delete [] m_pHashTable;
		m_pHashTable = NULL;
		m_nHashTableSize = 0;
		m_nCount = 0;
		m_pFreeList = NULL;
		if(m_pBlocks)
		{
			m_pBlocks->FreeDataChain();
			m_pBlocks = NULL;
		}
	}


	CAssoc* NewAssoc(size_t AssocSize)
	{
		if(m_pFreeList == NULL)
		{
			// add another block
			CPlex* newBlock = CPlex::Create(m_pBlocks, 9, AssocSize);
			// chain them into free list
			char* pAssoc = static_cast<char*>(newBlock->data());
			// free in reverse order to make it easier to debug
			pAssoc += (8 * AssocSize);
			for (int i = 8; i >= 0; --i, pAssoc-=AssocSize)
			{
				reinterpret_cast<CAssoc*>(pAssoc)->pNext = m_pFreeList;
				m_pFreeList = reinterpret_cast<CAssoc*>(pAssoc);
			}
		}
		++m_nCount;
		CAssoc* pAssoc = m_pFreeList;
		m_pFreeList = m_pFreeList->pNext;
		return pAssoc;
	}
	void FreeAssoc(CAssoc* pAssoc)
	{
		pAssoc->pNext = m_pFreeList;
		m_pFreeList = pAssoc;
		--m_nCount;
	}

	DWORD CalcHashSize(DWORD nCount)
	{
		DWORD hs = (nCount + (nCount>>1)) | 1;
		if(hs>101)
		{
			for(DWORD i=3; i * i <= hs; i+=2)
			{
				if(hs % i == 0)
				{
					hs+=2;
					i=1;
				}
			}
			return hs;
		}
		else if(hs <= 13)
			return 13;
		else if(hs <= 23)
			return 23;
		else if(hs <= 37)
			return 37;
		else if(hs <= 53)
			return 53;
		else if(hs <= 71)
			return 71;
		else
			return 101;
	}

	static BOOL IsStrEqual(LPCSTR ptr1, LPCSTR ptr2)
	{
		for(DWORD s1=ncm_symb::symbol(ptr1), s2=ncm_symb::symbol(ptr2); ;s1=ncm_symb::symbol(++ptr1), s2=ncm_symb::symbol(++ptr2))
		{
			if(m_lotable[s1] != m_lotable[s2])
				return FALSE;
			if(!s2)
				return !s1;
		}
	}

	static DWORD GetHash(LPCSTR key)
	{
		// Используется FNV-1a алгоритм
		// http://www.isthe.com/chongo/tech/comp/fnv/
		DWORD h=2166136261ul;
		while(DWORD symb=m_lotable[ncm_symb::symbol(key++)])
			h = (h ^ symb) * 16777619ul;
		return h;
	}
};

// Описание стратегий хранения, установки, получения ключей
// Стратегия по умолчанию.
struct default_map
{
	typedef CString key_type;				// Тип хранения ключа в CAssoc (Должен приводится к LPCSTR)
	typedef const CString& keysrc_type;		// Тип передачи ключа при занесении пар (Должен приводится к LPCSTR)
	typedef CString keydst_type;			// Тип приемника ключа при обходе карты. (Должен присваиваться из key_type)
};

// Хранение чисто ссылок на внешние ключи
struct refkey_map
{
	typedef LPCSTR key_type;
	typedef LPCSTR keysrc_type;
	typedef LPCSTR keydst_type;
};

// Хранение копии ключа без CString
struct charcopy_map
{
	struct key
	{
		key(LPCSTR p)
		{
			int len=strlen(p);
			ptr=new char[len+1];
			memcpy(ptr, p, len);
			ptr[len]=0;
		}
		~key(){delete [] ptr;}
		operator LPCSTR(){return ptr;}
		LPSTR ptr;
	};
	typedef key key_type;
	typedef LPCSTR keysrc_type;
	typedef LPCSTR keydst_type;
};


// Собственно, сам шаблон.
template<typename TVal, typename TArg=TVal, typename S=default_map>
class CNoCaseMap : public CNoCaseMapBase
{
public:
	typedef TVal value_type;
	typedef TArg valuearg_type;
	typedef S map_strategy;
	typedef typename map_strategy::key_type key_type;
	typedef typename map_strategy::keysrc_type keysrc_type;
	typedef typename map_strategy::keydst_type keydst_type;

	CNoCaseMap() : CNoCaseMapBase(){}
	~CNoCaseMap(){RemoveAll();}

	BOOL IsExist(LPCSTR key) const
	{
		DWORD nFullHash;
		assoc* pAssoc = GetAssocAt(key, nFullHash);
		return NULL != pAssoc;
	}

	BOOL Lookup(LPCSTR key, value_type& rValue) const
	{
		DWORD nFullHash;
		assoc* pAssoc = GetAssocAt(key, nFullHash);
		if(!pAssoc)
			return FALSE;
		rValue = pAssoc->value;
		return TRUE;
	}
	
	BOOL LookupKey(LPCSTR key, keydst_type& rKey) const
	{
		DWORD nFullHash;
		assoc* pAssoc = GetAssocAt(key, nFullHash);
		if(!pAssoc)
			return FALSE;
		rKey = pAssoc->key;
		return TRUE;
	}

	value_type& operator[](keysrc_type key)
	{
		DWORD nFullHash;
		assoc* pAssoc;
		if(!(pAssoc = GetAssocAt(key, nFullHash)))
		{
			if(!m_pHashTable || m_nCount >= m_nHashTableSize * 3 / 4)
				PrepeareHashTable(m_nCount+1);
			// it doesn't exist, add a new Association
			pAssoc = static_cast<assoc*>(NewAssoc(sizeof(assoc)));
			pAssoc->assoc::assoc(nFullHash, m_pHashTable[nFullHash % m_nHashTableSize], key);
		}
		return pAssoc->value;  // return new reference
	}
	void SetAt(keysrc_type key, valuearg_type newValue){(*this)[key]=newValue;}

	// Позволяет вставить значение и возвращает TRUE, если такой ключ уже существовал,
	// и FALSE, если такого ключа не было
	BOOL InsertExist(keysrc_type key, valuearg_type newValue, BOOL bReplace=FALSE)
	{
		DWORD nFullHash;
		assoc* pAssoc;
		BOOL bRet;
		if(pAssoc = GetAssocAt(key, nFullHash))
		{
			if(!bReplace)
				return TRUE;
			bRet=TRUE;
		}
		else
		{
			bRet=FALSE;
			if(!m_pHashTable || m_nCount >= m_nHashTableSize / 3 * 2)
				PrepeareHashTable(m_nCount+1);
			pAssoc = static_cast<assoc*>(NewAssoc(sizeof(assoc)));
			pAssoc->assoc::assoc(nFullHash, m_pHashTable[nFullHash % m_nHashTableSize], key);
		}
		pAssoc->value=newValue;
		return bRet;
	}

	BOOL RemoveKey(LPCSTR key)
	{
		if(!m_pHashTable || !m_nCount)
			return FALSE;
		DWORD nFullHash = GetHash(key);
		CAssoc** ppAssocPrev = &m_pHashTable[nFullHash % m_nHashTableSize];

		assoc* pAssoc;
		for(pAssoc = static_cast<assoc*>(*ppAssocPrev); pAssoc ; pAssoc = static_cast<assoc*>(pAssoc->pNext))
		{
			if(pAssoc->nHashValue == nFullHash && IsStrEqual(pAssoc->key, key))
			{
				*ppAssocPrev = pAssoc->pNext;
				pAssoc->~assoc();
				FreeAssoc(pAssoc);
				if(m_nCount < m_nHashTableSize/2)
					PrepeareHashTable(m_nCount);
				return TRUE;
			}
			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;
	}
	
	BOOL RemovePair(LPCSTR key, valuearg_type value)
	{
		if(!m_pHashTable || !m_nCount)
			return FALSE;
		DWORD nFullHash = GetHash(key);
		CAssoc** ppAssocPrev = &m_pHashTable[nFullHash % m_nHashTableSize];

		assoc* pAssoc;
		for(pAssoc = static_cast<assoc*>(*ppAssocPrev); pAssoc ; pAssoc = static_cast<assoc*>(pAssoc->pNext))
		{
			if(pAssoc->nHashValue == nFullHash && IsStrEqual(pAssoc->key, key))
			{
				if(pAssoc->value == value)
				{
					*ppAssocPrev = pAssoc->pNext;
					pAssoc->~assoc();
					FreeAssoc(pAssoc);
					if(m_nCount < m_nHashTableSize/2)
						PrepeareHashTable(m_nCount);
					return TRUE;
				}
				return FALSE;
			}
			ppAssocPrev = &pAssoc->pNext;
		}
		return FALSE;
	}
	
	void RemoveAll()
	{
		if(DWORD count=m_nCount)
		{
			assoc **ppAssoc=reinterpret_cast<assoc**>(m_pHashTable), *pAssoc=*ppAssoc;
			while(!pAssoc)
				pAssoc=*++ppAssoc;
			for(;;)
			{
				assoc* pNext=static_cast<assoc*>(pAssoc->pNext);
				pAssoc->~assoc();
				if(!--count)
					break;
				while(!pNext)
					pNext=*++ppAssoc;
				pAssoc=pNext;
			}
		}
		DeleteAll();
	}

	void GetNextAssoc(POSITION& rNextPosition, keydst_type& rKey, value_type& rValue) const
	{
		assoc* pAssocRet = reinterpret_cast<assoc*>(rNextPosition);
		if(pAssocRet == reinterpret_cast<assoc*>(BEFORE_START_POSITION))
		{
			CAssoc** ppAssoc=m_pHashTable;
			while(!(pAssocRet=static_cast<assoc*>(*ppAssoc++)));
		}

		CAssoc* pAssocNext=pAssocRet->pNext;
		if(!pAssocNext)
		{
			for(DWORD idx = pAssocRet->nHashValue % m_nHashTableSize + 1;
				idx < m_nHashTableSize; ++idx)
				if(pAssocNext = m_pHashTable[idx])
					break;
		}

		rNextPosition = reinterpret_cast<POSITION>(pAssocNext);
		// fill in return data
		rKey = pAssocRet->key;
		rValue = pAssocRet->value;
	}
	template<typename Op>
	void ForEachValue(Op op)
	{
		if(DWORD count=m_nCount)
		{
			assoc **ppAssoc=reinterpret_cast<assoc**>(m_pHashTable), *pAssoc=*ppAssoc;
			while(!pAssoc)
				pAssoc=*++ppAssoc;
			for(;;)
			{
				assoc* pNext=static_cast<assoc*>(pAssoc->pNext);
				op(pAssoc->value);
				if(!--count)
					break;
				while(!pNext)
					pNext=*++ppAssoc;
				pAssoc=pNext;
			}
		}
	}
	
	template<typename Op>
	void ForEachPair(Op op)
	{
		if(DWORD count=m_nCount)
		{
			assoc **ppAssoc=reinterpret_cast<assoc**>(m_pHashTable), *pAssoc=*ppAssoc;
			while(!pAssoc)
				pAssoc=*++ppAssoc;
			for(;;)
			{
				assoc* pNext=static_cast<assoc*>(pAssoc->pNext);
				op(pAssoc->key, pAssoc->value);
				if(!--count)
					break;
				while(!pNext)
					pNext=*++ppAssoc;
				pAssoc=pNext;
			}
		}
	}
protected:
	// Association
	struct assoc : CNoCaseMapBase::CAssoc
	{
		assoc(DWORD nHash, CAssoc*& pHashT, keysrc_type k):CAssoc(nHash, pHashT), key(k){}
		key_type key;
		value_type value;
	};
	
	assoc* GetAssocAt(LPCSTR key, DWORD& nFullHash) const
	{
		nFullHash = GetHash(key);
	
		if(!m_pHashTable)
			return NULL;

		// see if it exists
		for(CAssoc* pAssoc = m_pHashTable[nFullHash % m_nHashTableSize]; pAssoc ; pAssoc = pAssoc->pNext)
		{
			if(pAssoc->nHashValue == nFullHash && IsStrEqual(static_cast<assoc*>(pAssoc)->key, key))
				return static_cast<assoc*>(pAssoc);
		}
		return NULL;
	}
};
