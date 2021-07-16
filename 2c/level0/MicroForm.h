// MicroForm.h: interface for the CMicroForm class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MICROFORM_H__3681559A_7E2F_446E_B0E7_257FE933F2BC__INCLUDED_)
#define AFX_MICROFORM_H__3681559A_7E2F_446E_B0E7_257FE933F2BC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DialogUnit.h"

class CMicroForm:public CDialogUnit
{
	DECLARE_DYNCREATE(CMicroForm)
public:
	CMicroForm()
	{
		m_bConfigMode=0;
	};

	BOOL m_bConfigMode;
	CString m_csObjName;

	// Generated message map functions
	//{{AFX_MSG(CMicroForm)
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_MICROFORM_H__3681559A_7E2F_446E_B0E7_257FE933F2BC__INCLUDED_)
