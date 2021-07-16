// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
#if !defined(_AFX_DEFINITION)
#define _AFX_DEFINITION

//Список сообщений об ошибках
extern char* aErrors[];
enum//Номера сообщений об ошибках
{
	ERROR_USAGE=0,
	ERROR_FILE_READ,
	ERROR_FILE_OPEN,
	ERROR_ASSERT,
	ERROR_ASSERT_VALID,
	ERROR_SYS1,
	ERROR_TRANSLATE_BYTE,
	ERROR_TRANSLATE_WORD,
	ERROR_TRANSLATE_NUMBER,
	ERROR_TRANSLATE_STRING,
	ERROR_TRANSLATE_DATE,
	ERROR_IDENTIFIER_DUPLICATE,
	ERROR_LABEL_DEFINE,
	ERROR_KEYWORD_DEFINE,
	ERROR_CODE_DEFINE,
	ERROR_IDENTIFIER_DEFINE,
	ERROR_CODE,
	ERROR_DELIMETER,
	ERROR_FUNC_DELIMETER,
	ERROR_FUNC_DEFINE,
	ERROR_RETURN,
	ERROR_CONST_DEFINE,
	ERROR_ENDFUNC_DEFINE,
	ERROR_FILE_WRITE,
	ERROR_EXPRESSION,
	ERROR_KEYWORD,
	ERROR_IDENTIFIER_NOT_FOUND,
	ERROR_USE_BREAK,
	ERROR_USE_CONTINUE,
	ERROR_USE_RETURN,
	ERROR_USE_BLOCK,
	ERROR_EXPRESSION_REQUIRE,
	ERROR_CALL_FUNCTION,
	ERROR_DEF_FUNCTION,
	ERROR_MANY_PARAMS,
	ERROR_FEW_PARAMS,
	ERROR_VAR_NOT_FOUND,
	ERROR_END_PROGRAM,
	ERROR_ARRAY_SIZE_CONST,
	ERROR_DUBLICATE_IMPORT,
	ERROR_MODULE_NOT_FOUND,
	ERROR_USE_IMPORT,
	ERROR_USE_ENDDEF,

	ERROR_TYPE_DEF,
	ERROR_BAD_TYPE,
	ERROR_BAD_TYPE_EXPRESSION,
	ERROR_NUMBER_TYPE,
	ERROR_BAD_TYPE_EXPRESSION_N,
	ERROR_BAD_TYPE_EXPRESSION_S,
	ERROR_BAD_TYPE_EXPRESSION_D,
	ERROR_TYPE_OPERATION,


	LastError
};


enum//типы инструкций
{
	OPER_NOP=0,
	OPER_ADD,
	OPER_DIV,
	OPER_MULT,
	OPER_SUB,
	OPER_NOT,
	OPER_AND,
	OPER_OR,
	OPER_RET,
	OPER_GOTO,
	OPER_FOR,
	OPER_IF,
	OPER_LET,
	OPER_CONST,
	OPER_CONSTN,//целочисленная константа
	OPER_NEXT,
	OPER_MOD,
	OPER_INVERT,
	OPER_ITER,//?
	OPER_GT,//>
	OPER_EQ,//=
	OPER_LS,//<
	OPER_GE,//>=
	OPER_LE,//<=
	OPER_NE,//<>
	OPER_MSG,//для отладки
	OPER_TRY,
	OPER_RAISE,
	OPER_FUNC,//29
	OPER_ENDFUNC,
	OPER_CALL,//вызов функции
	OPER_SET,//задание параметра как переменной
	OPER_SETREF,//задание параметра как переменной по ссылке
	OPER_SETCONST,//задание параметра как константы
	OPER_SYSCALL,
	OPER_ADDCONS,
	OPER_DIVCONS,
	OPER_MULTCONS,
	OPER_SUBCONS,
	OPER_GTCONS,//>
	OPER_EQCONS,//=
	OPER_LSCONS,//<
	OPER_GECONS,//>=
	OPER_LECONS,//<=
	OPER_NECONS,//<>
	OPER_MODCONS,
	OPER_SET_A,
	OPER_GET_A,
	OPER_CALL_M,
	OPER_GET_ARRAY,
	OPER_SET_ARRAY,
	OPER_CHECK_ARRAY,
	OPER_SET_ARRAY_SIZE,
	OPER_ENDTRY,
	OPER_SET_TYPE,

	OPER_END,
};
#define TYPE_DELTA1	(OPER_END+1) //для числовых операций
#define TYPE_DELTA2	2*TYPE_DELTA1//для строковых операций
#define TYPE_DELTA3	3*TYPE_DELTA1//для операций с датами


//Массив имен операция для отладки (для метода ToDebug)
#ifdef _DEBUG
extern char *aOperNames[];
#endif

//Массив приоритетов математических операций
extern int aPriority[256];

enum//типы лексем
{
	ERRORTYPE=0,
	DELIMETER,//односимвольные разделители и операторы

	IDENTIFIER,//нераспознанный идентификатор (этап трансляции)
	CONSTANT,//константа
	KEYWORD,//содержит номер ключевого слова
	ENDPROGRAM,//конец программного модуля
};

enum//Номера ключевых слов (в строгой последовательности как заданы сами значения)
{
	KEY_IF=0,
	KEY_THEN,
	KEY_ELSE,
	KEY_ELSEIF,
	KEY_ENDIF,
	KEY_FOR,
	KEY_TO,
	KEY_DO,
	KEY_ENDDO,
	KEY_WHILE,
	KEY_GOTO,
	KEY_NOT,
	KEY_AND,
	KEY_OR,
	KEY_PROCEDURE,
	KEY_ENDPROCEDURE,
	KEY_FUNCTION,
	KEY_ENDFUNCTION,
	KEY_FORWARD,
	KEY_EXPORT,
	KEY_VAL,
	KEY_RETURN,
	KEY_TRY,
	KEY_EXCEPT,
	KEY_ENDTRY,
	KEY_CONTINUE,
	KEY_BREAK,
	KEY_RAISE,
	KEY_VAR,
//	KEY_CLASS,
//	KEY_ENDCLASS,
	KEY_PRINT,//служебный оператор для отладки
	KEY_VIRTUAL,
	KEY_STATIC,
//	KEY_IMPORT,
	KEY_INCLUDE,
	KEY_INCLUDE2,
	KEY_DEFINE,
	KEY_UNDEF,
	KEY_IFDEF,
	KEY_IFNDEF,
	KEY_ELSEDEF,
	KEY_ENDIFDEF,

/*	KEY_NUMBER,
	KEY_STRING,
	KEY_DATE,
	KEY_ARRAY,*/

	LastKeyWord
};


#define MODULESNAME CString("Общие модули")
#define FORMSNAME CString("Общие формы")
#define MAKETSNAME CString("Общие таблицы")
#define OBJECTSNAME CString("Объекты")
#define METADATANAME CString("Метаданные")
#define INITMODULENAME CString("Инициализация системы")

#define	OBJFORM		CString("Формы")
#define	OBJMODULE	CString("Модули")
#define	OBJMAKET	CString("Таблицы")
#define OBJCONST	CString("Глобальные переменные")


#define	ICON_CONSTGROUP	304
#define	ICON_CONST		250

#define	ICON_OBJECTGROUP 268//134
#define	ICON_OBJECT		270//134

#define	ICON_METADATA	209

#define	ICON_MAKETGROUP	318
#define	ICON_MAKET		79

#define	ICON_FORMGROUP	293
#define	ICON_FORM		294

#define	ICON_MODULEGROUP 317
#define	ICON_MODULE		308
#define	ICON_RUNMODULE	322
#define	ICON_CONFMODULE	241
#define	ICON_INITMODULE	309


#define	ENTERPRISEMODULENAME CString("Модуль среды исполнения")
#define	CONFIGMODULENAME CString("Модуль среды конфигурирования")
#define	GLOBALMODULENAME CString("Глобальный модуль")
#define	FORMOBJECTNAME CString("Форма объекта")
#define	FORMDIALOGNAME CString("Форма диалога")


#endif//_AFX_DEFINITION
