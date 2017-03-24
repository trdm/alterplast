// WorkAsRegister.cpp: implementation of the CWorkAsRegister class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "addin.h"
#include "WorkAsRegister.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

IMPLEMENT_DYNCREATE(CWorkAsRegister, CBLContext);

class CObjID CWorkAsRegister::ObjID;

void CWorkAsRegister::ShowError(SExcept& e, bool bEnableDebug)
{
	if (bEnableDebug == false) return;

	LPVOID lpMsgBuf;
	FormatMessage( 
		FORMAT_MESSAGE_ALLOCATE_BUFFER | 
		FORMAT_MESSAGE_FROM_SYSTEM | 
		FORMAT_MESSAGE_IGNORE_INSERTS,
		NULL,
		e.lCodeError,
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
		(LPTSTR) &lpMsgBuf,
		0,
		NULL 
	);
	// Process any inserts in lpMsgBuf.
	// ...
	// Display the string.
	CBLModule *pCurModule = CBLModule::GetExecutedModule();
	if (pCurModule) {
		CString str(32,500);
		char buf[40];
		int nLine = pCurModule->GetExecutedLineNum();
		pCurModule->GetCurSourceLine(nLine, str);
		sprintf(buf, "ќшибка в модуле строка: %d", nLine);
		pBkEndUI->DoMessageLine(buf, mmExclamation);
		str.TrimLeft();
		pBkEndUI->DoMessageLine(str, mmExclamation);
	}
	pBkEndUI->DoMessageLine((LPCTSTR)lpMsgBuf, mmExclamation);
	// Free the buffer.
	LocalFree( lpMsgBuf );
}

struct CWorkAsRegister::paramdefs CWorkAsRegister::defFnNames[] = {
	{{"EnableDebug", "ќтладочные—ообщени€"},0,1},
	{{"CreateKey", "—оздать люч"},1,3},
	{{"OpenKey", "ќткрыть люч"},1,3},
	{{"DeleteKey", "”далить люч"},1,1},
	{{"DeleteValue", "”далить«начение"},1,1},
	{{"SetValue", "”становить«начение"},1,3},
	{{"QueryValue", "«апросить«начение"},1,2},
	{{"ChooseKeys", "¬ыбрать лючи"},1,0},
	{{"GetKey", "ѕолучить люч"},1,1},
	{{"ChooseValues", "¬ыбрать«начени€"},1,0},
	{{"GetValue", "ѕолучить«начение"},1,1},
	{{"Flash", "«аписать¬сејтрибуты"},0,0},
	{{"ConnectRegistry", "ѕодключить–егистр"},1,2},
	{NULL,0,0}
};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CWorkAsRegister::CWorkAsRegister()
{
	pBLModule = new CBLModule7(NULL, NULL);
	bEnableDebug = false;
	nPosOfVal = 0;
	nPosOfKey = 0;
#ifdef _DEBUG
	pBkEndUI->DoMessageLine("Creating CWorkAsRegister class",mmInformation);
#endif
}

CWorkAsRegister::~CWorkAsRegister()
{
	delete pBLModule;
#ifdef _DEBUG
	pBkEndUI->DoMessageLine("Destroying CWorkAsRegister class",mmInformation);
#endif
}

int  CWorkAsRegister::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	switch(iMethNum)
	{
	case methCreateKey: 
	case methOpenKey:
		{
			CValue *hKey = ppValue[0];
			CValue *Name = ppValue[1];
			CValue *Desired = ppValue[2];
			
			CString str = hKey->GetString();
			HKEY key;
			if (str == "HKEY_CLASSES_ROOT") {
				key = HKEY_CLASSES_ROOT;
			}
			else if (str == "HKEY_CURRENT_CONFIG") {
				key = HKEY_CURRENT_CONFIG;
			}
			else if (str == "HKEY_CURRENT_USER") {
				key = HKEY_CURRENT_USER;
			}
			else if (str == "HKEY_LOCAL_MACHINE") {
				key = HKEY_LOCAL_MACHINE;
			}
			else if (str == "HKEY_USERS") {
				key = HKEY_USERS;
			}
			else {
				char buf[100];
				sprintf(buf, "Ќеверный ключ: %s", str);
				pBkEndUI->DoMessageLine(buf,mmExclamation);
				rValue = CNumeric(0);
				return 1;
			}
			str = Desired->GetString();
			REGSAM samDesired;
			if (str == "KEY_CREATE_LINK")
				samDesired = KEY_CREATE_LINK;
			else if (str == "KEY_CREATE_SUB_KEY")
				samDesired = KEY_CREATE_SUB_KEY;
			else if (str == "KEY_ENUMERATE_SUB_KEYS")
				samDesired = KEY_ENUMERATE_SUB_KEYS;
			else if (str == "KEY_EXECUTE")
				samDesired = KEY_EXECUTE;
			else if (str == "KEY_NOTIFY")
				samDesired = KEY_NOTIFY;
			else if (str == "KEY_QUERY_VALUE")
				samDesired = KEY_QUERY_VALUE;
			else if (str == "KEY_SET_VALUE")
				samDesired = KEY_SET_VALUE;
			else if (str == "KEY_ALL_ACCESS")
				samDesired = KEY_ALL_ACCESS;
			else if (str == "KEY_READ")
				samDesired = KEY_READ;
			else if (str == "KEY_WRITE")
				samDesired = KEY_WRITE;
			else {
				char buf[100];
				sprintf(buf, "Ќеверно указан режим: %s", str);
				pBkEndUI->DoMessageLine(buf,mmExclamation);
				rValue = CNumeric(0);
				return 0;
			}
			if (iMethNum == methCreateKey) {
				DWORD dwDisposition;
				try {
					CreateKey(key, Name->GetString(), samDesired, &dwDisposition);
					if (dwDisposition == REG_CREATED_NEW_KEY)
						rValue = CNumeric(1);
					else if (dwDisposition == REG_OPENED_EXISTING_KEY)
						rValue = CNumeric(0);
				}
				catch (CWorkAsRegister::SExcept& e) {
					rValue = CNumeric(-1);
					ShowError(e, bEnableDebug);
					return 0;
				}
			}
			else if (iMethNum == methOpenKey) {
				try {
					OpenKey(key, Name->GetString(), samDesired);
					rValue = CNumeric(1);
				}
				catch (CWorkAsRegister::SExcept& e) {
					rValue = CNumeric(0);
					ShowError(e, bEnableDebug);
					return 0;
				}
			}
			break;
		}
	case methConnectRegistry:
		{
			CValue *NameComputer = ppValue[0];
			CValue *hKey = ppValue[1];

			CString str = hKey->GetString();
			HKEY key;
			if (str == "HKEY_LOCAL_MACHINE") {
				key = HKEY_LOCAL_MACHINE;
			}
			else if (str == "HKEY_USERS") {
				key = HKEY_USERS;
			}
			else {
				char buf[100];
				sprintf(buf, "Ќеверно указан режим: %s", str);
				pBkEndUI->DoMessageLine(buf,mmExclamation);
				rValue = CNumeric(0);
				return 0;
			}
			try {

				str = NameComputer->GetString();
				ConnectRegistry(key, str);
				rValue = CNumeric(1);
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			break;
		}
	case methDeleteKey: case methDeleteValue:
		{
			CString strName = ppValue[0]->GetString();
			try {
				if (methDeleteKey == iMethNum) {
					nPosOfKey = (nPosOfKey <= 0) ? 0 : --nPosOfKey;
					DeleteKey(strName);
				}
				else if (methDeleteValue == iMethNum) {
					nPosOfVal = (nPosOfVal <= 0) ? 0 : --nPosOfVal;
					DeleteValue(strName);
				}
				dwCount = (dwCount <= 0) ? 0 : --dwCount;
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			rValue = CNumeric(1);
			break;
		}
	case methSetValue:
		{
			CString strName   = ppValue[0]->GetString();
			CString strType   = ppValue[1]->GetString();
			CValue *pVal      = ppValue[2];

			try {
				if (strType == "REG_DWORD") {
					DWORD dwVal = pVal->GetNumeric().operator long();
					SetValue(strName, REG_DWORD, (PBYTE)&dwVal, sizeof(dwVal));
				}
				else if (strType == "REG_SZ") {
					CString str = pVal->GetString();
					int nSize = str.GetLength();
					char *pcVal = str.GetBuffer(nSize);
					SetValue(strName, REG_SZ, (PBYTE)pcVal, nSize+1);
				}
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			rValue = CNumeric(1);
			break;
		}
	case methQueryValue:
		{
			CString strName   = ppValue[0]->GetString();
			CValue *pVal      = ppValue[1];
			DWORD dwType;
			try {
				QueryValue(strName, &dwType, NULL, NULL);
			}
			catch(CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			if (dwType == REG_DWORD) {
#ifdef _DEBUG
				pBkEndUI->DoMessageLine("dwType == REG_DWORD", mmExclamation);
#endif
				DWORD dwVal = 0;
				DWORD cbData = sizeof(dwVal);
				QueryValue(strName, &dwType, (PBYTE)&dwVal, &cbData);
				pVal->Reset();
				*pVal = CNumeric((long)dwVal);
			}
			else if (dwType == REG_SZ) {
#ifdef _DEBUG
				pBkEndUI->DoMessageLine("dwType == REG_SZ", mmExclamation);
#endif
				DWORD cbData = 1024;
				char* pStr = new char[cbData];
				try {
					QueryValue(strName, NULL, (PBYTE)pStr, &cbData);
				}
				catch (CWorkAsRegister::SExcept& e) {
					if (e.lCodeError == ERROR_MORE_DATA) {
						delete[] pStr;
						pStr = new char[cbData];
						QueryValue(strName, NULL, (PBYTE)pStr, &cbData);
					}
					else {
						rValue = CNumeric(0);
						ShowError(e, bEnableDebug);
						return 0;
					}
				}
				pVal->Reset();
				*pVal = pStr;
				delete[] pStr;
			}
			else {
#ifdef _DEBUG
				pBkEndUI->DoMessageLine("Another dwType!", mmExclamation);
#endif
				rValue = CNumeric(0);
				return 0;
			}

			rValue = CNumeric(1);
			break;
		}
	case methChooseKeys:
		{
			dwCount = dwMaxLen = nPosOfKey = 0;
			try {
				InfoKey(&dwCount, &dwMaxLen, NULL, NULL, NULL, NULL);
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			rValue = CNumeric((long)dwCount);
			break;
		}
	case methGetKey:
		{
			if (dwCount <= 0 || dwCount == nPosOfKey) { 
				rValue = CNumeric(0);
				return 0;
			}
			try {
				CValue *pVal     = ppValue[0];
				char *strNameOEM = new char[dwMaxLen+1];
				DWORD dwLen = dwMaxLen+1;
				EnumKey(nPosOfKey++, strNameOEM, &dwLen, NULL, NULL, NULL);
				pVal->Reset();
				*pVal = strNameOEM;
				delete[] strNameOEM;
				rValue = CNumeric(1);
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			break;
		}

	case methChooseValues:
		{
			dwCount = dwMaxLen = nPosOfVal = 0;
			try {
				InfoKey(NULL, NULL, &dwCount, &dwMaxLen, NULL, NULL);
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			rValue = CNumeric((long)dwCount);
			break;
		}
	case methGetValue:
		{
			if (dwCount <= 0 || dwCount == nPosOfVal) { 
				rValue = CNumeric(0);
				return 0;
			}
			try {
				CValue *pVal     = ppValue[0];
				char *strNameOEM = new char[dwMaxLen+1];
				DWORD dwLen = dwMaxLen+1;
				EnumValue(nPosOfVal++, strNameOEM, &dwLen, NULL, NULL, NULL);
				pVal->Reset();
				*pVal = strNameOEM;
				delete[] strNameOEM;
				rValue = CNumeric(1);
			}
			catch (CWorkAsRegister::SExcept& e) {
				rValue = CNumeric(0);
				ShowError(e, bEnableDebug);
				return 0;
			}
			break;
			break;
		}
	default:
		CBLProcInfo ProcInfo;
		pBLModule->GetProcInfo(iMethNum-lastMethod,ProcInfo);
		int j = ProcInfo.GetNParams();
		int i = pBLModule->CallAsFunc(iMethNum - lastMethod,rValue,j,ppValue);
		return i;
	};
	return 1;
}

int  CWorkAsRegister::CallAsProc(int iMethNum, class CValue * * ppValue)
{
	CValue* pValue = NULL;
	CBLContext* pContext = NULL;
	CType* pType = NULL;
	int iNumMethod=-1;

	switch(iMethNum)
	{
	case methEnableDebug:
		bEnableDebug = (ppValue[0]->GetNumeric() == 0) ? false : true;
#ifdef _DEBUG
		char buf[20];
		sprintf(buf, "Size of CWorkAsRegister = %d", sizeof(*this));
		pBkEndUI->DoMessageLine(buf, mmInformation);
#endif
		break;
	case methFlash:
		try {
			Flash();
		}
		catch (CWorkAsRegister::SExcept& e) {
			ShowError(e, bEnableDebug);
			return 0;
		}
		break;
	default:
		return 0;	
	};
	return 1;
}


int  CWorkAsRegister::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}
	i		= pBLModule->FindFunc(lpMethodName);
	i = pBLModule->FindFunc(lpMethodName);
	if (i>=0) 
		return i+lastMethod;
	i = pBLModule->FindProc(lpMethodName,0);
	if(i>=0)
		return i+lastMethod;
	else return i;
}

char const *  CWorkAsRegister::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	if (iMethodNum >= lastMethod) 
		return "mError";
	else
		return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CWorkAsRegister::GetNMethods(void)const
{
	return pBLModule->GetNProcs() + lastMethod;
}

int  CWorkAsRegister::HasRetVal(int iMethodNum)const
{
	CBLProcInfo ProcInfo;
	if (iMethodNum >= lastMethod) 
	{
		pBLModule->GetProcInfo(iMethodNum-lastMethod,ProcInfo);
		return ProcInfo.HasRetVal();
	}
	else
		return defFnNames[iMethodNum].HasReturnValue ;
}

int CWorkAsRegister::GetNParams(int iMethodNum)const
{
	CBLProcInfo ProcInfo;
	if (iMethodNum >= lastMethod) 
	{
		pBLModule->GetProcInfo(iMethodNum-lastMethod,ProcInfo);
		return ProcInfo.GetNParams();
	} 
	else
		return defFnNames[iMethodNum].NumberOfParams;
}

int  CWorkAsRegister::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
	if (iMethodNum >= lastMethod)
	{
		return 1;
	}
	return 0;
}



void  CWorkAsRegister::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  CWorkAsRegister::GetCode(void)const
{
	return 0;
}

int  CWorkAsRegister::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CWorkAsRegister::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CWorkAsRegister::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CWorkAsRegister::GetInternalData(void)
{

	return CBLContext::GetInternalData();

}




long  CWorkAsRegister::GetTypeID(void)const
{
	return 204;
}

char const *  CWorkAsRegister::GetTypeString(void)const
{
	return "–абота—–егистромWin";
}

class CType   CWorkAsRegister::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  CWorkAsRegister::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CWorkAsRegister::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CWorkAsRegister::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CWorkAsRegister::IsExactValue(void)const
{
	return 0;
}

int  CWorkAsRegister::IsOleContext(void)const
{
	return 0;
}

int  CWorkAsRegister::IsPropReadable(int iPropNum)const
{
	return 1;
}

int  CWorkAsRegister::IsPropWritable(int iPropNum)const
{
	return 1;
}

int  CWorkAsRegister::IsSerializable(void)
{
	return 0;
}

int  CWorkAsRegister::SaveToString(class CString & csStr)
{
	csStr = "Not saves state this object! Sorry!";
	return 1;
}

void  CWorkAsRegister::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CWorkAsRegister::GetNProps(void)const
{
	return pBLModule->GetNStaticVars();
}

char const *  CWorkAsRegister::GetPropName(int A,int B)const
{
	CBLVarInfo VarInfo;
	pBLModule->GetStaticVarDescr(A,VarInfo);
	return VarInfo.GetName();
}

int  CWorkAsRegister::GetPropVal(int iPropNum,class CValue & rValue)const
{
	return pBLModule->GetStaticVarValue(iPropNum,rValue,0);
}

int  CWorkAsRegister::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return pBLModule->SetStaticVarValue(iPropNum,vValue,0);
}

int  CWorkAsRegister::FindProp(char const * Name)const
{
	return pBLModule->FindStaticVar(Name);
}

void CWorkAsRegister::
OpenKey(HKEY key,LPCTSTR lpcName,REGSAM samDesired) {
	if (hKey) RegCloseKey(hKey);
	long res = RegOpenKeyEx(key, lpcName, NULL, samDesired,&hKey);	
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
CreateKey(HKEY key, LPCTSTR lpcNameValue, REGSAM samDesired, LPDWORD lpdwDisposition = NULL) {
	if (hKey) RegCloseKey(hKey);
	long res = 
		RegCreateKeyEx(key, lpcNameValue, NULL, NULL, REG_OPTION_NON_VOLATILE, samDesired, NULL, &hKey, lpdwDisposition);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
SetValue(LPCTSTR lpcNameValue, REGSAM samDesired, BYTE* pValue, DWORD dwSize)
{
	long res = RegSetValueEx(hKey, lpcNameValue, NULL, samDesired, pValue, dwSize);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
QueryValue(LPCTSTR lpcNameValue, LPDWORD lpType, BYTE* pValue, LPDWORD pDwSize)
{
	long res = RegQueryValueEx(hKey, lpcNameValue, NULL, lpType, pValue, pDwSize);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
DeleteKey(LPCTSTR lpSubKey)
{
	long res = RegDeleteKey(hKey, lpSubKey);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
DeleteValue(LPCTSTR lpValueName)
{
	long res = RegDeleteValue(hKey, lpValueName);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
void CWorkAsRegister::
Flash()
{
	long res = RegFlushKey(hKey);
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
bool CWorkAsRegister::
EnumKey(DWORD dwIndex, LPTSTR lpName, LPDWORD lpcName, LPTSTR lpClass, 
		     LPDWORD lpcClass, PFILETIME lpftLastWriteTime)
{	
	long res = RegEnumKeyEx(
							  hKey,                  // handle to key to enumerate
							  dwIndex,               // subkey index
							  lpName,                // subkey name
							  lpcName,               // size of subkey buffer
							  NULL,                  // reserved
							  lpClass,               // class string buffer
							  lpcClass,              // size of class string buffer
							  lpftLastWriteTime      // last write time
							);
	if (res == ERROR_NO_MORE_ITEMS) return false; // более нет ключей
	if (res != ERROR_SUCCESS) throw SExcept(res);
	return true; // ключи перечисленны не до конца
}
bool CWorkAsRegister::
EnumValue(DWORD dwIndex, LPTSTR lpValueName, LPDWORD lpcValueName, LPDWORD lpType, 
			   LPBYTE lpData, LPDWORD lpcbData)
{
	long res = RegEnumValue(
							  hKey,             // handle to key to query
							  dwIndex,          // index of value to query
							  lpValueName,      // value buffer
							  lpcValueName,     // size of value buffer
							  NULL,             // reserved
							  lpType,           // type buffer
							  lpData,           // data buffer
							  lpcbData          // size of data buffer
							);
	if (res == ERROR_NO_MORE_ITEMS) return false; // более нет ключей
	if (res != ERROR_SUCCESS) throw SExcept(res);
	return true; // ключи перечисленны не до конца

}
void CWorkAsRegister::
InfoKey(LPDWORD lpcSubKeys, LPDWORD lpcMaxSubKeyLen, LPDWORD lpcValues, LPDWORD lpcMaxValueNameLen, 
		     LPDWORD lpcMaxValueLen, PFILETIME lpftLastWriteTime)
{
	long res = RegQueryInfoKey(
								hKey,                   // handle to key
								NULL,                   // class buffer
								NULL,                   // size of class buffer
								NULL,                   // reserved
								lpcSubKeys,             // number of subkeys
								lpcMaxSubKeyLen,        // longest subkey name
								NULL,                   // longest class string
								lpcValues,              // number of value entries
								lpcMaxValueNameLen,     // longest value name
								lpcMaxValueLen,         // longest value data
								NULL,                   // descriptor length
								lpftLastWriteTime       // last write time
							  );
	if (res != ERROR_SUCCESS) throw SExcept(res);
}

CWorkAsRegister::operator HKEY() { return hKey; }

void CWorkAsRegister::ConnectRegistry(HKEY key, LPCTSTR lpMachineName)
{
	if (hKey) RegCloseKey(hKey);
	long res = RegConnectRegistry(
									  lpMachineName, // computer name
									  key,             // predefined registry handle
									  &hKey        // buffer for remote key handle
								  );
	if (res != ERROR_SUCCESS) throw SExcept(res);
}
