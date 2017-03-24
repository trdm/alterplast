#ifndef XMLLITEREADER_H_INCLUDED
#define XMLLITEREADER_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CXmlLiteReader class (c) kms 2012
//////////////////////////////////////////////////////////////////////////

#include "xmllite.h"
#include "../SimpleContext.h"

typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlReader)(
	__in REFIID riid,
	__out void ** ppvObject,
	__in_opt IMalloc * pMalloc);
typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlReaderInputWithEncodingCodePage)(
	__in IUnknown * pInputStream,
	__in_opt IMalloc * pMalloc,
	__in UINT nEncodingCodePage,
	__in BOOL fEncodingHint,
	__in_opt LPCWSTR pwszBaseUri,
	__out IXmlReaderInput ** ppInput);
typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlReaderInputWithEncodingName)(
	__in IUnknown * pInputStream,
	__in_opt IMalloc * pMalloc,
	__in LPCWSTR pwszEncodingName,
	__in BOOL fEncodingHint,
	__in_opt LPCWSTR pwszBaseUri,
	__out IXmlReaderInput ** ppInput);

class CXmlLiteReader : public CSimpleContext
{
	SIMPLE_DECLARE(CXmlLiteReader, CSimpleContext)
public:
	I_PROC(procRelease);
	I_PROC(procResetStream);

// 	I_PROC(procCreateFileStream);
// 	I_PROC(procCreateMemoryStream);

	I_PROC(procSetInput);
	I_PDEF(defsSetInput);

	I_FUNC(funcGetAttributeCount);
	I_FUNC(funcGetDepth);
	I_FUNC(funcGetLineNumber);
	I_FUNC(funcGetLinePosition);
	I_FUNC(funcGetLocalName);
	I_FUNC(funcGetNamespaceUri);
	I_FUNC(funcGetNodeType);
	I_FUNC(funcGetPrefix);
	I_FUNC(funcGetQualifiedName);
	I_FUNC(funcGetValue);
	I_FUNC(funcIsDefault);
	I_FUNC(funcIsEmptyElement);
	I_FUNC(funcIsEOF);
	I_FUNC(funcMoveToAttributeByName);
	I_PDEF(defsMoveToAttributeByName);
	I_PROC(procMoveToElement);
	I_FUNC(funcMoveToFirstAttribute);
	I_FUNC(funcMoveToNextAttribute);
	I_FUNC(funcRead);
	I_FUNC(funcReadValueChunk);
	I_PDEF(defsReadValueChunk);

	I_GET(getConformanceLevel);
	I_SET(setConformanceLevel);
	I_GET(getDtdProcessing);
	I_SET(setDtdProcessing);
	I_GET(getMaxElementDepth);
	I_SET(setMaxElementDepth);
	I_GET(getMaxEntityExpansion);
	I_SET(setMaxEntityExpansion);
	I_GET(getMultiLanguage);
	I_SET(setMultiLanguage);
	I_GET(getRandomAccess);
	I_SET(setRandomAccess);
	I_GET(getReadState);
	I_GET(getXmlResolver);
	I_SET(setXmlResolver);

public:
	IXmlReader* GetIXmlReader() { return m_pReader; }

private:
	void aux_SetLink() const;
	void aux_CheckReader() const;

private:
	static PF_CreateXmlReader m_pfCreateXmlReader;
	static PF_CreateXmlReaderInputWithEncodingCodePage m_pfCreateXmlReaderInputWithEncodingCodePage;
	static PF_CreateXmlReaderInputWithEncodingName m_pfCreateXmlReaderInputWithEncodingName;

private:
	CComPtr<IXmlReader> m_pReader;
};

#endif
