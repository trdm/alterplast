// ClassInfo.cpp: implementation of the CClassInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "ClassInfo.h"
#include ".\1cpp\MetaDataOfClasses.h"
#include "declprocinfo.h"
#include "mytextdoc.h"
#include "moduleparser.h"
#include "deffileinfo.h"

#include "procinfo.h"
#include "procexinfo.h"
#include "valinfo.h"
#include "valexinfo.h"
#include "classprop.h"
#include "syntax.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

CIStringMap<CClassInfo::CPtr, CClassInfo::CPtr&>  CClassInfo::m_ClassInfoMap; //CMapStringToPtr CClassInfo::m_ClassInfoMap;

CIStringMap<CClassInfo*, CClassInfo*> CClassInfo::m_ClassFileNameMap;//CMapStringToPtr CClassInfo::m_ClassFileNameMap;

CClassInfo::CClassInfo(const CString& name):CInfo(name,TYPE_CLASS_FOLDER)
{
	m_ImplementPath=CMetaDataOfClasses::m_oneMetadata->m_ClassBindFile[m_DisplayName];
// 	if (!this)
// 		RuntimeError("ClassInfo* this == NULL :(");
	assert(this);
	
	m_ClassFileNameMap[m_ImplementPath] = this; // artbear

	m_DefinePath=CMetaDataOfClasses::m_oneMetadata->m_FullFileNameOfDescrFile[m_DisplayName];
	CMapStringToOb* pMethodsMap = static_cast<CMapStringToOb*>(CMetaDataOfClasses::m_oneMetadata->m_TypeOfClassMeth[m_DisplayName]);
	if(pMethodsMap)
	{
		CMetaDataOfClasses::ARR_TYPE_NAME* pType;
		CString proc;
		for(POSITION pos=pMethodsMap->GetStartPosition();pos;)
		{
			pMethodsMap->GetNextAssoc(pos,proc,(CObject*&)pType);
			m_declProc[proc]=new CDeclProcInfo(proc,pType);
		}
	}
// 	m_mod.m_path=m_ImplementPath;
// 	m_mod.m_path.MakeLower();
	m_mod.Init(m_ImplementPath);
	PrepareKeys();
}

CClassInfo::~CClassInfo()
{
	ClearKeys();
}

HTREEITEM CClassInfo::InsertInTree(HTREEITEM parent)
{
	HTREEITEM item=CInfo::InsertInTree(parent);
	m_nodes.AddTail((void*)item);

	m_mod.ParseClassModule(m_DisplayName);

	CString proc;
	for(Str_Info_ITER it=m_declProc.begin();it!=m_declProc.end();it++)
		it->second->InsertInTree(item);

	InsertBaseClassesInTree(item);

	InsertKeys(item);
	return item;
}

void CClassInfo::OnSelect()
{
/*	//CMDSubDoc
	CWinApp* pApp = pMainApp;
	CMyDocTemp* pT;
	CAssistantDoc* pDoc=NULL;
	for(POSITION pos=pApp->GetFirstDocTemplatePosition();pos;)
	{
		pT=(CMyDocTemp*)pApp->GetNextDocTemplate(pos);
		if(CString("CAssistantDoc")==pT->GetDocClass()->m_lpszClassName)
		{
			POSITION p=pT->GetFirstDocPosition();
			if(p)
			{
				pDoc=(CAssistantDoc*)pT->GetNextDoc(p);
				break;
			}
		}
	}
	CAssistantView *pView=(CAssistantView*)pDoc->GetView();
	HTREEITEM item=pDoc->GetPermanentRoot();
	CItemList List;
	pDoc->GetItemData(item,&List);
	CString s1,s2;
	pDoc->GetPermStrins(&List,s1,s2);
	return;
	UINT id=CTemplate7::GetDocumentTemplateID(13);
	CTemplate7* pTemp=CTemplate7::FromID(id);
	if(pTemp)
	{
		CMyDocTemp* pT=(CMyDocTemp*)pTemp;
		CString rtc=PrintRTC(pT->GetDocClass());
		pTemp->OpenDocumentFileEx("X:\\Tornado\\1cv7srct.st",0,TRUE);
	}
	return;
	HINSTANCE h7=GetModuleHandle("seven.dll");
	static const char* names[]={
		"?classCAccContextDef@CAccContextDef@@2UCRuntimeClass@@B",
		"?classCAccFormContextDef@CAccFormContextDef@@2UCRuntimeClass@@B",
		"?classCAccListFormContextDef@CAccListFormContextDef@@2UCRuntimeClass@@B",
		"?classCAccountContextDef@CAccountContextDef@@2UCRuntimeClass@@B",
		"?classCCJContextDef@CCJContextDef@@2UCRuntimeClass@@B",
		"?classCCJFormContextDef@CCJFormContextDef@@2UCRuntimeClass@@B",
	//	"?classCCJPeriodContext@CCJPeriodContext@@2UCRuntimeClass@@B",
	//	"?classCCJRecordContext@CCJRecordContext@@2UCRuntimeClass@@B",
		"?classCCalcContextDef@CCalcContextDef@@2UCRuntimeClass@@B",
		"?classCDocContextDef@CDocContextDef@@2UCRuntimeClass@@B",
		"?classCDocEnvironContextDef@CDocEnvironContextDef@@2UCRuntimeClass@@B",
		"?classCDocFormContextDef@CDocFormContextDef@@2UCRuntimeClass@@B",
		"?classCFormContextDef@CFormContextDef@@2UCRuntimeClass@@B",
		"?classCGeneralContextDef@CGeneralContextDef@@2UCRuntimeClass@@B",
		"?classCJournalContextDef@CJournalContextDef@@2UCRuntimeClass@@B",
		"?classCOperContextDef@COperContextDef@@2UCRuntimeClass@@B",
		"?classCOperFormContextDef@COperFormContextDef@@2UCRuntimeClass@@B",
		"?classCOperJournalContextDef@COperJournalContextDef@@2UCRuntimeClass@@B",
		"?classCProvJournalContextDef@CProvJournalContextDef@@2UCRuntimeClass@@B",
		"?classCRepFormContextDef@CRepFormContextDef@@2UCRuntimeClass@@B",
		"?classCSubcContextDef@CSubcContextDef@@2UCRuntimeClass@@B",
		"?classCSubcFormContextDef@CSubcFormContextDef@@2UCRuntimeClass@@B",
		"?classCSubcItemFormContextDef@CSubcItemFormContextDef@@2UCRuntimeClass@@B",
		"?classCTradeContextDef@CTradeContextDef@@2UCRuntimeClass@@B"};
	//	"?classCXBaseContext@CXBaseContext@@2UCRuntimeClass@@B",
	//	"?classCXBaseKeyContext@CXBaseKeyContext@@2UCRuntimeClass@@B"};

	CRuntimeClass *pC;
	for(int r=0;r<22;r++)
	{
		pC=(CRuntimeClass*)GetProcAddress(h7,names[r]);
		CString txt=PrintRTC(pC);
		Log(txt);
		CBLContext* pDef=(CBLContext*)pC->CreateObject();
		if(!pDef)
		{
			Log("No create");
			continue;
		}
		int nMet=pDef->GetNMethods();
		for(int i=0;i<nMet;i++)
		{
			txt=pDef->GetMethodName(i,0);
			txt=txt+"  "+pDef->GetMethodName(i,1);
			Log(txt);
		}
	}
	return;
	*/


	if(m_ImplementPath.IsEmpty())
		return;
	OpenClass();
	//if(m_DefinePath.IsEmpty())
	//	return;
	//OpenDef();
}

enum MenuType{
	mtGotoDef = 1,
	mtGotoModule = 2,
	mtSave = 3,
	SyntaxWithGlobalModule = 4,
	SyntaxWithoutGlobalModule = 5,
};

int CClassInfo::GetContextMenu(CStringList &lst, DWORDPtrList &idflags)
{
	UINT flag;
	lst.AddTail("К объявлению");
	
	flag = m_DefinePath.IsEmpty() ? MF_GRAYED << 16 : 0;

	idflags.AddTail(mtGotoDef | flag);

	lst.AddTail("К &модулю");
	
	flag=m_ImplementPath.IsEmpty()?MF_GRAYED<<16:0;
	idflags.AddTail(mtGotoModule | flag);
	
	CDocument* pDoc;
	flag = MF_GRAYED << 16;
	
	pDoc = CDocumentInterceptor::LookupDoc(m_ImplementPath);
	if(pDoc)
		flag = pDoc->IsModified()?0:MF_GRAYED<<16;

	lst.AddTail("Сохранить\tCtrl+S");
	idflags.AddTail(mtSave | flag);

	AddSeparator(lst, idflags);

	flag = m_ImplementPath.IsEmpty() ? MF_GRAYED << 16 : 0;
	
	lst.AddTail("Синтакс-проверка с &ГМ");
	idflags.AddTail(SyntaxWithGlobalModule | flag);

	lst.AddTail("&Синтакс-проверка без ГМ");
	idflags.AddTail(SyntaxWithoutGlobalModule | flag);

	
// 	AddSeparator(lst, idflags);
// 
// 	lst.AddTail("Добавить метод...");
// 	idflags.AddTail((void*)3);
// 
// 	lst.AddTail("Добавить свойство...");
// 	idflags.AddTail((void*)4);
// 
// 	AddSeparator(lst, idflags);
// 
// 	lst.AddTail("Свойства");
// 	idflags.AddTail((void*)4);

	return 0;
}

void CClassInfo::OnContextMenu(UINT cmd)
{
	if(cmd == mtGotoDef)
		OpenDef();
	else if(cmd == mtGotoModule)
		OpenClass();
	else if(cmd == mtSave)
		CCPPView::GetView()->OnSave();
	else if(cmd == SyntaxWithGlobalModule || cmd == SyntaxWithoutGlobalModule)
	{
		if(m_ImplementPath.IsEmpty())
			return;
		bool bGlobalModuleFlag = cmd == SyntaxWithGlobalModule? true : false;
		CSyntax::CompileOne(this, bGlobalModuleFlag, true);
	}
}

CDocument* CClassInfo::OpenClass() const
{
//LogErr("m_ImplementPath %s", m_ImplementPath);

	if(m_ImplementPath.IsEmpty())
	{
			Log("Не указан путь к файлу реализации",mmNone,true);
			return NULL;
	}

	CDocument* pDoc = OpenDocumentByPath(m_ImplementPath);
	if(!pDoc)
	{
		Log("Ошибка открытия модуля класса %s",mmBlackErr,false,(LPCTSTR)m_DisplayName);
		return NULL;
	}
	
	if(!strcmp("CWorkBookDoc", pDoc->GetRuntimeClass()->m_lpszClassName))
		static_cast<CWorkBookDoc*>(pDoc)->SwitchToPage(1);

	//AfxMessageBox(pDoc->GetRuntimeClass()->m_lpszClassName);
	CDocumentInterceptor::HookDoc(pDoc,m_DisplayName);

	/*// artbeaR
		CGetDoc7* pDoc7 = (CGetDoc7*)pDoc;
		//CBLModule7* pMod = 
	Msg("pDoc7->m_ContName %s", pDoc7->m_ContName);
	Msg("pDoc7->m_pString %s", pDoc7->m_pString);
	Msg("pDoc7->m_String1 %s", pDoc7->m_String1);
	Msg("pDoc7->m_String2 %s", pDoc7->m_String2);
	Msg("pDoc7->m_pBLModule %d", pDoc7->m_pBLModule);
	*/

	return pDoc;
}

void CClassInfo::OpenDef()
{
// 	CModuleParser mod;
// 	int line;
// 	CBodyTextView* pEditor = mod.FindDef(::OpenDocumentByPath(m_DefinePath), m_DisplayName, line);
	int line;
	CBodyTextView* pEditor = CModuleParser::FindDef(::OpenDocumentByPath(m_DefinePath), m_DisplayName, line);
	if(pEditor)
	{
		if(line>-1)
		{
			CBodyTextDoc* pTextDoc = pEditor->GetDocument();
			pEditor->MoveCaret(CPoint(0,line+40),CPoint(0,line+40),1);
			pEditor->MoveCaret(CPoint(0,line),CPoint(pTextDoc->GetLineLength(line)-2,line),1);
		}
		else
			Log(m_DisplayName+" : Не найден в файле "+m_DefinePath);
	}
}

CClassInfo::CPtr CClassInfo::FindElem( const CString& ClassName )
{
	CClassInfo::CPtr pInfo; // = NULL;
	if(!m_ClassInfoMap.Lookup(ClassName, pInfo))
	{
		pInfo = CClassInfo::CPtr(new CClassInfo(ClassName));
//Msg("ClassName %s", ClassName);
		m_ClassInfoMap[ClassName]=pInfo;
	}
	return pInfo;
}

void CClassInfo::OpenKeys(const CString &name)
{
	CDocument* pDoc = OpenClass();
	CBodyTextView *pEditor = CCPPView::FindTextView(pDoc);
	if(!pEditor)
	{
		Log("Не удалось получить текст модуля");
		return;
	}

	m_mod.ParseClassModule(m_DisplayName);

	CKeyAndLinePair keyAndLine;
	if( m_mod.FindKey(name, &keyAndLine) )
	{
		//DWORD line = 0x3FFFFFFF & keyAndLine;
		DWORD line = keyAndLine.GetLine();
		line += m_mod.getOffsetLines();

		CBodyTextDoc* pTextDoc = pEditor->GetDocument();

		pEditor->GetParentFrame()->ActivateFrame();
		pEditor->SetFocus();
		pEditor->MoveCaret(CPoint(0,line+40),CPoint(0,line+40),10);
		pEditor->MoveCaret(CPoint(0,line),CPoint(pTextDoc->GetLineLength(line)-2,line),1);
	}
	else
	{
		CString err;
		err.Format("\"%s\" не найден в файле реализации.", name.operator LPCTSTR());
		AfxMessageBox(err);
	}
}

void CClassInfo::ClearKeys()
{
	for(Str_Info_ITER it=m_keys.begin();it!=m_keys.end();it++)
		delete it->second;
	m_keys.clear();
	for(it=m_declProc.begin();it!=m_declProc.end();it++)
		delete it->second;
	m_declProc.clear();
	ClearRemoved();
}

void CClassInfo::PrepareKeys()
{
	static const DWORD TypeArray[] = { TYPE_MODULE_PROC_PRIVATE, TYPE_MEMBER_PRIVATE, TYPE_MODULE_PROC_EXPORT, TYPE_MEMBER_EXPORT };

	m_mod.ParseClassModule(m_DisplayName);

	for(Str_Info_ITER itkeys = m_keys.begin(); itkeys != m_keys.end();)
	{
		CKeyAndLinePair key1; //DWORD key1;
		bool found = m_mod.FindKey(itkeys->first, &key1);
		//if(found && itkeys->second->GetType() != TypeArray[key1 >> 30] )
		if(found && itkeys->second->GetType() != TypeArray[key1.GetKey()] )
			found = false;

		if(!found)
		{
			m_removedKeys[itkeys->first] = itkeys->second;
			itkeys = m_keys.erase(itkeys);
			continue;
		}
		itkeys++;
	}

	CString proc;
	Str_Info_ITER itdecl;
	for(Str2KeyAndLine_IT itproc = m_mod.GetKeysMap().begin(); itproc != m_mod.GetKeysMap().end(); itproc++)
	{
		proc = itproc->first;
		itdecl = m_declProc.find(proc);
		if(itdecl != m_declProc.end())
				continue;
		itkeys = m_keys.find(proc);
		if(itkeys != m_keys.end())
			continue;

		CInfo *pNew = NULL;
		//DWORD type=itproc->second>>30;
		KeyType2 type = itproc->second.GetKey();
		switch(type){
		case kt2KEY_PROC: //0:
			pNew = new CPrivatedMethodInfo(proc);
			break;
		case kt2KEY_PROC_EXPORT: //2:
			pNew = new CExportedMethodInfo(proc);
			break;
		case kt2KEY_VAL: //1:
			pNew = new CPrivatedValInfo(proc);
			break;
		case kt2KEY_VAL_EXPORT: //3:
			pNew = new CExportedVaInfo(proc);
		}
		m_keys[proc] = pNew;
	}
}

void CClassInfo::InsertKeys(HTREEITEM item)
{
	CTreeCtrl& ctrl=CCPPView::GetMyTreeCtrl();
	HTREEITEM child=ctrl.GetNextItem(item,TVGN_CHILD);
	CInfo* pInfo;
	MAP_Str_Info exist;
	Str_Info_ITER it,ex;
	while(child)
	{
		pInfo=CInfo::GetData(child);
		if(pInfo->IsClassMember())
		{
			it=m_removedKeys.find(pInfo->GetDisplayName());
			if(it!=m_removedKeys.end())
			{
				HTREEITEM del=child;
				child=ctrl.GetNextItem(child,TVGN_NEXT);
				ctrl.DeleteItem(del);
				continue;
			}
			else
			{
				//Log(CString("Exist ")+pInfo->m_DisplayName);
				exist[pInfo->GetDisplayName()]=pInfo;
			}
		}
		child=ctrl.GetNextItem(child,TVGN_NEXT);
	}
	for(it=m_keys.begin();it!=m_keys.end();it++)
	{
		ex=exist.find(it->first);
		if(ex==exist.end())
			it->second->InsertInTree(item);
	}
}

void CClassInfo::ClearRemoved()
{
	for(Str_Info_ITER it=m_removedKeys.begin();it!=m_removedKeys.end();it++)
		delete it->second;
	m_removedKeys.clear();
}

void CClassInfo::GetPropertyPages(CPropertyPagePtr& pPropPage)
{
	pPropPage.reset( new CClassProp(m_DefinePath, m_ImplementPath) );
}

void CClassInfo::CompileSomeClassesWithFlagCompileAll()
{
	if(GetPath().IsEmpty())
		return;
	
	CSyntax::CompileSomeClassesWithFlagCompileAll(this);
}

void CClassInfo::UpdateLinks()
{
	CDefFileInfo::CPtr pDefFileInfo = CDefFileInfo::FindElem(m_DefinePath);
	assert(pDefFileInfo);

	m_pDefFileInfo = pDefFileInfo;

	if (m_BaseClassList)
		for(POSITION pos = m_BaseClassList->GetHeadPosition();pos;)
		{
			// artbear
			CClassInfo::CPtr pClass = CClassInfo::FindElem(m_BaseClassList->GetNext(pos));
			assert(pClass);
			
			m_ChildClasses.SetAt(pClass->GetDisplayName(), CWeakPtr(pClass));
		}
}

void CClassInfo::InsertBaseClassesInTree( HTREEITEM item )
{
	//CMapStringToOb& map = CMetaDataOfClasses::m_oneMetadata->m_ClassesHierarchyMap;
	CMetaDataOfClasses::CBaseClassesMap& map = CMetaDataOfClasses::m_oneMetadata->m_ClassesHierarchyMap;
	CClassInfo* pParent = static_cast<CClassInfo*>( GetData(item) );

		// artbear
		//Msg("pParent->m_DisplayName %s", pParent->m_DisplayName);
		//CString m_ImplementPath=CMetaDataOfClasses::m_oneMetadata->m_ClassBindFile[m_DisplayName];
		//Msg("pParent->m_DisplayName %s, m_ImplementPath %s", pParent->m_DisplayName, m_ImplementPath);
		//m_DefinePath=CMetaDataOfClasses::m_oneMetadata->m_FullFileNameOfDescrFile[m_DisplayName];
		//Msg("m_DefinePath %s", m_DefinePath);
	
	if(map.Lookup(pParent->m_DisplayName, m_BaseClassList))
	{
		if (m_BaseClassList)
			for(POSITION pos = m_BaseClassList->GetHeadPosition();pos;)
			{
				// artbear
				CClassInfo::CPtr pClass = CClassInfo::FindElem(m_BaseClassList->GetNext(pos));

				//	CString str = m_BaseClassList->GetNext(pos);
				//LogErr("базовый класс m_BaseClassList->GetNext(pos) = %s", str);
				//				CClassInfo* pClass=CClassInfo::FindElem(str);
				
				pClass->InsertInTree(item);
			}
	}
}