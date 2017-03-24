// ModuleString.cpp: implementation of the CModuleString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ModuleString.h"
#include "..\vis1cpp.h"
#include <atlconv.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//#import "C:\Program Files\1Cv77\BIN\zlibeng.dll" no_namespace
#import "C:\Program Files\1Cv77\Bin25\BIN\zlibeng.dll" no_namespace
//#import "C:\Program Files\1C\BIN25\Bin\zlibeng.dll" no_namespace

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static void ShowFileReadErrorMsg(const CString& strFileName);

bool CModuleString::LoadModuleFromERT(const CString& strFileName)
{
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
            STGM_READ | STGM_SHARE_DENY_NONE | STGM_TRANSACTED , //STGM_READ | STGM_SHARE_DENY_WRITE,
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
		// предложение от јль‘а - не создаем новый, а сначала пытаемс€ использовать существующий от 1—
		IzlibEnginePtr iLibEngine;
		CApp7* pApp = pMainApp;
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

		// TODO artbear - далее идет различие с 1—++
		
		HGLOBAL hGlobal    = NULL;
		GetHGlobalFromStream(pStreamOut, &hGlobal);
		if (NULL == hGlobal)
		{
			pBkEndUI->DoMessageLine("hGlobal == NULL", mmBlackErr);	                            
			return false;
		}
		
		LPVOID lpMemStream = GlobalLock(hGlobal);                  
		LPTSTR lpStr = GetBufferSetLength(cbStreamSizeOut);
		
		memcpy(&lpStr[0], lpMemStream, cbStreamSizeOut);            
		
		GlobalUnlock(hGlobal);
		GlobalFree(hGlobal);
	} 
	catch (_com_error&) 
	{
		pBkEndUI->DoMessageLine("catch (_com_error& er) ", mmBlackErr);
		return false;
	}                  

	return true;    
}

bool CModuleString::LoadModuleFromMD(const CString& strFileName)
{
	CString strTemp = strFileName; // artbear
    strTemp.MakeLower(); //const_cast<CString*>(&strFileName)->MakeUpper();
    
	int npos = strTemp.Find("@md");

	CString nameRep = strFileName.Left(npos);

	CMetaDataCont *pMD = GetMetaData();  
	if(!pMD)
		return false;
	
	CMetaDataObj* pObj = pMD->GetCalcVarDef(nameRep);
	if(!pObj)
		return false;
	
	long id = pObj->GetID();
	CTypedCont* pCont = NULL;
	CString text;

	CConfigCont::IDToPath(id,"CalcVar",text,&pCont,FALSE);
	if(pCont)
	{
		CTextDocument* pText = static_cast<CTextDocument*>( pCont->GetTextDocument() );

		pText->GetText(*this);           
		return true;
	}

	return false;
}

bool CModuleString::LoadModuleFromFile(const CString& strFileName)
{
	if (!FileExists(strFileName))
	{
		ShowFileReadErrorMsg(strFileName);
		return false;
	}
    
	if(ReadFileToStringWithoutExceptions(strFileName, *this))
		return true;
		
	ShowFileReadErrorMsg(strFileName);
	return false;
}

bool CModuleString::LoadModuleFromDLL(const CString& strFileName)
{
	typedef LPCSTR (WINAPI *PF_GetClassText)(LPCSTR lpStr);

	int npos = strFileName.Find('@');
    if (-1 == npos)
	{
		pBkEndUI->DoMessageLine("Ќеверный формат описани€ имени класса в dll.", mmBlackErr);	
		return false;
	}
	
	CString nameClass = strFileName.Left(npos);
	CString nameFile = AbsoluteFilePath(AllTrim(strFileName.Mid(npos+1)));
	HMODULE hDll = ::LoadLibrary(nameFile);
	if(!hDll)
	{
		ShowFileReadErrorMsg(nameFile);
		return false;
	}

	PF_GetClassText hGetClassText = (PF_GetClassText)::GetProcAddress(hDll, "GetClassText");
	if(!hGetClassText)
	{
		CString StrError;
		StrError = "Ќе найдена функци€ GetClassText() в библиотеке: ";
		StrError += nameFile;
		pBkEndUI->DoMessageLine(StrError, mmBlackErr);
		
		::FreeLibrary(hDll);
		return false;
	}

	this->CString::operator =((*hGetClassText)(nameClass));
	::FreeLibrary(hDll);
	
	return true;
}

int CModuleString::getOffsetLines() const
{
	return m_OffsetLines;
};

int CModuleString::DeleteWithLineCountTo(int posBegin)
{
	int Result = 0;
	
	CString TopOfTheModule = Left(posBegin);
	while (!TopOfTheModule.IsEmpty())
	{
		int pos = TopOfTheModule.Find("\n");
		if (pos != -1)
		{
			Result++;
			TopOfTheModule.Delete(0, pos+1); // +1 чтобы удалить и сам \n
		}
	}
	
	Delete(0, posBegin);

	return Result;
}

// получу исходный код класса из модул€
//
// если есть директивы //# ClassBegin »м€ ласса и //# ClassEnd »м€ ласса, 
//	выдаетс€ код именно этот класса
// иначе выдаетс€ код всего модул€
//
void CModuleString::CropModuleToClass(const CString& strClassName)
{
	int posBegin = 0;
	int posEnd = GetLength();

	int findBegin = Find("//# ClassBegin "+ strClassName+";");
	int findEnd = Find("//# ClassEnd " + strClassName+";");

	if (findBegin != -1)
	{
		//pBkEndUI->DoMessageLine("Begin of "+strClassName+" found", mmBlackErr);
		posBegin = findBegin;
	}
	if (findEnd != -1)
	{
		//pBkEndUI->DoMessageLine("End of "+strClassName+" found", mmBlackErr);
		posEnd = findEnd;
	}

	Delete(posEnd, GetLength() - posEnd);
	m_OffsetLines = DeleteWithLineCountTo(posBegin); // дл€ последующей передачи в CModuleParser

}

// получу карту классов, описанных в этом файле с помощью директив //# ClassBegin и //# ClassEnd
// количество пробелов и наличие точки с зап€той роли не играет
// главное, чтобы //# сто€ло в начале строки и было указано им€ класса
//
//#  ClassBegin  ќќѕѕредок¬ќдном‘айле—ќдинаковымићетодами
//#   ClassEnd  ќќѕѕредок¬ќдном‘айле—ќдинаковымићетодами
//
bool CModuleString::DivideTextOnClasses(CMapStringToString* pMap)
{
	bool bRes = false;

	LPCSTR szClassBegin = "ClassBegin";
	LPCSTR szClassEnd = "ClassEnd";
	if (-1 == Find(szClassBegin))
		return bRes;

	LPCSTR szBeginPrepoccessorDef = "//#";
	const int iCountBeginDef = 3;
	LPCSTR szDelim = " \t\r\n;";

	LPSTR szCurrStr = LockBuffer();
	szCurrStr = strstr(szCurrStr, szBeginPrepoccessorDef);
	if (!szCurrStr)
	{
		ReleaseBuffer();
		return NULL;
	}

	LPSTR szSaveCurrStr = szCurrStr;
	
	LPSTR szAllocString = (char*)malloc(strlen(szCurrStr) + 1);
	strcpy(szAllocString, szCurrStr);
	LPSTR szFind = NULL;

	while(szCurrStr) // "//#"
	{

		szCurrStr += iCountBeginDef;
		szFind = szAllocString + (szCurrStr - szSaveCurrStr);

		LPCSTR szToken1 = strtok(szFind, szDelim);
	
		if (!strstr(szToken1, szClassBegin))
		{
			szCurrStr = strstr(szCurrStr, szBeginPrepoccessorDef);
			szFind = szAllocString + (szSaveCurrStr - szSaveCurrStr);
			continue;
		}

		LPCSTR szTokenClassName1 = strtok(NULL, szDelim);
		if (!szTokenClassName1)
		{
			szCurrStr = strstr(szCurrStr, szBeginPrepoccessorDef);
			szFind = szAllocString + (szSaveCurrStr - szSaveCurrStr);
			continue;
		}
		// теперь szTokenClassName1 = им€ класса

		LPSTR szEndOfLine1 = strstr(szCurrStr, "\r\n");
		// TODO проверить в конце файла
		if (!szEndOfLine1)
		{
			szCurrStr = strstr(szCurrStr, szBeginPrepoccessorDef);
			szFind = szAllocString + (szSaveCurrStr - szSaveCurrStr);
			continue;
		}

		LPSTR szBeginOfClass = szEndOfLine1+1; // первый символ класса

		LPSTR szCurrStrEnd = strstr(szBeginOfClass, szBeginPrepoccessorDef);
		LPSTR szSaveCurrStrEnd = szCurrStrEnd;

		LPSTR szAllocStringEnd = (char*)malloc(strlen(szCurrStrEnd) + 1);
		strcpy(szAllocStringEnd, szCurrStrEnd);
		LPSTR szFindEnd = NULL;

		LPSTR szSaveEndOfClass = NULL;
		
		while(szCurrStrEnd)
		{
			szSaveEndOfClass = szCurrStrEnd;

			szCurrStrEnd += iCountBeginDef;
			szFindEnd = szAllocStringEnd + (szCurrStrEnd - szSaveCurrStrEnd);

			LPCSTR szToken1 = strtok(szFindEnd, szDelim);
			if (!strstr(szToken1, szClassEnd))
			{
				szCurrStrEnd = strstr(szCurrStrEnd, szBeginPrepoccessorDef);
				szFindEnd = szAllocStringEnd + (szCurrStrEnd - szSaveCurrStrEnd);
				continue;
			}

			LPCSTR szTokenClassName2 = strtok(NULL, szDelim);
			if (!szTokenClassName2)
			{
				szCurrStrEnd = strstr(szCurrStrEnd, szBeginPrepoccessorDef);
				szFindEnd = szAllocStringEnd + (szCurrStrEnd - szSaveCurrStrEnd);
				continue;
			}
			// теперь szTokenClassName2 - им€ класса
			if(!strstr(szTokenClassName1, szTokenClassName2)) // вдруг неверные классы
			{
				RuntimeError("Ќе найдена завершаюша€ скобка дл€ класса %s ", szTokenClassName1);
				//szCurrStrEnd = strstr(szCurrStrEnd, szBeginPrepoccessorDef);
				//szFindEnd = szAllocStringEnd + (szCurrStrEnd - szSaveCurrStrEnd);
				//continue;
			}
			
			LPSTR szEndOfLine2 = strstr(szCurrStrEnd, "\r\n");
			if (!szEndOfLine2)
			{
				// TODO проверить в конце файла
				szCurrStrEnd = strstr(szCurrStrEnd, szBeginPrepoccessorDef);
				szFindEnd = szAllocStringEnd + (szCurrStrEnd - szSaveCurrStrEnd);
				continue;
			}

			szCurrStr = szEndOfLine2+1;
			break;
		}
		
		free(szAllocStringEnd);

		// получу строку класса
		LPSTR szEndOfClass = szSaveEndOfClass-1;

		int iCount = szEndOfClass - szBeginOfClass + 1;

		CString strClassSource;
		LPSTR szClassSource = strClassSource.GetBufferSetLength(iCount);
		strncpy(szClassSource, szBeginOfClass, iCount);
		strClassSource.ReleaseBuffer(-1);

//  LogErr("ќчередной класс %s-------------------\n<<<<<%s>>>>>\n-----------------------", 
//  	   szTokenClassName1, szClassSource);

		CString s;
		if (pMap->Lookup(szTokenClassName1, s))
			RuntimeError(" ласс %s уже описан в этом файле ранее", szTokenClassName1);

		pMap->SetAt(szTokenClassName1, szClassSource);

		szCurrStr = strstr(szCurrStr, szBeginPrepoccessorDef);
		
		bRes = true;
	}
	
	free(szAllocString);

	ReleaseBuffer();

	return bRes;

}

bool CModuleString::LoadModule(const CString& strFileName)
{
	CString strTemp = strFileName;
	strTemp.MakeLower();
	
	if (strTemp.Find(".ert") != -1)
		return LoadModuleFromERT(strFileName);

	if ( strTemp.Find("@md") != -1)
		return LoadModuleFromMD(strFileName);

 	if (strTemp.Find(".dll") != -1)
 		return LoadModuleFromDLL(strFileName);

	return LoadModuleFromFile(strFileName);
}

bool CModuleString::LoadModule(const CString& strFileName, const CString& strClassName)
{
	if (LoadModule(strFileName))
	{
		if (!strClassName.IsEmpty())
			CropModuleToClass(strClassName);
		
		return true;
	}

	return false;
}

void ShowFileReadErrorMsg(const CString& strFileName)
{
	CString StrError;
	StrError = "Ќе удалось прочитать файл: ";
	StrError += strFileName;
	pBkEndUI->DoMessageLine(StrError, mmBlackErr);	
}