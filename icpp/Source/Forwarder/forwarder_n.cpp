// vs6 specialized function remapper
// whar_t / unsigned short remapper
// copyright (c) kms, 2008-2020

#include "stdafx.h"
#include "forwarder_ex.h"
#include <new>

namespace N
{
	namespace _com_util
	{
		char* __stdcall ConvertBSTRToString(BSTR pSrc) throw(_com_error)
		{
			return ::_com_util::ConvertBSTRToString(reinterpret_cast<EX_BSTR>(pSrc));
		}

		BSTR __stdcall ConvertStringToBSTR(const char* pSrc) throw(_com_error)
		{
			return reinterpret_cast<BSTR>(::_com_util::ConvertStringToBSTR(pSrc));
		}
	}

	CString::CString(LPCWSTR lpsz)
	{
		new (this) ::CString(reinterpret_cast<EX_LPCWSTR>(lpsz));
	}

	BSTR CString::AllocSysString() const
	{
		::CString const* pT = reinterpret_cast<::CString const*>(this);
		return reinterpret_cast<BSTR>(pT->AllocSysString());
	}

	const ::CString& CString::operator=(LPCWSTR lpsz)
	{
		::CString* pT = reinterpret_cast<::CString*>(this);
		return pT->operator=(reinterpret_cast<EX_LPCWSTR>(lpsz));
	}

	BOOL CWnd::CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
		const RECT& rect, ::CWnd* pParentWnd, UINT nID, ::CFile* pPersist,
		BOOL bStorage, BSTR bstrLicKey)
	{
		::CWnd* pT = reinterpret_cast<::CWnd*>(this);
		return pT->CreateControl(pszClass, pszWindowName, dwStyle,
			rect, pParentWnd, nID, pPersist,
			bStorage, reinterpret_cast<EX_BSTR>(bstrLicKey));
	}
}
