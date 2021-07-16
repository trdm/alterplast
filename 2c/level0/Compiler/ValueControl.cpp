// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueControl.cpp: implementation of the CValueControl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueControl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
//#include "1CDialog.h"

CMethods CValueControl::Methods;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueControl,CValue);
CValueControl::CValueControl()
{
	pWnd=NULL;
	nType=TYPE_STRING;//это чтобы для получения данных вызывался метод GetString
}

CValueControl::~CValueControl()
{

}

CValue Date(int cParam);
CValue CValueControl::GetValue(int)
{
	switch(nType)
	{
		case TYPE_NUMBER:
			return GetNumber();
		case TYPE_DATE:
			return Date(GetDate());
		case TYPE_STRING:
			return String(GetString());
	}
	return ValueByRef(this);//агрегатный объект - создаем новую переменную-ссылку
}

void CValueControl::SetValue(CValue &Val)
{
	SetString(Val.GetString());
};


//******************************
//Работа как агрегатного объекта
//******************************
enum
{
	enVisible=0,
	enEnable,
	enColor,
	enCaption,
	enEnableEdit,
	enMask,
	enClose,
	enModif,
	enFocus,
	enSetControlRect,
	enGetControlRect,
	enLastMethod
};

void CValueControl::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"Visible","Видимость"},
		{"Enable","Доступность"},
		{"Color","Цвет"},
		{"Caption","Заголовок"},
		{"EnableEdit","Редактирование"},
		{"Mask","Маска"},
		{"Close","Закрыть"},
//		{"Parameter","Параметр"},
		{"Modify","Модифицированность"},
		{"Activate","Активизировать"},
		{"SetControlRect","УстановитьКоординаты"},
		{"GetControlRect","ПолучитьКоординаты"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM,aMethods,nCountM);
}

CValue CValueControl::Method(int iName,CValue **p)
{
	if(!pWnd)
		Error("Не было привязки окна CWnd!");
	CValue Ret;
	switch(iName)
	{
		case enVisible:
			{
				Ret=Visible();
				if(p[0]->nType!=TYPE_EMPTY)
					Visible(p[0]->GetNumber());
				break;
			}
		case enEnable:
			{
				Ret=Enable();
				if(p[0]->nType!=TYPE_EMPTY)
					Enable(p[0]->GetNumber());
				break;
			}
		case enColor:
			{
				Ret=Color();
				if(p[0]->nType!=TYPE_EMPTY)
					Color(p[0]->GetNumber(),p[1]->GetNumber(),p[2]->GetNumber());
				break;
			}
		case enCaption:
			{
				Ret=String(Caption());
				if(p[0]->nType!=TYPE_EMPTY)
				{
					Caption(p[0]->GetString());
				}
				break;
			}
		case enEnableEdit:
			{
				Ret=EnableEdit();
				if(p[0]->nType!=TYPE_EMPTY)
					EnableEdit(p[0]->GetNumber());
				break;
			}
		case enMask:
			{
				//Ret=Mask();
				if(p[0]->nType!=TYPE_EMPTY)
					Ret=String(Mask(p[0]->GetString()));
				break;
			}
		case enClose:
			{
				Close();
				break;
			}
		case enModif:
			{
				int nSet=-1;
				if(p[0]->nType!=TYPE_EMPTY)
					nSet=p[0]->GetNumber();
				return Modify(nSet);
			}
		case enFocus:
			{
				((CWnd*)pWnd)->SetFocus();
			}
		case enSetControlRect:
			{
				int x, y, nWidth, nHeight;
				x=p[0]->GetNumber();
				y=p[1]->GetNumber();
				nWidth=p[2]->GetNumber();
				nHeight=p[3]->GetNumber();
				ASSERT(pWnd);
				((CWnd*)pWnd)->MoveWindow(x, y, nWidth, nHeight,1);
				((CWnd*)pWnd)->RedrawWindow();
				break;
			}
		case enGetControlRect:
			{
				CRect mRect(0,0,0,0);
				ASSERT(pWnd);
				((CWnd*)pWnd)->GetWindowRect(&mRect);
				CWnd* pParentWnd=((CWnd*)pWnd)->GetParent();
				pParentWnd->ScreenToClient(&mRect);
				*p[0] = CValue(mRect.left);
				*p[1] = CValue(mRect.top);
				*p[2] = CValue(mRect.right - mRect.left);
				*p[3] = CValue(mRect.bottom - mRect.top);
				break;
			}
	}
	return Ret;
}







NUMBER CValueControl::GetNumber(void) const
{
	return atof(GetString());
}
int CValueControl::GetDate(void)const
{
	return Date(GetString());
}

CString CValueControl::GetString(void)const
{
	if(!pWnd)
		return "<>";//SetError("Не определен указатель на объект диалога");

	CString csRes;
	CRuntimeClass* prt = pWnd->GetRuntimeClass();
	CString csName=prt->m_lpszClassName;
	if(csName=="CButton")
	{
		CButton *pObj=(CButton *)pWnd;
		int nCheck=pObj->GetCheck();
		csRes.Format("%d",nCheck);
	}
	else
	if(csName=="CComboBox")
	{
		CComboBox *pObj=(CComboBox *)pWnd;
		int i=pObj->GetCurSel();
		if(i!=LB_ERR)
			pObj->GetLBText(i,csRes);
	}
	else
	if(csName=="CListBox")
	{
		CListBox *pObj=(CListBox *)pWnd;

		int i=pObj->GetCurSel();
		if(i!=LB_ERR)
			pObj->GetText(i,csRes);
	}
	else
	if(csName=="CEdit"||csName=="CDropEdit"||csName=="CCalcEdit")
	{
		CEdit *pObj=(CEdit *)pWnd;
		pObj->GetWindowText(csRes);
		csRes.Replace("\r\n","\n");
	}
	else
	if(csName=="CStatic")
	{
		CStatic *pObj=(CStatic *)pWnd;
		pObj->GetWindowText(csRes);
	}
	else
	if(csName=="CGroup")
	{
		CButton *pObj=(CButton *)pWnd;
		pObj->GetWindowText(csRes);
	}
	else
	if(csName=="CDateTimeCtrl")
	{
		CDateTimeCtrl *pObj=(CDateTimeCtrl *)pWnd;
		pObj->GetWindowText(csRes);
	}
	else
	if(csName=="CRadio")
	{
		CButton *pObj=(CButton *)pWnd;
		HWND hParent=GetParent(pObj->m_hWnd);
		if(hParent)
		{
			int n1=pObj->GetDlgCtrlID();
			int n2=n1;
			
			//определяем n2
		    for(int nID=n1+1;nID<n1+MAX_RADIO;nID++)
			{
				CWnd* pWnd=CWnd::FromHandle(hParent)->GetDlgItem(nID);
				if(!pWnd)
					break;

				CRuntimeClass* prt = pWnd->GetRuntimeClass();
				if(prt->m_lpszClassName!="CRadio")
					break;
				if((1<<17)&pWnd->GetStyle())
					break;
				n2=nID;
			}
			int nCheck=CWnd::FromHandle(hParent)->GetCheckedRadioButton(n1,n2);
			nCheck=nCheck-n1+1;
			if(nCheck<0)
				nCheck=0;
			if(nCheck>n2-n1+1)
				nCheck=0;
			csRes.Format("%d",nCheck);
		}
	}
	else
	if(csName=="CProgressCtrl")
	{
		CProgressCtrl *pObj=(CProgressCtrl *)pWnd;
		csRes.Format("%d",pObj->GetPos());
	}
	else
	if(csName=="CSliderCtrl")
	{
		CSliderCtrl *pObj=(CSliderCtrl *)pWnd;
		csRes.Format("%d",pObj->GetPos());
	}


	return csRes;
};

void CValueControl::SetString(CString csParam)
{
	if(!pWnd)
		return;//SetError("Не определен указатель на объект диалога");

	CRuntimeClass* prt = pWnd->GetRuntimeClass();
	CString csName=prt->m_lpszClassName;
	if(csName=="CButton")
	{
		CButton *pObj=(CButton *)pWnd;
		int nValue=atoi(csParam);
		if(nValue)
			pObj->SetCheck(true);
		else
			pObj->SetCheck(false);
	}
	else
	if(csName=="CComboBox")
	{
		SetError("Данному типу переменной нельзя присвоить значение");
/*
		CComboBox *pObj=(CComboBox *)pWnd;
		int nCurrentLine=pObj->FindString(-1,csStr);
		if(nCurrentLine<0)
				nCurrentLine=0;
		pObj->SetCurSel(nCurrentLine);
		pObj->GetLBText(nCurrentLine,csStr);
*/
	}
	else
	if(csName=="CEdit"||csName=="CDropEdit"||csName=="CCalcEdit")
	{
		CEdit *pObj=(CEdit *)pWnd;
		csParam.Replace("\n","\r\n");
		pObj->SetWindowText(csParam);
	}
	else
	if(csName=="CStatic")
	{
		CStatic *pObj=(CStatic *)pWnd;
		pObj->SetWindowText(csParam);
//		COXStaticText *pObj=(COXStaticText *)pWnd;
//		pObj->SetWindowText(csParam,1);
	}
	else
	if(csName=="CGroup")
	{
		CButton *pObj=(CButton *)pWnd;
		pObj->SetWindowText(csParam);
	}
	else
	if(csName=="CListBox")
	{
		SetError("Данному типу переменной нельзя присвоить значение");
	}
	else
	if(csName=="CDateTimeCtrl")
	{
		if(!csParam.IsEmpty())
		{
			CDateTimeCtrl *pObj=(CDateTimeCtrl *)pWnd;
			int nYear;
			int nMonth;
			int nDay;
			Date(csParam,nYear,nMonth,nDay);
			// set with a COleDateTime object
			COleDateTime oleTime(nYear, nMonth, nDay, 0, 0, 0);
			//if(timeNew.GetStatus() != COleDateTime::invalid);

			pObj->SetTime(oleTime);
		}
	}
	else
	if(csName=="CRadio")
	{
		CButton *pObj=(CButton *)pWnd;
		int nValue=atoi(csParam);
		HWND hParent=GetParent(pObj->m_hWnd);
		if(hParent)
		{
			int n1=pObj->GetDlgCtrlID();
			int n2=n1;

			//определяем n2
		    for(int nID=n1+1;nID<n1+MAX_RADIO;nID++)
			{
				CWnd* pWnd=CWnd::FromHandle(hParent)->GetDlgItem(nID);
				if(!pWnd)
					break;

				CRuntimeClass* prt = pWnd->GetRuntimeClass();
				if(prt->m_lpszClassName!="CRadio")
					break;
				if((1<<17)&pWnd->GetStyle())
					break;
				n2=nID;
			}
						
			CWnd::FromHandle(hParent)->CheckRadioButton(n1,n2,n1+nValue-1);
		}
	}
	else
	if(csName=="CProgressCtrl")
	{
		CProgressCtrl *pObj=(CProgressCtrl *)pWnd;
		pObj->SetPos(atoi(csParam));
	}
	else
	if(csName=="CSliderCtrl")
	{
		CSliderCtrl *pObj=(CSliderCtrl *)pWnd;
		pObj->SetPos(atoi(csParam));
	}
	else
		((CWnd*)pWnd)->SetWindowText(csParam);

}

void *CValueControl::GetAttach()
{
	return pWnd;
};
void CValueControl::Attach(void *pObj)
{
	pWnd=(CObject*)pObj;
	ASSERT(pWnd);

	if(!pWnd)
		return;//SetError("Не определен указатель на объект диалога");

	CRuntimeClass* prt = pWnd->GetRuntimeClass();
	CString csName=prt->m_lpszClassName;
	if(csName=="CDateTimeCtrl")
	{
		nType=TYPE_DATE;
	}

}
void CValueControl::Attach(int nNewCtlID, CObject* pNewParentWnd)
{
	ASSERT(pNewParentWnd);
	ASSERT(nNewCtlID);
	pWnd=((CWnd*)pNewParentWnd)->GetDlgItem(nNewCtlID);
	ASSERT(pWnd);
}
void CValueControl::Detach()
{
	pWnd=0;
	nType=0;
}

CString CValueControl::GetTypeString(void)const
{
	if(!pWnd)
		return NOT_DEFINED;//SetError("Не определен указатель на объект диалога");
	if(nType==TYPE_DATE)
		return "Дата";
	else
		return "Строка";
/*
	CRuntimeClass* prt = pWnd->GetRuntimeClass();
	CString csName=prt->m_lpszClassName;
	if(
		csName=="CStatic"||
		csName=="CEdit"||
		csName=="CDropEdit"
		)
		return "Строка";
	else
	if(csName=="CListBox"||csName=="CComboBox")
		return "СписокЗначений";
	else
	if(csName=="CDateTimeCtrl")
		return "Дата";
	else
	if(
		csName=="CProgressCtrl"||
		csName=="CSliderCtrl"||
		csName=="CRadio"||
		csName=="CSliderCtrl"
		)
		return "Число";
	else
		return "Диалоговый элемент";
*/
};

#define PWINDOW ((CWnd*)pWnd)
#define PWINDOW2 ((CXColorStatic*)pWnd)
void CValueControl::Caption(CString Str)
{
	if(pWnd)
	{
		PWINDOW->SetWindowText(Str);
	}
};






//ПОДДЕРЖКА ТЕКСТА
IMPLEMENT_DYNCREATE(CValueStatic,CValueControl);

//ПОДДЕРЖКА ПОЛЕЙ РЕДАКТИРОВАНИЯ
IMPLEMENT_DYNCREATE(CValueEdit,CValueControl);
IMPLEMENT_DYNCREATE(CValueNumber,CValueEdit);
IMPLEMENT_DYNCREATE(CValueInt,CValueControl);

//ПОДДЕРЖКА КНОПОК
IMPLEMENT_DYNCREATE(CValueButton,CValueControl);
IMPLEMENT_DYNCREATE(CValueRadio,CValueControl);
IMPLEMENT_DYNCREATE(CValueGroup,CValueButton);
IMPLEMENT_DYNCREATE(CValueCheck,CValueButton);


//ДАТА
IMPLEMENT_DYNCREATE(CValueDate,CValueControl);
CValueDate::CValueDate()
{
	nType=TYPE_DATE;
}

CValueDate::~CValueDate()
{

}

int CValueDate::GetDate(void)const
{
	return (int)Date(GetString());
}






