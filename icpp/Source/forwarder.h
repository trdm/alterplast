// vs6 specialized function remapper
// whar_t / unsigned short remapper
// copyright (c) kms, 2008-2020

#if !defined FORWARDER_INCLUDED
#define FORWARDER_INCLUDED

#include <afxwin.h>
#include <afxtempl.h>
#include <comdef.h>
#include "Forwarder/forwarder_n.h"

namespace _com_util
{
	char* __stdcall ConvertBSTRToString(BSTR pSrc) throw(_com_error)
	{
		return N::_com_util::ConvertBSTRToString(pSrc);
	}
	BSTR __stdcall ConvertStringToBSTR(const char* pSrc) throw(_com_error)
	{
		return N::_com_util::ConvertStringToBSTR(pSrc);
	}
};

UINT AFXAPI HashKey(LPCSTR key);
template<>
UINT AFXAPI HashKey(LPCSTR key)
{
	return HashKey(key);
}

void AFXAPI SerializeElements(CArchive& ar, CString* pElements, INT_PTR nCount);
template<>
void AFXAPI SerializeElements(CArchive& ar, CString* pElements, INT_PTR nCount)
{
	SerializeElements(ar, pElements, nCount);
}

void AFXAPI ConstructElements(CString* pElements, INT_PTR nCount);
template<>
void AFXAPI ConstructElements(CString* pElements, INT_PTR nCount)
{
	ConstructElements(pElements, nCount);
}

void AFXAPI DestructElements(CString* pElements, INT_PTR nCount);
template<>
void AFXAPI DestructElements<CString>(CString* pElements, INT_PTR nCount)
{
	DestructElements(pElements, nCount);
}

CString::CString(LPCWSTR lpsz)
{
	new (this) N::CString(lpsz);
}

BSTR CString::AllocSysString() const
{
	N::CString const* pT = reinterpret_cast<N::CString const*>(this);
	return pT->AllocSysString();
}

const CString& CString::operator=(LPCWSTR lpsz)
{
	N::CString* pT = reinterpret_cast<N::CString*>(this);
	return pT->operator=(lpsz);
}

BOOL CWnd::CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
						 const RECT& rect, CWnd* pParentWnd, UINT nID, CFile* pPersist,
						 BOOL bStorage, BSTR bstrLicKey)
{
	N::CWnd* pT = reinterpret_cast<N::CWnd*>(this);
	return pT->CreateControl(pszClass, pszWindowName, dwStyle,
		rect, pParentWnd, nID, pPersist,
		bStorage, bstrLicKey);
}

#endif
