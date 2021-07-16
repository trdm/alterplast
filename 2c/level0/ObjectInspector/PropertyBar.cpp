// PropertyBar.cpp : implementation file
//

#include "stdafx.h"
//#include "enterprise.h"
#include "PropertyBar.h"
#include "..\SourceUT\OXMDIFloatWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar dialog

//CPropertyBar *pMessageWindow=NULL;//для вызова из языка
CPropertyBar *afx_pProperty=0;
CPropertyBar *AfxGetProperty()
{
	if(CONFIG_MODE==afxAppRunMode)
		return afx_pProperty;
	else
		return 0;
}

CPropertyBar::CPropertyBar(CWnd* pParent /*=NULL*/)
{
	//{{AFX_DATA_INIT(CPropertyBar)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	//pMessageWindow=this;
	afx_pProperty=this;
	pDocUpdate=0;
	pDocData=0;
	pDocType=0;

	pCommonParent=0;

}


void CPropertyBar::DoDataExchange(CDataExchange* pDX)
{
	PropertyBarParent::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CPropertyBar)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CPropertyBar, PropertyBarParent)
	//{{AFX_MSG_MAP(CPropertyBar)
	ON_COMMAND(WM_CLEAR, Clear)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPropertyBar message handlers


void CPropertyBar::Clear()
{
	CleanUp();
	aParentList.RemoveAll();
	RedrawWindow();
	pDocType=0;
	pDocUpdate=0;
	ReadOnly(0);
}
void CPropertyBar::PostClear()
{
	PostMessage(WM_COMMAND,WM_CLEAR,0);
}

CObjectInspector::CProperty *CPropertyBar::GetParentProp(CString csParentName)
{
	CObjectInspector::CProperty *pProp=(CObjectInspector::CProperty *)aParentList[csParentName];
	if(!pProp)
	{
		pProp = new CObjectInspector::CProperty(csParentName);
		aParentList[csParentName]=pProp;
		//pCommonParent->AddProperty(pProp);
		CObjectInspector::AddProperty (pProp);
		pProp->Expand ();
	}
	return pProp;
}
//TODO - переделать в шаблонные функции
//_________________________________________________________________________________________________________
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,int &Value, int nMin, int nMax)
{
	GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value,nMin,nMax));
}
//_________________________________________________________________________________________________________
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,double &Value)
{
	GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value));
}
//_________________________________________________________________________________________________________
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,bool &Value)
{
	GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value));
}

//_________________________________________________________________________________________________________
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,COLORREF &Value)
{
	GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value));
}

//_________________________________________________________________________________________________________
//ввод строки из списка строк
//(либо произвольной строки,если в встретилось выражение "...")
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,CString &Value,CString csValueStr)
{
	if(csValueStr.IsEmpty())
	{
		GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value));
	}
	else
	{
		CStringList *pStringList=new CStringList();
		csValueStr+=",";
		int nIndexPrev=-1;
		int nIndex=csValueStr.Find(",");
		while(nIndex>=0)
		{
			CString csStr=csValueStr.Mid(nIndexPrev+1,nIndex-nIndexPrev-1);
			if(csStr!="...")
				pStringList->AddTail(csStr);

			nIndexPrev=nIndex;
			nIndex=csValueStr.Find(",",nIndex+1);
		}
		int nStyle=CBS_DROPDOWNLIST;
		if(csValueStr.Find("...")>=0)
			nStyle=CBS_DROPDOWN;

		GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value,pStringList,-1,nStyle));
	}
}

//_________________________________________________________________________________________________________
//ввод числа из списка строк
void CPropertyBar::AddProperty(CString csParentName,CString csValueName,byte &Value,CString csValueStr)
{
	if(csValueStr.IsEmpty())
	{
		return;//GetParentProp(csParentName)->AddProperty (new CObjectInspector::CProperty(csValueName,&Value));
	}
	else
	{
	//	CObjectInspector::CProperty *pProp = new CObjectInspector::CProperty(csValueName);
	//	GetParentProp(csParentName)->AddProperty (pProp);
	//	pProp->Expand ();

		csValueStr+=",";
		int nIndexPrev=-1;
		int nIndex=csValueStr.Find(",");
		while(nIndex>=0)
		{
			CString Str=csValueStr.Mid(nIndexPrev+1,nIndex-nIndexPrev-1);
			GetParentProp(csValueName)->AddProperty (new CObjectInspector::CProperty(Str,true,&Value));
			GetParentProp(csValueName)->Collapse();

			nIndexPrev=nIndex;
			nIndex=csValueStr.Find(",",nIndex+1);
		}
	}
}


/*

//-------------------------------------------------------------------
// respond to this event as we need to override it
void CPropertyBar::OnSizedOrDocked(int cx, int cy, BOOL bFloating, int flags)
{
	UNREFERENCED_PARAMETER(bFloating);
	UNREFERENCED_PARAMETER(flags);
	UNREFERENCED_PARAMETER(cx);
	UNREFERENCED_PARAMETER(cy);

	CRect rect;
	GetClientRect(rect);
	m_Property.MoveWindow(&rect);

}




void CPropertyBar::OnSize(UINT nType, int cx, int cy) 
{
	OnSizedOrDocked(cx, cy, FALSE, 0);
	PropertyBarParent::OnSize(nType, cx, cy);
}

*/
void SendUpDate(CDocument *pDocUpdate,int nMode)
{
	if(pDocUpdate)
	{
		//if(!nMode)
		pDocUpdate->SetModifiedFlag(1);
		//else
		{
			POSITION pos=pDocUpdate->GetFirstViewPosition();
			if(pos)
			{
				CView* pView = pDocUpdate->GetNextView(pos);
				if(pView)
				{
					pView->SendMessage(UPDATE_PROPERTY,0,0);
				}
			}
		}
		
	}

}
BOOL CPropertyBar::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
	if (wParam == IDC_OI)
	{
	 	CObjectInspector::NM_OIVIEW *nmHdr = (CObjectInspector::NM_OIVIEW*) lParam;

		if (nmHdr->hdr.code == OIVN_ITEMCHANGED) 
			SendUpDate(pDocUpdate,1);

		//if (nmHdr->hdr.code == OIVN_ITEMCHANGING) 
		//	SendUpDate(pDocUpdate,1);

	}
	
	return PropertyBarParent::OnNotify(wParam, lParam, pResult);
}

void CPropertyBar::ReleaseDoc(CDocument *pDoc)
{
	if(pDoc==pDocUpdate) 
		Clear();
};
void CPropertyBar::SetDoc(CDocument *pSetDocUpdate,LPARAM pSetDocData,LPARAM pSetDocType,BOOL bReadOnly)
{
	Clear();
	pDocUpdate=pSetDocUpdate;
	pDocData=pSetDocData;
	pDocType=pSetDocType;
	ReadOnly(bReadOnly);
};
