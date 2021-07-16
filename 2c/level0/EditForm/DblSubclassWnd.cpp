// DblSubclassWnd.cpp: implementation of the CDblSubclassWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "DblSubclassWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#if _MFC_VER >= 0x0700
	#pragma warning( disable : 4312 )
	#pragma warning( disable : 4311 )
#endif

DBLSUBCLASS_WNDPROC *CDblSubclassWnd::m_pPREDblSubclassWndProc = NULL;
DBLSUBCLASS_WNDPROC *CDblSubclassWnd::m_pPOSTDblSubclassWndProc = NULL;

#ifndef WM_QUERYAFXWNDPROC
	#define WM_QUERYAFXWNDPROC  0x0360  
#endif

LRESULT AFXAPI OurCallWndProc(CWnd* pWnd, HWND hWnd, UINT nMsg,
	WPARAM wParam, LPARAM lParam)
{
	if (CDblSubclassWnd::m_pPREDblSubclassWndProc)
	{
		bool bDoReturn = false;
		LRESULT lRet = CDblSubclassWnd::m_pPREDblSubclassWndProc(pWnd, hWnd, nMsg, wParam, lParam, bDoReturn);
		if (bDoReturn)
			return lRet;
	}
	LRESULT lRet = AfxCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
	if (CDblSubclassWnd::m_pPOSTDblSubclassWndProc)
	{
		bool bDoReturn = false;
		LRESULT lRet = CDblSubclassWnd::m_pPOSTDblSubclassWndProc(pWnd, hWnd, nMsg, wParam, lParam, bDoReturn);
		if (bDoReturn)
			return lRet;
	}
	return lRet;
}

LRESULT CALLBACK OurWndProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{
	// special message which identifies the window as using AfxWndProc
	if (nMsg == WM_QUERYAFXWNDPROC)
		return 1;

	// all other messages route through message map
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd);

	ASSERT(pWnd != NULL);
	ASSERT(pWnd->m_hWnd == hWnd);

	return OurCallWndProc(pWnd, hWnd, nMsg, wParam, lParam);
}

BOOL CDblSubclassWnd::SubclassDlgItem(UINT nID, CWnd* pParent)
{
	ASSERT(pParent != NULL);
	ASSERT(::IsWindow(pParent->m_hWnd));

	// check for normal dialog control first
	HWND hWndControl = ::GetDlgItem(pParent->m_hWnd, nID);
	if (hWndControl != NULL)
		return SubclassWindow(hWndControl);

	return FALSE;   // control not found
}

BOOL CDblSubclassWnd::SubclassWindow(HWND hWnd)
{
	CWnd* pWnd = CWnd::FromHandlePermanent(hWnd); // if not subclassed once, let us subclass.
	if (!pWnd)
	{
		_CTempWndSubclass *pNewWnd = new _CTempWndSubclass;
		ASSERT(pNewWnd);
		VERIFY(pNewWnd->SubclassWindow(hWnd));

		pWnd = CWnd::FromHandlePermanent(hWnd);
		ASSERT(pWnd);
	}

	WNDPROC oldWndProc = (WNDPROC) ::GetWindowLong(hWnd, GWL_WNDPROC);
	if (oldWndProc == OurWndProc) // already subclassed by us
		return TRUE;	

	oldWndProc = (WNDPROC) ::SetWindowLong(hWnd, GWL_WNDPROC,
		(DWORD) OurWndProc);

	ASSERT(oldWndProc == AfxGetAfxWndProc());
	ASSERT(oldWndProc != OurWndProc);

	return TRUE;
}

void CDblSubclassWnd::UnSubclassWindow(HWND hWnd)
{
	ASSERT(::IsWindow(hWnd));
	WNDPROC WndProc = (WNDPROC) ::GetWindowLong(hWnd, GWL_WNDPROC);
	if (WndProc == OurWndProc) // subclassed by us
		SetWindowLong(hWnd, GWL_WNDPROC, (LONG) AfxGetAfxWndProc());
}

void CDblSubclassWnd::UnSubclassChildsRecurs(HWND hWnd)
{
//	return;
	if (!hWnd || !::IsWindow(hWnd))
		return;
	UnSubclassChildsRecurs(GetWindow(hWnd,GW_CHILD));
	UnSubclassChildsRecurs(GetWindow(hWnd,GW_HWNDNEXT));
	UnSubclassWindow(hWnd);
}

void CDblSubclassWnd::SubclassChildsRecurs(HWND hWnd, DBLSUBCLASS_RECURSIVECALLBACKPROC *pFunc)
{
//	return;
	if (!hWnd || !::IsWindow(hWnd))
		return;
	SubclassChildsRecurs(GetWindow(hWnd, GW_CHILD), pFunc);
	SubclassChildsRecurs(GetWindow(hWnd, GW_HWNDNEXT), pFunc);
	if (pFunc)
	{
		if (!pFunc(hWnd))
			return;
	}
	SubclassWindow(hWnd);
}

void CDblSubclassWnd::SetDblSubclassWndProc(DBLSUBCLASS_WNDPROC *pPREPointer, DBLSUBCLASS_WNDPROC *pPOSTPointer)
{
	m_pPREDblSubclassWndProc	= pPREPointer;
	m_pPOSTDblSubclassWndProc	= pPOSTPointer;
}

/////////////////////////////////////////////////////////////////////////////
// _CTempWndSubclass

long _CTempWndSubclass::_m_lObjectCount = 0;

_CTempWndSubclass::_CTempWndSubclass()
{
	InterlockedIncrement(&_m_lObjectCount);
}

_CTempWndSubclass::~_CTempWndSubclass()
{
	InterlockedDecrement(&_m_lObjectCount);
}

BEGIN_MESSAGE_MAP(_CTempWndSubclass, CWnd)
	//{{AFX_MSG_MAP(_CTempWndSubclass)
	ON_WM_NCDESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// _CTempWndSubclass message handlers

void _CTempWndSubclass::OnNcDestroy() 
{
	CWnd::OnNcDestroy();
	delete this; // auto delete the object
}

#if _MFC_VER >= 0x0700
	#pragma warning( error : 4312 )
	#pragma warning( error : 4311 )
#endif
