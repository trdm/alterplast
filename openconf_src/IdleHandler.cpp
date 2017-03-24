// IdleHandler.cpp: implementation of the CIdleHandler class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IdleHandler.h"
#include "IConfig.h"

CTrapVirtual CIdleHandler::m_trap;
CMapPtrToPtr CIdleHandler::m_IdleFunc;

typedef BOOL (CWinApp::*POnIndle)(long);

BOOL CIdleHandler::OnIdleWrap(long count)
{
	void(*pFunc)();
	void* val;
	for(POSITION pos=m_IdleFunc.GetStartPosition();pos;)
	{
		m_IdleFunc.GetNextAssoc(pos,(void*&)pFunc,val);
		pFunc();
	}
	CIConfigEvents::m_ConfigEvents.FireOnIdle();
	return (this->*(POnIndle)m_trap.m_pRealAdress)(count);
}

void CIdleHandler::SetIdleHandler(void(*pFunc)())
{
	m_IdleFunc[(void*)pFunc]=(void*)1;
}

void CIdleHandler::RemoveIdleHandler(void(*pFunc)())
{
	m_IdleFunc.RemoveKey((void*)pFunc);
}
static void OnInitApp()
{
	CWnd* pWnd=AfxGetMainWnd();
	CPtrList* pList=CConfigSvcImpl::m_pTheService->GetHandlersList(CFG_ON_CREATE_MAIN_WND);
	POSITION pos=pList->GetHeadPosition();
	while(pos)
		(*(HANDLER_ON_CREATE_WND)pList->GetNext(pos))(pWnd);
	CIConfigEvents::m_ConfigEvents.FireAllPluginsInit();
	CIdleHandler::RemoveIdleHandler(OnInitApp);
}

void CIdleHandler::Init()
{
	m_trap.SetTrap(pMainApp,0x68/4,(AFX_PMSG)OnIdleWrap);
	SetIdleHandler(OnInitApp);
}

void CIdleHandler::Done()
{
	m_trap.UnTrap();
	m_IdleFunc.RemoveAll();
}
