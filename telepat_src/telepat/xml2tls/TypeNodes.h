// TypeNodes.h
#ifndef TYPENODES_H
#define TYPENODES_H
/*
Структура бинарного файла хранения информации о типах.
Файл tls есть плоский zip, созданный с помощью zlibeng
После распаковки имеет структуру:
DWORD - количество строковых записей.
DWORD - количество типов
DWORD - количество атрибутов
DWORD - количество методов
DWORD - количество параметров
DWORD - количество допбаз
DWORD - количество доптиповметодов
DWORD - количество доптипов параметров
DWORD - количество создаваемыхтипов
строки
типы
атрибуты
методы
параметры
допбазы
доптипыметодов
доптипыпараметров
создаваемыетипы

Далее идут строковые записи, разделенные 0.
Далее записываются узлы дерева, начиная с корневого.
Порядок обхода дерева: узел, потомок, следующий.
При отсутсвии потомка или следующего записывается один байт 0.
Формат записи узла:
BYTE - тип узла
Данные узла (зависит от типа узла)

type
	name,
	alias,
	abstract,	8 bit
	basecnt,	8 bit
	atrcount,	8 bit
	metcnt,		8 bit
	attribs,
	methods,
	bases,

attrib
	parenttype,
	name,
	alias,
	descr,
	typename,
	flags - InExprOnly,

method
	parenttype
	name,
	alias,
	descr,
	retdescr,
	flags - proc, func, funcinexpr, handler,
	paramcnt,		16 bit
	rettypecnt,		16 bit
	params,
	rettype

param
	name,
	default,
	descr,
	typecnt
	type

creatables
	type-name
bases
	type


Подробнее см. сорцы
*********************************************************************
Размер блока памяти рассчитывается занимаемый при загрузке Телепатом.
Обычно данные - номера записей в массиве строк.
*********************************************************************
Данные загружаются в Телепат блоком, те сразу выделяется память
для всех узлов, в которую загружаются все узлы, последовательно
друг за другом. Освобождение памяти тоже происходит разом для всего
блока. Поэтому применять delete к одному узлу низя.
*/

/*
Структура хранения узлов дерева.
Дерево типов состоит из отдельных узлов.
Каждый узел имеет свой тип, записанный в первом байте.
А также имеет данные, записанные сразу после себя.
*/
#pragma pack(push,1)

struct STypeInfo;
struct SAttrInfo;
struct SMethInfo;
struct SParaInfo;

struct SHeadType
{
	DWORD StrCount;		// количество строковых записей.
	DWORD StrSize;		// размер строк
	DWORD TypeCount;	// количество типов
	DWORD AttrCount;	// количество атрибутов
	DWORD MethCount;	// количество методов
	DWORD ParaCount;	// количество параметров
	DWORD AddBaseCount;	// количество допбаз
	DWORD AddRetCount;	// количество доптиповметодов
	DWORD AddPTypeCount;// количество доптипов параметров
	DWORD CrObCount;	// количество создаваемыхтипов
};

struct STypeInfo
{
	DWORD		Name;		// Внутреннее имя типа
	DWORD		Alias;		// Пользовательское имя типа
	BYTE		Flags;		// 0x1 - Тип абстрактный
	BYTE		BaseCnt;	// Количество базовых типов
	BYTE		AttrCnt;	// Количество атрибутов
	BYTE		MethCnt;	// Количество методов
	DWORD		pBases;		// Базовые классы (если BaseCnt==0, то непосредственно указатель на базовый тип, иначе указатель на массив указателей на базовые типы)
	BYTE		Components;	// Используется в компонентах
};

struct STTypeInfo;
struct STAttrInfo;
struct STMethInfo;

struct STTypeInfo
{
	DWORD		Name;		// Внутреннее имя типа
	DWORD		Alias;		// Пользовательское имя типа
	BYTE		Flags;		// 0x1 - Тип абстрактный
	BYTE		BaseCnt;	// Количество базовых типов
	BYTE		AttrCnt;	// Количество атрибутов
	BYTE		MethCnt;	// Количество методов
	STTypeInfo*	pBases;		// Базовые классы (если BaseCnt==0, то непосредственно указатель на базовый тип, иначе указатель на массив указателей на базовые типы)
	STAttrInfo*	pAttribs;	// Атрибуты
	STMethInfo*	pMethods;	// Методы
};

struct SAttrInfo
{
	BYTE		Flags;	// 0x1 - InExprOnly
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		strTypeName;
	BYTE		Components;
};

struct STAttrInfo
{
	BYTE		Flags;	// 0x1 - InExprOnly
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		strTypeName;
	STTypeInfo*	ParentType;
};

struct SMethInfo
{
	BYTE		flags;// 0x1 func, 0x2 - funcinexpr, 0x4 - handler
	BYTE		ParamCnt;
	BYTE		retTypeCnt;
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		retDescr;
	DWORD		strRetTypeName;
	BYTE		Components;
};

struct STMethInfo
{
	BYTE		flags;// 0x1 func, 0x2 - funcinexpr, 0x4 - handler
	BYTE		ParamCnt;
	BYTE		retTypeCnt;
	DWORD		NameRus;
	DWORD		NameEng;
	DWORD		Descr;
	DWORD		retDescr;
	DWORD		strRetTypeName;
	STTypeInfo*	ParentType;
	SParaInfo*	pParams;
};

struct SParaInfo
{
	DWORD	Name;
	DWORD	DefVal;
	DWORD	Descr;
	BYTE	TypeCnt;
	DWORD	strTypeName;
};

#ifdef LOAD_XML_TYPE

enum NodeTypes
{
	ndSimple=1,
	ndType,
	ndAttribs,
	ndAttrib,
	ndAttribInExpr,
	ndMethods,
	ndProc,
	ndFunc,
	ndFuncInExpr,
	ndParams,
	ndParam,
	ndHandlers,
	ndReturn,
	ndLast
};

// Базовый класс узла дерева типов
struct SNode
{
	SNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:nodeType(t),parent(p),next(n),child(c){}
	NodeTypes nodeType;		// тип узла
	// связи узлов
	SNode* parent;
	SNode* next;
	SNode* child;

	virtual ~SNode(){if(child)delete child;if(next)delete next;}
	//// Работа с узлами при загрузке из xml и конвертации в формат Телепата
	// Создание узла по типу тега
	static SNode* CreateNode(SNode* pParent,LPCTSTR xmlName);
	// Загрузка узла из xml-данных
	// Считывает общие данные, вызывает чтение частных данных,
	// считывает потомков и следующий
	static SNode* LoadXMLNodeWithChild(SNode* pParent,IXMLDOMNodePtr& item);
	// Сохранение узла с потомками в бинарный файл
	// Записывает общие данные, потомка, следующий
	void SaveNodeWithChild();
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-ошибка загрузки, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны, -1 - узел загружен, но не включается в дерево
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item){return 1;}
	// Установка текстовых данных узла при их обнаружении в
	// процессе обхода его потомков
	virtual DWORD* WriteTextData(){return NULL;}
	// пометка используемых строк и вычисление размера данных
	// в режиме телепата
	virtual void SetUsedStrings(DWORD* pStrings){}
	// Корректировка номеров строк
	virtual void CorrectStringNum(DWORD* pNumbers){}
	// Вызывается при записи данных узла
	virtual void SaveNodeData(){}
};
/*
Базовый узел для элементов, зависящих от состава компонент
*/
struct SComponentsNode:public SNode
{
	SComponentsNode(NodeTypes t=ndSimple, SNode* p=NULL, SNode* n=NULL, SNode* c=NULL)
		:SNode(t,p,n,c){}
	BYTE UsedComponents;
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	void LoadComponents(IXMLDOMNodePtr& item);
};

/*
Узел представляющий тип
*/
struct STypeNode:public SComponentsNode
{
	// Данные о типе
	DWORD Name;			// Внутреннее имя типа
	DWORD Alias;		// Пользовательское имя типа
	DWORD Abstract;		// Тип абстрактный
	DWORD cntCreatable;	// Количество имен для СоздатьОбъект
	DWORD cntBase;		// Количество базовых классов
	DWORD*	Creatable;		// Номера строк с именами, используемыми в СоздатьОбъект
	DWORD*	baseTypes;		// Номера строк имен базовых типов

	STypeNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c),Name(0),Alias(0),baseTypes(NULL),Creatable(NULL){}
	~STypeNode(){if(baseTypes)delete[]baseTypes;if(Creatable)delete[]Creatable;}
	// Вызывается при записи данных узла
	virtual void SaveNodeData();
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// Вычисление размера данных в файле и пометка используемых строк
	virtual void SetUsedStrings(DWORD* pStrings);
	// Корректировка номеров строк
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
Узел представляющий атрибут
type - ndAttrib | ndAttribInExpr
*/
struct SAtributNode:public SComponentsNode
{
	SAtributNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c){NameRus=NameEng=Description=0;}
	// Данные об атрибуте
	DWORD NameRus;
	DWORD NameEng;
	DWORD Description;
	DWORD Type;
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// Вызывается при записи данных узла
	virtual void SaveNodeData();
	// Установка текстовых данных узла при их обнаружении в
	// процессе обхода его потомков
	virtual DWORD* WriteTextData(){return &Description;}
	// Вычисление размера данных в файле и пометка используемых строк
	virtual void SetUsedStrings(DWORD* pStrings);
	// Корректировка номеров строк
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
Узел представляющий метод
type - ndProc | ndFunc | ndFuncInExpr
*/
struct SMethodNode:public SComponentsNode
{
	// Описание метода
	DWORD NameRus;			// Номер русского имени
	DWORD NameEng;			// Номер английского имени
	DWORD Description;		// Номер описания
	DWORD RetDescription;	// Номер описания возвращаемого значения
	DWORD RetTypesCount;	// Количество возвращаемых типов
	DWORD* RetTypes;

	SMethodNode(NodeTypes t=ndSimple,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SComponentsNode(t,p,n,c),RetTypesCount(0),RetTypes(NULL){NameRus=NameEng=Description=RetDescription=0;}
	~SMethodNode(){if(RetTypes)delete [] RetTypes;}
	// Вызывается при записи данных узла
	virtual void SaveNodeData();
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// Установка текстовых данных узла при их обнаружении в
	// процессе обхода его потомков
	virtual DWORD* WriteTextData(){return &Description;}
	// Вычисление размера данных в файле и пометка используемых строк
	virtual void SetUsedStrings(DWORD* pStrings);
	// Корректировка номеров строк
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
Узел, представляющий параметр
*/
struct SParamNode:public SNode
{
	// Описание параметра
	DWORD Name;
	DWORD Default;
	DWORD Description;
	DWORD TypesCount;
	DWORD* Types;

	SParamNode(NodeTypes t=(NodeTypes)0,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SNode(t,p,n,c),TypesCount(0){Name=Default=Description=0;}
	// Вызывается при записи данных узла
	virtual void SaveNodeData();
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
	// Установка текстовых данных узла при их обнаружении в
	// процессе обхода его потомков
	virtual DWORD* WriteTextData(){return &Description;}
	// Вычисление размера данных в файле и пометка используемых строк
	virtual void SetUsedStrings(DWORD* pStrings);
	// Корректировка номеров строк
	virtual void CorrectStringNum(DWORD* pNumbers);
};

/*
Узел для return - используется при загрузке из XML
Записывает данные в узел родительского метода
*/
struct SReturnNode:public SNode
{
	SReturnNode(NodeTypes t=(NodeTypes)0,SNode* p=NULL,SNode* n=NULL,SNode* c=NULL)
		:SNode(t,p,n,c){}
	// Вызывается при чтении данных узла из xml-данных
	// Возвращает 0-узел не попадет в дерево, 1-узел считан, обойти потомков,
	// 2 - не обходить потомков, уже считаны.
	virtual int LoadNodeXMLData(IXMLDOMNodePtr& item);
};
#endif

#pragma pack(pop)

#endif