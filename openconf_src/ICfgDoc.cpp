// ICfgDoc.cpp: implementation of the CICfgDoc class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ICfgDoc.h"
#include "ICfgDocuments.h"
#include "IWorkBook.h"
#include "ITextDoc.h"
#include "ITableDoc.h"
#include "IDialogDoc.h"
#include "Metadata.h"

ITypeInfo* CICfgDoc::m_pTypeInfo=NULL;
CString CICfgDoc::m_tempPath;
CICfgDoc::Init CICfgDoc::m_init;

static inline BOOL IsModuleDoc(LPCTSTR type)
{
	return strstr("ModuleText,UserHelp,Transact,CalcAlg",type)!=NULL;
}

CICfgDoc::~CICfgDoc()
{
}

static CString GetDocPath(CDocument* pDoc)
{
	CString path=pDoc->GetPathName();
	if(path.IsEmpty())
		CConfigCont::DocumentToPath(pDoc,path);
	if(!path.CompareNoCase(CICfgDoc::m_tempPath))
		path.Empty();
	return path;
}

static DocTypes GetType(CDocument* pDoc)
{
	DocTypes t=docUnknown;
//	CString rtc=PrintRTC(pDoc);
//	if(rtc.Find("CWorkBookDoc")!=-1)
//	{
//		t=docWorkBook;
//		/*CWorkBookDoc* pWB=(CWorkBookDoc*)pDoc;
//		CTextDocument* pTD=(CTextDocument*)pWB->GetTextDocument();
//		POSITION pos=pTD->GetFirstViewPosition();
//		CTextEditor* pView=(CTextEditor*)pTD->GetNextView(pos);
//		CString text;
//		pView->GetSelText(text);
//		//DoMsgLine("Sel text=%s",mmNone,text);
//		CPoint pt1,pt2;
//		pTD->GetSel(pt1,pt2);
//		pTD->GetSelectedText(text);
//		DoMsgLine("text=%s\r\nx1=%i y1=%i, x2=%i,y2=%i",mmNone,text,pt1.x,pt1.y,pt2.x,pt2.y);
//		pTD->GetText(text);
//		DoMsgLine(text);
//		//pTD->SetSel(CPoint(0,0),CPoint(3,1));
//		*/
//	}
//	else if(rtc.Find("CTextDocument")!=-1)
//		t=docText;
//	else if(rtc.Find("CSheetDoc")!=-1)
//		t=docTable;
//	else if(rtc.Find("CEditDoc")!=-1)
//		t=docDEdit;
	CRuntimeClass* pRtc=RUNTIME_CLASS(CWorkBookDoc);
	if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[0]))
	{
		t=docWorkBook;
		/*CWorkBookDoc* pWB=(CWorkBookDoc*)pDoc;
		CTextDocument* pTD=(CTextDocument*)pWB->GetTextDocument();
		POSITION pos=pTD->GetFirstViewPosition();
		CTextEditor* pView=(CTextEditor*)pTD->GetNextView(pos);
		CString text;
		pView->GetSelText(text);
		//DoMsgLine("Sel text=%s",mmNone,text);
		CPoint pt1,pt2;
		pTD->GetSel(pt1,pt2);
		pTD->GetSelectedText(text);
		DoMsgLine("text=%s\r\nx1=%i y1=%i, x2=%i,y2=%i",mmNone,text,pt1.x,pt1.y,pt2.x,pt2.y);
		pTD->GetText(text);
		DoMsgLine(text);
		//pTD->SetSel(CPoint(0,0),CPoint(3,1));
		*/
	}
	else if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[3]))
		t=docText;
	else if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[1]))
		t=docTable;
	else if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[2]))
		t=docDEdit;
	return t;
}

STDMETHODIMP CICfgDoc::get_Type(DocTypes *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		*pVal=GetType(pDoc);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CICfgDoc::get_ID(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		CString type;
		*pVal=CConfigCont::DocumentToID(pDoc,type);
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CICfgDoc::get_Kind(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		CString type;
		CConfigCont::DocumentToID(pDoc,type);
		*pVal=type.AllocSysString();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CICfgDoc::get_Path(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		CString path=m_path;
		if(!path.IsEmpty())
			path.Replace(m_tempPath,"");
		*pVal=path.AllocSysString();
		return S_OK;
	}
	return E_FAIL;
}
STDMETHODIMP CICfgDoc::Open()
{
	CDocument* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	POSITION pos=pDoc->GetFirstViewPosition();
	if(!pos)
	{
		CString type;
		long id=CConfigCont::DocumentToID(pDoc,type);
		if(id>0)
		{
			CString path;
			if(IsModuleDoc(type))
			{
				CModuleCont* pCont=NULL;
				CConfigCont::IDToTextModule(id,type,path,&pCont,0);
				if(pCont)
				{
					if(type!="UserHelp")
						((CTextDocument*)pDoc)->SetModule(TRUE);
					pCont->ShowDocument(type);
				}
			}
			else
			{
				CTypedCont* pCont=NULL;
				CConfigCont::IDToPath(id,type,path,&pCont,0);
				if(pCont)
				{
					pCont->ShowDocument(id);
					DocTypes t=GetType(pDoc);
					if(t!=docWorkBook)
					{
						CWorkBookDoc* pWB=pCont->GetWorkBook();
						if(pWB)
						{
							int page=0;
							if(t==docText)
								page=1;
							else if(t==docTable)
								page=pWB->GetPagePos(pDoc);
							pWB->SwitchToPage(page);								
						}
					}
				}
			}
			pos=pDoc->GetFirstViewPosition();
		}
	}
	if(pos)
	{
		CView* pView=pDoc->GetNextView(pos);
		if(pView)
		{
			CFrameWnd* pFrame=pView->GetParentFrame();
			if(pFrame)
			{
				((CMDIFrameWnd*)AfxGetMainWnd())->MDIActivate(pFrame);
				pView->SetFocus();
			}
		}
	}
	return S_OK;
}

STDMETHODIMP CICfgDoc::get_IsOpen(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		POSITION pos=pDoc->GetFirstViewPosition();
		*pVal=pos?-1:0;
		return S_OK;
	}
	return E_FAIL;
}

class CSaveDocWrap:public CDocument
{
public:
	void Save(){OnFileSave();}
};

STDMETHODIMP CICfgDoc::Save()
{
	CDocument* pDoc=GetDocument();
	if(pDoc)
	{
		((CSaveDocWrap*)pDoc)->Save();
		return S_OK;
	}
	return E_FAIL;
}

STDMETHODIMP CICfgDoc::get_Name(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CDocument* pDoc=GetDocument();
	if(!pDoc)
		return E_FAIL;
	CString type,path;
	long id=CConfigCont::DocumentToID(pDoc,type);
	CString name;
	if(id>-1)
	{
		CString lastname;
		name=GetModuleFullName(id,type,1,CIMetaData::m_pMetaData);
		int dec=name.ReverseFind('.');
		if(dec>-1)
		{
			lastname=name.Mid(dec);
			name=name.Left(dec);
		}
		if(type=="UserHelp")
		{
			CString txt;
			txt.Format("Описание.%i.%s",id,name);
			name=txt;
		}
		else
		{
			if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[2]))			// Диалог
				name+=".Диалог";
			else if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[1]))	// Таблица
			{
				CConfigCont::DocumentToPath(pDoc,path);
				dec=path.ReverseFind('.');
				int pagenum=atoi(path.Mid(dec+1))-1;
				CTypedCont* pCont=NULL;
				CConfigCont::IDToPath(id,type,path,&pCont,TRUE);
				CStringArray names;
				pCont->GetTablesName(names);
				name=name+"."+names[pagenum];
			}
			else if(pDoc->IsKindOf(CICfgDocuments::m_pUsefulRTC[3]))	// Модуль
				name+=lastname;
		}
	}
	else
		name.Format("%s::%s",pDoc->GetRuntimeClass()->m_lpszClassName,pDoc->GetTitle());
	*pVal=name.AllocSysString();
	return S_OK;
}

CDocument* CICfgDoc::GetDocument()
{
	// try find in template by pointer
	CDocument* pFndDoc=NULL;
	for(POSITION pos=m_pTemplate->GetFirstDocPosition();pos;)
	{
		CDocument* pDoc=m_pTemplate->GetNextDoc(pos);
		if(pDoc==m_pDoc)
		{
			pFndDoc=pDoc;
			break;
		}
	}
	if(pFndDoc)		// Нашли. надо проверить путь
	{
		CString path=GetDocPath(pFndDoc);
		if(m_path.IsEmpty())
			m_path=path;
		else if(m_path.CompareNoCase(path))
			pFndDoc=NULL;
	}
	else			// Не нашли. Надо искать по пути
	{
		if(!m_path.IsEmpty())
			pFndDoc=CConfigCont::PathToDocument(m_path);
	}
	if(!pFndDoc)
		SetError(E_FAIL,"Документ не найден. Возможно он был закрыт.");
	return pFndDoc;
}

ICfgDoc* CICfgDoc::CreateDoc(CDocument* pDoc)
{
	if(!pDoc)
		return NULL;
	DocTypes t=GetType(pDoc);
	switch(t)
	{
	case docWorkBook:
		return new CIWorkBook(pDoc);
	case docText:
		return new CITextDoc(pDoc);
	case docTable:
		return new CITableDoc(pDoc);
	case docDEdit:
		return new CIDialogDoc(pDoc);
	default:
		return new CICfgDoc(pDoc);
	};
}

CICfgDoc::CICfgDoc(CDocument* pDoc):m_pDoc(pDoc)
{
	m_pTemplate=pDoc->GetDocTemplate();
	m_path=GetDocPath(m_pDoc);
}
