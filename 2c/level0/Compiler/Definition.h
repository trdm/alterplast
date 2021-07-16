// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
#if !defined(_AFX_DEFINITION)
#define _AFX_DEFINITION

//������ ��������� �� �������
extern char* aErrors[];
enum//������ ��������� �� �������
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


enum//���� ����������
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
	OPER_CONSTN,//������������� ���������
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
	OPER_MSG,//��� �������
	OPER_TRY,
	OPER_RAISE,
	OPER_FUNC,//29
	OPER_ENDFUNC,
	OPER_CALL,//����� �������
	OPER_SET,//������� ��������� ��� ����������
	OPER_SETREF,//������� ��������� ��� ���������� �� ������
	OPER_SETCONST,//������� ��������� ��� ���������
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
#define TYPE_DELTA1	(OPER_END+1) //��� �������� ��������
#define TYPE_DELTA2	2*TYPE_DELTA1//��� ��������� ��������
#define TYPE_DELTA3	3*TYPE_DELTA1//��� �������� � ������


//������ ���� �������� ��� ������� (��� ������ ToDebug)
#ifdef _DEBUG
extern char *aOperNames[];
#endif

//������ ����������� �������������� ��������
extern int aPriority[256];

enum//���� ������
{
	ERRORTYPE=0,
	DELIMETER,//�������������� ����������� � ���������

	IDENTIFIER,//�������������� ������������� (���� ����������)
	CONSTANT,//���������
	KEYWORD,//�������� ����� ��������� �����
	ENDPROGRAM,//����� ������������ ������
};

enum//������ �������� ���� (� ������� ������������������ ��� ������ ���� ��������)
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
	KEY_PRINT,//��������� �������� ��� �������
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


#define MODULESNAME CString("����� ������")
#define FORMSNAME CString("����� �����")
#define MAKETSNAME CString("����� �������")
#define OBJECTSNAME CString("�������")
#define METADATANAME CString("����������")
#define INITMODULENAME CString("������������� �������")

#define	OBJFORM		CString("�����")
#define	OBJMODULE	CString("������")
#define	OBJMAKET	CString("�������")
#define OBJCONST	CString("���������� ����������")


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


#define	ENTERPRISEMODULENAME CString("������ ����� ����������")
#define	CONFIGMODULENAME CString("������ ����� ����������������")
#define	GLOBALMODULENAME CString("���������� ������")
#define	FORMOBJECTNAME CString("����� �������")
#define	FORMDIALOGNAME CString("����� �������")


#endif//_AFX_DEFINITION
