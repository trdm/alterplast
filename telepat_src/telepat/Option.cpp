// Option.cpp: implementation of the COption class.
#include "stdafx.h"
#include "Option.h"
#include "TypesCont.h"

COption* COption::m_pOption=NULL;

COption::COption()
{
	m_pOption=this;
	CString filePath=pConfSvc->IBDir(),ibName;
	HKEY hKey;
	if(RegOpenKeyEx(HKEY_CURRENT_USER,"Software\\1C\\1Cv7\\7.7\\Titles",0,KEY_READ,&hKey)==ERROR_SUCCESS)
	{
		BYTE title[MAX_PATH];
		DWORD size=MAX_PATH;
		if(RegQueryValueEx(hKey,filePath,0,NULL,title,&size)==ERROR_SUCCESS)
			ibName=title;
		RegCloseKey(hKey);
	}
	m_hKey=NULL;
	if(ibName.IsEmpty())
	{
		DoMsgLine("Телепат: не удалось получить имя базы",mmRedErr);
		return;
	}
	filePath="Software\\1C\\1Cv7\\7.7\\";
	filePath=filePath+ibName+"\\telepat";
	DWORD exist;
	RegCreateKeyEx(HKEY_CURRENT_USER,filePath,0,NULL,0,KEY_ALL_ACCESS,NULL,&m_hKey,&exist);
	m_Language=m_Components=m_UseStdDialog=0;
	if(m_hKey)
	{
		DWORD type,size=sizeof(int);
		if(0==RegQueryValueEx(m_hKey,"lang",NULL,&type,(LPBYTE)&m_Language,&size))
		{
			if(type!=REG_DWORD)
				m_Language=0;
			if(m_Language<0 || m_Language>3)
				m_Language=0;
		}
		else
			m_Language=0;
		if(0==RegQueryValueEx(m_hKey,"components",NULL,&type,(LPBYTE)&m_Components,&size))
		{
			if(type!=REG_DWORD)
				m_Components=0;
			if(m_Components<0 || m_Components>7)
				m_Components=0;
		}
		else
			m_Components=0;
		if(0==RegQueryValueEx(m_hKey,"UseStdEnumProc",NULL,&type,(LPBYTE)&m_UseStdDialog,&size))
		{
			if(type==REG_DWORD && m_UseStdDialog)
				m_UseStdDialog=1;
			else
				m_UseStdDialog=0;
		}
	}
	if(!m_Language)
	{
		CTaskDef* pDef=pMetaDataCont->GetTaskDef();
		SetLanguage(pDef->GetDefaultLanguage()+1);
	}
	if(!m_Components)
	{
		if(pMetaDataCont->GetNRegDefs())
			m_Components|=1;
		if(pMetaDataCont->GetNCJDefs())
			m_Components|=4;
		CBuhDef* pDef=pMetaDataCont->GetBuhDef();
		if(pDef)
		{
			if(pDef->GetNPlanDefs())
				m_Components|=2;
		}
		if(GetModuleHandle("distrdb.dll"))
			m_Components|=8;
	}

	m_AComplSymb=GetInt("AutoCompleteSymb")+1;
	if(m_AComplSymb<0)
		m_AComplSymb=0;
	else if(m_AComplSymb>9)
		m_AComplSymb=9;
	m_DisableTemplate=GetInt("DisableTemplate");
	if(m_DisableTemplate<0)
		m_DisableTemplate=0;
	else if(m_DisableTemplate>1)
		m_DisableTemplate=1;
	m_AddTemplate=GetInt("AddTemplate");
	m_SortMethods=GetInt("EnumMethDlg\\NoSort");
	m_FilterMethods=GetInt("EnumMethDlg\\Filter");
}

COption::~COption()
{
}

void COption::SetLanguage(int num)
{
	m_Language=num;
	if(m_hKey)
		RegSetValueEx(m_hKey,"lang",0,REG_DWORD,(LPBYTE)&m_Language,sizeof(m_Language));
}

void COption::SetComponents(int num)
{
	if(num<0 || num>15)
		return;
	m_Components=num;
	if(m_hKey)
		RegSetValueEx(m_hKey,"components",0,REG_DWORD,(LPBYTE)&m_Components,sizeof(m_Components));
	CTypesCont::m_pOneCont->LoadTypes(num);
}

void COption::SetUseEnumProc(bool val)
{
	m_UseStdDialog=val?1:0;
	if(m_hKey)
		RegSetValueEx(m_hKey,"UseStdEnumProc",0,REG_DWORD,(LPBYTE)&m_UseStdDialog,sizeof(m_UseStdDialog));
}

bool COption::GetUseEnumProc()
{
	return m_UseStdDialog!=0;
}


static inline void SplitKeyVal(const CString& str,CString& key,CString& val)
{
	int fnd=str.ReverseFind('\\');
	if(fnd>-1)
	{
		key=str.Left(fnd);
		val=str.Mid(fnd+1);
	}
	else
	{
		key=str;
		val.Empty();
	}
}

int COption::GetInt(const CString& path)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	int ret=0;
	if(ERROR_SUCCESS==RegOpenKeyEx(m_hKey,keyName,0,KEY_QUERY_VALUE,&hKey))
	{
		DWORD type,size=sizeof(int);
		if(ERROR_SUCCESS==RegQueryValueEx(hKey,valName,0,&type,(BYTE*)&ret,&size))
		{
			if(type!=REG_DWORD)
				ret=0;
		}
		RegCloseKey(hKey);
	}
	return ret;
}

CString COption::GetString(const CString& path)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	CString ret;
	if(ERROR_SUCCESS==RegOpenKeyEx(m_hKey,keyName,0,KEY_QUERY_VALUE,&hKey))
	{
		DWORD type,size=0;
		if(ERROR_SUCCESS==RegQueryValueEx(hKey,valName,0,&type,NULL,&size))
		{
			if(type==REG_SZ)
			{
				RegQueryValueEx(hKey,valName,0,&type,(BYTE*)ret.GetBuffer(size-1),&size);
				ret.ReleaseBuffer();
			}
		}
		RegCloseKey(hKey);
	}
	return ret;
}

void COption::SetInt(const CString& path,int data)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	DWORD exist;
	if(ERROR_SUCCESS==RegCreateKeyEx(m_hKey,keyName,
		0,NULL,0,KEY_ALL_ACCESS,NULL,&hKey,&exist))
	{
		RegSetValueEx(hKey,valName,0,REG_DWORD,(BYTE*)&data,sizeof(int));
		RegCloseKey(hKey);
	}
}

void COption::SetString(const CString& path,const CString& data)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	DWORD exist;
	if(ERROR_SUCCESS==RegCreateKeyEx(m_hKey,keyName,
		0,NULL,0,KEY_ALL_ACCESS,NULL,&hKey,&exist))
	{
		RegSetValueEx(hKey,valName,0,REG_SZ,(BYTE*)(LPCTSTR)data,data.GetLength()+1);
		RegCloseKey(hKey);
	}
}

int COption::GetACompleteSymbols()
{
	return m_AComplSymb;
}

void COption::SetACompleteSymbols(int num)
{
	if(num<0)
		num=0;
	else if(num>9)
		num=9;
	if(num==m_AComplSymb)
		return;
	SetInt("AutoCompleteSymb",(m_AComplSymb=num)-1);
}

int COption::GetDisableTemplate()
{
	return m_DisableTemplate;
}

void COption::SetDisableTemplate(int num)
{
	num&=3;
	if(num==m_DisableTemplate)
		return;
	SetInt("DisableTemplate",m_DisableTemplate=num);
}

int COption::GetAddTemplate()
{
	return m_AddTemplate;
}

void COption::SetAddTemplate(int num)
{
	if(num!=m_AddTemplate)
	{
		SetInt("AddTemplate",num);
		m_AddTemplate=num;
	}
}

int COption::GetSortMethods()
{
	return m_SortMethods;
}

void COption::SetSortMethods(int num)
{
	if(num!=m_SortMethods)
	{
		m_SortMethods=num;
		SetInt("EnumMethDlg\\NoSort",num);
	}
}

int COption::GetFilterMethods()
{
	return m_FilterMethods;
}

void COption::SetFilterMethods(int num)
{
	if(num!=m_FilterMethods)
	{
		m_FilterMethods=num;
		SetInt("EnumMethDlg\\Filter",num);
	}
}
