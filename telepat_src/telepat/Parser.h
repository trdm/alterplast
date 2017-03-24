// Parser.h: interface for the CParser class.
#if !defined(AFX_PARSER_H__048C5297_2BE3_46D4_A9B8_16CA8BF5BD2D__INCLUDED_)
#define AFX_PARSER_H__048C5297_2BE3_46D4_A9B8_16CA8BF5BD2D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "blmap.h"
#include "methodparser.h"
#include "keywords.h"
#include "telecont.h"

#include "proccont.h"
#include "varcont.h"
#include "localvarcont.h"
class CMetaGroup;
class CTypeGroup;
class CTForm;
class CTxtTrap;

class CParser  
{
public:
	void ParseParams(CTxtTrap* pEditor);
	void ParseString(CString str);
	int AnalizeLine(SLexem& lexem,CTextDocument* pDoc,int lineNum,int colNum);
	void AutoComplete(CTxtTrap* pEditor);
	void QuickParse(CTxtTrap* pEditor,BYTE curChar);
	CTextDocument* GetDocument();
	int GetMethodText(int line,int col);
	void ParseMethod(int line=-1,int col=-1);
	void ParseModul(int line=-1,int col=-1);
	CParser(CTextDocument* pDoc);
	virtual ~CParser();
	void FillGroup(CPtrArray& arr,int line=-1,int col=-1);
	bool IsGM(){return this==m_GM;}
	DWORD m_dwRef;
	void IncrRef()
	{
		m_dwRef++;
	}
	void DecrRef()
	{
		if(!--m_dwRef)
			delete this;
	}

	CLexer		m_lexParser;
	CMethodParser	m_MethParser;

	SLexem m_curLexem;
	CString m_text;
	CString	m_textMethod;
	int m_state;

	int m_LastParsedFrom;
	int m_LastParsedTo;

	bool m_NeedQuick;

	enum typeOfConts{
		Methods,
		ModuleVars,
		LocalVars,
		MetaAttribs,
		typeLast,
		Members,
	};
	CTextDocument* m_pDocument;
	CTeleCont*	m_Conts[typeLast];
	bool m_AllowedConts[typeLast];


	CTypeGroup*	m_pMetaMethods;
	CTForm* m_pForm;
	long m_ID;
	
	enum typeOfFlags{
		InExpression	=0x1,
		InProc			=0x2,
		NotArrayVar		=0x4,
	};
	DWORD m_flags;
	
	static CParser* m_GM;
	static CKeyWords	m_kwords;
};

#endif // !defined(AFX_PARSER_H__048C5297_2BE3_46D4_A9B8_16CA8BF5BD2D__INCLUDED_)
