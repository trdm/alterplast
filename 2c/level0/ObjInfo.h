// ObjInfo.h: interface for the CObjInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_OBJINFO_H__C7744329_C50C_442A_B40D_5E72CA0FCCEF__INCLUDED_)
#define AFX_OBJINFO_H__C7744329_C50C_442A_B40D_5E72CA0FCCEF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

//****************************
#define OBJINFO CObjInfo //<int>
#include "FileMeta.h"

void SaveMetaTree(class CMetadataTree *pTree);


class CObjInfo 
{
public:
	CObjInfo()
	{
		m_MetaTree=afxMetaTree;
		m_MetaObj=afxMetaObj;
		bDeleteObj=0;
		This=0;
	};

private:
// Attributes
	class CMetadataTree *m_MetaTree;//текущее дерево метаданных
	class CMetaObject *m_MetaObj;	//текущий объект метаданных
	CString csObjTypeName;
protected:
	CDocument *This;
	BOOL bDeleteObj;
public:
// Operations
	void SetTree(class CMetadataTree *p)
	{
		m_MetaTree=p;
	};
	void SetObj(class CMetaObject *p)
	{
		m_MetaObj=p;
	};
	class CMetadataTree * GetTree()
	{
		return m_MetaTree;
	};
	class CMetaObject * GetObj()
	{
		return m_MetaObj;
	};
	void SetObjName(CString Str)
	{
		csObjTypeName=Str;
	};
	CString GetObjName()
	{
		return csObjTypeName;
	};


	CView *MyGetView()
	{
		ASSERT(This);
		POSITION pos = This->GetFirstViewPosition();
		return This->GetNextView(pos);
	};
	void Activate()
	{
		CView *pView=MyGetView();
		if(pView)
			if(pView->GetParentFrame())
				pView->GetParentFrame()->BringWindowToTop();
	};



	//OnCloseDocument
	virtual void OnCloseObj();
	//SaveModified
	virtual void OnSaveObj()
	{
		ASSERT(0);
	};

	//OnChangedViewList,OnUpdate
	virtual void OnUpdateObj()
	{
		ASSERT(0);
	};









#define DECLARE_FOR_OBJINFO(CBaseClass)\
	virtual CFile* GetFile(LPCTSTR str, UINT fl, CFileException* pEx)\
	{\
		This=this;\
		if(GetTree())\
		{\
			CObjInfo **p=(CObjInfo**)pEx;\
			*p=this;\
			CFileMeta *pFile=new CFileMeta();\
			pFile->SetObj(GetObj());\
			return (CFile*)pFile;\
		}\
		else\
		if(str!=NULL)\
		{\
				return CBaseClass::GetFile(str,fl,pEx);\
		}\
		return NULL;\
	}\
	void OnCloseDocument() \
	{\
		OnCloseObj();\
		CBaseClass::OnCloseDocument();\
	}\
	void SetModifiedFlag( BOOL bModified)\
	{\
		if(GetTree()&&bModified)\
			((CView*)GetTree())->GetDocument()->SetModifiedFlag(bModified);\
		else\
		{\
			CBaseClass::SetModifiedFlag(bModified);\
			CString Str=GetTitle();\
			Str.TrimRight('*');\
			if(bModified)\
				Str+="*";\
			SetTitle(Str);\
		}\
	}\
	afx_msg void OnFileSave() \
	{\
		if(GetTree())\
		{\
			SaveMetaTree(GetTree());\
		}\
		else\
		{\
			CString Str=GetTitle();\
			Str.TrimRight('*');\
			SetTitle(Str);\
			DoFileSave();\
			CBaseClass::SetModifiedFlag(0);\
			SetTitle(Str);\
		}\
	}\
	afx_msg void OnFileSaveAs() \
	{\
		if(GetTree())\
		{\
			OnCloseObj();\
		}\
		DoSave(NULL);\
	}\


};




//****************************
//Функция гарантированного получения объекта-описателя
CObjInfo *GetObjInfo(CDocument *pDoc);
CMetaObject *GetMeta(CDocument *pDoc);
CMetadataTree *GetMetaTree(CDocument *pDoc);
void ActivateObj(CDocument *pDoc);
#endif // !defined(AFX_OBJINFO_H__C7744329_C50C_442A_B40D_5E72CA0FCCEF__INCLUDED_)

