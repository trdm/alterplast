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

#pragma once

#ifndef __BLPTR__INCLUDED
#define __BLPTR__INCLUDED

#include <boost/concept_check.hpp>

template <class T>
class CBLPtr
{
	template<class Y> friend class CBLPtr;
public:
	typedef T _PtrClass;
	CBLPtr() : p(NULL), m_bIsGroupContext(FALSE)
	{	}

	// ВНИМАНИЕ: код типа следующего ошибочен - остается лишняя ссылка
	//	CBLPtr<CExecuteModule> pContExecModule(new CExecuteModule);
	//
	// если использовать уже существующий указатель, то нужно использовать след.код
	//	CBLContext* pCont = new CExecuteModule;
	//	CBLPtr<CExecuteModule> pContExecModule(pCont);
	//	pCont.DecrRef();
	//
	// РЕКОМЕНДАЦИЯ: вместо подобного создания юзать CBLPtr<CExecuteModule> pContExecModule("");
	explicit CBLPtr(T* lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp);
	}

	CBLPtr(const CBLPtr<T>& lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp.p);
	}

	// explicit, скорее всего, не нужен - artbear нет, все-таки нужен для исключения неявного преобразования
	template<class U>
	explicit CBLPtr(const CBLPtr<U>& lp) : p(NULL), m_bIsGroupContext(FALSE)
	{
		_Assign(lp.p); // разрешено использовать только простые преобразования - от CBLPtr<Наследник_CBLContext> к CBLPtr<CBLContext>
	}

	// создать через СRuntimeClass::CreateObject
	explicit CBLPtr(CRuntimeClass& RuntimeClassRef) : p(NULL), m_bIsGroupContext(FALSE)
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
	// пришлось юзать const CString& - если юзать LPCSTR, то будет ошибка при создании из NULL
	// например, ошибка компиляции на CBLPtr<CBLContext> pCont(NULL);
    explicit CBLPtr(const CString& szTypeName) : p(NULL), m_bIsGroupContext(FALSE)
    {
		if (szTypeName.IsEmpty())
			Create();
		else
			Create(szTypeName);
    };
	~CBLPtr()
	{
		boost::function_requires< boost::ConvertibleConcept<T*, CBLContext*> >();

		Release();
	}
	operator T*() const
	{
		return get();
	}
	
	T* get() const
	{
		return p;
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

	CBLPtr<T>& operator=(const CBLPtr<T>& lp)
	{
		if (&lp == this)
			return *this;

		return operator=(lp.p);
	}

	template<class U>
	CBLPtr<T>& operator=(const CBLPtr<U>& lp)
	{
		assert(static_cast<void*>(const_cast<CBLPtr<U>*>(&lp)) != static_cast<void*>(this));
		//if (static_cast<void*>(const_cast<CBLPtr<U>*>(&lp)) == static_cast<void*>(this))
		//	return *this;

		return operator=(lp.p); // разрешено использовать только простые преобразования - от CBLPtr<Наследник_CBLContext> к CBLPtr<CBLContext>
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

    void Create(LPCSTR szTypeName)
    { // TODO нет проверки соответствия переданного имени и имени класса-шаблона

		Release();

		// имхо try не нужен artbear - а базы без справочников считаем багом :)
		// во избежание висящей ссылки не делаю Assign
		p = static_cast<T*>(CBLContext::CreateInstance(szTypeName)); //no cast is special: this only for CBLContext instance
		_CheckContext();
    };

	// является групповым контекстом
	inline BOOL IsGroupContext() const
	{
		return m_bIsGroupContext;
	};

	void reset()
	{
		CBLPtr<T>().swap(*this);
	}

	void swap(CBLPtr<T> & other) // never throws
	{
		std::swap(p, other.p);
		std::swap(m_bIsGroupContext, other.m_bIsGroupContext);
	}

	boost::shared_ptr<T> get_shared_ptr() const
	{
		p->IncrRef();
		return boost::shared_ptr<T>(p, CBLContext_deleter<T>());
	}

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