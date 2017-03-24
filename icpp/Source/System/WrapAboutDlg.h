#if !defined(AFX_WRAPABOUTDLG_H__2DE2DC9B_D287_4475_9207_80ECD8EB2858__INCLUDED_)
#define AFX_WRAPABOUTDLG_H__2DE2DC9B_D287_4475_9207_80ECD8EB2858__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifdef FORMEX_VK_DEFINED
	#include "artbear\OwnAboutWindowManager.h"
#else
	#include "OwnAboutWindowManager.h"
#endif


/////////////////////////////////////////////////////////////////////////////
// CWrapAboutDlg dialog

class CWrapAboutDlg
{
public:
	static void Init();

	static bool AddMyAboutWindowManager(IOwnAboutWindowManager* pMyAboutWindowManager);
};

class CWrapColumn
{
public:
	static void Wrap(CColumn* pColumn);
protected:
	static DWORD* m_pMyVTable;
	static DWORD* m_pRealVTable;
	virtual class CWnd *  Edit(class CRect & a_Rect,class CBrowse * a_pBrowse);
};

#endif // !defined(AFX_WRAPABOUTDLG_H__2DE2DC9B_D287_4475_9207_80ECD8EB2858__INCLUDED_)
