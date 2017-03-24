
#ifndef _ANALYSER_H_
#define _ANALYSER_H_

#include "istrmap.h"
#include "parser.h"
#include "Types1C.h"
#include "CommonFunctions.h"

class CMethod;


class CCodeContainer
{
protected:
	CCode* Code;

public:
	CCodeContainer(CCode* pCode) {Code = pCode;};

	int StartLine() {return Code->StartLine;};
	int StartCol()  {return Code->StartCol;};
	int EndLine()   {return Code->EndLine;};
	int EndCol()    {return Code->EndCol;};

	const char* CodeTypeAsStr() {return Code->CodeTypeAsStr(Code->CodeType);};
	CString GetText() {return Code->GetText();};
	
	bool HasErrors() {return Code->HasErrors();};


	CCodeIterator* CreateIterator() {return new CCodeIterator(Code);};

};


class CExecutionContext : public CCodeContainer
{
protected:
	CExecutionContext* pParentContext;

private:
	CVarList DeclaredVars;
	CVarList AutomaticVars;
	CString ReturnedType;

public:
	int DesiredLine, DesiredCol;
	CString TypeFound;
	CString MethodFound;

public:
	CExecutionContext() :CCodeContainer(NULL) {pParentContext = NULL;};
	CExecutionContext(CCode* pCode, CExecutionContext* pParentCont);
	CExecutionContext(CCode* pCode);

	void SetParentContext(CExecutionContext* pParentCont) {pParentContext = pParentCont;};

	CCode* GetStatements();
	CCode* GetVarDeclList();

	void Analyse(CCode* pCode);
	void ReadVarDeclarations(CCode* pCode);
	void AnalyseStatements(CCode* pCode);
	bool CheckDesiredPos(CCode* left, CCode* right, const char* Type);

	virtual void Analyse(const char* DesiredMethName) {};
	virtual CMethod* GetMethod(CString MethName, bool bScanIfNotInList = true) {return NULL;};

	void AddDeclaredVar(CVariable* pVar);
	CVariable* AddDeclaredVar(const char* VarName, CString& VarType) { CutLastDot(VarType); return DeclaredVars.Add(VarName, VarType); };
	CVariable* GetDeclaredVar(const char* VarName) { return DeclaredVars.GetVar(VarName); };
	bool SetDeclaredVarType(const char* VarName, const char* VarType);

	void AddAutomaticVar(CVariable* pVar);
	CVariable* AddAutomaticVar(const char* VarName, const char* VarType) { return AutomaticVars.Add(VarName, VarType); };
	CVariable* GetAutomaticVar(const char* VarName) { return AutomaticVars.GetVar(VarName); };

	virtual void AddTypedVar(const char* VarName, const char* VarType);
	virtual CVariable* GetVar(const char* VarName);
	const char* GetVarType(const char* VarName);

	virtual void AddVarProp(const char* VarName, const char* PropName, const char* PropType);

	virtual void SetReturnedType(const char* type) { ReturnedType = type; };
	const char* GetReturnedType() { return ReturnedType; };
	virtual const char* GetReturnedType(const char* MethodName) { return ""; };

	virtual CString GetFormalParameterType(int param_no) {return "";};
	virtual CString GetFormalParameterType(CString& MethName, int param_no) {return "";};

private:
	void SetTypeFound(const char* type);
};

//------------------------------------------------
class CModule : public CExecutionContext
{
private:
	CIStringMapToPtr Methods;

public:
	CModule(CCode* pCode) :CExecutionContext(pCode) { DesiredLine = -1; DesiredCol = -1; };
	~CModule();

	void AddMethod(CString MethName, CMethod* Method);
	virtual CMethod* GetMethod(CString MethName, bool bScanIfNotInList = true);

	virtual const char* GetReturnedType(const char* MethodName);

	virtual void Analyse(const char* DesiredMethName);
	void Analyse();

private:
	CCode* GetMethList();
	void AnalyseStatements(CCode* Statements);
};

//------------------------------------------------
class CFormalParameter : public CVariable
{
public:
	int Number;

public:
	CFormalParameter(const char* aType) :CVariable(aType) {};
};

//------------------------------------------------
class CMethod : public CExecutionContext
{
private:
	CModule* ParentModule;

	CLexema* MethodName;

	CCode* ParamList;
	CVarList FormalParameters;
	//CIStringMapToPtr FormalParameters;

public:
	CMethod(CCode* pCode, CModule* pMod);

	CString GetName();

	void Analyse();

	virtual void AddTypedVar(const char* VarName, const char* VarType);
	virtual void AddVarProp(const char* VarName, const char* PropName, const char* PropType);
	virtual CVariable* GetVar(const char* VarName);

	const char* GetReturnedType() {return CExecutionContext::GetReturnedType(); };
	virtual const char* GetReturnedType(const char* MethodName);


	virtual CString GetFormalParameterType(int param_no);
	virtual CString GetFormalParameterType(CString& MethName, int param_no);

	void PrintHeader();
	void PrintFormalParameters();

private:
	void ParseHeader();
	void FillParamList();
};


//------------------------------------------------
class CStatements : public CCodeContainer
{
private:
	CExecutionContext* pParentContext;

public:
	CString ReturnedType;

public:
	CStatements(CCode* pCode, CExecutionContext* pContext) :CCodeContainer(pCode) { pParentContext = pContext; };
	void Analyse();

private:
	static void RecursiveAnalyse(CExecutionContext* Context, CCode* Code);
};


//------------------------------------------------
class CMethCall : public CCodeContainer
{
private:
	CCode* Params;
public:
	CMethCall(CCode* pCode) :CCodeContainer(pCode) {Params = NULL;};

	CString GetMethName();
	CCode* GetParams();

	CString GetType1C(CExecutionContext* Context);
	CString GetSubType(CString& LeftPartType);

private:
	void UsedVars(CExecutionContext* Context, CString& LeftPartType);

	bool ExtractLeftmostString(CCode* pCode, CString& strRes);

	bool IsObjectCreatingMethod(CString& MethName, CString& Type);
	void ModifyExemplar(CVariable* pVar, CTypeExemplarModifyingMethod* pModMeth);
};

//------------------------------------------------
class CLValue : public CCodeContainer
{
public:
	CLValue(CCode* pCode) :CCodeContainer(pCode) {};

	CString GetType1C(CExecutionContext* Context);
	CString GetSubType(CString& LeftPartType);
};

//------------------------------------------------
class CExpression : public CCodeContainer
{
public:
	CExpression(CCode* pCode) :CCodeContainer(pCode) {};

	CString GetType1C(CExecutionContext* Context);
	CString GetSubType(CString& LeftPartType);
};

//------------------------------------------------
class COperator : public CCodeContainer
{
public:
	COperator(CCode* pCode) :CCodeContainer(pCode) {};

	bool IsAssignment();
	void AnalyseAssignment(CExecutionContext* Context);

	bool IsReturn(CExecutionContext* Context, CString& ReturnedType);
	void PrintAssignment();
};


#endif
