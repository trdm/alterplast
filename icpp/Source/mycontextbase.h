// mycontextbase.h
// Заголовочный файл для базового класса, реализующего функциональность CBLContext'а
// Поддерживается:
// - авторегистрация конечного класса в CBLContext::RegisterContextClass
// - автовызов инициализирующей функции при первой регистрации класса
// - работа с картой методов, быстрый поиск номера метода, перенаправление
//		вызова метода в обработчик, заданный в карте
// - работа с картой постоянных свойств, быстрый поиск номера свойства,
//		установка/чтение свойства через обработчики, заданные в карте.
// - поддержка функций с неопределенным числом параметров
// Ограничения:
// - должен быть первым в списке наследования вашего класса.

#ifndef MYCONTEXTBASE_H
#define MYCONTEXTBASE_H
#include "blmap.h"
#include "blptr.h"

typedef int (CBLContext::*PF_GET_PROP)(CValue&)const;
typedef int (CBLContext::*PF_SET_PROP)(CValue const&);
typedef int (CBLContext::*PF_CALL_AS_PROC)(CValue**);
typedef int (CBLContext::*PF_CALL_AS_FUNC)(CValue&,CValue**);
typedef int (CBLContext::*PF_GET_PARAM_DEF)(int,CValue*)const;
//typedef int (PF_CTOR_PARAM_DEF)(int,CValue*);

typedef int (CBLContext::*PF_CALL_AS_PROC_WITH_UN_LIM_PARAMS)(int nParamsCount, CValue**);
typedef int (CBLContext::*PF_CALL_AS_FUNC_WITH_UN_LIM_PARAMS)(int nParamsCount, CValue&, CValue**);

// Собственно, сам базовый класс
class CMyContextBase : public CBLContext
{
public:
	CMyContextBase();
	virtual ~CMyContextBase();  
	virtual int  CallAsFunc(int,CValue&,CValue**);
	virtual int  CallAsProc(int,CValue**);
	virtual int  FindMethod(char const *)const;
	virtual int  FindProp(char const *)const;
    //virtual char const*  GetCode(void)const;
	virtual int  GetDestroyUnRefd(void)const;
	//virtual CObjID GetID(void)const;
	virtual char const*  GetMethodName(int,int)const;
	virtual int  GetNMethods(void)const;
	virtual int  GetNParams(int)const;
	virtual int  GetNProps(void)const;
	virtual int  GetParamDefValue(int,int,CValue*)const;
	virtual char const* GetPropName(int,int)const;
	virtual int  GetPropVal(int,CValue&)const;
	//virtual long  GetTypeID(void)const;
	virtual void InitObject(char const *) {}
	virtual void InitObject(CType const &) {}
	virtual char const* GetTypeString(void)const;
	//virtual CType GetValueType(void)const;
	virtual int  HasRetVal(int)const;
	virtual int  IsExactValue(void)const;
	virtual int  IsOleContext(void)const;
	virtual int  IsPropReadable(int)const;
	virtual int  IsPropWritable(int)const;
	virtual int  IsSerializable(void);
	virtual int  SaveToString(CString &);
	virtual int  SetPropVal(int,CValue const&);

	virtual struct _S_MyContextBaseInfo* GetBaseInfo()const=0;
	static void InitAllContextClasses();
	static void DoneAllContextClasses();

    virtual BOOL _Create(CValue** ppValue) {return TRUE;};
};

// Разное вспомогательное

// Структра одной записи в карте методов
struct _S_MethDef
{
	const char *Names[2];	// Имена метода
	DWORD NumberOfParam;
	PF_CALL_AS_PROC pDoProc;
	PF_CALL_AS_FUNC pDoFunc;
	PF_GET_PARAM_DEF pDefValProc;

	bool bIsUnLimParMethod; // функция с неопределенным количеством параметров
	PF_CALL_AS_PROC_WITH_UN_LIM_PARAMS pDoProcWithUnLimParams;
	PF_CALL_AS_FUNC_WITH_UN_LIM_PARAMS pDoFuncWithUnLimParams;
};

// Структура одной записи в карте свойств
struct _S_PropDef
{
	const char *Names[2];
	PF_GET_PROP pGetProp;
	PF_SET_PROP pSetProp;
};

// Структура информации о классе-контексте
typedef void (*PVVF)();

struct _S_MyContextBaseInfo
{
	// Параметры, жестко задаваемые при компиляции
	const _S_MethDef* pMethodDef;	// Карта методов
	const _S_PropDef* pPropDef;		// Карта свойств
	const char* EngTypeString;		// Имя типа английское
	const char* RusTypeString;		// Имя типа русское
	BOOL RegToCreate;				// Регистрировать имена для создания объектов
	const CRuntimeClass* pRTC;		// Для регистрации как создаваемого
	int methCount;					// Количество методов
	int propCount;					// Количество свойств
	PVVF pInitFunc;					// Указатель на функцию-инициализатор
	PVVF pDoneFunc;					// Указатель на функцию-инициализатор
    int nFactoryCtorParamCount;     // Количество параметров конструктора фабрики
    //PF_CTOR_PARAM_DEF pCtorParamDef;// Указатель на функцию получения значения параметра по-умолчанию для конструктора фабрики

	// artbear
	int m_CountParametersInCallMethod; // количество параметров в методе, который вызываем

	// Параметры, создаваемые и заполняемые при инициализации
	CBLMap* pMethMap;	// Быстрый поиск номера метода
	CBLMap* pPropMap;	// Быстрый поиск номера свойства
};

// Макрос для стандартного объявления класса-контекста
#define DECLARE_MY_CONTEXT() \
public: \
	static _S_MyContextBaseInfo		m_myCtxBaseInfo[]; \
	static const _S_MethDef			m_MethDef[]; \
	static const _S_PropDef			m_PropDef[]; \
	static _S_MyContextBaseInfo* const* const m_ppForcedRef; \
	virtual _S_MyContextBaseInfo* GetBaseInfo() const { return *m_ppForcedRef; } \

// макрос для определения данных создаваемого класса-контекста
#define IMPLEMENT_MY_CONTEXT(parClassName, parEngTypeString, parRusTypeString, parRegToCreate, parInitFunc, parDoneFunc, parFactoryCtorParamCount) \
	_S_MyContextBaseInfo parClassName::m_myCtxBaseInfo[]= \
	{\
		parClassName::m_MethDef, \
		parClassName::m_PropDef, \
		parEngTypeString, \
		parRusTypeString, \
		parRegToCreate, \
		RUNTIME_CLASS(parClassName), \
		sizeof(parClassName::m_MethDef)/sizeof(_S_MethDef) - 1, \
		sizeof(parClassName::m_PropDef)/sizeof(_S_PropDef) - 1, \
		parInitFunc, \
		parDoneFunc, \
        parFactoryCtorParamCount,\
		-1,\
		NULL, NULL \
	};\
	IMPLEMENT_DYNCREATE(parClassName, CBLContext)\
	__declspec (allocate(".blc$b"))\
	_S_MyContextBaseInfo* const init##parClassName = parClassName::m_myCtxBaseInfo; \
	_S_MyContextBaseInfo* const* const parClassName::m_ppForcedRef = &init##parClassName;

// 2 макроса для упрощенного определения без лишних параметров по умолчанию
#define IMPLEMENT_MY_CREATE_CONTEXT(parClassName, parEngTypeString, parRusTypeString) \
	IMPLEMENT_MY_CONTEXT(parClassName, parEngTypeString, parRusTypeString, 1, NULL, NULL, -1)

#define IMPLEMENT_MY_NONCREATE_CONTEXT(parClassName, parEngTypeString, parRusTypeString) \
	IMPLEMENT_MY_CONTEXT(parClassName, parEngTypeString, parRusTypeString, 0, NULL, NULL, -1)

#define BEGIN_BL_METH_MAP(parClassName) \
	const _S_MethDef parClassName::m_MethDef[]={

// макросы для определения методов
#define BL_METH(parNameEng,parNameRus,parNumberOfParam,parDoProc,parDoFunc,parDefValProc) \
	{{parNameEng,parNameRus},parNumberOfParam,static_cast<PF_CALL_AS_PROC>(parDoProc),static_cast<PF_CALL_AS_FUNC>(parDoFunc),static_cast<PF_GET_PARAM_DEF>(parDefValProc), false, NULL, NULL},

// 4 макроса для упрощенного определения без лишних параметров по умолчанию
#define BL_METH_PROC(parNameEng,parNameRus,parNumberOfParam,parDoProc) \
	{{parNameEng,parNameRus},parNumberOfParam,static_cast<PF_CALL_AS_PROC>(parDoProc), NULL, NULL, false, NULL, NULL},

#define BL_METH_FUNC(parNameEng,parNameRus,parNumberOfParam,parDoFunc) \
	{{parNameEng,parNameRus},parNumberOfParam, NULL, static_cast<PF_CALL_AS_FUNC>(parDoFunc), NULL, false, NULL, NULL},

#define BL_METH_PROC_DEF_PARAM(parNameEng,parNameRus,parNumberOfParam,parDoProc,parDefValProc) \
	{{parNameEng,parNameRus},parNumberOfParam,static_cast<PF_CALL_AS_PROC>(parDoProc), NULL, static_cast<PF_GET_PARAM_DEF>(parDefValProc), false, NULL, NULL},

#define BL_METH_FUNC_DEF_PARAM(parNameEng,parNameRus,parNumberOfParam,parDoFunc,parDefValProc) \
	{{parNameEng,parNameRus},parNumberOfParam, NULL, static_cast<PF_CALL_AS_FUNC>(parDoFunc), static_cast<PF_GET_PARAM_DEF>(parDefValProc), false, NULL, NULL},

// artbear
// 2 макроса для определения методов с неопределенным числом параметров
// parNumberOfParam - число обязательных параметров
// если 0, значит, постоянных параметров нет, и функция без параметров
//
#define BL_METH_PROC_WITH_UN_LIM_PARAMS(parNameEng,parNameRus,parNumberOfParam,parDoProc) \
	{{parNameEng,parNameRus},parNumberOfParam,NULL, NULL, NULL, true, static_cast<PF_CALL_AS_PROC_WITH_UN_LIM_PARAMS>(parDoProc), NULL},

#define BL_METH_FUNC_WITH_UN_LIM_PARAMS(parNameEng,parNameRus,parNumberOfParam,parDoFunc) \
	{{parNameEng,parNameRus},parNumberOfParam, NULL, NULL, NULL, true, NULL, static_cast<PF_CALL_AS_FUNC_WITH_UN_LIM_PARAMS>(parDoFunc)},

#define END_BL_METH_MAP() {{NULL,NULL},0,0,NULL,NULL}};

// макросы для определения свойств
#define BEGIN_BL_PROP_MAP(parClassName) \
	const _S_PropDef parClassName::m_PropDef[]={

#define BL_PROP(parNameEng,parNameRus,parGet,parSet) \
	{{parNameEng,parNameRus},static_cast<PF_GET_PROP>(parGet),static_cast<PF_SET_PROP>(parSet)},

#define END_BL_PROP_MAP() {{NULL,NULL},NULL,NULL}};

// artbear макрос для класса без свойств
#define BEGIN_EMPTY_BL_PROP_MAP(parClassName) \
	BEGIN_BL_PROP_MAP(parClassName) \
	END_BL_PROP_MAP()

#define IS_BLTYPE(Value, class_name) (Value.GetTypeCode() == AGREGATE_TYPE_1C && Value.GetContext()->GetRuntimeClass() == RUNTIME_CLASS(##class_name))

#define CHECK_BLTYPE(Value, class_name)\
    if (!IS_BLTYPE(Value, class_name))\
    {\
        CBLModule::RaiseExtRuntimeError("Недопустимое значение свойства.", 0);\
        return FALSE;\
    }

#define GET_ENUM_PROP(type, first, last)\
    type _enum = (type)Value.GetNumeric().operator long();\
    if (_enum < first || _enum > last)\
    {\
        CBLModule::RaiseExtRuntimeError("Недопустимое значение свойства.", 0);\
        return FALSE;\
    }

// определить свойство-массив для класса
#define DECLARE_PROPERTY_ARRAY(PropertyName, PropertyType, PropertyKeyType) \
	__declspec(property(get=get_##PropertyName, put=put_##PropertyName)) PropertyType PropertyName []; \
	BOOL get_##PropertyName(PropertyKeyType key);  \
	void put_##PropertyName(PropertyKeyType key, PropertyType newValue);

// определить обычное свойство для класса
#define DECLARE_ONE_PROPERTY(Property, PropertyType) \
	__declspec(property(get=get_##Property, put=put_##Property)) PropertyType Property; \
	BOOL get_##Property();  \
	void put_##Property(PropertyType newValue);

// Заранее объявим имя сегмента, чтобы работало __declspec allocate
#pragma data_seg(".blc$b")
#pragma data_seg()

// Укажем линкеру в конечном файле слить сегменты .blc и .data
#pragma comment(linker, "/MERGE:.blc=.data")

#endif