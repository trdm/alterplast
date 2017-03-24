#ifndef XMLLITEWRITER_H_INCLUDED
#define XMLLITEWRITER_H_INCLUDED

//////////////////////////////////////////////////////////////////////////
// CXmlLiteWriter class (c) kms 2012
//////////////////////////////////////////////////////////////////////////

#include "xmllite.h"
#include "../SimpleContext.h"

typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlWriter)(
	  __in REFIID riid,
	  __out void ** ppvObject,
	  __in_opt IMalloc * pMalloc);
typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlWriterOutputWithEncodingCodePage)(
				 __in IUnknown *pOutputStream,
				 __in_opt IMalloc *pMalloc,
				 __in UINT nEncodingCodePage,
				 __out IXmlWriterOutput **ppOutput);
typedef HRESULT (STDAPICALLTYPE *PF_CreateXmlWriterOutputWithEncodingName)(
	__in IUnknown *pOutputStream,
	__in_opt IMalloc *pMalloc,
	__in LPCWSTR pwszEncodingName,
	__out IXmlWriterOutput **ppOutput);

class CXmlLiteWriter : public CSimpleContext
{
	SIMPLE_DECLARE(CXmlLiteWriter, CSimpleContext)
public:
	I_PROC(procRelease);

	I_PROC(procSetOutput);
	I_PDEF(defsSetOutput);

	I_PROC(procFlush);
	I_PROC(procWriteAttributes);
	I_PROC(procWriteAttributeString);
	I_PDEF(defsWriteAttributeString);
	I_PROC(procWriteCData);
	I_PROC(procWriteCharEntity);
	I_PROC(procWriteChars);
	I_PROC(procWriteComment);
	I_PROC(procWriteDocType);
	I_PDEF(defsWriteDocType);
	I_PROC(procWriteElementString);
	I_PDEF(defsWriteElementString);
	I_PROC(procWriteEndDocument);
	I_PROC(procWriteEndElement);
	I_PROC(procWriteEntityRef);
	I_PROC(procWriteFullEndElement);
	I_PROC(procWriteName);
	I_PROC(procWriteNmToken);
	I_PROC(procWriteNode);
	I_PROC(procWriteNodeShallow);
	I_PROC(procWriteProcessingInstruction);
	I_PROC(procWriteQualifiedName);
	I_PROC(procWriteRaw);
	I_PROC(procWriteRawChars);
	I_PROC(procWriteStartDocument);
	I_PDEF(defsWriteStartDocument);
	I_PROC(procWriteStartElement);
	I_PDEF(defsWriteStartElement);
	I_PROC(procWriteString);
	I_PROC(procWriteSurrogateCharEntity);
	I_PROC(procWriteWhitespace);

	I_GET(getByteOrderMark);
	I_SET(setByteOrderMark);
	I_GET(getConformanceLevel);
	I_SET(setConformanceLevel);
	I_GET(getIndent);
	I_SET(setIndent);
	I_GET(getMultiLanguage);
	I_SET(setMultiLanguage);
	I_GET(getOmitXmlDeclaration);
	I_SET(setOmitXmlDeclaration);

private:
	void aux_SetLink() const;
	void aux_CheckWriter() const;
	IXmlReader* safe_CValue2IXmlReader(CValue const& val) const;

private:
	static PF_CreateXmlWriter m_pfCreateXmlWriter;
	static PF_CreateXmlWriterOutputWithEncodingCodePage m_pfCreateXmlWriterOutputWithEncodingCodePage;
	static PF_CreateXmlWriterOutputWithEncodingName m_pfCreateXmlWriterOutputWithEncodingName;

private:
	CComPtr<IXmlWriter> m_pWriter;
};

#endif
