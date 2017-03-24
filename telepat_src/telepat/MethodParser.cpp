// MethodParser.cpp: implementation of the CMethodParser class.
#include "stdafx.h"
#include "Parser.h"
#include "txttrap.h"
#include "MethodParser.h"
#include "typegroup.h"
#include "metagroup.h"
#include "tform.h"

#include "lexem2tokenMet.inl"
#include "btyacc\CMethodParser.cpp"

/*
IMPLEMENT_PARSE_UNIT(CModuleUnit)
	RULE_ACTION(1,OnRule1)
END_IMPLEMENT_PARSE_UNIT
*/

void CMethodParser::SetParseMode(int mode)
{
	static void (CMethodParser::*SetModeFunc[])()={
		SetModeParseModul,
		SetModeParseMethod,
		SetModeParseParams,
		SetModeCountParams,
		SetModeMethHeader,
		SetModeParseMembers,
	};
	m_ParseMode=mode;
	(this->*SetModeFunc[mode])();
}

void CMethodParser::SetModeParseModul()
{
	m_pParser->m_Conts[CParser::Methods]->Clear();
	m_pParser->m_Conts[CParser::ModuleVars]->Clear();
	m_curMethod=NULL;
	m_RulesFunc=MainParseFuncArray;
}

CMethodParser::CMethodParser():m_pLastRule(NULL)
{
	m_RulesFunc=MainParseFuncArray;
	m_pLex2Token=m_Lexem2Token;
}

void CMethodParser::LoadRuleFunc(SRuleFunc* ptr,PF_ON_RULE* ruleArray)
{
	for(int i=0;i<NumOfRules;i++)
		ruleArray[i]=NULL;
	while(ptr->pfunc)
	{
		ruleArray[ptr->RuleNum]=(PF_ON_RULE)ptr->pfunc;
		ptr++;
	}
}

int CMethodParser::IsTokenAllowed(SState* state,int token)
{
	SState states[200];
	SState* ps=state;
	while(ps->state)
		ps--;
	int i=0;
	while(ps<=state)
		states[i++]=*ps++;
	ps=states+i-1;
	SAction* pAction=&m_Actions[ps->state][token];
	while(pAction->action!=actionError)
	{
		if(pAction->action==actionShift)
			return pAction->iNum;
		else
		{
			ps-=m_Rules[pAction->iNum].iRuleLen-1;
			ps->state=m_Goto[(ps-1)->state][m_Rules[pAction->iNum].sRight];
			pAction=&m_Actions[ps->state][token];
		}
	}
	return -1;
}

#pragma optimize("agt",on)
void CMethodParser::Parse(int mode)
{
	register SAction* pAction;
	SState statesstack[200];
	m_pStateStack=statesstack;
	SetParseMode(mode);
	try{
	while(1)
	{
		do
		{
			m_lexer->NextLexem(m_LastLexem);
			m_LastLexemType=m_pLex2Token[m_LastLexem.type];
			if(m_lexer->m_pCurPos==m_lexer->m_pEndPos)
				OnCurrentLexem();
		}
		while(m_LastLexemType<0);
tryanother:
		pAction=&m_Actions[m_pStateStack->state][m_LastLexemType];
lswitch:
		switch(pAction->action)
		{
		case actionShift:
			m_pStateStack++;
			m_pStateStack->lexem=m_LastLexem;
			m_pStateStack->token=m_LastLexemType;
			m_pStateStack->state=pAction->iNum;
			break;
		case actionReduce:
		case actionAccept:
		{
			do{
				m_RuleNum=pAction->iNum;
				m_pLastRule=m_Rules+m_RuleNum;
				if(m_RulesFunc[m_RuleNum])
					(this->*m_RulesFunc[m_RuleNum])();
				m_pStateStack-=m_pLastRule->iRuleLen-1;
				m_pStateStack->state=m_Goto[(m_pStateStack-1)->state][m_pLastRule->sRight];
				m_pStateStack->token=-m_pLastRule->sRight;
				if(m_pLastRule->sRight==0)
					return;
				pAction=&m_Actions[m_pStateStack->state][m_LastLexemType];
			}while(pAction->action>actionShift);
			goto lswitch;
		}
		case actionError:
			{
				int err=OnError();
				if(err<0)
					goto tryanother;
				else if(err>0)
					continue;
				// Make autocorrect
				// View previous states to found state, non error with current token
				SState* pRollBack=m_pStateStack-1;
				SAction* pErrAction;
				while(pRollBack>=statesstack)
				{
					pErrAction=&m_Actions[pRollBack->state][m_LastLexemType];
					// Found nonerror state, rollback to him
					if(pErrAction->action!=actionError)
					{
						m_pStateStack=pRollBack;
						pAction=pErrAction;
						goto lswitch;
					}
					pRollBack--;
				}
				if(pRollBack==statesstack && !m_LastLexem.type)
					return;
			}
		}
	}
	}catch(int){}
}

void CMethodParser::Init()
{
#ifdef BUILD_RESOURCE
	CString projectdir=__FILE__;
	projectdir=projectdir.Left(projectdir.ReverseFind('\\')+1);
	char fName[100];
	wsprintf(fName,"lexRes%i",IDR_PARSER);
	HANDLE hFile=CreateFile(projectdir+fName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	DWORD rw;
	WriteFile(hFile,m_Actions,			sizeof(m_Actions),&rw,NULL);
	WriteFile(hFile,m_Goto,				sizeof(m_Goto),&rw,NULL);
	WriteFile(hFile,m_Rules,			sizeof(m_Rules),&rw,NULL);
	WriteFile(hFile,m_Lexem2Token,		sizeof(m_Lexem2Token),&rw,NULL);
	WriteFile(hFile,m_Lexem2TokenFast,	sizeof(m_Lexem2TokenFast),&rw,NULL);
	CloseHandle(hFile);
#else
	char *pData=(char*)GetZipResource(IDR_PARSER),*pRead=pData;
	memcpy(m_Actions,pRead,sizeof(m_Actions));
	pRead+=sizeof(m_Actions);

	memcpy(m_Goto,pRead,sizeof(m_Goto));
	pRead+=sizeof(m_Goto);

	memcpy(m_Rules,pRead,sizeof(m_Rules));
	pRead+=sizeof(m_Rules);

	memcpy(m_Lexem2Token,pRead,sizeof(m_Lexem2Token));
	pRead+=sizeof(m_Lexem2Token);

	memcpy(m_Lexem2TokenFast,pRead,sizeof(m_Lexem2TokenFast));
	delete [] pData;
#endif
}

void CMethodParser::OnCurrentLexem()
{
	static const PF_ON_RULE curLexemFunc[]=
	{
		OnCurLexemModul,
		OnCurLexemMethod,
		OnCurLexemParam,
		OnCurLexemCountParams,
		OnCurLexemMetHeaderName,
		OnCurLexemMembers,
	};
	(this->*curLexemFunc[m_ParseMode])();
}

int CMethodParser::OnError()
{
	if((m_LastLexemType==_crob_1C) ||
		((m_pStateStack->state==83 || m_pStateStack->state==81) && m_LastLexemType>=and_1C) )
	{
		m_LastLexemType=_zname_1C;
		return -1;
	}
	if(m_ParseMode==ParseMembers)
	{
		m_arrMembers.RemoveAll();
		m_typeMembers.RemoveAll();
		throw int(1);
	}
	//DoMsgLine("Error in line %i, state=%i, lexem=%s",mmNone,m_LastLexem.line,m_pStateStack->state,CString(m_LastLexem));
	return 0;
}

void CMethodParser::OnModuleVar()
{
/*
   6  VARIABLES : _zname_1C
   7            | _zname_1C export_1C
   8            | _zname_1C _lBracket_1C _number_1C _rBracket_1C
   9            | _zname_1C _lBracket_1C _number_1C _rBracket_1C export_1C
*/
	SState* st=m_pStateStack;
	CString array;
	while(st->token!=_zname_1C)
	{
		if(st->token==_number_1C)
			array=st->lexem;
		st--;
	}
	SVarInfo* pInfo=((CVarCont*)m_pParser->m_Conts[CParser::ModuleVars])->AddVar(st->lexem);
	pInfo->array=array;
	pInfo->export=(m_pStateStack->token==export_1C);
}

void CMethodParser::OnModuleMethName()
{
/*
  20  METHNAME : _zname_1C
*/
	if(m_curMethod)
		delete m_curMethod;
	SState* start=m_pStateStack-1;
	m_curMethod=new SProcInfo(start->lexem,m_pStateStack->lexem);
	m_curMethod->func=start->token==func_1C;
}

void CMethodParser::OnModuleMethHeader()
{
/*
  18  METHEADER : METHNAME PARAMLIST
  19            | METHNAME PARAMLIST export_1C
*/
	SState* st=m_pStateStack;
	m_curMethod->export=(st->token==export_1C);
	bool forward=(m_LastLexemType==forward_1C);
	bool func=m_curMethod->func;
	((CProcCont*)m_pParser->m_Conts[CParser::Methods])->AddProc(m_curMethod,forward);
	m_curMethod=NULL;
	if(forward)
		return;
	while(m_Lexem2TokenFast[m_LastLexem.type]<0)
		m_lexer->NextLexem(m_LastLexem);
	m_LastLexemType=m_Lexem2TokenFast[m_LastLexem.type];
	if(!(m_LastLexemType==endproc_1C || m_LastLexemType==endfunc_1C))
		m_lexer->m_pCurPos=m_LastLexem.pStart;
	m_LastLexemType=func?endfunc_1C:endproc_1C;
}


void CMethodParser::OnModuleParams()
{
/*
  23  PARAMS : _zname_1C
  24         | val_1C _zname_1C
  25         | _zname_1C _equal_1C
  26         | val_1C _zname_1C _equal_1C
  27         | _zname_1C _equal_1C CONSTANT
  28         | val_1C _zname_1C _equal_1C CONSTANT
  29         | _zname_1C _lBracket_1C _rBracket_1C
*/
	SState* name=m_pStateStack;
	CString def;
	bool val=false;
	bool arr=false;

	switch(m_RuleNum){
	case 24:
		val=true;
		break;
	case 25:
		name--;
		def="\"\"";
		break;
	case 26:
		name--;
		def="\"\"";
		val=true;
		break;
	case 27:
		name-=2;
		def=m_pStateStack->lexem;
		break;
	case 28:
		name-=2;
		val=true;
		def=m_pStateStack->lexem;
		break;
	case 29:
		name-=2;
		arr=true;
	}

	SParamInfo* param=new SParamInfo;
	param->name=name->lexem;
	param->array=arr;
	param->def=def;
	param->val=val;
	m_curMethod->AddParam(param);
}

void CMethodParser::OnCurLexemModul()
{
}

IMPLEMENT_RULE_FUNC(CMethodParser,MainParseFunc)
	RULE_FUNC(6,OnModuleVar)
	RULE_FUNC(7,OnModuleVar)
	RULE_FUNC(8,OnModuleVar)
	RULE_FUNC(9,OnModuleVar)

	RULE_FUNC(18,OnModuleMethHeader)
	RULE_FUNC(19,OnModuleMethHeader)

	RULE_FUNC(20,OnModuleMethName)

	RULE_FUNC(23,OnModuleParams)
	RULE_FUNC(24,OnModuleParams)
	RULE_FUNC(25,OnModuleParams)
	RULE_FUNC(26,OnModuleParams)
	RULE_FUNC(27,OnModuleParams)
	RULE_FUNC(28,OnModuleParams)
	RULE_FUNC(29,OnModuleParams)

	RULE_FUNC(109,ExpandConstant)
	RULE_FUNC(113,ExpandConstant)
	RULE_FUNC(114,ExpandConstant)
END_RULE_FUNC

void CMethodParser::OnMethodMethName()
{
/*
  20  METHNAME : _zname_1C
*/
	if(m_curMethod && !m_IsCurMethodExist)
		delete m_curMethod;

	m_curMethod=((CProcCont*)m_pParser->m_Conts[CParser::Methods])->GetProc(m_pStateStack->lexem);
	if(!m_curMethod)
	{
		SState* start=m_pStateStack-1;
		m_curMethod=new SProcInfo(start->lexem,m_pStateStack->lexem);
		m_curMethod->func=start->token==func_1C;
		m_IsCurMethodExist=false;
	}
	else
	{
		if(((m_pStateStack-1)->token==func_1C) != m_curMethod->func)
		{
			m_curMethod->func=!m_curMethod->func;
			m_pParser->m_Conts[CParser::Methods]->m_builded=false;
		}
		m_IsCurMethodExist=true;
	}
}

void CMethodParser::OnMethodParams()
{
/*
  23  PARAMS : _zname_1C
  24         | val_1C _zname_1C
  25         | _zname_1C _equal_1C
  26         | val_1C _zname_1C _equal_1C
  27         | _zname_1C _equal_1C CONSTANT
  28         | val_1C _zname_1C _equal_1C CONSTANT
  29         | _zname_1C _lBracket_1C _rBracket_1C
*/
	SState* st=m_pStateStack;
	CString array;
	if(st->token==_rBracket_1C)
		array=" ";
	while(st->token!=_zname_1C)
		st--;
	SLocalVarInfo* pVar=((CLocalVarCont*)m_pParser->m_Conts[CParser::LocalVars])->AddVar(st->lexem);
	pVar->type=0;
	pVar->array=array;
	pVar->lastDef.pStart=NULL;
}

void CMethodParser::OnMethodMethHeader()
{
/*
  18  METHEADER : METHNAME PARAMLIST
  19            | METHNAME PARAMLIST export_1C
*/
	SState* st=m_pStateStack;
	bool export=st->token==export_1C;
	bool refresh=false;
	if(m_IsCurMethodExist)
	{
		if(export!=m_curMethod->export)
			refresh=true;
	}
	else
	{
		refresh=true;
		((CProcCont*)m_pParser->m_Conts[CParser::Methods])->AddProc(m_curMethod,
			m_LastLexemType==forward_1C);
	}
	if(refresh)
		((CProcCont*)m_pParser->m_Conts[CParser::Methods])->m_builded=false;
	m_curMethod->export=export;
	m_curMethod=NULL;
}

void CMethodParser::OnMethodVar()
{
/*
  35  LVARIABLES : _zname_1C
  36             | _zname_1C _lBracket_1C _number_1C _rBracket_1C
*/
	SState* st=m_pStateStack;
	CString array;
	if(st->token!=_zname_1C)
	{
		st--;
		array=st->lexem;
	}
	while(st->token!=_zname_1C)
		st--;
	SLocalVarInfo* pVar=((CLocalVarCont*)m_pParser->m_Conts[CParser::LocalVars])->AddVar(st->lexem);
	pVar->type=1;
	pVar->array=array;
	pVar->lastDef=st->lexem;
}

void CMethodParser::OnMethodLocalVar()
{
/*
  56  LOCALVAR : _zname_1C _equal_1C EXPRESSION
  57           | _zname_1C _equal_1C _crob_1C _lPar_1C EXPRESSION _rPar_1C
*/
	SState* st=m_pStateStack;//-2;
	while(st->token!=_zname_1C)
		st--;
	// Ищем в атрибутах метаданных
	if(m_pParser->m_pMetaMethods && m_pParser->m_pMetaMethods->IsAttribExist(st->lexem))
		return;
	if(((CMetaGroup*)m_pParser->m_Conts[CParser::MetaAttribs])->IsAttribExist(st->lexem))
		return;
	// Ищем в реквизитах диалога
	if(m_pParser->m_pForm && m_pParser->m_pForm->IsAttribExist(st->lexem))
		return;

	// Ищем в локальных переменных
	SLocalVarInfo* pInfo=((CLocalVarCont*)m_pParser->m_Conts[CParser::LocalVars])->GetVar(st->lexem);
	if(pInfo)
	{
		// Если есть, обновляем инфу о последнем присваивании
		pInfo->lastDef=st->lexem;
		return;
	}
	// Теперь ищем в переменных модуля
	SVarInfo* pModInfo=((CVarCont*)m_pParser->m_Conts[CParser::ModuleVars])->GetVar(st->lexem);
	if(pModInfo)
		return;
	if(!m_pParser->IsGM())
	{
		// Если не ГМ, то надо проверить, нет ли в экспортируеммых переменных
		pModInfo=((CVarCont*)CParser::m_GM->m_Conts[CParser::ModuleVars])->GetVar(st->lexem);
		if(pModInfo && pModInfo->export)
			return;
	}
	// Нигде не нашли. Добавим
	pInfo=((CLocalVarCont*)m_pParser->m_Conts[CParser::LocalVars])->AddVar(st->lexem);
	pInfo->type=2;
	pInfo->lastDef=st->lexem;
}

void CMethodParser::OnMethodMethod()
{
/*
  14  METHOD : proc_1C METHEADER LVARS STATEMENTS endproc_1C
  15         | func_1C METHEADER LVARS STATEMENTS endfunc_1C
  16         | proc_1C METHEADER forward_1C
  17         | func_1C METHEADER forward_1C
*/
	((CLocalVarCont*)m_pParser->m_Conts[CParser::LocalVars])->Clear();
}

void CMethodParser::SetModeParseMethod()
{
	m_pParser->m_Conts[CParser::LocalVars]->Clear();
	m_pParser->m_Conts[CParser::MetaAttribs]->Clear();
	m_curMethod=NULL;
	m_RulesFunc=ParseMethodFuncArray;
	if(m_pParser->m_LastParsedFrom>0)
	{
		// Если парсинг метода не с первой строки модуля,
		// то надо свернуть к VARS METHODS
		(++m_pStateStack)->state=3;
		m_pStateStack->token=-VARS;
		(++m_pStateStack)->state=9;
		m_pStateStack->token=-METHODS;
	}
}

void CMethodParser::OnCurLexemMethod()
{
	if(m_LastLexemType && m_LastLexemType<=_what_1C)
	{
		// Курсор после разделителя
		if(m_LastLexemType==-1)	// В комментариях разрешим почти все
		{
			for(int i=0;i<3;i++)
				m_pParser->m_AllowedConts[i]=true;
			throw int(1);
		}
		return;
	}
	m_pParser->m_curLexem=m_LastLexem;

	// todo state 6 return
	// 1	- allow method
	// 2	- allow module var
	// 4	- allow local var,
	// 8	- allow metaatribs
	// 16	- members
	// 32	-
	// 64	- types
	// 128-allow only not array
	static const unsigned char States[NumOfStates]=
	{
		0,	0,	0,	0,	0,	0,	0,	0,	0,	15,
		0,	0,	0,	0,	0,	0,	0,	15,	0,	15,
		0,	0,	134,15,	15,	15,	15,	0,	0,	0,	// label
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	0,	15,	0,	0,
		15,	0,	15,	15,	15,	15,	15,	0,	0,	0,
		0,	0,	0,	15,	15,	15,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	15,	0,	0,	0,	0,
		15,	16,	15,	16,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	15,	15,	0,	0,	0,	0,	15,	0,
		0,	0,	0,	0,	0,	15,	15,	15,	15,	15,
		0,	0,	0,	0,	15,	15,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	15,	0,	15,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	15,	0,	0,	0,
		0,	15,	15,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	15,	15,	0,	15,
		15,	0,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	0,	0,	0,	0,	0,	0,
		0,	0,	0,	0,	15,	15,	0,	0,	0,	0,
	};

	for(int i=and_1C;i<=while_1C;i++)
	{
		if(IsTokenAllowed(m_pStateStack,i)>-1)
			CParser::m_kwords.SetWord(i-and_1C);
	}
	if(IsTokenAllowed(m_pStateStack,_zname_1C)>-1)
	{
		SAction* pAction=&m_Actions[m_pStateStack->state][_zname_1C];
		while(pAction->action==actionReduce)
		{
			m_RuleNum=pAction->iNum;
			m_pLastRule=m_Rules+m_RuleNum;
			if(m_RulesFunc[m_RuleNum])
				(this->*m_RulesFunc[pAction->iNum])();
			m_pStateStack-=m_pLastRule->iRuleLen-1;
			m_pStateStack->state=m_Goto[(m_pStateStack-1)->state][m_pLastRule->sRight];
			m_pStateStack->token=-m_pLastRule->sRight;
			pAction=&m_Actions[m_pStateStack->state][_zname_1C];
		}
		for(int i=0;i<4;i++)
			m_pParser->m_AllowedConts[i]=(States[m_pStateStack->state] & (1<<i))!=0;
		m_IsMember=(States[m_pStateStack->state] & 16)!=0;

		if(States[m_pStateStack->state] & 128)
			m_pParser->m_flags|=CParser::NotArrayVar;
		else
			m_pParser->m_flags&=~CParser::NotArrayVar;
		if(States[m_pStateStack->state] & 16)
		{
			SetModeParseMembers();
			return;
		}
	}

	SState* st=m_pStateStack;
	if(m_Goto[st->state][STATEMENT])
	{
		CParser::m_kwords.m_allowedWords&=~((1<<CKeyWords::kwBreak)|(1<<CKeyWords::kwContinue));
		while(st->state)
		{
			if(st->token==for_1C || st->token==while_1C)
			{
				CParser::m_kwords.m_allowedWords|=((1<<CKeyWords::kwBreak)|(1<<CKeyWords::kwContinue));
				break;
			}
			st--;
		}
	}
	else
	{
		do
		{
			if(m_Goto[st->state][EXPRESSION])
			{
				m_pParser->m_flags|=CParser::InExpression;
				break;
			}
		}while((--st)->state);
		st=m_pStateStack;
		do
		{
			if(m_Goto[st->state][METHEADER])
			{
				m_pParser->m_LastParsedFrom=-1;
				break;
			}
		}while((--st)->state);
	}
	DWORD kwRet=1<<CKeyWords::kwReturn;
	if(CParser::m_kwords.m_allowedWords  & kwRet)
	{
		bool remove=true;
		st=m_pStateStack;
		while(st->state)
		{
			if(st->token==proc_1C || st->token==func_1C)
			{
				remove=false;
				if(st->token==proc_1C)
					m_pParser->m_flags|=CParser::InProc;
				break;
			}
			st--;
		}
		if(remove)
			CParser::m_kwords.m_allowedWords &=~kwRet;
	}
	throw (int)1;
}

IMPLEMENT_RULE_FUNC(CMethodParser,ParseMethodFunc)
	RULE_FUNC(14,OnMethodMethod)
	RULE_FUNC(15,OnMethodMethod)
	RULE_FUNC(16,OnMethodMethod)
	RULE_FUNC(17,OnMethodMethod)

	RULE_FUNC(18,OnMethodMethHeader)
	RULE_FUNC(19,OnMethodMethHeader)
	
	RULE_FUNC(20,OnMethodMethName)

	RULE_FUNC(23,OnMethodParams)
	RULE_FUNC(24,OnMethodParams)
	RULE_FUNC(25,OnMethodParams)
	RULE_FUNC(26,OnMethodParams)
	RULE_FUNC(27,OnMethodParams)
	RULE_FUNC(28,OnMethodParams)
	RULE_FUNC(29,OnMethodParams)

	RULE_FUNC(35,OnMethodVar)
	RULE_FUNC(36,OnMethodVar)

	RULE_FUNC(56,OnMethodLocalVar)
	RULE_FUNC(57,OnMethodLocalVar)
END_RULE_FUNC

void CMethodParser::SetModeParseParams()
{
	m_RulesFunc=ParseParamsFuncArray;
	m_ParamsMethod.Empty();
	m_Lexem2Token[STATE_CREATEOBJECT]=_zname_1C;
}

void CMethodParser::OnCurLexemParam()
{
	ShiftReduce();
	SState* st=m_pStateStack;
	while(st->state)
	{
		if(m_Goto[st->state][REALPARAM])
		{
			while(st->state)
			{
				if(st->token==_zname_1C)
				{
					//if(m_Goto[(st-1)->state][RVAL])
					//{
						// Простой метод
						m_pStateStack=st;
						m_lexer->m_pCurPos=st->lexem.pStart+st->lexem.len;
						m_lexer->m_curLine=st->lexem.line;
						m_ParamsMethod=st->lexem;
						do
							m_lexer->NextLexem(m_LastLexem);
						while((m_LastLexemType=m_pLex2Token[m_LastLexem.type])<0);
						SetParseMode(CountParams);
						return;
					/*}
					else
					{
						// Метод объекта
					}
					break;
					*/
				}
				st--;
			}
			break;
		}
		st--;
	}
	m_Lexem2Token[STATE_CREATEOBJECT]=_crob_1C;
	throw (int)1;
}

IMPLEMENT_RULE_FUNC(CMethodParser,ParseParamsFunc)
END_RULE_FUNC

void CMethodParser::SetModeCountParams()
{
	m_RulesFunc=CountParamsFuncArray;
	m_ParamsCount=0;
}

void CMethodParser::OnCurLexemCountParams()
{
	m_Lexem2Token[STATE_CREATEOBJECT]=_crob_1C;
}

void CMethodParser::OnCountParams()
{
/*
  66  REALPARAM : EXPRESSION
  67            | REALPARAM _comma_1C REALPARAM
  68            | context_1C
  69            |
*/
	SState* st=m_pStateStack-1;
	while(st->state)
	{
		if(st->token==_zname_1C)
		{
			if(st->lexem.pStart==m_ParamsMethod.pStart)
				m_ParamsCount++;
			break;
		}
		st--;
	}
}

IMPLEMENT_RULE_FUNC(CMethodParser,CountParamsFunc)
	RULE_FUNC(66,OnCountParams)
	RULE_FUNC(68,OnCountParams)
	RULE_FUNC(69,OnCountParams)
END_RULE_FUNC

void CMethodParser::ExpandConstant()
{
	SState* st=m_pStateStack-1;
	st->lexem.len=m_pStateStack->lexem.pStart-st->lexem.pStart+m_pStateStack->lexem.len;
}

void CMethodParser::ShiftReduce()
{
	if(m_LastLexemType<0)
		return;
	SAction* pAction=&m_Actions[m_pStateStack->state][m_LastLexemType];
	while(pAction->action>actionError)
	{
		if(pAction->action==actionShift)
		{
			m_pStateStack++;
			m_pStateStack->state=pAction->iNum;
			m_pStateStack->lexem=m_LastLexem;
			m_pStateStack->token=m_LastLexemType;
			m_LastLexemType=0;
		}
		else
		{
			m_RuleNum=pAction->iNum;
			m_pLastRule=m_Rules+m_RuleNum;
				if(m_RulesFunc[m_RuleNum])
					(this->*m_RulesFunc[m_RuleNum])();
			m_pStateStack-=m_pLastRule->iRuleLen-1;
			m_pStateStack->state=m_Goto[(m_pStateStack-1)->state][m_pLastRule->sRight];
			m_pStateStack->token=-m_pLastRule->sRight;
			if(m_pLastRule->sRight==0 || m_pStateStack->state==0)
				return;
		}
		pAction=&m_Actions[m_pStateStack->state][m_LastLexemType];
	}
}

IMPLEMENT_RULE_FUNC(CMethodParser,ParseMetHeaderFunc)
	RULE_FUNC(20,OnMetHeaderName)
END_RULE_FUNC

void CMethodParser::SetModeMethHeader()
{
	m_curMetName.Empty();
	m_pParser->m_lexParser.SetSource(m_pParser->m_textMethod,m_pParser->m_textMethod.GetLength());
	m_RulesFunc=ParseMetHeaderFuncArray;
}

void CMethodParser::OnCurLexemMetHeaderName()
{
}

void CMethodParser::OnMetHeaderName()
{
	m_curMetName=m_pStateStack->lexem;
	throw int(1);
}

/*
59  LVAL : _zname_1C
60       | _zname_1C _lBracket_1C EXPRESSION _rBracket_1C
61       | LVAL _period_1C _zname_1C
62       | RVAL _period_1C _zname_1C

63  RVAL : _zname_1C _lPar_1C REALPARAM _rPar_1C
64       | RVAL _period_1C _zname_1C _lPar_1C REALPARAM _rPar_1C
65       | LVAL _period_1C _zname_1C _lPar_1C REALPARAM _rPar_1C
*/
IMPLEMENT_RULE_FUNC(CMethodParser,ParseMembersFunc)
	RULE_FUNC(59,OnMembers59)
	RULE_FUNC(60,OnMembers60)
	RULE_FUNC(61,OnMembers59)
	RULE_FUNC(62,OnMembers59)
	RULE_FUNC(63,OnMembers63)
	RULE_FUNC(64,OnMembers63)
	RULE_FUNC(65,OnMembers63)
END_RULE_FUNC

void CMethodParser::SetModeParseMembers()
{
	m_ParseMode=ParseMembers;
	m_RulesFunc=ParseMembersFuncArray;
	m_arrMembers.RemoveAll();
	m_typeMembers.RemoveAll();

	m_pStateStack--;
	m_lexer->m_pCurPos=m_pStateStack->lexem.pStart;
	m_lexer->m_curLine=m_pStateStack->lexem.line;
	m_lexer->NextLexem(m_LastLexem);
	m_LastLexemType=m_pLex2Token[m_LastLexem.type];
	m_pStateStack--;
}

void CMethodParser::OnCurLexemMembers()
{
	if(m_LastLexemType && m_LastLexemType<=_what_1C)
		return;
	/*int s=m_arrMembers.GetSize();
	for(int i=0;i<s;i++)
	{
		DWORD flag=m_typeMembers[i];
		DoMsgLine("%s - %s",mmNone,m_arrMembers[i],(flag & 1)?((flag & 4)?"array":"prop"):"method");
	}
	*/
	throw(int)1;
}
void CMethodParser::OnMembers59()
{
/*
59  LVAL : _zname_1C
61       | LVAL _period_1C _zname_1C
62       | RVAL _period_1C _zname_1C
*/
	m_arrMembers.Add(m_pStateStack->lexem);
	m_typeMembers.Add(1);
}

void CMethodParser::OnMembers60()
{
/*
60       | _zname_1C _lBracket_1C EXPRESSION _rBracket_1C
*/
	m_arrMembers.Add((m_pStateStack-3)->lexem);
	m_typeMembers.Add(5);
}

void CMethodParser::OnMembers63()
{
/*
63  RVAL : _zname_1C _lPar_1C REALPARAM _rPar_1C
64       | RVAL _period_1C _zname_1C _lPar_1C REALPARAM _rPar_1C
65       | LVAL _period_1C _zname_1C _lPar_1C REALPARAM _rPar_1C
*/
	m_arrMembers.Add((m_pStateStack-3)->lexem);
	m_typeMembers.Add(2);
}