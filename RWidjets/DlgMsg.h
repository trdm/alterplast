// DlgMsg.h: interface for the CDlgMsg class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DLGMSG_H__801E5734_AD01_11D5_BD40_0002B31F6532__INCLUDED_)
#define AFX_DLGMSG_H__801E5734_AD01_11D5_BD40_0002B31F6532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"

class CDlgMsg : public CDialogImpl<CDlgMsg>  
{
public:
	CString m_message;
	enum { IDD = IDD_DlgMsg };
	BEGIN_MSG_MAP(CDlgMsg)
		COMMAND_ID_HANDLER(IDOK, OnOK)
		MESSAGE_HANDLER(WM_SIZE, OnSize)
	END_MSG_MAP()
	HRESULT OnOK(WORD, WORD, HWND, BOOL&);
	LRESULT OnSize(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
};

#endif // !defined(AFX_DLGMSG_H__801E5734_AD01_11D5_BD40_0002B31F6532__INCLUDED_)
