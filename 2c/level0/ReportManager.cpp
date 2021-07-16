//Авторские права - VTOOLS.RU (info@vtools.ru)
// ReportManager.cpp: implementation of the CReportManager class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ReportManager.h"
#include "Compiler\ChooseValue.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CReportManager::CReportManager()
{
	csTitle="Выбор";
	bSmall=FALSE;
}
 
CReportManager::~CReportManager()
{

}

void CReportManager::OnFileNew()
{
	CChooseValue dlg;
	CArray <CDocTemplate*,CDocTemplate*> listTemlate;
	POSITION pos=GetFirstDocTemplatePosition();
	for(int i=0;i<aDescription.GetSize();i++)
	{
		CDocTemplate* pTemplate=GetNextDocTemplate(pos);
		if(aDescription[i].bCanVisible)
		{
			dlg.List.Add(aDescription[i].csDescription);
			dlg.ListImage.Add(aDescription[i].nImage);
			listTemlate.Add(pTemplate);
		}
	}

	dlg.nStart=0;
	dlg.csTitle=csTitle;
	dlg.nMode=0;
	if(bSmall)
		dlg.Small();

	int nResponse = IDOK;
	int nIndex=0;
	if(dlg.List.GetSize()>1)
	{
		nResponse = dlg.DoModal();
		nIndex=dlg.nRes;
	}
	if (nResponse == IDOK)
	{
		if(listTemlate.GetSize()>0)
		if(listTemlate[nIndex])
			listTemlate[nIndex]->OpenDocumentFile(NULL);
	}
}
void CReportManager::AddDocTemplate(CMultiDocTemplate *p,CString csName,CString csDescription,bool bCanView,int nImage)
{
	CDocManager::AddDocTemplate(p);
	csName.TrimLeft();
	csName.TrimRight();
	
	CReportElement data;
	data.csName=csName;
	data.csDescription=csDescription;
	data.bCanVisible=bCanView;
	data.nImage=nImage;

	aDescription.Add(data);
}

CDocument* CReportManager::OpenForm(CString csName,BOOL bVisible)
{
	if(!AfxGetMainWnd())
		return 0;

	POSITION pos=GetFirstDocTemplatePosition();
	CString csFind=mUpper(csName);
	CDocTemplate* pTemplate=NULL;
	BOOL bFind=FALSE;
	for(int i=0;i<aDescription.GetSize();i++)
	{
		pTemplate=GetNextDocTemplate(pos);

		CString csCurName=aDescription[i].csName;
		if(csFind==mUpper(csCurName))
		{
			bFind=TRUE;
			break;
		}
	}
	if(bFind)
	{
		if(pTemplate)
		{
			CDocument* pDoc=pTemplate->OpenDocumentFile(NULL,bVisible);
			((CEnterpriseApp*)AfxGetApp())->DeleteConfigName();
			return pDoc;
		}
	}
	return NULL;
}
void AFXAPI _AfxAppendFilterSuffix(CString& filter, OPENFILENAME& ofn,
	CDocTemplate* pTemplate, CString* pstrDefaultExt)
{
	ASSERT_VALID(pTemplate);
	ASSERT_KINDOF(CDocTemplate, pTemplate);

	CString strFilterExt, strFilterName;
	if (pTemplate->GetDocString(strFilterExt, CDocTemplate::filterExt) &&
	 !strFilterExt.IsEmpty() &&
	 pTemplate->GetDocString(strFilterName, CDocTemplate::filterName) &&
	 !strFilterName.IsEmpty())
	{
		// a file based document template - add to filter list
		ASSERT(strFilterExt[0] == '.');
		if (pstrDefaultExt != NULL)
		{
			// set the default extension
			*pstrDefaultExt = ((LPCTSTR)strFilterExt) + 1;  // skip the '.'
			ofn.lpstrDefExt = (LPTSTR)(LPCTSTR)(*pstrDefaultExt);
			ofn.nFilterIndex = ofn.nMaxCustFilter + 1;  // 1 based number
		}

		// add to filter
		filter += strFilterName;
		ASSERT(!filter.IsEmpty());  // must have a file type name
		filter += (TCHAR)'\0';  // next string please
		filter += (TCHAR)'*';
		filter += strFilterExt;
		filter += (TCHAR)'\0';  // next string please
		ofn.nMaxCustFilter++;
	}
}
//*/
BOOL CReportManager::DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate)
{
	CFileDialog dlgFile(bOpenFileDialog);

	CString title;
	VERIFY(title.LoadString(nIDSTitle));

	dlgFile.m_ofn.Flags |= lFlags;

	CString strFilter;
	CString strDefault;
	if (pTemplate != NULL)
	{
		ASSERT_VALID(pTemplate);
		_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate, &strDefault);
	}
	else
	{
		// do for all doc template
		POSITION pos = m_templateList.GetHeadPosition();
		BOOL bFirst = TRUE;
		while (pos != NULL)
		{
			CDocTemplate* pTemplate = (CDocTemplate*)m_templateList.GetNext(pos);
			_AfxAppendFilterSuffix(strFilter, dlgFile.m_ofn, pTemplate,
				bFirst ? &strDefault : NULL);
			bFirst = FALSE;
		}
	}

	CString allFilter;

	// append the "*.html" files filter
	if(mUpper(strDefault)=="2C")
	{
		allFilter="Файлы HTML (*.htm)";
		strFilter += allFilter;
		strFilter += (TCHAR)'\0';   // next string please
		strFilter += _T("*.htm");
		strFilter += (TCHAR)'\0';   // last string
		dlgFile.m_ofn.nMaxCustFilter++;
	}

	// append the "*.*" all files filter
	VERIFY(allFilter.LoadString(AFX_IDS_ALLFILTER));
	strFilter += allFilter;
	strFilter += (TCHAR)'\0';   // next string please
	strFilter += _T("*.*");
	strFilter += (TCHAR)'\0';   // last string
	dlgFile.m_ofn.nMaxCustFilter++;


	dlgFile.m_ofn.lpstrFilter = strFilter;
	dlgFile.m_ofn.lpstrTitle = title;
	dlgFile.m_ofn.lpstrFile = fileName.GetBuffer(_MAX_PATH);

	int nResult = dlgFile.DoModal();
	fileName.ReleaseBuffer();
	return nResult == IDOK;
}
