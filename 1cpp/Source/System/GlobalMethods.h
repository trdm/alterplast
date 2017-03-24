#ifndef GLOBALMETHODS_H
#define GLOBALMETHODS_H

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#include <atlcom.h>
// #include <atlctl.h>
// #include <comdef.h>

// #include "Preprocessor.h"

//#include <Rpcdce.h>
#include "blptr.h"

//#pragma warning(pop)
//#pragma warning(push, 4)

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration = TRUE);

// макрос для проверки типа класса, возвращает тип bool
//(object)->IsKindOf(RUNTIME_CLASS(class_name))
#define IS_KINDOF_RUNTIME_CLASS(object, class_name) \
	((((void*)(object)) != NULL) && (object)->IsKindOf(RUNTIME_CLASS(class_name)))

// указатель на групповой контекст ?
// #define IS_KINDOF_CGroupContext(pCont) (IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext))
template<class Type>
bool IS_KINDOF_CGroupContext(Type* pCont)
{
	//return IS_KINDOF_RUNTIME_CLASS((CBLContext*)pCont, CGroupContext);
	return IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext);
};
template<class Type>
bool IS_KINDOF_CGroupContext(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS((Type*)pCont, CGroupContext);
};

// указатель на класс КОП ?
// #define IS_KINDOF_CComponentClass(pCont) (IS_KINDOF_RUNTIME_CLASS(pCont, CComponentClass))
template<class Type>
bool IS_KINDOF_CComponentClass(Type* pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont, CComponentClass);
};

template<class Type>
bool IS_KINDOF_CComponentClass(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS((Type*)pCont, CComponentClass);
};

// имя файла определений классов
static const char * pcstrNameOfDef = "defcls.prm";

// #include <atlcom.h>
// // #include <atlctl.h>
// // #include <comdef.h>
//
#include "Preprocessor.h"
//
// //#include <Rpcdce.h>

// в 2.0.3 нужна след.строка
//#define VK_FormEx_Hooks

// в 2.5 нужна след.строка
#undef VK_FormEx_Hooks

typedef CArray<void *,void *> PValueItemArrayT;
//typedef CArray<class CValueItem *,class CValueItem *> PValueItemArrayT;
typedef CValue* PValueT;

/*typedef struct tagValueItemArray
{
	CValue	Value;
	struct tagValueItemArray**	NextLevel;
	DWORD	Index;
	BYTE	Type;
	int		Count;
	CString* Descr;
	BOOL	Checked;
	CValueItem* ValueItem;
	BOOL Selected;
} structVIA, *PstructVIA;

typedef union tagMessageMapFunctions
{
	AFX_PMSG pfn;

	void (CBrowse::*pfn_my)(void);
	void (CChildFrame7::*pfn_mdi)(BOOL,CWnd *,CWnd *);
} FormEx_MessageMapFunctions;

struct CProcInfoFormEx
{
	int num;
	int type;
	int numarg;
};

struct CModuleInfoFormEx
{
	CMapStringToPtr cProcMap;
};


LRESULT __stdcall Mouse_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Key_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall Wnd_Hook(int code, WPARAM wParam, LPARAM lParam);
LRESULT __stdcall WndPre_Hook(int code, WPARAM wParam, LPARAM lParam);
void CALLBACK TimerProc(HWND hWnd, UINT nMsg, UINT nIDEvent, DWORD dwTime);
LRESULT FormEx_OnSetCursor(WORD nHittest);
*/
void InitAllBoost();

//Функции для выдачи сообщений в окно сообщений 1С
void ShowMsg(const char* format,MessageMarker marker=mmNone,...);
void Msg(const char *msg, ...);
void LogErr(const char* format,...);
//Функция сообщает об ошибке, и информирует 1С что произошло исключение
void RuntimeError(const char *msg, ...);
void RuntimeErrorRes(UINT nID);

// функция выдает окно с информацией
void MessageBox(const char* format, ...);

CBLModule* GetModuleByContext(const CGroupContext* pCont);
/*BOOL _CallFromGlobalModules(CGroupContext *pCont, CBLModule *pMod, const char* nameProc0, const char* nameProc1, int argNum, CValue** vArray, bool bCallSysProc = false);
void _CallModuleProc(class CGetDoc7* pDoc, const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray, BOOL bCallGlobal = TRUE);
void CallModuleProc(const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray);
CString PrintRTC(CObject* pObj);
void CopyMenu(CMenu* pMenuSrc, CMenu* pMenuDes, bool f = false);*/
CString strLastError(DWORD dwMessageID = 0);

#define DATA_SOURCE_TYPE_DBF    1
#define DATA_SOURCE_TYPE_SQL    2

const char TypeNamePrefix[] = "_1c_type_";

void GetRTCFull(CRuntimeClass* rtc, CString& Str);
void Trace0(const char* str);
void MessageFullRTC(CObject* obj);

//Функции для преобразования CValue в ТаблицаЗначений, СписокЗначений и ИндексированнаяТаблица.
//Если в CValue содержится не тот тип, то возвращают NULL
CValueTable* CValue2VT(CValue const& Value);    //ТаблицаЗначений
CPtrArray* CValue2VL(CValue const& Value);      //СписокЗначений
class CVTExtended* CValue2VTExt(CValue const& Value); //ИндексированнаяТаблица

//функции для упрощения вызова функции/процедуры объекта
bool CallAsFunc(CBLContext* pContext, char const* sFuncName, CValue& RetVal, CValue** params);
bool CallAsProc(CBLContext* pContext, char const* sProcName, CValue** ppParams);
bool GetPropVal(CBLContext* pContext, char const* sPropName, CValue& RetVal);

// класс, упрощающий преобразование из char* или CString в WCHAR*
class CWChar
{
private:
    WCHAR* wStr;
public:
	CWChar(CString str)
	{
	    wStr = new wchar_t[str.GetLength()+1];
	    swprintf(wStr, L"%S", str);
	};

	CWChar(const char* str)
	{
	    wStr = new wchar_t[strlen(str)+1];
	    swprintf(wStr, L"%S", str);
	};
	operator LPCOLESTR() const
	{ return wStr; }

	~CWChar(void)
	{
		if(wStr) delete[] wStr;
	};
};

// Переназначить/снять переназначение ветки HKEY_CLASSES_ROOT в ветку HKEY_CURRENT_USER
void RedirectHKCR(BOOL bRedirect);

// проверка на существование файла или папки
bool FileExists(CString fileName);
bool FolderExists(CString folderName);
bool DirectoryExists(CString directoryName);

// получить имя папки из полного пути к файлу
// существование файла не проверяется
// если не удалось получить имя папки, возвращается ""
//
// ВАЖНО: папка всегда возвращается со слешем в конце!!
//
//	из T:\Папка Файла\Имя Файла.txt получу T:\Папка Файла\
//
CString ExtractFileDir(CString FileName);

// получить абсолютный путь к файлу\папке
// учитывается наличие "." или ".."
//
CString AbsoluteFilePath(CString sRelativePath);

// получить версию
CString GetVersionField(LPCSTR szField); // для ВК
CString GetVersionField(LPCTSTR filePath, LPCTSTR szField); // для произвольного файла

// Возвращает строку, полную версию 1С++, как она показана  на
// закладке "О программе..."
// Например, "2.0.2.0 Nightly build 2006-05-15" или "2.0.3.0"
//
CString GetFullVersion();

// вызвать метод одного из глобальных контекстов
// например, "IBDir"
// в случае ошибки выдает исключение throw
//
// TODO добавить метод для работы с произвольным количество аргументов !!
//
LPCTSTR CallMethodOfGlobalContexts0(CString MethodName);
CValue CallMethodOfGlobalContexts1(CString MethodName, CValue& param1);

// найти метод одного из глобальных контекстов
// например, "IBDir"
// возвращает номер найденного метода
//	или -1, если метод не найден
//
// в параметр pBLContext возвращется указатель на найденный глобальный контекст
//
int FindMethodOfGlobalContexts(CString MethodName, CBLContext*& pBLContext);

// удобно использовать для перехвата всех исключений в методах,
// которые напрямую используются в коде 1С
// исключения 1С пропускаются !!
//
// Рекомендуется к использованию в методах CallAsFunc и CallAsProc
// Пример использования:
/*
	int CMyContextBase::CallAsFunc(int iMethNum,CValue& rValue,CValue **ppValue)
	{
		TRY{
			// код метода
		}
		CATCH_ALL_AND_DO_RUNTIME_ERROR
	}
*/
//
#define CATCH_ALL_AND_DO_RUNTIME_ERROR \
	CATCH(CFileException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка работы с файлами"); \
		 return TRUE; \
	} \
	AND_CATCH(CMemoryException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка работы с памятью"); \
		 return TRUE; \
	} \
	AND_CATCH(CNotSupportedException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"операция не поддерживается"); \
		 return TRUE; \
	} \
	AND_CATCH(CResourceException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка работы с ресурсами"); \
		 return TRUE; \
	} \
	AND_CATCH(COleException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка COleException"); \
		 return TRUE; \
	} \
	AND_CATCH(COleDispatchException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка COleDispatchException"); \
		 return TRUE; \
	} \
	AND_CATCH(CUserException, pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"ошибка CUserException"); \
		 return TRUE; \
	} \
	AND_CATCH_ALL(pEx) \
	{ \
		RuntimeError("Метод <%s::%s>: исключение - %s", GetTypeString(), GetMethodName(iMethNum, 1),  \
			"неизвестная ошибка"); \
		 return TRUE; \
	} \
	} }  \
	catch(...) \
	{ \
		CBLModule* pCurMod = CBLModule::GetExecutedModule(); \
		int iRuntimeErrCode = pCurMod->GetRuntimeErrCode(); \
		"комментарий - это исключения порождается вызовом CBLModule::RaiseExtRuntimeError внутри кода ВК"; \
		if (iRuntimeErrCode) \
		{  \
			CString strGetRuntimeErrDescr;  \
			pCurMod->GetRuntimeErrDescr(iRuntimeErrCode,strGetRuntimeErrDescr);  \
			RuntimeError((LPCTSTR)strGetRuntimeErrDescr);  \
		} \
		else{ \
			RuntimeError("Метод <%s::%s>: неизвестное исключение, код ошибки <%d>", GetTypeString(), \
				GetMethodName(iMethNum, 1), iRuntimeErrCode);  \
		}; \
		return TRUE; \
	} \
	}

// получить контекст формы из контекста
CFormAllCtrlsContext* GetFormContext(const CBLContext* pCont);
CGetDoc7* GetDoc7(const CBLContext* pCont);

// получить модуль проведения документа из контекста документа
CDocActModule* GetDocumentTransactionModule(CDocContext* pDoc);

// получить контекст документа из формы документа
CDocContext* GetDocContextFromCGetDoc7(CGetDoc7* pDoc);

// получить контекст документа из группового контекста
CDocContext* GetDocContextFromGroupContext(CGroupContext* pGroupCont);

// проверить параметры CValue
void ValidateParamContext(const CValue& ContValue, const CString& strErrorMessage);
void ValidateParamContext(const CValue& ContValue, const CString& strContextType, const CString& strErrorMessage);
void ValidateParamGroupContext(const CValue& GroupContValue, const CString& strErrorMessage);
void ValidateParamNumeric(const CValue& NumericValue, const CString& strErrorMessage);
void ValidateParamString(const CValue& StringValue, const CString& strErrorMessage);
void ValidateParamNotEmptyString(const CValue& StringValue, const CString& strErrorMessage);
void ValidateParamValueListContext(const CValue& ValueListValue, const CString& strErrorMessage);
void ValidateParamUserClass(const CValue& UserClassContValue, const CString& strErrorMessage);

// класс для правильного возврата значения переменной, независимо от исключения
template <class T>
class CValueGuard
{
private:
	T& m_Value;
	T m_SavedValue;
public:
	CValueGuard(T& bSavedValue, const T& newValue) :
		m_Value(bSavedValue)
	{
		m_SavedValue = m_Value;
		m_Value = newValue;
	};
	~CValueGuard()
	{
		m_Value = m_SavedValue;
	}
};

// класс для правильного возврата значения булевой переменной, независимо от исключения
typedef CValueGuard<bool> CFlagGuard;

// это SQL-версия ?
bool IsSQLVersion(void);

// функции для удобного вызова функций модуля - решает иногда возникающий баг
// если передавать в следующий вызов CallAsFunc непосредственно retVal, может возникать ошибка вложенного вызова -
// юнит-тест "TurboBLТесты::ТестВызовВложенногоМетодаКонтекста"
inline int CallAsFunc(CBLModule* pMod, int iProcNum, CValue& retVal, int iParamsCount, CValue** ppParams)
{
	CValue locVal;
	int ret = pMod->CallAsFunc(iProcNum, locVal, iParamsCount, ppParams);
	retVal = locVal;
	return ret;
}

// AddParams_IntoValueArray
//
// к существуему массиву в конце добавить указанное количество параметров
//
void AddParams_IntoValueArray(CValuePtrArray& AddedParams, unsigned int nParamsCount, CValue** ParamsArray);

// Windows 2000/ХП или позже ?
bool IsSystem_Windows2000AndHigher();

void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest);
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest);

#endif