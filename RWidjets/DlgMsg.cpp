// DlgMsg.cpp: implementation of the CDlgMsg class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DlgMsg.h"
//#include "1CHEADERS\\blang.h"

extern CBkEndUI * pBkEndUI;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

HRESULT CDlgMsg::OnOK(WORD, WORD, HWND, BOOL&)
{
	//EndDialog(0);
	CBLModule::GetExecutedModule()->ExecuteBatch("Предупреждение(РабочаяДата())", NULL);
	//CBLModule::GetExecutedModule()->RaiseExtRuntimeError("HELLO", 2);
	//SendMessage(GetActiveWindow(), 0x376, 0, 0);
	
	return 0;
}

LRESULT CDlgMsg::OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	RECT r;
	GetClientRect(&r);
	r.left	= r.left + 2;
	r.right	= r.right - 4;
	r.top		= r.top + 2;
	r.bottom	= r.bottom - 4;
	::MoveWindow(GetDlgItem(IDC_Text), r.left, r.top, r.right, r.bottom, TRUE);
	::SetWindowText(GetDlgItem(IDC_Text), m_message);
	return 0;
}
	