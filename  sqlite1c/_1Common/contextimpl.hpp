// contextimpl.hpp
#pragma once
#pragma warning(disable: 4786)	// Подавляем ворнинги об слишком длинных именах
/*
(с) Александр Орефков telepat@inbox.ru

Заголовочный файл для реализации CBLContext'ных объектов.
Использование:
Для объявления класса, автоматически регистрируемого для СоздатьОбъект, объявите свой класс как

  class ВашКласс : public CContextImpl<ВашКласс>

либо, если класс не должен создаваться через СоздатьОбъект, то так:

  class ВашКласс : public CContextImpl<ВашКласс, no_init_done>


Перед объявлением методов и свойств контекста используйте макрос
	
	BL_BEGIN_CONTEXT("EnglishName", "РусскоеИмя");

Он задает имена типа 1С для этого класса (возвращаются в ТипЗначенияСтр) и вводит
необходимые для реализации детали.

Для создания методов контекстного объекта используйте макросы в объявлении класса:
BL_PROC - объявляет процедуру контекста
BL_FUNC - объявляет функцию контекста
BL_PROC_WITH_DEFVAL - объявляет процедуру контекста, принимающую параметры по умолчанию
BL_FUNC_WITH_DEFVAL - объявляет функцию контекста, принимающую параметры по умолчанию

В качестве параметров эти макросы принимают:
АнглийскоеИмя			- задает английское имя метода контекста
"РусскоеИмя"			- задает русское имя метода контекста
КоличествоПараметров	- задает количество параметров метода

Данные макросы создают необходимую инфраструктуру для реализации CBLContext, и объявляют методы
	int АнглийскоеИмя(CValue** ppParams) для процедур
	или
	int АнглийскоеИмя(CValue& retVal, CValue** ppParams) для функций.
То есть непосредственно после макроса можно либо поставить ; и реализовать метод вне класса,
либо сразу написать реализацию метода.
Например:

	// В заголовке класса
	BL_PROC(Test, "ЭтоТест", 0)
	{
		AfxMessageBox("Test");
		return TRUE;
	}

либо так:

	// В заголовке класса
	BL_PROC(Test, "ЭтоТест", 0);

// Вне класса:
int CMyContextClass::Test(CValue** ppParams)
{
	AfxMessageBox("Test");
	return TRUE;
}

Для методов, принимающих значения по умолчанию, необходимо объявить функцию класса вида
int АнглийскоеИмя_GetDefVal(int nParam, CValue* pVal)
что удобно делать макросом

  BL_DEFVAL_FOR(АнглийскоеИмя)


Для объявления свойств контекста используйте макросы:

BL_PROP_RO	- задает свойство только для чтения
BL_PROP_WO	- задает свойство только для записи
BL_PROP_RW	- задает свойство для чтения/записи

Параметры макросов:
АнглийскоеИмя	- Задает английское имя свойства
"РусскоеИмя"	- Задает русское имя свойства

Макросы создают имена обработчиков свойств как
int Get_АнглийскоеИмя(CValue& value) const - для чтения
int Set_АнглийскоеИмя(CValue const& value) - для записи

Сразу после макроса можно либо поставить ; и реализовать обработчик вне класса,
либо сразу написать реализацию обработчика - для BL_PROP_RO и BL_PROP_RW
обработчик чтения, для BL_PROP_WO - обработчик записи.
При объявлении свойства для чтения/записи для написания обработчика записи
используйте макрос
BL_PROP_WRITE(АнглийскоеИмя)

После указания всех методов/свойств контекста, вставьте макрос

 	BL_END_CONTEXT();

Для авторегистрации вашего класса для СоздатьОбъект, в файле реализации
класса используйте макрос:

  BL_INIT_CONTEXT(ИмяВашегоКласса);

 Пример класса:
// файл .h

class CTestBL : public CContextImpl<CTestBL>
{
public:
	BL_BEGIN_CONTEXT("TestBL", "ТестКонтекст");

	BL_PROC(Test, "Тест", 0);

	long m_a;

	BL_PROC(DoBox, "Показать", 0)
	{
		AfxMessageBox("DoBox");
		return TRUE;
	}

	BL_FUNC(SomeFunc, "Проверка", 0)
	{
		retVal=1;
		return TRUE;
	}

	BL_PROC_WITH_DEFVAL(DefTest, "СУмолчанием", 1)
	{
		CString txt=ppParams[0]->GetString();
		if(!txt.IsEmpty())
			AfxMessageBox(txt);
		return TRUE;
	}
	BL_DEFVAL_FOR(DefTest)
	{
		if(nParam==0)
		{
			if(pValue)
				*pValue="Hello World!!!";
			return TRUE;
		}
		return FALSE;
	}

	BL_PROP_RO(SomeProp, "Нечто")
	{
		value="Test of Some Prop";
		return TRUE;
	}
	BL_PROP_WO(SomeWrite, "ТолькоЗапись")
	{
		return TRUE;
	}

	BL_PROP_RW(ReadWrite, "ПишиЧитай")
	{
		value=m_a;
		return TRUE;
	}
	BL_PROP_WRITE(ReadWrite)
	{
		m_a=value.GetNumeric();
		return TRUE;
	}

	BL_END_CONTEXT();
};
.... 

//файл .cpp
BL_INIT_CONTEXT(CTestBL);

int CTestBL::Test(CValue** ppParams)
{
	AfxMessageBox("Test");
	return TRUE;
}

Для инициализации и регистрации всех классов контекста
при инициализации компоненты вызовите метод:
  context_obj::CContextBase::InitAllContextClasses();

При выгрузке компоненты вызовите метод:
  context_obj::CContextBase::DoneAllContextClasses()
*/

namespace context_obj{
// Основной язык конфигурации
int __declspec(selectany) defLang=0;

namespace str_find{
// Таблица нижнерегистровых символов
DWORD __declspec(selectany) lotable[256]={0};
}//namespace str_find

// Заранее объявим нужные сегменты
#pragma data_seg(".blc$a")
#pragma data_seg(".blc$b")
#pragma data_seg(".blc$c")
#pragma data_seg(".blc$d")
#pragma data_seg(".blc$e")
#pragma data_seg()
// Укажем линкеру в конечном файле слить сегменты .blc и .data
#pragma comment(linker, "/MERGE:.blc=.data")

typedef void (*P_VV)();

__declspec (allocate(".blc$a")) P_VV __declspec(selectany) pBLInit=NULL;
__declspec (allocate(".blc$c")) P_VV __declspec(selectany) pBLInitDone=NULL;
__declspec (allocate(".blc$e")) P_VV __declspec(selectany) pBLDone=NULL;

// Базовая реализация методов, практически всегда одинаково реализуемых
class CContextBase : public CBLContext
{
public:
	CContextBase():CBLContext(){}
	virtual char const*  GetCode(void)const		{return NULL;}
	virtual int  GetDestroyUnRefd(void)const	{return TRUE;}
	virtual CObjID GetID(void)const				{return CObjID();}
	virtual long  GetTypeID(void)const			{return 100;}
	virtual CType GetValueType(void)const		{return CType(100);}
	virtual int  IsExactValue(void)const		{return TRUE;}
	virtual int  IsOleContext(void)const		{return FALSE;}
	virtual int  IsSerializable(void)			{return FALSE;}
	virtual int  SaveToString(CString &)		{return FALSE;}
	virtual void InitObject(CType const &t)		{}
	virtual void InitObject(LPCSTR lpName)		{}
	

	static void InitAllContextClasses()
	{
		defLang=GetMetaData()->GetTaskDef()->GetDefaultLanguage();
		for(int i=0;i<256;i++)
			str_find::lotable[i]=(DWORD)CharLower((LPSTR)i);
		for(P_VV* ppInit=&pBLInit+1;ppInit<&pBLInitDone;ppInit++)
			if(*ppInit)
				(*ppInit)();
	}

	static void DoneAllContextClasses()
	{
		for(P_VV* ppDone=&pBLInitDone+1;ppDone<&pBLDone;ppDone++)
			if(*ppDone)
				(*ppDone)();
	}
};

/*
Описание механизма реализации.
Сделана на основе базового шаблонного класса, в котором параметром шаблона
задается конечный класс, для использования статического полиморфизма.

Каждый метод/свойство описывается новым типом, выполнящим определенные требования.
И для методов, и для свойств эти требования таковы:
- Каждый тип должен иметь ссылку prev на тип предыдущего метода/свойства:
	typedef ПредыдущийТип prev;
- Каждый тип должен иметь перечисление val, равное val предыдущего типа плюс 1:
	enum{val=prev::val+1};
- Каждый тип должен иметь методы:
	static LPCSTR GetRusName(); // возвращает русское имя метода/свойства
	static LPCSTR GetEngName(); // возвращает английское имя метода/свойства

Для описаний методов тип кроме этого должен иметь:
- перечисление IsFunc = 0 | 1, показывает, процедура это или функция.
- перечисление IsDefVal = 0 | 1, показывает, есть ли у метода параметры по умолчанию.
- перечисление nParam, показывает, сколько параметров у метода.
- методы:
	static int CallProc(T* ptr, CValue** ppParams);						// Вызов как процедуры
	static int CallFunc(T* ptr, CValue& retVal, CValue** ppParams);		// Вызов как функции
	static int GetDefVal(const T* ptr, int nParam, CValue* pParam);		// Получить параметр по умолчанию

Для описаний параметров тип должен иметь:
- перечисление IsRead	= 0 | 1, показывает, доступно ли свойство для чтения
- перечисление IsWrite	= 0 | 1, показывает, доступно ли свойство для записи
- методы:
	static int Read(const T* ptr, CValue& value);	// Вызов для чтения
	static int Write(T* ptr, CValue const& value);	// Вызов для записи

Тип первого метода/свойства в качестве предыдущего типа должен использовать тип first_def.
В самом классе должны быть определены типы endmeths для методов и endprops для свойств,
в которых указываются ссылка prev на тип последнего из реализуемых методов/свойств,
и перечисление val, равное val типа последнего метода/свойства плюс один.

Все это хозяйство упрятано в макросы BL_BEGIN_CONTEXT, BL_PROC, BL_FUNC,
BL_END_CONTEXT и тп.

Реализация через тип T::endmeths и T::endprops получает доступ к списку типов
методов/свойств контекста, и обрабатывая его хитрыми шаблонами, генерирует
оптимальный код реализации виртуальных функций CBLContext'а.
*/

// Основа инфраструктуры работы с описаниями методов/свойств
struct first_def
{
	enum
	{
		val		=-1,
		IsFunc	=-1,
		nParam	=-1,
		IsDefVal=-1,
		IsRead	=-1,
		IsWrite	=-1,
	};
};

// Набор алгоритмов для осуществления поиска номера по строке
namespace str_find{

static inline DWORD symbol(LPCSTR ptr){return static_cast<DWORD>(static_cast<BYTE>(*ptr));}

// Копирует строку в буфер в нижнем регистре
static inline void makelower(LPCSTR& ptr, LPSTR buf)
{
	for(;;)
	{
		DWORD s=lotable[symbol(ptr)];
		*buf=static_cast<char>(s);
		if(!s)
			return;
		buf++;
		ptr++;
	}
}
// Сравнивает на равенство строки, где ptr1 и ptr2 - любая строка
static inline BOOL compareUU(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && lotable[symbol(ptr1)] == lotable[symbol(ptr2)])
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// Сравнивает на равенство строки, где ptr1 - строка в нижнем регистре,
// а ptr2 - любая строка
static inline BOOL compareLU(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && symbol(ptr1) == lotable[symbol(ptr2)])
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// Сравнивает на равенство строки, где ptr1 и ptr2 - строки в нижнем регистре
static inline BOOL compareLL(LPCSTR ptr1, LPCSTR ptr2)
{
	while(symbol(ptr1) && symbol(ptr1) == symbol(ptr2))
		ptr1++,ptr2++;
	return symbol(ptr1)==symbol(ptr2);
}

// Стратегии поиска номера элемента по его имени

// Стратегия при отсутствии элементов
template<typename T>
struct nofnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name){return -1;}
};

// Стратегия при наличии всего одного элемента
template<typename T>
struct onefnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name)
	{
		if(name && (compareUU(name, T::prev::GetRusName()) ||
			compareUU(name, T::prev::GetEngName())))
			return 0;
		return -1;
	}
};

// Стратегия поиска при количестве элементов от 2 до 3
template<typename T>
struct simplefnd_pol
{
	static void OnInitFirstObject(){}
	static int Find(LPCSTR name)
	{
		if(!name)
			return -1;
		char buf[512];
		makelower(name, buf);
		return find(buf, (T::prev*)NULL);
	}
	template<typename M>
	static int find(LPCSTR buf, M*)
	{
		if(compareLU(buf, M::GetRusName()) ||
			compareLU(buf, M::GetEngName()))
			return M::val;
		return find(buf, (M::prev*)NULL);
	}
	template<>static int find<first_def>(LPCSTR buf, first_def* p){return -1;}
};

// Стратегия при количестве элементов больше 3
struct hs_find_nearest_bigger
{
	template<int I>	struct hashsize{enum{val = hashsize<I + 1>::val};};
	#define HASH_SIZE(par) template<>struct hashsize<par>{enum{val=par};}
	HASH_SIZE(13); HASH_SIZE(19); HASH_SIZE(29); HASH_SIZE(37); HASH_SIZE(47); HASH_SIZE(53);
};

struct hs_simple_calc
{
	template<int I>struct hashsize{enum{ val = (I + I / 2) | 1};};
};

template<BOOL I> struct hs_selector { typedef hs_find_nearest_bigger type;};
template<> struct hs_selector<FALSE>{ typedef hs_simple_calc type;};

template<typename T, int I> struct hs_get {	enum{val = T::hashsize<I>::val};};

template<int I>
struct hashsizes
{
	enum{ val = hs_get<hs_selector<(I < 53 ? TRUE : FALSE)>::type, I>::val };
};


template<typename T>
struct hashfnd_pol
{
	enum{
		Count=T::val,
		HashSize=hashsizes<Count*2>::val,
	};
	struct Assoc // Структура для хранения данных о парах ключ-номер
	{
		Assoc(int p, BYTE* buf):pos(p), strKey(buf){}
		DWORD hash;
		BYTE *strKey;
		int pos;
		Assoc *next;
	};

	Assoc* ppHashTable[HashSize];	// хэш-таблица. Массив указателей на односвязные списки с ключами
	
	static DWORD GetHash(LPCTSTR key, DWORD& _hash, BYTE *buf)
	{
		_hash=0;
		for(;;)
		{
			DWORD symb=lotable[symbol(key)];
			*buf=static_cast<BYTE>(symb);
			if(!symb)
				return _hash % HashSize;
			_hash=(_hash<<1)^symb;
			buf++;
			key++;
		}
	}
	
	hashfnd_pol(){memset(ppHashTable, 0, sizeof(ppHashTable));}
	~hashfnd_pol()
	{
		int cnt=Count*2;
		for(int i=0;i<HashSize && cnt;i++)
		{
			Assoc* pAssoc=ppHashTable[i];
			while(pAssoc)
			{
				delete [] pAssoc->strKey;
				Assoc* pDel=pAssoc;
				pAssoc=pAssoc->next;
				delete pDel;
				cnt--;
			}
		}
	}
	int find(LPCSTR name)
	{
		if(!name)
			return -1;
		BYTE buf[512];
		DWORD hash;
		Assoc *pAssoc=ppHashTable[GetHash(name, hash, buf)];
		while(pAssoc)
		{
			if(pAssoc->hash==hash)
			{
				if(compareLL(reinterpret_cast<LPCSTR>(pAssoc->strKey), reinterpret_cast<LPCSTR>(buf)))
					return pAssoc->pos;
			}
			pAssoc=pAssoc->next;
		}
		return -1;
	}
	void Insert(LPCSTR name, int pos)
	{
		Assoc* pNew=new Assoc(pos, new BYTE[strlen(name)+1]);
		int posInTable=GetHash(name, pNew->hash, pNew->strKey);
		pNew->next=ppHashTable[posInTable];
		ppHashTable[posInTable]=pNew;
	}

	static hashfnd_pol<T>*& getme(){static hashfnd_pol<T>* ptr;return ptr;}

	template<typename M>
	static void AddNames(M* p=NULL)
	{
		getme()->Insert(M::GetRusName(), M::val);
		getme()->Insert(M::GetEngName(), M::val);
		AddNames((M::prev*)NULL);
	}
	template<> static void AddNames<first_def>(first_def* p){}


	static void OnInitFirstObject()
	{
		getme()=new hashfnd_pol<T>;
		AddNames((T::prev*)NULL);
	}
	static int Find(LPCSTR name){return getme()->find(name);}
};

}//namespace str_find

// Мета-функция, выбирает тип поиска номера по строке,
// взависимости от количества методов/свойств.
// Использовать: fndselector<T::endmeths>::type, fndselector<T::endprops>::type
template<typename T>
struct fndselector
{
	template<int K>struct tdef;
	template<>struct tdef<0>{typedef str_find::nofnd_pol<T>		type;};
	template<>struct tdef<1>{typedef str_find::onefnd_pol<T>	type;};
	template<>struct tdef<2>{typedef str_find::simplefnd_pol<T>	type;};
	template<>struct tdef<3>{typedef str_find::hashfnd_pol<T>	type;};

	enum{
		Count=T::val,
		val = Count==0 ? 0 : (Count==1 ? 1 : (Count<4 ? 2 : 3)),
	};
	typedef typename tdef<val>::type type;
};

// Возврат имени из массива имен по индексу.
// Массив имен создается при первом обращении к GetName.
template<typename T>
struct getname
{
	template<int Count>
	struct finder
	{
		LPCSTR names[Count][2];
		static finder<Count>* getme(){static finder<Count>* one=new finder<Count>;return one;}
		finder(){fill((T::prev*)NULL);}
		template<typename M>
		void fill(M*)
		{
			names[M::val][0]=M::GetEngName();
			names[M::val][1]=M::GetRusName();
			fill((M::prev*)NULL);
		}
		template<>void fill<first_def>(first_def*){}

		static LPCSTR GetName(DWORD dwMethod, DWORD dwLang)
		{
			if(dwMethod < Count && dwLang < 2)
				return getme()->names[dwMethod][dwLang];
			return NULL;
		}
	};
	template<>
	struct finder<0>
	{
		static LPCSTR GetName(DWORD dwMethod, DWORD dwLang){return NULL;}
	};
	typedef typename finder<T::val> type;
};

namespace meta_func{
// Набор различных метафункций, работающих со списками типов
// описаний методов и свойств.

// Проверка на то, есть BL_END_CONTEXT() и он в правильном месте
template<typename T, int I>
struct test_last
{
	enum{val = I < T::val ? 1 : -1};
	// Если у вас здесь ошибка, значит, макрос BL_END_CONTEXT() в вашем
	// классе отсутствует, либо вставлен не после последнего метода/свойства
	// Нажмите F4, чтобы посмотреть, где
	typedef char type[val];
};

// Проверка на то, что методы с параметрами по умолчанию имеют не меньше 1 параметра
template<int I, BOOL bDef> struct check_defval
{
	enum{val = bDef && !I ? -1 : 1};
	// Если здесь у вас ошибка, значит, для метода с параметрами по умолчанию
	// задано 0 параметров
	typedef char type[val];
};

// Мета-функция, подсчитывает, сколько среди методов процедур и функций.
// Использовать:
// meth_count<T>::proc - количество процедур
// meth_count<T>::func - количество функций
template<typename T>
struct meth_count
{
	template<int I>
	struct counter
	{
		template<typename M>
		struct check
		{
			enum{
				next = M::prev::val == -1 ? 0 : 1,
				val = M::IsFunc + counter<next>::template check<M::prev>::val,
			};
		};
	};
	template<> struct counter<0>{template<typename M>struct check{enum{val = 0};};};

	enum{
		first	= T::endmeths::prev::val == -1 ? 0 : 1,
		func	= counter<first>::check<T::endmeths::prev>::val,
		proc	= T::endmeths::val - func,
	};
};

// Мета-функция, определяет, во всех-ли методах одинаковое количество параметров
// Использовать: some_np<T>::val
// Возвращает: -1 - в методах разное количество параметров, N - во всех методах N параметров
template<typename T>
struct some_np
{
	template<int I>	struct _some_np{template<typename M>struct check{enum{val=I};};};
	template<> struct _some_np<-1>{template<typename M>struct check{enum{val=-1};};};
	template<> struct _some_np<-2>
	{
		template<typename M>
		struct check
		{
			enum{
				stop = M::prev::val==-1? 1 : 0,
				some = M::nParam == M::prev::nParam ? 1 : 0,
				next = stop ? M::nParam : (some ? -2 : -1),
				val  = _some_np<next>::template check<M::prev>::val,
			};
		};
	};
	enum{
		first=T::endmeths::prev::val==-1 ? 0 : -2,
		val=_some_np<first>::check<T::endmeths::prev>::val
	};
};

// Мета-функция, подсчитывает, сколько параметров доступно для чтения/записи.
// Использовать:
// rw_count<T>::read	- доступных для чтения
// rw_count<T>::write	- доступных для записи
// rw_count<T>::noread	- недоступных для чтения
// rw_count<T>::nowrite	- недоступных для записи
template<typename T>
struct rw_count
{
	template<int I>
	struct rcounter
	{
		template<typename M>
		struct check
		{
			enum{
				next= M::prev::val==-1 ? 0 : 1,
				val = M::IsRead + rcounter<next>::template check<M::prev>::val,
			};
		};
	};
	template<>struct rcounter<0>{template<typename M>struct check{enum{val = 0};};};

	template<int I>
	struct wcounter
	{
		template<typename M>
		struct check
		{
			enum{
				next= M::prev::val==-1 ? 0 : 1,
				val = M::IsWrite + wcounter<next>::template check<M::prev>::val,
			};
		};
	};
	template<>struct wcounter<0>{template<typename M>struct check{enum{val = 0};};};

	enum{
		first=T::endprops::prev::val==-1 ? 0 : 1,
		read = rcounter<first>::check<T::endprops::prev>::val,
		write= wcounter<first>::check<T::endprops::prev>::val,
		noread = T::endprops::val - read,
		nowrite= T::endprops::val - write,
	};
};

}//namespace meta_func

template<typename T>
struct meth_pol
{
	enum{
		isFuncMin = meta_func::meth_count<T>::func <= meta_func::meth_count<T>::proc ? 1 : 0,
		nParamCmn = meta_func::some_np<T>::val,
	};

	static int HasRetVal(DWORD dwMethod)
		{return policy<isFuncMin>::HasRetVal(dwMethod, (T::endmeths::prev*)NULL);}

	static int GetNParams(DWORD dwMethod)
		{return nparam<nParamCmn>::GetNParams(dwMethod);}

	static int GetDefVal(const T* ptr, DWORD dwMethod, DWORD dwParam, CValue* pParam)
		{return GetDefVal(ptr, dwMethod, dwParam, pParam, (T::endmeths::prev*)NULL);}
	
	static int CallAsProc(T* ptr, DWORD dwMethod, CValue** ppParams)
	{
		return CallAsProc(ptr, dwMethod, ppParams, (T::endmeths::prev*)NULL);
	}
	static int CallAsFunc(T* ptr, DWORD dwMethod, CValue& ret, CValue** ppParams)
	{
		return CallAsFunc(ptr, dwMethod, ret, ppParams, (T::endmeths::prev*)NULL);
	}

	template<BOOL bFunc/*==TRUE*/>
	struct policy
	{
		template<typename M>
		static int HasRetVal(DWORD dwMethod, M*)
		{
			if(M::IsFunc && dwMethod==M::val)
				return TRUE;
			return HasRetVal(dwMethod, (M::prev*)NULL);
		}
		template<> static int HasRetVal<first_def>(DWORD, first_def*){return FALSE;}
	};
	template<>
	struct policy<FALSE>
	{
		template<typename M>
		static int HasRetVal(DWORD dwMethod, M*)
		{
			if(!M::IsFunc && dwMethod==M::val)
				return FALSE;
			return HasRetVal(dwMethod, (M::prev*)NULL);
		}
		template<> static int HasRetVal<first_def>(DWORD, first_def*){return TRUE;}
	};
	
	template<int I>struct nparam{static int GetNParams(DWORD){return I;}};

	template<>
	struct nparam<-1>
	{
		static int GetNParams(DWORD dwMethod)
		{
			if(dwMethod<T::endmeths::val)
				return _GetNParams(dwMethod, (T::endmeths::prev*)NULL);
			return -1;
		}
		template<typename M>
		static int _GetNParams(DWORD dwMethod, M*)
		{
			if(dwMethod==M::val)
				return M::nParam;
			return _GetNParams(dwMethod, (M::prev*)NULL);
		}
		template<>static int _GetNParams<first_def>(DWORD, first_def*){return -1;}
	};

	template<typename M>
	static int GetDefVal(const T* ptr, DWORD dwMethod, DWORD dwParam, CValue* pParam, M*)
	{
		if(M::IsDefVal && dwMethod==M::val && dwParam < M::nParam)
			return M::GetDefVal(ptr, dwParam, pParam);
		return GetDefVal(ptr, dwMethod, dwParam, pParam, (M::prev*)NULL);
	}
	template<>static int GetDefVal<first_def>(const T*, DWORD, DWORD, CValue*, first_def*){return FALSE;}

	template<typename M>
	static int CallAsProc(T* ptr, DWORD dwMethod, CValue** ppParams, M*)
	{
		if(!M::IsFunc && dwMethod==M::val)
			return M::CallProc(ptr, ppParams);
		return CallAsProc(ptr, dwMethod, ppParams, (M::prev*)NULL);
	}
	template<>static int CallAsProc<first_def>(T*, DWORD, CValue**, first_def*){return FALSE;}

	template<typename M>
	static int CallAsFunc(T* ptr, DWORD dwMethod, CValue& ret, CValue** ppParams, M*)
	{
		if(M::IsFunc && dwMethod==M::val)
			return M::CallFunc(ptr, ret, ppParams);
		return CallAsFunc(ptr, dwMethod, ret, ppParams, (M::prev*)NULL);
	}
	template<>static int CallAsFunc<first_def>(T*, DWORD, CValue&, CValue**, first_def*){return FALSE;}
};

template<typename T>
struct prop_pol
{
	enum{
		bReadMin  = meta_func::rw_count<T>::read < meta_func::rw_count<T>::noread ? 1 : 0,
		bWriteMin = meta_func::rw_count<T>::write < meta_func::rw_count<T>::nowrite ? 1 : 0,
	};
	static int IsPropReadable(DWORD dwProp)
		{return policy<bReadMin>::IsRead(dwProp, (T::endprops::prev*)NULL);}

	static int IsPropWritable(DWORD dwProp)
		{return policy<bWriteMin>::IsWrite(dwProp, (T::endprops::prev*)NULL);}
	
	static int ReadProp(const T* ptr, DWORD dwProp, CValue& val)
		{return ReadProp(ptr, dwProp, val, (T::endprops::prev*)NULL);}

	static int WriteProp(T* ptr, DWORD dwProp, CValue const& val)
		{return WriteProp(ptr, dwProp, val, (T::endprops::prev*)NULL);}

	template<BOOL I/*==TRUE*/>
	struct policy
	{
		template<typename M>
		static int IsRead(DWORD dwProp, M*) // Читаемых меньше, чем не читаемых
		{
			if(M::IsRead && dwProp == M::val)
				return TRUE;
			return IsRead(dwProp, (M::prev*)NULL);
		}
		template<>static int IsRead<first_def>(DWORD, first_def*){return FALSE;}

		template<typename M>
		static int IsWrite(DWORD dwProp, M*) // Записываемых меньше, чем не записываемых
		{
			if(M::IsWrite && dwProp == M::val)
				return TRUE;
			return IsWrite(dwProp, (M::prev*)NULL);
		}
		template<>static int IsWrite<first_def>(DWORD, first_def*){return FALSE;}
	};
	template<>
	struct policy<FALSE>
	{
		template<typename M>
		static int IsRead(DWORD dwProp, M*) // Читаемых больше, чем не читаемых
		{
			if(!M::IsRead && dwProp == M::val)
				return FALSE;
			return IsRead(dwProp, (M::prev*)NULL);
		}
		template<>static int IsRead<first_def>(DWORD, first_def*){return TRUE;}

		template<typename M>
		static int IsWrite(DWORD dwProp, M*) // Записываемых больше, чем не записываемых
		{
			if(!M::IsWrite && dwProp == M::val)
				return FALSE;
			return IsWrite(dwProp, (M::prev*)NULL);
		}
		template<>static int IsWrite<first_def>(DWORD, first_def*){return TRUE;}
	};

	template<typename M>
	static int ReadProp(const T* ptr, DWORD dwProp, CValue& val, M*)
	{
		if(M::IsRead && dwProp==M::val)
			return M::ReadProp(ptr, val);
		return ReadProp(ptr, dwProp, val, (M::prev*)NULL);
	}
	template<>static int ReadProp<first_def>(const T*, DWORD, CValue&, first_def*){return FALSE;}

	template<typename M>
	static int WriteProp(T* ptr, DWORD dwProp, CValue const& val, M*)
	{
		if(M::IsWrite && dwProp==M::val)
			return M::WriteProp(ptr, val);
		return WriteProp(ptr, dwProp, val, (M::prev*)NULL);
	}
	template<>static int WriteProp<first_def>(T*, DWORD, CValue const&, first_def*){return FALSE;}
};

// Стратегии для реализации GetRuntimeClass
template<typename T>
struct implement_rtc
{
	static CObject* PASCAL createobject_rtc(){return new T;}
	static CRuntimeClass* PASCAL GetBaseRTC(){return RUNTIME_CLASS(CBLContext);}
	static CRuntimeClass* GetRTC()
	{
		static CRuntimeClass RTC={"blclass", sizeof(T), 0, createobject_rtc, GetBaseRTC};
		return &RTC;
	}
};

template<typename T>
struct no_implement_rtc
{
	static CRuntimeClass* GetRTC()
	{
		return RUNTIME_CLASS(CBLContext);
	}
};

template<typename T>
struct rtc_selector
{
	template<BOOL I>
	struct sel
	{
		typedef typename implement_rtc<T> type;
	};
	template<>
	struct sel<FALSE>
	{
		typedef typename no_implement_rtc<T> type;
	};
};

// Стратегии поведения при загрузке/выгрузке компоненты

// Не делать ничего
struct no_init_done
{
	static void Init(){}
	static void Done(){}
};

// Зарегистрировать для СоздатьОбъект
template<typename T>
struct reg_CreateObject
{
	static void Init()
	{
		CRuntimeClass* pRtc=T::GetRTC();
		CBLContext::RegisterContextClass(pRtc, T::GetRusTypeString(), CType(100));
		CBLContext::RegisterContextClass(pRtc, T::GetEngTypeString(), CType(100));
	}
	static void Done(){CBLContext::UnRegisterContextClass(T::GetRTC());}
};


// Собственно, сам базовый класс
// T - конечный класс-наследник
// InitDonePolicy - стратегия поведения при инициализации/выгрузке компоненты.
// По умолчанию используется reg_CreateObject, регистрирует класс для СоздатьОбъект.
// Также есть предопределенная стратегия - no_init_done - ничего не делать.
// Можно писать свои стратегии, которые должны реализовывать методы
//	static void Init();
//	static void Done();

template<typename T, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CContextImpl: public CContextBase
{
public:
	typedef T ImplType;
	typedef CContextImpl<T, InitDonePolicy, bImplementRTC> BaseType;
	typedef InitDonePolicy init_done_policy;

	// Реализация RunTimeClass
	static CRuntimeClass* GetRTC()
	{
		return rtc_selector<T>::sel<bImplementRTC>::type::GetRTC();
	}
	virtual CRuntimeClass* GetRuntimeClass()const{return GetRTC();}

	// При создании первого объекта инициализируем карты методов/свойств
	CContextImpl(){static init_all_map iam;}
	virtual ~CContextImpl(){}


	template<typename P> struct end_def {typedef P prev;enum{val=prev::val+1};};
	// В конечном классе эти тайпдефы возможно будут переопределены, указуя на последний метод/свойство
	typedef end_def<first_def> endmeths;
	typedef end_def<first_def> endprops;

	// Реализация виртуальных методов контекста
	virtual int GetNMethods(void)const
		{return T::endmeths::val;}

	virtual int FindMethod(char const *name)const
		{return fndselector<T::endmeths>::type::Find(name);}

	virtual char const* GetMethodName(int nMethod, int nLang)const
		{return getname<T::endmeths>::type::GetName(nMethod, nLang);}

	virtual int HasRetVal(int nMethod)const
		{return meth_pol<T>::HasRetVal(nMethod);}

	virtual int GetNParams(int nMethod)const
		{return meth_pol<T>::GetNParams(nMethod);}

	virtual int GetParamDefValue(int nMethod, int nParam, CValue* pValue)const
		{return meth_pol<T>::GetDefVal(static_cast<const T*>(this), nMethod, nParam, pValue);}

	virtual int CallAsFunc(int nMethod, CValue& retVal,CValue** ppParams)
		{return meth_pol<T>::CallAsFunc(static_cast<T*>(this), nMethod, retVal, ppParams);}

	virtual int CallAsProc(int nMethod, CValue** ppParams)
		{return meth_pol<T>::CallAsProc(static_cast<T*>(this), nMethod, ppParams);}

	virtual int GetNProps(void)const
		{return T::endprops::val;}
	
	virtual char const* GetPropName(int nProp, int nLang)const
		{return getname<T::endprops>::type::GetName(nProp, nLang);}

	virtual int FindProp(char const *name)const
		{return fndselector<T::endprops>::type::Find(name);}
	
	virtual int IsPropReadable(int nProp)const
		{return prop_pol<T>::IsPropReadable(nProp);}

	virtual int GetPropVal(int nProp, CValue& value)const
		{return prop_pol<T>::ReadProp(static_cast<const T*>(this), nProp, value);}

	virtual int IsPropWritable(int nProp)const
		{return prop_pol<T>::IsPropWritable(nProp);}

	virtual int SetPropVal(int nProp, CValue const& value)
		{return prop_pol<T>::WriteProp(static_cast<T*>(this), nProp, value);}

	// Если здесь ошибка, значит, вы забыли вставить в свой класс
	// BL_BEGIN_CONTEXT("EnglishTypeName", "РусскоеИмяТипа");
	virtual char const* GetTypeString(void)const
		{return defLang ? T::GetRusTypeString() : T::GetEngTypeString();}

	// Объект, строящий карты методов и свойств. Прописан как static в конструкторе
	// основного класса, то есть построение карты идет при создании первого объекта
	struct init_all_map
	{
		init_all_map()
		{
			fndselector<T::endmeths>::type::OnInitFirstObject();
			fndselector<T::endprops>::type::OnInitFirstObject();
		}
	};

	static void Init(){init_done_policy::Init();}
	static void Done(){init_done_policy::Done();}
};

template<typename T>
struct variable_arguments
{
	static int GetNParams(DWORD dwMethod, const DWORD& dwCallParams)
	{
		if(dwMethod<T::endmeths::val)
			return _GetNParams(dwMethod, dwCallParams, (T::endmeths::prev*)NULL);
		return -1;
	}
	template<typename M>
	static int _GetNParams(DWORD dwMethod, const DWORD& dwCallParams, M*)
	{
		if(dwMethod==M::val)
		{
			if(0==(M::nParam >> 16))
				return M::nParam;
			else if (dwCallParams== -1)
				return M::nParam & 0xFFFF;
			return dwCallParams > (M::nParam & 0xFFFF) ? dwCallParams : (M::nParam & 0xFFFF);
		}
		return _GetNParams(dwMethod, dwCallParams, (M::prev*)NULL);
	}
	template<>static int _GetNParams<first_def>(DWORD, const DWORD&, first_def*){return -1;}
};

// Базовый класс для контекстов, имеющих в своем составе методы с неопределенным
// количеством аргументов. Для задания метода, имеющего неопределенное количество
// аргументов, в макросах объявления методов используйте для задания количества аргументов
// VRBL_ARG(минимальное количество параметров)
// Например, BL_PROC(Test, "Тест", VRBL_ARG(2));
// В методе-обработчике количество переданных параметров можно считать из m_dwCallParamCount
template<typename T, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CContextVrblArgs: public CContextImpl<T, InitDonePolicy, bImplementRTC>
{
protected:
	mutable DWORD m_dwCallParamCount;
public:
	CContextVrblArgs(){}
	virtual int IsOleContext(void)const
	{
		DWORD regEBX = 0;
		__asm
		{
			mov regEBX, ebx // В этом регистре 1С хранит количество параметров при вызове метода
		}
		m_dwCallParamCount = regEBX;
		return FALSE;
	}

	virtual int GetNParams(int nMethod)const
	{
		return variable_arguments<T>::GetNParams(nMethod, m_dwCallParamCount);
	}
	
};
#define VRBL_ARG(x) (0x10000 | (x))


///////////////////////////////////////////////////////////////////////////////
// Классы для раздельного объявления и реализации контекстов

// Данный класс служит для задания "заголовка" контекста (состава методов и свойств),
// и реализует все методы, связанные с поиском методов/свойств, количеством их параметров,
// типом возвращаемого значения.
// Использование:
// Заголовок контекста -
// template<typename T>
// class CMyContext : public CDeclareCtx<CMyContext>
// {
//		typedef T ImplType;
//		... объявления методов/свойств
// };
// Реализация контекста:
// class CImplMyContext : public CDeclCtxImpl<CImplMyContext, CMyContext<CImplMyContext> ...>
// {
//		... реализация методов контекста.
// }

template<typename T>
class CDeclareCtx : public CContextBase
{
public:
	typedef T DeclType;
	typedef CDeclareCtx<T> BaseDeclType;

	CDeclareCtx() { static init_all_map iam; }
	~CDeclareCtx(){}
	
	
	template<typename P> struct end_def {typedef P prev;enum{val=prev::val+1};};
	// В конечном классе эти тайпдефы возможно будут переопределены, указуя на последний метод/свойство
	typedef end_def<first_def> endmeths;
	typedef end_def<first_def> endprops;
	
	// Реализация виртуальных методов контекста
	virtual int GetNMethods(void)const
		{return T::endmeths::val;}
	
	virtual int FindMethod(char const *name)const
		{return fndselector<T::endmeths>::type::Find(name);}
	
	virtual char const* GetMethodName(int nMethod, int nLang)const
		{return getname<T::endmeths>::type::GetName(nMethod, nLang);}
	
	virtual int HasRetVal(int nMethod)const
		{return meth_pol<T>::HasRetVal(nMethod);}
	
	virtual int GetNParams(int nMethod)const
		{return meth_pol<T>::GetNParams(nMethod);}
	
	virtual int GetNProps(void)const
		{return T::endprops::val;}
	
	virtual char const* GetPropName(int nProp, int nLang)const
		{return getname<T::endprops>::type::GetName(nProp, nLang);}
	
	virtual int FindProp(char const *name)const
		{return fndselector<T::endprops>::type::Find(name);}
	
	virtual int IsPropReadable(int nProp)const
		{return prop_pol<T>::IsPropReadable(nProp);}
	
	virtual int IsPropWritable(int nProp)const
		{return prop_pol<T>::IsPropWritable(nProp);}
	
	// Если здесь ошибка, значит, вы забыли вставить в свой класс
	// BL_BEGIN_CONTEXT("EnglishTypeName", "РусскоеИмяТипа");
	virtual char const* GetTypeString(void)const
		{return defLang ? T::GetRusTypeString() : T::GetEngTypeString();}
	
	// Объект, строящий карты методов и свойств. Прописан как static в конструкторе
	// основного класса, то есть построение карты идет при создании первого объекта
	struct init_all_map
	{
		init_all_map()
		{
			fndselector<T::endmeths>::type::OnInitFirstObject();
			fndselector<T::endprops>::type::OnInitFirstObject();
		}
	};
};

// Шаблон для написания реализации контекста
template<typename T, typename D, typename InitDonePolicy = reg_CreateObject<T>, BOOL bImplementRTC=TRUE>
class CDeclCtxImpl : public D
{
public:
	typedef D DeclType;
	typedef CDeclCtxImpl<T, D, InitDonePolicy, bImplementRTC> BaseType;
	typedef InitDonePolicy init_done_policy;

	// Реализация RunTimeClass
	static CRuntimeClass* GetRTC()
	{
		return rtc_selector<T>::sel<bImplementRTC>::type::GetRTC();
	}
	virtual CRuntimeClass* GetRuntimeClass()const{return GetRTC();}
	
	virtual int GetParamDefValue(int nMethod, int nParam, CValue* pValue)const
		{return meth_pol<T>::GetDefVal(static_cast<const T*>(this), nMethod, nParam, pValue);}
	
	virtual int CallAsFunc(int nMethod, CValue& retVal,CValue** ppParams)
		{return meth_pol<T>::CallAsFunc(static_cast<T*>(this), nMethod, retVal, ppParams);}
	
	virtual int CallAsProc(int nMethod, CValue** ppParams)
		{return meth_pol<T>::CallAsProc(static_cast<T*>(this), nMethod, ppParams);}
	
	virtual int GetPropVal(int nProp, CValue& value)const
		{return prop_pol<T>::ReadProp(static_cast<const T*>(this), nProp, value);}
	
	virtual int SetPropVal(int nProp, CValue const& value)
		{return prop_pol<T>::WriteProp(static_cast<T*>(this), nProp, value);}
};


// Макросы для реализации контекстных объектов.

// Объявить имена типа контекста.
#define BL_BEGIN_CONTEXT(parNameEng, parNameRus)\
	static LPCSTR GetRusTypeString(){return parNameRus;}\
	static LPCSTR GetEngTypeString(){return parNameEng;}\
	template<BOOL bMeth, int I>struct prev_type{typedef typename prev_type<bMeth, I-1>::type type;};\
	template<> struct prev_type<TRUE, __LINE__>{typedef context_obj::first_def type;};\
	template<> struct prev_type<FALSE, __LINE__>{typedef context_obj::first_def type;}

#define BEGIN_STRUCT(parNameE, parNameR, parMeth)\
	struct parNameE##_def\
	{\
		typedef prev_type<parMeth, __LINE__-1>::type prev;\
		enum{val=prev::val+1};\
		static LPCSTR GetEngName(){return #parNameE;}\
		static LPCSTR GetRusName(){return  parNameR;}

#define END_STRUCT(parNameE, parMeth)\
	};friend struct parNameE##_def; template<> struct prev_type<parMeth, __LINE__>{typedef parNameE##_def type;};

#define BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, parCallFunc, parCallProc, parDefProc, parIsFunc, parIsDef)\
	BEGIN_STRUCT(parNameEng, parNameRus, TRUE)\
		static int CallProc(ImplType* p, CValue** pp){return parCallProc;}\
		static int CallFunc(ImplType* p, CValue& r, CValue** pp){return parCallFunc;}\
		static int GetDefVal(const ImplType* p, int n, CValue* v){return parDefProc;}\
		enum{nParam=parNumParam, IsFunc=parIsFunc, IsDefVal=parIsDef};\
		void test(){context_obj::meta_func::check_defval<nParam, IsDefVal>::type t={0};\
			context_obj::meta_func::test_last<endmeths, val>::type t2={0};}\
	END_STRUCT(parNameEng, TRUE)

#define BL_DEFVAL_FOR(parNameEng) int parNameEng##_GetDefVal(int nParam, CValue* pValue)const

#define COMMA ,

#define BL_PROC_(parNameEng,parNameRus,parNumParam, parDefVal, parIsDef)\
	BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, FALSE, p->parNameEng(pp), parDefVal, 0, parIsDef)\
	int parNameEng(CValue** ppParams)

#define BL_FUNC_(parNameEng,parNameRus,parNumParam, parDefVal, parIsDef)\
	BL_METHOD_DEF(parNameEng, parNameRus, parNumParam, p->parNameEng(r COMMA pp), FALSE, parDefVal, 1, parIsDef)\
	int parNameEng(CValue& retVal, CValue** ppParams)

// Объявить метод - процедуру контекста, без параметров по умолчанию
#define BL_PROC(parNameEng, parNameRus, parNumParam) \
	BL_PROC_(parNameEng, parNameRus, parNumParam, FALSE, 0)

// Объявить метод - процедуру контекста, имеющую параметры по умолчанию
#define BL_PROC_WITH_DEFVAL(parNameEng,parNameRus,parNumParam) \
	BL_PROC_(parNameEng, parNameRus, parNumParam, p->parNameEng##_GetDefVal(n COMMA v), 1)

// Объявить метод - функцию контекста, без параметров по умолчанию
#define BL_FUNC(parNameEng, parNameRus, parNumParam) \
	BL_FUNC_(parNameEng, parNameRus, parNumParam, FALSE, 0)

// Объявить метод - функцию контекста, имеющую параметры по умолчанию
#define BL_FUNC_WITH_DEFVAL(parNameEng,parNameRus,parNumParam) \
	BL_FUNC_(parNameEng, parNameRus, parNumParam, p->parNameEng##_GetDefVal(n COMMA v), 1)

#define BL_PROP_(parNameE, parNameR, parGet, parSet, parR, parW)\
	BEGIN_STRUCT(parNameE, parNameR, FALSE)\
		static int ReadProp(const ImplType* p, CValue& v){return parGet;}\
		static int WriteProp(ImplType* p, CValue const& v){return parSet;}\
		enum{IsRead=parR, IsWrite=parW};\
		void test(){context_obj::meta_func::test_last<endprops, val>::type t2={0};}\
	END_STRUCT(parNameE, FALSE)

#define BL_PROP_READ(parName)	int Get_##parName(CValue& value)const
// Объявить обработчик записи свойства для свойства с чтением/записью
#define BL_PROP_WRITE(parName)	int Set_##parName(CValue const& value)

// Объявить свойство контекста, только для чтения
#define BL_PROP_RO(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, p->Get_##parNameEng(v), FALSE, 1, 0)\
	BL_PROP_READ(parNameEng)

// Объявить свойство контекста, только для записи
#define BL_PROP_WO(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, FALSE, p->Set_##parNameEng(v), 0, 1)\
	BL_PROP_WRITE(parNameEng)

// Объявить свойство контекста, чтение/запись
#define BL_PROP_RW(parNameEng, parNameRus)\
	BL_PROP_(parNameEng, parNameRus, p->Get_##parNameEng(v), p->Set_##parNameEng(v), 1, 1)\
	BL_PROP_READ(parNameEng)


// Объявить окончание методов и свойств контекста
#define BL_END_CONTEXT()\
	typedef end_def<prev_type<TRUE,  __LINE__>::type> endmeths;\
	typedef end_def<prev_type<FALSE, __LINE__>::type> endprops


// Реализовать авто-инициализацию класса контекста
#define BL_INIT_CONTEXT(parClassName)\
	__declspec (allocate(".blc$b"))\
	context_obj::P_VV init##parClassName = &parClassName::Init;\
	__declspec (allocate(".blc$d"))\
	context_obj::P_VV done##parClassName = &parClassName::Done

}//namespace context_obj
using context_obj::CContextImpl;
using context_obj::CContextVrblArgs;
using context_obj::no_init_done;
