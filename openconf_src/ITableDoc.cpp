// ITableDoc.cpp: implementation of the CITableDoc class.
#include "stdafx.h"
#include "configsvcimpl.h"
#include "ITableDoc.h"
ITypeInfo* CITableDoc::m_pTypeInfo=NULL;

CITableDoc::CITableDoc(CDocument* pDoc):CCfgDocDerived<ITableDoc,TableDoc>(pDoc)
{
}

CITableDoc::~CITableDoc()
{
}
typedef CPtrArray (CSheet::*PGetOut)(CSheetDirection);

STDMETHODIMP CITableDoc::SaveToFileAs(BSTR FileName,TableSaveTypes type,VARIANT_BOOL *bSucces)
{
	if(IsBadWritePtr(bSucces,sizeof(*bSucces)))
		return E_POINTER;
	CSheetDoc* pDoc=GetDoc();
	if(!pDoc)
		return E_FAIL;
	if(type<tbMxl || type>tbText)
		type=tbMxl;
	*bSucces=0;
	try{
		*bSucces=pDoc->SaveAs((LPCTSTR)_bstr_t(FileName),CSheetSaveAsType(type))?-1:0;
	}catch(...){
	}
	return S_OK;
}

STDMETHODIMP CITableDoc::LoadFromFile(BSTR FileName,VARIANT_BOOL *bSuccess)
{
	if(IsBadWritePtr(bSuccess,sizeof(*bSuccess)))
		return E_POINTER;
	CSheetDoc* pDoc=GetDoc();
	if(!pDoc)
		return E_FAIL;
	*bSuccess=0;
	try{
		CFile file((LPCTSTR)_bstr_t(FileName),CFile::modeRead);
		CArchive ar(&file,CArchive::load);
		pDoc->DeleteContents();
		pDoc->Serialize(ar);
		pDoc->SetModifiedFlag(TRUE);
		pDoc->UpdateAllViews(NULL);
		*bSuccess=-1;
	}
	catch(...){}
	return S_OK;
}

STDMETHODIMP CITableDoc::SaveToFile(BSTR FileName,VARIANT_BOOL *bSucces)
{
	return SaveToFileAs(FileName,tbMxl,bSucces);
}
