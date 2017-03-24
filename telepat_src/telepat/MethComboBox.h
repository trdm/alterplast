#if !defined(AFX_METHCOMBOBOX_H__4ED44EFC_DBA1_4496_8379_02AF3458B913__INCLUDED_)
#define AFX_METHCOMBOBOX_H__4ED44EFC_DBA1_4496_8379_02AF3458B913__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MethComboBox.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMethComboBox window
#include "TeleList.h"
#include "hintwnd.h"

class CMyToolBarMgr:public SECToolBarManager
{
public:
	static void Init();
	void SetBtnMapEx(SECBtnMapEntry* pMap);
};

class SECComboBtnEdit;
class CMethComboBox : public CComboBox, public SECWndBtn
{
// Construction
public:
	CMethComboBox();
	SECComboBtnEdit* m_pEdit;
	CPtrArray m_array;
	CHintWnd* m_hint;
	HWND m_hList;
	bool m_enabled;
	int m_lastSel;
	int m_line;
	static int m_lastMethListLine;
	static CTxtTrap* m_pEditor;
	static CMapPtrToWord m_boxes;

// Attributes
public:

// Operations
public:
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnLButtonUp(UINT nFlags, CPoint point);
	void OnLButtonDblClk(UINT nFlags, CPoint point);
	void OnRButtonDown(UINT nFlags, CPoint point);
	void OnRButtonUp(UINT nFlags, CPoint point);
	void OnRButtonDblClk(UINT nFlags, CPoint point);
	void OnPaint();
	BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	static SECButtonClass classMyBtn;
	virtual SECButtonClass *  GetButtonClass(void)const
	{
		return &classMyBtn;
	}
	static class SECStdBtn * __stdcall CreateButton(void)
	{
		return new CMethComboBox;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMethComboBox)
	public:
	virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
	//}}AFX_VIRTUAL

// Implementation
public:
	static void UpdateList(CTxtTrap* pEditor);
	virtual ~CMethComboBox();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMethComboBox)
	afx_msg void OnDropdown();
	afx_msg void OnSelendok();
	afx_msg void OnCloseup();
	afx_msg void OnEditchange();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnSelchange();
	afx_msg void OnDestroy();
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	CWnd* GetWnd(){return (CWnd*)this;}
	virtual BOOL CreateWnd(CWnd* pParentWnd, DWORD dwStyle, CRect& rect, int nID);
};

class SECComboBtnEdit : public CEdit
{
// Construction
public:
	SECComboBtnEdit();

// Implementation
public:
	virtual ~SECComboBtnEdit() {}

public:
	// Implementation, data members.
	CMethComboBox* m_pCombo;				// Parent combobox

protected:
	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	void OnSetFocus(CWnd* pOldWnd);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_METHCOMBOBOX_H__4ED44EFC_DBA1_4496_8379_02AF3458B913__INCLUDED_)
