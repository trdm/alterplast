#if !defined(_RUNTIME_EXCEPTION_H__INCLUDED_)
#define _RUNTIME_EXCEPTION_H__INCLUDED_

//////////////////////////////////////////////////////////////////////////
// CRuntimeExceptionClass class (c) artbear 2007-2008
//////////////////////////////////////////////////////////////////////////

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// класс для работы с исключениями 1С
class CRuntimeExceptionClass
{
private:
	CRuntimeExceptionClass() {}; // класс нельзя создать

	static bool isExceptionStatus;
	static CString strTextException;
	static CValue* ExceptionValue;

	//static CBLModule7* pMod;
	static CSafeModulePtr pMod;
	static int iRuntimeErrLineNum;
	static CString m_strModulePath;
	static DWORD m_nID; // -1 или 0
	static bool m_bStatusOfTryingBlock; // флаг, находимся ли в блоке Попытка-Исключение
public:
	static void Init(void);
	static void Destroy(void);

	static bool IsNeedException() { return isExceptionStatus; };
	static void Empty(void);

	// запомнить исключение и данные о нем
	static void SetException(const CString& strExceptionA);
	static void SetException(const CString& strExceptionA, CBLModule7* pModA,
		DWORD m_nIDA, const CString& m_strModulePathA, int iRuntimeErrLineNumA);

	// выбросить исключение для пользовательских классов 1C++
	static void IfNeedExceptionThenThrowException(void);

	// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет пустое
	static void RuntimeError(const CValue& param);
	// выбросить исключение, запомнив переданный объект. Сообщение об ошибке будет равно указанному
	static void RuntimeError(const CValue& param, const CString& strTextException);
	// получить ранее сохраненный объект-исключение
	static void GetException(CValue & rValue);

	// получить флаг, находимся ли в блоке Попытка-Исключение
	static void SaveStatusOfTryingBlock(void);
	static bool GetStatusOfTryingBlock(void);

	// проверить, не находимся ли мы в блоке Попытка-Исключение
	static bool IsInsideTryingBlock(void);

private:
	static void ThrowException(bool isThrow = true);
	static void ThrowException(const CString& strException);
};


#endif // _RUNTIME_EXCEPTION_H__INCLUDED_