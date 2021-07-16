// MicroForm.cpp: implementation of the CMicroForm class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "MicroForm.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CMicroForm, CDialogUnit)

BEGIN_MESSAGE_MAP(CMicroForm, CDialogUnit)
	//{{AFX_MSG_MAP(CMicroForm)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void CMicroForm::OnPaint() 
{
	if (!IsIconic())
	if(m_bConfigMode)
	{
		CPaintDC dc(this); // device context for painting
		CRect Rect;
		GetClientRect(&Rect);
		dc.Draw3dRect(Rect,100,100);
	}
	else
		CDialogUnit::OnPaint();

}
