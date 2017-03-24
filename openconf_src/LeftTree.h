#if !defined(AFX_LEFTTREE_H__D3A5B160_1E29_41AA_B30A_27CF733DC76D__INCLUDED_)
#define AFX_LEFTTREE_H__D3A5B160_1E29_41AA_B30A_27CF733DC76D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// LeftTree.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CLeftTree window
class CTreeItemInfo;
class CScripting;

class CLeftTree : public CTreeCtrl
{
// Construction
public:
	CLeftTree();
	CPoint m_click;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CLeftTree)
	//}}AFX_VIRTUAL

// Implementation
public:
	void InsertPlugin(void* pInfo);
	void RemoveScript(CScripting* pScript);
	void InsertScript(CScripting* pScript);
	void RemoveChilds(CTreeItemInfo* pInfo);
	virtual ~CLeftTree();

	// Generated message map functions
protected:
	//{{AFX_MSG(CLeftTree)
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LEFTTREE_H__D3A5B160_1E29_41AA_B30A_27CF733DC76D__INCLUDED_)
