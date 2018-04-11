// blmap.h
#ifndef BLMAP_H
#define BLMAP_H

class CBLMap{
	struct Assoc // Структура для хранения данных о парах ключ-номер
	{
		Assoc():pos(NULL),next(NULL),strKey(NULL){}
		Assoc(DWORD h,char* buf,int p):hash(h),strKey(buf),pos(p),next(NULL){}
		DWORD hash;
		char *strKey;
		int pos;
		Assoc *next;
	};
	Assoc** m_ppHashTable;	// хэш-таблица. Массив указателей на односвязные списки с ключами
	int m_HashSize;			// размер таблицы
	int m_Count;			// колво значений в мапе
public:
	CBLMap():m_HashSize(0),m_ppHashTable(NULL),m_Count(0){SetHashSize(7);}
	~CBLMap(){RemoveAll();}
	void InsertKey(LPCTSTR key,int pos);	// Вставка ключа-строки и значения-номера
	int  GetKey(LPCTSTR key)const;			// Получение номера по ключу
	int	 GetHashSize(){return m_HashSize;}
	int  GetCount(){return m_Count;}

	// Устанавливает размер хэш-таблицы. Если есть старая таблица, она переформируется
	void SetHashSize(int size);
	void RemoveAll();
	void Optimize();
	void MakePropMapFromContext(CBLContext* pCont)
	{
		RemoveAll();
		int nProps=pCont->GetNProps();
		SetHashSize(nProps);
		for(int i=0;i<nProps;i++)
		{
			InsertKey(pCont->GetPropName(i,0),i);
			InsertKey(pCont->GetPropName(i,1),i);
		}
		Optimize();
	}
	void MakeMethMapFromContext(CBLContext* pCont)
	{
		RemoveAll();
		int nMeth=pCont->GetNMethods();
		SetHashSize(nMeth);
		for(int i=0;i<nMeth;i++)
		{
			InsertKey(pCont->GetMethodName(i,0),i);
			InsertKey(pCont->GetMethodName(i,1),i);
		}
		Optimize();
	}
	static char m_LowerTable[257]; // Массив для быстрого перевода строк в нижний регистр
};

#endif