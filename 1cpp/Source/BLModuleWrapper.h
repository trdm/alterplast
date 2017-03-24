// BLModuleWrapper.h: interface for the CBLModuleWrapper class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_)
#define AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//#include "ExecuteModule.h"
//#include "ModuleString.h"
class CModuleString;

extern int CallAsFunc(CBLModule* pMod, int iProcNum, CValue& retVal, int iParamsCount, CValue** ppParams);

class CBLModuleWrapper  : public CBLModule7
{
public:	
	static CBLContext* GetContextFromModule (CBLModule* pMod);

	//CBLModuleWrapper(const CString& strNameOfComponent, class CBLContext * pContext, char const *pStr, const char* pStrNameFile = NULL, bool bEnableThrow = false);	//12
	CBLModuleWrapper(const CString& strNameOfComponent, CBLContext* pContext, CBLModuleWrapper* pMod);
	CBLModuleWrapper(const CString& strNameOfComponent, CBLContext* pContext, CModuleString const& ModuleString, bool bEnableThrow = false);

protected:
	CString GetModuleDescription();
	
	void SetModulePathForRightUseIntoDebugger( const CString& strNewModulePath );

public:	
	virtual ~CBLModuleWrapper();

protected:
	//	artbear - при нативной отладки этот метод только мешает !
	//virtual int GetKind(void)const; // artbear по рекомендации АльФ-а

	virtual int				OnSyntaxError(void);	
	virtual int				OnRuntimeError(void);	
	virtual int				OnStartExecution(void);	
	virtual int				OnNextLine(void);	
	virtual void			OnEnterProc(int);	
	virtual void			OnExitProc(int);	
	virtual void			OnStopExecution(void);	
	virtual void			OnErrorMessage(char const *);	
	virtual void			OnSetSpeller(class CValue const &);	
	virtual void			OnGetErrorDescription(class CString &);	
	virtual int				OnDoMessageBox(char const *, unsigned int);	
	
public:

	bool m_bEnableThrow; // todo рефакторинг - используется в исполняемом модуле

	// функции для удобного вызова процедур модуля с проверкой количества параметров
	static int CallAsProc0(CBLModule* pMod, int iProcNum)
	{
		CValue ValueParam;
		CValue *pParams[1] = {0};
		pParams[0] = &ValueParam;
		return pMod->CallAsProc(iProcNum, 0, pParams);
	};

	static int CallAsProc1(CBLModule* pMod, int iProcNum, class CValue const & param1)
	{
      const CValue *pParams[1] = {0};
      pParams[0] = &param1;
      return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));
	};

	static int CallAsProc2(CBLModule* pMod, int iProcNum, class CValue const & param1, class CValue const & param2)
	{
        const CValue* pParams[2] = {0};
        pParams[0] = &param1;
        pParams[1] = &param2;
        return pMod->CallAsProc(iProcNum, 2, const_cast<CValue**>(pParams));
	};

	// функции для удобного вызова процедур модуля с проверкой количества параметров
	static int CallAsFunc0(CBLModule* pMod, int iProcNum, CValue& retVal)
	{
		CValue ValueParam;
		CValue *pParams[1] = {0};
		pParams[0] = &ValueParam;
		//return pMod->CallAsProc(iProcNum, 0, pParams);
		//return pMod->CallAsFunc(iProcNum, retVal, 0,pParams);
		return ::CallAsFunc(pMod, iProcNum, retVal, 0,pParams);
	};

	static int CallAsFunc1(CBLModule* pMod, int iProcNum, CValue& retVal, class CValue const & param1)
	{
		const CValue *pParams[1] = {0};
		pParams[0] = &param1;
		//return pMod->CallAsProc(iProcNum, 1, const_cast<CValue**>(pParams));
		//return pMod->CallAsFunc(iProcNum, retVal, 1, const_cast<CValue**>(pParams));
		return ::CallAsFunc(pMod, iProcNum, retVal, 1, const_cast<CValue**>(pParams));
	};

	static int CallAsFunc2(CBLModule* pMod, int iProcNum, CValue& retVal, class CValue const & param1, class CValue const & param2)
	{
        const CValue* pParams[2] = {0};
        pParams[0] = &param1;
        pParams[1] = &param2;
        //return pMod->CallAsProc(iProcNum, 2, const_cast<CValue**>(pParams));
		//return pMod->CallAsFunc(iProcNum, retVal, 2, const_cast<CValue**>(pParams));
		return ::CallAsFunc(pMod, iProcNum, retVal, 2, const_cast<CValue**>(pParams));
	};

private:
  //CString m_strFileName;
  const CString m_strNameOfComponent;
	CString m_csFileName;
	BOOL m_bModuleContValid;

// public:
// 	// UNUSED (исп. в вып. модуль, todo refactoring)
// 	BOOL m_bEnableThrow;
};

#endif // !defined(AFX_BLMODULEWRAPPER_H__ACECBD63_19A6_11D4_8E2E_00A024AAD7AF__INCLUDED_)
