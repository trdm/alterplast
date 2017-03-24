#include "stdafx.h"
#include "RuntimeException.h"

//////////////////////////////////////////////////////////////////////////
// CRuntimeExceptionClass class (c) artbear 2007-2008
//////////////////////////////////////////////////////////////////////////

// ======================================================================
//
// begin of CRuntimeExceptionClass
//

bool CRuntimeExceptionClass::isExceptionStatus = false;
CString CRuntimeExceptionClass::strTextException;
CValue* CRuntimeExceptionClass::ExceptionValue = NULL;

//CBLModule7* CRuntimeExceptionClass::pMod = NULL;
CSafeModulePtr CRuntimeExceptionClass::pMod;

int CRuntimeExceptionClass::iRuntimeErrLineNum = 0;
CString CRuntimeExceptionClass::m_strModulePath;
DWORD CRuntimeExceptionClass::m_nID = 0;
bool CRuntimeExceptionClass::m_bStatusOfTryingBlock = false; // флаг, находимся ли в блоке Попытка-Исключение

void CRuntimeExceptionClass::Init(void)
{
	ExceptionValue = new CValue;
	Empty();
}

void CRuntimeExceptionClass::Destroy(void)
{
	delete ExceptionValue;
}

void CRuntimeExceptionClass::Empty(void)
{
	isExceptionStatus = false;
	strTextException.Empty();

	pMod = NULL;
	iRuntimeErrLineNum = 0;
	m_strModulePath.Empty();
	m_nID = 0;

}

// запомнить исключение и данные о нем
void CRuntimeExceptionClass::SetException(const CString& strExceptionA)
{
	Empty(); // TODO

	isExceptionStatus = true;
	strTextException = strExceptionA;
}

void CRuntimeExceptionClass::SetException(const CString& strExceptionA, CBLModule7* pModA, DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA)
{
	// для вызова CBLModule7::OnErrorMessageEx(strGetRuntimeErrDescr, m_nID, m_strModulePath, GetRuntimeErrLineNum());

	SetException(strExceptionA);

	pMod = pModA;
	m_nID = m_nIDA;
	m_strModulePath = m_strModulePathA;
	iRuntimeErrLineNum = iRuntimeErrLineNumA;
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::ThrowException(bool isThrow)
{
	CString strTextException_copy(strTextException);
	//static CString* p_strTextException_copy = new CString;
	//static CString& strTextException_copy ( *p_strTextException_copy );
	//strTextException_copy = strTextException;

	CBLModule7* pMod_copy = static_cast<CBLModule7*>(pMod.operator CBLModule*()); // TODO
	DWORD m_nID_copy = m_nID;

	CString m_strModulePath_copy( m_strModulePath );
	int iRuntimeErrLineNum_copy = iRuntimeErrLineNum;

	Empty();

	if (pMod_copy)
	{
		// для каждого класса согласно иерархии создания будет показано отдельное сообщение об ошибке
		// т.е. если открыты форма Форма1, в ней создан класс КОП1, в нем КОП2, будет показано 3 сообщения
		// сначала для КОП2, далее КОП1, далее Форма1 - и двойной щелчок на каждом из сообщений при открытом Конфигураторе вызовет переход на соответствующий файл в Конфигураторе
		//
		if (!GetStatusOfTryingBlock() && !IsInsideTryingBlock())
		{

			pMod_copy->OnErrorMessageEx(strTextException_copy, m_nID_copy, m_strModulePath_copy, iRuntimeErrLineNum_copy);

		}
		::RuntimeError(strTextException_copy) ;
	}
	else
	{
		if (isThrow)
			::RuntimeError(strTextException_copy) ;
		else
			::ShowMsg(strTextException_copy, mmExclamation);
	}
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::IfNeedExceptionThenThrowException(void)
{
	if (IsNeedException()) ThrowException(); 
}

// выбросить исключение для пользовательских классов 1C++
void CRuntimeExceptionClass::ThrowException(const CString& strException)
{
	SetException(strException);
	ThrowException();
}

// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет пустое
void CRuntimeExceptionClass::RuntimeError(const CValue& param)
{
	Empty();
	*ExceptionValue = param;
	::RuntimeError("");
}

// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет равно указанному
void CRuntimeExceptionClass::RuntimeError(const CValue& param, const CString& strNewTextException)
{
	Empty();
	*ExceptionValue = param;
	::RuntimeError(strNewTextException);
}

// получить ранее сохраненный объект-исключение
void CRuntimeExceptionClass::GetException(CValue & rValue)
{
	rValue = *ExceptionValue;
	ExceptionValue->Reset();
}

// получить флаг, находимся ли в блоке Попытка-Исключение
void CRuntimeExceptionClass::SaveStatusOfTryingBlock(void)
{
	m_bStatusOfTryingBlock = IsInsideTryingBlock(); // флаг, находимся ли в блоке Попытка-Исключение
}

bool CRuntimeExceptionClass::GetStatusOfTryingBlock(void)
{
	return m_bStatusOfTryingBlock;
}

// проверить, не находимся ли мы в блоке Попытка-Исключение
bool CRuntimeExceptionClass::IsInsideTryingBlock(void)
{

	CBLModule* pCurModule = CBLModule::GetExecutedModule();
	if (!pCurModule) 
		return false;
	if (!pCurModule->pIntInfo)
		return false;

	CExecutedModule* pExecutedModule = pCurModule->pIntInfo->pExecutedModule;
	if (!pExecutedModule)
		return false;

	// это разведал АльФ - огромное ему спасибо
	bool bRetInsideTryingBlock = (pExecutedModule->m_DWordArray2.GetSize() > pExecutedModule->val_23);

	// для классов - особая обработка, т.к. у них использованные выше параметры всегда нулевые
	CBLContext* pCont = CBLModuleWrapper::GetContextFromModule(pCurModule);

	if (pCont && !bRetInsideTryingBlock && IS_KINDOF_CComponentClass(pCont))
	{
		bRetInsideTryingBlock = m_bStatusOfTryingBlock;
	}

	return bRetInsideTryingBlock;
}

//
// end of CRuntimeExceptionClass
// ======================================================================
