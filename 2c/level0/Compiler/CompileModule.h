// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// CompileModule.h: interface for the CCompileModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_)
#define AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "TranslateModule.h"
#include "SystemFunctions.h"
#include "StringExt.h"


typedef CArray<CValue,CValue&> CDefValueList;
typedef CArray<bool,bool> CDefBoolList;
//typedef CArray<int,int> CDefIntList;
typedef CUIntArray CDefIntList;



//�������� �������:
enum
{
	RETURN_NONE=0,	//��� �������� (��� ������)
	RETURN_PROCEDURE,//������� �� ���������
	RETURN_FUNCTION,//������� �� �������
};

//�������� ���������� (�������� � �����. ��������� � ������� nArray ����-����)
enum
{
	DEF_VAR_SKIP=-1,//����������� ��������
	DEF_VAR_DEFAULT=-2,//�������� �� ���������
	DEF_VAR_TEMP=-3,//������� ��������� ��������� ����������
	DEF_VAR_NORET=-7,//������� (���������) �� ���������� ��������
	DEF_VAR_CONST=1000,//�������� ��������


};

//____________________________________________________________________________
struct SParamRun
{
public:
	SParamRun()
	{
		nArray=0;
		nIndex=0;
	}
	int nArray;
	int nIndex;
};
//____________________________________________________________________________
struct SParam : public SParamRun
{
public:
	CString csType;//��� ���������� � ����. ������� (� ������ ����� ���������)
};

//____________________________________________________________________________
class CVariable
{
public:
	CVariable()
	{
		bExport=0;
		bExist=0;
		nNumber=0;
	};
	BOOL bExist;
	BOOL bExport;
	int nNumber;
	CString csName;//��� ����������
	CString csType;//��� ��������
};//CVariable


//____________________________________________________________________________
//�������� ������ ���� ���������
class CByte
{
public:
	CByte() 
	{
		//ZeroMemory(this,sizeof(CByte)-sizeof(CString));
		nOper=0;
		nStringNumber=0;
		nNumberLine=0;
	}
	//��� ����������:
	int	nOper;
	int	nStringNumber;	//����� ��������� ������ (��� ������ ������)
	int	nNumberLine;	//����� ������ ��������� ������ (��� ����� ��������)

	//��������� ��� ����������:
	struct SParamRun Param1;
	struct SParamRun Param2;
	struct SParamRun Param3;
	struct SParamRun Param4;// - ������������ ��� �����������
	CString csModuleName;//��� ������ (�.�. �������� include ����������� �� ������ �������)
};

//____________________________________________________________________________
//����� ����-����
class CByteCode
{
public:
	CByteCode()
	{
		CodeList.SetSize(0,64);
		Clear();
	};
	//�������� ��������� ��������������
	BOOL	bOptimize1;
	BOOL	bOptimize2;
	BOOL	bOptimize3;

	//��������
	CArray <CByte,CByte&>	CodeList;//����������� ��� ������
	CArray <CValue,CValue&>	ConstList;//������ �������� ������
	CMapStringToPtr			FuncList;//������ ������� ������
	CMapStringToPtr			VarList;//������ ���������� ������
	CMapStringToPtr			ExportVarList;//������ ���������� ���������� ������
	CMapStringToPtr			ExportFuncList;//������ ���������� ������� ������
	CArray <CString,CString&>aExportList;//���������������� ������� � ����������
	int nVarCount;		//���������� ��������� ���������� � ������
	int nStartModule;	//������ ������� ������ ������
	CString csModuleName;//��� ������������ ������, �������� ����������� ����-���
	CString csModule;	//��� ������������ ������
	BOOL bCompile;		//������� �������� ����������
	//int nCurLine;		//������� ����������� ������ ����-����
	CByteCode *pParent;	//������������ ����-��� (��� ��������)

	class CCompileModule *pModule;//�������� ����� � ������������� ������� (�.�. �� �������� ���. ���� ������ ��� ������ ������� ������)

	//������ ������� ����������
	CArray <CValue *,CValue *> aExternValue;


	void SetModule(class CCompileModule *pSetModule){pModule=pSetModule;};


	void Clear()
	{
		bOptimize1=0;
		bOptimize2=0;
		bOptimize3=0;
		pParent=0;
		bCompile=0;
		nStartModule=0;
		nVarCount=0;
		CodeList.RemoveAll();
		ConstList.RemoveAll();
		aExternValue.RemoveAll();
		FuncList.RemoveAll();
		VarList.RemoveAll();
		ExportVarList.RemoveAll();
		ExportFuncList.RemoveAll();
		aExportList.RemoveAll();
	};
	void Save(CString csFileName);
	void Load(CString csFileName);
};

//____________________________________________________________________________
struct SLabel
{
public:
	CString	csName;
	int		nLine;
	int		nError;
};

#define VAR_LOCAL	1
#define VAR_EXPORT	2
#define VAR_NUMBER	10
#define VAR_STRING	11
#define VAR_DATE	12
#define VAR_ARRAY	13

/******************************************************************
�����: �������� ����������  
*******************************************************************/
class CCompileContext
{
public:
	CCompileContext(CCompileContext *hSetParent=0)
	{
		pParent=hSetParent;
		nDoNumber=0;
		nReturn=0;
		nFindLocalInParent=1;
		pModule=0;

		pStopParent=0;
		pContinueParent=0;
		if(hSetParent)
		{
			pStopParent=hSetParent->pStopParent;
			pContinueParent=hSetParent->pContinueParent;
		}
		nTempVar=0;
		bStaticVariable=0;
	};
	~CCompileContext();

	class CCompileModule *pModule;
	void SetModule(class CCompileModule *pSetModule){pModule=pSetModule;};


	CCompileContext *pParent;//������������ ��������
	CCompileContext *pStopParent;//������ ����������� ������� �����������
	CCompileContext *pContinueParent;//������ ����������� ������� �����������
	BOOL bStaticVariable;		//��� ���������� ���������

	//����������
//	CMapStringToPtr cVariablesDef;	//����������
//	CMapStringToPtr cVariables;		//������ ������������� ������������� ����������
	CMap <class CStringExt ,class CStringExt ,CVariable ,CVariable&> cVariables;
	BOOL FindVariable(CString csName);
	SParam GetVariable(CString csName,bool bFindInParent=1,bool bCheckError=0);
	SParam AddVariable(CString csName,CString csType="",BOOL bExport=0);
	int nTempVar;//����� ������� ��������� ����������

	int nFindLocalInParent;//������� ������ ���������� � �������� (�� ���� �������), � ��������� ������� � ��������� ������ ������ ���������� ����������)


	//������� � ���������
	CMapStringToPtr		cFunctions;//������ ������������� ����������� �������
	int nReturn;//����� ��������� ��������� RETURN : RETURN_NONE,RETURN_PROCEDURE,RETURN_FUNCTION
	CString csCurFuncName;//��� ������� ������������� ������� (��� ��������� �������� ������ ����������� �������)

	//�����
	//��������� ��������
	int nDoNumber;//����� ���������� �����
	CMapWordToPtr   aContinueList;//������ ���������� Continue
	CMapWordToPtr   aBreakList;//������ ���������� Break

	//��������� ������� �������� ��� ������ Continue � Break
	void StartDoList();
	void FinishDoList(CByteCode	&cByteCode,int nGotoContinue,int nGotoBreak);


	//�����
	CMapStringToPtr cLabelsDef;	//����������
	CArray <SLabel,SLabel> cLabels;	//������ ������������� ��������� �� �����
	void DoLabels();
};//CCompileContext



//____________________________________________________________________________
class CParamVariable
{
public:
	CParamVariable()
	{
		bByRef=1;
		vData.nArray=-1;
		vData.nIndex=-1;
	};
	CString csName;//��� ����������
	SParam 	vData;//�������� �� ���������
	int	bByRef;
	CString csType;//��� ��������
};//CParamVariable


//____________________________________________________________________________
//����������� �������
class CFunction
{
public:
	CFunction(CString csFuncName,CCompileContext *pSetContext)
	{
		csName=csFuncName;
		pContext=pSetContext;
		bExport=0;
		bVirtaul=0;
		nVarCount=0;
		nStart=0;
		nFinish=0;
		bSysFunction=0;
		RealRetValue.nArray=0;
		RealRetValue.nIndex=0;
		nNumberLine=-1;
	};
	~CFunction()
	{
		if(pContext)//������� ����������� �������� (� ������ ������� ���� ������ ����� � ��������� ����������)
			delete pContext;
	};
	CString csRealName;//��� �������
	CString csName;//��� ������� � ������� ��������
	CArray<CParamVariable,CParamVariable> aParamList;
	bool bExport;
	bool bVirtaul;
	CCompileContext *pContext;//������� ����������
	int nVarCount;//����� ��������� ����������
	int nStart;//��������� ������� � ������� ����-�����
	int nFinish;//�������� ������� � ������� ����-�����

	SParam RealRetValue;//��� �������� ���������� ��� �������� ������
	bool	bSysFunction;
	CString csType;		//��� (� ����. �������), ���� ��� �������������� �������

	//��� IntelliSense
	int	nNumberLine;	//����� ������ ��������� ������ (��� ����� ��������)
	CString csShortDescription;//�������� � ���� ��� ������ ����� ��������� ����� �������(���������)
	CString csLongDescription;//�������� � ���� ���� ������� (�.�.� ��� ������ �����) ���� ����������� �� ����������� ������� (���������)
};//CFunction

//____________________________________________________________________________
class CCallFunction
{
public: 
	CString csName;//��� ���������� �������
	CArray<SParam,SParam> aParamList;//������ ������������ ���������� (������ ���������, ���� �������� �� ������, �� �.�. (-1,-1))
	SParam sRetValue;//���������� ���� ������ ������������ ��������� ���������� �������
	SParam sContextVal;//��������� �� ���������� ��������
	int nAddLine;//��������� � ������� ����-�����, ��� ���������� ����� (��� ������, ����� ���� �����, �� ������� ��� �� ���������)
	int nError;//��� ������ ��������� ��� �������

	int	nStringNumber;	//����� ��������� ������ (��� ������ ������)
	int	nNumberLine;	//����� ������ ��������� ������ (��� ����� ��������)
	CString csModuleName;//��� ������ (�.�. �������� include ����������� �� ������ �������)
};//CCallFunction


/******************************************************************
�����: ����������
*******************************************************************/
class CCompileModule  : public CTranslateModule
{
public:
	CCompileModule();
	virtual ~CCompileModule();

	//�������� ������:
	void Clear();//����� ������ ��� ���������� ������������� �������
	void AddVariable(CString csName,CValue *pValue);//��������� ������� ����������
	void SetParent(CCompileModule *pSetParent,CCompileModule *pSetStopParent=0,CCompileModule *pSetContinueParent=0);//��������� ������������� ������ � ������������ ����. �����������
	bool Compile(CString csStrModule);//���������� ������



	//��������:
	CCompileModule *pParent;//������������ ������ (�.�. �� ��������� � �������� ��������� ���� ����������� ������)
	CCompileModule *pStopParent;//������-�����������, ������� � �������� ������������ ����� ���������� ������� � ����������, �.�. �� �� �������� ������������ (���� �������� �������� ������� ��� �������� ������� ������) - ��� ��� ����� ��� �������� �������������� ������������
	CCompileModule *pContinueParent;//������ ����������� ������� �����������

	int nCommonModule;
	int bIsObject;//������� �������
	int bExtendMode;//������� ���������� ���� ������������ � �������� ������� � ����

	//������� �������� ����������, ������� � �����
	CCompileContext	cContext;
	CCompileContext	*pContext;	

	//�������� ������ ����-����� ��� ���������� ����������� �������
	CByteCode	cByteCode;
	int nCurrentCompile;		//������� ��������� � ������� ������

	BOOL bExpressionOnly;		//������ ���������� ��������� (��� ����� ������� �������)

	//������ ����� ����������
	CMapStringToString cTypeList;

protected:
	CMapStringToPtr	HashConstList;
	CArray <CCallFunction*,CCallFunction*> apCallFunctions;	//������ ������������� ������� �������� � �������

	int nLastNumberLine;



public:
	CCompileContext	*GetContext()
	{
		cContext.SetModule(this);
		return &cContext;
	};

	//������ ������ ������ ��� ����������:
	void SetError(int nErr,CString s="");
	void SetError(int nErr,char c);
	SParam GetExpression(int nPriority=0);
	bool CompileBlock();


	
protected:
	CLexem PreviewGetLexem();
	CLexem	GetLexem();
	CLexem	GETLexem();
	void	GETDelimeter(char c);

	bool IsNextDelimeter(char c);
	bool IsNextKeyWord(int nKey);
	void GETKeyWord(int nKey);
	CString GETIdentifier(int nOrig=0);
	CValue GETConstant();

	void AddLineInfo(CByte &code);
	bool CompileModule();
	bool CompileFunction();
	//bool CompileContext();

	bool CompileDeclaration();
	bool CompileGoto();
	bool CompileIf();
	bool CompileWhile();
	bool CompileFor();
	SParam GetCallFunction(CString csName);


	SParam GetCurrentIdentifier(int &nIsSet);

	SParam GetVariable(CString csName,bool bCheckError=0);
	SParam GetVariable();
	SParam FindConst(CValue &vData);

	bool AddCallFunction(CCallFunction* pRealCall);
	CFunction *GetFunction(CString csName,int *pNumber=0);


	int IsTypeVar(CString Str="");
	CString GetTypeVar(CString Str="");
	void AddTypeSet(SParam Var);

	int GetConstString(CString csMethod);

	void SetImport();
public:
	void ToDebug();

	static CMapStringToPtr	cSysFunctions;//������ ��������� �������
	static void LoadSystemFunctions();
	static void ClearSystemFunctions();

};

#endif // !defined(AFX_COMPILEMODULE_H__8904ABEE_EFAB_454A_A913_93F908FCB275__INCLUDED_)
