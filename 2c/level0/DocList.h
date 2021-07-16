// Авторские права - VTOOLS.RU (info@vtools.ru)
// DocList.h: interface for the CDocList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DOCLIST_H__8D5E2620_0E41_4F96_BCCD_4F1FECE4CDF9__INCLUDED_)
#define AFX_DOCLIST_H__8D5E2620_0E41_4F96_BCCD_4F1FECE4CDF9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ObjInfo.h"

class CMetaDocument : public CDocument,public CObjInfo
{  
	DECLARE_DYNCREATE(CMetaDocument)
public:
	DECLARE_FOR_OBJINFO(CDocument)

// Operations
	virtual void OnInitialUpdate()
	{
		POSITION pos = GetFirstViewPosition();
		while (pos != NULL)
		{
			CView* pView = GetNextView(pos);
			pView -> OnInitialUpdate();
		}
	} 
};

class CDocList
{
public:
	CDocList()
	{
	};
	~CDocList();
// Attributes
	CMapStringToPtr aList;

// Operations
	BOOL IsModified();
	void SaveModified();
	void CloseWindows();
};

#endif // !defined(AFX_DOCLIST_H__8D5E2620_0E41_4F96_BCCD_4F1FECE4CDF9__INCLUDED_)
