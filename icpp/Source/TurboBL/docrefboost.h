// docrefboost.h
#ifndef DOCREFBOOST_H
#define DOCREFBOOST_H
#include "../blmap.h"
extern HANDLE hProcess;

class CFastFindProp
{
public:
	typedef int(CFastFindProp::*PF)(const char*);

	DWORD vTable[64];
	CBLMap* prop_map;
	CBLMap* meth_map;

	CFastFindProp(CBLContext* pCont, BOOL bBoostProp, BOOL bBoostMeth)
	{
		DWORD *pVtable=*(DWORD**)pCont;
		memcpy(vTable,pVtable,256);
		if (bBoostProp)
		{
			prop_map = new CBLMap;
			prop_map->MakePropMapFromContext(pCont);
			PF pFunc=&CFastFindProp::FindProp;
			vTable[19]=*(DWORD*)&pFunc;
		}
		if (bBoostMeth)
		{
			meth_map = new CBLMap;
			meth_map->MakeMethMapFromContext(pCont);
			PF pFunc=&CFastFindProp::FindMeth;
			vTable[26]=*(DWORD*)&pFunc;
		}
		DWORD old;
		VirtualProtect(vTable, sizeof(vTable), PAGE_EXECUTE_READWRITE, &old);
	}

	int FindProp(const char* name)
	{
		return (*(CFastFindProp**)this)->prop_map->GetKey(name);
	}

	int FindMeth(const char* name)
	{
		return (*(CFastFindProp**)this)->meth_map->GetKey(name);
	}

	void MakeFast(CBLContext* pCont)
	{
		*(CFastFindProp**)pCont=this;
	}

	static void SetHook(void* pObj, void* pAdr, ptrdiff_t nOffset)
	{
		DWORD *pVtable=*(DWORD**)pObj,old;
		pVtable+=nOffset;
		VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
		*pVtable=*(DWORD*)pAdr;
		FlushInstructionCache(hProcess, *(DWORD**)pObj, 256);
	}

	static void SetHookOnProp(void* pObj,void* pAdr)
	{
		SetHook(pObj, pAdr, 19);
	}

	static void SetHookOnMeth(void* pObj,void* pAdr)
	{
		SetHook(pObj, pAdr, 26);
	}
};

class CManyPropBoost
{
	struct Assoc
	{
		int key;
		CFastFindProp *fastfind;
		Assoc *next;
	};
	Assoc** m_ppHashTable;
	int m_HashSize;
	CBLMap* m_AllPropMap;
	CBLMap* m_AllMethMap;
	BOOL m_bBoostProp;
	BOOL m_bBoostMeth;
public:
	CManyPropBoost(BOOL bBoostProp, BOOL bBoostMeth, int size, bool optimize=true)
	{
		m_bBoostProp = bBoostProp;
		m_bBoostMeth = bBoostMeth;
		m_AllPropMap=NULL;
		m_AllMethMap=NULL;
		m_HashSize=size;
		if(optimize)
		{
			m_HashSize+=m_HashSize/2;
			if(m_HashSize%2==0)
				m_HashSize++;
			for(int i=3;i*i<=m_HashSize;i+=2)
			{
				if(m_HashSize % i==0)
				{
					m_HashSize+=2;
					i=1;
				}
			}
		}
		m_ppHashTable=new Assoc*[m_HashSize];
		for(int i=0;i<m_HashSize;i++)
			m_ppHashTable[i]=NULL;
	}
	int FindProp(const char* name,CBLContext* pCont=NULL,int key=0,bool MakeFast=true)
	{
		if(!pCont)
			return m_AllPropMap->GetKey(name);
		Assoc* pAssoc=m_ppHashTable[key%m_HashSize];
		while(pAssoc)
		{
			if(pAssoc->key==key)
			{
				if(MakeFast)
					pAssoc->fastfind->MakeFast(pCont);
				return pAssoc->fastfind->prop_map->GetKey(name);
			}
			pAssoc=pAssoc->next;
		}
		pAssoc=new Assoc;
		pAssoc->key=key;
		pAssoc->fastfind=new CFastFindProp(pCont, m_bBoostProp, m_bBoostMeth);
		pAssoc->next=m_ppHashTable[key%m_HashSize];
		m_ppHashTable[key%m_HashSize]=pAssoc;
		if(MakeFast)
			pAssoc->fastfind->MakeFast(pCont);
		return pAssoc->fastfind->prop_map->GetKey(name);
	}
	int FindMeth(const char* name,CBLContext* pCont=NULL,int key=0,bool MakeFast=true)
	{
		if(!pCont)
			return m_AllMethMap->GetKey(name);
		Assoc* pAssoc=m_ppHashTable[key%m_HashSize];
		while(pAssoc)
		{
			if(pAssoc->key==key)
			{
				if(MakeFast)
					pAssoc->fastfind->MakeFast(pCont);
				return pAssoc->fastfind->meth_map->GetKey(name);
			}
			pAssoc=pAssoc->next;
		}
		pAssoc=new Assoc;
		pAssoc->key=key;
		pAssoc->fastfind=new CFastFindProp(pCont, m_bBoostProp, m_bBoostMeth);
		pAssoc->next=m_ppHashTable[key%m_HashSize];
		m_ppHashTable[key%m_HashSize]=pAssoc;
		if(MakeFast)
			pAssoc->fastfind->MakeFast(pCont);
		return pAssoc->fastfind->meth_map->GetKey(name);
	}
	void InitAllProps(CBLContext *pCont)
	{
		m_AllPropMap=new CBLMap;
		m_AllPropMap->MakePropMapFromContext(pCont);
	}
	void InitAllMeths(CBLContext *pCont)
	{
		m_AllMethMap=new CBLMap;
		m_AllMethMap->MakeMethMapFromContext(pCont);
	}
};

class CDocBoost{
	static CManyPropBoost *m_props;
public:
	typedef int(CDocBoost::*PF)(const char*);
	static void Init();
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для новых объектов
		// типа "Документ.КонкретныйВид"
		// и каждый раз для типа "Документ".
		// Для типа "Документ.КонкретныйВид" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		CDocDef* id=((CDocContext*)this)->m_pDocDef;
		if(!id)
		{
			// просто Документ
			CDocDef* pDef=((CDocContext*)this)->CurDocDef();
			if(!pDef)
				return m_props->FindProp(name);
			else
				return m_props->FindProp(name,(CBLContext*)this,(int)pDef,false);
		}
		return m_props->FindProp(name,(CBLContext*)this,(int)id);
	}
};

class CRefBoost{
	static CManyPropBoost *m_props;
public:
	typedef int(CRefBoost::*PF)(const char*);
	static void Init();
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для новых объектов
		// типа "Справочник.КонкретныйВид"
		// и каждый раз для типа "Справочник".
		// Для типа "Справочник.КонкретныйВид" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида справочников,
		// и данная функция уже не вызывается.
		CBLContext* pCont=(CBLContext*)this;
		DWORD id=pCont->GetValueType().m_mdid;
		if(!id)
		{
			// просто Справочник
			CValue val;
			pCont->GetExactValue(val);
			id=val.GetRealTypeID();
			if(!id)
				return m_props->FindProp(name);
			else
				return m_props->FindProp(name,pCont,id,false);
		}
		return m_props->FindProp(name,pCont,id);
	}
};

class CMetadataBoost
{
	static CManyPropBoost* m_props;
public:
	typedef int(CMetadataBoost::*PF)(const char*);
	static void Init(CBLContext* pCont);
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для новых объектов
		// типа "Метаданные.КонкретныйWhat"
		// и каждый раз для типа "Метаданные".
		// Для типа "Метаданные.КонкретныйWhat" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		CMetadataContext* pCont = (CMetadataContext*)this;
		CTaskDef* pDef = pCont->m_pTaskDef;
		int id = pDef ? pDef->WhatIsIt() : 0;
		return m_props->FindProp(name, pCont, id);
	}
	int FindMeth(const char* name)
	{
		// Данная функция вызывается только один раз для новых объектов
		// типа "Метаданные.КонкретныйWhat"
		// и каждый раз для типа "Метаданные".
		// Для типа "Метаданные.КонкретныйWhat" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		CMetadataContext* pCont = (CMetadataContext*)this;
		CTaskDef* pDef = pCont->m_pTaskDef;
		int id = pDef ? pDef->WhatIsIt() : 0;
		return m_props->FindMeth(name, pCont, id);
	}
};

class CRegBoost{
	static CManyPropBoost *m_props;
	static int m_offset;
public:
	typedef int(CRegBoost::*PF)(const char*);
	static void Init();
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для новых объектов
		// типа "Регистр.КонкретныйВид"
		// Для типа "Регистр.КонкретныйВид" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		return m_props->FindProp(name,(CBLContext*)this,*(int*)(((char*)this)+m_offset));
	}
};

class CEnumBoost{
	static CManyPropBoost *m_props;
	static int m_offset;
public:
	typedef int(CEnumBoost::*PF)(const char*);
	static void Init(CBLContext*);
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для объектов
		// типа "Перечисление.КонкретныйВид"
		// Для типа "Перечисление.КонкретныйВид" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		return m_props->FindProp(name,(CBLContext*)this,*(int*)(((char*)this)+m_offset));
	}
};

class CCalcBoost{
	static CManyPropBoost *m_props;
	static int m_offset;
public:
	typedef int(CCalcBoost::*PF)(const char*);
	static void Init();
	int FindProp(const char* name)
	{
		// Данная функция вызывается только один раз для объектов
		// типа "ЖурналРасчетов.КонкретныйВид"
		// Для типа "ЖурналРасчетов.КонкретныйВид" происходит подмена
		// указателя vTable на псевдо-таблицу для этого вида документов,
		// и данная функция уже не вызывается.
		return m_props->FindProp(name,(CBLContext*)this,*(int*)(((char*)this)+m_offset));
	}
};

class CVTBoost
{
	struct SInfo{
		int AutoDestroy;
		CBLMap* propMap;
		SInfo(CBLContext* pCont)
		{
			propMap=new CBLMap;
			propMap->MakePropMapFromContext(pCont);
			AutoDestroy=pCont->m_FlagAutoDestroy;
			pCont->m_FlagAutoDestroy=(int)this;
			*(DWORD**)pCont=Vtable;
		}
		~SInfo(){delete propMap;}
	};
public:
	typedef int(CVTBoost::*PF)(const char*);
	typedef void(CVTBoost::*PD)();
	static DWORD Vtable[64];
	static PD pRealDecr;

	void DecrRef()
	{
		CBLContext* pCont=(CBLContext*)this;
		if(pCont->m_RefCount==1)
		{
			SInfo* pInfo=(SInfo*)pCont->m_FlagAutoDestroy;
			pCont->m_FlagAutoDestroy=pInfo->AutoDestroy;
			delete pInfo;
		}
		(this->*pRealDecr)();
	}
	int FindPropFirst(const char* name)
	{
		SInfo* pInfo=new SInfo((CBLContext*)this);
		return pInfo->propMap->GetKey(name);
	}
	void FillPropMap(CValueTable* pVT, DWORD dwCols, CBLMap* propMap)
	{
		propMap->RemoveAll();
		propMap->SetHashSize((dwCols * 3 / 2) | 1);
		propMap->InsertKey("НомерСтроки", 0);
		propMap->InsertKey("LineNum", 0);
		for(DWORD i=0; i<dwCols; )
		{
			CVTColumn* pCol=pVT->GetColumn(i);
			propMap->InsertKey(pCol->GetCode(), ++i);
		}
	}
	int FindProp(const char* name)
	{
		CBLContext* pCont=(CBLContext*)this;
		CValueTable* pVT=static_cast<CValueTableContextData*>(pCont->GetInternalData())->GetValueTable();
		DWORD dwCols=pVT->GetColumnCount();
		SInfo* pInfo=(SInfo*)pCont->m_FlagAutoDestroy;
		// Ищем в старой карте
		int ret=pInfo->propMap->GetKey(name);
		if(!ret)	// LineNum, НомерСтроки
			return 0;
		if(ret < 0 || ret > dwCols)
		{
			// Не нашли или нашли заведомо старое. Пересоздадим карту, и поищем еще раз
			FillPropMap(pVT, dwCols, pInfo->propMap);
			return pInfo->propMap->GetKey(name);
		}
		// Нашли номер какой-то колонки, проверим, что по этому номеру действительно нужная колонка
		const char* ptr1=pVT->GetColumn(ret-1)->GetCode();
		const char* ptr2=name;
		while(*ptr1 && CBLMap::m_LowerTable[(BYTE)*ptr1]==CBLMap::m_LowerTable[(BYTE)*ptr2])
			ptr1++, ptr2++;
		if(*ptr1==*ptr2)
			return ret;
		// Упс. По этому номеру другая колонка. Пересоздадим карту
		FillPropMap(pVT, dwCols, pInfo->propMap);
		return pInfo->propMap->GetKey(name);
	}
	static void Init();
};

class CQueryBoost
{
	struct SInfo{
		int AutoDestroy;
		CBLMap* propMap;
		SInfo(CBLContext* pCont)
		{
			propMap=new CBLMap;
			propMap->MakePropMapFromContext(pCont);
			AutoDestroy=pCont->m_FlagAutoDestroy;
			pCont->m_FlagAutoDestroy=(int)this;
			*(DWORD**)pCont=Vtable;
		}
		~SInfo(){delete propMap;}
	};
public:
	typedef int(CQueryBoost::*PF)(const char*);
	typedef void(CQueryBoost::*PD)();
	static DWORD Vtable[64];
	static PD pRealDecr;

	void DecrRef()
	{
		CBLContext* pCont=(CBLContext*)this;
		if(pCont->m_RefCount==1)
		{
			SInfo* pInfo=(SInfo*)pCont->m_FlagAutoDestroy;
			pCont->m_FlagAutoDestroy=pInfo->AutoDestroy;
			delete pInfo;
		}
		(this->*pRealDecr)();
	}
	int FindPropFirst(const char* name)
	{
		SInfo* pInfo=new SInfo((CBLContext*)this);
		return pInfo->propMap->GetKey(name);
	}
	int FindProp(const char* name)
	{
		CBLContext* pCont=(CBLContext*)this;
		SInfo* pInfo=(SInfo*)pCont->m_FlagAutoDestroy;
		int ret=pInfo->propMap->GetKey(name);
		if(ret==-1)
		{
			pInfo->propMap->MakePropMapFromContext(pCont);
			return pInfo->propMap->GetKey(name);
		}
		const char* ptr1=pCont->GetPropName(ret,1);
		const char* ptr2=name;
		if(ptr1)
		{
			while(*ptr1 && CBLMap::m_LowerTable[(BYTE)*ptr1]==CBLMap::m_LowerTable[(BYTE)*ptr2])
				ptr1++,ptr2++;
			if(*ptr1==*ptr2)
				return ret;
			ptr1=pCont->GetPropName(ret,0);
			ptr2=name;
			while(*ptr1 && CBLMap::m_LowerTable[(BYTE)*ptr1]==CBLMap::m_LowerTable[(BYTE)*ptr2])
				ptr1++,ptr2++;
			if(*ptr1==*ptr2)
				return ret;
		}
		pInfo->propMap->MakePropMapFromContext(pCont);
		return pInfo->propMap->GetKey(name);
	}
	static void Init();
};
#endif