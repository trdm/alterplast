// blmap.h
#ifndef BLMAP_H
#define BLMAP_H
struct SLexem;

class CBLMap{
	struct Assoc{	// Структура для хранения данных о парах ключ-номер
		Assoc():pos(NULL),next(NULL),strKey(NULL){}
		Assoc(DWORD h,char* buf,void* p):hash(h),strKey(buf),pos(p),next(NULL){}
		DWORD hash;
		char *strKey;
		void* pos;
		Assoc *next;
	};
	Assoc** m_ppHashTable;	// хэш-таблица. Массив указателей на односвязные списки с ключами
	int m_HashSize;			// размер таблицы
	int m_Count;			// колво значений в мапе
	
	// Получение хэша строки с одновременным заполнением буфера нижнерегистровым значением
	DWORD GetHash(LPCTSTR key,DWORD& hash,char *buf) const
	{
		hash=0;
		while(*key)
			hash=(hash<<1)^(*buf++=m_LowerTable[(BYTE)*key++]);
		*buf=0;
		return hash%m_HashSize;
	}
	DWORD GetHash(LPCTSTR key,int len,DWORD& hash,char *buf) const
	{
		hash=0;
		LPCTSTR end=key+len;
		while(key<end)
			hash=(hash<<1)^(*buf++=m_LowerTable[(BYTE)*key++]);
		*buf=0;
		return hash%m_HashSize;
	}
public:
	CBLMap():m_HashSize(0),m_ppHashTable(NULL),m_Count(0){SetHashSize(7);}
	~CBLMap(){RemoveAll();}
	void InsertKey(LPCTSTR key,void* pos);	// Вставка ключа-строки и значения-номера
	void InsertKey(SLexem& key,void* pos);	// Вставка ключа-строки и значения-номера
	void* GetKey(LPCTSTR key)const;			// Получение номера по ключу
	void* GetKey(SLexem& key)const;			// Получение номера по ключу
	int	 GetHashSize(){return m_HashSize;}
	int  GetCount(){return m_Count;}
	POSITION GetStart()
	{
		return m_Count?(POSITION)-1:NULL;
	}
	CString GetNext(POSITION& pos)
	{
		Assoc* pAssoc=(Assoc*)pos;
		if(pAssoc==(Assoc*)-1)
		{
			for(int i=0;i<m_HashSize;i++)
			{
				if(m_ppHashTable[i])
				{
					pAssoc=m_ppHashTable[i];
					break;
				}
			}
		}
		if(pAssoc->next)
			pos=(POSITION)pAssoc->next;
		else
		{
			pos=NULL;
			for(int i=pAssoc->hash%m_HashSize+1;i<m_HashSize;i++)
			{
				if(m_ppHashTable[i])
				{
					pos=(POSITION)m_ppHashTable[i];
					break;
				}
			}
		}
		return pAssoc->strKey;
	}
	void* GetNext(POSITION& pos,CString& key)
	{
		Assoc* pAssoc=(Assoc*)pos;
		if(pAssoc==(Assoc*)-1)
		{
			for(int i=0;i<m_HashSize;i++)
			{
				if(m_ppHashTable[i])
				{
					pAssoc=m_ppHashTable[i];
					break;
				}
			}
		}
		if(pAssoc->next)
			pos=(POSITION)pAssoc->next;
		else
		{
			pos=NULL;
			for(int i=pAssoc->hash%m_HashSize+1;i<m_HashSize;i++)
			{
				if(m_ppHashTable[i])
				{
					pos=(POSITION)m_ppHashTable[i];
					break;
				}
			}
		}
		key=pAssoc->strKey;
		return pAssoc->pos;
	}
	/*
	void Merge(CBLMap& other)
	{
		if(!m_ppHashTable)
		{
			if(!other.m_Count)
				return;
			SetHashSize(other.m_HashSize);
		}
		Assoc* pAssoc;
		for(int i=0;i<other.m_HashSize;i++)
		{
			pAssoc=other.m_ppHashTable[i];
			while(pAssoc)
			{
				Assoc* pFound=m_ppHashTable[pAssoc->hash%m_HashSize];
				while(pFound)
				{
					if(!lstrcmp(pFound->strKey,pAssoc->strKey))
					{
						pFound->pos=pAssoc->pos;
						break;
					}
				}
				if(!pFound)
				{
					char* buf=new char[lstrlen(pAssoc->strKey)+1];
					lstrcpy(buf,pAssoc->strKey);
					pFound=new Assoc(pAssoc->hash,buf,pAssoc->pos);
					pFound->next=m_ppHashTable[pAssoc->hash%m_HashSize];
					m_ppHashTable[pAssoc->hash%m_HashSize]=pFound;
					m_Count++;
				}
				pAssoc=pAssoc->next;
			}
		}
		Optimize();
	}
	void MergeAndRemove(CBLMap& other)
	{
		if(!m_ppHashTable)
		{
			if(!other.m_Count)
				return;
			SetHashSize(other.m_HashSize);
		}
		Assoc* pAssoc;
		for(int i=0;i<other.m_HashSize;i++)
		{
			pAssoc=other.m_ppHashTable[i];
			while(pAssoc)
			{
				Assoc* pFound=m_ppHashTable[pAssoc->hash%m_HashSize];
				while(pFound)
				{
					if(!lstrcmp(pFound->strKey,pAssoc->strKey))
					{
						pFound->pos=pAssoc->pos;
						delete pAssoc;
						break;
					}
				}
				if(!pFound)
				{
					pAssoc->next=m_ppHashTable[pAssoc->hash%m_HashSize];
					m_ppHashTable[pAssoc->hash%m_HashSize]=pAssoc;
					m_Count++;
				}
				pAssoc=pAssoc->next;
			}
		}
		delete [] other.m_ppHashTable;
		other.m_ppHashTable=NULL;
		other.m_Count=0;
		other.m_HashSize=0;
		Optimize();
	}
	*/

	// Устанавливает размер хэш-таблицы. Если есть старая таблица, она переформируется
	void SetHashSize(int size);
	void RemoveAll();
	void Optimize();
	static char m_LowerTable[257]; // Массив для быстрого перевода строк в нижний регистр
};

#endif