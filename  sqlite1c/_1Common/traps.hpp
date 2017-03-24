// traps.hpp
/*
(c) Александр Орефков telepat@inbox.ru
Набор классов для перехвата функций.
*/
#pragma once

// Функция для реинтерпрет каста адресов методов классов
// Использование:
// Каст указателя на метод в простой тип
// DWORD* pFunc = mf_cast<DWORD*>(&Class::Method);
// Каст простого типа в указатель на метод
// int(Class::*pMemFunc)(int, int) = mf_cast<int(Class::*)(int, int)>(ptr);
template<typename T, typename K>
__forceinline static T mf_cast(const K& ptr)
{
	return *reinterpret_cast<T*>(&const_cast<K&>(ptr));
}

// Класс для подмены адреса в таблице виртуальных функций
class CTrapVirtual
{
public:
	//CTrapVirtual():	m_pVtable(NULL), m_dwRealAdress(0){}
	//~CTrapVirtual(){UnTrap();}
	
	// Установка перехвата.
	// На вход подаем указатель на объект, виртуальную таблицу которого будем исправлять,
	// номер функции в таблице, и адрес новой функции.
	// ВИРТУАЛЬНАЯ ТАБЛИЦА ОДНА НА КЛАСС. ПЕРЕХВАТ ДЕЙСТВУЕТ НА ВСЕ ОБЪЕКТЫ ЭТОГО КЛАССА.
	template<typename T>
	void SetTrap(void* pObject, int offset, T pFunc)
	{
		_SetTrap(pObject, offset, *reinterpret_cast<DWORD*>(&pFunc));
	}

	template<typename T>
	void GetAddress(T& t){t=*reinterpret_cast<T*>(&m_dwRealAdress);}
	
	// Снятие перехвата.
	void UnTrap()
	{
		if(m_pVtable)
		{
			*m_pVtable=m_dwRealAdress;
			m_pVtable=NULL;
		}
	}

	void _SetTrap(void* pObject, int offset, DWORD dwFunction)
	{
		m_pVtable =*reinterpret_cast<DWORD**>(pObject);
		m_pVtable+=offset;
		DWORD old;
		VirtualProtect(m_pVtable,4,PAGE_EXECUTE_READWRITE,&old);
		m_dwRealAdress=*m_pVtable;
		*m_pVtable=dwFunction;
	}
	DWORD* m_pVtable;
	DWORD  m_dwRealAdress;
};

typedef unsigned __int64 ui64;

// Класс для перехвата функций путем установки переходника в начало функции.
class CTrapSwap
{
public:
	//CTrapSwap():m_pFunc(NULL), m_SwapBuf(0){}
	
	template<typename T>
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, T newFunc)
	{
		DWORD* pFunc=reinterpret_cast<DWORD*>(GetProcAddress(hDll, strFuncName));
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}
	
	template<typename T>
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, T newFunc)
	{
		SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
	}

	template<typename T>
	void SetTrapByAddress(DWORD* pFuncAddress, T newFunc)
	{
		DWORD* pFunc = pFuncAddress;
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	template<typename T, typename M>
	void SetTrapOnImportFunc(T oldFunc, M newFunc)
	{
		BYTE* ptr=*reinterpret_cast<BYTE**>(&oldFunc);
		ptr+=2;
		DWORD* pFunc=**reinterpret_cast<DWORD***>(ptr);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	template<typename T, typename M>
	void SetTrapOnDeclImportFunc(T oldFunc, M newFunc)
	{
		DWORD* pFunc=*reinterpret_cast<DWORD**>(&oldFunc);
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}

	void Swap()
	{
		ui64 t=*m_pFunc;
		*m_pFunc=m_SwapBuf;
		m_SwapBuf=t;
	}
	void _SetTrap(DWORD* pFunc, DWORD pNewFunc)
	{
		m_pFunc=reinterpret_cast<ui64*>(pFunc);
		DWORD old;
		VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &old);
		// Формируем джамп на адрес новой функции
		m_SwapBuf = (static_cast<ui64>(pNewFunc - reinterpret_cast<DWORD>(pFunc)-5)<<8) | 0xE9;
		// И вписываем в начало функции сформированный джамп
		Swap();
		FlushInstructionCache(GetCurrentProcess(), m_pFunc, 8);
	}
	ui64* m_pFunc;
	ui64 m_SwapBuf;
};


struct trapswaper
{
	CTrapSwap& trap;
	trapswaper(CTrapSwap& t):trap(t){trap.Swap();}
	~trapswaper(){trap.Swap();}
};


//Window subclassing
struct WndSubclasser
{
	#pragma pack(push,1)
	struct _Thunk
	{
		DWORD   m_mov;          // mov dword ptr [esp+0x4], pThis (esp+0x4 is hWnd)
		DWORD   m_this;         //
		BYTE    m_jmp;          // jmp WndProc
		DWORD   m_relproc;      // relative jmp
	} thunk;
	#pragma pack(pop)
	HWND m_hWnd;
	WNDPROC m_pfWndProc;

	template<typename T>
	WndSubclasser(T* ptr, LRESULT (CALLBACK *pFunc)(T*,UINT,WPARAM,LPARAM), HWND hWindow)
	{
		thunk.m_mov = 0x042444C7;  //C7 44 24 0C
		thunk.m_this = reinterpret_cast<DWORD>(ptr);
		thunk.m_jmp = 0xE9;
		thunk.m_relproc = (int)pFunc - ((int)&thunk+sizeof(thunk));
		m_hWnd = hWindow;
		m_pfWndProc = (WNDPROC) SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)this);
	}

	template<typename T>
	struct jumper
	{
		static LRESULT CALLBACK WndProc(T* pThis, UINT msg, WPARAM w, LPARAM l)
		{
			return pThis->WndProc(msg, w, l);
		}
	};
	template<typename T>
	WndSubclasser(T* ptr, HWND hWindow)
	{
		thunk.m_mov = 0x042444C7;  //C7 44 24 0C
		thunk.m_this = reinterpret_cast<DWORD>(ptr);
		thunk.m_jmp = 0xE9;
		thunk.m_relproc = (int)&jumper<T>::WndProc - ((int)&thunk+sizeof(thunk));
		m_hWnd = hWindow;
		m_pfWndProc = (WNDPROC) SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)this);
	}

	static LRESULT CALLBACK MyWndProc(WndSubclasser* pThis, UINT nMessage, WPARAM w, LPARAM l)
	{
		LRESULT res = CallWindowProc(pThis->m_pfWndProc, pThis->m_hWnd, nMessage, w, l);
		if(nMessage==WM_NCDESTROY)
			delete pThis;
		return res;
	}

	void Detach()
	{
		if ((LONG) this == GetWindowLong(m_hWnd, GWL_WNDPROC))
		{
			SetWindowLong(m_hWnd, GWL_WNDPROC, (LONG)m_pfWndProc);
			delete this;
		}
		else
		{
			thunk.m_this = reinterpret_cast<DWORD>(this);
			thunk.m_relproc = (int)MyWndProc - ((int)&thunk+sizeof(thunk));
		}
	}
};

/* USING WndSubclasser
struct WS_Client
{
	WS_Client() : pWndSubclassInfo(NULL){}
	~WS_Client()
	{
		if(pWndSubclassInfo)
			pWndSubclassInfo->Detach();
	}
	WndSubclasser* pWndSubclassInfo;
	
	void AttachToWnd(HWND hWnd)
	{
		if(pWndSubclassInfo)
			pWndSubclassInfo->Detach();
		pWndSubclassInfo = new WndSubclasser(this, &WS_Client::MyWndProc, hWnd);
	}

	LRESULT ClassWndProc(UINT msg, WPARAM w, LPARAM l)
	{
		return CallWindowProc(pWndSubclassInfo->m_pfWndProc, pWndSubclassInfo->m_hWnd, msg, w, l);
	}

	static LRESULT CALLBACK MyWndProc(WS_Client* ptr, UINT msg, WPARAM w, LPARAM l)
	{
		return ptr->ClassWndProc(msg, w, l);
	}
};
*/

/*
Перехват msgmap всех окон одного класса.
Для этого возьмем карту основного окна, свою карту, и переставим
в них lpEntries. Также pfnGetBaseMap из карты основного окна
перепишем в свою карту, а в основном окне вместо нее пропишем адрес
функции, возращающей нашу карту.
*/

class CMsgMapTrapBase	// Базовый класс для шаблонных классов
{
protected:
	static void _SetTrap(AFX_MSGMAP* pTrapMap, AFX_MSGMAP* pNewMap, const AFX_MSGMAP* (PASCAL* pfnGetBaseMap)())
	{
		DWORD old;
		VirtualProtect(pTrapMap, sizeof(AFX_MSGMAP), PAGE_EXECUTE_READWRITE, &old);
		VirtualProtect(pNewMap,  sizeof(AFX_MSGMAP), PAGE_EXECUTE_READWRITE, &old);
		AFX_MSGMAP_ENTRY* pTemp = (AFX_MSGMAP_ENTRY*) pTrapMap->lpEntries;
		pTrapMap->lpEntries = pNewMap->lpEntries;
		pNewMap->lpEntries = pTemp;
		pNewMap->pfnGetBaseMap = pTrapMap->pfnGetBaseMap;
		pTrapMap->pfnGetBaseMap = pfnGetBaseMap;
	}
	static AFX_PMSG FindRealHandler(UINT nMessage, AFX_MSGMAP* pMap)
	{
		AFX_MSGMAP_ENTRY* pEntry;
		while(pMap)
		{
			pEntry=(AFX_MSGMAP_ENTRY*)pMap->lpEntries;
			while(pEntry->nMessage)
			{
				if(pEntry->nMessage==nMessage)
					return pEntry->pfn;
				pEntry++;
			}
			pMap=(AFX_MSGMAP*)pMap->pfnGetBaseMap();
		}
		return NULL;
	}
	static AFX_PMSG FindRealHandler(UINT nMessage, UINT nCode, UINT nID, UINT nLastID, AFX_MSGMAP* pMap)
	{
		AFX_MSGMAP_ENTRY* pEntry;
		while(pMap)
		{
			pEntry=(AFX_MSGMAP_ENTRY*)pMap->lpEntries;
			while(pEntry->nMessage)
			{
				if(pEntry->nMessage==nMessage && pEntry->nCode == nCode
					&& pEntry->nID >= nID && pEntry->nLastID <= nLastID)
					return pEntry->pfn;
				pEntry++;
			}
			pMap=(AFX_MSGMAP*)pMap->pfnGetBaseMap();
		}
	}
};

template<typename T>
class CMsgMapTrap : public CMsgMapTrapBase
{
protected:
	struct wnd_get_map : public T
	{
		AFX_MSGMAP* GetMap() {return const_cast<AFX_MSGMAP*>(GetMessageMap());}
		static AFX_MSGMAP* GetTMap(){return const_cast<AFX_MSGMAP*>(&T::messageMap);}
	};
	static const AFX_MSGMAP* PASCAL GetBase() { return wnd_get_map::GetTMap(); }
public:
	static void SetTrap(CWnd* pWnd)
	{
		_SetTrap(static_cast<wnd_get_map*>(pWnd)->GetMap(), wnd_get_map::GetTMap(),
			&CMsgMapTrap<T>::GetBase);
	}
	template<typename K>
	static void FindHandler(UINT nMessage, K& pHandler)
	{
		AFX_PMSG pMsg = FindRealHandler(nMessage, wnd_get_map::GetTMap());
		pHandler = *reinterpret_cast<K*>(&pMsg);
	}
	template<typename K>
	static void FindHandler(UINT nMessage, UINT nCode, UINT nID, UINT nLastID, K& pHandler)
	{
		pHandler = *reinterpret_cast<K*>(FindRealHandler(nMessage, nCode, nID, nLastID, wnd_get_map::GetTMap()));
	}
};



/*
//////////////////////////////////////////////////////////////////
TRAP_HEADER(CSheetView_OnLButtonDblClk, CV7TableDocManager, CSheetDoc, "moxel.dll", "?OnLButtonDblClk@CSheetView@@IAEXIVCPoint@@@Z", void)(UINT, CPoint);
// Доступ к перехватчику и тп
CSheetViewOnLButtonDown::trapper()
CSheetViewOnLButtonDown::mapper()
CSheetViewOnLButtonDown::isTrap()
*/
#define TRAP_HEADER(funcName, ThisType_mapKEY, mapVAL, dllName, decorName, retType)\
struct funcName{\
	typedef CMap<ThisType_mapKEY*, ThisType_mapKEY*, mapVAL*, mapVAL*> trapmap;\
	static CTrapSwap&  trapper(){static CTrapSwap tr; return tr;}\
	static trapmap& mapper(){static trapmap m;return m;}\
	static BOOL& isTrap(){static BOOL f;return f;}\
	static void Wrap(ThisType_mapKEY* pKey, mapVAL* pVal){\
		if(!isTrap()){\
			trapper().SetTrapByName(dllName, decorName, &ThisType_mapKEY::##funcName);\
			isTrap() = TRUE;}\
		mapper()[pKey] = pVal;}\
	static void UnWrap(ThisType_mapKEY* pKey){mapper().RemoveKey(pKey);};\
};\
friend struct funcName;\
retType funcName
