// ModuleString.h: interface for the CModuleString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
#define AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CModuleString : public CString  
{
private:
	int m_nModuleID;			// в терминах 1—
	CString m_csModulePath;		// в терминах 1C
	BOOL m_bModuleContValid;	// в терминах 1— (отладка возможна)
	CString m_csFileName;		// оригинальный путь, дл€ справки (1cpp)
public:
	CModuleString() { Reset(); }

	void Reset() { m_nModuleID = -1; m_csModulePath.Empty(); m_bModuleContValid = FALSE; m_csFileName.Empty(); }
	int GetModuleID() const { return m_nModuleID; }
	CString const& GetModulePath() const { return m_csModulePath; }
	BOOL GetModuleContValid() const { return m_bModuleContValid; }
	CString const& GetFileName() const { return m_csFileName; }

/*	
	CModuleString(const CString& stringSrc) : CString(stringSrc) {}
	// from a single character
	CModuleString(TCHAR ch, int nRepeat = 1):CString(ch, nRepeat) {}
	// from an ANSI string (converts to TCHAR)
	CModuleString(LPCSTR lpsz):CString(lpsz) {}
	// from a UNICODE string (converts to TCHAR)
	CModuleString(LPCWSTR lpsz):CString(lpsz) {}
	// subset of characters from an ANSI string (converts to TCHAR)
	CModuleString(LPCSTR lpch, int nLength):CString(lpch, nLength) {}
	// subset of characters from a UNICODE string (converts to TCHAR)
	CModuleString(LPCWSTR lpch, int nLength):CString(lpch, nLength) {}
	// from unsigned characters
	CModuleString(const unsigned char* psz):CString(psz) {}
	~CModuleString() {}
*/	
private:
	void ExtractClass(const CString& strClassName);
	
	bool LoadModuleFromERT(const CString& strFileName);
	bool LoadModuleFromMD(const CString& strFileName);
	bool LoadModuleFromFile(const CString& strFileName);
	bool LoadModuleFromDLL(const CString& strFileName);

public:
	bool LoadModule(const CString& strFileName);
	bool LoadModule(const CString& strFileName, const CString& strClassName);
};

#endif // !defined(AFX_MODULESTRING_H__44088562_A489_4581_B7AC_D31694735690__INCLUDED_)
