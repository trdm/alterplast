
#ifndef _PARSER_H_
#define _PARSER_H_

#include <stdlib.h>
//#include <string.h>
//#include <ctype.h>
#include <afxcoll.h>

class CCode;


//ќбъ€влени€ дл€ Bison'а
#if !defined(YYLTYPE_IS_DECLARED)
	typedef CCode* YYSTYPE;
	#define YYSTYPE_IS_DECLARED 1
#endif
#if !defined(YYLTYPE_IS_DECLARED)
	typedef struct YYLTYPE
	{
		int first_line;
		int first_column;
		int last_line;
		int last_column;
	} YYLTYPE;
	#define YYLTYPE_IS_DECLARED 1
	#define YYLTYPE_IS_TRIVIAL 1
#endif
//-----------------------------
#include "lexer_1c.h" //WARNING: this include can not be placed before previous declarations!



class CCode;

class CMemCollector
{
private:
	static CMapPtrToPtr MemMap;
public:
	static long maxRecords;

public:
	static void Add(CCode* code);
	static void Remove(CCode* code);
	static void Free();
};

typedef enum 
{
	CT_Error,
	CT_Lexema,
	CT_VarDef,
	CT_VarDefList,
	CT_VarDecl,
	CT_VarDeclList,
	CT_Expr,
	CT_Operator,
	CT_Statement,
	CT_Statements,
	CT_LogicalExpr,
	CT_LValue,
	CT_If,
	CT_Else,
	CT_ElseIf,
	CT_While,
	CT_For,
	CT_Try,
	CT_MethCall,
	CT_CallParam,
	CT_CallParamList,
	CT_MethHeader,
	CT_ParamList,
	CT_Param,
	CT_Function,
	CT_Procedure,
	CT_MethList,
	CT_Module
} TCodeType;

typedef enum
{
	LT_String,
	LT_Number,
	LT_Date,
	LT_Label,
	LT_Word,
	LT_Keyword,
	LT_Other
} TLexemaType;

class CCodeIterator;
class CLexema;

class CCode
{
friend CCodeIterator;

public:
	int StartLine;
	int StartCol;
	int EndLine;
	int EndCol;
	TCodeType CodeType;

private:
	CPtrList Parts;

public:
	CCode() {};
	CCode(YYLTYPE& yyloc, TCodeType ct = CT_Expr);
	void SetLoc(YYLTYPE& yyloc);
	void DestroyNodes();
	virtual ~CCode();

	void AddHead(CCode* part);
	void AddTail(CCode* part);
	//метод исключительно дл€ парсера. ѕараметры после nParts д.б. типа CCode*
	void AddTail(int nParts, ...);

	CCode* GetFirst();
	CCode* GetLast();
	int GetPartsCount() { return Parts.GetCount(); };
	CLexema* GetLeftmostLexema();
	
	virtual CString GetText();

	bool HasErrors();

	const char* CodeTypeAsStr(TCodeType type);

	void TypeNode(int level = 0);
	void PrintTree(int level = 0);
};

class CLexema : public CCode
{
public:
	CString Text;
	TLexemaType LexType;
	int KeyWord;

public:
	CLexema(YYLTYPE& yyloc, const char* text, TLexemaType type);
	virtual CString GetText();
};

class CCodeIterator
{
private:
	CPtrList* Parts;
	POSITION pos, head_pos, tail_pos;
public:
	CCodeIterator(CCode* aCode) {Parts = &(aCode->Parts);};
	void Reset(CCode* aCode) {Parts = &(aCode->Parts);};

	CCode* First();
	CCode* Last();
	CCode* Next();
	CCode* Prev();
	int Count() {return Parts->GetCount();};
};


#endif
