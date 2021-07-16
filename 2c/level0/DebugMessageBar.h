// Авторские права - VTOOLS.RU (info@vtools.ru)
// DebugMessageBar.h: interface for the CDebugMessageBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(_AFX_DEBUGMESSAGEBAR_H__4F343C1B_AB41_462E_AC0C_5B10E27744A7__INCLUDED_)
#define _AFX_DEBUGMESSAGEBAR_H__4F343C1B_AB41_462E_AC0C_5B10E27744A7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FListCtrl/FListCtrl.h"
#include "DropTarget/MyDropTarget.h"

class CDebugMessageBar  : public CExtWFF<CFListCtrl>
{
public:
	CDebugMessageBar();
	virtual ~CDebugMessageBar();

    CMyDropTarget m_droptarget;

	int iControl;

	void SetItem(HFROW hFRow);
	CString GetPath(HFROW hFRow);
	void SetItem(HFROW hFRow,CString csVariable);
	void Init();
	void Refresh();

	virtual void OnEvent(FL_NOTIFY &notify, HFROW hFRow, int iFColumn, int code);
	int OnSetText(WPARAM wParam, LPARAM lParam);
	afx_msg void OnDestroy();
	DECLARE_MESSAGE_MAP()
};

#endif
