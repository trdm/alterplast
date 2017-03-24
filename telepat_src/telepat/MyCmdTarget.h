#if !defined(AFX_MYCMDTARGET_H__CE1C0C29_92A8_4562_86D1_F05A42D8172B__INCLUDED_)
#define AFX_MYCMDTARGET_H__CE1C0C29_92A8_4562_86D1_F05A42D8172B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyCmdTarget.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CMyCmdTarget command target

class CMyCmdTarget : public CCmdTarget
{
// Attributes
public:
	CMyCmdTarget();
	static CMyCmdTarget* m_pTarget;


// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyCmdTarget)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CMyCmdTarget();

	// Generated message map functions
	//{{AFX_MSG(CMyCmdTarget)
	afx_msg void OnCodeJumpBack();
	afx_msg void OnUpdateCJBack(CCmdUI* pCmdUI);
	afx_msg void OnCodeJumpForward();
	afx_msg void OnUpdateCJForward(CCmdUI* pCmdUI);
	afx_msg void OnCjstack();
	afx_msg void OnUpdateCjstack(CCmdUI* pCmdUI);
	afx_msg void OnUpdateMethodList(CCmdUI* pCmdUI);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYCMDTARGET_H__CE1C0C29_92A8_4562_86D1_F05A42D8172B__INCLUDED_)
