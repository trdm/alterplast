// vs6 specialized function remapper
// whar_t / unsigned short remapper
// copyright (c) kms, 2008-2020

#if !defined FORWARDER_INCLUDED
#define FORWARDER_INCLUDED

#define FORWARDER_DEFINE_TYPES
#include "forwarder_n.h"

//////////////////////////////////////////////////////////////////////////
// comsupp.lib
//////////////////////////////////////////////////////////////////////////

namespace _com_util
{
	char* __stdcall ConvertBSTRToString(EX_BSTR pSrc) throw(_com_error);
	EX_BSTR __stdcall ConvertStringToBSTR(const char* pSrc) throw(_com_error);
};

//////////////////////////////////////////////////////////////////////////
// mfc42.lib
//////////////////////////////////////////////////////////////////////////

class CString
{
public:
	CString(EX_LPCWSTR lpsz);
	EX_BSTR AllocSysString() const;
	const CString& operator=(EX_LPCWSTR lpsz);
};

class CWnd
{
public:
	BOOL CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
		const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist,
		BOOL bStorage, EX_BSTR bstrLicKey);
};

#endif
