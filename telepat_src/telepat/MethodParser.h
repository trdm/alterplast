// MethodParser.h: interface for the CMethodParser class.
#if !defined(AFX_METHODPARSER_H__3C6CAAF1_7A2B_4750_928C_30654EDDB995__INCLUDED_)
#define AFX_METHODPARSER_H__3C6CAAF1_7A2B_4750_928C_30654EDDB995__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#define DECLARE_TERMINALS enum Terminals{EndOfFile,
#define END_TERMINALS Error};

#define DECLARE_NON_TERMINALS enum NonTerminals{accept,
#define END_NON_TERMINALS lastNonTerminal};

#define DECLARE_GRAMMAR(iNumOfStates,iNumOfRules) \
	enum{NumOfStates=iNumOfStates,NumOfRules=iNumOfRules}; \

#define DECLARE_RULE_FUNC(RulesName) \
	static SRuleFunc RulesName[];\
	static PF_ON_RULE RulesName##Array[NumOfRules]

#define IMPLEMENT_RULE_FUNC(ClassName,RulesName) \
	static struct S##RulesName##Init{S##RulesName##Init(){ClassName::LoadRuleFunc(ClassName::RulesName,ClassName::RulesName##Array);}}RulesName##Init;\
	ClassName::PF_ON_RULE ClassName::RulesName##Array[];\
	ClassName::SRuleFunc ClassName::RulesName[]={
#define RULE_FUNC(RuleNum,RuleFunc)	{RuleNum,(void (SRuleFunc::*)())&RuleFunc},
#define END_RULE_FUNC	{0,NULL}};


#ifdef BUILD_RESOURCE
#define IMPLEMENT_RULE(ClassName) ClassName::SRule ClassName::m_Rules[NumOfRules]={
#define RULE(iLen,iNonTerm) {iLen,iNonTerm},
#define END_IMPLEMENT_RULE };

#define IMPLEMENT_ACTIONS(ClassName) ClassName::SAction ClassName::m_Actions[NumOfStates][Error]={
#define BEGIN_STATE {
#define END_STATE },
#define SHT(par) {actionShift,par},
#define RDC(par) {actionReduce,par},
#define ACC {actionAccept,0},
#define ERR {actionError,0},
#define DSC {actionDiscard,0},
#define END_IMPLEMENT_ACTIONS };

#define IMPLEMENT_GOTO(ClassName) int ClassName::m_Goto[NumOfStates][lastNonTerminal]={
#define BEGIN_GOTO {0,
#define GOTO(par)	par,
#define END_GOTO },
#define END_IMPLEMENT_GOTO };

#else // BUILD_RESOURCE

#define IMPLEMENT_RULE(ClassName) ClassName::SRule ClassName::m_Rules[NumOfRules];
#define RULE(iLen,iNonTerm)
#define END_IMPLEMENT_RULE

#define IMPLEMENT_ACTIONS(ClassName) ClassName::SAction ClassName::m_Actions[NumOfStates][Error];
#define BEGIN_STATE 
#define END_STATE
#define SHT(par)
#define RDC(par)
#define ACC
#define ERR
#define DSC
#define END_IMPLEMENT_ACTIONS

#define IMPLEMENT_GOTO(ClassName) int ClassName::m_Goto[NumOfStates][lastNonTerminal];
#define BEGIN_GOTO 
#define GOTO(par)
#define END_GOTO
#define END_IMPLEMENT_GOTO

#endif // BUILD_RESOURCE

#include "syntax.h"
#include "btyacc\CMethodParser.h"
#include "resource.h"

class CParser;
class CMethodParser;

class CParseUnit
{
public:
	typedef void (CParseUnit::*P_RULE_ACTION)();
	struct SRulesFunc
	{
		int iRuleNum;
		P_RULE_ACTION pFunc;
	};

	CParseUnit(){}
	virtual ~CParseUnit(){}
	virtual void OnLastLexem(){}
	virtual int	OnError(){return 0;}
	virtual P_RULE_ACTION* GetRulesAction()=0;

	CMethodParser* m_pParser;
	CString	m_text;

	static void LoadRulesActions(P_RULE_ACTION* pRuleActions,SRulesFunc* pRules);
};
#define DECLARE_PARSE_UNIT \
	static P_RULE_ACTION	RulesArray[NumOfRules]; \
	static SRulesFunc		RulesFunc[]; \
	P_RULE_ACTION* GetRulesAction(){return RulesArray;}

#define IMPLEMENT_PARSE_UNIT(ClassName) \
	static struct S##ClassName##Init\
	{\
		S##ClassName##Init()\
		{\
			CParseUnit::LoadRulesActions(ClassName::RulesArray,ClassName::RulesFunc);\
		}\
	}s##ClassName##Init;\
	CParseUnit::P_RULE_ACTION ClassName::RulesArray[NumOfRules];\
	CParseUnit::SRulesFunc	ClassName::RulesFunc[]={	
#define RULE_ACTION(Num,Func) {Num,(P_RULE_ACTION)&Func},
#define END_IMPLEMENT_PARSE_UNIT	{0,NULL}};

enum SyntActions{
	actionError,
	actionShift,
	actionReduce,
	actionAccept,
};


struct SState{
	SState():state(0),token(0){}
	SLexem lexem;
	int state;
	int token;
};
struct SProcInfo;

class CMethodParser
{
public:
	void ShiftReduce();
	typedef void (CMethodParser::*PF_ON_RULE)();
	struct SRule{
		DWORD iRuleLen;
		DWORD sRight;
	};
	struct SAction{
		DWORD action;
		DWORD iNum;
	};
	struct SRuleFunc{
		int RuleNum;
		void(SRuleFunc::*pfunc)();
	};

	CMethodParser();
	~CMethodParser(){}
	int		IsTokenAllowed(SState* state,int token);
	void	SetParseMode(int mode);
	
	void	Parse(int mode);
	void	OnCurrentLexem();
	int		OnError();

	DECLARE_RULE_FUNC(MainParseFunc);
		void SetModeParseModul();
		void OnCurLexemModul();
		void OnModuleParams();
		void OnModuleMethHeader();
		void OnModuleMethName();
		void OnModuleVar();
		SProcInfo* m_curMethod;

	DECLARE_RULE_FUNC(ParseMethodFunc);
		void SetModeParseMethod();
		void OnCurLexemMethod();
		void OnMethodLocalVar();
		void OnMethodVar();
		void OnMethodParams();
		void OnMethodMethName();
		void OnMethodMethHeader();
		void OnMethodMethod();
		bool m_IsCurMethodExist;

	DECLARE_RULE_FUNC(ParseParamsFunc);
		void SetModeParseParams();
		void OnCurLexemParam();

	DECLARE_RULE_FUNC(CountParamsFunc);
		void SetModeCountParams();
		void OnCurLexemCountParams();
		void OnCountParams();
		int	 m_ParamsCount;
		SLexem m_ParamsMethod;
	
	DECLARE_RULE_FUNC(ParseMetHeaderFunc);
		void SetModeMethHeader();
		void OnCurLexemMetHeaderName();
		void OnMetHeaderName();
		CString m_curMetName;
	DECLARE_RULE_FUNC(ParseMembersFunc);
		void SetModeParseMembers();
		void OnCurLexemMembers();
		void OnMembers59();
		void OnMembers60();
		void OnMembers63();
		CStringArray m_arrMembers;
		CDWordArray  m_typeMembers;

	SState* m_pStateStack;
	SLexem	m_LastLexem;
	int		m_LastLexemType;

	SRule*  m_pLastRule;
	int		m_RuleNum;

	CLexer* m_lexer;
	int		m_ParseMode;
	int*	m_pLex2Token;
	bool	m_IsMember;
	
	enum ParseModes{
		ParseModul,
		ParseMethod,
		ParseParam,
		CountParams,
		ParseMetHeader,
		ParseMembers,
	};

	static void		Init();
	static void		LoadRuleFunc(SRuleFunc* ptr,PF_ON_RULE* ruleArray);
	static int		m_Lexem2Token[STATE_LAST];
	static int		m_Lexem2TokenFast[STATE_LAST];
	static SAction	m_Actions[NumOfStates][Error];
	static int		m_Goto[NumOfStates][lastNonTerminal];
	static SRule	m_Rules[NumOfRules];
	PF_ON_RULE*		m_RulesFunc;

	CParser* m_pParser;

	void ExpandConstant();
};

/*
class CModuleUnit:public CParseUnit
{
public:
	void OnRule1();
	DECLARE_PARSE_UNIT
};
*/
#endif // !defined(AFX_METHODPARSER_H__3C6CAAF1_7A2B_4750_928C_30654EDDB995__INCLUDED_)
