// StackBar.h: interface for the CStackBar class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STACKBAR_H__6488B963_FB2A_47CC_A397_A7818E99E209__INCLUDED_)
#define AFX_STACKBAR_H__6488B963_FB2A_47CC_A397_A7818E99E209__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "DebugMessageBar.h"

class CStackBar  : public CDebugMessageBar  
{
public:
	CStackBar();
	virtual ~CStackBar();


	void Refresh();
	void Init();
	DECLARE_MESSAGE_MAP()

	afx_msg void OnDestroy();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

};

#endif // !defined(AFX_STACKBAR_H__6488B963_FB2A_47CC_A397_A7818E99E209__INCLUDED_)
