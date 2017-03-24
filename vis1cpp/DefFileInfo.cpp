// DefFileInfo.cpp: implementation of the CDefFileInfo class.
//
//////////////////////////////////////////////////////////////////////
#include "rootprop.h"
#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "DefFileInfo.h"
#include "classinfo.h"
#include "InfoWrcSpace.h"
#include "mytextdoc.h"
#include "rootprop.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CIStringMap<CDefFileInfo::CStringListPtr, CDefFileInfo::CStringListPtr&> CDefFileInfo::m_mapDef;
CIStringMap<CDefFileInfo::CPtr, CDefFileInfo::CPtr&> CDefFileInfo::m_ElementsMap;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CDefFileInfo::CDefFileInfo(const CString& path, const CStringListPtr& pClassesNameList): CInfo("",TYPE_DEF_FILE_FOLDER),
	m_path( path ), m_pClassesNameList( pClassesNameList )
{
	m_DisplayName = EvalDisplayName(path);
}

CDefFileInfo::~CDefFileInfo()
{
//	Msg("~CDefFileInfo %s", GetDisplayName().operator LPCTSTR());
}

HTREEITEM CDefFileInfo::InsertInTree(HTREEITEM parent)
{
	HTREEITEM item=CInfo::InsertInTree(parent);
	
	CreateAndInsertInTreeForEachClassInDefFile(item);

	return item;
}

void CDefFileInfo::OnSelect()
{
	Open();
}

CString CDefFileInfo::EvalDisplayName(const CString &path)
{
	CString ib=pSvc->IBDir(),p=path,key;
	ib.MakeLower();p.MakeLower();
	if(!p.Find(ib))
		return path.Mid(ib.GetLength());
	
	CMapStringToString& map=CMetaDataOfClasses::m_oneMetadata->m_AliasOfPaths;
	for(POSITION pos=map.GetStartPosition();pos;)
	{
		map.GetNextAssoc(pos,key,ib);
		ib.MakeLower();
		if(!p.Find(ib))
			return CString("#")+key+" - "+path.Mid(ib.GetLength()+1);
	}
	return path;
}

CDocument* CDefFileInfo::Open() const
{
	CDocument* pDoc = ::OpenDocumentByPath(m_path);
	if (!pDoc){
		AfxMessageBox("Не указан файл объявления.");
		return NULL;
	}

	if(!strcmp("CWorkBookDoc",pDoc->GetRuntimeClass()->m_lpszClassName))
		static_cast<CWorkBookDoc*>(pDoc)->SwitchToPage(1);
	
	CDocumentInterceptor::HookDoc(pDoc, m_path);

	return pDoc;
}

void CDefFileInfo::GetPropertyPages(CPropertyPagePtr& pPropPage)
{
	pPropPage.reset( new CRootProp(m_path) );
}

enum MenuType{
	mtOpenDef = 1,
	mtSave = 2,
};

int  CDefFileInfo::GetContextMenu(CStringList& lst, DWORDPtrList& idflags)
{
	lst.AddTail("Открыть");
	idflags.AddTail(mtOpenDef);
	UINT flag=MF_GRAYED;
	
	CDocument* pDoc = CDocumentInterceptor::LookupDoc(m_path);
	if(pDoc)
		flag=pDoc->IsModified()?0:MF_GRAYED;

	lst.AddTail("Сохранить\tCtrl+S");
	idflags.AddTail(mtSave | (flag<<16));
	return 0;
}

void CDefFileInfo::OnContextMenu(UINT cmd)
{
	if(cmd == mtOpenDef)
		Open();

	else if(cmd == mtSave)
		CCPPView::GetView()->OnSave();
}

// TODO - метод CDefFileInfo::CreateListOfClassesNameForEachDefFile нужно переместить в другой класс типа CMetaDataOfClasses
void CDefFileInfo::CreateListOfClassesNameForEachDefFile()
{
	CMapStringToString& mapOfFullFileNameOfDescrFile = CMetaDataOfClasses::m_oneMetadata->m_FullFileNameOfDescrFile;
	
	CString ClassName, DefFilePath;
	for(POSITION pos = mapOfFullFileNameOfDescrFile.GetStartPosition(); pos; )
	{
		mapOfFullFileNameOfDescrFile.GetNextAssoc(pos, ClassName, DefFilePath);
		
		CStringList *pList = m_mapDef[DefFilePath].get();
		if(!pList)
		{
			pList = new CStringList;
			m_mapDef[DefFilePath] = CStringListPtr(pList);
		}
		pList->AddTail(ClassName);
	}
}

void CDefFileInfo::CreateAndInsertInTreeForEachDefFile( HTREEITEM item )
{
	CString DefFilePath;
	CStringListPtr pClassesNameList;
	for(POSITION pos = m_mapDef.GetStartPosition(); pos;)
	{
		m_mapDef.GetNextAssoc(pos, DefFilePath, pClassesNameList);
		
		CPtr pElem = CreateElem(DefFilePath, pClassesNameList);

		pElem->InsertInTree(item);
	}
}

void CDefFileInfo::RemoveAllDefFiles()
{
	m_mapDef.RemoveAll();
	m_ElementsMap.RemoveAll();
}

void CDefFileInfo::CreateAndInsertInTreeForEachClassInDefFile( HTREEITEM item )
{
	CStringList* pList = m_pClassesNameList.get();
	for(POSITION pos = pList->GetHeadPosition();pos;)
	{
		CClassInfo::CPtr pClassInfo = CClassInfo::FindElem(pList->GetNext(pos));
		pClassInfo->InsertInTree(item);

		m_ChildClasses.SetAt(pClassInfo->GetDisplayName(), CClassInfo::CWeakPtr(pClassInfo));
	}
}

CDefFileInfo::CPtr CDefFileInfo::FindElem( const CString& strDefPath )
{
	CPtr pElem;
	if(m_ElementsMap.Lookup(strDefPath, pElem)){
		assert(pElem);
		assert(!pElem->GetPath().CompareNoCase(strDefPath));
		return pElem;
	}

	assert(pElem && "элемент уже должен существовать !!"); // TODO элемент уже должен существовать !!

	MessageBox("ВНИМАНИЕ: ошибка логики - CDefFileInfo::FindElem 1 - не найдено имя %s", strDefPath.operator LPCTSTR());

	CStringListPtr pClassesNameList;
	if (!m_mapDef.Lookup(strDefPath, pClassesNameList)){
		MessageBox("ВНИМАНИЕ: ошибка логики - CDefFileInfo::FindElem 2 - m_mapDef - не найдено имя %s", strDefPath.operator LPCTSTR());
		return CPtr();
	}

	pElem = CreateElem(strDefPath, pClassesNameList);
 	
	return pElem;
}

CDefFileInfo::CPtr CDefFileInfo::CreateElem( const CString& DefFilePath, const CStringListPtr& pClassesNameList )
{
	CPtr pElem ( new CDefFileInfo(DefFilePath, pClassesNameList) ); // TODO возможно, что лик ?
	
	assert(pElem);
	assert(!pElem->GetPath().CompareNoCase(DefFilePath));

	m_ElementsMap[DefFilePath] = pElem;
	return pElem;
}

// TODO отрефакторить лишний метод UpdateLinksForEachDefFileInner - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CDefFileInfo::UpdateLinksForEachDefFile()
{
	CDefFileInfo::ForEach( &CDefFileInfo::UpdateLinksForEachDefFileInner );
}

void CDefFileInfo::UpdateLinksForEachDefFileInner( const CDefFileInfo::CPtr& pClassInfo )
{
	assert(pClassInfo);
	pClassInfo->UpdateLinks();
}

// TODO отрефакторить лишний метод UpdateLinksForEachDefFileInner - с помощью mem_fun, bind и т.д. - пока что-то не получилось 
void CDefFileInfo::UpdateLinks()
{
	ForEachChildClassInfoIntoDefFile( &CDefFileInfo::UpdateLinksForClassInfo );
}

void CDefFileInfo::UpdateLinksForClassInfo(const CClassInfo::CWeakPtr& pClassInfo)
{
	assert(!pClassInfo.expired());
	pClassInfo.lock()->UpdateLinks();
}