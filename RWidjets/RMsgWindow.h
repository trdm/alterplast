// RMsgWindow.h: interface for the CRMsgWindow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_RMSGWINDOW_H__10C2729F_CE6E_48E6_8277_90AF947EE811__INCLUDED_)
#define AFX_RMSGWINDOW_H__10C2729F_CE6E_48E6_8277_90AF947EE811__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include ".\\MyContext\\mycontextbase.h"
#include "DlgMsg.h"

class CRMsgWindow : public CMyContextBase
{
	DECLARE_DYNCREATE(CRMsgWindow);
public:
	CRMsgWindow();
	virtual ~CRMsgWindow();

	BOOL funcMessage(CValue& RetVal, CValue **params);
	BOOL funcClose(CValue& RetVal, CValue **params);
	BOOL funcMove(CValue& RetVal, CValue **params);

	RDECLARE_MY_CONTEXT;

	CDlgMsg * msg;
	HWND hwndText;
};

#endif // !defined(AFX_RMSGWINDOW_H__10C2729F_CE6E_48E6_8277_90AF947EE811__INCLUDED_)
