//______________________________________________________________________________
//
// Быстрый и удобный класс-указатель на CBLContext
//
// ВАЖНО: класс является владельцем контекста
//		поэтому при удалениии объекта класса для контекста выполняется операция DecrRef
//
// поэтому класс удобно использовать для контекстов, создаваемых непосредственно в коде ВК
//
//	ВАЖНО: класс нельзя использовать для хранения групповых контекстов, 
//		т.к. не отслеживается закрытие формы пользователем и удаление соответствующих контекстов
//
//	Существует подобный вполне безопасный класс - умный указатель на контекст CSafeContextPtr (SafeContext.h)
//		Это очень безопасный класс, отслеживающий уничтожение любых контекстов, 
//			в т.ч. и групповых контекстов
//		Выполняется автоподсчет ссылок и автоматическое разыменование 
// 
//	Автор DmitrO 
// Класс поправлен и упрощен artbear, 2006

// kms:
// для групповых контекстов скобки IncrRef /DecrRef не вызываются - это небезопасно
//______________________________________________________________________________
//

//#pragma once
#ifndef __BLPTR__INCLUDED
#define __BLPTR__INCLUDED

template <class T>
class CBLPtr
{
public:
	typedef T _PtrClass;
	CBLPtr() : p(NULL), m_bIsGroupContext(FALSE)
	{	}

	CBLPtr(T* lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp);
	}

	template<class U>
	CBLPtr(const CBLPtr<U>& lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign((U*)lp); // чтобы можно было выполнять приведение от CBLPtr<CBLContext> к CBLPtr<CComponentClass>
	}

	// создать через СRuntimeClass::CreateObject
	CBLPtr(CRuntimeClass& RuntimeClassRef) : p(NULL), m_bIsGroupContext(FALSE)
	{
		T* newptr = static_cast<T*>(RuntimeClassRef.CreateObject());
		_Assign(newptr);
		newptr->DecrRef();
	}

	// если передать пустую строку, будет создан контекст того типа, что прописан в шаблоне
	//	чтобы не писать две строки типа
	//		CBLPtr<CExecuteModule> pContExecModule();
	//		pContExecModule.Create();
	//	а просто
	//		CBLPtr<CExecuteModule> pContExecModule("");
	//
    CBLPtr(const CString& szTypeName) : p(NULL), m_bIsGroupContext(FALSE)
    {
		if (szTypeName.IsEmpty())
			Create();
		else
			Create(szTypeName);
    };
	~CBLPtr()
	{
		Release();
	}
	operator T*() const
	{
		return (T*)p;
	}

	// TODO почему-то при использовании CBLPTr<CBLFont> и использовании этого оператора
	// если этот оператор не определять в h-файле, а в cpp-файле, будет ошибка линковки
	// типа не найден метод CBLContext& CSafeContextPtr<CBLFont>::operator*() const
	// а почему, разобраться так и не удалось :(
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		if (p)
			return *p;

		// TODO в h-файле не удается воспользоваться след. строкой
		// если подключать заголовочные файлы, выдается ошибка компиляции
		//
		//::RuntimeError("CSafeContextPtr:: Попытка обращения к несуществующему контексту");
		_ErrorNULLObject(); // выбрасывается исключение

		return *p; // для исключения предупреждения компилятора

	}

/* artbear слишком опасная операция - любой клиент класса может изменить значение внутреннего указателя
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
*/    
	T* operator->() const
	{
		ATLASSERT(p!=NULL);
		if (p)
			return p;

		_ErrorNULLObject(); // выбрасывается исключение

		return p; // для исключения предупреждения компилятора
	}
    
	CBLPtr& operator=(T* lp)
	{
		if (p != lp)
			Assign(lp);

	    return *this;
	}

	template<class U>
	CBLPtr& operator=(const CBLPtr<U>& lp)
	{
		if (&lp == this)
			return *this;

		return operator=(lp.p);
	}

	bool operator!() const
	{
		return !operator bool();
	}

	operator bool() const
	{
		return (p != NULL);
	}

	bool operator==(T* pT) const
	{
		return p == pT;
	}

	bool operator!=(T* pT) const
	{
		return !operator ==(pT);
	}

    void Create()
    {
		Release();

		// во избежание висящей ссылки не делаю Assign
        p = new T();
		_CheckContext();
    };

    void Create(const CString& szTypeName)
    { // TODO нет проверки соответствия переданного имени и имени класса-шаблона

		Release();

		// имхо try не нужен artbear - а базы без справочников считаем багом :)
		// во избежание висящей ссылки не делаю Assign
		p = (T*)CBLContext::CreateInstance(szTypeName); //no cast is special: this only for CBLContext instance
		_CheckContext();
    };

	// является групповым контекстом
	inline BOOL IsGroupContext() const
	{
		return m_bIsGroupContext;
	};

	void Empty(void) { Release(); p = 0; }

protected: // artbear
	//T* ptr;
	T* p;
	BOOL m_bIsGroupContext;

	void _CheckContext()
	{
		if (p)
			m_bIsGroupContext = p->IsKindOf( RUNTIME_CLASS( CGroupContext ));

		else
			m_bIsGroupContext = FALSE;
	};

	void _IncrRef()
	{
		if (p && !IsGroupContext())
			p->IncrRef();
	}

	void _DecrRef()
	{
		if (p && !IsGroupContext())
			p->DecrRef();
	}

	// назначение ссылки
	void _Assign(T * pCont)
	{
		p = pCont;

		_CheckContext();
		_IncrRef();
	};

	void Assign(T * pCont)
	{
		Release();
		_Assign(pCont);
	};

	// освобождение ссылки
	void Release()
	{
		_DecrRef();
		//p = 0;
	};

	void _ErrorNULLObject(LPCSTR szErrorString = NULL) const; // выбрасывается исключение с возможным сообщением

};

// выбросить исключение с возможным сообщением
template <class T>
void CBLPtr<T>::_ErrorNULLObject(LPCSTR szErrorString) const
{
	if (!szErrorString)
		szErrorString = "CBLPtr<T> : попытка обращения к указателю NULL"; // TODO подставить имя реального класса для большего удобства

	::RuntimeError("%s", szErrorString); // TODO в дальнейшем можно выбрать другой вид исключения

};

#endif