// ITextDoc.cpp: implementation of the CITextDoc class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ITextDoc.h"
ITypeInfo* CITextDoc::m_pTypeInfo=NULL;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CITextDoc::CITextDoc(CDocument* pDoc):CCfgDocDerived<ITextDoc,TextDoc>(pDoc)
{
}

CITextDoc::~CITextDoc()
{
}

STDMETHODIMP CITextDoc::SaveToFile(BSTR FileName,VARIANT_BOOL *bSucces)
{
	if(IsBadWritePtr(bSucces,sizeof(*bSucces)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*bSucces=0;
	try{
		CFile file(_bstr_t(FileName),CFile::modeWrite|CFile::modeCreate);
		CString txt;
		pDoc->GetText(txt);
		file.Write((LPCTSTR)txt,txt.GetLength());
		*bSucces=-1;
	}
	catch(...){}
	return S_OK;
}

STDMETHODIMP CITextDoc::LoadFromFile(BSTR FileName,VARIANT_BOOL *bSuccess)
{
	if(IsBadWritePtr(bSuccess,sizeof(*bSuccess)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*bSuccess=0;
	try
	{
		CFile file((LPCTSTR)_bstr_t(FileName),CFile::modeRead);
		CString txt;
		DWORD s=file.GetLength();
		file.Read(txt.GetBuffer(s),s);
		txt.ReleaseBuffer();
		pDoc->SetText(txt);
		*bSuccess=-1;
	}catch(...){}
	return S_OK;
}

STDMETHODIMP CITextDoc::get_LineLen(long LineNum,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	if(LineNum<0 || LineNum>=pDoc->GetLineCount())
		return SetError(E_FAIL,"Недопустимый номер строки: %i",LineNum);
	*pVal=pDoc->GetLineLength(LineNum);
	return S_OK;
}
static long GetLongFromV(VARIANT& v,long def)
{
	if((v.vt==VT_ERROR && v.scode==DISP_E_PARAMNOTFOUND) || v.vt==VT_EMPTY)
		return def;
	if(v.vt!=VT_I4)
		VariantChangeType(&v,&v,0,VT_I4);
	return v.vt==VT_I4?v.lVal:def;
}
static void SetSelection(CTextDocument* pDoc,long StartLine,VARIANT& StartCol,
								  VARIANT& EndLine,VARIANT& EndCol)
{
	int lncount=pDoc->GetLineCount()-1;
	if(StartLine<0)
		StartLine=0;
	else if(StartLine>lncount)
		StartLine=lncount;

	int lineLen=pDoc->GetLineLength(StartLine);
	long col1=GetLongFromV(StartCol,0);
	if(col1>lineLen)
		col1=lineLen;
	if(col1<0)
		col1=0;

	long lin2=GetLongFromV(EndLine,StartLine);
	if(lin2>lncount)
		lin2=lncount;
	if(lin2<0)
		lin2=0;
	lineLen=pDoc->GetLineLength(lin2);
	long col2=GetLongFromV(EndCol,lineLen);
	if(col2>lineLen)
		col2=lineLen;
	if(col2<0)
		col2=0;
	pDoc->SetSel(CPoint(col1,StartLine),CPoint(col2,lin2));
}

STDMETHODIMP CITextDoc::get_Range(long StartLine,VARIANT StartCol,
								  VARIANT EndLine,VARIANT EndCol,BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	SetSelection(pDoc,StartLine,StartCol,EndLine,EndCol);
	CString txt;
	pDoc->GetSelectedText(txt);
	*pVal=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CITextDoc::put_Range(long StartLine,VARIANT StartCol,
								  VARIANT EndLine,VARIANT EndCol,BSTR newVal)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	SetSelection(pDoc,StartLine,StartCol,EndLine,EndCol);
	pDoc->Replace(_bstr_t(newVal));
	return S_OK;
}

bool CITextDoc::GetSelection(long num,long* res)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return false;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return false;
	CString txt;
	pEd->GetSelText(txt);
	CPoint pt1,pt2;
	pDoc->GetSel(pt1,pt2);
	switch(num)
	{
	case 0:
		*res=pt1.y;
		break;
	case 1:
		*res=pt1.x;
		break;
	case 2:
		*res=pt2.y;
		break;
	case 3:
		*res=pt2.x;
		break;
	}
	return true;
}

STDMETHODIMP CITextDoc::get_SelStartLine(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(GetSelection(0,pVal))
		return S_OK;
	return E_FAIL;
}

STDMETHODIMP CITextDoc::get_SelStartCol(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(GetSelection(1,pVal))
		return S_OK;
	return E_FAIL;
}

STDMETHODIMP CITextDoc::get_SelEndLine(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(GetSelection(2,pVal))
		return S_OK;
	return E_FAIL;
}

STDMETHODIMP CITextDoc::get_SelEndCol(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	if(GetSelection(3,pVal))
		return S_OK;
	return E_FAIL;
}

STDMETHODIMP CITextDoc::get_CurrentWord(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	CString txt;
	pEd->GetCurrentWord(txt);
	*pVal=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CITextDoc::MoveCaret(long LineStart,long ColStart,
								  VARIANT LineEnd,VARIANT ColEnd)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	long lncount=pDoc->GetLineCount()-1;
	if(LineStart>lncount)
		LineStart=lncount;
	if(LineStart<0)
		LineStart=0;
	long lnlen=pDoc->GetMaxLineLen();

	if(ColStart>lnlen)
		ColStart=lnlen;
	if(ColStart<0)
		ColStart=0;

	long lnEnd=GetLongFromV(LineEnd,LineStart);

	if(lnEnd>lncount)
		lnEnd=lncount;
	if(lnEnd<0)
		lnEnd=0;
	
	long colEnd=GetLongFromV(ColEnd,ColStart);
	if(colEnd>lnlen)
		colEnd=lnlen;
	if(colEnd<0)
		colEnd=0;
	pEd->MoveCaret(CPoint(ColStart,LineStart),CPoint(colEnd,lnEnd),TRUE);
	return S_OK;
}

STDMETHODIMP CITextDoc::get_Text(BSTR *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CString txt;
	pDoc->GetText(txt);
	*pVal=txt.AllocSysString();
	return S_OK;
}

STDMETHODIMP CITextDoc::put_Text(BSTR newVal)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	pDoc->SetText((LPCTSTR)_bstr_t(newVal));
	return S_OK;
}

STDMETHODIMP CITextDoc::get_LineCount(long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->GetLineCount();
	return S_OK;
}

STDMETHODIMP CITextDoc::get_IsModule(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->IsModule()?-1:0;
	return S_OK;
}

STDMETHODIMP CITextDoc::put_IsModule(VARIANT_BOOL newVal)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	return S_OK;
}

STDMETHODIMP CITextDoc::get_ReadOnly(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->IsReadOnly()?-1:0;
	return S_OK;
}

STDMETHODIMP CITextDoc::put_ReadOnly(VARIANT_BOOL newVal)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	pDoc->SetReadOnly(newVal?TRUE:FALSE);
	return S_OK;
}

STDMETHODIMP CITextDoc::get_BookMark(long LineNum,VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	if(LineNum<0 || LineNum>=pDoc->GetLineCount())
		return SetError(E_FAIL,"Неправильный номер строки: %i",LineNum);
	*pVal=pDoc->IsSetBookMark(LineNum)?-1:0;
	return S_OK;
}

STDMETHODIMP CITextDoc::put_BookMark(long LineNum,VARIANT_BOOL newVal)
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	if(LineNum<0 || LineNum>=pDoc->GetLineCount())
		return SetError(E_FAIL,"Неправильный номер строки: %i",LineNum);
	pDoc->SetBookMark(LineNum,newVal?TRUE:FALSE);
	return S_OK;
}

STDMETHODIMP CITextDoc::NextBookmark(long StartLine,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	if(StartLine<0 || StartLine>=pDoc->GetLineCount())
		return SetError(E_FAIL,"Неправильный номер строки: %i",StartLine);
	*pVal=pDoc->GetNextBookMark(StartLine);
	return S_OK;
}

STDMETHODIMP CITextDoc::PrevBookmark(long StartLine,long *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	if(StartLine<0 || StartLine>=pDoc->GetLineCount())
		return SetError(E_FAIL,"Неправильный номер строки: %i",StartLine);
	*pVal=pDoc->GetPrevBookMark(StartLine);
	return S_OK;
}

STDMETHODIMP CITextDoc::ClearAllBookMark()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	pDoc->ClearAllBookMarks();
	return S_OK;
}

STDMETHODIMP CITextDoc::get_CanUndo(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->CanUndo()?-1:0;
	return S_OK;
}

STDMETHODIMP CITextDoc::get_CanRedo(VARIANT_BOOL *pVal)
{
	if(IsBadWritePtr(pVal,sizeof(*pVal)))
		return E_POINTER;
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	*pVal=pDoc->CanRedo()?-1:0;
	return S_OK;
}

STDMETHODIMP CITextDoc::Undo()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	pDoc->Undo();
	return S_OK;
}

STDMETHODIMP CITextDoc::Redo()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	pDoc->Redo();
	return S_OK;
}

STDMETHODIMP CITextDoc::Cut()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,ID_EDIT_CUT);
	return S_OK;
}

STDMETHODIMP CITextDoc::Copy()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,ID_EDIT_COPY);
	return S_OK;
}

STDMETHODIMP CITextDoc::Paste()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,ID_EDIT_PASTE);
	return S_OK;
}

STDMETHODIMP CITextDoc::CommentSel()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,33908);
	return S_OK;
}

STDMETHODIMP CITextDoc::UncommentSel()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,33909);
	return S_OK;
}

STDMETHODIMP CITextDoc::FormatSel()
{
	CTextDocument* pDoc=GetTD();
	if(!pDoc)
		return E_FAIL;
	CTextEditor* pEd=GetView(pDoc);
	if(!pEd)
		return E_FAIL;
	pEd->SendMessage(WM_COMMAND,33289);
	return S_OK;
}
