// MyCmdTarget.cpp : implementation file
#include "stdafx.h"
#include "resource.h"
#include "MyCmdTarget.h"
#include "parser.h"
#include "txttrap.h"
#include "codejump.h"

CMyCmdTarget* CMyCmdTarget::m_pTarget=NULL;

CMyCmdTarget::CMyCmdTarget()
{
	m_pTarget=this;
}

CMyCmdTarget::~CMyCmdTarget()
{
}


BEGIN_MESSAGE_MAP(CMyCmdTarget, CCmdTarget)
	//{{AFX_MSG_MAP(CMyCmdTarget)
	ON_COMMAND(ID_CJBACK, OnCodeJumpBack)
	ON_UPDATE_COMMAND_UI(ID_CJBACK, OnUpdateCJBack)
	ON_COMMAND(ID_CJFORWARD, OnCodeJumpForward)
	ON_UPDATE_COMMAND_UI(ID_CJFORWARD, OnUpdateCJForward)
	ON_COMMAND(ID_CJSTACK, OnCjstack)
	ON_UPDATE_COMMAND_UI(ID_CJSTACK, OnUpdateCjstack)
	ON_UPDATE_COMMAND_UI(ID_METHOD_LIST, OnUpdateMethodList)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMyCmdTarget message handlers

void CMyCmdTarget::OnCodeJumpBack() 
{
	CCodeJump::GetCodeJump()->Back();
}

void CMyCmdTarget::OnUpdateCJBack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CCodeJump::GetCodeJump()->m_CurPos>0);
}

void CMyCmdTarget::OnCodeJumpForward() 
{
	CCodeJump::GetCodeJump()->Forward();
}

void CMyCmdTarget::OnUpdateCJForward(CCmdUI* pCmdUI) 
{
	CCodeJump* pcj=CCodeJump::GetCodeJump();
	pCmdUI->Enable(pcj->m_CurPos<pcj->m_TopPos);
}

void CMyCmdTarget::OnCjstack() 
{
	CCodeJump::GetCodeJump()->ShowStack();
}

void CMyCmdTarget::OnUpdateCjstack(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CCodeJump::GetCodeJump()->m_TopPos>-1);
	
}

void CMyCmdTarget::OnUpdateMethodList(CCmdUI* pCmdUI) 
{
	pCmdUI->Enable(CTxtTrap::GetCurrentView()!=NULL);
}
