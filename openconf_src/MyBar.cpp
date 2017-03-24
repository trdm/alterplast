// MyBar.cpp : implementation file
#include "stdafx.h"
#include "config.h"
#include "MyBar.h"

/////////////////////////////////////////////////////////////////////////////
// CMyBar
AFX_MSGMAP* CMyBar::m_pBaseMap=NULL;
CRuntimeClass* CMyBar::m_pBaseClass=NULL;

CMyBar::CMyBar()
{
	if(!m_pBaseMap)
	{
		HINSTANCE hFrame=GetModuleHandle("frame.dll");
		m_pBaseMap=(AFX_MSGMAP*)GetProcAddress(hFrame,"?messageMap@SECControlBar@@1UAFX_MSGMAP@@B");
		m_pBaseClass=(CRuntimeClass*)GetProcAddress(hFrame,"?classSECControlBar@SECControlBar@@2UCRuntimeClass@@B");
		
	}
}

CMyBar::~CMyBar()
{
}

CRuntimeClass* PASCAL CMyBar::_GetBaseClass()
{
	return m_pBaseClass;
}
AFX_COMDAT const AFX_DATADEF CRuntimeClass CMyBar::classCMyBar= 
{
	"CMyBar", sizeof(class CMyBar), 0xFFFF,NULL,
	&CMyBar::_GetBaseClass, NULL 
};

CRuntimeClass* CMyBar::GetRuntimeClass() const
{
	return (CRuntimeClass*)&classCMyBar;
}


const AFX_MSGMAP* PASCAL CMyBar::_GetBaseMessageMap()
{
	return m_pBaseMap;
}

const AFX_MSGMAP* CMyBar::GetMessageMap() const
{
	return &CMyBar::messageMap;
}
AFX_COMDAT AFX_DATADEF const AFX_MSGMAP CMyBar::messageMap = 
	{ &CMyBar::_GetBaseMessageMap, &CMyBar::_messageEntries[0] };
AFX_COMDAT const AFX_MSGMAP_ENTRY CMyBar::_messageEntries[] = 
{ 
	//{{AFX_MSG_MAP(CMyBar)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CMyBar message handlers

int CMyBar::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (SECControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;
	CRect rc;
	GetClientRect(rc);
	//rc.DeflateRect(5,5,5,5);
	m_edit.Create(WS_VISIBLE|WS_CHILD,rc,this,1000);
	return 0;
}
