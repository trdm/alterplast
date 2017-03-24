#if !defined(AFX_TELELIST_H__A01B0ECA_B4BF_11D7_8D5E_B5A19D85DF00__INCLUDED_)
#define AFX_TELELIST_H__A01B0ECA_B4BF_11D7_8D5E_B5A19D85DF00__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// TeleList.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CTeleList window
class CToolTip;
#include "tentrydrawing.h"
#include "hintwnd.h"
#include "blmap.h"
class CTxtTrap;
class CTeleGroup;
class CTeleEntry;

class CTeleList : public CListBox
{
// Construction
public:
	CTeleList();
	bool Prepare(CTxtTrap* pEditor);
	CTxtTrap* m_pEditor;
	CToolTip* m_pShowText;
	CHintWnd* m_pHint;
	CPoint m_LastMousePoint;
	CRect m_ClientRect;
	DWORD m_timer;
	static CTeleList* m_pOneList;
	static void CALLBACK CheckView(HWND hwnd,UINT uMsg,UINT idEvent,DWORD dwTime);
// Attributes
public:
	CPoint m_CaretPos;
	CPoint m_CaretPoint;
	CRect  m_ParentRect;
	bool   m_UpDown;
	CString m_buf;
	int m_curPosInBuf;
	CPtrArray m_pArrayOfGroup;
	CPtrArray m_pArrayOfEntries;
	CStringArray m_HotString;
	CBLMap	m_mHotString;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTeleList)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	void SetBufer(CString txt);
	CPoint Insert(CTeleEntry* pEntry);
	void FilterList(CString str);
	void ResetContent();
	void AddTEntry(CTeleEntry* pEntry);
	void AddTGroup(CTeleGroup* pGroup);
	void Hide();
	void ShowList();
	void ShowToolTips(CPoint pt);
	void MakeSelect(UINT nChar=0);
	bool m_ProcessChar;
	virtual ~CTeleList();

	// Generated message map functions
//protected:
	//{{AFX_MSG(CTeleList)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSelchange();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TELELIST_H__A01B0ECA_B4BF_11D7_8D5E_B5A19D85DF00__INCLUDED_)
