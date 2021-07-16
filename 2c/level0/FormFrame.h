//Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_FORMFRAME_H__828A140B_5E46_4A28_99DA_9274D2EA3792__INCLUDED_)
#define AFX_FORMFRAME_H__828A140B_5E46_4A28_99DA_9274D2EA3792__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FormFrame.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFormFrame frame

class CFormFrame : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFormFrame)
protected:
	CFormFrame();           // protected constructor used by dynamic creation

// Attributes
public:
	class CFormUnit *pForm;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFormFrame)
	public:
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle = WS_CHILD | WS_VISIBLE | WS_OVERLAPPEDWINDOW, const RECT& rect = rectDefault, CMDIFrameWnd* pParentWnd = NULL, CCreateContext* pContext = NULL);
	virtual void ActivateFrame(int nCmdShow = -1);
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFormFrame();

	// Generated message map functions
	//{{AFX_MSG(CFormFrame)
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FORMFRAME_H__828A140B_5E46_4A28_99DA_9274D2EA3792__INCLUDED_)
