// Option.h: interface for the COption class.
#if !defined(AFX_OPTION_H__BC17FEA9_A7D0_42A9_BB35_F576CF7248E3__INCLUDED_)
#define AFX_OPTION_H__BC17FEA9_A7D0_42A9_BB35_F576CF7248E3__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class COption  
{
public:
	COption();
	virtual ~COption();
	int GetInt(const CString& path);
	CString GetString(const CString& path);
	void SetInt(const CString& path,int data);
	void SetString(const CString& path,const CString& data);
	bool Exist(const CString& path);
	void Delete(const CString& path);

	static COption& GetOptions();

	HKEY m_hKey;
};

#endif // !defined(AFX_OPTION_H__BC17FEA9_A7D0_42A9_BB35_F576CF7248E3__INCLUDED_)
