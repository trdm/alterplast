//////////////////////////////////////////////////////////////////////////
// Extended support for object serialization
// ( «начение¬—троку¬нутр() / «начение»з—троки¬нутр() )
// Copyright (c) kms, 2008-2012
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// Serialization client class provides IV7SerializeToString interface
// and registers serialization handlers by using CSerializeToString::Add()
// CSerializeToString::Add() can throw CSerializeToString::s_exception
//
// Serialization client must conform to rules:
// 1. SaveValue routine: CValue::FastSaveToString(CString&);
// 2. LoadValue routine: LoadValueFromString(CValue*, LPCSTR);
// 3. CBLContext::SaveToString uses CSerializeToString::QuoteString routine
//    to conform to CItemList format
//
// example:
//
// BOOL CSampleContext::SaveToString(CString &str)
// {
// 	str = "class data in free format";
// 	CString csQuotedPrefix = CSerializeToString::QuotePrefix("CSamplePrefix");
// 	CSerializeToString::QuoteString(str, csQuotedPrefix);
// 
// 	return TRUE;
// }
//
// It is essential for szPrefix used for class serialization to be unique
// in the serialization domain for each class and class instance;
//////////////////////////////////////////////////////////////////////////

#if !defined(_V7SerializeToString_H__INCLUDED_)
	#define _V7SerializeToString_H__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "MethodsWrapper.h"
#include <string>

class IV7SerializeToString
{
public:
	virtual BOOL LoadFromString(LPCSTR szSource) = 0;
};

class CSerializeToString
{
public:
	struct s_exception : public std::exception
	{
		s_exception(const char* szWhat = "ќшибка при выполнении метода!") : m_info(szWhat) {}
		const char* what() const { return m_info.c_str(); }

		std::string m_info;
	};

	struct s_record
	{
		s_record(LPCSTR szPrefix, LPCSTR szClassName, LPCSTR szInstanceName)
			: m_csPrefix(szPrefix), m_csClassName(szClassName), m_csInstanceName(szInstanceName)
		{}

		CString m_csPrefix;
		CString m_csClassName;
		CString m_csInstanceName;
	};

	typedef CIStringMap<s_record const*, s_record const*> S_MAP;
private:
	CSerializeToString(CSerializeToString const&);
	CSerializeToString& operator=(CSerializeToString const&);

private:
	static void Setup();
public:
	static void Add(LPCSTR szPrefix, LPCSTR szClassName, LPCSTR szInstanceName);
	static CString QuotePrefix(LPCSTR szPrefix)
	{
		CString csPrefix = "{\"";
		csPrefix += szPrefix;
		csPrefix += "\",\"";

		return csPrefix;
	}
	static void QuoteString(CString& csData, LPCSTR szQuotedPrefix)
	{
		csData.Replace("\"", "\"\"");
		csData.Insert(0, szQuotedPrefix);
		csData += "\"}";
	}

private:
	// CValue handlers
	BOOL FastSaveToString(CString& csTarget);
	BOOL LoadValueFromList(CItemList* pList, BOOL bValidate);

private:
	static CTrapSwap m_trFastSaveToString;
	static CTrapSwap m_trLoadValueFromList;
	static S_MAP m_prefix_map;
};

#endif // _V7SerializeToString_H__INCLUDED_