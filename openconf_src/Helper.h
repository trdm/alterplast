// Helper.h: interface for the CHelper class.
#if !defined(AFX_HELPER_H__A4E9952A_ACA7_4C95_93C8_9A3774BA53CB__INCLUDED_)
#define AFX_HELPER_H__A4E9952A_ACA7_4C95_93C8_9A3774BA53CB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CHelper  
{
public:
	static void ShowAbout();
	CHelper();
	virtual ~CHelper();
	static void ShowHelp();
	static HINSTANCE hHtmlHelp;

};

#endif // !defined(AFX_HELPER_H__A4E9952A_ACA7_4C95_93C8_9A3774BA53CB__INCLUDED_)
