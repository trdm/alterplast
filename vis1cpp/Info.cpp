// Info.cpp: implementation of the CInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Info.h"
#include "vis1cpp.h"
#include "vis1cpp_my.h"

#include "cppview.h"
#include "resource.h"

#include "ClassInfo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

HTREEITEM CInfo::m_CurItem;
bool CInfo::m_IsSort=true;

//CInfo::CInfo(const CString& name,UINT icon1, UINT icon2)
CInfo::CInfo(const CString& name, InfoType icon1)
{
	m_DisplayName	=	name;
	m_Type	=	icon1;
	//m_IconExpand	=	icon2 == -1 ? icon1 : icon2;
	m_IconExpand	=	m_Type;	// TODO возможно вставл€ть разные иконки дл€ открыти€/закрыти€
}

CInfo::~CInfo()
{
}

HTREEITEM CInfo::InsertInTree(HTREEITEM parent)
{
	HTREEITEM item = CCPPView::GetMyTreeCtrl().InsertItem(m_DisplayName,
		m_Type, m_IconExpand, parent, TVI_LAST);
	CCPPView::GetMyTreeCtrl().SetItemData(item, reinterpret_cast<DWORD>(this));
	return item;
}

void CInfo::ShowContextMenu(CPoint pt)
{
// 	if (!this)
// 		return;
	assert(this);
	
	CStringList lst;
	DWORDPtrList idflags;
	int def = GetContextMenu(lst, idflags);
	if(lst.GetCount())
	{
		AddSeparator(lst, idflags);
	}

	const int mtSort		= 100;
	const int mtCopy		= 101;
	const int mtProperty	= 102;

	lst.AddTail("√руппировать по &типу");
	idflags.AddTail(mtSort|(CInfo::m_IsSort?MF_CHECKED<<16:0));

	AddSeparator(lst, idflags);

	lst.AddTail(" опировать\tCtrl+C");
	idflags.AddTail(mtCopy);

	lst.AddTail("—войства\tAlt+Enter");
	idflags.AddTail(mtProperty);

	CMenu menu;
	if(!menu.CreatePopupMenu())
		return;
	
	CString txt;
	DWORD val;
	UINT id,flags;
	for(POSITION pos=lst.GetHeadPosition(),
		pf=idflags.GetHeadPosition();pos;)
	{
		txt=lst.GetNext(pos);
		val=(DWORD)idflags.GetNext(pf);
		if(txt.IsEmpty())
		{
			id=0;
			flags=MF_SEPARATOR;
		}
		else
		{
			id=LOWORD(val);
			flags=MF_STRING|(val>>16);
		}
		menu.AppendMenu(flags,id,txt);
	}
	if(def>-1)
		menu.SetDefaultItem(def,TRUE);
	
	const UINT ret = TrackPopupMenu(menu.m_hMenu,
		TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RIGHTBUTTON|TPM_NONOTIFY|TPM_RETURNCMD,
		pt.x,pt.y,0,CCPPView::GetView()->m_hWnd,NULL);
	
	menu.DestroyMenu();
	
	if(ret == mtCopy)
	{
		CCPPView::GetView()->OnCopy();
	}
	else if(ret == mtSort)
	{
		CInfo::SortAllTree(!m_IsSort);
	}
	else if(ret == mtProperty)
	{
		ShowProperties();
	}
	else if(ret)
		OnContextMenu(ret);
}

void CInfo::ShowProperties()
{
// 	if(!this)
// 		return;
	assert(this);
	
	boost::scoped_ptr<CPropertySheet> sheet ( new CPropertySheet(CString("—войства ")+m_DisplayName) );
	
	boost::scoped_ptr<CPropertyPage> pPropPage;

	CMain1C_ResourceHandler_Guard Main1C_ResourceHandler_Saver;

	GetPropertyPages(pPropPage);
	
	if (!pPropPage)
		pPropPage.reset(new CPropertyPage(IDD_EMPTY));

	sheet->AddPage(pPropPage.get());

	sheet->DoModal();
}

void CInfo::OpenKeysForClassInfoFromCurItem( const CString& strItemID )
{
	CClassInfo* pInfo = static_cast<CClassInfo*>( GetParentData(GetCurItem()) );
	//pInfo->OpenKeys(m_DisplayName);
	pInfo->OpenKeys(strItemID);
}

CInfo* CInfo::GetData(HTREEITEM item)
{
	if(!item)
		return NULL;
	m_CurItem=item;
	
	return reinterpret_cast<CInfo*>( CCPPView::GetMyTreeCtrl().GetItemData(item) );
}

int CALLBACK CInfo::CallbackCompare(LPARAM lParam1, LPARAM lParam2, 
								 LPARAM lParamSort)
{
	CInfo* pItem1 = reinterpret_cast<CInfo*>( lParam1 );
	CInfo* pItem2 = reinterpret_cast<CInfo*>( lParam2 );
	assert(pItem1);
	assert(pItem2);
	
	return pItem1->Compare(pItem2);
}

int CALLBACK CInfo::CallbackCompareWithoutSort(LPARAM lParam1, LPARAM lParam2, 
									   LPARAM lParamSort) // TODO подобрать более точное им€
{
	CInfo* pItem1 = reinterpret_cast<CInfo*>( lParam1 );
	CInfo* pItem2 = reinterpret_cast<CInfo*>( lParam2 );
	assert(pItem1);
	assert(pItem2);
	
	return pItem1->CompareWithoutSort(pItem2);
}

void CInfo::AddSeparator( CStringList& lst, DWORDPtrList& idflags )
{
	lst.AddTail("");
	idflags.AddTail(static_cast<UINT>(0));
}

void CInfo::SortAllTree(bool bNeedSort)
{
	m_IsSort = bNeedSort; //m_IsSort = !m_IsSort;
	CCPPView::GetView()->Sort(TVI_ROOT);
}

void CClassMemberInfo::OnSelect()
{
	// 	CClassInfo* pInfo=(CClassInfo*)GetParentData(GetCurItem());
	// 	pInfo->OpenKeys(m_DisplayName);
	OpenKeysForClassInfoFromCurItem(m_DisplayName);
}

CDocument* OpenDocumentByPath(const CString& szPath)
{
	//LogErr("szPath %s", szPath);
	if (szPath.IsEmpty()) //if(!szPath || !*szPath)
		return NULL;

	CDocument* pDoc = NULL;

	const CString tPath = pSvc->IBDir() + szPath;
	// почему конструируетс€ полный путь? дело в том, что если
	// мы передадим ему относительный, и файл будет существовать, то форма
	// откроетс€, но позиционирование на метод и даже на текст модул€ не произойдет

// 	DWORD tRes = GetFileAttributes((LPCTSTR)tPath);
// 	if (tRes != 0xFFFFFFFF){
	if (FileExists(tPath)){
		pDoc = pSvc->OpenFile( tPath );
	}
	if (!pDoc){
// 		DWORD tRes = GetFileAttributes(szPath); // (LPCTSTR)
// 		if (tRes != 0xFFFFFFFF){
		if (FileExists(szPath)){
			pDoc = pSvc->OpenFile(szPath);
		}
	}

	if(pDoc)
		return pDoc;

	CString strTemp = szPath;
	strTemp.MakeLower();

	const int npos = strTemp.Find("@md");
	if (-1 == npos)
		return pDoc;

	const CString nameRep = strTemp.Left(npos);

	CMetaDataCont *pMD = GetMetaData();
	if(!pMD)
		return NULL;

	CMetaDataObj* pObj = pMD->GetCalcVarDef(nameRep);
	if(!pObj)
		return NULL;

	const long id=pObj->GetID();
	CTypedCont* pCont=NULL;
	CString text;
	CConfigCont::IDToPath(id,"CalcVar",text,&pCont,FALSE);

	if(!pCont)
		return NULL;

	//try{
		//pDoc = (CDocument*)pCont->GetWorkBook();
		pDoc = static_cast<CDocument*>( pCont->GetWorkBook() );
		//pCont->ShowDocument(id);  // сразу падает

		POSITION aPOS = pDoc->GetFirstViewPosition();
		CView* pView = pDoc->GetNextView(aPOS);

		if (!pView)
			pCont->ShowDocument(id);
		else{
			CFrameWnd* pParentFrame = pView->GetParentFrame();
			pParentFrame->ActivateFrame();
		}

	/*}
	catch(...)
	{}*/

	return pDoc;
}
