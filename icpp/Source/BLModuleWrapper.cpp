// BLModuleWrapper.cpp: implementation of the CBLModuleWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "BLModuleWrapper.h"
#include "ExecuteModule.h"
#include "RuntimeException.h"
#include "MetaDataOfClasses.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

class CModuleWithDebugSupport  : public CBLModuleWrapper
{
protected:
	CModuleWithDebugSupport(BOOL bEnableDebug, CBLContext* pContext);
	CModuleWithDebugSupport(BOOL bEnableDebug, CBLContext* pContext, LPCSTR szModuleString);

protected:
	virtual int				OnStartExecution(void);	
	virtual int				OnNextLine(void);	
	virtual void			OnEnterProc(int);	
	virtual void			OnExitProc(int);	
	virtual void			OnStopExecution(void);	

	void SetModulePathForRightUseIntoDebugger( const CString& strNewModulePath );
	void EnableUserModulesNativeDebugging();

	static BOOL IsDbgExprEvaluating() { return CBLModule7::m_bDbgExprEvaluating; };

	BOOL m_bModuleContValid;
};

class CBaseModuleWithErrorHandlers : public CModuleWithDebugSupport
{
protected:
	CBaseModuleWithErrorHandlers(BOOL bEnableDebug, CBLContext* pContext);
	CBaseModuleWithErrorHandlers(BOOL bEnableDebug, CBLContext* pContext, LPCSTR szModuleString);

protected:
	virtual int				OnSyntaxError(void);	
	virtual int				OnRuntimeError(void);	
	virtual void			OnErrorMessage(char const *) = 0;	

	virtual CString GetRuntimeErrorString(const CString& strRuntimeErrSourceLine, const CString& strGetRuntimeErrDescr) const = 0;
};

class CSimpleModule : public CBaseModuleWithErrorHandlers // CModuleWithDebugSupport
{
public:	
	CSimpleModule(CBLContext* pContext);

protected:
	virtual void			OnErrorMessage(char const *);	

	virtual CString			GetRuntimeErrorString(const CString& strRuntimeErrSourceLine, const CString& strGetRuntimeErrDescr) const;
};

class CBLModuleWrapperImpl  : public CBaseModuleWithErrorHandlers // CModuleWithDebugSupport
{
public:	
	CBLModuleWrapperImpl(const CString& strNameOfComponent, CBLContext* pContext, CBLModuleWrapper* pMod);
	CBLModuleWrapperImpl(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString);

protected:
	virtual void			OnErrorMessage(char const *);	

	virtual CString			GetRuntimeErrorString(const CString& strRuntimeErrSourceLine, const CString& strGetRuntimeErrDescr) const;

private:
	//CString GetModuleDescription();

	const CString m_strNameOfComponent;
	CString m_csFileName;
};

CBLContext* CBLModuleWrapper::GetContextFromModule(CBLModule *pMod)
{
	CBLContext *pCont  = NULL;        
	if(pMod)
	{
		pCont = pMod->GetInternalData()->pGeneralContext;
	}
	return pCont;
}

CBLModuleWrapper::CBLModuleWrapper(class CBLContext* pContext) : CBLModule7(pContext, NULL)
{}

CBLModuleWrapper::CBLModuleWrapper(CBLContext* pContext, LPCSTR szModuleString) : CBLModule7(pContext, szModuleString)
{}

CBLModuleWrapper::~CBLModuleWrapper()
{}

CBLModuleWrapper* CBLModuleWrapper::CreateSimpleModule()
{ return new CSimpleModule(NULL); }

CBLModuleWrapper* CBLModuleWrapper::CreateClassModule(const CString& strNameOfComponent, CBLContext* pContext, CBLModuleWrapper* pMod)
{	return new CBLModuleWrapperImpl(strNameOfComponent, pContext, pMod); }

CBLModuleWrapper* CBLModuleWrapper::CreateClassModule(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString)
{	return new CBLModuleWrapperImpl(strNameOfComponent, pContext, ModuleString); }

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLModuleWrapperImpl::CBLModuleWrapperImpl(const CString& strNameOfComponent, class CBLContext* pContext, CBLModuleWrapper* pMod)
: CBaseModuleWithErrorHandlers(FALSE, pContext), m_strNameOfComponent(strNameOfComponent)
{
	ASSERT(pMod != NULL);
	assert(pMod != NULL);
	
	//if (pMod)
	//{
		CBLModuleWrapperImpl* pModImpl = safe_dynamic_cast<CBLModuleWrapperImpl*>(pMod);

		m_csFileName = pModImpl->m_csFileName;
		
		m_nID = pMod->m_nID;
		SetModulePathForRightUseIntoDebugger(pMod->m_strModulePath);

		m_bModuleContValid = pModImpl->m_bModuleContValid;
		
		m_nCounter = 0;
		m_bIsProcessBroken = FALSE;
		
		AssignCompiled(*pMod);
	//}
	//else
	//{
	//	m_csFileName = "";
	//	
	//	m_nID = -1;
	//	m_strModulePath = "";
	//	m_bModuleContValid = FALSE;
	//	
	//	m_nCounter = 0;
	//	m_bIsProcessBroken = FALSE;
	//}
}

CBLModuleWrapperImpl::CBLModuleWrapperImpl(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString)
: CBaseModuleWithErrorHandlers(ModuleString.GetModuleContValid(), pContext, ModuleString.operator LPCTSTR()), m_strNameOfComponent(strNameOfComponent)
{
	m_csFileName = ModuleString.GetFileName();
	
	m_nID = ModuleString.GetModuleID();
	SetModulePathForRightUseIntoDebugger(ModuleString.GetModulePath());

	m_nCounter = 0;
	m_bIsProcessBroken = FALSE;
}

//CString CBLModuleWrapperImpl::GetModuleDescription()
//{
//	CString csDescr = "'";
//	csDescr += m_csFileName.IsEmpty() ? "unknown" : m_csFileName;
//	csDescr += '\'';
//	
//	if (m_nID != -1)
//	{
//		csDescr += " (";
//		csDescr += GetFullName();
//		csDescr += ')';
//	}
//
//	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy(); 
//	CComponentClass* pComp = CComponentClass::GetComponentClassCurExModule(this);
//	if (pComp)
//	{
//		csDescr += " (класс ";
//		csDescr += pComp->GetTypeString();
//		csDescr += ')';
//	}
//
//	return csDescr;
//}

void CBLModuleWrapperImpl::OnErrorMessage(char const *pStr)
{    
  CString strSyntaxErrDescr("",1000);
  CBLModule::GetSyntaxErrDescr(GetSyntaxErrCode(),strSyntaxErrDescr);
  
  char buffer[20];
  _itoa(GetSyntaxErrLineNum(), buffer, 10 );
  
  CString strSyntaxErrMarkedLine = '(';
  strSyntaxErrMarkedLine += buffer;
  strSyntaxErrMarkedLine += ')';
  strSyntaxErrMarkedLine += GetSyntaxErrMarkedLine();
  strSyntaxErrMarkedLine += '\t';
  strSyntaxErrMarkedLine += strSyntaxErrDescr;

 	CString strError;
	{
	if (!m_strNameOfComponent.IsEmpty())
	  {

		CString strNameOfComp = m_strNameOfComponent;        
		CString strOldGetRuntimeErrDescr = strSyntaxErrDescr;

		strError += "Класс: ";
		strError += strNameOfComp;
		strError += " - ошибка синтаксиса";

		strError += "\r\n";
		strError += "Строка №";  
		strError += pStr;  

		strError += "\r\n{";     
		CString strNameOfFile = ISetOfHierarchy::GetMetaDataOfClasses().GetNameOfFile(strNameOfComp);
		ISetOfHierarchy::MakeFullFileName(&strNameOfFile);
		strError += strNameOfFile;
		
		char buffer[20];
		_itoa( GetSyntaxErrLineNum(), buffer, 10 );
		strError += '(';
		strError += buffer;
		strError += ')';
		strError += " }";     
	  }
	}
	// при использовании следующего кода можно будет открывать те классы в конфигураторе,
	// в которых возникла ошибка в 1С
	// более подробное описание смотреть в методе OnRuntimeError
  
    /*CString strModulePath_temp = m_strModulePath;
    strModulePath_temp.MakeLower();

	if (strModulePath_temp.Find(".ert") != -1) // это чтобы попытка открыть текстовый файл не пугала пользователя сообщением в конфигураторе
		CBLModule7::OnErrorMessageEx(pStr, m_nID, m_strModulePath,GetSyntaxErrLineNum());
	else
		CBLModule7::OnErrorMessage(pStr);
	*/

	if (strError.IsEmpty())
		CBLModule7::OnErrorMessageEx(pStr, m_nID, m_strModulePath,GetSyntaxErrLineNum());
	else
		CBLModule7::OnErrorMessageEx(strError, m_nID, m_strModulePath,GetSyntaxErrLineNum());

}

CString CBLModuleWrapperImpl::GetRuntimeErrorString(const CString& strRuntimeErrSourceLine, const CString& strGetRuntimeErrDescr) const
{
	ISetOfHierarchy* pHierarchy = ISetOfHierarchy::GetHierarchy(); 

	CComponentClass* pContInter = CComponentClass::GetComponentClassCurExModule(const_cast<CBLModuleWrapperImpl*>(this));

	assert(pContInter != NULL);

	CString strNameOfComp = pContInter->GetTypeString();        
	CString strOldGetRuntimeErrDescr = strGetRuntimeErrDescr;

	CString strError (strNameOfComp);
	strError += "::";
	strError += CExecuteModule::GetExecuteMethInfo(const_cast<CBLModuleWrapperImpl*>(this));
	strError += " : "+strOldGetRuntimeErrDescr;

	strError += "\r\n";
	strError += strRuntimeErrSourceLine;  

	strError += "\r\n{";     
	CString strNameOfFile = ISetOfHierarchy::GetMetaDataOfClasses().GetNameOfFile(strNameOfComp);
	ISetOfHierarchy::MakeFullFileName(&strNameOfFile);

	strError += strNameOfFile;
	char buffer[20];
	_itoa( GetRuntimeErrLineNum(), buffer, 10 );
	strError += '(';
	strError += buffer;
	strError += ')';
	strError += " }";     

	CString strResult = strError;    
	strResult += "\r\n";

	return strResult;
}

CModuleWithDebugSupport::CModuleWithDebugSupport(BOOL bEnableDebug, CBLContext* pContext) : 
	CBLModuleWrapper(pContext), m_bModuleContValid(bEnableDebug)
{
	EnableUserModulesNativeDebugging();
}

CModuleWithDebugSupport::CModuleWithDebugSupport(BOOL bEnableDebug, CBLContext* pContext, LPCSTR szModuleString) :
	CBLModuleWrapper(pContext, szModuleString), m_bModuleContValid(bEnableDebug)
{
	EnableUserModulesNativeDebugging();
}

int CModuleWithDebugSupport::OnStartExecution(void)
{
	if (m_bModuleContValid)
		return CBLModule7::OnStartExecution();

	return 1;
}

int CModuleWithDebugSupport::OnNextLine(void)	
{
	if (m_bModuleContValid)
		return CBLModule7::OnNextLine();
	
	return 1;
}

void CModuleWithDebugSupport::OnEnterProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnEnterProc(n);  
}

void CModuleWithDebugSupport::OnExitProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnExitProc(n);
}

void CModuleWithDebugSupport::OnStopExecution(void)
{
	if (m_bModuleContValid)
		CBLModule7::OnStopExecution();
}

void CModuleWithDebugSupport::SetModulePathForRightUseIntoDebugger( const CString& strNewModulePath )
{
	m_strModulePath = strNewModulePath;
	m_strModulePath.MakeUpper(); // иначе в отладчике не всегда будет ловиться точка отладки
}

void CModuleWithDebugSupport::EnableUserModulesNativeDebugging()
{
	// переклад проблем на custom tracer.dll
	//if(pMainAddIn->m_EnableDebugging)  // у kms в отладке этого нет !!
	//	m_bModuleContValid = TRUE; // TODO artbear из-за этой строки баг http://www.1cpp.ru/forum/YaBB.pl?num=1206042291/85#85
}

CBaseModuleWithErrorHandlers::CBaseModuleWithErrorHandlers(BOOL bEnableDebug, CBLContext* pContext) : 
CModuleWithDebugSupport(bEnableDebug, pContext)
{
}

CBaseModuleWithErrorHandlers::CBaseModuleWithErrorHandlers(BOOL bEnableDebug, CBLContext* pContext, LPCSTR szModuleString) :
CModuleWithDebugSupport(bEnableDebug, pContext, szModuleString)
{
}

int CBaseModuleWithErrorHandlers::OnSyntaxError(void)	
{  
	//if (!CModuleWithDebugSupport::IsDbgExprEvaluating()) // if (!CBLModule7::m_bDbgExprEvaluating)
	//	LogErr("=== Syntax error в модуле %s %i", GetModuleDescription().operator LPCTSTR());

	int ret = 0;

	// иначе при использовании EvalDebugExpr выдается синтакс-ошибка
	//(например, в ФормЕкс - для получения значения переменной, Чтобы можно было не запариваться с ее определением. И неважно было где она определена.) 
	if (!CModuleWithDebugSupport::IsDbgExprEvaluating()) // if (!CBLModule7::m_bDbgExprEvaluating) // artbear по рекомендации АльФ-а 
		ret = CBLModule::OnSyntaxError(); 
	else
		ret = CBLModule7::OnSyntaxError(); // artbear TODO kms зачем-то поставил такой вариант

	return ret;
}

int CBaseModuleWithErrorHandlers::OnRuntimeError(void)	
{ 
	// kms почему-то использовал такой код
	// 	if (!*m_pDbgExprEvaluating)
	// 	{
	// 		LogErr("=== Runtime error в модуле %s", GetModuleDescription().operator LPCTSTR());
	// 		
	// 		//	CString strRuntimeErrSourceLine;
	// 		//	GetRuntimeErrSourceLine(strRuntimeErrSourceLine);
	// 		
	// 		GetRuntimeErrDescr(GetRuntimeErrCode(), CComponentClass::strTextException);
	// 		CComponentClass::strTextException.TrimLeft();
	// 		CComponentClass::m_bExceptionStatus = TRUE;
	// 		
	// 	}
	// 	return CBLModule7::OnRuntimeError();

	CRuntimeExceptionClass::Empty();

	CString strRuntimeErrSourceLine;
	CString strGetRuntimeErrDescr;

	GetRuntimeErrSourceLine(strRuntimeErrSourceLine);
	strRuntimeErrSourceLine.TrimLeft();    

	GetRuntimeErrDescr(GetRuntimeErrCode(),strGetRuntimeErrDescr);

	CString strRuntimeErrorInfo = GetRuntimeErrorString(strRuntimeErrSourceLine, strGetRuntimeErrDescr);

	// artbear совместно с АльФ-ом
	// при использовании следующего кода можно будет открывать те классы в конфигураторе,
	// в которых возникла ошибка в 1С
	//
	// для текстовых файлов в этом случае в Конфигураторе может выдаваться ошибка "Общая файловая ошибка при доступе к "+ПутьКФайлу
	// есть несколько путей для исправления
	// 1. простой путь для исключения ошибки - возможно использовать скрипт Опенконф с  текстом
	///* рекомендую его (artbear)
	//Dim bFlagInsideOnMsgBox
	//bFlagInsideOnMsgBox = false
	//Sub Configurator_OnMsgBox(text, style, def, Answer)
	//if InStr(text, "Общая файловая ошибка при доступе к") > 0 then
	//' открываю класс 1С++
	//if 	bFlagInsideOnMsgBox = false then
	//strFileName = Replace(text, "Общая файловая ошибка при доступе к ", "")
	//Set FSO = CreateObject("Scripting.FileSystemObject")
	//if FSO.FileExists(strFileName) then
	//bFlagInsideOnMsgBox = true
	//Documents.Open ( strFileName )
	//bFlagInsideOnMsgBox = false
	//Answer = mbaOK
	//Exit Sub
	//end if
	//end if
	//end if
	//bFlagInsideOnMsgBox = false
	//End Sub

	//2. запретить подобное открытие ошибок в текстовых файлах - лично я против запрета (artbear)
	//3. сделать данный вариант опциональным
	//4. досконально разобраться, каким образом выполняется открытие файлов в Конфигураторе 
	//при двойном щелчке в окне сообщений 1С:Предприятия - пока спецов/или желающих нет :(

	// для каждого класса согласно иерархии создания будет показано отдельное сообщение об ошибке
	// т.е. если открыты форма Форма1, в ней создан класс КОП1, в нем КОП2, будет показано 3 сообщения
	// сначала для КОП2, далее КОП1, далее Форма1 - и двойной щелчок на каждом из сообщений при открытом Конфигураторе вызовет переход на соответствующий файл в Конфигураторе
	//

	//CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());

	CRuntimeExceptionClass::SetException(strRuntimeErrorInfo, this, m_nID, m_strModulePath, GetRuntimeErrLineNum());

	return GetRuntimeErrLineNum();
}

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSimpleModule::CSimpleModule(class CBLContext* pContext)
: CBaseModuleWithErrorHandlers(FALSE, pContext) //, m_bEnableThrow(false)
{
	m_nID = -1;
	m_strModulePath = "";

	m_nCounter = 0;
	m_bIsProcessBroken = FALSE;
}

void CSimpleModule::OnErrorMessage(char const *pStr)
{    
	CString strSyntaxErrDescr("",1000);
	CBLModule::GetSyntaxErrDescr(GetSyntaxErrCode(),strSyntaxErrDescr);

	char buffer[20];
	_itoa(GetSyntaxErrLineNum(), buffer, 10 );

	CString strSyntaxErrMarkedLine = '(';
	strSyntaxErrMarkedLine += buffer;
	strSyntaxErrMarkedLine += ')';
	strSyntaxErrMarkedLine += GetSyntaxErrMarkedLine();
	strSyntaxErrMarkedLine += '\t';
	strSyntaxErrMarkedLine += strSyntaxErrDescr;

	// ВНИМАНИЕ, ошибки на экран выдаем только в особых случаях
	CRuntimeExceptionClass::SetException(strSyntaxErrMarkedLine, this, m_nID, m_strModulePath, GetSyntaxErrLineNum());
	return;

	//CBLModule7::OnErrorMessageEx(pStr, m_nID, m_strModulePath,GetSyntaxErrLineNum());
}

CString CSimpleModule::GetRuntimeErrorString(const CString& strRuntimeErrSourceLine, const CString& strGetRuntimeErrDescr) const
{
	CString strGetRuntimeErrDescr1;
	GetRuntimeErrDescr(GetRuntimeErrCode(),strGetRuntimeErrDescr1);
	CString strRuntimeErrorInfo = "Ошибка выполнения: ";
	strRuntimeErrorInfo += strGetRuntimeErrDescr1 +"\r\n"+
		'<'+strRuntimeErrSourceLine+'>';	

	return strRuntimeErrorInfo;
}