// глобальные методы в целом для компоненты
//#define USE_LOG_FILE 1

#include "stdafx.h"
#include "GlobalMethods.h"

//#include "..\TurboBL\gcboost.h"

#ifdef USE_LOG_FILE
	#include "LogFile.h"
#endif 

extern CBkEndUI * pBkEndUI;

void WriteToLog(const CString& txt, const CString& Title)
{
#ifdef USE_LOG_FILE
	if(!g_Log.IsOpen()){
		g_Log.Open("C:\\vis1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log << Title << " "<< txt << CRLF;
#endif 
}

//Функции для выдачи сообщений в окно сообщений 1С
void ShowMsg(const char* format,MessageMarker marker,...)
{
	va_list arg;
	va_start(arg,marker);
	CString txt;
	txt.FormatV(format,arg);
	va_end(arg);
	if(!pBkEndUI)
		AfxMessageBox(txt);
	else
		pBkEndUI->DoMessageLine(txt,marker);

	WriteToLog(txt, "ShowMsg");
}

void Msg(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	pBkEndUI->DoMessageLine((LPCSTR)str, mmBlueTriangle);

	WriteToLog(str, "Msg");
}

void LogErr(const char* format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	pBkEndUI->DoMessageLine(txt,mmBlackErr);
	va_end(arg);

	WriteToLog(txt, "LogErr");
}

// функция выдает окно с информацией
void MessageBox(const char* format, ...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	va_end(arg);

	AfxMessageBox(txt);

	WriteToLog(txt, "MessageBox");
}

//Функция сообщает об ошибке, и информирует 1С что произошло исключение
void RuntimeError(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	WriteToLog(str, "RuntimeError");

	CBLModule::RaiseExtRuntimeError((LPCSTR)str, mmRedErr);
}

void RuntimeErrorRes(UINT nID)
{
	CString err;
	err.LoadString(nID);

	WriteToLog(err, "RuntimeErrorRes");

	CBLModule::RaiseExtRuntimeError(err, mmRedErr);
}

void GetRTCFull(CRuntimeClass* rtc, CString& Str)
{
    if (rtc)
    {
        Str += "<-";
        Str += rtc->m_lpszClassName;
        GetRTCFull(rtc->m_pfnGetBaseClass(), Str);
    }
}

void Trace0(const char* str)
{
    GetBkEndUI()->DoMessageLine(str, mmNone);
}

void MessageFullRTC(CObject* obj)
{
    CString Str;
    GetRTCFull(obj->GetRuntimeClass(), Str);
    Trace0(Str);
}

/*
CVTExtended* CValue2VTExt(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CVTExtended") == 0 )
	{
		return (CVTExtended*)pVTContext;
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("ИндексированнаяТаблица", vBaseClass) )
		{
			return (CVTExtended*)(vBaseClass.GetContext());
		}
	}

	return NULL;
}

CValueTable* CValue2VT(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CValueTableContext") == 0 )
	{
		return ((CValueTableContextData*)pVTContext->GetInternalData())->GetValueTable();
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("ТаблицаЗначений", vBaseClass) )
		{
			pVTContext = vBaseClass.GetContext();
			return ((CValueTableContextData*)pVTContext->GetInternalData())->GetValueTable();
		}
	}

	return NULL;
}

CPtrArray* CValue2VL(CValue const& Value)
{
	if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

    CBLContext* pVTContext = Value.GetContext();
	if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CValueListContext") == 0 )
	{
		return (CPtrArray*)(((CValueListContext*)(pVTContext))->m_pValueItemList);
	}
	else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	{
		CValue vBaseClass;
		if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("СписокЗначений", vBaseClass) )
		{
			pVTContext = vBaseClass.GetContext();
			return (CPtrArray*)(((CValueListContext*)(pVTContext))->m_pValueItemList);
		}
	}

	return NULL;
}*/

CString strLastError(DWORD dwMessageID /* = 0*/)
{
	void* cstr = NULL;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwMessageID==0 ? GetLastError() : dwMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &cstr,
		0,
		NULL
		);
	CString res = (char*)cstr;
	::LocalFree(cstr);
	return res;
}

// проверка на существование файла или папки
bool FileExists(CString fileName)
{
	CString sAbsoluteFilePath(::AbsoluteFilePath(fileName));
	DWORD tRes = GetFileAttributes((LPCSTR)sAbsoluteFilePath);
//	if (tRes != 0xFFFFFFFF)
//		if (!(tRes & FILE_ATTRIBUTE_DIRECTORY))
//			bFileExists = true;
	return tRes != 0xFFFFFFFF && !(tRes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FolderExists(CString folderName)
{
	DWORD tRes = GetFileAttributes((LPCSTR)folderName);
	return tRes != 0xFFFFFFFF && tRes & FILE_ATTRIBUTE_DIRECTORY;
}

bool DirectoryExists(CString directoryName)
{
	return FolderExists(directoryName);
}

// получить имя папки из полного пути к файлу
// существование файла не проверяется
// если не удалось получить имя папки, возвращается ""
//
// ВАЖНО: папка всегда возвращается со слешем в конце!!
//
//	из "T:\Папка Файла\Имя Файла.txt" получу T:\Папка Файла\
//
CString ExtractFileDir(CString FileName)
{
	int i = 0;
	const char PathDelim = '\\';
	const char DriveDelim = ':';
	LPCSTR pName = (LPCSTR)FileName;
	LPCSTR s = strrchr(pName, PathDelim);
	if (!s) return "";

	int count = (s-pName)+1+1; // 1 байт под '\0'
	char* _ret = new char[count];
	lstrcpyn(_ret, pName, count);
	CString ret = _ret;
	delete [] _ret;
	return ret;
}

// получить абсолютный путь к файлу\папке
// учитывается наличие "." или ".."
//
CString AbsoluteFilePath(const CString sRelativePath)
{
	LPSTR fullpath = new char[_MAX_PATH];
	if (_fullpath( fullpath, (LPCTSTR)sRelativePath, _MAX_PATH ) == NULL )
		return ""; // TODO или выбросить исключение
	CString ret(fullpath);

	delete [] fullpath;
	return ret;
}
/*
// получить версию произвольного файла
CString GetVersionField(LPCTSTR filePath, LPCTSTR szField)
{
	if (!FileExists(filePath))
		AfxThrowFileException(CFileException::fileNotFound);

    CString sRet;
	LPTSTR BufFileName = const_cast<LPTSTR>(filePath);

    DWORD z,size;
    size = GetFileVersionInfoSize(BufFileName, &z);
	//if (!size) throw CException();
    if(size)
    {
        char* pVersion = new char[size];
        GetFileVersionInfo(BufFileName, 0, size, pVersion);
        DWORD* translate;
        UINT len;
        VerQueryValue(pVersion, "\\VarFileInfo\\Translation", (void**)&translate, &len);
        CString blockName;
        blockName.Format("\\StringFileInfo\\%04x%04x\\", LOWORD(translate[0]), HIWORD(translate[0]));
        LPCTSTR valuebuf;
        CString fdName = blockName + szField;
        if (VerQueryValue(pVersion, (char*)(LPCTSTR)fdName, (void**)&valuebuf, &len))
			sRet = valuebuf;
        delete pVersion;
    }
    return sRet;
}

// получить версию
CString GetVersionField(LPCSTR szField)
{
    LPTSTR BufFileName = new char[256];
    GetModuleFileName(_Module.m_hInst, BufFileName, 256);
	CString ret = ::GetVersionField(BufFileName, szField);
	delete [] BufFileName;
	return ret;
}

// Возвращает строку, полную версию 1С++, как она показана  на
// закладке "О программе..."
// Например, "2.0.2.0 Nightly build 2006-05-15" или "2.0.3.0"
//
CString GetFullVersion()
{
	CString sVersion = ::GetVersionField("FileVersion");
	sVersion.Replace(" ", "");
	sVersion.Replace(',', '.');
	CString sPrivateBuild = ::GetVersionField("PrivateBuild");
	if (!sPrivateBuild.IsEmpty())
	{
		sVersion += ' ';
		sVersion += sPrivateBuild;
	}
	CString sSpecialBuild = ::GetVersionField("SpecialBuild");
	if (!sSpecialBuild.IsEmpty())
	{
		sVersion += ' ';
		sVersion += sSpecialBuild;
	}
	return sVersion;
}*/

// вызвать метод одного из глобальных контекстов
// например, "IBDir"
// в случае ошибки выдает исключение throw
LPCTSTR CallMethodOfGlobalContexts0(CString MethodName)
{
	CBLContext* pCont = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts(MethodName, pCont);
    if (nMethNum != -1)
    {
	  CValue retval;
      CValue* MasPar[1] = {0};
      CValue Par;
      MasPar[0] = &Par;

      if (pCont->CallAsFunc(nMethNum, retval, &MasPar[0])!=0)
      {
        const char *pC = retval.GetString().operator LPCSTR();
        //pathBase1C = pC;
        //bFindMethod = true;
		return pC;
      }
    }
	//throw LPCTSTR(NULL);
	RuntimeError("Не нашли метод 1С <%s>", MethodName);
	return "";
}

// вызвать метод одного из глобальных контекстов
// например, "IBDir"
// в случае ошибки выдает исключение throw
CValue CallMethodOfGlobalContexts1(CString MethodName, CValue& param1)
{
  CValue retval = CNumeric(0);

	CBLContext* pCont = NULL;
	int nMethNum = ::FindMethodOfGlobalContexts(MethodName, pCont);
    if (nMethNum != -1)
    {
	  CValue retval;
      const CValue *pParams[1] = {0};
      pParams[0] = &param1;
      //return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));

      if (pCont->CallAsFunc(nMethNum, retval, const_cast<CValue**>(pParams)))
      {
		return retval;
      }
    }
	//throw LPCTSTR(NULL);
	RuntimeError("Не нашли метод 1С <%s>", MethodName);
	return retval;
}

// найти метод одного из глобальных контекстов
// например, "IBDir"
// возвращает номер найденного метода
//	или -1, если метод не найден
//
// в параметр pBLContext возвращется указатель на найденный глобальный контекст
//
int FindMethodOfGlobalContexts(CString MethodName, CBLContext*& pBLContext)
{
    ULONG first = CBLContext::GetFirstLoadedContextID();
    for (ULONG ind = first; ind != 0; ind = CBLContext::GetNextLoadedContextID(ind))
    {
		CBLContext* pContLoaded = CBLContext::GetLoadedContext(ind);

		int nMethNum = pContLoaded->FindMethod((LPCTSTR)MethodName);
		if (nMethNum != -1)
		{
			pBLContext = pContLoaded;
			return nMethNum;
		}
	}
	return -1;
}
/*
CBLModule* GetModuleByContext(const CGroupContext* pCont)
{
  CBLModule * pMod = NULL;
	if(pCont->m_FlagAutoDestroy > 100)
  {
    pMod = ((CGCBoost::SGCInfo*)pCont->m_FlagAutoDestroy)->m_pMod;
  }
  return pMod;
}*/

// получить путь к документу
CString GetDocumentPath(CGetDoc7* pDoc)
{
	CString DocPath;

	if (pDoc)
	{
		long lMetaID=CConfigCont::DocumentToID(pDoc,DocPath);
		if(lMetaID>0)
			CConfigCont::DocumentToPath(pDoc,DocPath);
		else
			DocPath=pDoc->GetPathName();
//LogErr("lMetaID %d DocPath %s", lMetaID, DocPath);
//LogErr("lMetaID %d DocPath %s", lMetaID, pDoc->GetPathName());
//MessageBox("lMetaID %d DocPath %s", lMetaID, DocPath);
//MessageBox("lMetaID %d DocPath %s", lMetaID, pDoc->GetPathName());

		CTypedCont* m_pTypedCont = pDoc->m_pTypedCont;
		CString csPath;
		CConfigCont::DocumentToPath (m_pTypedCont, csPath);
//LogErr("csPath %s", csPath);
//MessageBox("csPath %s", csPath);

		if (lMetaID > 0)
		{
			CString str = GetModuleFullName (lMetaID, DocPath, 0, pMetaDataCont);
//LogErr("str %s", str);
//MessageBox("str %s", str);
			return str;
		}
	}
	return DocPath;
}

// получить номер метаданного обработки ? отчета
// если возвращаем 0, значит, неудача
//
long GetCalcVarMetaID(const CString& strFileNameA)
{
	CString strFileName = strFileNameA;
    strFileName.MakeUpper();

	int npos = strFileName.Find("@MD");
	//if(npos != -1)
/*	if(-1 == npos)
		return 0;
	else*/
	if(-1 != npos)
	{
		//bool res = false;
		CString nameRep = strFileName.Left(npos);
		CMetaDataCont *pMD = GetMetaData();  
		if(pMD)
		{
			CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
			if(pObj)
			{
				long lMetaID = pObj->GetID();
//MessageBox("lMetaID %d", lMetaID);
				return lMetaID;
			}
		}
	}

	return 0;
}

// получить внутренний путь к документу, 
//	передаем имя внутренней обработки - ОбработкаВнутриКонфигурации@MD
//
CString GetInternalPath(const CString& strFileNameA)
{
/*    //const_cast<CString*>(&strFileName)->MakeUpper();
	CString strFileName = strFileNameA;
    strFileName.MakeUpper();

	int npos = strFileName.Find("@MD");
	//if(npos != -1)
	if(-1 == npos)
		return "";
	else
	{
		//bool res = false;
		CString nameRep = strFileName.Left(npos);
		CMetaDataCont *pMD = GetMetaData();  
		if(pMD)
		{
			CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
			if(pObj)
			{
				long lMetaID = pObj->GetID();
*/
	long lMetaID = GetCalcVarMetaID(strFileNameA);
	if(lMetaID)
	{
		CTypedCont* pCont=NULL;
		CString DocPath;
		CConfigCont::IDToPath(lMetaID, "CalcVar", DocPath, &pCont, FALSE);
		if(pCont)
		{
			/*CTextDocument* pText = (CTextDocument*)pCont->GetTextDocument();        
			pText->GetText(*this);           
			res = true;*/
			if (lMetaID > 0)
			{
				CString str = GetModuleFullName (lMetaID, DocPath, 0, pMetaDataCont);
//LogErr("str %s", str);
//MessageBox("str %s", str);
				return str;
			}
			//return text;
		}
/*			}
		}*/
	}

	return "";
}

// получить внутренний путь к документу, 
//	передаем имя внутренней обработки - ОбработкаВнутриКонфигурации@MD
//
CString GetInternalStoragePath(const CString& strFileNameA)
{
/*	CString strFileName = strFileNameA;
    strFileName.MakeUpper();

	int npos = strFileName.Find("@MD");
	//if(npos != -1)
	if(-1 == npos)
		return "";
	else
	{
		//bool res = false;
		CString nameRep = strFileName.Left(npos);
		CMetaDataCont *pMD = GetMetaData();  
		if(pMD)
		{
			CMetaDataObj* pObj=(CMetaDataObj*)pMD->GetCalcVarDef(nameRep);
			if(pObj)
			{
				long lMetaID = pObj->GetID();
//MessageBox("lMetaID %d", lMetaID);
*/
	long lMetaID = GetCalcVarMetaID(strFileNameA);
	if(lMetaID)
	{
		CTypedCont* pCont=NULL;
		CString DocPath;
		CConfigCont::IDToPath(lMetaID, "CalcVar", DocPath, &pCont, FALSE);
		if(pCont)
		{
//MessageBox("DocPath %s", DocPath);
			return DocPath;
		}
	}
	return "";
}

CGetDoc7* GetActiveDocument()
{
		SECMDIFrameWnd* pFrame=(SECMDIFrameWnd*)AfxGetMainWnd();
		CMDIChildWnd* pChild=pFrame->MDIGetActive();
		if(pChild && pChild->IsWindowVisible())
		{
			pChild->ActivateFrame();
			CView* pView=pChild->GetActiveView();
			if(pView)
			{
				//pView->SetFocus(); // TODO нужно или нет??
				return (CGetDoc7*)pView->GetDocument();
			}
		}			
	return NULL;
}

static CFile::OpenFlags ofFileClassesReadMode = static_cast<CFile::OpenFlags>(CFile::modeRead | CFile::shareDenyNone);

static void _ReadFileToString(CFile& file, const CString& strFileName, CString& strDest)
{
	DWORD dwLength = file.GetLength();
	LPTSTR buff = strDest.GetBuffer(dwLength);
	
	file.Read(buff, dwLength);
	
	strDest.ReleaseBuffer(dwLength);
	file.Close();
}

// могут выбрасываться исключения
void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest)
{
	CFile file(strFileName, ofFileClassesReadMode);
	_ReadFileToString(file, strFileName, strDest);
}

// нет исключений, false - неуспешно, true - успешно
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest)
{
	CFile file;
	if (!file.Open(strFileName, ofFileClassesReadMode))
		return false;
	
	_ReadFileToString(file, strFileName, strDest);
	return true;
}
