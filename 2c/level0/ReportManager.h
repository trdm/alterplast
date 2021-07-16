// Авторские права - VTOOLS.RU (info@vtools.ru)
// ReportManager.h: interface for the CReportManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORTMANAGER_H__19C6E1AA_7CFE_4D02_89E8_8CCF0E0BDBFC__INCLUDED_)
#define AFX_REPORTMANAGER_H__19C6E1AA_7CFE_4D02_89E8_8CCF0E0BDBFC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


class CReportElement
{
public:
	CString csName;
	CString csDescription;
	bool	bCanVisible;
	int		nImage;

	CReportElement()
	{
		bCanVisible=0;
		nImage=0;
	};
};

class CReportManager : public CDocManager  
{
public:
	CReportManager();
	virtual ~CReportManager();
	CReportManager(CString csName,BOOL bSmallDialog=FALSE)
	{ 
		csTitle=csName;
		bSmall=bSmallDialog;
	};

public:
	CString csTitle;
	CArray <CReportElement,CReportElement> aDescription;
	BOOL bSmall;//признак маленького диалога выбора


	//CDocument* OpenDocumentFile(LPCTSTR lpszFileName);
	void AddDocTemplate(CMultiDocTemplate *p,CString csName,CString csDescription,bool bCanView,int nImage=0);
	void OnFileNew();
	CDocument* OpenForm(CString csName,BOOL bVisible=TRUE);//NULL-неудача
	BOOL DoPromptFileName(CString& fileName, UINT nIDSTitle, DWORD lFlags, BOOL bOpenFileDialog, CDocTemplate* pTemplate);

};

#endif // !defined(AFX_REPORTMANAGER_H__19C6E1AA_7CFE_4D02_89E8_8CCF0E0BDBFC__INCLUDED_)
