// Option.h: interface for the COption class.
#if !defined(AFX_OPTION_H__B189B564_4179_4334_B351_B3C0982A6D83__INCLUDED_)
#define AFX_OPTION_H__B189B564_4179_4334_B351_B3C0982A6D83__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COption  
{
public:
	void SetFilterMethods(int num);
	int GetFilterMethods();
	void SetSortMethods(int num);
	int GetSortMethods();
	void SetAddTemplate(int num);
	int GetAddTemplate();
	void SetDisableTemplate(int num);
	int GetDisableTemplate();
	void SetACompleteSymbols(int num);
	int GetACompleteSymbols();
	bool GetUseEnumProc();
	void SetUseEnumProc(bool val);
	COption();
	virtual ~COption();
	int		GetLanguage(){return m_Language;}
	void	SetLanguage(int num);
	int		GetComponents(){return m_Components;}
	void	SetComponents(int num);

	int		GetInt(const CString& path);
	CString GetString(const CString& path);
	void	SetInt(const CString& path,int data);
	void	SetString(const CString& path,const CString& data);

	HKEY m_hKey;
	int m_Language;
	int m_Components;
	int m_UseStdDialog;
	int m_AComplSymb;
	int m_DisableTemplate;
	int m_AddTemplate;
	int m_SortMethods;
	int m_FilterMethods;
	static COption* m_pOption;

};

#endif // !defined(AFX_OPTION_H__B189B564_4179_4334_B351_B3C0982A6D83__INCLUDED_)
