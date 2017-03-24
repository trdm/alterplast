// ModuleString.cpp: implementation of the CModuleString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "ModuleString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#include "zlibeng.tlh"

static void ShowFileReadErrorMsg(const CString& strFileName);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//CBLModule7

bool CModuleString::LoadModuleFromERT(const CString& strFileName)
{
	Reset();

	if (!FileExists(strFileName))
	{
		ShowFileReadErrorMsg(strFileName);
		return false;
	}

	IStoragePtr pStgRoot;
	IStorage* pStorage = NULL;
	HRESULT hr;
	USES_CONVERSION;
	hr = ::StgOpenStorage(
		T2COLE(strFileName),
		NULL,		
		STGM_READ | STGM_SHARE_DENY_NONE | STGM_TRANSACTED , //STGM_READ | STGM_SHARE_DENY_WRITE ,
		NULL,
		0,
		&pStorage);
	
	if (FAILED(hr))
	{
		ShowFileReadErrorMsg(strFileName);
		return false;
	}              
	pStgRoot.Attach(pStorage, false);
	
	IStreamPtr pStream;
	IStream *pIStr = NULL;
	hr = pStgRoot->OpenStream(T2COLE("MD Programm text"),0,STGM_READ | STGM_SHARE_EXCLUSIVE,0,&pIStr);
	if( FAILED(hr) )
	{
		pBkEndUI->DoMessageLine("failed IStorage::OpenStream", mmBlackErr);
		return false;
	}
	pStream.Attach(pIStr, false);
	
	IStreamPtr pStreamOut;
	pIStr = NULL;
	if (S_OK != CreateStreamOnHGlobal(NULL, TRUE, &pIStr))
	{
		pBkEndUI->DoMessageLine("error in CreateStreamOnHGlobal", mmBlackErr);    
		return false; 
	}

	pStreamOut.Attach(pIStr, false);
	try
	{
		// предложение от АльФа - не создаем новый, а сначала пытаемся использовать существующий от 1С
		IzlibEnginePtr iLibEngine;
		CApp7* pApp = (CApp7*)AfxGetApp();
		if (pApp)
			iLibEngine = pApp->get_zlibEngine();
 		else
 			iLibEngine.CreateInstance(L"V75.zlibEngine");
		
		hr = iLibEngine->pkDecompress(pStream, pStreamOut);            
		
		if (FAILED(hr))
		{
			pBkEndUI->DoMessageLine("error iLibEngine->pkDecompress", mmBlackErr);          
			return false;
		}          
		
		STATSTG St;
		pStreamOut->Stat(&St, STATFLAG_NONAME);
		ULONG cbStreamSizeOut = St.cbSize.LowPart;                               
		LPTSTR lpStr = GetBufferSetLength(cbStreamSizeOut);
		if (lpStr == NULL)
		{
			pBkEndUI->DoMessageLine("lpStr == NULL", mmBlackErr);	                            
			return false;
		}
		
		LARGE_INTEGER __zero;
		__zero.QuadPart = __int64(0);
		if (pStreamOut->Seek(__zero, 0, NULL) != S_OK)
		{
			pBkEndUI->DoMessageLine("pStreamOut->Seek(__zero, 0, NULL) != S_OK", mmBlackErr);	                            
			return false;
		}
		
		ULONG pcbRead = 0;
		
		// main job here
		if (pStreamOut->Read(&lpStr[0], cbStreamSizeOut, &pcbRead) != S_OK)
			
		{
			pBkEndUI->DoMessageLine("pStreamOut->Read(&lpStr[0], cbStreamSizeOut, &pcbRead) != S_OK", mmBlackErr);	                            
			return false;
		}              
		
	} 
	catch (_com_error& err) 
	{
		CString str;
		str.Format("V75.zlibEngine Error:%s", err.ErrorMessage());
		pBkEndUI->DoMessageLine(str, mmBlackErr);
		return false;
	}                  
	
	m_csModulePath = strFileName;
	m_bModuleContValid = TRUE;
	return true;    
}

bool CModuleString::LoadModuleFromMD(const CString& strFileName)
{
	Reset();

	int npos = strFileName.Find("@MD");
    if (-1 == npos)
		npos = strFileName.Find("@md");
	
	CString nameRep = strFileName.Left(npos);
	CMetaDataCont *pMD = GetMetaData();
	
	CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
	if (pObj)
	{
		LPCSTR szContName = "CalcVar";

		long id = pObj->GetID();
		CTypedCont* pCont = NULL;
		CString text;
		CConfigCont::IDToPath(id, szContName, text, &pCont, FALSE);
		
		if(pCont)
		{
			CTxtModuleReader* pText = (CTxtModuleReader*)pCont->GetTextDocument();
			pText->GetText(*this);

			m_nModuleID = id;
			m_csModulePath = szContName;
			m_bModuleContValid = TRUE;
			return true;
		}
		else
			return false;
	}
	else
	{
		return false;
	}
}

bool CModuleString::LoadModuleFromFile(const CString& strFileName)
{
	Reset();

	if (!FileExists(strFileName))
	{
		ShowFileReadErrorMsg(strFileName);
		return false;
	}

	if(ReadFileToStringWithoutExceptions(strFileName, *this))
	{
		m_csModulePath = strFileName;
		return true;
	}
	else
	{
		ShowFileReadErrorMsg(strFileName);
		return false;
	}
}

typedef LPCSTR (WINAPI *PF_GetClassText)(LPCSTR lpStr);
bool CModuleString::LoadModuleFromDLL(const CString& strFileName)
{
	Reset();

	int npos = strFileName.Find("@");
    if (-1 == npos)
	{
		pBkEndUI->DoMessageLine("Неверный формат описания имени класса в dll.", mmBlackErr);	
		return false; // неверный формат имени класса в dll
	}
	
	CString nameClass = strFileName.Left(npos);
	CString nameFile = AbsoluteFilePath(AllTrim(strFileName.Mid(npos+1)));
	HMODULE hDll = ::LoadLibrary(LPCSTR(nameFile));
	if(hDll == NULL)
	{
		CString StrError;
		StrError = "Ошибка при загрузке файла: ";
		StrError += nameFile;
		pBkEndUI->DoMessageLine(StrError, mmBlackErr);	
		return false;
	}
	PF_GetClassText hGetClassText = (PF_GetClassText)::GetProcAddress(hDll, "GetClassText");
	if(hGetClassText == NULL)
	{
		CString StrError;
		StrError = "Не найдена функция GetClassText() в библиотеке: ";
		StrError += nameFile;
		pBkEndUI->DoMessageLine(StrError, mmBlackErr);
		::FreeLibrary(hDll);
		return false;
	}
	CString::operator =((*hGetClassText)(nameClass));
	::FreeLibrary(hDll);

	m_csModulePath = strFileName;
	return true;
}

bool CModuleString::LoadModule(const CString& strFileName)
{
	bool bSuccess = FALSE;

	if (strFileName.Find(".ert") != -1)
	{
		bSuccess = LoadModuleFromERT(strFileName);
	}
	else
	{
		if ((strFileName.Find("@MD") != -1) || (strFileName.Find("@md") != -1))
		{
			bSuccess = LoadModuleFromMD(strFileName);
		}
		else
		{
			if (strFileName.Find(".dll") != -1)
			{
				bSuccess = LoadModuleFromDLL(strFileName);
			}
			else
			{
				bSuccess = LoadModuleFromFile(strFileName);
			}
		}
	}

	if (bSuccess)
		m_csFileName = strFileName;

	return bSuccess;
}

void CModuleString::ExtractClass(const CString& strClassName)
{
	int posBegin = 0;
	int posEnd = GetLength();
	
	int findBegin = Find("//# ClassBegin "+ strClassName+";");
	int findEnd = Find("//# ClassEnd " + strClassName+";");
	
	if (findBegin != -1)
	{
		posBegin = findBegin;
		m_bModuleContValid = FALSE;
	}

	if (findEnd != -1)
	{
		posEnd = findEnd;
		m_bModuleContValid = FALSE;
	}
	
	Delete(posEnd, GetLength() - posEnd);
	Delete(0, posBegin);
}

bool CModuleString::LoadModule(const CString& strFileName, const CString& strClassName)
{
	if (LoadModule(strFileName))
	{
		ExtractClass(strClassName);
		return true;
	}
	
	return false;
}

void ShowFileReadErrorMsg(const CString& strFileName)
{
	CString StrError;
	StrError = "Не удалось прочитать файл: ";
	StrError += strFileName;
	pBkEndUI->DoMessageLine(StrError, mmBlackErr);	
}
