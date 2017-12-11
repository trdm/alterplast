// MDSplitter.cpp : implementation file
//

#include "stdafx.h"
#include "stdafx.h"
#include "MDSplitter.h"
/*
#include "MDview.h"
#include "infopane.h"
*/

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMDSplitter

IMPLEMENT_DYNCREATE(CMDSplitter, CSplitterWnd)

CMDSplitter::CMDSplitter()
{
}

CMDSplitter::~CMDSplitter()
{
}


BEGIN_MESSAGE_MAP(CMDSplitter, CSplitterWnd)
	//{{AFX_MSG_MAP(CMDSplitter)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CMDSplitter message handlers

int CMDSplitter::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSplitterWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	return 0;
}

BOOL CMDSplitter::PreCreateWindow(CREATESTRUCT& cs) 
{
	BOOL ret=CSplitterWnd::PreCreateWindow(cs);
	cs.dwExStyle&=~WS_EX_CLIENTEDGE;	
	cs.style&=~WS_BORDER;
	cs.style&=~WS_OVERLAPPED;
	return ret;
}
