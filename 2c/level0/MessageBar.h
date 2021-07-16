// Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_MESSAGEBAR_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_)
#define AFX_MESSAGEBAR_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MessageBar.h : header file
//
//#include "secwb.h"
//#include "Sbartool.h"
#include "MyEdit.h"
#include "EditLog/EditLog.h"


extern class CMessageBar *pMessageWindow;//для вызова из языка

/////////////////////////////////////////////////////////////////////////////
// CMessageBar dialog

class CMessageBar : public CExtWFF<CMyEdit>
{
public:
	CEditLog	m_EditLogger;
	CMessageBar();

	void OnClear();
	void Message(CString Str);
	void Init();

	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);

	DECLARE_MESSAGE_MAP()
};
//*/
#endif // !defined(AFX_MESSAGEBAR_H__04E25A11_52FB_4BF2_9C93_4DB24511C746__INCLUDED_)
