#ifndef WRAPPER_FOR_SYNTAXCHECK_H
#define WRAPPER_FOR_SYNTAXCHECK_H

//extern HANDLE hProcess;

typedef unsigned __int64 ui64;

// Класс для перехвата функций путем установки переходника в начало функции.
// без четкой типизации
// Автор - orefkov
//
// artbear - добавлен вывод диагностических сообщений в случае ошибок
//
class CTrapSwap
{
public:
	CTrapSwap():m_pFunc(NULL), m_SwapBuf(0){}

	template<typename T>
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, T newFunc)
	{
		if(!hDll)
		{
			MessageBox("Неверный указатель на DLL - %s ", strFuncName);
			return;
		}
		DWORD* pFunc=reinterpret_cast<DWORD*>(GetProcAddress(hDll, strFuncName));
		if(!pFunc)
		{
			MessageBox("Не получен указатель на функцию - %s ", strFuncName);
			return;
		}
		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);

		_SetTrap(pFunc, pNewFunc);
	}
	
	template<typename T>
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, T newFunc)
	{
		SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
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
	void SetTrapOnFunc(T oldFunc, M newFunc)
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
	ui64* m_pFunc; // TODO нужен ли здесь public-доступ ?
protected:
	ui64 m_SwapBuf;
	void _SetTrap(DWORD* pFunc, DWORD pNewFunc)
	{
		if(!pFunc)
		{
			MessageBox("CTrapSwap::_SetTrap - Не получен указатель на функцию pFunc - %s ");
			return;
		}
		if(!pNewFunc)
		{
			MessageBox("CTrapSwap::_SetTrap - Не получен указатель на функцию pNewFunc - %s ");
			return;
		}
		m_pFunc=reinterpret_cast<ui64*>(pFunc);
		DWORD old;
		VirtualProtect(pFunc, 8, PAGE_EXECUTE_READWRITE, &old);
		// Формируем джамп на адрес новой функции
		m_SwapBuf = (static_cast<ui64>(pNewFunc - reinterpret_cast<DWORD>(pFunc)-5)<<8) | 0xE9;
		// И вписываем в начало функции сформированный джамп
		Swap();
		FlushInstructionCache(GetCurrentProcess(), m_pFunc, 8);
	}
};

struct trapswaper
{
	CTrapSwap& trap;
	trapswaper(CTrapSwap& t):trap(t){trap.Swap();}
	~trapswaper(){trap.Swap();}
};

// Класс для перехвата функций путем установки переходника в начало функции
// c четкой типизацией
// Автор - artbear (c) 2006
//
// Основан на классе CTrapSwap (с) orefkov
//	Фактически это объектная надстройка с типизацией над классом CTrapSwap
//
template<typename TypeFunc_Wrap>
class CTrap
{
private:
	CTrapSwap trap;
public:
	
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, TypeFunc_Wrap newFunc)
	{
		trap.SetTrapByName(hDll, strFuncName, newFunc);
	}
	
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, TypeFunc_Wrap newFunc)
	{
		trap.SetTrapByName(GetModuleHandle(strDll), strFuncName, newFunc);
	}
	
	template<typename M>
	void SetTrapOnImportFunc(TypeFunc_Wrap oldFunc, M newFunc)
	{
		trap.SetTrapOnImportFunc(oldFunc, newFunc);
	}
	
	template<typename M>
	void SetTrapOnFunc(TypeFunc_Wrap oldFunc, M newFunc)
	{
		trap.SetTrapOnFunc(oldFunc, newFunc);
	}
	
	void Swap()
	{
		trap.Swap();
	}
};

// сторож перехвата - используется в функциях в качестве стекового объекта
// при любых возвратах или исключениях перехват будет восстановлен
//
template<typename TypeFunc_Wrap>
class CTrapGuard
{
private:
	CTrap<TypeFunc_Wrap>& trap;
public:
	CTrapGuard(CTrap<TypeFunc_Wrap>& t): trap(t) {trap.Swap();}
	~CTrapGuard() {trap.Swap();}
};

class CWrapperForSyntaxCheck
{
public:
	typedef BOOL(CWnd::*PPRETRANSLATE)(MSG*);
	typedef int	(CBLModule7::*TFunc_CBLModule7_Compile)();
	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_WriteMessage)(char const *,enum MessageMarker,int);

	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_ClearAll)(void);
	typedef void (CMainMessageHandler::*TFunc_CMainMessageHandler_Hide)(void);
	
	static void SetHooks();
	static void DestroyHooks();
	
	static int CallOrigCBLModuleCompile(CBLModule7* pMod);

private:
	static CTrap<TFunc_CBLModule7_Compile> trCompile;
	int	Wrap_CBLModule_Compile();
 	static bool bInsideCompileHandler;

	// перехват вывода в окно сообщений
	static CTrap<TFunc_CMainMessageHandler_WriteMessage> TrWrap_CMainMessageHandler_WriteMessage;
	void Wrap_CMainMessageHandler_WriteMessage(char const * szString, enum MessageMarker marker, int i1);

	// для перехвата нажатия кнопки "Синтакс-проверки" на панели "Текстовый редактор"
	static HHOOK WH_CallWndProc_HookHandle;
	static LRESULT __stdcall WH_CallWndProc_Hook(int code, WPARAM wParam, LPARAM lParam);

	// для отработки клавиши F4 - переход к очередной ошибке
	//		TODO если ошибки нет, проверять модуль с нуля
	//
	// для перехвата вызова "Синтакс-проверки" через макрос Опенконфа
	//
	BOOL PreTranslateMessage(MSG* pMsg);
	static PPRETRANSLATE m_pPreTranslate;
};

#endif