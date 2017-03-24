#include "StdAfx.h"
#include "XmlLiteReader.h"
#include "../BinaryData.h"

const char* const szXmlLiteDll = "xmllite.dll";
PF_CreateXmlReader CXmlLiteReader::m_pfCreateXmlReader = NULL;
PF_CreateXmlReaderInputWithEncodingCodePage CXmlLiteReader::m_pfCreateXmlReaderInputWithEncodingCodePage;
PF_CreateXmlReaderInputWithEncodingName CXmlLiteReader::m_pfCreateXmlReaderInputWithEncodingName;

SIMPLE_IMPLEMENT(CXmlLiteReader, CSimpleContext, "1cpp.XmlLiteReader", NULL)

void CXmlLiteReader::InitDef()
{
	AddDef("Release",				"Release",					0, &CXmlLiteReader::procRelease);

// 	AddDef("CreateFileStream",		"CreateFileStream",			1, &CXmlLiteReader::procCreateFileStream);
// 	AddDef("CreateMemoryStream",	"CreateMemoryStream",		1, &CXmlLiteReader::procCreateMemoryStream);

	AddDef("SetInput",				"SetInput",					3, &CXmlLiteReader::procSetInput, &CXmlLiteReader::defsSetInput);
	AddDef("GetValue",				"GetValue",					0, &CXmlLiteReader::funcGetValue);
	AddDef("GetAttributeCount",		"GetAttributeCount",		0, &CXmlLiteReader::funcGetAttributeCount);
	AddDef("GetDepth",				"GetDepth",					0, &CXmlLiteReader::funcGetDepth);
	AddDef("GetLineNumber",			"GetLineNumber",			0, &CXmlLiteReader::funcGetLineNumber);
	AddDef("GetLinePosition",		"GetLinePosition",			0, &CXmlLiteReader::funcGetLinePosition);
	AddDef("GetLocalName",			"GetLocalName",				0, &CXmlLiteReader::funcGetLocalName);
	AddDef("GetNamespaceUri",		"GetNamespaceUri",			0, &CXmlLiteReader::funcGetNamespaceUri);
	AddDef("GetNodeType",			"GetNodeType",				0, &CXmlLiteReader::funcGetNodeType);
	AddDef("GetPrefix",				"GetPrefix",				0, &CXmlLiteReader::funcGetPrefix);
	AddDef("GetQualifiedName",		"GetQualifiedName",			0, &CXmlLiteReader::funcGetQualifiedName);
	AddDef("IsDefault",				"IsDefault",				0, &CXmlLiteReader::funcIsDefault);
	AddDef("IsEmptyElement",		"IsEmptyElement",			0, &CXmlLiteReader::funcIsEmptyElement);
	AddDef("IsEOF",					"IsEOF",					0, &CXmlLiteReader::funcIsEOF);
	AddDef("MoveToAttributeByName",	"MoveToAttributeByName",	2, &CXmlLiteReader::funcMoveToAttributeByName, &CXmlLiteReader::defsMoveToAttributeByName);
	AddDef("MoveToElement",			"MoveToElement",			0, &CXmlLiteReader::procMoveToElement);
	AddDef("MoveToFirstAttribute",	"MoveToFirstAttribute",		0, &CXmlLiteReader::funcMoveToFirstAttribute);
	AddDef("MoveToNextAttribute",	"MoveToNextAttribute",		0, &CXmlLiteReader::funcMoveToNextAttribute);
	AddDef("Read",					"Read",						1, &CXmlLiteReader::funcRead);
	AddDef("ReadValueChunk",		"ReadValueChunk",			3, &CXmlLiteReader::funcReadValueChunk, defsReadValueChunk);

	AddDef("ConformanceLevel",		"ConformanceLevel",			&CXmlLiteReader::getConformanceLevel,		&CXmlLiteReader::setConformanceLevel);
	AddDef("DtdProcessing",			"DtdProcessing",			&CXmlLiteReader::getDtdProcessing,			&CXmlLiteReader::setDtdProcessing);
	AddDef("MaxElementDepth",		"MaxElementDepth",			&CXmlLiteReader::getMaxElementDepth,		&CXmlLiteReader::setMaxElementDepth);
	AddDef("MaxEntityExpansion",	"MaxEntityExpansion",		&CXmlLiteReader::getMaxEntityExpansion,		&CXmlLiteReader::setMaxEntityExpansion);
	AddDef("MultiLanguage",			"MultiLanguage",			&CXmlLiteReader::getMultiLanguage,			&CXmlLiteReader::setMultiLanguage);
	AddDef("RandomAccess",			"RandomAccess",				&CXmlLiteReader::getRandomAccess,			&CXmlLiteReader::setRandomAccess);
	AddDef("ReadState",				"ReadState",				&CXmlLiteReader::getReadState);
	AddDef("XmlResolver",			"XmlResolver",				&CXmlLiteReader::getXmlResolver,			&CXmlLiteReader::setXmlResolver);
}

void CXmlLiteReader::aux_SetLink() const
{
	if (!m_pfCreateXmlReader)
	{
		HMODULE hMod = GetModuleHandle(szXmlLiteDll);
		if (!hMod)
			hMod = LoadLibrary(szXmlLiteDll);
		if (!hMod)
			RuntimeError("Не загружена библиотека %s!", szXmlLiteDll);

		m_pfCreateXmlReaderInputWithEncodingCodePage = (PF_CreateXmlReaderInputWithEncodingCodePage)(GetProcAddress(hMod, "CreateXmlReaderInputWithEncodingCodePage"));
		if (!m_pfCreateXmlReaderInputWithEncodingCodePage)
			RuntimeError("Не удалось получить адрес функции 'CreateXmlReaderInputWithEncodingCodePage'!");

		m_pfCreateXmlReaderInputWithEncodingName = (PF_CreateXmlReaderInputWithEncodingName)(GetProcAddress(hMod, "CreateXmlReaderInputWithEncodingName"));
		if (!m_pfCreateXmlReaderInputWithEncodingName)
			RuntimeError("Не удалось получить адрес функции 'CreateXmlReaderInputWithEncodingName'!");

		m_pfCreateXmlReader = (PF_CreateXmlReader)(GetProcAddress(hMod, "CreateXmlReader"));
		if (!m_pfCreateXmlReader)
			RuntimeError("Не удалось получить адрес функции 'CreateXmlReader'!");
	}
}

void CXmlLiteReader::aux_CheckReader() const
{
	if (!m_pReader)
		RuntimeError("Нет активного источника!");
}

BOOL CXmlLiteReader::procRelease(CValue** ppValues)
{
	m_pReader.Release();
	return TRUE;
}

// параметры:
//	0: szFilename
// BOOL CXmlLiteReader::procCreateFileStream(CValue** ppValues)
// {
// 	m_pStream.Release();
// 	HRESULT hr = SHCreateStreamOnFile(ppValues[0]->GetString().operator LPCSTR(), STGM_READ, &m_pStream);
// 	AssertHResult(hr);
// 
// 	return TRUE;
// }

// параметры:
//	0: szSource
// BOOL CXmlLiteReader::procCreateMemoryStream(CValue** ppValues)
// {
// 	m_pStream.Release();
// 
// 	HRESULT hr = CreateStreamOnHGlobal(NULL, TRUE, &m_pStream);
// 	AssertHResult(hr);
// 
// 	size_t nLen = ppValues[0]->GetString().GetLength();
// 	if (nLen)
// 	{
// 		hr = m_pStream->Write(ppValues[0]->GetString().operator LPCSTR(), nLen, NULL);
// 		AssertHResult(hr);
// 		
// 		ResetStream();
// 	}
// 
// 	return TRUE;
// }

// параметры:
//	0: CBLContext/IStream source (e.g. CBinaryData)
//	1: nEncodingCodePage | pwszEncodingName
//	2: bEncodingHint
BOOL CXmlLiteReader::procSetInput(CValue** ppValues)
{
	aux_SetLink();

	CComPtr<IStream> pStream;

	CBinaryData* pCont = CValue2Type<CBinaryData*>(*ppValues[0]);
	if (pCont)
		pStream = pCont->GetIStream();

	if (!pStream)
		RuntimeError("Нет активного потока!");

	HRESULT hr = S_OK;

	if (!m_pReader)
	{
		hr = (*m_pfCreateXmlReader)(__uuidof(IXmlReader), (void**)&m_pReader, NULL);
		AssertHResult(hr);
	}

	if (ppValues[2]->GetTypeCode() != NUMBER_TYPE_1C)
		RuntimeError("Неверный тип значения хинта кодовой страницы (ожидается число, 0 | 1)!");

	BOOL bEncodingHint = ppValues[2]->GetNumeric().operator long() ? TRUE : FALSE;

	CComPtr<IXmlReaderInput> pInputStream;
	switch (ppValues[1]->GetTypeCode())
	{
	case NUMBER_TYPE_1C:
		{
			UINT nCodePage = ppValues[1]->GetNumeric().operator long();
			if (nCodePage != CP_UTF8 || bEncodingHint)
			{
				hr = (*m_pfCreateXmlReaderInputWithEncodingCodePage)(pStream, NULL, nCodePage, bEncodingHint, NULL, &pInputStream);
				AssertHResult(hr);
			}
		}
		break;
	case STRING_TYPE_1C:
		{
			LPCSTR szName = ppValues[1]->GetString().operator LPCSTR();
			if (FastCompareNoCase.CompareTrimSpc(szName, "UTF-8") || bEncodingHint)
			{
				CComBSTR wzName(szName);
				hr = (*m_pfCreateXmlReaderInputWithEncodingName)(pStream, NULL, wzName, bEncodingHint, NULL, &pInputStream);
				AssertHResult(hr);
			}
		}
		break;
	default:
		RuntimeError("Неверный тип значения кодовой страницы (ожидается число или строка)!");
	}

	hr = m_pReader->SetInput(pInputStream ? pInputStream : pStream);
	AssertHResult(hr);

	return TRUE;
}

BOOL CXmlLiteReader::defsSetInput(int nIndex, CValue* pValue) const
{
	switch (nIndex)
	{
	case 1:
		*pValue = 0L;
		break;
	case 2:
		*pValue = TRUE;
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CXmlLiteReader::funcGetAttributeCount(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	UINT nVal;
	HRESULT hr = m_pReader->GetAttributeCount(&nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetDepth(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	UINT nVal;
	HRESULT hr = m_pReader->GetDepth(&nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetLineNumber(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	UINT nVal;
	HRESULT hr = m_pReader->GetLineNumber(&nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetLinePosition(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	UINT nVal;
	HRESULT hr = m_pReader->GetLineNumber(&nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetLocalName(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	LPCWSTR wsz;
	HRESULT hr = m_pReader->GetLocalName(&wsz, NULL);
	AssertHResult(hr);
	CString csValue(wsz);
	RetValue = csValue.operator LPCTSTR();
	return TRUE;
}

BOOL CXmlLiteReader::funcGetNamespaceUri(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	LPCWSTR wsz;
	HRESULT hr = m_pReader->GetNamespaceUri(&wsz, NULL);
	AssertHResult(hr);
	CString csValue(wsz);
	RetValue = csValue.operator LPCTSTR();
	return TRUE;
}

BOOL CXmlLiteReader::funcGetNodeType(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	XmlNodeType nNodeType;
	HRESULT hr = m_pReader->GetNodeType(&nNodeType);
	AssertHResult(hr);
	RetValue = (LONG)nNodeType;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetPrefix(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	LPCWSTR wsz;
	HRESULT hr = m_pReader->GetPrefix(&wsz, NULL);
	AssertHResult(hr);
	CString csValue(wsz);
	RetValue = csValue.operator LPCTSTR();
	return TRUE;
}

BOOL CXmlLiteReader::funcGetValue(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	LPCWSTR wsz;
	HRESULT hr = m_pReader->GetValue(&wsz, NULL);
	AssertHResult(hr);
	RetValue ="";
	RetValue.m_String = wsz;
	return TRUE;
}

BOOL CXmlLiteReader::funcGetQualifiedName(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	LPCWSTR wsz;
	HRESULT hr = m_pReader->GetQualifiedName(&wsz, NULL);
	AssertHResult(hr);
	CString csValue(wsz);
	RetValue = csValue.operator LPCTSTR();
	return TRUE;
}

BOOL CXmlLiteReader::funcIsDefault(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	RetValue = m_pReader->IsDefault();
	return TRUE;
}

BOOL CXmlLiteReader::funcIsEmptyElement(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	RetValue = m_pReader->IsEmptyElement();
	return TRUE;
}

BOOL CXmlLiteReader::funcIsEOF(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	RetValue = m_pReader->IsEOF();
	return TRUE;
}

// возвращает
// 0 успех | 1 атрибут не найден
BOOL CXmlLiteReader::funcMoveToAttributeByName(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	HRESULT hr = S_OK;
	CComBSTR bstrName(ppValues[0]->GetString().operator LPCTSTR());
	if (ppValues[1]->GetCTypeCode() == UNDEFINED_TYPE_1C)
		hr = m_pReader->MoveToAttributeByName(bstrName, NULL);
	else
	{
		CComBSTR bstrUri(ppValues[1]->GetString().operator LPCTSTR());
		hr = m_pReader->MoveToAttributeByName(bstrName, bstrUri);
	}
	if (FAILED(hr))
		AssertHResult(hr);
	RetValue = hr;
	return TRUE;
}

BOOL CXmlLiteReader::defsMoveToAttributeByName(int nIndex, CValue* pValue) const
{
	if (nIndex == 1)
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
}

BOOL CXmlLiteReader::procMoveToElement(CValue** ppValues)
{
	aux_CheckReader();

	HRESULT hr = m_pReader->MoveToElement();
	AssertHResult(hr);
	return TRUE;
}

// возвращает
// 0 успех | 1 атрибут не найден
BOOL CXmlLiteReader::funcMoveToFirstAttribute(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	HRESULT hr = m_pReader->MoveToFirstAttribute();
	if (FAILED(hr))
		AssertHResult(hr);
	RetValue = hr;
	return TRUE;
}

// возвращает
// 0 успех | 1 атрибут не найден
BOOL CXmlLiteReader::funcMoveToNextAttribute(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	HRESULT hr = m_pReader->MoveToNextAttribute();
	if (FAILED(hr))
		AssertHResult(hr);
	RetValue = hr;
	return TRUE;
}

// параметры
//	0, out: XmlNodeType
// возвращает
//	0 = ОК | 1 = EOF
BOOL CXmlLiteReader::funcRead(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	XmlNodeType nNodeType;
	HRESULT hr = m_pReader->Read(&nNodeType);

	if (FAILED(hr))
		AssertHResult(hr);

	*ppValues[0] = (LONG)(nNodeType);

	RetValue = hr;
	return TRUE;
}

// параметры
//	0, out: szChunk
//	1, in: nChunkSize
//	2, out: nRead
// возвращает
//	0 = ОК | 1 = NOTHING MORE TO READ
BOOL CXmlLiteReader::funcReadValueChunk(CValue& RetValue, CValue** ppValues)
{
	aux_CheckReader();

	UINT nRead;
	UINT nSize = ppValues[1]->GetNumeric().operator long();
	std::vector<WCHAR> vBuf(nSize + 1);

	HRESULT hr = m_pReader->ReadValueChunk(&vBuf[0], nSize, &nRead);
	if (FAILED(hr))
		AssertHResult(hr);

	*ppValues[0] = "";
	ppValues[0]->m_String = &vBuf[0];

	*ppValues[2] = nRead;

	RetValue = hr;
	return TRUE;
}

BOOL CXmlLiteReader::defsReadValueChunk(int nIndex, CValue* pValue) const
{
	if (nIndex == 0 || nIndex == 2) 
	{
		pValue->Reset();
		return TRUE;
	}
	return FALSE;
}

BOOL CXmlLiteReader::getConformanceLevel(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_ConformanceLevel, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::setConformanceLevel(CValue const& Value)
{
	aux_CheckReader();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pReader->SetProperty(XmlReaderProperty_ConformanceLevel, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteReader::getDtdProcessing(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_DtdProcessing, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::setDtdProcessing(CValue const& Value)
{
	aux_CheckReader();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pReader->SetProperty(XmlReaderProperty_DtdProcessing, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteReader::getMaxElementDepth(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_MaxElementDepth, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::setMaxElementDepth(CValue const& Value)
{
	aux_CheckReader();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pReader->SetProperty(XmlReaderProperty_MaxElementDepth, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteReader::getMaxEntityExpansion(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_MaxEntityExpansion, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::setMaxEntityExpansion(CValue const& Value)
{
	aux_CheckReader();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pReader->SetProperty(XmlReaderProperty_MaxEntityExpansion, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteReader::getMultiLanguage(CValue& RetValue) const
{
	RuntimeError("Not implemented!");
	return TRUE;
}

BOOL CXmlLiteReader::setMultiLanguage(CValue const& Value)
{
	RuntimeError("Not implemented!");
	return TRUE;
}

BOOL CXmlLiteReader::getRandomAccess(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_RandomAccess, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::setRandomAccess(CValue const& Value)
{
	aux_CheckReader();

	LONG_PTR nVal = Value.GetNumeric().operator long();
	HRESULT hr = m_pReader->SetProperty(XmlReaderProperty_RandomAccess, nVal);
	AssertHResult(hr);
	return TRUE;
}

BOOL CXmlLiteReader::getReadState(CValue& RetValue) const
{
	aux_CheckReader();

	LONG_PTR nVal;
	HRESULT hr = m_pReader->GetProperty(XmlReaderProperty_ReadState, &nVal);
	AssertHResult(hr);
	RetValue = nVal;
	return TRUE;
}

BOOL CXmlLiteReader::getXmlResolver(CValue& RetValue) const
{
	RuntimeError("Not implemented!");
		return TRUE;
}

BOOL CXmlLiteReader::setXmlResolver(CValue const& Value)
{
	RuntimeError("Not implemented!");
		return TRUE;
}
