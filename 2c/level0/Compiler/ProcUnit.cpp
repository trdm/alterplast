// ��������� �����: VTOOLS.RU (info@vtools.ru) 2002,2003�.
// CProcUnit.cpp: implementation of the CProcUnit class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "afxwin.h"
#include "math.h"
#include "ProcUnit.h"
#include "OutToDebug.h"
#include "Definition.h"
#include "ValueArray.h"

typedef CArray <BOOL,BOOL> CMapInt;

#ifndef SMALL_TRANSLATE
	#include "ValueContext.h"
	#include "ValueObject.h"
#else
	//��������
	BOOL bUseDebug=0;
	BOOL bUseDebug2=0;
	BOOL bDebugLoop=0;
	BOOL bDebugStopLine=0;
	BOOL bStopRunProgram=0;
	int bStopDebug=0;
	int nCurrentNumberStopContext=0;
	int nWaitCursor=1;

	void SendStringToConfig(CString csMessage){};
	void DoDebugLoop(CString csModule,int nLine,CRunContext *pSetRunContext){};
	CMapInt *GetDebugList(CString csModule){return 0;};
	CString  csCurrentExeDir;
	CString  csIBDir;
	CString  csTitleWnd;
#endif

extern	int	bDoLoop;
extern BOOL bUseDebug;
extern BOOL bDebugStopLine;
extern int nCurrentNumberStopContext;
extern int bStopDebug;
extern BOOL bStopRunProgram;

extern BOOL afxMessageFromConfig;
extern BOOL afxSimpleErrorMessage;
extern CString afxStrErrorMessage;
extern CString sCurError;
extern BOOL bConfigGetAttribute;

void DoDebugLoop(CString csModule,int nLine,CRunContext *pRunContext);
CMapInt *GetDebugList(CString csModule);


int afxEventMessage=0;
CProcUnit *afxCurrentRunModule=0;
#define EscapePress()	GetAsyncKeyState(VK_ESCAPE)==-32768


#include <vector>
using namespace std;
vector <CRunContext *> afxRunContext;           //������ ����������� ����� �������
CErrorPlace afxErrorPlace;


#include "ProcUnitContext.c"
#include "ProcUnitInline.c"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CProcUnit::CProcUnit()
{
	pppArrayList=0;
	ppArrayCode=0;
	pByteCode=0;
	nCommonModule=0;
	pCurrentObj=0;
	nAutoDeleteParent=0;
}
void CProcUnit::Clear()
{
	if(pppArrayList)
		delete []pppArrayList;
	if(ppArrayCode)
		delete ppArrayCode;
	vContext.Detach();

	if(this==afxCurrentRunModule)
		afxCurrentRunModule=0;

/*
	if(GetParent())
	if(GetParent()->nAutoDeleteParent)
	if(!GetParent()->nCommonModule)
	{
		ASSERT(GetParent()->nCommonModule!=1);
		ASSERT(GetParent()->nCommonModule!=3);
		delete GetParent();
	}
	if(nAutoDeleteParent)
	{
		ASSERT(GetParent());
		delete GetParent();
	}*/

	nAutoDeleteParent=0;
	aParent.RemoveAll();
	pppArrayList=0;
	ppArrayCode=0;
	pByteCode=0;
	nCommonModule=0;
	pCurrentObj=0;
}
CProcUnit::~CProcUnit()
{
	Clear();
}
void CProcUnit::SetParent(CProcUnit *pSetParent)
{
	aParent.RemoveAll();
	if(pSetParent)
	{
		int nCount=pSetParent->aParent.GetSize();
		aParent.SetSize(nCount+1);
		aParent[0]=pSetParent;
		for(int i=1;i<=nCount;i++)
			aParent[i]=pSetParent->aParent[i-1];
	}
}
CProcUnit *CProcUnit::GetParent(int nLevel)
{
	if(nLevel>=aParent.GetSize())
	{
		ASSERT(0==nLevel);
		return 0;
	}
	else
	{
		ASSERT(nLevel==aParent.GetSize()-1 || aParent[nLevel]);
		return aParent[nLevel];
	}
}
int	CProcUnit::GetParentCount()
{
	return aParent.GetSize();
}



#define CurCode	pByteCode->CodeList[nCodeLine]
#define Index1	CurCode.Param1.nIndex
#define Index2	CurCode.Param2.nIndex
#define Index3	CurCode.Param3.nIndex
#define Index4	CurCode.Param4.nIndex

#define Array1	CurCode.Param1.nArray
#define Array2	CurCode.Param2.nArray
#define Array3	CurCode.Param3.nArray
#define Array4	CurCode.Param4.nArray


/*
#define pCurCode	(&pByteCode->CodeList[nCodeLine])

#define iIndex1	pCurCode->Param1.nIndex
#define iIndex2	pCurCode->Param2.nIndex
#define iIndex3	pCurCode->Param3.nIndex

#define iArray1	pCurCode->Param1.nArray
#define iArray2	pCurCode->Param2.nArray
#define iArray3	pCurCode->Param3.nArray
*/
#define LocVariable1 *pRefLocVars[Index1]
#define LocVariable2 *pRefLocVars[Index2]
#define LocVariable3 *pRefLocVars[Index3]
#define LocVariable4 *pRefLocVars[Index4]

//#define Variable(x) (Array##x<=0?*pRefLocVars[Index##x]:*pppArrayList[Array##x+nDelta][Index##x])
//#define Variable(x) (iArray##x<=0?*pRefLocVars[iIndex##x]:*pppArrayList[iArray##x+nDelta][iIndex##x])
#define Variable(x) (Array##x<=0?*pRefLocVars[Index##x]:(Array##x==DEF_VAR_CONST?pByteCode->ConstList[Index##x]:*pppArrayList[Array##x+nDelta][Index##x]))

#define Variable1 Variable(1)
#define Variable2 Variable(2)
#define Variable3 Variable(3)
#define Variable4 Variable(4)

#define COUNT_FOR_ESCAPE 0x1000
int nCountForEscape=COUNT_FOR_ESCAPE;

CValue CProcUnit::Exec(CRunContext *pContext,int nDelta)
{
	if(bStopRunProgram)
	{
		afxCurrentRunModule=0;
		return CValue();
	}

#ifdef _DEBUG
	if(!pContext) 
		Error("�� ����� �������� ����������");
	if(!pByteCode)
		Error("�� ����� ��� ����������");
#endif


	pContext->pProcUnit=this;

	CValue cRetValue; 
	CValue *pLocVars=pContext->pLocVars;
	CValue **pRefLocVars=pContext->pRefLocVars;
	CByte	*pCodeList=pByteCode->CodeList.GetData();
	//BOOL	bOptimize1=pByteCode->bOptimize1;//����������� ������ �������

	int nCodeLine=pContext->nStart;
	int nFinish = pByteCode->CodeList.GetSize();

	CSmartStack cStack(pContext);


	if(bDebugStopLine||nCurrentNumberStopContext)
		bUseDebug=1;
	int nPrevLine=-1;

#ifdef _DEBUG//���� ����� ���� ����� �����...
		int nModuleLine;
		CString csModuleName=pByteCode->csModuleName;
#endif
	
	CArray<CPoint,CPoint> aTryList;
Start:
	try{//��������� �� 2-3% �� ������ ��������� ������
	while(nCodeLine<nFinish)
	{
		if(afxMessageFromConfig==0&&afxSimpleErrorMessage==0)
		{
			pContext->nCurLine=nCodeLine;
			afxCurrentRunModule=this;
		}

#ifdef _DEBUG//���� ����� ���� ����� �����...
		nModuleLine=pByteCode->CodeList[nCodeLine].nNumberLine;
#endif


		//�������
		if(bUseDebug)
		if(afxMessageFromConfig==0&&afxSimpleErrorMessage==0)
		if(!(CurCode.nOper==OPER_FUNC&&nDelta==1))
		{
			int nLine=pByteCode->CodeList[nCodeLine].nNumberLine;
			if(nLine!=nPrevLine)
			{
				//nPrevLine=nLine;

				CString csModuleName=pByteCode->CodeList[nCodeLine].csModuleName;
				if(csModuleName.IsEmpty())
					csModuleName=pByteCode->csModuleName;

				bDoLoop=0;
				if(bDebugStopLine>0 && nLine>=0)//������� � 
				{
					bDebugStopLine=0;
					bDoLoop=1;
				}
				else//������� �����
				if(nCurrentNumberStopContext && nCurrentNumberStopContext>=AfxGetCountRunContext() && nLine>=0)
				{
					nCurrentNumberStopContext=AfxGetCountRunContext();
					bDoLoop=1;
				}
				else//������������ ����� ��������
				{
					if(nLine>=0)
					{
						CMapInt *aDebugPointList=GetDebugList(csModuleName);
						if(aDebugPointList)
						if(aDebugPointList->GetSize()>nLine)
						{
							if((*aDebugPointList)[nLine])
								bDoLoop=1;
						}
					}
				}

				if(bDoLoop) 
				{
					DoDebugLoop(csModuleName,nLine+1,pContext);
					if(bDebugStopLine||nCurrentNumberStopContext||GetDebugList(csModuleName))
						bUseDebug=1;
				}
				if(bStopDebug>0)
				{
					if(AfxGetMainWnd())
						::BringWindowToTop(AfxGetMainWnd()->m_hWnd);
					bStopDebug=0;
					//sCurError="��������� ����������� �������������";
					sCurError="";
					aTryList.SetSize(0);
					throw (new CTranslateError());
				}
				if(bStopRunProgram)
				{
					afxCurrentRunModule=0;
					return CValue();
				}
			}
			nPrevLine=nLine;
		}

		switch (CurCode.nOper)
		{
			
		case OPER_CONST:
			CopyValue(Variable1,pByteCode->ConstList[Index2]);
			break;
		case OPER_CONSTN:
			SetTypeNumber(Variable1,Index2);
			break;

		case OPER_ADD:
			AddValue(Variable1,Variable2,Variable3);
			break;

		case OPER_SUB:
			SubValue(Variable1,Variable2,Variable3);
			break;

		case OPER_DIV:
			DivValue(Variable1,Variable2,Variable3);
			break;

		case OPER_MOD:
			ModValue(Variable1,Variable2,Variable3);
			break;

		case OPER_MULT:
			MultValue(Variable1,Variable2,Variable3);
			break;

		case OPER_LET:
			CopyValue(Variable1,Variable2);
			break;


		case OPER_INVERT:
			SetTypeNumber(Variable1,-Variable2.fData);
			break;
		case OPER_NOT:
			SetTypeNumber(Variable1,IsEmptyValue(Variable2));
			break;
		case OPER_AND:
			if(IsHasValue(Variable2)&&IsHasValue(Variable3))
				SetTypeNumber(Variable1,1);
			else
				SetTypeNumber(Variable1,0);
			break;
		case OPER_OR:
			if(IsHasValue(Variable2)||IsHasValue(Variable3))
				SetTypeNumber(Variable1,1);
			else
				SetTypeNumber(Variable1,0);
			break;

		case OPER_EQ:
			CompareValueEQ(Variable1,Variable2,Variable3);
			break;

		case OPER_NE:
			CompareValueNE(Variable1,Variable2,Variable3);
			break;

		case OPER_GT:
			CompareValueGT(Variable1,Variable2,Variable3);
			break;

		case OPER_LS:
			CompareValueLS(Variable1,Variable2,Variable3);
			break;

		case OPER_GE:
			CompareValueGE(Variable1,Variable2,Variable3);
			break;

		case OPER_LE:
			CompareValueLE(Variable1,Variable2,Variable3);
			break;
			
		case OPER_IF:
			if(IsEmptyValue(Variable1))
				nCodeLine=Index2-1;
			break;

		case OPER_FOR:
			if(Variable1.nType!=TYPE_NUMBER)
			if(Variable1.nType!=TYPE_DATE)
				Error("��� ����������� ����� ����� ������������ ������ ���������� � ����� \"����\" ��� \"�����\"");

			if(Variable1.nType==TYPE_NUMBER)
			{
				if(Variable1.fData>Variable2.fData)
					nCodeLine = Index3-1;	
			}
			else
			{
				if(Variable1.dData>Variable2.dData)
					nCodeLine = Index3-1;	
			}
			break;
		case OPER_NEXT:
			if(Variable1.nType==TYPE_NUMBER)
				Variable1.fData=Variable1.fData+1;
			else
				Variable1.dData=Variable1.dData+1;
			nCodeLine = Index2-1;
			break;

		case OPER_MSG:
			nCountForEscape-=0x100;
			//Message(Variable1);
			pByteCode->pModule->ToDebug();
			
			break;
		case OPER_ITER:
			if(IsHasValue(Variable2))
				CopyValue(Variable1,Variable3);
			else 
				CopyValue(Variable1,Variable4);
			break;
		case OPER_SET_A://��������� ��������
			{
				int nAttr=Variable1.FindAttribute(pByteCode->ConstList[Index2].sData);
				if(nAttr<0)
					CheckAndError(Variable1,pByteCode->ConstList[Index2].sData);

				Variable1.SetAttribute(nAttr,GetValue(Variable3));
				break;
			}
		case OPER_GET_A://��������� ��������
			{
				CValue *pRetValue=&Variable1;
				int nAttr=Variable2.FindAttribute(pByteCode->ConstList[Index3].sData);
				if(nAttr<0)
					CheckAndError(Variable2,pByteCode->ConstList[Index3].sData);

				CValue vRet=Variable2.GetAttribute(nAttr);
				if(vRet.nType==TYPE_REFFER)
					*pRetValue=vRet;
				else
					CopyValue(*pRetValue,vRet);

//				CopyValue(*pRetValue,Variable2.GetAttribute(nAttr));
				break;
			}
		case OPER_CALL_M://����� ������
			{
				CValue *pRetValue=&Variable1;
				CValue *pVariable2=&Variable2;
				int nMethod;

				//����������� �������
				//bOptimize1 && 
				if(Array4 && (CValue *)Array4==pVariable2->pRef)//����� ���� ������
				{
					nMethod=Index4;
#ifdef _DEBUG
					nMethod=pVariable2->FindMethod(pByteCode->ConstList[Index3].sData);
					if(nMethod!=Index4)
					{
						CString Str;
						Str.Format("Error value %d mast %d (������������� ��������� ����������� �������)",Index4,nMethod);
						Error(Str);

					}

#endif
				}
				else//�� ���� �������
				{
					nMethod=pVariable2->FindMethod(pByteCode->ConstList[Index3].sData);
					Index4=nMethod;
					Array4=(int)pVariable2->pRef;
				}

				if(nMethod<0)
					CheckAndError(Variable2,pByteCode->ConstList[Index3].sData);
#ifdef _DEBUG
				CString csName=pByteCode->ConstList[Index3].sData;
#endif

				CRunContext cRunContext(max(Array3,MAX_STATIC_VAR));
				cRunContext.nParamCount=Array3;
				//��������� ���������
				for(int i=0;i<cRunContext.nParamCount;i++)
				{
					nCodeLine++;
					if(Index1>=0)
					{
#ifdef _DEBUG
						BOOL bReadOnly=Variable1.bReadOnly;
						CValue *pVal=&Variable1;
#endif
						if(Variable1.bReadOnly&&Variable1.nType!=TYPE_REFFER)
							CopyValue(cRunContext.pLocVars[i],Variable1);
						else
							cRunContext.pRefLocVars[i]=&Variable1;
					}
				} 

				CopyValue(*pRetValue,pVariable2->Method(nMethod,cRunContext.pRefLocVars));

				nCountForEscape-=0x10;
				break;
			}
		case OPER_SYSCALL://����� ��������� �������
			{
				CValue *pRetValue=&Variable1;

				CRunContextSmall cRunContext(Array3);
				cRunContext.nParamCount=Array3;
				DEFFUNCTION Func=(DEFFUNCTION)Index2;
				//��������� ���������
				for(int i=0;i<cRunContext.nParamCount;i++)
				{
					nCodeLine++;
					if(Index1>=0)
					{
						if(Variable1.bReadOnly&&Variable1.nType!=TYPE_REFFER)
							CopyValue(cRunContext.pLocVars[i],Variable1);
						else
							cRunContext.pRefLocVars[i]=&Variable1;
					}
				}
				CopyValue(*pRetValue,Func(cRunContext.pRefLocVars));
				nCountForEscape-=0x10;
				break;
			}
		case OPER_CALL://����� ������� �������
			{
				int nModuleNumber=Array2;
				CRunContext cRunContext(Index3);
				cRunContext.nStart=Index2;
				cRunContext.nParamCount=Array3;

				CByteCode *pLocalByteCode=ppArrayCode[nModuleNumber]->pByteCode;


				CByte	*pCodeList2=pLocalByteCode->CodeList.GetData();
				cRunContext.pCompileContext=(CCompileContext *)pCodeList2[cRunContext.nStart].Param1.nArray;//�������� ��������� ����������

				CValue *pRetValue=&Variable1;

				//��������� ���������
				for(int i=0;i<cRunContext.nParamCount;i++)
				{
					//todo - bug: ��� ������ ������� ��� ���������� (������� ���������) - ��������
					//fixing...
					nCodeLine++;
					if(CurCode.nOper==OPER_SETCONST)
						CopyValue(cRunContext.pLocVars[i],pLocalByteCode->ConstList[Index1]);
					else
					{
						if(Variable1.bReadOnly||Index2!=1)//�������� ��������� �� ��������
							CopyValue(cRunContext.pLocVars[i],Variable1);
						else
							cRunContext.pRefLocVars[i]=&Variable1;
					}
				}
/*
				static int nError=0;

				nRecCount++;
				if(nRecCount>500)
				{
					nError=1;//������ - ���� ���������� ���� �����
					nRecCount--;
					return CValue();
				}
				CopyValue(*pRetValue,ppArrayCode[nModuleNumber]->Exec(&cRunContext,0));
				nRecCount--;

				if(nError)//��������� ����� �����
				{
					if(nRecCount>0)
						return CValue();
					afxCurrentRunModule=0;
					nError=0;
					Error("���������� �����c����� ������� ��������� ����������� ���������� ��������!");
				}
*/

				CopyValue(*pRetValue,ppArrayCode[nModuleNumber]->Exec(&cRunContext,0));
				nCountForEscape-=0x10;
			}
			break;
		case OPER_SET_ARRAY://��������� �������� �������
			{
				SetArrayValue(Variable1,Variable2,GetValue(Variable3));
				break;
			}

/*		case OPER_SET_ARRAY://��������� �������� �������
			{
				CValue Val;
				if(Variable3.bReadOnly&&Variable3.nType!=TYPE_REFFER)
				{
					CopyValue(Val,Variable3);
				}
				else
				{
					Val=Variable3;
				}
				SetArrayValue(Variable1,Variable2,Val);
				break;
			}
*/
		case OPER_GET_ARRAY://��������� �������� �������
				GetArrayValue(Variable1,Variable2,Variable3);
				break;
		case OPER_CHECK_ARRAY://�������� ���� ���������� �������
				CheckArray(Variable1,Variable2);
				break;
		case OPER_SET_ARRAY_SIZE://��������� ������� �������
				SetSizeArray(LocVariable1,Array2);
				break;
		case OPER_GOTO:
			{
				int nNewLine=Index1;
				int n=aTryList.GetSize()-1;
				if(n>=0)
				{
					int nLineStart=aTryList[n].x;
					int nLineEnd=aTryList[n].y;
					if(nNewLine>=nLineEnd||nNewLine<=nLineStart)
					{
						aTryList.SetSize(n);//����� �� ���� ���������  try..catch
					}
				}
				nCodeLine = nNewLine-1;//�.�. ����� ������� 1
				break;
			}
		case OPER_TRY:
			{
				int nGotoCatch=Index1;//������� ��� ������
				aTryList.Add(CPoint(nCodeLine,nGotoCatch));
				break;
			}
		case OPER_RAISE:
			{
				//AfxPopBackErrorByteCode();//������������ ����� � ���������� ������ (���� �� ����)
				//AfxEndByteCode();
				Error(sCurError);
				break;
			}
		case OPER_RET:
			if(Index1!=DEF_VAR_NORET)
				CopyValue(cRetValue,Variable1);
		case OPER_ENDFUNC:
		case OPER_END:
			{
				nCodeLine=nFinish;//�����
				if(bDoLoop)
				if(nCurrentNumberStopContext>=AfxGetCountRunContext())
					bDebugStopLine=1;
				break;
			}
		case OPER_FUNC:
			if(nDelta==1)//��� ��������� ������ - ���������� ���� �������� � �������
			while(nCodeLine<nFinish)
			{
				if (CurCode.nOper!=OPER_ENDFUNC)
					nCodeLine++;
				else
					break;
			}
			break;

		case OPER_SET_TYPE:
			Variable1.nType=Array2;
			break;
		//��������� ������ � ��������������� �������
		//�����
		case OPER_ADD + TYPE_DELTA1:
			Variable1.fData=Variable2.fData+Variable3.fData;
			break;
		case OPER_SUB + TYPE_DELTA1:
			Variable1.fData=Variable2.fData-Variable3.fData;
			break;
		case OPER_DIV + TYPE_DELTA1:
			if(Variable3.fData==0)
				Error("������� �� ����");
			Variable1.fData=Variable2.fData/Variable3.fData;
			break;
		case OPER_MOD + TYPE_DELTA1:
			if(Variable3.fData==0)
				Error("������� �� ����");
			Variable1.fData=(int)Variable2.fData%(int)Variable3.fData;
			break;
		case OPER_MULT + TYPE_DELTA1:
			Variable1.fData=Variable2.fData*Variable3.fData;
			break;
		case OPER_LET + TYPE_DELTA1:
			Variable1.fData=Variable2.fData;
			break;

		case OPER_NOT + TYPE_DELTA1:
			Variable1.fData=!Variable2.fData;
			break;
		case OPER_INVERT + TYPE_DELTA1:
			Variable1.fData=-Variable2.fData;
			break;

		case OPER_EQ + TYPE_DELTA1:
			Variable1.fData=(Variable2.fData==Variable3.fData);
			break;
		case OPER_NE + TYPE_DELTA1:
			Variable1.fData=(Variable2.fData!=Variable3.fData);
			break;
		case OPER_GT + TYPE_DELTA1:
			Variable1.fData=(Variable2.fData>Variable3.fData);
			break;
		case OPER_LS + TYPE_DELTA1:
			{
#ifdef _DEBUG
				CValue *pVariable1=&Variable1;
				CValue *pVariable2=&Variable2;
				CValue *pVariable3=&Variable3;
#endif
			Variable1.fData=(Variable2.fData<Variable3.fData);
			}
			break;
		case OPER_GE + TYPE_DELTA1:
			Variable1.fData=(Variable2.fData>=Variable3.fData);
			break;
		case OPER_LE + TYPE_DELTA1:
			Variable1.fData=(Variable2.fData<=Variable3.fData);
			break;
		case OPER_SET_ARRAY + TYPE_DELTA1://��������� �������� �������
			{
				SetArrayValue(Variable1,int(Variable2.fData),GetValue(Variable3));
				break;
			}
		case OPER_GET_ARRAY + TYPE_DELTA1://��������� �������� �������
				GetArrayValue(Variable1,Variable2,int(Variable3.fData));
				break;

		case OPER_IF + TYPE_DELTA1:
			if(!Variable1.fData)
				nCodeLine=Index2-1;
			break;



		//������
		case OPER_ADD + TYPE_DELTA2:
			Variable1.sData=Variable2.sData+Variable3.sData;
			break;
		case OPER_SUB + TYPE_DELTA2:
			Variable1.sData=Variable2.sData-Variable3.sData;
			break;
		case OPER_LET + TYPE_DELTA2:
			Variable1.sData=Variable2.sData;
			break;

		case OPER_EQ + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData==Variable3.sData);
			break;
		case OPER_NE + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData!=Variable3.sData);
			break;
		case OPER_GT + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData>Variable3.sData);
			break;
		case OPER_LS + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData<Variable3.sData);
			break;
		case OPER_GE + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData>=Variable3.sData);
			break;
		case OPER_LE + TYPE_DELTA2:
			Variable1.sData=(Variable2.sData<=Variable3.sData);
			break;
		case OPER_SET_ARRAY + TYPE_DELTA2://��������� �������� �������
			{
				SetArrayValue(Variable1,Variable2.sData,GetValue(Variable3));
				break;
			}
		case OPER_GET_ARRAY + TYPE_DELTA2://��������� �������� �������
				GetArrayValue(Variable1,Variable2,Variable3.sData);
				break;

		case OPER_IF + TYPE_DELTA2:
			if(Variable1.sData.IsEmpty())
				nCodeLine=Index2-1;
			break;



		//����
		case OPER_ADD + TYPE_DELTA3:
			Variable1.dData=Variable2.dData+Variable3.dData;
			break;
		case OPER_SUB + TYPE_DELTA3:
			Variable1.dData=Variable2.dData-Variable3.dData;
			break;
		case OPER_DIV + TYPE_DELTA3:
			if(Variable3.dData==0)
				Error("������� �� ����");
			Variable1.dData=Variable2.dData/Variable3.dData;
			break;
		case OPER_MOD + TYPE_DELTA3:
			if(Variable3.dData==0)
				Error("������� �� ����");
			Variable1.dData=(int)Variable2.dData%(int)Variable3.dData;
			break;
		case OPER_MULT + TYPE_DELTA3:
			Variable1.dData=Variable2.dData*Variable3.dData;
			break;
		case OPER_LET + TYPE_DELTA3:
			Variable1.dData=Variable2.dData;
			break;

		case OPER_NOT + TYPE_DELTA3:
			Variable1.dData=!Variable2.dData;
			break;
		case OPER_INVERT + TYPE_DELTA3:
			Variable1.dData=-Variable2.dData;
			break;

		case OPER_EQ + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData==Variable3.dData);
			break;
		case OPER_NE + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData!=Variable3.dData);
			break;
		case OPER_GT + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData>Variable3.dData);
			break;
		case OPER_LS + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData<Variable3.dData);
			break;
		case OPER_GE + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData>=Variable3.dData);
			break;
		case OPER_LE + TYPE_DELTA3:
			Variable1.dData=(Variable2.dData<=Variable3.dData);
			break;
		case OPER_SET_ARRAY + TYPE_DELTA3://��������� �������� �������
			{
				SetArrayValue(Variable1,Variable2.dData,GetValue(Variable3));
				break;
			}
		case OPER_GET_ARRAY + TYPE_DELTA3://��������� �������� �������
				GetArrayValue(Variable1,Variable2,Variable3.dData);
				break;

		case OPER_IF + TYPE_DELTA3:
			if(!Variable1.dData)
				nCodeLine=Index2-1;
			break;


/*
		default:
			{
				OutToDebug("��������� ������� %d",(int)CurCode.nOper);
				extern char *aOperNames[];
				CString Str=aOperNames[CurCode.nOper];
				OutToDebug(" : %s\n",Str);
			}
*/
		}
		nCodeLine++;
		nCountForEscape--;
		if(nCountForEscape<0)
		if(afxEventMessage==0)
		{
			nCountForEscape=COUNT_FOR_ESCAPE;
			MSG msg;
			//����� ������� ��� ���������
			while( ::PeekMessage( &msg, NULL, WM_COPYDATA, WM_COPYDATA, PM_NOREMOVE ) )
			{
				if( ! ::GetMessage( &msg, NULL, WM_COPYDATA, WM_COPYDATA ) )
					break;
				::DispatchMessage( &msg );
			}
			while( ::PeekMessage( &msg, NULL, WM_MESSAGE_FROM_2C, WM_MESSAGE_FROM_2C, PM_NOREMOVE ) )
			{
				if( ! ::GetMessage( &msg, NULL, WM_MESSAGE_FROM_2C, WM_MESSAGE_FROM_2C ) )
					break;
				::DispatchMessage( &msg );
			}

			static int nLastTimeUpdate=0;
			int nNewTime=GetTickCount();
			if(abs(nLastTimeUpdate-nNewTime)>300)//��� � 300 ��
			{
				nLastTimeUpdate=nNewTime;

				//��������� ����
				while( ::PeekMessage( &msg, NULL, WM_PAINT, WM_PAINT, PM_NOREMOVE ) )
				{
					if( ! ::GetMessage( &msg, NULL, WM_PAINT, WM_PAINT ) )
						break;
					::DispatchMessage( &msg );
				}
			}

			if(bStopRunProgram)
			{
				afxCurrentRunModule=0;
				return CValue();
			}


#ifndef SMALL_TRANSLATE
			if(pByteCode->csModuleName.Left(7)!=OBJECTSNAME)
#endif
			if(EscapePress())
			if(GetActiveWindow())
			{
				HWND hWnd=0;
				if(AfxGetMainWnd())
					hWnd=AfxGetMainWnd()->m_hWnd;

				int nRes=MessageBox(hWnd,"�������� ���������� ���������?","2�:�����������", MB_YESNO|MB_ICONSTOP);
				if(nRes==IDYES)
				{
					//sCurError="��������� ����������� �������������";
					afxErrorPlace.Reset();
					sCurError="";
					//AfxEndByteCode();
					throw (new CTranslateError());
					break;
				}
			}
		}
	}
	}
	catch(CTranslateError *err)
	{ 
		int n=aTryList.GetSize()-1;
		if(n>=0)
		{
			afxErrorPlace.Reset();//������ �������������� � ���� ������ - ������� ����� ������
			int nLine=aTryList[n].y;
			aTryList.SetSize(n);
			nCodeLine=nLine;
			goto Start;
		}

		//� ���� ������ ��� ����������� - ��������� ����� ������ ��� ��������� �������
		//�� ������ ����� �� ������, �.�. �� ����� ���� �� ������ �����������
		if(!afxErrorPlace.pByteCode)
		if(pByteCode!=afxErrorPlace.pSkipByteCode)//��������� ������� ������ ������ ���������� ������ ��� �������� �������
		{
			//����� ��������� ���������� ����� ������ (�.�. ������ ��������� �� � ���� ������)
			afxErrorPlace.pByteCode=pByteCode;
			afxErrorPlace.nLine=nCodeLine;
		}
		throw (err); 
	}
//*/
	return cRetValue;
}

//��������� nRunModule:
//0-�� ���������
//1-���������
CValue CProcUnit::Exec(CByteCode &ByteCode,int nRunModule)
{
	int i;
	if(!ByteCode.bCompile)
		Error(CString("������: ")+ByteCode.csModuleName+" �� �������������!");

	nRecCount=0;
	pByteCode=&ByteCode;

	//��������� ������������ ������� (����������������� � �����������)
	if(GetParent(0))
	{
		if(GetParent(0)->pByteCode!=pByteCode->pParent)
		{
			pByteCode=0;
			Error("��������� ������ - ���� ���������� (#1)\n������1:%s\n��������1:%s\n��������2:%s",ByteCode.csModuleName,ByteCode.pParent->csModuleName,GetParent(0)->pByteCode->csModuleName); 
		}
	}
	else
		if(pByteCode->pParent)
		{
			pByteCode=0;
			Error("��������� ������ - ���� ���������� (#2)\n������1:%s\n��������1:%s",ByteCode.csModuleName,ByteCode.pParent->csModuleName);
		}



	cCurContext.SetLocalCount(ByteCode.nVarCount);

	cCurContext.nStart=ByteCode.nStartModule;

	int nParentCount=GetParentCount();

	ppArrayCode=new CProcUnit*[nParentCount+1];
	ppArrayCode[0]=this;


	pppArrayList=new CValue**[nParentCount+2];
	pppArrayList[0]=cCurContext.pRefLocVars;
	pppArrayList[1]=cCurContext.pRefLocVars;//�������� � 1, �.�. 0 - �������� ��������� ��������

	for(i=0;i<nParentCount;i++)
	{
		CProcUnit *pCurUnit=GetParent(i);
		ppArrayCode[i+1]=pCurUnit;
		pppArrayList[i+2]=pCurUnit->cCurContext.pRefLocVars;
	}

	//��������� ������� ����������
	if(ByteCode.aExternValue.GetSize()>0)
	{
#ifndef SMALL_TRANSLATE
		//���� �������� ������ ������ - �� ��������� �������� ��� ������(�.�. ������ ���������� ����� ����������)
		if(GetParent())
		if(GetParent()->pCurrentObj) 
		{
			vParentObj=ValueByRef(GetParent()->pCurrentObj);
		}

		//������� ������� ���������� ��� �������� ������ ������
		vContext.bReadOnly=0;
		CValueContext *pValContext=new CValueContext();
		pValContext->Attach(this);
		vContext=ValueByRef(pValContext);
		ByteCode.aExternValue[0]=&vContext;
		vContext.bReadOnly=1;


		//��� ������� �������� ������� ���������� � �������� 1 - ��� ��������
		if(pByteCode->csModuleName.Right(ENTERPRISEMODULENAME.GetLength())==ENTERPRISEMODULENAME)
		{
			ByteCode.aExternValue[1]=&vParentObj;
			vContext.bReadOnly=1;
		}
#endif
	}
//	else
//		Error("��� ����������� ����������!");

	for(i=0;i<ByteCode.aExternValue.GetSize();i++)
	{
		if(ByteCode.aExternValue[i]) 
			cCurContext.pRefLocVars[i]=ByteCode.aExternValue[i];
	}

	int nDelta=1;
	//��������� ������������� ���������� ������
	int nFinish = pByteCode->CodeList.GetSize();
	CByte *pCodeList=pByteCode->CodeList.GetData();
	CValue **pRefLocVars=cCurContext.pRefLocVars;
	for(int nCodeLine=0;nCodeLine<nFinish;nCodeLine++)
	{
		CByte byte=pCodeList[nCodeLine];
		if(byte.nOper==OPER_SET_TYPE)
			Variable1.nType=Array2;
		else
		if(byte.nOper==OPER_SET_ARRAY_SIZE)
			SetSizeArray(LocVariable1,Array2);
		else
			break;

	}

	//��������� �� ������ ���������
	for(i=0;i<pByteCode->ConstList.GetSize();i++)
	{
		pByteCode->ConstList[i].bReadOnly=1;
	}

	if(nRunModule)
	{
		cCurContext.pCompileContext=&ByteCode.pModule->cContext;//�������� ��������� ����������
		return Exec(&cCurContext,nDelta);
	}

	return CValue();
}

//����� ������� � ������ �� �����
//bExportOnly=0-����� ����� ������� � ������� ������ + ���������� � ������������ �������
//bExportOnly=1-����� ���������� ������� � ������� � ������������ �������
//bExportOnly=2-����� ���������� ������� � ������ ������� ������
int CProcUnit::FindFunction(CString csName,BOOL bError,int bExportOnly)
{
	if(!pByteCode)
		Error("������ �� �������������!");
	if(!pByteCode->bCompile)
		Error("������ �� �������������!!");
	int nCodeLine;
	if(bExportOnly)
	{
		nCodeLine=(int)pByteCode->ExportFuncList[mUpper(csName)]-1;
	}
	else
	{
		nCodeLine=(int)pByteCode->FuncList[mUpper(csName)]-1;
	}
	if(bError&&nCodeLine<0)
	{
		CString Str;
		Str.Format("��������� ��� ������� \"%s\" �� �������",csName);
		Error(Str.GetBuffer(0));
	}
	if(nCodeLine>=0)
	{
		return nCodeLine;
	}
	if(GetParent()&&bExportOnly<=1)
	{
		int nCodeSize=pByteCode->CodeList.GetSize();
		nCodeLine=(int)GetParent()->FindFunction(csName,0,1);
		if(nCodeLine>=0)
			return nCodeSize+nCodeLine;
	}
	return -1;
}

//����� ���������� �������
int CProcUnit::FindExportFunction(CString csName)
{
	return FindFunction(csName,0,2);

}

//����� ������� �� �� ������ � ������� ���� �����
//����� �������������� � �.�. � � ������������ ������
CValue CProcUnit::CallFunction(int nCodeLine,CValue **ppParams,int nReceiveParamCount)
{
	if(!pByteCode)
		Error("������ �� �������������!");
	if(!pByteCode->bCompile)
		Error("������ �� �������������!!");

	int nCodeSize=pByteCode->CodeList.GetSize();
	if(nCodeLine>=nCodeSize)
	{
		if(GetParent())
			return GetParent()->CallFunction(nCodeLine-nCodeSize,ppParams);
		Error("������ ������ ������� ������");
	}

	CByte *pCodeList=pByteCode->CodeList.GetData();
	CRunContext cRunContext(Index3);//����� ��������� ����������
	cRunContext.nParamCount=Array3;//����� ���������� ����������
	cRunContext.nStart=nCodeLine;
	cRunContext.pCompileContext=(CCompileContext *)pCodeList[cRunContext.nStart].Param1.nArray;//�������� ��������� ����������

	//��������� ���������
	memcpy(&cRunContext.pRefLocVars[0],&ppParams[0],min(nReceiveParamCount,cRunContext.nParamCount)*sizeof(CValue*));
//	for(int i=0;i<min(nReceiveParamCount,cRunContext.nParamCount);i++)
//		cRunContext.pRefLocVars[i]=ppParams[i];
	return Exec(&cRunContext,0);
}

//����� ������� �� ������
//����� �������������� ������ � ������� ������
CValue CProcUnit::CallFunction(CString csName,CValue **ppParams,int nParamCount)
{
	if(pByteCode)
	{
		int nCodeLine=(int)pByteCode->FuncList[mUpper(csName)]-1;
		if(nCodeLine>=0)
		{
			return CallFunction(nCodeLine,ppParams,nParamCount);
		}
	}
	return CValue();
}
/*
CValue CProcUnit::CallFunction(CString csName,CValue **ppParams,int nParamCount)
{
	if(!pByteCode)
		Error("������ �� �������������!");
	if(!pByteCode->bCompile)
		Error("������ �� �������������!!");

	int nCodeLine=(int)pByteCode->FuncList[mUpper(csName)];
	if(nCodeLine>0)
	{
		nCodeLine--;
		CByte *pCodeList=pByteCode->CodeList.GetData();
		CRunContext cRunContext(Index3);//����� ��������� ����������
		cRunContext.nParamCount=Array3;//����� ���������� ����������
		cRunContext.nStart=nCodeLine;
		cRunContext.pCompileContext=(CCompileContext *)pCodeList[cRunContext.nStart].Param1.nArray;//�������� ��������� ����������

		//��������� ���������
		//memcpy(&cRunContext.pRefLocVars[0],&ppParams[0],min(nParamCount,cRunContext.nParamCount)*sizeof(CValue*));
		for(int i=0;i<min(nParamCount,cRunContext.nParamCount);i++)
			cRunContext.pRefLocVars[i]=ppParams[i];
			//CopyValue(cRunContext.pLocVars[i],*ppParams[i]);

#ifndef SMALL_TRANSLATE
		if(pByteCode->csModuleName.Left(7)!=OBJECTSNAME)
#endif
		if(bStopRunProgram==0&&afxEventMessage==0)
		{
			extern int nWaitCursor;
			if(nWaitCursor==0)
			{
				nWaitCursor=1;
				SetStatus("����������� ���������");
				AfxGetApp()->BeginWaitCursor();
			}
		}

		return Exec(&cRunContext,0);
	}
	return CValue();
}
*/
CValue CProcUnit::CallFunction(CString csName)
{
	return CallFunction(csName,0,0);
}

CValue CProcUnit::CallFunction(CString csName,CValue &vParam1)
{
	CValue *ppParams[1];
	ppParams[0]=&vParam1;
	CValue Ret=CallFunction(csName,ppParams,1);
	return Ret;
}
CValue CProcUnit::CallFunction(CString csName,CValue &vParam1,CValue &vParam2)
{
	CValue *ppParams[2];
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	CValue Ret=CallFunction(csName,ppParams,2);
	return Ret;
}

CValue CProcUnit::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3)
{
	CValue *ppParams[3];
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	CValue Ret=CallFunction(csName,ppParams,3);
	return Ret;
}
CValue CProcUnit::CallFunction(CString csName,CValue &vParam1,CValue &vParam2,CValue &vParam3,CValue &vParam4,CValue &vParam5,CValue &vParam6,CValue &vParam7)
{
	CValue *ppParams[7];
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	ppParams[2]=&vParam3;
	ppParams[3]=&vParam4;
	ppParams[4]=&vParam5;
	ppParams[5]=&vParam6;
	ppParams[6]=&vParam7;
	CValue Ret=CallFunction(csName,ppParams,7);
	return Ret;
}

CValue CProcUnit::CallFunction(int nCodeLine,CValue &vParam1)
{
	CValue *ppParams[1];
	ppParams[0]=&vParam1;
	CValue Ret=CallFunction(nCodeLine,ppParams);
	return Ret;
}
CValue CProcUnit::CallFunction(int nCodeLine,CValue &vParam1,CValue &vParam2)
{
	CValue *ppParams[2];
	ppParams[0]=&vParam1;
	ppParams[1]=&vParam2;
	CValue Ret=CallFunction(nCodeLine,ppParams);
	return Ret;
}

void CProcUnit::SetAttribute(int iName,CValue &Val)//��������� ��������
{	
	*cCurContext.pRefLocVars[iName]=Val;
}
void CProcUnit::SetAttribute(CString csName,CValue &Val)//��������� ��������
{	
	int iName=FindAttribute(csName);
	if(iName==-1)
	{
		iName=cCurContext.GetLocalCount();
		cCurContext.SetLocalCount(iName+1);
		cCurContext.cLocVars[iName]=CValue(csName);
		*cCurContext.pRefLocVars[iName]=Val;
	}
	else
	{
		*cCurContext.pRefLocVars[iName]=Val;
	}
}
CValue CProcUnit::GetAttribute(int iName)//�������� ��������
{
	return *cCurContext.pRefLocVars[iName];
}
int  CProcUnit::FindAttribute(CString csName)
{
	void *Res;
	if(pByteCode->ExportVarList.Lookup(csName,Res))
		return ((int)Res)-1;
	else
		return -1;
}

void DoError2();

CValue CProcUnit::Eval(CString StrRun,CRunContext *pRunContext,int bCompileBlock)
{
	CValue cRetValue;

	if(pRunContext==0)
		pRunContext=AfxGetCurrentRunContext();
	if(StrRun.IsEmpty()||!pRunContext)
		return CValue();

	afxStrErrorMessage="";

	CCompileModule *pModule=0;
	CProcUnit *pRun=(CProcUnit*)pRunContext->aEvalString[StrRun];
	if(!pRun)//��� �� ���� ���������� ������ ������
	{
		pRun=new CProcUnit();
		pModule=new CCompileModule ();
		if(pRun->CompileExpr(StrRun,pRunContext,*pModule,bCompileBlock)==0)
		{//�������
			delete pRun;
			delete pModule;
			return String(afxStrErrorMessage);;
		}
		//��� ��
		pRunContext->aEvalString[StrRun]=pRun;
		pRun->pByteCode->pModule=pModule;
	}
	pModule=pRun->pByteCode->pModule;

	//���������
	afxSimpleErrorMessage=1;
	try
	{
		cRetValue=pRun->Exec(&pRun->cCurContext,0);
	}
	catch(CTranslateError *)
	{
		DoError2();
		afxSimpleErrorMessage=0;
		cRetValue=String(afxStrErrorMessage);
	}
	afxSimpleErrorMessage=0;
	return cRetValue;

}

int CProcUnit::CompileExpr(CString StrRun,CRunContext *pRunContext,CCompileModule &cModule,int bCompileBlock)
{
	CByteCode *	pByteCode=&pRunContext->pCompileContext->pModule->cByteCode;

	cModule.Load(StrRun);
	//������ � �������� ��������� �������� ������ ���������
	cModule.cByteCode.pParent	=pByteCode;
	cModule.pParent				=pByteCode->pModule;
	cModule.cContext.pParent	=pRunContext->pCompileContext;


	cModule.bExpressionOnly=1;
	cModule.cContext.nFindLocalInParent=2;
	cModule.nCurrentCompile=-1;
	cModule.PrepareLexem();

	//������������ ������ ����-����� ��� ����������� ���������� ���������
	CByte code;
	code.nOper=OPER_RET;
	try
	{
		if(bCompileBlock)
			cModule.CompileBlock();
		else
			code.Param1=cModule.GetExpression();
	}
	catch(CTranslateError *)
	{
		DoError2();
		return 0;
	}

	if(!bCompileBlock)
		cModule.cByteCode.CodeList.Add(code);

	CByte code2;
	code2.nOper=OPER_END;
	cModule.cByteCode.CodeList.Add(code2);
	cModule.cByteCode.nVarCount=cModule.cContext.cVariables.GetCount();

	//������� ������������� ��������������
	cModule.cByteCode.bCompile=1;

	//cModule.ToDebug();


	//���������� � ������
	SetParent(pRunContext->pProcUnit);

	try
	{
		Exec(cModule.cByteCode,0);
		cCurContext.pCompileContext=&cModule.cByteCode.pModule->cContext;//�������� ��������� ����������
		pppArrayList[1]=pRunContext->pRefLocVars;

	}
	catch(CTranslateError *)
	{
		DoError2();
		return 0;
	}
	return 1;
}

void DoError2()
{
	//OutToDebug("������ ���������: %s\n",sCurError);
	nCountForEscape=COUNT_FOR_ESCAPE;
	if(!afxMessageFromConfig)
	{
		DoError();
		Sleep(2);
		//���������� ����
		MSG msg;
		while( ::PeekMessage( &msg, NULL, 0, 0, PM_NOREMOVE ) )
		{
			if( ! ::GetMessage( &msg, NULL, 0, 0 ) )
				break;
			::DispatchMessage( &msg );
		}

		if(bStopDebug==-1)
		{
			bStopDebug=0;
			throw(new CTranslateError());
		}
	}
	afxMessageFromConfig=0;
	afxSimpleErrorMessage=0;
}

CRunContext::~CRunContext()
{
	if(nVarCount>MAX_STATIC_VAR)
	{
		delete []pLocVars;
		delete []pRefLocVars;
	}

	//�������� aEvalString
	if(aEvalString.GetCount())
	{
		POSITION pos;
		void *p;
		CString csKey;
		for( pos = aEvalString.GetStartPosition(); pos != NULL; )
		{
			aEvalString.GetNextAssoc( pos, csKey, p);
			if(p)
			{
				CProcUnit *pRun=(CProcUnit*)p;
				CCompileModule *pModule=pRun->pByteCode->pModule;
				delete pRun;
				delete pModule;

			}
		}
	}
}

