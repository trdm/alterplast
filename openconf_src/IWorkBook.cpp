// IWorkBook.cpp: implementation of the CIWorkBook class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "IWorkBook.h"
ITypeInfo* CIWorkBook::m_pTypeInfo=NULL;

CIWorkBook::CIWorkBook(CDocument* pDoc):CCfgDocDerived<IWorkBook,WorkBook>(pDoc)
{
}

CIWorkBook::~CIWorkBook()
{
}

int CIWorkBook::GetPageMinCount(CWorkBookDoc* pDoc)
{
	CString type;
	long id=CConfigCont::DocumentToID(pDoc,type);
	if(id<=0)
		return 3;
	else if(id==1)
		return 0;
	else
		return 2;
}

CDocument* CIWorkBook::GetPage(CWorkBookDoc* pDoc,VARIANT& idx,int& pos,CString* name)
{
	int fnd=-1;
	int cnt=GetPageMinCount(pDoc);
	CString PageName;
	CDocument* pFoundDoc=NULL;
	if(idx.vt==VT_BSTR)
	{
		_bstr_t bName=idx;
		CString sName=(LPCTSTR)bName;
		pFoundDoc=pDoc->FindDocument(sName,&fnd);
		int i=pDoc->GetPagePos(sName);
		if(!pFoundDoc)
		{
			if(!sName.CompareNoCase("Диалог") && cnt>0)
			{
				fnd=0;
				pFoundDoc=pDoc->GetDEditDocument();
			}
			else if(!sName.CompareNoCase("Модуль") && cnt>0)
			{
				fnd=1;
				pFoundDoc=pDoc->GetTextDocument();
			}
		}
		if(!pFoundDoc)
			SetError(E_FAIL,"Таблица с именем %s не найдена.",sName);
		PageName=sName;
	}
	else
	{
		if(idx.vt!=VT_I4)
			VariantChangeType(&idx,&idx,0,VT_I4);
		if(idx.vt!=VT_I4)
			SetError(E_FAIL,"Недопустимый тип индекса.");
		else
		{
			bool BadIndex=false;
			if(idx.lVal<0)
				BadIndex=true;
			else if(idx.lVal<cnt)
			{
				if(idx.lVal==0)
				{
					pFoundDoc=pDoc->GetDEditDocument();
					fnd=0;
					PageName="Диалог";
				}
				else if(idx.lVal==1)
				{
					pFoundDoc=pDoc->GetTextDocument();
					fnd=1;
					PageName="Модуль";
				}
				else
				{
					PageName="Описание";
					pFoundDoc=pDoc->FindDocument(PageName,&fnd);
				}
			}
			else
			{
				CStringArray tables;
				pDoc->GetTablesName(tables);
				if(idx.lVal>=cnt+tables.GetSize())
					BadIndex=true;
				else
				{
					PageName=tables[idx.lVal-cnt];
					pFoundDoc=pDoc->FindDocument(PageName,&fnd);
				}
			}
			if(BadIndex)
				SetError(E_FAIL,"Недопустимый индекс: %i",idx.lVal);
		}
	}
	if(name)
		*name=PageName;
	pos=fnd;
	return pFoundDoc;
}

STDMETHODIMP CIWorkBook::get_CountPages(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	CStringArray tables;
	pDoc->GetTablesName(tables);
	*pVal=GetPageMinCount(pDoc)+tables.GetSize();
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_NamePage(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	*pVal=NULL;
	CString name;
	int pos=-1;
	if(GetPage(pDoc,idx,pos,&name))
	{
		*pVal=name.AllocSysString();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CIWorkBook::put_NamePage(VARIANT idx,BSTR newVal)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	_bstr_t bNewName=newVal;
	CString sNewName=(LPCTSTR)bNewName;
	if(sNewName.IsEmpty())
		return SetError(E_INVALIDARG,"Пустое новое имя");
	int pos=-1;
	CString name;
	if(!GetPage(pDoc,idx,pos,&name))
		return E_FAIL;
	if(pos<GetPageMinCount(pDoc))
		return SetError(E_FAIL,"Нельзя переименовать страницу \"%s\"",name);
	if(!sNewName.CompareNoCase(name))
		return S_OK;
	pDoc->RenamePage(pos,sNewName);
	pDoc->SetModifiedFlag(TRUE);
	POSITION p=pDoc->GetFirstViewPosition();
	if(p)
	{
		CView* pView=pDoc->GetNextView(p);
		if(pView)
		{
			HWND hTab=::GetWindow(pView->m_hWnd,GW_CHILD);
			TCITEM item;
			item.mask=TCIF_TEXT;
			item.dwStateMask=TCIF_TEXT;
			item.pszText=(LPTSTR)(LPCTSTR)sNewName;
			TabCtrl_SetItem(hTab,pos,&item);
		}
	}
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_Page(VARIANT idx,ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	int pos=-1;
	CDocument* pPage=GetPage(pDoc,idx,pos);
	if(pPage)
	{
		*pVal=CICfgDoc::CreateDoc(pPage);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CIWorkBook::get_ActivePage(VARIANT *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	POSITION p=pDoc->GetFirstViewPosition();
	if(!p)
		return SetError(E_FAIL,"Данный метод можно вызывать только при открытом документе");
	CDocument* pActive=pDoc->GetActiveDocument();
	if(pActive)
	{
		pVal->vt=VT_I4;
		pVal->lVal=pDoc->GetPagePos(pActive);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CIWorkBook::put_ActivePage(VARIANT newVal)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	POSITION p=pDoc->GetFirstViewPosition();
	if(!p)
		return SetError(E_FAIL,"Данный метод можно вызывать только при открытом документе");
	int cnt=GetPageMinCount(pDoc);
	int pos=-1;
	if(newVal.vt==VT_BSTR)
	{
		_bstr_t bName=newVal;
		CString sName=(LPCTSTR)bName;
		if(!sName.CompareNoCase("Диалог") && cnt>0)
			pos=0;
		else if(!sName.CompareNoCase("Модуль") && cnt>0)
			pos=1;
		else
		{
			if(!pDoc->FindDocument(sName,&pos))
				pos=-1;
		}
		if(pos==-1)
			return SetError(E_FAIL,"Страница не найдена: \"%s\"",sName);
	}
	else
	{
		if(newVal.vt!=VT_I4)
			VariantChangeType(&newVal,&newVal,0,VT_I4);
		if(newVal.vt!=VT_I4)
			return SetError(E_FAIL,"Недопустимый тип индекса.");
		CStringArray tables;
		pDoc->GetTablesName(tables);
		int pages=cnt+tables.GetSize();
		if(newVal.lVal<0 || newVal.lVal>=pages)
			return SetError(E_FAIL,"Недопустимый номер страницы: %i",newVal.lVal);
		pos=newVal.lVal;
	}
	pDoc->SwitchToPage(pos);
	return S_OK;
}

STDMETHODIMP CIWorkBook::AddPage(BSTR Name,ICfgDoc **ppNewPage)
{
	if(IsBadWritePtr(ppNewPage,sizeof(*ppNewPage)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	_bstr_t bName=Name;
	CString sName=(LPCTSTR)bName;
	if(sName.IsEmpty())
		return SetError(E_INVALIDARG,"Пустое имя");
	int pos=pDoc->GetPagePos(sName);
	if(pos<0)
	{
		int cnt=GetPageMinCount(pDoc);
		if(!sName.CompareNoCase("Диалог") && cnt>0)
			pos=0;
		else if(!sName.CompareNoCase("Модуль") && cnt>0)
			pos=1;
		else if(!sName.CompareNoCase("Описание") && cnt>2)
			pos=2;
	}
	if(pos>=0)
		return SetError(E_FAIL,"Страница с именем \"%s\" уже существует.",sName);
	
	// 142-form
	// 161-table
	// 206-text
	/*
	pDoc->InsertNewPage(142,1,"Форма1","AddinForm");
	pDoc->SetModifiedFlag(TRUE);
	*/
	
	*ppNewPage=CICfgDoc::CreateDoc(pDoc->AddPage(161,sName));
	if(*ppNewPage)
		pDoc->SetModifiedFlag(TRUE);
	return S_OK;
}

STDMETHODIMP CIWorkBook::RemovePage(VARIANT idx)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	int pos;
	if(GetPage(pDoc,idx,pos))
	{
		if(pos<GetPageMinCount(pDoc))
			return SetError(E_FAIL,"Нельзя удалить эту страницу");
		CView* pView=NULL;
		HWND hTab=NULL;
		POSITION p=pDoc->GetFirstViewPosition();
		if(p)
		{
			pView=pDoc->GetNextView(p);
			if(pView)
			{
				hTab=::GetWindow(pView->m_hWnd,GW_CHILD);
				int CurPage=TabCtrl_GetCurSel(hTab);
				if(CurPage==pos)
					pDoc->SwitchToPage(pos?pos-1:1);
			}
		}
		pDoc->DeletePage(pos);
		if(pView)
		{
			TabCtrl_DeleteItem(hTab,pos);
			pDoc->UpdateAllViews(NULL);
			pDoc->UpdateMoxelHook();
		}
		pDoc->SetModifiedFlag(TRUE);
		return S_OK;
	}
	return E_FAIL;
}


/*
STDMETHODIMP CIWorkBook::get_TableCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	CStringArray tables;
	pDoc->GetTablesName(tables);
	*pVal=tables.GetSize();
	return S_OK;
}

STDMETHODIMP CIWorkBook::SwitchToPage(VARIANT Page)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	POSITION p=pDoc->GetFirstViewPosition();
	if(!p)
		return SetError(E_FAIL,"Данный метод можно вызывать только при открытом документе");
	int cnt=GetPageMinCount(pDoc);
	int pos=-1;
	if(Page.vt==VT_BSTR)
	{
		_bstr_t bName=Page;
		CString sName=(LPCTSTR)bName;
		if(!sName.CompareNoCase("Диалог") && cnt>0)
			pos=0;
		else if(!sName.CompareNoCase("Модуль") && cnt>0)
			pos=1;
		else
		{
			if(!pDoc->FindDocument(sName,&pos))
				pos=-1;
		}
		if(pos==-1)
			return SetError(E_FAIL,"Страница не найдена: \"%s\"",sName);
	}
	else
	{
		if(Page.vt!=VT_I4)
			VariantChangeType(&Page,&Page,0,VT_I4);
		if(Page.vt!=VT_I4)
			return SetError(E_FAIL,"Недопустимый тип индекса.");
		CStringArray tables;
		pDoc->GetTablesName(tables);
		int pages=cnt+tables.GetSize();
		if(Page.lVal<0 || Page.lVal>=pages)
			return SetError(E_FAIL,"Недопустимый номер страницы: %i",Page.lVal);
		pos=Page.lVal;
	}
	pDoc->SwitchToPage(pos);
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_TableName(VARIANT idx,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	int pos;
	CString name;
	if(!GetTable(pDoc,idx,pos,&name))
		return E_FAIL;
	*pVal=name.AllocSysString();
	return S_OK;
}

STDMETHODIMP CIWorkBook::put_TableName(VARIANT idx,BSTR newVal)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	_bstr_t name=newVal;
	CString strName=(LPCTSTR)name;
	int pos;
	if(!GetTable(pDoc,idx,pos))
		return E_FAIL;
	pDoc->RenamePage(pos,strName);
	pDoc->SetModifiedFlag(TRUE);
	POSITION p=pDoc->GetFirstViewPosition();
	if(p)
	{
		CView* pView=pDoc->GetNextView(p);
		if(pView)
		{
			HWND hTab=::GetWindow(pView->m_hWnd,GW_CHILD);
			TCITEM item;
			item.mask=TCIF_TEXT;
			item.dwStateMask=TCIF_TEXT;
			item.pszText=(LPTSTR)(LPCTSTR)strName;
			TabCtrl_SetItem(hTab,pos,&item);
		}
	}
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_ActivePage(ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	POSITION pos=pDoc->GetFirstViewPosition();
	if(!pos)
		return SetError(E_FAIL,"Данный метод можно вызывать только при открытом документе");
	*pVal=CICfgDoc::CreateDoc(pDoc->GetActiveDocument());
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_FormEditDoc(ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	*pVal=CICfgDoc::CreateDoc(pDoc->GetDEditDocument());
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_TextDoc(ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	*pVal=CICfgDoc::CreateDoc(pDoc->GetTextDocument());
	return S_OK;
}

STDMETHODIMP CIWorkBook::get_Table(VARIANT idx,ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	int pos;
	*pVal=CICfgDoc::CreateDoc(GetTable(pDoc,idx,pos));
	return S_OK;
}

STDMETHODIMP CIWorkBook::AddTable(BSTR Name,ICfgDoc **NewTable)
{
	if(IsBadWritePtr(NewTable,sizeof(*NewTable)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	_bstr_t bName=Name;
	CString sName=(LPCTSTR)bName;
	int pos=pDoc->GetPagePos(sName);
	if(pos<0)
	{
		int cnt=GetPageMinCount(pDoc);
		if(!sName.CompareNoCase("Диалог") && cnt>0)
			pos=0;
		else if(!sName.CompareNoCase("Модуль") && cnt>0)
			pos=1;
		else if(!sName.CompareNoCase("Описание") && cnt>2)
			pos=2;
	}
	if(pos>=0)
		return SetError(E_FAIL,"Недопустимое имя таблицы: \"%s\"",sName);
	*NewTable=CICfgDoc::CreateDoc(pDoc->AddPage(161,sName));
	if(*NewTable)
		pDoc->SetModifiedFlag(TRUE);
	return S_OK;
}

STDMETHODIMP CIWorkBook::RemoveTable(VARIANT idx)
{
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	int pos;
	if(GetTable(pDoc,idx,pos))
	{
		CView* pView=NULL;
		HWND hTab=NULL;
		POSITION p=pDoc->GetFirstViewPosition();
		if(p)
		{
			pView=pDoc->GetNextView(p);
			if(pView)
			{
				hTab=::GetWindow(pView->m_hWnd,GW_CHILD);
				int CurPage=TabCtrl_GetCurSel(hTab);
				if(CurPage==pos)
					pDoc->SwitchToPage(pos-1);
			}
		}
		pDoc->DeletePage(pos);
		if(pView)
		{
			TabCtrl_DeleteItem(hTab,pos);
			pDoc->UpdateAllViews(NULL);
			pDoc->UpdateMoxelHook();
		}
		pDoc->SetModifiedFlag(TRUE);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CIWorkBook::get_Description(ICfgDoc **pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CWorkBookDoc* pDoc=GetWB();
	if(!pDoc)
		return E_FAIL;
	*pVal=NULL;
	if(GetPageMinCount(pDoc)==3)
	{
		int page;
		*pVal=CICfgDoc::CreateDoc(pDoc->FindDocument("Описание",&page));
	}
	return S_OK;
}
*/
