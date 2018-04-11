#include "stdafx.h"
#include "RaiseEvent.h"

/////////////////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int GetProcNumFromModule(CBLModule *pMod, const char* nameProc0, const char* nameProc1, int& type, int& numarg)
{
	
	int r = pMod->FindProc(nameProc0,0);
	if(r<0)
	{
		r = pMod->FindProc(nameProc1,0);
		if(r<0)
		{
			r = pMod->FindFunc(nameProc0);
			if(r<0)
			{
				r = pMod->FindFunc(nameProc1);
			}
		}
	}
	if(r>=0)
	{
		CBLProcInfo cblProcInfo;
		if(pMod->GetProcInfo(r,cblProcInfo))
		{
			type = cblProcInfo.GetType();
			numarg = cblProcInfo.GetNParams();
		}
	}
	
	
	return r;
}

BOOL _CallFromGlobalModules(CGroupContext *pCont, CBLModule *pMod, const char* nameProc0, const char* nameProc1, int argNum, CValue** vArray, bool bCallSysProc)
{

	ULONG id = CBLModule::GetFirstLoadedModuleID(); //CBLModule7
	bool f = (id != 0);

	int r = -1;
	CBLModule7* pGlobalModule = GetGlobalModule();

	BOOL result = FALSE;
	int type;
	int numarg;

	while(f)
	{
		pMod = CBLModule::GetLoadedModule(id);
		if(pMod)
		{
			r = GetProcNumFromModule(pMod, nameProc0, nameProc1, type, numarg);
			if(r>=0)
			{
				CBLProcInfo blProcInfo;
				pMod->GetProcInfo(r, blProcInfo);

				if(pGlobalModule)
				if(pGlobalModule != pMod)
					((CBLModule7*)pMod)->SetStatusCode(pGlobalModule->GetStatusCode());

				int GLargNum = argNum;
				int s = 0;

				int t = type;
				if(bCallSysProc)
				{
					GLargNum = numarg;
					if(t == 1)
					{
						if(pMod->CallAsProc(r,GLargNum,vArray))
							result = TRUE;
					}
					else
					{
						CValue arValue;
						if(pMod->CallAsFunc(r,arValue,GLargNum,vArray))
							result = TRUE;
					}
				}
				else
				{
					if(numarg < GLargNum) GLargNum = numarg;
					if(t == 1)
					{
						if(pMod->CallAsProc(r,GLargNum,vArray))
							result = TRUE;
					}
					else
					{
						CValue arValue;
						if(pMod->CallAsFunc(r,arValue,GLargNum,vArray))
							result = TRUE;
					}
				}
				
				if(pGlobalModule)
				if(pGlobalModule->GetStatusCode())
					pGlobalModule->SetStatusCode(((CBLModule7*)pMod)->GetStatusCode());
			}
		}
		id = CBLModule::GetNextLoadedModuleID(id);
		f = (id != 0);
	}
	return result;
}

void _CallModuleProc(class CGetDoc7* pDoc, const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray, BOOL bCallGlobal)
{
	if(pDoc)
	{
		CGroupContext *pCont = pDoc->GetGroupContext();
		if(pCont)
		{
			CBLContext *pCont0;
			for(int i = 0; i<pCont->GetSize(); i++)
			{
				pCont0 = pCont->GetContext(i);
				if(!strcmp(pCont0->GetRuntimeClass()->m_lpszClassName,"CDocContext"))
					((CDocContext*)pCont0)->SetEnableNewLine(1);
			}
			CBLModule7 * pMod = pDoc->m_pBLModule;
			pCont->Accept();
			int r = -1;
			if(pMod)
			{
				int type;
				int numarg;
				r = GetProcNumFromModule(pMod, nameProc0, nameProc1, type, numarg);
				if(r>=0)
				{
					if(numarg < argNum) argNum = numarg;
					int t = type;

					pDoc->OnStartBatch();
					if(t == 1)
					{
						pMod->CallAsProc(r,argNum,vArray);
					}
					else
					{
						CValue arValue;
						pMod->CallAsFunc(r,arValue,argNum,vArray);
					}
					pDoc->OnEndBatch(0);
					CBLContext *pCont0;
					for(int i = 0; i<pCont->GetSize(); i++)
					{
						pCont0 = pCont->GetContext(i);
						if(!strcmp(pCont0->GetRuntimeClass()->m_lpszClassName,"CDocContext"))
							((CDocContext*)pCont0)->SetEnableNewLine(1);
					}
				}
			}
			if(bCallGlobal)
				_CallFromGlobalModules(pCont, NULL, nameProc0, nameProc1, argNum, vArray);
		}
		else if(bCallGlobal)
		{
			_CallFromGlobalModules(NULL, NULL, nameProc0, nameProc1, argNum, vArray);
		}
	}
	else if(bCallGlobal)
	{
		_CallFromGlobalModules(NULL, NULL, nameProc0, nameProc1, argNum, vArray);
	}
}

void CallModuleProc(const char* nameProc0,const  char* nameProc1, int argNum, CValue** vArray)
{
	CMDIFrameWnd * pMainFrame1 = GetMainFrame();
	CWnd* pActiveWnd = pMainFrame1->GetActiveWindow();
	CGetDoc7* pDoc7 = NULL;
	if(pMainFrame1->GetSafeHwnd() != pActiveWnd->GetSafeHwnd())
		pDoc7 = CGetDlg::GetTopmostDocunent();

	bool bNeedCallGlob = true;
	if(pDoc7)
	{
		CWnd* pWndFrame = pDoc7->GetFrame();
		_CallModuleProc(pDoc7,nameProc0,nameProc1,argNum,vArray);
		bNeedCallGlob = false;
	}
	else
	{
		CMDIFrameWnd * pMainFrame = GetMainFrame();
		if(pMainFrame)
		{
			CMDIChildWnd * pChild = pMainFrame->MDIGetActive();
			if(pChild)
			{
				CWnd* wndFocus = pMainFrame->GetFocus();
				if(wndFocus)
				{
					CFrameWnd * pParentFrame = wndFocus->GetParentFrame();
					if(pParentFrame && pParentFrame->GetSafeHwnd() == pChild->GetSafeHwnd())
					{
						CDocument* pDoc = pChild->GetActiveDocument();
						if(pDoc)
							if(!strcmp(pDoc->GetRuntimeClass()->m_pfnGetBaseClass()->m_lpszClassName,"CGetDoc7"))
							{
								_CallModuleProc((CGetDoc7*)pDoc,nameProc0,nameProc1,argNum,vArray);
								bNeedCallGlob = false;
							}
					}
				}
			}
		}
	}
	if(bNeedCallGlob)
	{
		_CallFromGlobalModules(NULL, NULL, nameProc0, nameProc1, argNum, vArray);
	}
}

