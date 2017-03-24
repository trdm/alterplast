// ExecuteModule.h: interface for the CExecuteModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_EXECUTEMODULE_H__36AFAA24_D023_4F96_BAE4_06DF05F44716__INCLUDED_)
#define AFX_EXECUTEMODULE_H__36AFAA24_D023_4F96_BAE4_06DF05F44716__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
//#include <process.h> 
#include <boost/utility.hpp>
#include <memory>

#include "System/sh_ptr.hpp"

class CExecuteModule : public CBLContext
{
	DECLARE_DYNCREATE(CExecuteModule);
public:	
	static CString GetExecuteMethInfo(CBLModule *pModule);
	enum {
	methStart,
	methAssignSource,
	methAssignContext,
	methCompile,
	methExecute,
	methAssignFriendModule,
	methHasRetVal,
	methGetContextDescr,
	methGetTextModules,
	methWorkWithMD,
  methGetInfo,
  procGetContextMeth,
  procRaiseError,
  IDProcErasePostState,
  IDProcRestorePostState,
  IDProcExInfo,
  IDProcInfoAboutCurExeMeth,
  IDFuncGetExeption,
  IDProcThrow,
  //methStartThread,
  IDFuncGetProcInfo,
	IDProcCompileAndExecute,
	IDProcEnableExceptions,
	iFuncCallContextProcedure,
	iFuncCallContextFunction,
	iProcEnableInsideCalling,

	iFuncGetModuleText,

	lastMethod,
	};

	CExecuteModule();
	virtual ~CExecuteModule();
	virtual int  CallAsFunc(int,CValue &,CValue * *);	//935
	virtual int  CallAsProc(int,CValue * *);	//937
	//virtual void  DecrRef(void);	//1086
	virtual int  FindMethod(char const *)const;	//1366
	virtual int  FindProp(char const *)const;	//1369
	virtual char const *  GetCode(void)const;	//1544
	virtual int  GetDestroyUnRefd(void)const;	//1657
	//virtual void  GetExactValue(CValue &);	//1708
	virtual class CObjID   GetID(void)const;	//1804
	//virtual class CBLContextInternalData *  GetInternalData(void);	//1826
	virtual char const *  GetMethodName(int,int)const;	//1964
	virtual int  GetNMethods(void)const;	//2005
	virtual int  GetNParams(int)const;	//2008
	virtual int  GetNProps(void)const;	//2015
	virtual int  GetParamDefValue(int,int,CValue *)const;	//2122
	virtual char const *  GetPropName(int,int)const;	//2187
	virtual int  GetPropVal(int,CValue &)const;	//2221
	virtual long  GetTypeID(void)const;	//2505
	virtual char const *  GetTypeString(void)const;	//2513
	virtual CType   GetValueType(void)const;	//2547
	virtual int  HasRetVal(int)const;	//2657
	//virtual void  IncrRef(void);	//2668
	//virtual void  InitObject(CType const &);	//2721
	//virtual void  InitObject(char const *);	//2722
	virtual int  IsExactValue(void)const;	//2808
	virtual int  IsOleContext(void)const;	//2846
	virtual int  IsPropReadable(int)const;	//2863
	virtual int  IsPropWritable(int)const;	//2865
	virtual int  IsSerializable(void);	//2874
	virtual int  SaveToString(CString &);	//3295
	//virtual void  SelectByID(class CObjID,long);	//3350
	virtual int  SetPropVal(int,CValue const &);	//3694

	void GetMethodOfContext(CValue * * ppValue) const;

	// эти два метода выполн€ют проверку, а затем вызывают один из след.2 методов
	bool CallContextProcedure(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList) const;
	bool CallContextFunction(const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue& RetValue) const;

	// эти 2 метода без проверки параметров
	bool CallContextProcedure(CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont) const;
	bool CallContextFunction(CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont, CValue& RetValue) const;

	BOOL funcCompile( CValue &rValue );
	BOOL funcExecute( CValue &rValue );
	void funcHasRetVal( const CValue& ObjectContextValue, const CValue& MethodNameValue, CValue &rValue );
	int funcGetExeption( CValue & rValue );
	void funcGetProcInfo( CValue * * ppValue, CValue & rValue );

	void procStart();
	void procAssignSource( const CString& strModuleText );
	void procAssignContext( CBLContext* pCont );
	void procGetContextDescr( CValue * * ppValue );
	void procAssignFriendModule();
	void procWorkWithMD();
	void procGetInfo( CValue * * ppValue );
	void procGetMethodOfContext( CValue * * ppValue );
	void procErasePostState();
	void procRestorePostState();
	void procExInfo();
	void procInfoAboutCurExeMeth();
	void procThrow( CValue * * ppValue );
	void procCompileAndExecute();
	void procEnableExceptions();
	void procEnableInsideCalling();

	CString GetModuleText() const;

private:
	static class CObjID ObjID;

	static struct paramdefs {
		char * Names[2];
		int HasReturnValue;
		int NumberOfParams;
	}  defFnNames[];

	boost::scoped_ptr<CBLModuleWrapper> pBLModule;

	unsigned char *pPoint;
	//bool m_bEnableThrow;

	bool CallContextMethod(bool bIsFunction, CBLContext* pCont, const CString& strMethodName, CValueListContext* pValueListCont, CValue* pRetValue = NULL) const;
	bool CallContextMethod(bool bIsFunction, const CValue& ValueContext, const CValue& ValueMethodName, const CValue& ValueArgList, CValue* pRetValue = NULL) const;

	bool m_bInsideMethodCall;
	bool m_bEnableInsideCalling;

	friend class CInsideExecuteModuleGuard;

	int CallUsersMethod( bool bIsFunc, int iMethNum, CValue* rValue, CValue * * ppValue );

	bool IsUserMethod(int iMethodNumber) const { return iMethodNumber >= lastMethod; };
	int GetMethodNumberInUserModule(int iMethodNumber) const { return iMethodNumber - lastMethod; };
};

// класс-сторож перед вызовом/после вызова определенных процедур/функций с аргументами в списке значений
class CCallWithValueListGuard
{
private:
	int m_iMethodParams;
	int m_iNumArgs;
	CValueListContext* m_pValListCont;

	std::vector<CValue*> ppValues;

	std::vector<sh_ptr<CValue> > AddedValuesVector;
public:
	CValue** GetData() { return ppValues.empty() ? NULL : &ppValues[0] ; };

 	CCallWithValueListGuard(int iMethodParamsA, CValueListContext* pValListContA);
	~CCallWithValueListGuard();
};

#endif // !defined(AFX_EXECUTEMODULE_H__36AFAA24_D023_4F96_BAE4_06DF05F44716__INCLUDED_)