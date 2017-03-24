// Option.cpp: implementation of the COption class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "Option.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
COption& COption::GetOptions()
{
	static COption option;
	return option;
}

COption::COption()
{
	DWORD exist;
	RegCreateKeyEx(HKEY_CURRENT_USER,"Software\\1C\\1Cv7\\7.7\\OpenConf",
		0,NULL,0,KEY_ALL_ACCESS,NULL,&m_hKey,&exist);
}

COption::~COption()
{
	if(m_hKey)
		RegCloseKey(m_hKey);
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

bool COption::Exist(const CString& path)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	bool ret=false;
	if(ERROR_SUCCESS==RegOpenKeyEx(m_hKey,keyName,0,KEY_QUERY_VALUE,&hKey))
	{
		ret=true;
		if(!valName.IsEmpty())
			ret=(RegQueryValueEx(hKey,valName,0,NULL,NULL,NULL)==ERROR_SUCCESS);
		RegCloseKey(hKey);
	}
	return ret;
}

static void RemoveSubKeys(HKEY hKey)
{
	DWORD keys;
	RegQueryInfoKey(hKey,NULL,NULL,NULL,&keys,NULL,NULL,NULL,NULL,NULL,NULL,NULL);
	while(keys>0)
	{
		char buf[MAX_PATH];
		RegEnumKey(hKey,--keys,buf,MAX_PATH);
		HKEY hSubKey;
		RegOpenKeyEx(hKey,buf,0,KEY_ALL_ACCESS,&hSubKey);
		RemoveSubKeys(hSubKey);
		RegDeleteKey(hKey,buf);
	}
	RegCloseKey(hKey);
}

void COption::Delete(const CString& path)
{
	CString keyName,valName;
	SplitKeyVal(path,keyName,valName);
	HKEY hKey;
	if(ERROR_SUCCESS==RegOpenKeyEx(m_hKey,keyName,0,KEY_ALL_ACCESS,&hKey))
	{
		if(!valName.IsEmpty())
			RegDeleteValue(hKey,valName);
		else
		{
			RemoveSubKeys(hKey);
			hKey=NULL;
			int fnd=keyName.ReverseFind('\\');
			if(fnd!=-1)
			{
				RegOpenKeyEx(m_hKey,keyName.Left(fnd),0,KEY_ALL_ACCESS,&hKey);
				keyName=keyName.Mid(fnd+1);
			}
			else
				hKey=m_hKey;
			if(hKey)
			{
				RegDeleteKey(hKey,keyName);
				if(fnd!=-1)
					RegCloseKey(hKey);
			}
		}
	}
}
