// KeyWords.h: interface for the CKeyWords class.

#if !defined(AFX_KEYWORDS_H__899D83E0_57C6_49AE_965D_F4FB8BF67B35__INCLUDED_)
#define AFX_KEYWORDS_H__899D83E0_57C6_49AE_965D_F4FB8BF67B35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CKeyWords  
{
public:
	enum eKeyWords
	{
		kwAnd,
		kwBreak,
		kwContext,
		kwContinue,
		kwDo,
		kwElse,
		kwElsIf,
		kwEndDo,
		kwEndFunc,
		kwEndIf,
		kwEndProc,
		kwEndTry,
		kwExcept,
		kwExport,
		kwFor,
		kwForward,
		kwFunc,
		kwGoto,
		kwIf,
		kwNot,
		kwOr,
		kwProc,
		kwReturn,
		kwThen,
		kwTo,
		kwTry,
		kwVal,
		kwVar,
		kwWhile,
		kwLast
	};

	CKeyWords();
	void SetWord(char w){m_allowedWords|=(1<<w);}
	void FillGroup(CPtrArray& arr);
	DWORD m_allowedWords;
	struct SKWInfo{
		LPCTSTR name[2];
		LPCTSTR descr;
		LPCTSTR delim;
		bool NeedCR;
	};
	struct SKWTemplates{
		LPCTSTR name[2];
		LPCTSTR insert[2];
		LPCTSTR descr;
	};

	static SKWInfo keyWords[kwLast];
	static SKWTemplates keyTemplates[];
};

#endif // !defined(AFX_KEYWORDS_H__899D83E0_57C6_49AE_965D_F4FB8BF67B35__INCLUDED_)

