// TextBox.h: interface for the CTextBox class.
#if !defined(AFX_TEXTBOX_H__49876F73_EC7C_4C7F_97DA_9A6DDBE3ECB8__INCLUDED_)
#define AFX_TEXTBOX_H__49876F73_EC7C_4C7F_97DA_9A6DDBE3ECB8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
struct SLexem;

class CTextBox  
{
public:
	void AddString(LPCTSTR ptr,int len,void* param);
	void Sort();
	struct STextEntry{
		LPTSTR pStr;
		int len;
		void* param;
	};
	CString GetString(int pos);
	int GetCount();
	void AddString(SLexem& lex,void* param);
	void AddString(CString& str,void* param);
	CTextBox();
	virtual ~CTextBox();

	STextEntry* m_Array;
	STextEntry*	m_pLastEntry;
	STextEntry*	m_pEntry;

	char*		m_text;
	char*		m_pLastText;
	char*		m_pText;
	
	bool		m_IsSorted;
	int			m_count;
	static const char m_LowerTable[257]; // Массив для быстрого перевода строк в нижний регистр
};
#endif
