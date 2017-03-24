#include "StdAfx.h"
#include "MetaNameParserProxy.h"
#include "MetaNameParser.h"

CMetaNameParserProxy::CMetaNameParserProxy () : m_Parser (new CMetaNameParser)
{
}

void CMetaNameParserProxy::SetParameter (CString const & Name, CValue const & Value)
{
	m_Parser->SetParameter (Name, Value);
}

void CMetaNameParserProxy::SetQueryText (char const* pText)
{
	m_Parser->SetQueryText (pText);
}

bool CMetaNameParserProxy::Parse ()
{
	try {
		m_Parser->Parse ();
	}
	catch (CMNPException *pError) {
		m_LastError = pError->GetErrorDescr();
		return false;
	}

	return true;
}

CString const &CMetaNameParserProxy::GetLastError () const
{
	return m_LastError;
}

const char* CMetaNameParserProxy::GetQueryText() const
{
	return m_Parser->GetQueryText ();
}

bool CMetaNameParserProxy::CValueToDBValue (const CValue & Value,
	const int Modificator, CString & StrValue, bool & IsStrLiteral, CString& SQLType)
{
	return CMetaNameParser::CValueToDBValue (Value, Modificator, StrValue,
		IsStrLiteral, SQLType);
}
