#if !defined(AFX_RAISEEVENT_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_)
#define AFX_RAISEEVENT_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_

//CMapStringToPtr cModuleMap;

struct CProcInfoFormEx
{
	int num;
	int type;
	int numarg;
};

struct CModuleInfoFormEx
{
	CMapStringToPtr cProcMap;
};

BOOL _CallFromGlobalModules(CGroupContext *pCont, CBLModule *pMod, const char* nameProc0, const char* nameProc1, int argNum, CValue** vArray, bool bCallSysProc = false);
void _CallModuleProc(class CGetDoc7* pDoc, const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray, BOOL bCallGlobal = TRUE);
void CallModuleProc(const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray);

#endif // !defined(AFX_RAISEEVENT_H__91E838EB_5DBB_4F94_862A_BABB25E35A41__INCLUDED_)
