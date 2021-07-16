// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
#if !defined(AFX_MYDATAEXCHANGE_H__D07756E1_B382_11D5_B583_006052092467__INCLUDED_)
#define AFX_MYDATAEXCHANGE_H__D07756E1_B382_11D5_B583_006052092467__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyDataExchange.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CMyDataExchange dialog
enum
{
	MYMESSAGE_START=0,
	DDE_FINISH,
	MYMESSAGE_DATA,
	MYMESSAGE_FINISH,
	MYMESSAGE_ACTIVATE,

	SET_CUR_LINE,

	GET_DEBUG_POINT,

	ADD_DEBUG_POINT,
	REMOVE_ALL_DEBUG_POINT,
	NEXT_DEBUG,
	NEXT_DEBUG_OVER,
	RUN_DEBUG,

	PAUSE_DEBUG,
	STOP_DEBUG,


	GET_VARIABLE,
	SET_VARIABLE,

	SET_ATTRIBUTE_NAME,
	SET_ATTRIBUTE_TYPE,
	SET_ATTRIBUTE_VALUE,

	SET_STRING_ARRAY,
	GET_VAR_STRING_ARRAY,
	SET_VAR_STRING_ARRAY,

	STOP_PROGRAM,

	GET_STACK,
	SET_STACK,
	SET_STACK_ARRAY,
	SET_CURRENT_STACK,
};
#define MAX_LIST	0x40

class CMyDataExchange : public CDialog
{
// Construction
public:
	//virtual BOOL PreTranslateMessage(MSG* pMsg);
	CMyDataExchange(CWnd* pParent = NULL);   // standard constructor
	~CMyDataExchange();

// Dialog Data
	//{{AFX_DATA(CMyDataExchange)
	enum { IDD = IDD_MYDATAEXCHANGE_DIALOG };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyDataExchange)
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	protected:
	//}}AFX_VIRTUAL


// Implementation
protected:
	//CToolTipCtrl m_tooltip;

	// Generated message map functions
	//{{AFX_MSG(CMyDataExchange)
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);
	afx_msg int OnMessageFrom2C(WPARAM, LPARAM);
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYDATAEXCHANGE_H__D07756E1_B382_11D5_B583_006052092467__INCLUDED_)
