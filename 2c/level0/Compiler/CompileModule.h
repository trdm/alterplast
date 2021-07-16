// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// CompileModule.h: interface for the CCompileModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_)
#define AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TranslateModule.h"
#include "SystemFunctions.h"
#include "StringExt.h"


typedef CArray<CValue,CValue&> CDefValueList;
typedef CArray<bool,bool> CDefBoolList;
//typedef CArray<int,int> CDefIntList;
typedef CUIntArray CDefIntList;



//Свойства функций:
enum
{
	RETURN_NONE=0,	//нет возврата (код модуля)
	RETURN_PROCEDURE,//возврат из процедуры
	RETURN_FUNCTION,//возврат из функции
};

//признаки переменных (задаются с отриц. значением в атрибут nArray байт-кода)
enum
{
	DEF_VAR_SKIP=-1,//пропущенный параметр
	DEF_VAR_DEFAULT=-2,//параметр по умолчанию
	DEF_VAR_TEMP=-3,//признак временной локальной переменной
	DEF_VAR_NORET=-7,//функция (процедура) не возвращает значения
	DEF_VAR_CONST=1000,//загрузка констант


};

//____________________________________________________________________________
struct SParamRun
{
public:
	SParamRun()
	{
		nArray=0;
		nIndex=0;
	}
	int nArray;
	int nIndex;
};
//____________________________________________________________________________
struct SParam : public SParamRun
{
public:
	CString csType;//тип переменной в англ. нотации (в случае явной типизации)
};

//____________________________________________________________________________
class CVariable
{
public:
	CVariable()
	{
		bExport=0;
		bExist=0;
		nNumber=0;
	};
	BOOL bExist;
	BOOL bExport;
	int nNumber;
	CString csName;//Имя переменной
	CString csType;//тип значения
};//CVariable


//____________________________________________________________________________
//хранение одного шага программы
class CByte
{
public:
	CByte() 
	{
		//ZeroMemory(this,sizeof(CByte)-sizeof(CString));
		nOper=0;
		nStringNumber=0;
		nNumberLine=0;
	}
	//код инструкции:
	int	nOper;
	int	nStringNumber;	//номер исходного текста (для вывода ошибок)
	int	nNumberLine;	//номер строки исходного текста (для точек останова)

	//параметры для инструкции:
	struct SParamRun Param1;
	struct SParamRun Param2;
	struct SParamRun Param3;
	struct SParamRun Param4;// - используется для оптимизации
	CString csModuleName;//имя модуля (т.к. возможны include подключения из разных модулей)
};

//____________________________________________________________________________
//класс БАЙТ-КОДА
class CByteCode
{
public:
	CByteCode()
	{
		CodeList.SetSize(0,64);
		Clear();
	};
	//Атрибуты условного компилирования
	BOOL	bOptimize1;
	BOOL	bOptimize2;
	BOOL	bOptimize3;

	//Атрибуты
	CArray <CByte,CByte&>	CodeList;//исполняемый код модуля
	CArray <CValue,CValue&>	ConstList;//список констант модуля
	CMapStringToPtr			FuncList;//список функций модуля
	CMapStringToPtr			VarList;//список переменных модуля
	CMapStringToPtr			ExportVarList;//список экспортных переменных модуля
	CMapStringToPtr			ExportFuncList;//список экспортных функций модуля
	CArray <CString,CString&>aExportList;//массивэкспортных функций и переменных
	int nVarCount;		//количество локальных переменных в модуле
	int nStartModule;	//начало позиции старта модуля
	CString csModuleName;//имя исполняемого модуля, которому принадлежит байт-код
	CString csModule;	//код исполняемого модуля
	BOOL bCompile;		//признак успешной компиляции
	//int nCurLine;		//текущая исполняемая строка байт-кода
	CByteCode *pParent;	//Родительский байт-код (для проверки)

	class CCompileModule *pModule;//обратная связь с компилирующим классом (т.к. он содержит доп. инфу нужную для работы функции Шаблон)

	//список внешних переменных
	CArray <CValue *,CValue *> aExternValue;


	void SetModule(class CCompileModule *pSetModule){pModule=pSetModule;};


	void Clear()
	{
		bOptimize1=0;
		bOptimize2=0;
		bOptimize3=0;
		pParent=0;
		bCompile=0;
		nStartModule=0;
		nVarCount=0;
		CodeList.RemoveAll();
		ConstList.RemoveAll();
		aExternValue.RemoveAll();
		FuncList.RemoveAll();
		VarList.RemoveAll();
		ExportVarList.RemoveAll();
		ExportFuncList.RemoveAll();
		aExportList.RemoveAll();
	};
	void Save(CString csFileName);
	void Load(CString csFileName);
};

//____________________________________________________________________________
struct SLabel
{
public:
	CString	csName;
	int		nLine;
	int		nError;
};

#define VAR_LOCAL	1
#define VAR_EXPORT	2
#define VAR_NUMBER	10
#define VAR_STRING	11
#define VAR_DATE	12
#define VAR_ARRAY	13

/******************************************************************
Класс: контекст компиляции  
*******************************************************************/
class CCompileContext
{
public:
	CCompileContext(CCompileContext *hSetParent=0)
	{
		pParent=hSetParent;
		nDoNumber=0;
		nReturn=0;
		nFindLocalInParent=1;
		pModule=0;

		pStopParent=0;
		pContinueParent=0;
		if(hSetParent)
		{
			pStopParent=hSetParent->pStopParent;
			pContinueParent=hSetParent->pContinueParent;
		}
		nTempVar=0;
		bStaticVariable=0;
	};
	~CCompileContext();

	class CCompileModule *pModule;
	void SetModule(class CCompileModule *pSetModule){pModule=pSetModule;};


	CCompileContext *pParent;//родительский контекст
	CCompileContext *pStopParent;//начало запрещенной области прародителя
	CCompileContext *pContinueParent;//начало разрешенной области прародителя
	BOOL bStaticVariable;		//все переменные статичные

	//ПЕРЕМЕННЫЕ
//	CMapStringToPtr cVariablesDef;	//объявления
//	CMapStringToPtr cVariables;		//список встретившихся использований переменных
	CMap <class CStringExt ,class CStringExt ,CVariable ,CVariable&> cVariables;
	BOOL FindVariable(CString csName);
	SParam GetVariable(CString csName,bool bFindInParent=1,bool bCheckError=0);
	SParam AddVariable(CString csName,CString csType="",BOOL bExport=0);
	int nTempVar;//номер текущей временной переменной

	int nFindLocalInParent;//признак поиска переменных в родителе (на один уровень), в остальных случаях в родителях ищутся только экспортные переменные)


	//ФУНКЦИИ И ПРОЦЕДУРЫ
	CMapStringToPtr		cFunctions;//список встретившихся определений функций
	int nReturn;//режим обработки оператора RETURN : RETURN_NONE,RETURN_PROCEDURE,RETURN_FUNCTION
	CString csCurFuncName;//имя текущей компилируемой функции (для обработки варианта вызова рекурсивной функции)

	//ЦИКЛЫ
	//Служебные атрибуты
	int nDoNumber;//номер вложенного цикла
	CMapWordToPtr   aContinueList;//адреса операторов Continue
	CMapWordToPtr   aBreakList;//адреса операторов Break

	//Установка адресов перехода для команд Continue и Break
	void StartDoList();
	void FinishDoList(CByteCode	&cByteCode,int nGotoContinue,int nGotoBreak);


	//МЕТКИ
	CMapStringToPtr cLabelsDef;	//объявления
	CArray <SLabel,SLabel> cLabels;	//список встретившихся переходов на метки
	void DoLabels();
};//CCompileContext



//____________________________________________________________________________
class CParamVariable
{
public:
	CParamVariable()
	{
		bByRef=1;
		vData.nArray=-1;
		vData.nIndex=-1;
	};
	CString csName;//Имя переменной
	SParam 	vData;//Значение по умолчанию
	int	bByRef;
	CString csType;//тип значения
};//CParamVariable


//____________________________________________________________________________
//определение функции
class CFunction
{
public:
	CFunction(CString csFuncName,CCompileContext *pSetContext)
	{
		csName=csFuncName;
		pContext=pSetContext;
		bExport=0;
		bVirtaul=0;
		nVarCount=0;
		nStart=0;
		nFinish=0;
		bSysFunction=0;
		RealRetValue.nArray=0;
		RealRetValue.nIndex=0;
		nNumberLine=-1;
	};
	~CFunction()
	{
		if(pContext)//Удаляем подчиненный контекст (у каждой функции свой список меток и локальных переменных)
			delete pContext;
	};
	CString csRealName;//Имя функции
	CString csName;//Имя функции в верхнем регистре
	CArray<CParamVariable,CParamVariable> aParamList;
	bool bExport;
	bool bVirtaul;
	CCompileContext *pContext;//конекст компиляции
	int nVarCount;//число локальных переменных
	int nStart;//стартовая позиция в массиве байт-кодов
	int nFinish;//конечная позиция в массиве байт-кодов

	SParam RealRetValue;//для хранения переменной при реальном вызове
	bool	bSysFunction;
	CString csType;		//тип (в англ. нотации), если это типизированная функция

	//для IntelliSense
	int	nNumberLine;	//номер строки исходного текста (для точек останова)
	CString csShortDescription;//включает в себя всю строку после ключевого слова Функция(Процедура)
	CString csLongDescription;//включает в себя весь слитный (т.е.е буз пустых строк) блок комментарий до определения функции (процедуры)
};//CFunction

//____________________________________________________________________________
class CCallFunction
{
public: 
	CString csName;//Имя вызываемой функции
	CArray<SParam,SParam> aParamList;//список передаваемых параметров (список перменных, если значение не задано, то д.б. (-1,-1))
	SParam sRetValue;//переменная куда должен возвращаться результат выполнения функции
	SParam sContextVal;//указатель на переменную Контекст
	int nAddLine;//положение в массиве байт-кодов, где встретился вызов (для случая, когда есть вызов, но функция еще не объявлена)
	int nError;//для выдачи сообщений при ошибках

	int	nStringNumber;	//номер исходного текста (для вывода ошибок)
	int	nNumberLine;	//номер строки исходного текста (для точек останова)
	CString csModuleName;//имя модуля (т.к. возможны include подключения из разных модулей)
};//CCallFunction


/******************************************************************
Класс: компилятор
*******************************************************************/
class CCompileModule  : public CTranslateModule
{
public:
	CCompileModule();
	virtual ~CCompileModule();

	//Основные методы:
	void Clear();//Сброс данных для повторного использования объекта
	void AddVariable(CString csName,CValue *pValue);//поддержка внешних переменных
	void SetParent(CCompileModule *pSetParent,CCompileModule *pSetStopParent=0,CCompileModule *pSetContinueParent=0);//Установка родительского модуля и запрещенного макс. прародителя
	bool Compile(CString csStrModule);//Компиляция модуля



	//атрибуты:
	CCompileModule *pParent;//родительский модуль (т.е. по отношению к текущему выполняет роль глобального модуля)
	CCompileModule *pStopParent;//модуль-прародитель, начиная с которого прекращается поиск глобальных функций и переменных, т.е. от не является прародителем (хотя возможно является таковым для родителя данного модуля) - все это нужно для имитации множественного наследования
	CCompileModule *pContinueParent;//начало разрешенной области прародитель

	int nCommonModule;
	int bIsObject;//признак объекта
	int bExtendMode;//признак компиляции всех родительских и дочерних модулей в один

	//текущий контекст переменных, функций и меток
	CCompileContext	cContext;
	CCompileContext	*pContext;	

	//выходной массив байт-кодов для исполнения виртуальной машиной
	CByteCode	cByteCode;
	int nCurrentCompile;		//текущее положение в массиве лексем

	BOOL bExpressionOnly;		//только вычисление выражений (нет новых заданий функций)

	//список типов переменных
	CMapStringToString cTypeList;

protected:
	CMapStringToPtr	HashConstList;
	CArray <CCallFunction*,CCallFunction*> apCallFunctions;	//список встретившихся вызовов процедур и функций

	int nLastNumberLine;



public:
	CCompileContext	*GetContext()
	{
		cContext.SetModule(this);
		return &cContext;
	};

	//методы вывода ошибок при компиляции:
	void SetError(int nErr,CString s="");
	void SetError(int nErr,char c);
	SParam GetExpression(int nPriority=0);
	bool CompileBlock();


	
protected:
	CLexem PreviewGetLexem();
	CLexem	GetLexem();
	CLexem	GETLexem();
	void	GETDelimeter(char c);

	bool IsNextDelimeter(char c);
	bool IsNextKeyWord(int nKey);
	void GETKeyWord(int nKey);
	CString GETIdentifier(int nOrig=0);
	CValue GETConstant();

	void AddLineInfo(CByte &code);
	bool CompileModule();
	bool CompileFunction();
	//bool CompileContext();

	bool CompileDeclaration();
	bool CompileGoto();
	bool CompileIf();
	bool CompileWhile();
	bool CompileFor();
	SParam GetCallFunction(CString csName);


	SParam GetCurrentIdentifier(int &nIsSet);

	SParam GetVariable(CString csName,bool bCheckError=0);
	SParam GetVariable();
	SParam FindConst(CValue &vData);

	bool AddCallFunction(CCallFunction* pRealCall);
	CFunction *GetFunction(CString csName,int *pNumber=0);


	int IsTypeVar(CString Str="");
	CString GetTypeVar(CString Str="");
	void AddTypeSet(SParam Var);

	int GetConstString(CString csMethod);

	void SetImport();
public:
	void ToDebug();

	static CMapStringToPtr	cSysFunctions;//список системных функций
	static void LoadSystemFunctions();
	static void ClearSystemFunctions();

};

#endif // !defined(AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_)
