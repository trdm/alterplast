// BLModuleWrapper.cpp: implementation of the CBLModuleWrapper class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "BLModuleWrapper.h"
#include "ExecuteModule.h"
#include "ModuleString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CBLModuleWrapper::CBLModuleWrapper(const CString& strNameOfComponent, class CBLContext* pContext, CBLModuleWrapper* pMod)
: CBLModule7(pContext, NULL), m_strNameOfComponent(strNameOfComponent), m_bEnableThrow(false)
{
	ASSERT(pMod != NULL); // мечты (todo: refactoring выполняемый модуль)

	if (pMod)
	{
		m_csFileName = pMod->m_csFileName;

		m_nID = pMod->m_nID;
		SetModulePathForRightUseIntoDebugger(pMod->m_strModulePath);

		m_bModuleContValid = pMod->m_bModuleContValid;

		m_nCounter = 0;
		m_bIsProcessBroken = FALSE;

		AssignCompiled(*pMod);
	}
	else
	{
		m_csFileName = "";

		m_nID = -1;
		m_strModulePath = "";
		m_bModuleContValid = FALSE;

		m_nCounter = 0;
		m_bIsProcessBroken = FALSE;
	}

	// переклад проблем на custom tracer.dll
	if(pMainAddIn->m_EnableDebugging)
		m_bModuleContValid = TRUE;
}

CBLModuleWrapper::CBLModuleWrapper(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString, bool bEnableThrow)
: CBLModule7(pContext, (LPCSTR)ModuleString), m_strNameOfComponent(strNameOfComponent), m_bEnableThrow(bEnableThrow)
{
	m_csFileName = ModuleString.GetFileName();

	m_nID = ModuleString.GetModuleID();
	SetModulePathForRightUseIntoDebugger(ModuleString.GetModulePath());

	m_bModuleContValid = ModuleString.GetModuleContValid();

	m_nCounter = 0;
	m_bIsProcessBroken = FALSE;

	// переклад проблем на custom tracer.dll
	if(pMainAddIn->m_EnableDebugging)
		m_bModuleContValid = TRUE;
}

CBLModuleWrapper::~CBLModuleWrapper()
{
  //OnStopExecution();
}

CString CBLModuleWrapper::GetModuleDescription()
{
	CString csDescr = "'";
	csDescr += m_csFileName.IsEmpty() ? "unknown" : m_csFileName;
	csDescr += "'";

	if (m_nID != -1)
	{
		csDescr += " (";
		csDescr += GetFullName();
		csDescr += ")";
	}

	CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();
	CComponentClass* pComp = CComponentClass::GetComponentClassCurExModule(this);
	if (pComp)
	{
		csDescr += " (класс ";
		csDescr += pComp->GetTypeString();
		csDescr += ")";
	}

	return csDescr;
}

int CBLModuleWrapper::OnSyntaxError(void)
{
	if (!CBLModule7::m_bDbgExprEvaluating)
		LogErr("=== Syntax error в модуле %s %i", (LPCSTR)GetModuleDescription());

  //int ret = CBLModule::OnSyntaxError();
  int ret = CBLModule7::OnSyntaxError(); // artbear TODO kms зачем-то поставил такой вариант

  /* чтобы не дублировать путь к файлу в окне сообщений:
  // иначе при использовании EvalDebugExpr выдается синтакс-ошибка
  //(например, в ФормЕкс - для получения значения переменной, Чтобы можно было не запариваться с ее определением. И неважно было где она определена.)
  if (1 != CBLModule7::m_bDbgExprEvaluating) // artbear по рекомендации АльФ-а
	if (!m_strFileName.IsEmpty())
		pBkEndUI->DoMessageLine(m_strFileName, mmRedErr);
*/

  return ret;
}

//	artbear - при нативной отладке этот метод только мешает !
// // artbear по рекомендации АльФ-а - связано с изменением в OnSyntaxError
// // возможные проблемы с ТурбоБЛ метод CGCBoost::WrapCompile использует вызов BOOL bNormalModule = 7 == pMod->GetKind();
// // но вроде бы я все варианты протестировал :)
// //
// int CBLModuleWrapper::GetKind(void)const
// {
//  	// artbear - используется для возможности перехода от сообщений об ошибках
//  	//	из Предприятии в Конфигуратор в любой класс в стеке вызовов
//  	if(!pMainAddIn->m_EnableDebugging)
//  		return 8;
//
// 	// нужно для отладки текстовых файлов
// 	return CBLModule7::GetKind();
// }

void CBLModuleWrapper::OnErrorMessage(char const *pStr)
{
  CString strSyntaxErrDescr("",1000);
  CBLModule::GetSyntaxErrDescr(GetSyntaxErrCode(),strSyntaxErrDescr);

  char buffer[20];
  _itoa(GetSyntaxErrLineNum(), buffer, 10 );

  CString strSyntaxErrMarkedLine = "(";
  strSyntaxErrMarkedLine += buffer;
  strSyntaxErrMarkedLine += ")";
  strSyntaxErrMarkedLine += GetSyntaxErrMarkedLine();
  strSyntaxErrMarkedLine += "\t";
  strSyntaxErrMarkedLine += strSyntaxErrDescr;

  // ВНИМАНИЕ, ошибки на экран выдаем только в особых случаях
	if (m_bEnableThrow)
	{
		throw(strSyntaxErrMarkedLine); // artbear
	}

	CString strError;
	{
	  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

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
		CString strNameOfFile = pHierarchy->GetNameOfFile(strNameOfComp);
		CSetOfHierarchy::MakeFullFileName(&strNameOfFile);
		strError += strNameOfFile;

		char buffer[20];
		_itoa( GetSyntaxErrLineNum(), buffer, 10 );
		strError += "(";
		strError += buffer;
		strError += ")";
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

// todo refactoring выполняемый модуль (корр. обработка исключений)
int CBLModuleWrapper::OnRuntimeError(void)
{
// kms почему-то использовал такой код
// 	if (!*m_pDbgExprEvaluating)
// 	{
// 		LogErr("=== Runtime error в модуле %s", (LPCSTR)GetModuleDescription());
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
#ifdef _DEBUG
  CString strRuntimeErrSourceLine("", 1000);
  CString strGetRuntimeErrDescr("", 500);
#else
  CString strRuntimeErrSourceLine;
  CString strGetRuntimeErrDescr;
#endif

  GetRuntimeErrSourceLine(strRuntimeErrSourceLine);
  strRuntimeErrSourceLine.TrimLeft();

  GetRuntimeErrDescr(GetRuntimeErrCode(),strGetRuntimeErrDescr);
// artbear
	if ((16 == GetRuntimeErrCode()) && m_bEnableThrow)
	{
		// TODO проверить !
		CRuntimeExceptionClass::ThrowException("Ошибка выполнения: "+strGetRuntimeErrDescr +"\r\n"+
			"<"+strRuntimeErrSourceLine+">");
	}
// завершение

  CSetOfHierarchy* pHierarchy = CSetOfHierarchy::GetHierarchy();

//  CComponentClass* pContInter = pHierarchy->GetComponentClassCurExModule(this);
  CComponentClass* pContInter = CComponentClass::GetComponentClassCurExModule(this);

  if (pContInter)
  {
    CString strError;

    CString strNameOfComp = pContInter->GetTypeString();
	CString strOldGetRuntimeErrDescr = strGetRuntimeErrDescr;

    strError += strNameOfComp;
    strError += "::";
    strError += CExecuteModule::GetExecuteMethInfo(this);
	strError += " : "+strOldGetRuntimeErrDescr;

    strError += "\r\n";
    strError += strRuntimeErrSourceLine;

    strError += "\r\n{";
    CString strNameOfFile = pHierarchy->GetNameOfFile(strNameOfComp);
    CSetOfHierarchy::MakeFullFileName(&strNameOfFile);

    strError += strNameOfFile;
    char buffer[20];
    _itoa( GetRuntimeErrLineNum(), buffer, 10 );
    strError += "(";
    strError += buffer;
    strError += ")";
    strError += " }";

    strGetRuntimeErrDescr = strError;
    strGetRuntimeErrDescr += "\r\n";
  }

	//CRuntimeExceptionClass::SetException(strGetRuntimeErrDescr);
  //CBLModule7::OnRuntimeError();
  {

    /*CString strModulePath_temp = m_strModulePath;
    strModulePath_temp.MakeLower();

	if (strModulePath_temp.Find(".ert") != -1) // это чтобы попытка открыть текстовый файл не пугала пользователя сообщением в конфигураторе
		CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());
	else
		//CBLModule7::OnErrorMessage(pStr);
		return GetRuntimeErrLineNum();
	*/

    // artbear совместно с АльФ-ом
	// при использовании следующего кода можно будет открывать те классы в конфигураторе,
	// в которых возникла ошибка в 1С
	  //
	// для текстовых файлов в этом случае в Конфигураторе может выдаваться ошибка "Общая файловая ошибка при доступе к "+ПутьКФайлу
	// есть несколько путей для исправления
	// 1. простой путь для исключения ошибки - возможно использовать скрипт Опенконф с  текстом
	/* рекомендую его (artbear)
		Dim bFlagInsideOnMsgBox
		bFlagInsideOnMsgBox = false
		Sub Configurator_OnMsgBox(text, style, def, Answer)
			if InStr(text, "Общая файловая ошибка при доступе к") > 0 then
				' открываю класс 1С++
				if 	bFlagInsideOnMsgBox = false then
					strFileName = Replace(text, "Общая файловая ошибка при доступе к ", "")
					Set FSO = CreateObject("Scripting.FileSystemObject")
					if FSO.FileExists(strFileName) then
						bFlagInsideOnMsgBox = true
						Documents.Open ( strFileName )
						bFlagInsideOnMsgBox = false
						Answer = mbaOK
		    			Exit Sub
					end if
				end if
			end if
			bFlagInsideOnMsgBox = false
		End Sub

	2. запретить подобное открытие ошибок в текстовых файлах - лично я против запрета (artbear)
	3. сделать данный вариант опциональным
	4. досконально разобраться, каким образом выполняется открытие файлов в Конфигураторе
		при двойном щелчке в окне сообщений 1С:Предприятия - пока спецов/или желающих нет :(

	// для каждого класса согласно иерархии создания будет показано отдельное сообщение об ошибке
	// т.е. если открыты форма Форма1, в ней создан класс КОП1, в нем КОП2, будет показано 3 сообщения
	// сначала для КОП2, далее КОП1, далее Форма1 - и двойной щелчок на каждом из сообщений при открытом Конфигураторе вызовет переход на соответствующий файл в Конфигураторе
	//

*/
	//CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());
	CRuntimeExceptionClass::SetException(strGetRuntimeErrDescr, this, m_nID, m_strModulePath, GetRuntimeErrLineNum());
  }

	return GetRuntimeErrLineNum();
}

int CBLModuleWrapper::OnStartExecution(void)
{
	if (m_bModuleContValid)
		return CBLModule7::OnStartExecution();

	return 1;
}

int CBLModuleWrapper::OnNextLine(void)
{
	if (m_bModuleContValid)
		return CBLModule7::OnNextLine();

	return 1;
}

void CBLModuleWrapper::OnEnterProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnEnterProc(n);
}

void CBLModuleWrapper::OnExitProc(int n)
{
	if (m_bModuleContValid)
		CBLModule7::OnExitProc(n);
}

void CBLModuleWrapper::OnStopExecution(void)
{
	if (m_bModuleContValid)
		CBLModule7::OnStopExecution();
}
// enum EventType
// {
//   EventType_one,
//   EventType_two
// };

void CBLModuleWrapper::OnSetSpeller(class CValue const & val)
{
  CBLModule7::OnSetSpeller(val);
}

int CBLModuleWrapper::OnDoMessageBox(char const *pStr,unsigned int n1)
{
	return CBLModule7::OnDoMessageBox(pStr, n1);
}

void CBLModuleWrapper::OnGetErrorDescription(class CString &str)
{
  CBLModule7::OnGetErrorDescription(str);
}

CBLContext* CBLModuleWrapper::GetContextFromModule(CBLModule *pMod)
{
  CBLContext *pCont  = NULL;
  if(pMod)
  {
	pCont = pMod->GetInternalData()->pGeneralContext;
  }
  return pCont;
}

void CBLModuleWrapper::SetModulePathForRightUseIntoDebugger( const CString& strNewModulePath )
{
	m_strModulePath = strNewModulePath;
	m_strModulePath.MakeUpper(); // иначе в отладчике не всегда будет ловиться точка отладки
}
