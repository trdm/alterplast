// DerivClsFolder.cpp: implementation of the CDerivClsFolder class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "DerivClsFolder.h"
#include "classinfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CDerivClsFolderInfo::~CDerivClsFolderInfo()
{

}
HTREEITEM CDerivClsFolderInfo::InsertInTree(HTREEITEM parent,HTREEITEM after)
{
	HTREEITEM item=NULL;
	CMapStringToOb& map=CMetaDataOfClasses::m_oneMetadata->m_DerivedClasses;
	CClassInfo* pParent=(CClassInfo*)GetData(parent);
	CStringList* list;
	if(map.Lookup(pParent->m_DisplayName,(CObject*&)list))
	{
		item=CInfo::InsertInTree(parent,TVI_FIRST);
		for(POSITION pos=list->GetHeadPosition();pos;)
		{
			CClassInfo* pClass=new CClassInfo(list->GetNext(pos));
			pClass->InsertInTree(item);
		}
	}
	else
		delete this;
	return item;
}
