//Авторские права - VTOOLS.RU (info@vtools.ru)
// DocList.cpp: implementation of the CDocList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "DocList.h"
#include "MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDocList::~CDocList()
{
	CloseWindows();
	aList.RemoveAll();
};


BOOL CDocList::IsModified()
{
	POSITION pos;
	CString key; 
	CDocument* pa;
	for( pos = aList.GetStartPosition(); pos != NULL; )
	{
		aList.GetNextAssoc( pos, key, (void*&)pa);
		if(pa)
		{
			if(pa->IsModified())
				return TRUE;
		}
	}
	return FALSE;
};
void CDocList::SaveModified()
{
	POSITION pos;
	CString key; 
	CDocument* pa;
	for( pos = aList.GetStartPosition(); pos != NULL; )
	{
		aList.GetNextAssoc( pos, key, (void*&)pa);
		if(pa)
		{
			pa->SaveModified();
		}
	}
};

void CDocList::CloseWindows()
{
	POSITION pos;
	CString key;
	CDocument* pa;
	for( pos = aList.GetStartPosition(); pos != NULL; )
	{
		aList.GetNextAssoc( pos, key, (void*&)pa);
		if(pa)
		{
			//pa->SetTree(0);
			//pa->CloseObj();
			pa->OnCloseDocument();
		}
	}
};
