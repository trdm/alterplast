// CodeDecodeFile.h: interface for the CCodeDecodeFile class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7EA5B36F9333__INCLUDED_)
#define AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7EA5B36F9333__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include <vector>
#include <wincrypt.h>

class CCodeDecodeFile  
{
	typedef std::vector<void* > VEC_POINTS;
	typedef VEC_POINTS::const_iterator CONST_ITER;
  typedef VEC_POINTS::iterator ITER;

  // RC4 struct for crypting file
  typedef struct{
    BYTE table[256];
    BYTE index[2];
  } RC4_key_t;

  void RC4_setup_key(const BYTE *key_text, size_t len, RC4_key_t * key);

public:
	struct Exeption { 
		Exeption(int code, const char* pConstChar) : iCode(code)
		{ 
			strcpy(cDescr = new char[strlen(pConstChar)+1], pConstChar);
		}
		int iCode;
		char *cDescr;
	};

  void GetString(CString &);
	void Write();
	void Read();
	void Encipher(const char* pass);
	void EncryptingMemory(void *pVoidKey, int nSize);
  void DecryptingMemory(void *pVoidKey, int nSize);


	CCodeDecodeFile(const char* );
	virtual ~CCodeDecodeFile();

private:
	CCodeDecodeFile(const CCodeDecodeFile&);
	CCodeDecodeFile& operator=(const CCodeDecodeFile&);
	HANDLE hFile;
	CString strNameOfFile;
	int countReadBytes;
	VEC_POINTS *pVecPoints;
	int massKeyWord[10];

  HCRYPTPROV hCryptProv;
	bool bState;
	static int SIZE_BLOCK;
	static int ENCRYPT_ALGORITHM;  
};

#endif // !defined(AFX_CODEDECODEFILE_H__2E9A56C7_EB99_47AA_BF90_7EA5B36F9333__INCLUDED_)
