// vs6 specialized function remapper
// whar_t / unsigned short remapper
// copyright (c) kms, 2008-2020

#if !defined FORWARDER_N_INCLUDED
#define FORWARDER_N_INCLUDED

typedef unsigned short EX_WCHAR;
typedef EX_WCHAR* EX_LPWSTR;
typedef EX_WCHAR const* EX_LPCWSTR;
typedef EX_LPWSTR EX_BSTR;

#if defined FORWARDER_DEFINE_TYPES

typedef wchar_t WCHAR;
typedef WCHAR* LPWSTR;
typedef WCHAR const* LPCWSTR;
typedef LPWSTR BSTR;

class _com_error;
class CWnd;
class CFile;
class CString;

#endif

//////////////////////////////////////////////////////////////////////////
// address converter
//////////////////////////////////////////////////////////////////////////

/*
// PF models function type
template <typename PF>
class CFunPtr
{
public:
	// для ICL будет работать максимум для одиночного наследования
	union ADDR
	{
		FARPROC fp;
		PF pf;
	};
	CFunPtr() { m_addr.fp = NULL; }
	CFunPtr(LPCSTR szModule, LPCSTR szName) { Set(szModule, szName); }

	PF Get() { return m_addr.pf; }
	PF Set(LPCSTR szModule, LPCSTR szName)
	{
		HMODULE hm = GetModuleHandle(szModule);
		m_addr.fp = !hm ? NULL : GetProcAddress(hm, szName);
		return m_addr.pf;
	}

private:
	ADDR m_addr;
};
*/

namespace N
{
	namespace _com_util
	{
		char* __stdcall ConvertBSTRToString(BSTR pSrc) throw(_com_error);
		BSTR __stdcall ConvertStringToBSTR(const char* pSrc) throw(_com_error);
	}

	class CString
	{
	public:
		CString(LPCWSTR lpsz);
		BSTR AllocSysString() const;
		const ::CString& operator=(LPCWSTR lpsz);
	};

	class CWnd
	{
	public:
		BOOL CreateControl(LPCTSTR pszClass, LPCTSTR pszWindowName, DWORD dwStyle,
			const RECT& rect, ::CWnd* pParentWnd, UINT nID, ::CFile* pPersist,
			BOOL bStorage, BSTR bstrLicKey);
	};
}

#endif
