#if !defined(AFX_MYBAR_H__94C020E2_8DA1_11D8_9FCD_D5551308CA08__INCLUDED_)
#define AFX_MYBAR_H__94C020E2_8DA1_11D8_9FCD_D5551308CA08__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MyBar.h : header file
//

#define WM_EXTENDCONTEXTMENU		0x38ff	// replaces	(WM_USER+800) to avoid
											// conflict with help subsystem messages.
// Extended Styles
#define CBRS_EX_STDCONTEXTMENU		0x0001L  // Standard context menu items
#define CBRS_EX_STRETCH_ON_SIZE		0x0002L  // Control bar is stretched on resize
#define CBRS_EX_UNIDIRECTIONAL		0x0004L  // Bar can be sized in one direction
                                             //    at a time (i.e. toolbar)
#define CBRS_EX_DRAWBORDERS			0x0008L  // Draw a border around the bar
#define CBRS_EX_BORDERSPACE			0x0010L  // Leave border space for ease of dragging
#define CBRS_EX_ALLOW_MDI_FLOAT		0x0020L  // Control bar can be re-parented
											 //    by an MDI Child window
#define CBRS_EX_SIZE_TO_FIT			0x0040L  // Size the (single) child to fit
#define CBRS_EX_FULL_ROW			0x0080L	 // control bar will always occupy entire row

#define CBRS_EX_COOLBORDERS			0x0100L  // floating buttons, no border
#define CBRS_EX_GRIPPER				0x0200L	 // draw dragging "gripper"
#define CBRS_EX_GRIPPER_CLOSE		0x0400L	 // draw close button on gripper
#define CBRS_EX_GRIPPER_EXPAND		0x0800L	 // draw expand/contract button
#define CBRS_EX_TRANSPARENT			0x1000L	 // toolbar buttons drawn transparently
#define CBRS_EX_DISALLOW_FLOAT		0x2000L	 // Control bar can only be docked // not implemented
#define CBRS_EX_MENU_STYLE			0X4000L  // Do not become large unless a large object
											 // is dropped on bar.
#define CBRS_EX_COOL				CBRS_EX_COOLBORDERS | CBRS_EX_GRIPPER | \
									CBRS_EX_GRIPPER_CLOSE | CBRS_EX_GRIPPER_EXPAND

class CMyBar : public SECControlBar
{
// Construction
public:
	DECLARE_DYNAMIC(CMyBar)
	CMyBar();
	CEdit m_edit;
	static AFX_MSGMAP* m_pBaseMap;
	static CRuntimeClass* m_pBaseClass;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CMyBar)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CMyBar();

	// Generated message map functions
protected:
	//{{AFX_MSG(CMyBar)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MYBAR_H__94C020E2_8DA1_11D8_9FCD_D5551308CA08__INCLUDED_)
