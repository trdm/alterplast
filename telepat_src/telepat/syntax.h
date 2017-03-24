// syntax.h
#ifndef SYNTAX_H
#define SYNTAX_H
#include "blmap.h"

struct SLexem{
	SLexem():pStart(NULL),len(0),type(0){}
	SLexem(LPCTSTR pS,LPCTSTR pE,short s):pStart(pS),len(pE-pS),type(s){}
	SLexem(LPCTSTR pS,int l=0):pStart(pS),len(l){}
	SLexem(LPCTSTR pS,LPCTSTR pE=NULL);

	operator CString()const{return pStart?CString(pStart,len):CString("");}
	void Empty(){pStart=NULL;line=type=len=0;}
	bool IsStrBegin(LPCTSTR ptr);
	bool IsLexemBegin(LPCTSTR ptr,bool id=true);

	LPCTSTR pStart;
	int line;
	short len;
	short type;
};
#include "statesandsymbols.inl"

class CLexer
{
public:
	void NextLexem(SLexem& lexem);
	static void Init();
	void SetSource(LPCTSTR pStart,int length,LPCTSTR pCurrent=NULL,int StartLine=0);

	LPCTSTR m_pStartPos;
	LPCTSTR m_pCurPos;
	LPCTSTR m_pEndPos;
	int m_curLine;
	static DWORD	m_ClassesOfSymbol[256];
	static int		m_SwitchTable[STATE_LAST][CLS_LAST+1];
};
#endif