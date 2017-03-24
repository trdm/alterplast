//______________________________________________________________________________
//
// Класс-указатель на CBLContext, определенный через CValue
//
// Это очень безопасный класс, отслеживающий уничтожение любых контекстов, 
//		в т.ч. и групповых контекстов
//	Выполняется автоподсчет ссылок и автоматическое разыменование 
//
//	Существует подобный шаблонный класс - умный указатель на контекст CBLPtr<T> (blptr.h)
//		но он не так безопасен при использовании контекстов, например, групповых
// 
// artbear, 2006
//______________________________________________________________________________
//

#ifndef __SAFE__CONTEXT__INCLUDED
#define __SAFE__CONTEXT__INCLUDED

template <class TCont = CBLContext>
class CSafeContextPtr
{
// 	typedef CBLContext TCont;
	typedef TCont * TContPtr;
	typedef const TCont * ConstTContPtr;
private:

	CValue *m_pValue;
	
private:

	// назначение ссылки
	void _Assign(ConstTContPtr pCont) // без использования Release (типа для скорости)
	{
		if (pCont)
		{
			m_pValue = new CValue;
			m_pValue->AssignContext(const_cast<TContPtr>(pCont));
			
		}
	};
	
	void Assign(ConstTContPtr pCont)
	{
		Release();
		_Assign(pCont);
	};

 	// освобождение ссылки
	void Release()
	{
		if (m_pValue)
		{
			delete m_pValue;
			
			m_pValue = NULL;
		}
	};

	// выбросить исключение с возможным сообщением
	static void _ErrorNULLObject(LPCSTR szErrorString = NULL)
	{
		if (!szErrorString)
			szErrorString = "CSafeContextPtr : попытка обращения к указателю NULL"; // TODO подставить имя реального класса для большего удобства

		::RuntimeError("%s", szErrorString); // TODO в дальнейшем можно выбрать другой вид исключения

	};

public:

	// инициализаторы
	CSafeContextPtr() : m_pValue(NULL)
	{ };

	CSafeContextPtr(ConstTContPtr pCont) : m_pValue(NULL)
	{ 
		_Assign(pCont);
	};

	CSafeContextPtr(const CSafeContextPtr& ref) : m_pValue(NULL)
	{
		if (ref.m_pValue)
			_Assign(static_cast<ConstTContPtr>(ref.m_pValue->GetContext()));

	};

	// создание объекта заданного типа
	CSafeContextPtr(LPCSTR szName) : m_pValue(NULL)
	{
		Create(szName);
	};

	virtual ~CSafeContextPtr()
	{
		Release();
	};

	// является групповым контекстом
	inline bool IsGroupContext() const
	{
		return m_pValue && IS_KINDOF_RUNTIME_CLASS(m_pValue->GetContext(), CGroupContext );
	};
public:

	// операторы
	CSafeContextPtr& operator =(ConstTContPtr pCont)
	{
		if (operator TContPtr() != pCont)
			Assign(pCont);

		return *this;
	};
	CSafeContextPtr& operator=(const CSafeContextPtr& lp)
	{
		if (&lp == this)
			return *this;

		return operator=((TContPtr)lp);
	};

	operator bool() const
	{
		return (m_pValue && m_pValue->GetContext());
	};
	operator !() const
	{ 
		return !operator bool();
	};

	bool operator==(ConstTContPtr pT) const
	{
		return operator TContPtr() == pT;
	};
	bool operator!=(ConstTContPtr pT) const
	{
		return !operator ==(pT);
	};

	operator TContPtr() const
	{ 
		return !m_pValue ? NULL : static_cast<TContPtr>(m_pValue->GetContext());
	};
	TContPtr operator->() const
	{
		TContPtr pCont = operator TContPtr();
		ATLASSERT(pCont!=NULL);

		if (pCont)
			return pCont;

		_ErrorNULLObject();
		return pCont; // иначе предупреждение компилятора
	}
	
	// код метода вынесен в cpp-файл для возможности runtime-исключения 1C в случае разыменования указателя NULL
	// иначе компилятор ругается
	// но возможна странная ошибка для этого оператора, описанная в blptr.h
	//inline TCont& operator*() const;
	TCont& operator*() const
	{
		TContPtr pCont = operator TContPtr();
		ATLASSERT(pCont!=NULL);

		if (pCont)
			return *pCont;

		_ErrorNULLObject();
		return *pCont; // иначе предупреждение компилятора
	}

    void Create(const CString& szTypeName)
	{
		Release();

		// с автоудалением созданного контекста, иначе получим висящий объект, который никогда не будет удален
		CBLPtr<TCont> pCont(szTypeName); //no cast is special: this only for CBLContext instance

		if (pCont)
			Assign(pCont);
		else
			if (!szTypeName.CompareNoCase("ГрупповойКонтекст") || !szTypeName.CompareNoCase("GroupContext"))
			{
				TContPtr pCont = static_cast<CGroupContext*>(RUNTIME_CLASS(CGroupContext)->CreateObject());
			
				// и второй раз IncrRef
				Assign(pCont);

				pCont->DecrRef(); // во избежание висящей ссылки
			}

	};


	CFormAllCtrlsContext* GetFormContext(void)
	{
		if(!IsGroupContext())
			return NULL;

		return GetFormContext(operator TContPtr());
	}

	CGetDoc7* GetDoc7(void)
	{
		return GetDoc7(operator TContPtr());
	}

};

typedef CSafeContextPtr<CBLContext> CSafeContextPtrDef;

// безопасный класс-указатель на CBLModule7
//	класс отслеживает создание/удаление модулей 1С
//  в деструкторе модуля CBLModule7 обнуляются все указатели-объекта класса, 
// которые указывают на удаляемый модуль
//
//	Схема связи построена аналогично связи CBLContext - CVlalue
//
//	В итоге становится безопасно хранить указатели на модули, не опасаясь обращения к модулю после его уничтожения
//		например, смотрите CComponentClass::pCurModule и метод ПолучитьКонтекстОкружения 
//	( устаревший, но теперь работающий стабильно, метод)
//
//	для CBLModuleWrapper* сделана схема с подсчетом ссылок для возможности удаления контекста, выделенного через new
//
class CSafeModulePtr
{
private:
	typedef CBLModule T; // ? CBLModule7 ?
	typedef CBLModule* TPtr;
	typedef const CBLModule* ConstTPtr;

	TPtr m_pMod;
	bool m_bIsAutoDelete;

private:

	void Link(ConstTPtr pMod);
	void Unlink();

	// назначение ссылки
	void _Assign(ConstTPtr pMod) // без использования Release (типа для скорости)
	{ 
		Link(pMod);

		m_pMod = const_cast<TPtr>(pMod);
		
		if (m_pMod)
			if (m_bIsAutoDelete)
			{	
				int iRefCount;
				if (m_mapOfRefModules.Lookup(m_pMod, iRefCount))
					m_mapOfRefModules.SetAt(m_pMod, ++iRefCount);
				else
					m_mapOfRefModules.SetAt(m_pMod, 1);
			}
	}
	
	void Assign(ConstTPtr pMod)
	{
		Release();
		m_bIsAutoDelete = false;
		_Assign(pMod);
	};

	void _Assign(const CBLModuleWrapper* pNewMod)
	{
		m_bIsAutoDelete = true;
		_Assign((ConstTPtr)pNewMod);
	};
	
	void _Assign(const CSafeModulePtr& rp)
	{
		m_bIsAutoDelete = rp.m_bIsAutoDelete;
		_Assign(rp.m_pMod);
	};
	

	template <class T>
	void Assign(const T& rp)
	{
		Release();
		_Assign(rp);
	};

	// освобождение ссылки
	void Release()
	{ 
		Unlink();

		if (m_pMod)
			if (m_bIsAutoDelete)
			{
				int iRefCount;
				if (m_mapOfRefModules.Lookup(m_pMod, iRefCount))
				{
					m_mapOfRefModules.SetAt(m_pMod, --iRefCount);
					if (!iRefCount)
					{
						m_mapOfRefModules.RemoveKey(m_pMod);
						m_bIsAutoDelete = false;
						delete m_pMod;
					}
				}
			}
		m_pMod = NULL;
	}

	// выбросить исключение с возможным сообщением
	void _ErrorNULLObject(LPCSTR szErrorString = NULL) const;

public:

	// включает перехват конструкторов и деструктора CBLModule7 для правильного учета
	static void InitHook();
	static void DestroyHook();

	// инициализаторы
	CSafeModulePtr() : m_pMod(NULL), m_bIsAutoDelete(false)
	{ };

public:

	CSafeModulePtr(ConstTPtr pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	{ 
		InitHook();
		_Assign(pMod);
	};

	// указатель должен быть получен через new !!
	// ссылка const CBLModuleWrapper*& нужна для того, чтобы исключить неопределенность с пред. оператором =
	// например, без ссылки код CSafeModulePtr pMod(NULL); не скомпилируется
	//CSafeModulePtr(const CBLModuleWrapper*& pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	CSafeModulePtr(const CBLModuleWrapper& pMod) : m_pMod(NULL), m_bIsAutoDelete(false)
	{ 
		InitHook();
		_Assign(&pMod);
	};

	CSafeModulePtr(const CSafeModulePtr& rp) : m_pMod(NULL), m_bIsAutoDelete(false)
	{
		InitHook();
		// TODO
		_Assign(rp);
	};

	~CSafeModulePtr()
	{ Release(); };

	// является групповым контекстом
	inline bool IsGroupContext() const
	{
		return false; //return m_pValue && IS_KINDOF_RUNTIME_CLASS(m_pValue->GetContext(), CGroupContext );
	};

	// операторы
	CSafeModulePtr& operator =(ConstTPtr  pMod)
	{
		//if (operator TPtr () != pMod)
		Assign(pMod);

		return *this;
	};

	// указатель должен быть получен через new !!
	// ссылка const CBLModuleWrapper*& нужна для того, чтобы исключить неопределенность с пред. оператором =
	// например, без ссылки код pMod = NULL; не скомпилируется
	CSafeModulePtr& operator=(const CBLModuleWrapper& pMod)
	{ 
		Assign(&pMod);

		return *this;
	};
	
	CSafeModulePtr& operator=(const CSafeModulePtr& rp)
	{
		if (&rp == this)
			return *this;

		Assign(rp);

		return *this;

	};
public:

	operator bool() const
	{
		return m_pMod != NULL; // return (m_pValue && m_pValue->GetContext());
	};
	operator !() const
	{ 
		return !operator bool();
	};

	bool operator==(TPtr pT) const
	{
		return operator TPtr() == pT;
	};
	bool operator!=(TPtr pT) const
	{
		return !operator ==(pT);
	};

	operator TPtr() const
	{ 
		return m_pMod; // return !m_pValue ? NULL : m_pValue->GetContext();
	};
	TPtr operator->() const
	{
		TPtr pMod = operator TPtr();
		ATLASSERT(pMod!=NULL);

		if (pMod)
			return pMod;

		_ErrorNULLObject();
		return pMod; // иначе предупреждение компилятора
	}
	
	// код метода вынесен в cpp-файл для возможности runtime-исключения 1C в случае разыменования указателя NULL
	// иначе компилятор ругается
	// но возможна странная ошибка для этого оператора, описанная в blptr.h
	//inline CBLContext& operator*() const;
	T& operator*() const
	{
		TPtr pMod = operator TPtr();
		ATLASSERT(pMod!=NULL);

		if (pMod)
			return *pMod;

		_ErrorNULLObject();
		return *pMod; // иначе предупреждение компилятора
	}
	
	typedef vector<CSafeModulePtr*> CVectorModulePtr;
	typedef CMap<ConstTPtr, ConstTPtr, CVectorModulePtr*, CVectorModulePtr*> CModulesMap;

	typedef CMap<CBLModule*, CBLModule*, int, int> CRefModules;
	
private:
	CBLModule* WrapCBLModuleConstructor1(class CBLModule const &);
	CBLModule* WrapCBLModuleConstructor2(class CBLContext *,char const *);
	void WrapCBLModuleDestructor(void);

	static bool bAlreadyInit; 

	static CModulesMap m_mapOfLinkModules; // карта связанных модулей

	// карта модулей, которые мы сами должны удалять (CBLModuleWrapper)
	static CRefModules m_mapOfRefModules; // карта связанных модулей

	static void ClearModulePtr(CSafeModulePtr* ptr);

};

#endif //#ifndef __SAFE__CONTEXT__INCLUDED
