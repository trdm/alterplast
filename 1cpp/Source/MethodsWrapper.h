// Определение класса-шаблона CDllMethodWrapper - шаблон для использования с любым типом
// для перехвата любых функций (как обычных, так и виртуальных) в dll от 1С
//
// Автор Артур Аюханов aka artbear
//
//	Все методы этого класса являются inline. Т.е. потерь в скорости никаких
//	а использовать охренительно удобно !!
//
/*
	Пример использования
	1. Определяем тип метода в длл-ке
		typedef int(CBLModule::*PF_FF)(char const * )const;
	2. Создаем новую переменную 
		CDllMethodWrapper<PF_FF> wrapperFindFunc;
	
	3. Выполняем перехват
		HINSTANCE hBl=GetModuleHandle("blang.dll");

		wrapperFindFunc.DoWrap(hBl,"?FindFunc@CBLModule@@QBEHPBD@Z", (PF_FF)WrapFindFunc);
		1 параметр - это ссылка на длл от 1С
		2 - полное наименование функции
		3 - ссылка на свой метод, оболочку для перехваченной функции
		возможен 4 параметр - если true, значит, это виртуальная функция
		 если false или пропущен, значит, это обычная, невиртуальная функция

	4. Создаем перехваченную функцию с кодом

	int  CWrapModule::WrapFindFunc(char const * fName)const
	{
		int ret = 0;

		// восстановить исходный адрес функции
		//wrapperFindFunc.RestoreOrigAddress(); // лучше след.строкой
		CDllMethodWrapperGuard<PF_FF> wrapper(wrapperFindFunc);


		// здесь свои действия
		// вызываю исходную функцию
		//ret = (((CBLModule*)this)->*wrapperFindFunc.GetOrigMethod())(fName);
		ret = (((CBLModule*)this)->*wrapper.GetOrigMethod())(fName);
		// или можно как обычно
		ret = ((CBLModule*)this)->FindFunc(fName);

		// здесь свои действия

		// восстановить адрес перехватчика
		//wrapperFindFunc.RestoreWrapAddress(); // при использовании стекового объекта вызов не нужен

		return ret; // восстановление адреса перехватчика произойдет автоматически !!
	}
*/

#ifndef METHODSWRAPPER_H
#define METHODSWRAPPER_H

extern HANDLE hProcess;
typedef unsigned __int64 ui64;

template <class TFunctionType>
class CDllMethodWrapper
{
private:
	ui64* m_pFunc;
	ui64 m_SwapBuf;
	DWORD m_dwProtection;
	
	union PtrC
	{
		void *pV;
		TFunctionType pMethod;
	};
	
	// устанавливается в блоке между "RestoreOrigAddress" и "RestoreWrapAddress"
	BOOL m_bIsActive;
public:
	CDllMethodWrapper() : m_pFunc(NULL), m_bIsActive(TRUE){};
	virtual ~CDllMethodWrapper(){};
	
	void Swap(void)
	{
		if (!IsWrapped())
			RuntimeError("CDllMethodWrapper::Swap failed: m_pFunc == NULL");
			
		VirtualProtect(m_pFunc, 8, PAGE_EXECUTE_READWRITE, &m_dwProtection);

		ui64 buf = *m_pFunc;
		*m_pFunc = m_SwapBuf;
		m_SwapBuf = buf;
		
		m_bIsActive = !m_bIsActive;

		VirtualProtect(m_pFunc, 8, m_dwProtection, &m_dwProtection);
		FlushInstructionCache(hProcess, m_pFunc, 8);
	}
	
	void RestoreOrigAddress(void)
	{
		if (!m_bIsActive)
			Swap();
		//	else
		//		Msg("double orig %x", m_pFunc);
	}
	
	void RestoreWrapAddress(void)
	{
		if (m_bIsActive)
			Swap();
		//	else
		//		Msg("double wrap %x", m_pFunc);
	}
	
	TFunctionType GetOrigMethod(void)
	{
		if (!IsWrapped())
			RuntimeError("CDllMethodWrapper::GetOrigMethod failed: m_pFunc == NULL");

		register PtrC conv;
		
		conv.pV = m_pFunc;
		return conv.pMethod;
	}
	
	// FALSE: перехват не выполнялся; TRUE: перехват выполнен
	BOOL IsWrapped(void)
	{
		return (m_pFunc != NULL);
	}
	
	// FALSE: установлен новый обработчик перехвата
	// TRUE: установлен оригинальный обработчик
	// используется как флаг запуска оригинального обработчика
	BOOL IsActive(void)
	{
		return m_bIsActive;
	}
	
	void DoWrap(HINSTANCE hDllInstance, LPCSTR strMethodNameInDll, TFunctionType pNewFunc)
	{
		if (!IsWrapped())
		{
			m_pFunc = (ui64*)GetProcAddress(hDllInstance, strMethodNameInDll);
			if (m_pFunc)
			{
				register PtrC conv;
				conv.pMethod = pNewFunc;
				
				// сохранение оригинального контента
				m_SwapBuf = (static_cast<ui64>((DWORD)conv.pV - reinterpret_cast<DWORD>(m_pFunc)-5))<<8 | 0xE9;
				Swap();
			}
			else
				RuntimeError("DoWrap failed: %s", strMethodNameInDll);
		}
	};
	
};

// данный объект используется в качестве стекового объекта
template <class TFunctionType>
class CDllMethodWrapperGuard
{

	private:
		CDllMethodWrapper<TFunctionType>& m_DllMethodWrapper;
	
	public:
		CDllMethodWrapperGuard(CDllMethodWrapper<TFunctionType>& aDllMethodWrapper ) :
			m_DllMethodWrapper(aDllMethodWrapper)
		{
			m_DllMethodWrapper.RestoreOrigAddress();
		};

		~CDllMethodWrapperGuard()
		{
			m_DllMethodWrapper.RestoreWrapAddress();
		};

		TFunctionType GetOrigMethod(void)
		{
			return m_DllMethodWrapper.GetOrigMethod();
		}
		
};

class CTrapSwap
{
public:
	CTrapSwap() : m_pFunc(NULL), m_SwapBuf(0){}

	template<typename T>
	void SetTrapByName(HINSTANCE hDll, LPCSTR strFuncName, T newFunc)
	{
		DWORD* pFunc=reinterpret_cast<DWORD*>(GetProcAddress(hDll, strFuncName));
		if (!pFunc)
			RuntimeError("Ошибка получения указателя на функцию %s", strFuncName);

		DWORD pNewFunc=*reinterpret_cast<DWORD*>(&newFunc);
		_SetTrap(pFunc, pNewFunc);
	}
	
	template<typename T>
	void SetTrapByName(LPCSTR strDll, LPCSTR strFuncName, T newFunc)
	{
		HINSTANCE hDll = GetModuleHandle(strDll);
		if (!hDll)
			RuntimeError("Ошибка получения дескриптора модуля %s", strDll);

		SetTrapByName(hDll, strFuncName, newFunc);
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
		VirtualProtect(m_pFunc, 8, PAGE_EXECUTE_READWRITE, &m_dwProtection);

		ui64 t = *m_pFunc;
		*m_pFunc = m_SwapBuf;
		m_SwapBuf = t;

		VirtualProtect(m_pFunc, 8, m_dwProtection, &m_dwProtection);
		FlushInstructionCache(hProcess, m_pFunc, 8);
	}

	const ui64* GetOrigMethod() const { return m_pFunc; }
	
protected:

	ui64* m_pFunc;

	ui64 m_SwapBuf;
	DWORD m_dwProtection;

	void _SetTrap(DWORD* pFunc, DWORD pNewFunc)
	{
		m_pFunc=reinterpret_cast<ui64*>(pFunc);
		// Формируем джамп на адрес новой функции
		m_SwapBuf = (static_cast<ui64>(pNewFunc - reinterpret_cast<DWORD>(pFunc)-5)<<8) | 0xE9;
		// И вписываем в начало функции сформированный джамп
		Swap();
	}
};

struct trapswapper
{
	CTrapSwap& trap;
	trapswapper(CTrapSwap& t):trap(t){trap.Swap();}
	~trapswapper(){trap.Swap();}
};

#endif
