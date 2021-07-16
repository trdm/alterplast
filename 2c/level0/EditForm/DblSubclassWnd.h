// DblSubclassWnd.h: interface for the CDblSubclassWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DBLSUBCLASSWND_H__EFFF068B_6C83_4F09_A202_59A90DDB4B0C__INCLUDED_)
#define AFX_DBLSUBCLASSWND_H__EFFF068B_6C83_4F09_A202_59A90DDB4B0C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

///////////////////////////////////
// Written By Ralph Varjabedian  //
// 08/02/2002                    //
// software@sandmik.net          //
///////////////////////////////////

typedef LRESULT CALLBACK DBLSUBCLASS_WNDPROC(CWnd*, HWND, UINT, WPARAM, LPARAM, bool &);
typedef bool CALLBACK DBLSUBCLASS_RECURSIVECALLBACKPROC(HWND);

class CDblSubclassWnd  
{
#ifdef _DEBUG
private:
	CDblSubclassWnd() {}	// do not create instances of this class, use statically
#endif

public:
	static void SetDblSubclassWndProc(DBLSUBCLASS_WNDPROC *pPREPointer = NULL, DBLSUBCLASS_WNDPROC *pPOSTPointer = NULL);
	static BOOL SubclassDlgItem(UINT nID, CWnd* pParent);
	static BOOL SubclassWindow(HWND hWnd);
	static void UnSubclassWindow(HWND hWnd);
	static void UnSubclassChildsRecurs(HWND hWnd);
	static void SubclassChildsRecurs(HWND hWnd, DBLSUBCLASS_RECURSIVECALLBACKPROC *pFunc);
private:
	static DBLSUBCLASS_WNDPROC *m_pPREDblSubclassWndProc;
	static DBLSUBCLASS_WNDPROC *m_pPOSTDblSubclassWndProc;

	friend LRESULT AFXAPI OurCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg,
		WPARAM wParam, LPARAM lParam);
};

/////////////////////////////////////////////////////////////////////////////
// _CTempWndSubclass window

class _CTempWndSubclass : public CWnd
{
// Construction
public:
	_CTempWndSubclass();
	static long _m_lObjectCount;

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(_CTempWndSubclass)
	public:
	protected:
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~_CTempWndSubclass();

	// Generated message map functions
protected:
	//{{AFX_MSG(_CTempWndSubclass)
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#endif // !defined(AFX_DBLSUBCLASSWND_H__EFFF068B_6C83_4F09_A202_59A90DDB4B0C__INCLUDED_)
