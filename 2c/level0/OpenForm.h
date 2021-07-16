//Авторские права - VTOOLS.RU (info@vtools.ru)
#if !defined(AFX_OPENFORM_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_)
#define AFX_OPENFORM_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

int OpenFormExt(CString csFormName0,CValue &vContext,CValue Param=0,CString csFileName="",CString csFormID="",int nMode=0,  CValue vChoiceContext=0,int nChoiceMode=0,CValue vCurVal=0, class CMicroForm* pControl=0,CWnd *pParent=0);
CDocument* OpenFormMDI(CString csClassName,int bVisible=1,CMetadataTree *pMetaTree=0,CMetaObject *pMetaObj=0);


class CLock
{
public:
	CLock(CRITICAL_SECTION& cs, const CString& strFunc, BOOL &bInit)
	{
		if(!bInit)
		{
			InitializeCriticalSection(&cs);
			bInit=1;
		}

		m_strFunc = strFunc;
		m_pcs = &cs;
		Lock();
	}
	~CLock()
	{
		Unlock();

	}
	
	void Unlock()
	{
		LeaveCriticalSection(m_pcs);
		TRACE(_T("LC %d %s\n") , GetCurrentThreadId() , m_strFunc);
	}

	void Lock()
	{
		TRACE(_T("EC %d %s\n") , GetCurrentThreadId(), m_strFunc);
		EnterCriticalSection(m_pcs);
	}


protected:
	CRITICAL_SECTION*	m_pcs;
	CString				m_strFunc;

};



#endif // !defined(AFX_OPENFORM_H__C022A143_B09B_4A8D_B9EB_830DBF155662__INCLUDED_)
