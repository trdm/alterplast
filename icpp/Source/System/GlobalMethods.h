#ifndef GLOBALMETHODS_H
#define GLOBALMETHODS_H

//You may derive a class from CComModule and use it if you want to override
//something, but do not change the name of _Module
extern CComModule _Module;

#include <atlcom.h>
// #include <atlctl.h>
// #include <comdef.h>

//#include <Rpcdce.h>
#include "blptr.h"

//#pragma warning(pop)
//#pragma warning(push, 4)

void RegisterCBLContext(CRuntimeClass* pRuntimeClass, LPCSTR szClassName, BOOL bNativeRegistration = TRUE);

// макрос для проверки типа класса, возвращает тип bool
#define IS_KINDOF_RUNTIME_CLASS(object, class_name) \
	((((void*)(object)) != NULL) && (object)->IsKindOf(RUNTIME_CLASS(class_name)))

// указатель на групповой контекст ?
template<class Type>
bool IS_KINDOF_CGroupContext(Type* pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont, CGroupContext);
};
template<class Type>
bool IS_KINDOF_CGroupContext(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont.operator Type*(), CGroupContext);
};

// указатель на класс КОП ?
template<class Type>
bool IS_KINDOF_CComponentClass(Type* pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont, CComponentClass);
};

template<class Type>
bool IS_KINDOF_CComponentClass(const CBLPtr<Type>& pCont)
{
	return IS_KINDOF_RUNTIME_CLASS(pCont.operator Type*(), CComponentClass);
};

typedef CArray<CValue*, CValue*> PValueItemArrayT;
//typedef CArray<CValueItem *,CValueItem *> PValueItemArrayT;
typedef CValue* PValueT;

void InitAllBoost();

//Функции для выдачи сообщений в окно сообщений 1С
void ShowMsg(const char* format,MessageMarker marker=mmNone,...);
void Msg(const char *msg, ...);
void LogErr(const char* format,...);
//Функция сообщает об ошибке, и информирует 1С что произошло исключение
void RuntimeError(const char *msg, ...);
void RuntimeErrorRes(UINT nID);
void AssertHResult(HRESULT hresult);

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

extern class CDataBase7* pDataBase7;
extern class CMetaDataCont* pMetaDataCont;
extern class CBkEndUI* pBkEndUI;
extern class boost::shared_ptr<class CAddIn> pMainAddIn;

// имя файла определений классов
extern const char* const pcstrNameOfDef;
extern const char* const TypeNamePrefix;
extern const char* const szDotSubst;

void GetRTCFull(CRuntimeClass* rtc, CString& Str);
void Trace0(const char* str);
void MessageFullRTC(CObject* obj);

// CValueListContext accessor
class CValueListContextFriend : public CValueListContext
{
public:
	typedef CValueListContext BASE;
	void CValueListContextFriend::P_RemoveAll(CValue** ppVal) {	BASE::P_RemoveAll(ppVal); }
	void CValueListContextFriend::P_Add(CValue** ppVal)	{ BASE::P_Add(ppVal); }
	int CValueListContextFriend::GetSize() { return BASE::GetPValueList()->GetSize(); }
	int CValueListContextFriend::IsEmpty() { return !GetSize();	}
	CValue F_FromDelimStr(CValue * * ppValues)	{ return BASE::F_FromDelimStr(ppValues); }
	CValue F_ToDelimStr(CValue * *ppValues)		{ return BASE::F_ToDelimStr(ppValues); }
};

//Функции для преобразования CValue в ТаблицаЗначений, СписокЗначений и ИндексированнаяТаблица.
//Если в CValue содержится не тот тип, то возвращают NULL
CValueTable* CValue2VT(CValue const& Value); //ТаблицаЗначений
CValueListContextFriend* CValue2VLC(CValue const& Value); //СписокЗначений
CValueItemList* CValue2VL(CValue const& Value); //СписокЗначений
class CVTExtended* CValue2VTExt(CValue const& Value); //ИндексированнаяТаблица

//функции для упрощения вызова функции/процедуры объекта
bool CallAsFunc(CBLContext* pContext, char const* sFuncName, CValue& RetVal, CValue** params, bool bEnableException = true);
bool CallAsProc(CBLContext* pContext, char const* sProcName, CValue** ppParams, bool bEnableException = true);
bool GetPropVal(CBLContext* pContext, char const* sPropName, CValue& RetVal, bool bEnableException = true);

// класс, упрощающий преобразование из char* или CString в WCHAR*
class CWChar
{
private:
    WCHAR* wStr;
public:
	CWChar(CString const& str)
	{
		size_t nLen = str.GetLength() + 1;
	    wStr = new wchar_t[nLen];
		std::swprintf(wStr, L"%S", str.operator LPCTSTR());
	};

	CWChar(const char* str)
	{
		size_t nLen = strlen(str) + 1;
	    wStr = new wchar_t[nLen];
		std::swprintf(wStr, L"%S", str);
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
LPCTSTR CallMethodOfGlobalContexts0(const CString& MethodName);
CValue CallMethodOfGlobalContexts1(const CString& MethodName, CValue& param1);

// найти метод одного из глобальных контекстов
// например, "IBDir"
// возвращает номер найденного метода
//	или -1, если метод не найден
//
// в параметр pBLContext возвращется указатель на найденный глобальный контекст
//
int FindMethodOfGlobalContexts(const CString& MethodName, CBLContext*& pBLContext);

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

// Windows 2000/ХП или позже ?
bool IsSystem_Windows2000AndHigher();

void ReadFileToStringWithExceptions(const CString& strFileName, CString& strDest);
bool ReadFileToStringWithoutExceptions(const CString& strFileName, CString& strDest);

void DumpContext(CBLContext* pCont);
void CallIsOleContext(CBLContext* pCont, int nParams);

// если ставить это объявление в конце или середине файла, то будет баг, например, с CFlagGuard :(
#include "Preprocessor.h" 

template <typename T>
struct null_deleter
{
	void operator()(T *) const
	{
	}
};

template <typename T>
struct CBLContext_deleter
{
	void operator()(T * p) const
	{
		boost::function_requires< boost::ConvertibleConcept<T*, CBLContext*> >();
		if (p)
			p->DecrRef();
	}
};

class CBLMyEx:public CBLExportContext
{
public:
	static long DispParamToValueEx(CValue** ppVal, DISPPARAMS* pDisp,unsigned int* pUI)
	{
		return static_cast<CBLMyEx*>(NULL)->DispParamToValue(ppVal, pDisp,pUI);
	}
	int  TryReturnByRefEx(CValue ** ppVal,DISPPARAMS* pDisp)
	{
		return TryReturnByRef(ppVal,pDisp);
	}
	static int  ValueToVariantEx(CValue& Val,tagVARIANT * pVariant){
		return static_cast<CBLMyEx*>(NULL)->ValueToVariant(Val,pVariant);
	}
};

void DispatchToValue(IUnknown* pUnk, CValue* pVal);

CString GetModuleVersionInfo(HMODULE hModule, CString VerInfoName);
CString GetModuleVersionInfo(CString ModuleName, CString VerInfoName);
void TraceMenu(CMenu* Menu, CString Indent="");

#endif