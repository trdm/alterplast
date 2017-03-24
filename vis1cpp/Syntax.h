// Syntax.h: interface for the CSyntax class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SYNTAX_H__FBE32DEF_AF60_4613_B6D1_3BDF510A5A5A__INCLUDED_)
#define AFX_SYNTAX_H__FBE32DEF_AF60_4613_B6D1_3BDF510A5A5A__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include <boost/scoped_ptr.hpp>

class CClassInfo;
class CMyModule;

class CSyntax  
{
public:
	CSyntax(CMyModule*);
	virtual ~CSyntax();

	static BOOL SyntaxCheck();

	static void OpenNextError(bool prev=false);

	static BOOL CompileOne(CClassInfo* pInfo,bool bWithGM, bool bCallOrigCompile = false);
	static BOOL CompileSomeClasses(CClassInfo *pInfo, bool bWithGM, bool bCallOrigCompile = false);
	static void CompileSomeClassesWithFlagCompileAll( CClassInfo* pClassInfo );

	static void CompileAll();
	
	static void Clear();

	static int ModuleCompile(CBLModule7* pMod, bool& bInsideCompileHandler);
	static long GetTotalError() { return m_TotalError; }

	// чтобы не было сообщений на каждый проверяемый файл
	static void EnableFlagCompileAll(void) { m_bInsideCompileAll = true; }
	static void DisableFlagCompileAll(void) { m_bInsideCompileAll = false; }

private:
	void ShowError();

	// возвращает true, если компилировали класс, false - если иначе
	static bool CompileIfClass(const CString& strDocFullPath, bool bDisableSystemAddCompile, BOOL& bCompileResult);

	CString m_Description;
	CString m_Ident;
	CString m_MarkedLine;
	DWORD m_LineNum;
	CClassInfo* m_pInfo;
	CMyModule* pMod;

	static bool bEnableCompileGM;
	static bool bEnableCompileModule;
	//static bool bInsideCompileHandler;
	static bool m_bInsideCompileAll;

	static CTypedPtrList<CPtrList, CSyntax*> m_ErrorList; //TODO отрефакторить этот объект, т.к. его использование чревато ошибками с new/delete

	static long m_TotalError;
	static bool m_silent;
	static boost::scoped_ptr<CMyModule> m_pGM; //CMyModule* m_pGM;

	static void IfClassThenAddSpecialFunctionIntoClassText(CString* pClassText);

protected:
	static void DeleteError(bool prev);
	static POSITION m_CurPos;
	static BOOL Compile(CClassInfo* pInfo, bool bCallOrigCompile = false);
	static void CompileGM();
	static void DeleteGM(void);
};

#endif // !defined(AFX_SYNTAX_H__FBE32DEF_AF60_4613_B6D1_3BDF510A5A5A__INCLUDED_)
