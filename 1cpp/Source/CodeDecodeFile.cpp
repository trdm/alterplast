// CodeDecodeFile.cpp: implementation of the CCodeDecodeFile class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "CodeDecodeFile.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CBkEndUI * pBkEndUI;

int CCodeDecodeFile::SIZE_BLOCK = 0x1000; // 4 kb
int CCodeDecodeFile::ENCRYPT_ALGORITHM = CALG_RC2;

CCodeDecodeFile::CCodeDecodeFile(const char* nameFile) : countReadBytes(0), strNameOfFile(nameFile)
	{
		hFile = CreateFile(strNameOfFile,		   // name of open file
								GENERIC_READ,              // open for reading 
								FILE_SHARE_READ,           // share for reading 
								NULL,                      // no security 
								OPEN_EXISTING,             // existing file only 
								FILE_ATTRIBUTE_NORMAL,     // normal file 
								NULL);                     // no attr. template 
	
 
		if (hFile == INVALID_HANDLE_VALUE) 
		{ 
      CloseHandle(hFile);
			throw Exeption(GetLastError(), "Error open file");
		} 
    /*if(CryptAcquireContext(
			  &hCryptProv, 
			  NULL, 
			  NULL, 
			  PROV_RSA_FULL, 
			  0))
		{
		   
		}
		else
		{
      CloseHandle(hFile);
		  Exeption(GetLastError(), "Error during CryptAcquireContext!");
		}
    */
		pVecPoints = new VEC_POINTS;
		massKeyWord[0] = 687848672;
	  massKeyWord[1] = 346930486;
	  massKeyWord[2] = 345838659;
	  massKeyWord[3] = -234892418;
}

CCodeDecodeFile::~CCodeDecodeFile()
{
	CloseHandle(hFile);
	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
		void* pVoid = *iter;
		VirtualFree(pVoid, SIZE_BLOCK, MEM_DECOMMIT);
	}
	/*int size = pVecPoints->size();
	for (int i = 0; i < size; i++)
		pVecPoints->pop_back();*/
	delete (pVecPoints);
  /*if(hCryptProv) 
			CryptReleaseContext(hCryptProv, 0);*/
}

void CCodeDecodeFile::RC4_setup_key(const BYTE *key_text, size_t len, RC4_key_t * key)
{
  for (int i = 0; i < 256; i++)
    key->table[i] = i;

  key->index[0] = 0;
  key->index[1] = 0;

  BYTE e = 0;
  for (i = 0; i < 256; i++)
  {
    e += key_text[i%len] + key->table[i];
  
    BYTE temp     = key->table[i];
    key->table[i] = key->table[e];
    key->table[e] = temp;
  }
}

void CCodeDecodeFile::Encipher(const char* pass)
{
  int len = strlen(pass);

  RC4_key_t key;
  RC4_setup_key((BYTE*)pass, len, &key);

  /*for (int i = 0; i < len; i++) {
    massKeyWord[0] += static_cast<int>(pass[i]);
    massKeyWord[1] ^= static_cast<int>(pass[i]);
    massKeyWord[2] %= static_cast<int>(pass[i]);
    massKeyWord[3] -= static_cast<int>(pass[i]);
  }
  
	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{

		void* pVoid = *iter;
		int key = 0;
		int *pType = static_cast<int *> (pVoid);
		for (int i = 0; i < SIZE_BLOCK; i+=sizeof(int), pType++){
			int temp = *pType;
			temp ^= massKeyWord[key];
			*pType = temp;
			key++;
			key = (key == 3)? 0 : key;
			
		}
	}*/
  for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{

		void* pVoid = *iter;
		BYTE *pType = static_cast<BYTE *> (pVoid);
		for (int i = 0; i < SIZE_BLOCK; i+=sizeof(BYTE), pType++){
      key.index[1] += key.table[++key.index[0]];

      BYTE temp               = key.table[key.index[0]];
      key.table[key.index[0]] = key.table[key.index[1]];
      key.table[key.index[1]] = temp;

			temp = *pType;
      temp ^= key.table[
        key.table[key.index[0]] +
        key.table[key.index[1]]
        ];
			*pType = temp;
		}
	}
}

void CCodeDecodeFile::Read()
{
	for (;;) {
		DWORD dwBytesOfRead;
		void *pBuf = VirtualAlloc(NULL, SIZE_BLOCK, MEM_COMMIT,PAGE_READWRITE);
		if (pBuf == NULL)
			throw Exeption(GetLastError(), "Error allocate memory"); // упс! памяти нет!

		ReadFile(hFile, pBuf, SIZE_BLOCK, &dwBytesOfRead, NULL);
    if (dwBytesOfRead == 0) {
		  VirtualFree(pBuf, SIZE_BLOCK, MEM_DECOMMIT);
		  break;
    }
		countReadBytes += dwBytesOfRead;
		pVecPoints->push_back(pBuf);
	}
}

void CCodeDecodeFile::Write()
{
  CloseHandle(hFile);
	hFile = CreateFile(strNameOfFile,		   // name of open file
							GENERIC_WRITE,              // open for reading 
							FILE_SHARE_READ,           // share for reading 
							NULL,                      // no security 
							CREATE_ALWAYS,             // existing file only 
							FILE_ATTRIBUTE_NORMAL,     // normal file 
							NULL);                     // no attr. template 

	int countBytes = countReadBytes;
	CONST_ITER preEndIter = pVecPoints->end()-1;

	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
		if (countBytes == 0) break;
		
		ULONG numberOfBytesWritten = 0;
		int nNumberOfBytesToWrite = 0;
		
		if (bState == true && iter == preEndIter) {
			// значит блок памяти в последенем векторе больше, чем константа
			nNumberOfBytesToWrite = countBytes;	
		}
		else {
			if (countBytes > SIZE_BLOCK) {
				nNumberOfBytesToWrite = SIZE_BLOCK;
			}
			else
				nNumberOfBytesToWrite = countBytes;
		}
		
			
		void* pVoid = *iter;
		WriteFile(hFile, pVoid, nNumberOfBytesToWrite, &numberOfBytesWritten, NULL);
		countBytes -= numberOfBytesWritten;
		
	}
	/*CloseHandle(hFile);
	hFile = CreateFile(strNameOfFile,		   // name of open file
							GENERIC_WRITE,              // open for reading 
							FILE_SHARE_READ,           // share for reading 
							NULL,                      // no security 
							CREATE_ALWAYS,             // existing file only 
							FILE_ATTRIBUTE_NORMAL,     // normal file 
							NULL);                     // no attr. template 

	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
		int countBytes = countReadBytes;
		ULONG numberOfBytesWritten = 0;
		int nNumberOfBytesToWrite = 0;
		if (countBytes > SIZE_BLOCK) {
			nNumberOfBytesToWrite = SIZE_BLOCK;
			countBytes -= SIZE_BLOCK;
		}
		else
			nNumberOfBytesToWrite = countBytes;
			
		void* pVoid = *iter;
		WriteFile(hFile, pVoid, nNumberOfBytesToWrite, &numberOfBytesWritten, NULL);
	}*/ // Old algorithm 
}

void CCodeDecodeFile::GetString(CString &Str)
{
	int Size = SIZE_BLOCK;
  int countBytes = countReadBytes;

	LPTSTR lpStr = Str.GetBuffer(countBytes+1);
  
	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
    
		int nNumberOfBytesToWrite = 0;

		if (countBytes > SIZE_BLOCK) {
			nNumberOfBytesToWrite = SIZE_BLOCK;
			countBytes -= SIZE_BLOCK;
		}
		else
			nNumberOfBytesToWrite = countBytes;

		void* pVoid = *iter;
		char *pType = static_cast<char *> (pVoid);
		for (int i = 0; i < nNumberOfBytesToWrite; i+=sizeof(char))
    {
				*lpStr = *pType;
				lpStr++;
        pType++;
		}
	}
  *lpStr = '\0';
	Str.ReleaseBuffer();
}

void CCodeDecodeFile::EncryptingMemory(void *pVoidKey, int nSize)
{
	bState = false;
	HCRYPTKEY hKey;
	HCRYPTHASH hHash;
	DWORD dwCount; 
	
	if(CryptCreateHash(
		 hCryptProv, 
		 CALG_MD5, 
		 0, 
		 0, 
		 &hHash) == FALSE)
	{ 
		throw Exeption(GetLastError(), "Error during CryptCreateHash!");
	}  
	//--------------------------------------------------------------------
	// Hash the password. 

	if(CryptHashData(
			 hHash, 
			 (BYTE *)pVoidKey, 
			 nSize, 
			 0) == FALSE)
	 {
		throw Exeption(GetLastError(), "Error during CryptHashData!");
	 }
	//--------------------------------------------------------------------
	// Derive a session key from the hash object. 

	if(CryptDeriveKey(
			 hCryptProv, 
			 ENCRYPT_ALGORITHM, 
			 hHash, 
			 0, 
			 &hKey) == FALSE)
	 {
		throw Exeption(GetLastError(), "Error during CryptDeriveKey!");
	 }
	// Get handle to the default provider. 
	CryptDestroyHash(hHash); 
	hHash = 0; 

	int countBytes = countReadBytes;
	// здесь проверка кратна ли прочитанная память размеру блока, если да, то
	// добавим еще одну страницу памяти для 
	int scale = 1;
	int nInteger = countBytes / SIZE_BLOCK;
	int nDiff = countBytes - nInteger*SIZE_BLOCK;

	if ((countBytes % SIZE_BLOCK == 0)||(nDiff+20 > SIZE_BLOCK)) {
		bState = true;
		scale = 2;
		ITER iter = pVecPoints->end()-1;
		void *pVoid = *iter;
		BYTE* pByteSource = (BYTE*)pVoid;
		

		void *pBuf = VirtualAlloc(NULL, SIZE_BLOCK * 2, MEM_COMMIT,PAGE_READWRITE);
		BYTE* pByteDest = (BYTE*)pBuf;

		for (int i = 0; i < SIZE_BLOCK; i++) {
			*(pByteDest++) = *(pByteSource++);
		}

		VirtualFree(pVoid, SIZE_BLOCK, MEM_DECOMMIT);
		*iter = pBuf;
	}
	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
		void* pByte = *iter;
		dwCount = 0;
		BOOL bFinal = (++iter == pVecPoints->end());
		
		
		int nNumberOfBytesToWrite = 0;
		if (countBytes > SIZE_BLOCK) {
			nNumberOfBytesToWrite = SIZE_BLOCK;
			countBytes -= SIZE_BLOCK;
		}
		else
			nNumberOfBytesToWrite = countBytes;
		


		dwCount = nNumberOfBytesToWrite;
		if(CryptEncrypt(
			hKey, 
			0, 
			bFinal, 
			0, 
			(BYTE*)pByte, 
			&dwCount, 
			SIZE_BLOCK * scale) == FALSE)
		{ 
			throw Exeption(GetLastError(), "Error during CryptEncrypt!");
		} 

		--iter;
		if (nNumberOfBytesToWrite < dwCount) {
			countReadBytes += (dwCount - nNumberOfBytesToWrite);
		}

	}
	if(hKey) 
		CryptDestroyKey(hKey); 
}
void CCodeDecodeFile::DecryptingMemory(void *pVoidKey, int nSize)
{
	HCRYPTKEY hKey;
	HCRYPTHASH hHash;
	DWORD dwCount; 
	bState = false;

	if(CryptCreateHash(
		 hCryptProv, 
		 CALG_MD5, 
		 0, 
		 0, 
		 &hHash) == FALSE)
	{ 
		throw Exeption(GetLastError(), "Error during CryptCreateHash!");
	}  
	//--------------------------------------------------------------------
	// Hash the password. 

	if(CryptHashData(
			 hHash, 
			 (BYTE *)pVoidKey, 
			 nSize, 
			 0) == FALSE)
	 {
		throw Exeption(GetLastError(), "Error during CryptHashData!");
	 }
	//--------------------------------------------------------------------
	// Derive a session key from the hash object. 

	if(CryptDeriveKey(
			 hCryptProv, 
			 ENCRYPT_ALGORITHM, 
			 hHash, 
			 0, 
			 &hKey) == FALSE)
	 {
		throw Exeption(GetLastError(), "Error during CryptDeriveKey!");
	 }
	// Get handle to the default provider. 
	CryptDestroyHash(hHash); 
	hHash = 0; 
	int countBytes = countReadBytes;
	for (CONST_ITER iter = pVecPoints->begin(); iter != pVecPoints->end(); iter++)
	{
		int nNumberOfBytesToWrite = 0;
		if (countBytes > SIZE_BLOCK) {
			nNumberOfBytesToWrite = SIZE_BLOCK;
			countBytes -= SIZE_BLOCK;
		}
		else
			nNumberOfBytesToWrite = countBytes;

		dwCount = nNumberOfBytesToWrite;
		void* pByte = *iter;

		if(CryptDecrypt(
			hKey, 
			0, 
			(++iter == pVecPoints->end()), 
			0, 
			(BYTE*)pByte, 
			&dwCount) == FALSE)
		{ 
			throw Exeption(GetLastError(), "Error during CryptDecrypt!");
		} 
		--iter;
		if (nNumberOfBytesToWrite > dwCount)
			countReadBytes -= (nNumberOfBytesToWrite - dwCount);
	}
	if(hKey) 
		CryptDestroyKey(hKey);

}
