#include "StdAfx.h"
#include "XmlLiteReader.h"
#include "XmlLiteWriter.h"
#include "../BinaryData.h"

const char* const szXmlLiteDll = "xmllite.dll";
PF_CreateXmlWriter CXmlLiteWriter::m_pfCreateXmlWriter = NULL;
PF_CreateXmlWriterOutputWithEncodingCodePage CXmlLiteWriter::m_pfCreateXmlWriterOutputWithEncodingCodePage = NULL;
PF_CreateXmlWriterOutputWithEncodingName CXmlLiteWriter::m_pfCreateXmlWriterOutputWithEncodingName = NULL;

SIMPLE_IMPLEMENT(CXmlLiteWriter, CSimpleContext, "1cpp.XmlLiteWriter", NULL)

void CXmlLiteWriter::InitDef()
{
	AddDef("Release",				"Release",					0, &CXmlLiteWriter::procRelease);

	AddDef("SetOutput",				"SetOutput",				2, &CXmlLiteWriter::procSetOutput, &CXmlLiteWriter::defsSetOutput);
	AddDef("Flush",					"Flush",					0, &CXmlLiteWriter::procFlush);
	AddDef("WriteAttributes",		"WriteAttributes",			2, &CXmlLiteWriter::procWriteAttributes);
	AddDef("WriteAttributeString",	"WriteAttributeString",		4, &CXmlLiteWriter::procWriteAttributeString, &CXmlLiteWriter::defsWriteAttributeString);
	AddDef("WriteCData",			"WriteCData",				1, &CXmlLiteWriter::procWriteCData);
	AddDef("WriteCharEntity",		"WriteCharEntity",			1, &CXmlLiteWriter::procWriteCharEntity);
	AddDef("WriteChars",			"WriteChars",				2, &CXmlLiteWriter::procWriteChars);
	AddDef("WriteComment",			"WriteComment",				1, &CXmlLiteWriter::procWriteComment);
	AddDef("WriteDocType",			"WriteDocType",				4, &CXmlLiteWriter::procWriteDocType, &CXmlLiteWriter::defsWriteDocType);
	AddDef("WriteElementString",	"WriteElementString",		4, &CXmlLiteWriter::procWriteElementString, &CXmlLiteWriter::defsWriteElementString);
	AddDef("WriteEndDocument",		"WriteEndDocument",			0, &CXmlLiteWriter::procWriteEndDocument);
	AddDef("WriteEndElement",		"WriteEndElement",			0, &CXmlLiteWriter::procWriteEndElement);
	AddDef("WriteEntityRef",		"WriteEntityRef",			1, &CXmlLiteWriter::procWriteEntityRef);
	AddDef("WriteFullEndElement",	"WriteFullEndElement",		0, &CXmlLiteWriter::procWriteFullEndElement);
	AddDef("WriteName",				"WriteName",				1, &CXmlLiteWriter::procWriteName);
	AddDef("WriteNmToken",			"WriteNmToken",				1, &CXmlLiteWriter::procWriteNmToken);
	AddDef("WriteNode",				"WriteNode",				2, &CXmlLiteWriter::procWriteNode);
	AddDef("WriteNodeShallow",		"WriteNodeShallow",			2, &CXmlLiteWriter::procWriteNodeShallow);
	AddDef("WriteProcessingInstruction", "WriteProcessingInstruction", 2, &CXmlLiteWriter::procWriteProcessingInstruction);
	AddDef("WriteQualifiedName",	"WriteQualifiedName",		2, &CXmlLiteWriter::procWriteQualifiedName);
	AddDef("WriteRaw",				"WriteRaw",					1, &CXmlLiteWriter::procWriteRaw);
	AddDef("WriteRawChars",			"WriteRawChars",			2, &CXmlLiteWriter::procWriteRawChars);
	AddDef("WriteStartDocument",	"WriteStartDocument",		1, &CXmlLiteWriter::procWriteStartDocument, &CXmlLiteWriter::defsWriteStartDocument);
	AddDef("WriteStartElement",		"WriteStartElement",		3, &CXmlLiteWriter::procWriteStartElement, &CXmlLiteWriter::defsWriteStartElement);
	AddDef("WriteString",			"WriteString",				1, &CXmlLiteWriter::procWriteString);
	AddDef("WriteSurrogateCharEntity", "WriteSurrogateCharEntity", 2, &CXmlLiteWriter::procWriteSurrogateCharEntity);
	AddDef("WriteWhitespace",		"WriteWhitespace",			2, &CXmlLiteWriter::procWriteWhitespace);

	AddDef("ByteOrderMark",			"ByteOrderMark",			&CXmlLiteWriter::getByteOrderMark,		&CXmlLiteWriter::setByteOrderMark);
	AddDef("ConformanceLevel",		"ConformanceLevel",			&CXmlLiteWriter::getConformanceLevel,	&CXmlLiteWriter::setConformanceLevel);
	AddDef("Indent",				"Indent",					&CXmlLiteWriter::getIndent,				&CXmlLiteWriter::setIndent);
	AddDef("MultiLanguage",			"MultiLanguage",			&CXmlLiteWriter::getMultiLanguage,		&CXmlLiteWriter::setMultiLanguage);
	AddDef("OmitXmlDeclaration",	"OmitXmlDeclaration",		&CXmlLiteWriter::getOmitXmlDeclaration,	&CXmlLiteWriter::setOmitXmlDeclaration);
}

void CXmlLiteWriter::aux_SetLink() const
{
	if (!m_pfCreateXmlWriter)
	{
		HMODULE hMod = GetModuleHandle(szXmlLiteDll);
		if (!hMod)
			hMod = LoadLibrary(szXmlLiteDll);
		if (!hMod)
			RuntimeError("Ќе загружена библиотека %s!", szXmlLiteDll);

		m_pfCreateXmlWriterOutputWithEncodingCodePage = (PF_CreateXmlWriterOutputWithEncodingCodePage)(GetProcAddress(hMod, "CreateXmlWriterOutputWithEncodingCodePage"));
		if (!m_pfCreateXmlWriterOutputWithEncodingCodePage)
			RuntimeError("Ќе удалось получить адрес функции 'CreateXmlWriterOutputWithEncodingCodePage'!");

		m_pfCreateXmlWriterOutputWithEncodingName = (PF_CreateXmlWriterOutputWithEncodingName)(GetProcAddress(hMod, "CreateXmlWriterOutputWithEncodingName"));
		if (!m_pfCreateXmlWriterOutputWithEncodingName)
			RuntimeError("Ќе удалось получить адрес функции 'CreateXmlWriterOutputWithEncodingName'!");

		m_pfCreateXmlWriter = (PF_CreateXmlWriter)(GetProcAddress(hMod, "CreateXmlWriter"));
		if (!m_pfCreateXmlWriter)
			RuntimeError("Ќе удалось получить адрес функции 'CreateXmlWriter'!");
	}
}

void CXmlLiteWriter::aux_CheckWriter() const
{
	if (!m_pWriter)
		RuntimeError("Ќет активного приемника!");
}

IXmlReader* CXmlLiteWriter::safe_CValue2IXmlReader(CValue const& val) const
{
	IXmlReader* pReader = NULL;

	CXmlLiteReader* pCont = CValue2Type<CXmlLiteReader*>(val);
	if (pCont)
		pReader = pCont->GetIXmlReader();

	if (!pReader)
		RuntimeError("Ќет активного источника!");

	return pReader;
}

BOOL CXmlLiteWriter::procRelease(CValue** ppValues)
{
	m_pWriter.Release();
	return TRUE;
}

// параметры:
//	0: szFileName
//	1: nEncodingCodePage | pwszEncodingName
BOOL CXmlLiteWriter::procSetOutput(CValue** ppValues)
{
	aux_SetLink();

	CComPtr<IStream> pStream;

	CBinaryData* pCont = CValue2Type<CBinaryData*>(*ppValues[0]);
	if (pCont)
		pStream = pCont->GetIStream();

	if (!pStream)
		RuntimeError("Ќет активного потока!");

	HRESULT hr = S_OK;

	if (!m_pWriter)
	{
		hr = (*m_pfCreateXmlWriter)(__uuidof(IXmlWriter), (void**)&m_pWriter, NULL);
		AssertHResult(hr);
	}

	CComPtr<IXmlWriterOutput> pOutputStream;
	switch (ppValues[1]->GetTypeCode())
	{
	case NUMBER_TYPE_1C:
		{
			UINT nCodePage = ppValues[1]->GetNumeric().operator long();
			if (nCodePage != CP_UTF8)
			{
				hr = (*m_pfCreateXmlWriterOutputWithEncodingCodePage)(pStream, NULL, nCodePage, &pOutputStream);
				AssertHResult(hr);
			}
		}
		break;
	case STRING_TYPE_1C:
		{
			LPCSTR szName = ppValues[1]->GetString().operator LPCSTR();
			if (FastCompareNoCase.CompareTrimSpc(szName, "UTF-8"))
			{
				CComBSTR wzName(szName);
				hr = (*m_pfCreateXmlWriterOutputWithEncodingName)(pStream, NULL, wzName, &pOutputStream);
				AssertHResult(hr);
			}
		}
		break;
	default:
		RuntimeError("Ќеверный тип значени€ кодовой страницы (ожидаетс€ число или строка)!");
	}

	hr = m_pWriter->SetOutput(pOutputStream ? pOutputStream : pStream);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsSetOutput(int nIndex, CValue* pValue) const
{
	if (nIndex == 1)
	{
		*pValue = CP_UTF8;
		return TRUE;
	}
	return FALSE;
}

BOOL CXmlLiteWriter::procFlush(CValue** ppValues)
{
	aux_CheckWriter();

	HRESULT hr = m_pWriter->Flush();
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: CBLContext / IXmlReader source
//	1: bWriteDefaultAttributes
BOOL CXmlLiteWriter::procWriteAttributes(CValue** ppValues)
{
	CComPtr<IXmlReader> pReader(safe_CValue2IXmlReader(*ppValues[0]));
	BOOL bWriteDefaultAttributes = ppValues[1]->GetNumeric().operator long() ? TRUE : FALSE;

	HRESULT hr = m_pWriter->WriteAttributes(pReader, bWriteDefaultAttributes);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: Prefix
//	1: LocalName
//	2: NamespaceUri
//	3: Value
BOOL CXmlLiteWriter::procWriteAttributeString(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrPrefix (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrLocalName (ppValues[1]->GetString().operator LPCTSTR());
	CComBSTR bstrNamespaceUri (ppValues[2]->GetString().operator LPCTSTR());
	CComBSTR bstrValue (ppValues[3]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteAttributeString(bstrPrefix, bstrLocalName, bstrNamespaceUri, bstrValue);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsWriteAttributeString(int nIndex, CValue* pValue) const
{
	if (nIndex == 0 || nIndex == 2)
	{
		*pValue = "";
		return TRUE;
	}
	return FALSE;
}

// параметры:
//	0: szText
BOOL CXmlLiteWriter::procWriteCData(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrText (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteCData(bstrText);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: character
BOOL CXmlLiteWriter::procWriteCharEntity(CValue** ppValues)
{
	aux_CheckWriter();

	WCHAR wChar = ppValues[0]->GetNumeric().operator long();
	HRESULT hr = m_pWriter->WriteCharEntity(wChar);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szText
//	1: nLen
BOOL CXmlLiteWriter::procWriteChars(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrText (ppValues[0]->GetString().operator LPCTSTR());
	UINT nLen = ppValues[0]->GetNumeric().operator long();

	HRESULT hr = m_pWriter->WriteChars(bstrText, nLen);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szText
BOOL CXmlLiteWriter::procWriteComment(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrText (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteComment(bstrText);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szName
//	1: szPublicId
//	2: szSystemId
//	3: szSubset
BOOL CXmlLiteWriter::procWriteDocType(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrName (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrPublicId (ppValues[1]->GetString().operator LPCTSTR());
	CComBSTR bstrSystemId(ppValues[2]->GetString().operator LPCTSTR());
	CComBSTR bstrSubset (ppValues[3]->GetString().operator LPCTSTR());

	BSTR wszPublicId = ppValues[1]->GetTypeCode() == UNDEFINED_TYPE_1C ? NULL : bstrPublicId.operator BSTR();
	BSTR wszSystemID = ppValues[2]->GetTypeCode() == UNDEFINED_TYPE_1C ? NULL : bstrSystemId.operator BSTR();
	BSTR wszSubset = ppValues[3]->GetTypeCode() == UNDEFINED_TYPE_1C ? NULL : bstrSubset.operator BSTR();

	HRESULT hr = m_pWriter->WriteDocType(bstrName, wszPublicId, wszSystemID, wszSubset);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsWriteDocType(int nIndex, CValue* pValue) const
{
	if (nIndex > 0)
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
}

// параметры:
//	0: Prefix
//	1: LocalName
//	2: NamespaceUri
//	3: Value
BOOL CXmlLiteWriter::procWriteElementString(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrPrefix (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrLocalName (ppValues[1]->GetString().operator LPCTSTR());
	CComBSTR bstrNamespaceUri (ppValues[2]->GetString().operator LPCTSTR());
	CComBSTR bstrValue (ppValues[3]->GetString().operator LPCTSTR());

	BSTR wszValue = ppValues[3]->GetTypeCode() == UNDEFINED_TYPE_1C ? NULL : bstrValue.operator BSTR();

	HRESULT hr = m_pWriter->WriteElementString(bstrPrefix, bstrLocalName, bstrNamespaceUri, wszValue);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsWriteElementString(int nIndex, CValue* pValue) const
{
	if (nIndex == 0 || nIndex == 2)
	{
		*pValue = "";
		return TRUE;
	}
	else if (nIndex == 3)
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
}

BOOL CXmlLiteWriter::procWriteEndDocument(CValue** ppValues)
{
	aux_CheckWriter();

	HRESULT hr = m_pWriter->WriteEndDocument();
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::procWriteEndElement(CValue** ppValues)
{
	aux_CheckWriter();

	HRESULT hr = m_pWriter->WriteEndElement();
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szName
BOOL CXmlLiteWriter::procWriteEntityRef(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrName (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteEntityRef(bstrName);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::procWriteFullEndElement(CValue** ppValues)
{
	aux_CheckWriter();

	HRESULT hr = m_pWriter->WriteFullEndElement();
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szName
BOOL CXmlLiteWriter::procWriteName(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrName (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteName(bstrName);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szNmToken
BOOL CXmlLiteWriter::procWriteNmToken(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrNmToken (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteNmToken(bstrNmToken);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: CBLContext / IXmlReader source
//	1: bWriteDefaultAttributes
BOOL CXmlLiteWriter::procWriteNode(CValue** ppValues)
{
	CComPtr<IXmlReader> pReader(safe_CValue2IXmlReader(*ppValues[0]));
	BOOL bWriteDefaultAttributes = ppValues[1]->GetNumeric().operator long() ? TRUE : FALSE;

	HRESULT hr = m_pWriter->WriteNode(pReader, bWriteDefaultAttributes);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: CBLContext / IXmlReader source
//	1: bWriteDefaultAttributes
BOOL CXmlLiteWriter::procWriteNodeShallow(CValue** ppValues)
{
	CComPtr<IXmlReader> pReader(safe_CValue2IXmlReader(*ppValues[0]));
	BOOL bWriteDefaultAttributes = ppValues[1]->GetNumeric().operator long() ? TRUE : FALSE;

	HRESULT hr = m_pWriter->WriteNodeShallow(pReader, bWriteDefaultAttributes);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szName
//	1: szText
BOOL CXmlLiteWriter::procWriteProcessingInstruction(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrName (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrText (ppValues[1]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteProcessingInstruction(bstrName, bstrText);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szLocalName
//	1: szNamespaceUri
BOOL CXmlLiteWriter::procWriteQualifiedName(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrLocalName (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrNamespaceUri (ppValues[1]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteQualifiedName(bstrLocalName, bstrNamespaceUri);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szData
BOOL CXmlLiteWriter::procWriteRaw(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrData (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteRaw(bstrData);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szText
//	1: nLen
BOOL CXmlLiteWriter::procWriteRawChars(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrText (ppValues[0]->GetString().operator LPCTSTR());
	UINT nLen = ppValues[0]->GetNumeric().operator long();

	HRESULT hr = m_pWriter->WriteRawChars(bstrText, nLen);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: XmlStandalone
BOOL CXmlLiteWriter::procWriteStartDocument(CValue** ppValues)
{
	aux_CheckWriter();

	XmlStandalone nVal = static_cast<XmlStandalone>(ppValues[0]->GetNumeric().operator long());
	HRESULT hr = m_pWriter->WriteStartDocument(nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsWriteStartDocument(int nIndex, CValue* pValue) const
{
	if (nIndex == 0)
	{
		pValue = 0L;
		return TRUE;
	}
	return FALSE;
}

// параметры:
//	0: Prefix
//	1: LocalName
//	2: NamespaceUri
BOOL CXmlLiteWriter::procWriteStartElement(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrPrefix (ppValues[0]->GetString().operator LPCTSTR());
	CComBSTR bstrLocalName (ppValues[1]->GetString().operator LPCTSTR());
	CComBSTR bstrNamespaceUri (ppValues[2]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteStartElement(bstrPrefix, bstrLocalName, bstrNamespaceUri);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::defsWriteStartElement(int nIndex, CValue* pValue) const
{
	if (nIndex == 0 || nIndex == 2)
	{
		*pValue = "";
		return TRUE;
	}
	return FALSE;
}

// параметры:
//	0: szText
BOOL CXmlLiteWriter::procWriteString(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrText(ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteString(bstrText);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: lowChar
//	0: highChar
BOOL CXmlLiteWriter::procWriteSurrogateCharEntity(CValue** ppValues)
{
	aux_CheckWriter();

	WCHAR lowChar = ppValues[0]->GetNumeric().operator long();
	WCHAR highChar = ppValues[1]->GetNumeric().operator long();

	HRESULT hr = m_pWriter->WriteSurrogateCharEntity(lowChar, highChar);
	AssertHResult(hr);
	return TRUE;
}

// параметры:
//	0: szWhiteSpace
BOOL CXmlLiteWriter::procWriteWhitespace(CValue** ppValues)
{
	aux_CheckWriter();

	CComBSTR bstrWhitespace (ppValues[0]->GetString().operator LPCTSTR());

	HRESULT hr = m_pWriter->WriteWhitespace(bstrWhitespace);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::getByteOrderMark(CValue& RetValue) const
{
	aux_CheckWriter();

	LONG_PTR nVal;
	HRESULT hr = m_pWriter->GetProperty(XmlWriterProperty_ByteOrderMark, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteWriter::setByteOrderMark(CValue const& Value)
{
	aux_CheckWriter();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pWriter->SetProperty(XmlWriterProperty_ByteOrderMark, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::getConformanceLevel(CValue& RetValue) const
{
	aux_CheckWriter();

	LONG_PTR nVal;
	HRESULT hr = m_pWriter->GetProperty(XmlWriterProperty_ConformanceLevel, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteWriter::setConformanceLevel(CValue const& Value)
{
	aux_CheckWriter();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pWriter->SetProperty(XmlWriterProperty_ConformanceLevel, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::getIndent(CValue& RetValue) const
{
	aux_CheckWriter();

	LONG_PTR nVal;
	HRESULT hr = m_pWriter->GetProperty(XmlWriterProperty_Indent, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteWriter::setIndent(CValue const& Value)
{
	aux_CheckWriter();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pWriter->SetProperty(XmlWriterProperty_Indent, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteWriter::getMultiLanguage(CValue& RetValue) const
{
	RuntimeError("Not implemented!");
	return TRUE;
}

BOOL CXmlLiteWriter::setMultiLanguage(CValue const& Value)
{
	RuntimeError("Not implemented!");
	return TRUE;
}

BOOL CXmlLiteWriter::getOmitXmlDeclaration(CValue& RetValue) const
{
	aux_CheckWriter();

	LONG_PTR nVal;
	HRESULT hr = m_pWriter->GetProperty(XmlWriterProperty_OmitXmlDeclaration, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteWriter::setOmitXmlDeclaration(CValue const& Value)
{
	aux_CheckWriter();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pWriter->SetProperty(XmlWriterProperty_OmitXmlDeclaration, nVal);
	AssertHResult(hr);
	return TRUE;
}
