// глобальные методы в целом для компоненты

#include "stdafx.h"
#include "GlobalMethods.h"
#include "../System/CreationBoost.h"

#include "..\TurboBL\gcboost.h"
#include "LogFile.h"

class CDataBase7* pDataBase7 = NULL;
class CMetaDataCont* pMetaDataCont = NULL;
class CBkEndUI* pBkEndUI = NULL;
boost::shared_ptr<class CAddIn> pMainAddIn;

const char* const pcstrNameOfDef = "defcls.prm"; 
const char* const TypeNamePrefix = "_1c_type_";
const char* const szDotSubst = "_1c_dot_";

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration)
{
	CCreateInstanceBoost::TheObject().RegisterCBLContext(pRuntimeClass, szClassName, bNativeRegistration);
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

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log<<"ShowMsg "<<txt<<CRLF;*/
}

void Msg(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	pBkEndUI->DoMessageLine(str.operator LPCTSTR(), mmBlueTriangle);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log<< "Msg " << str << CRLF;*/
}

void LogErr(const char* format,...)
{
	va_list arg;
	va_start(arg,format);
	CString txt;
	txt.FormatV(format,arg);
	
	CBkEndUI *pBkEndUI = GetBkEndUI();
	if (pBkEndUI)
		pBkEndUI->DoMessageLine(txt,mmBlackErr);
	else
		AfxMessageBox(txt);

	va_end(arg);

// 	if(!g_Log.IsOpen()){
// 		g_Log.Open("C:\\1cpp.txt");
// 		g_Log.Clear();
// 		g_Log<<"Program started at "<<CRLF;
// 	}
// 	g_Log << "1C++ LogErr " << txt << CRLF;
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

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at "<<CRLF;
	}
	g_Log << "MessageBox " << txt << CRLF;*/
}

//Функция сообщает об ошибке, и информирует 1С что произошло исключение
void RuntimeError(const char *msg, ...)
{
	CString str;
	va_list ap;
	va_start(ap, msg);
	str.FormatV(msg, ap);
	va_end(ap);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at " << CRLF;
	}
	g_Log << "RuntimeError " << str << CRLF;*/

	// чтобы не могло возникнуть исключения во время исключения
	// т.к. тогда 1С будет падать
	// а для полной гарантии нужно перехватить CBLModule::RaiseExtRuntimeError и
	// вызывать этот метода там !
	//
	//CRuntimeExceptionClass::Empty(); 

	if (CBLModule::GetExecutedModule())
		CBLModule::RaiseExtRuntimeError(str.operator LPCTSTR(), mmRedErr);
	else
		GetBkEndUI()->DoMessageLine(str.operator LPCTSTR(), mmRedErr);
}

void RuntimeErrorRes(UINT nID)
{
	CString err;
	err.LoadString(nID);

	/*if(!g_Log.IsOpen()){
		g_Log.Open("C:\\1cpp.txt");
		g_Log.Clear();
		g_Log<<"Program started at " << CRLF;
	}
	g_Log << "RuntimeErrorRes " << err << CRLF;*/

	CBLModule::RaiseExtRuntimeError(err, mmRedErr);
}

void AssertHResult(HRESULT hresult)
{
	if (hresult != S_OK)
	{
		LPVOID lpMsgBuf;
		DWORD res = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER|FORMAT_MESSAGE_FROM_SYSTEM, NULL,
			hresult, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
			(LPTSTR)&lpMsgBuf, 0, NULL);
		CString strErr;
		if (res)
			strErr = (LPTSTR)lpMsgBuf;
		else
			strErr.Format("Неизвестная ошибка: 0x%08X", hresult);
		LocalFree(lpMsgBuf);
		RuntimeError((LPCSTR)strErr);
	}
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
	if (obj)
	{
		CString Str;
		GetRTCFull(obj->GetRuntimeClass(), Str);
		Trace0(Str);
	}
}

//CVTExtended* CValue2VTExt(CValue const& Value)
//{
//	//if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;
//
// //   CBLContext* pVTContext = Value.GetContext();
//	//if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CVTExtended") == 0 )
//	//{
//	//	return reinterpret_cast<CVTExtended*>(pVTContext); // todo dynamic_cast
//	//}
//	//else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
//	//{
//	//	CValue vBaseClass;
//	//	if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("ИндексированнаяТаблица", vBaseClass) )
//	//	{
//	//		return reinterpret_cast<CVTExtended*>(vBaseClass.GetContext()); // todo dynamic_cast
//	//	}
//	//}
//
//	//return NULL;
//	return CValue2Type<CVTExtended>(Value);
//}

CValueTable* CValue2VT(CValue const& Value)
{
	//if( Value.GetTypeCode() != AGREGATE_TYPE_1C ) return NULL;

 //   CBLContext* pVTContext = Value.GetContext();
	//if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CValueTableContext") == 0 )
	//{
	//	return static_cast<CValueTableContextData*>(pVTContext->GetInternalData())->GetValueTable();
	//}
	//else if( strcmp(pVTContext->GetRuntimeClass()->m_lpszClassName, "CComponentClass") == 0 )
	//{
	//	CValue vBaseClass;
	//	if( static_cast<CComponentClass*>(pVTContext)->GetBaseClass("ТаблицаЗначений", vBaseClass) )
	//	{
	//		pVTContext = vBaseClass.GetContext();
	//		return static_cast<CValueTableContextData*>(pVTContext->GetInternalData())->GetValueTable();
	//	}
	//}

	//return NULL;
	CValueTableContext* pTableContext = CValue2AfxType<CValueTableContext*>(Value, "ТаблицаЗначений");
	if (!pTableContext)
		return NULL;
	return static_cast<CValueTableContextData*>(pTableContext->GetInternalData())->GetValueTable();
}

CValueListContextFriend* CValue2VLC(CValue const& Value)
{
	CValueListContext* pListContext = CValue2AfxType<CValueListContext*>(Value, "СписокЗначений");
	return static_cast<CValueListContextFriend*>(pListContext);
}

CValueItemList* CValue2VL(CValue const& Value)
{
	CValueListContext* pListContext = CValue2VLC(Value);
	if (!pListContext)
		return NULL;
	return pListContext->GetPValueList();
}

CString strLastError(DWORD dwMessageID /* = 0*/)
{
	LPTSTR cstr = NULL;
	::FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL,
		dwMessageID==0 ? GetLastError() : dwMessageID,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		cstr,
		0,
		NULL
		);
	CString res(cstr);
	::LocalFree(cstr);
	return res;
}

// Переназначить/снять переназначение ветки HKEY_CLASSES_ROOT в ветку HKEY_CURRENT_USER

typedef LONG (APIENTRY f_LONG_HKEY_HKEY)(HKEY, HKEY);
void RedirectHKCR(BOOL bRedirect)
{
	static f_LONG_HKEY_HKEY* pRegOverridePredefKey = NULL;
	static BOOL bUnsupported = FALSE;

	if (!pRegOverridePredefKey && !bUnsupported)
	{
// 		BYTE dwWindowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
// 		if (dwWindowsMajorVersion < 5) // Ранее, чем Windows 2000
		if (!IsSystem_Windows2000AndHigher())
			bUnsupported = TRUE;
		else
		{
			HMODULE hm = GetModuleHandle("ADVAPI32.DLL");
			if (hm)
				pRegOverridePredefKey = reinterpret_cast<f_LONG_HKEY_HKEY*>(GetProcAddress(hm, "RegOverridePredefKey"));

			if (!pRegOverridePredefKey)
				bUnsupported = TRUE;
		}
	}
	if (bUnsupported)
		return;

	if (bRedirect)
	{
		HKEY hNewHKey; 
		RegOpenKeyEx(HKEY_CURRENT_USER, "Software\\Classes", 0, KEY_READ|KEY_WRITE , &hNewHKey  ); 
		(*pRegOverridePredefKey)(HKEY_CLASSES_ROOT, hNewHKey); 
		RegCloseKey(hNewHKey);
	} else
		(*pRegOverridePredefKey)(HKEY_CLASSES_ROOT, NULL); 
}

// проверка на существование файла или папки
bool FileExists(CString fileName)
{
	CString sAbsoluteFilePath(::AbsoluteFilePath(fileName));
	DWORD tRes = GetFileAttributes(sAbsoluteFilePath.operator LPCTSTR());
/*	if (tRes != 0xFFFFFFFF)
		if (!(tRes & FILE_ATTRIBUTE_DIRECTORY))
			bFileExists = true;*/
	return tRes != 0xFFFFFFFF && !(tRes & FILE_ATTRIBUTE_DIRECTORY);
}

bool FolderExists(CString folderName)
{
	DWORD tRes = GetFileAttributes(folderName.operator LPCTSTR());
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
	LPCSTR pName = FileName.operator LPCTSTR();
	LPCSTR s = strrchr(pName, PathDelim);
	if (!s) return "";

	int count = (s-pName)+1+1; // 1 байт под '\0'
	std::vector<char> buf(count); //char* _ret = new char[count];
	char* _ret = &buf[0];
	lstrcpyn(_ret, pName, count);
	CString ret = _ret;
	//delete [] _ret;
	return ret;
}

// получить абсолютный путь к файлу\папке
// учитывается наличие "." или ".."
//
CString AbsoluteFilePath(const CString sRelativePath)
{
	std::vector<char> buf(_MAX_PATH); //LPSTR fullpath = new char[_MAX_PATH];
	LPSTR fullpath = &buf[0];
	if (_fullpath( fullpath, (LPCTSTR)sRelativePath, _MAX_PATH ) == NULL )
		return ""; // TODO или выбросить исключение
	CString ret(fullpath);

	//delete [] fullpath;
	return ret;
}

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
		std::vector<char> buf(size); //char* pVersion = new char[size];
		char* pVersion = &buf[0];
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
        //delete pVersion;
    }
    return sRet;
}

// получить версию
CString GetVersionField(LPCSTR szField)
{
	std::vector<char> buf(256); //LPTSTR BufFileName = new char[256];
	LPTSTR BufFileName = &buf[0];
    GetModuleFileName(_Module.m_hInst, BufFileName, 256);
	CString ret = ::GetVersionField(BufFileName, szField);
	//delete [] BufFileName;
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
}

// вызвать метод одного из глобальных контекстов
// например, "IBDir"
// в случае ошибки выдает исключение throw
LPCTSTR CallMethodOfGlobalContexts0(const CString& MethodName)
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
	RuntimeError("Не нашли метод 1С <%s>", MethodName.operator LPCTSTR());
	return "";
}

// вызвать метод одного из глобальных контекстов
// например, "IBDir"
// в случае ошибки выдает исключение throw
CValue CallMethodOfGlobalContexts1(const CString& MethodName, CValue& param1)
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
	RuntimeError("Не нашли метод 1С <%s>", MethodName.operator LPCTSTR());
	return retval;
}

// найти метод одного из глобальных контекстов
// например, "IBDir"
// возвращает номер найденного метода
//	или -1, если метод не найден
//
// в параметр pBLContext возвращется указатель на найденный глобальный контекст
//
int FindMethodOfGlobalContexts(const CString& MethodName, CBLContext*& pBLContext)
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

CBLModule* GetModuleByContext(const CGroupContext* pCont)
{
//   CBLModule * pMod = NULL;
// 	if(pCont->m_FlagAutoDestroy > 100)
//   {
//     pMod = ((CGCBoost::SGCInfo*)pCont->m_FlagAutoDestroy)->m_pMod;
//   }
//   return pMod;
	return CGCBoost::GetModuleByContext(pCont);
}

// получить контекст формы из контекста
CFormAllCtrlsContext* GetFormContext(const CBLContext* pCont)
{
	if (!pCont)
		return NULL;

	CBLContext* pCtxForm = NULL;//TContPtr pCtxForm = NULL;
	if(IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext))
	{
		int nForm=pCont->FindProp("Форма");
		if(nForm>=0)
		{
			CValue vForm;
			pCont->GetPropVal(nForm, vForm);
			pCtxForm=vForm.GetContext();
		}
	}
	else
	{
// 		pCtxForm = const_cast<TContPtr>(pCont);
		pCtxForm = const_cast<CBLContext*>(pCont);
	}

	if(pCtxForm && !strcmp(pCtxForm->GetRuntimeClass()->m_lpszClassName, "CFormAllCtrlsContext"))
	{
		return static_cast<CFormAllCtrlsContext*>(pCtxForm);
	}

	return NULL;
}

CGetDoc7* GetDoc7(const CBLContext* pCont)
{
	CFormAllCtrlsContext* pFormContext = GetFormContext(pCont);
	return !pFormContext ? NULL : pFormContext->m_GetDoc;
}

// получить модуль проведения документа из контекста документа
CDocActModule* GetDocumentTransactionModule(CDocContext* pDoc)
{
	CDocTransacter* pDocTransacter = pDoc->m_pDocTransacter;
	//CValue rValue;
	//pDoc->CallAsFunc(38, rValue, NULL); // Док.Провести
	//pDoc->CallAsFunc(39, rValue, NULL); // Док.СделатьНеПроведенным
	if (!pDocTransacter)
		return NULL;
	
	return pDocTransacter->GetAt(-1);
}

// получить контекст документа из группового контекста
CDocContext* GetDocContextFromGroupContext(CGroupContext* pGroupCont)
{
	if(pGroupCont)
	{
		for(int i = 0; i<pGroupCont->GetSize(); i++)
		{
			CBLContext *pCont = pGroupCont->GetContext(i);
			if(IS_KINDOF_RUNTIME_CLASS(pCont, CDocContext))
				return (CDocContext*)pCont;
		}
	}
	return NULL;
}

// получить контекст документа из формы документа
CDocContext* GetDocContextFromCGetDoc7(CGetDoc7* pDoc)
{
	return GetDocContextFromGroupContext(pDoc->GetGroupContext());
}

// проверить параметры CValue
void ValidateParamContext(const CValue& ContValue, const CString& strErrorMessage)
{
	CBLContext* pCont = ContValue.GetContext();

	if (!pCont)
		RuntimeError(strErrorMessage);
}

void ValidateParamContext(const CValue& ContValue, const CString& strContextType, const CString& strErrorMessage)
{
	CBLContext* pCont = ContValue.GetContext();

	if (!pCont)
		RuntimeError(strErrorMessage);

	if (strcmp(pCont->GetRuntimeClass()->m_lpszClassName, strContextType))
		RuntimeError(strErrorMessage);
}

void ValidateParamGroupContext(const CValue& GroupContValue, const CString& strErrorMessage)
{
	CBLContext* pGroupCont = GroupContValue.GetContext();

	if (!IS_KINDOF_CGroupContext(pGroupCont))
		RuntimeError(strErrorMessage);
}

void ValidateParamNumeric(const CValue& NumericValue, const CString& strErrorMessage)
{
	if (1 != NumericValue.GetTypeCode())
		RuntimeError(strErrorMessage);
}

void ValidateParamString(const CValue& StringValue, const CString& strErrorMessage)
{
	if (2 != StringValue.GetTypeCode())
		RuntimeError(strErrorMessage);
}

void ValidateParamNotEmptyString(const CValue& StringValue, const CString& strErrorMessage)
{
	if (2 != StringValue.GetTypeCode())
		RuntimeError(strErrorMessage);

	CString strPar = StringValue.GetString();
	if (strPar.IsEmpty())
		RuntimeError(strErrorMessage);
}

void ValidateParamValueListContext(const CValue& ValueListValue, const CString& strErrorMessage)
{
	ValidateParamContext(ValueListValue, "CValueListContext", strErrorMessage);
}

void ValidateParamUserClass(const CValue& UserClassContValue, const CString& strErrorMessage)
{
	CBLContext* pUserClassCont = UserClassContValue.GetContext();

	if (!IS_KINDOF_CComponentClass(pUserClassCont))
		RuntimeError(strErrorMessage);
}

extern CDataBase7 * pDataBase7;
bool IsSQLVersion(void)
{
  return "CODBCDB7" == CString(pDataBase7->GetRuntimeClass()->m_lpszClassName);

};

// Windows 2000/ХП или позже ?
bool IsSystem_Windows2000AndHigher()
{
	const BYTE dwMajorVersion_Windows2000AndHigher = 5;

	BYTE dwWindowsMajorVersion = LOBYTE(LOWORD(GetVersion()));
	if (dwWindowsMajorVersion < dwMajorVersion_Windows2000AndHigher)
		return false;
	
	return true;
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

void DumpContext(CBLContext* pCont)
{
	pBkEndUI->DoMessageLine("", mmBlueTriangle);

	// original section by Deb
	pBkEndUI->DoMessageLine(pCont->GetRuntimeClass()->m_lpszClassName, mmInformation);

	int nMeth = pCont->GetNMethods();
	if (nMeth > 0)
		pBkEndUI->DoMessageLine("==============================\r\nMethods\r\n==============================", mmInformation);
	for (int i = 0; i < nMeth; i++)
	{
		char buf[1000] = {0};
		char whatIs[100] = {0};
		if (pCont->HasRetVal(i)==1)
			strcpy(whatIs, "Function ");
		else
			strcpy(whatIs, "Procedure ");

		int nParams = pCont->GetNParams(i);

		sprintf(buf, "%d. %s [%s]  [%s] nParams = %d",i,whatIs,pCont->GetMethodName(i, 0), pCont->GetMethodName(i, 1), nParams);
		pBkEndUI->DoMessageLine(buf, mmInformation);
	}
	int nProps = pCont->GetNProps();
	if (nProps > 0)
		pBkEndUI->DoMessageLine("==============================\r\nProperties\r\n==============================", mmInformation);

	for (int i = 0; i < nProps; i++)
	{
		char buf[1000] = {0};
		char whatIs[100] = {0};

		if (pCont->IsPropReadable(i)==0)
			strcpy(whatIs, "Write only ");
		if (pCont->IsPropWritable(i)==0)
			strcpy(whatIs, "Read only ");

		//int nParams = pCont->GetNParams(i);

		sprintf(buf, "%d. [%s]  [%s]  %s",i,pCont->GetPropName(i, 0), pCont->GetPropName(i, 1), whatIs);
		pBkEndUI->DoMessageLine(buf, mmInformation);
	}
}

// вариант передачи информации о числе параметров метода контекста
// для методов с неопределенным числом параметров
void CallIsOleContext(CBLContext* pCont, int nParams)
{
	__asm
	{
		push ebx;
		mov ebx, nParams;
	}
	pCont->IsOleContext();
	__asm
	{
		pop ebx;
	}
}

void DispatchToValue(IUnknown* pUnk, CValue* pVal)
{
	pVal->Reset();
	IDispatchPtr pDisp = pUnk;
	if(pDisp != NULL)
	{
		VARIANT var;
		var.vt		 = VT_DISPATCH;
		var.pdispVal = pDisp;
		DISPPARAMS disp = {&var, NULL, 1, 0};
		UINT ui;
		static_cast<CBLMyEx*>(NULL)->DispParamToValueEx(&pVal, &disp, &ui);
	}
}

#pragma comment(lib, "version.lib")

CString GetModuleVersionInfo(HMODULE hModule, CString VerInfoName)
//VerInfoName: FileVersion, PrivateBuild, etc
{
	CString res;

	char* fname = new char[MAX_FNAME_LEN];
	GetModuleFileName(hModule, fname, MAX_FNAME_LEN);

	DWORD size, z;
	size = ::GetFileVersionInfoSize(fname, &z);

	if(size)
	{
		static const char* EngNames[]={
			"FileVersion",
			"PrivateBuild",
		};
		char* pVersion=new char[size];
		GetFileVersionInfo(fname, 0, size, pVersion);

		WORD* translate;
		UINT len;
		VerQueryValue(pVersion, "\\VarFileInfo\\Translation", (void**)&translate, &len);
		CString blockName;
		blockName.Format("\\StringFileInfo\\%04x%04x\\%s",translate[0], translate[1], VerInfoName);

		LPCTSTR valuebuf;
		VerQueryValue(pVersion, (char*)(LPCTSTR)(blockName), (void**)&valuebuf, &len);
		res = valuebuf;

		delete pVersion;
	}
	delete fname;

	return res;
}

CString GetModuleVersionInfo(CString ModuleName, CString VerInfoName)
//VerInfoName: FileVersion, PrivateBuild, etc
{
	return GetModuleVersionInfo(GetModuleHandle(ModuleName), VerInfoName);
}

void TraceMenu(CMenu* Menu, CString Indent)
{
	if (Menu)
	{
		CString str;
		MENUITEMINFO mii;
		memset(&mii, 0, sizeof MENUITEMINFO);
		mii.cbSize = sizeof MENUITEMINFO;
		mii.fMask = MIIM_TYPE|MIIM_STATE;

		for (UINT i=0; i < Menu->GetMenuItemCount(); i++)
		{
			Menu->GetMenuString(i, str, MF_BYPOSITION);
			DWORD id = Menu->GetMenuItemID(i);
			
			Menu->GetMenuItemInfo(i, &mii, TRUE);

			CString strType;
			if (mii.fMask & MIIM_TYPE)
			{
				if (mii.fType & MFT_BITMAP) strType += "|MFT_BITMAP";
				if (mii.fType & MFT_MENUBARBREAK) strType += "|MFT_MENUBARBREAK";
				if (mii.fType & MFT_MENUBREAK) strType += "|MFT_MENUBREAK";
				if (mii.fType & MFT_OWNERDRAW) strType += "|MFT_OWNERDRAW";
				if (mii.fType & MFT_RADIOCHECK) strType += "|MFT_RADIOCHECK";
				if (mii.fType & MFT_RIGHTJUSTIFY) strType += "|MFT_RIGHTJUSTIFY";
				if (mii.fType & MFT_RIGHTORDER) strType += "|MFT_RIGHTORDER";
				if (mii.fType & MFT_SEPARATOR) strType += "|MFT_SEPARATOR";
				if (mii.fType & MFT_STRING) strType += "|MFT_STRING";
			}
			else
				strType = "NoType";

			CString strState;
			if (mii.fMask & MIIM_STATE)
			{
				if (mii.fState & MFS_CHECKED) strState += "|MFS_CHECKED";
				if (mii.fState & MFS_DEFAULT) strState += "|MFS_DEFAULT";
				if (mii.fState & MFS_DISABLED) strState += "|MFS_DISABLED";
				if (mii.fState & MFS_ENABLED) strState += "|MFS_ENABLED";
				if (mii.fState & MFS_GRAYED) strState += "|MFS_GRAYED";
				if (mii.fState & MFS_HILITE) strState += "|MFS_HILITE";
				if (mii.fState & MFS_UNCHECKED) strState += "|MFS_UNCHECKED";
				if (mii.fState & MFS_UNHILITE) strState += "|MFS_UNHILITE";
			}
			else
				strState = "NoState";

			Msg("%sName:%s (ID:%08X)  TYPE : %s  STATE : %s", Indent, str, id, strType, strState);

			TraceMenu(Menu->GetSubMenu(i), Indent+"  ");
		}
	}
}
