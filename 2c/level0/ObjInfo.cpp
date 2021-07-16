// ObjInfo.cpp: implementation of the CObjInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "ObjInfo.h"
#include "MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
void SaveMetaTree(class CMetadataTree *pTree)
{
	pTree->GetDocument()->OnFileSave();
}

//****************************
void CObjInfo::OnCloseObj()
{
	if(GetTree())
	{
		GetTree()->RemoveFromList(GetObj(),GetObjName());
		SetTree(0);
	}
	else
	{
		if(bDeleteObj&&GetObj())
		{
			delete 	GetObj();
			SetObj(0);
		}
	}
};

//****************************
//Функция гарантированного получения объекта-описателя
CObjInfo *GetObjInfo(CDocument *pDoc)
{
	ASSERT(pDoc);
	CObjInfo *pThis=0;
	//CFileMeta *pFile=(CFileMeta *)
	pDoc->GetFile(0,0,(CFileException*)&pThis);
	return pThis;
}
CMetaObject *GetMeta(CDocument *pDoc)
{
	CObjInfo *p=GetObjInfo(pDoc);
	if(p)
		return p->GetObj();
	else
		return 0;
}
CMetadataTree *GetMetaTree(CDocument *pDoc)
{
	CObjInfo *p=GetObjInfo(pDoc);
	if(p)
		return p->GetTree();
	else
		return 0;
}

void ActivateObj(CDocument *pDoc)
{
	CObjInfo *p=GetObjInfo(pDoc);
	if(p)
		p->Activate();
}
