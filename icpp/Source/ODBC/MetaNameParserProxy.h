#ifndef META_NAME_PARSER_PROXY_H
#define META_NAME_PARSER_PROXY_H

#pragma once

class CMetaNameParser;

class DLLEXPORT CMetaNameParserProxy
{
	boost::shared_ptr<CMetaNameParser> m_Parser;
	CString m_LastError;
public:
	CMetaNameParserProxy ();
	CString const &GetLastError () const;

	void SetParameter(CString const & Name, CValue const & Value);
	void SetQueryText(char const* pText);
	const char* GetQueryText() const;
	bool Parse();
	static bool CValueToDBValue(const CValue & Value, const int Modificator,
		CString & StrValue, bool & IsStrLiteral, CString& SQLType);
}; // class CMetaNameParserProxy

#endif // META_NAME_PARSER_PROXY_H
