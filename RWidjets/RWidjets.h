// RWidjets.h : main header file for the RWidjets DLL
//

#if !defined(AFX_RWidjets_H__58E96795_F2E6_11D5_AE41_0002B31F6532__INCLUDED_)
#define AFX_RWidjets_H__58E96795_F2E6_11D5_AE41_0002B31F6532__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "RWidjets_i.h"

/////////////////////////////////////////////////////////////////////////////
// CRWidjetsApp
// See RWidjets.cpp for the implementation of this class
//

class CRWidjetsApp : public CWinApp
{
public:
	CRWidjetsApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CRWidjetsApp)
		virtual BOOL InitInstance();
		virtual int ExitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CRWidjetsApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	BOOL InitATL();
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RWidjets_H__58E96795_F2E6_11D5_AE41_0002B31F6532__INCLUDED_)
