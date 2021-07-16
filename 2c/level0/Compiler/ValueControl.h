// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueControl.h: interface for the CValueControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUECONTROL_H__C05903D7_64B8_4117_BA65_7C1CF59AD752__INCLUDED_)
#define AFX_VALUECONTROL_H__C05903D7_64B8_4117_BA65_7C1CF59AD752__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Value.h"
#include "../GUI/XColorStatic.h"


#define MAX_RADIO  100 // максимальное количество элементов у переключателя (RADIO)

//поддержка диалоговых эоементов
class CValueControl : public CValue  
{
DECLARE_DYNCREATE(CValueControl);
public:
	CValueControl();
	virtual ~CValueControl();

	CObject *pWnd;
	void Attach(void *pObj);
	void Detach();
	void Attach(int nNewCtlID, CObject* pNewParentWnd);
	void *GetAttach();
	virtual int Modify(int){return 0;};

	virtual void SetValue(CValue &Val);
	virtual void SetString(CString);

	virtual CValue GetValue(int nThis=0);
	virtual NUMBER GetNumber(void) const;
	virtual CString GetString(void) const;
	virtual int GetDate(void)const;

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//эти методы нужно переопределить в ваших агрегатных объектах:
	virtual CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	virtual void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	virtual CValue Method(int iName,CValue **aParams);//вызов метода
	//virtual CValue &Attribute(int iName);//вызов атрибута

	CString GetTypeString(void)const;
	//void Init(CString StrVid){csObjectName=StrVid;};


#define PWINDOW ((CWnd*)pWnd)
	//ПРЯМОЕ УПРАВЛЕНИЕ ДИАЛОГОВЫМ ЭЛЕМЕНТОМ
	//Видимость
	virtual int Visible()
	{
		if(pWnd)
		return PWINDOW->IsWindowVisible();
		return 0;
	};
	virtual int Visible(BOOL bStat)
	{
		if(pWnd)
		{
		int nRes=Visible();
		PWINDOW->ShowWindow(bStat);
		return nRes;
		}
		return 0;
	};

	//Доступность
	virtual int Enable()
	{
		if(pWnd)
		return PWINDOW->IsWindowEnabled();
		return 0;
	};
	virtual int Enable(BOOL bStat)
	{
		if(pWnd)
		{
		int nRes=Enable();
		PWINDOW->EnableWindow(bStat);
		return nRes;
		}
		return 0;
	};

	//Заголовок
	virtual CString Caption()
	{
		CString csRes;
		if(pWnd)
		PWINDOW->GetWindowText(csRes);
		return csRes;
	};
	virtual void Caption(CString Str);

	//Цвет
	virtual int Color()
	{
		ASSERT(pWnd);
		return 0;
	};
	virtual int Color(int nColor)
	{
		ASSERT(pWnd);
		return Color();
	};
	virtual int Color(int R,int G,int B)
	{
		ASSERT(pWnd);
		return Color();
	};
	virtual int EnableEdit()
	{
		ASSERT(pWnd);
		return 0;
	};
	virtual int EnableEdit(BOOL bStat)
	{
		ASSERT(pWnd);
		int nRes=EnableEdit();
		return nRes;
	};
	virtual CString Mask(CString Str)
	{
		ASSERT(pWnd);
		return "";
	};
	virtual void Close()
	{
		if(pWnd)
			PWINDOW->SendMessage(WM_CLOSE);
	};
	
	#undef	PWINDOW
};


//ПОДДЕРЖКА ПОЛЕЙ РЕДАКТИРОВАНИЯ
#define PWINDOW ((CEdit *)pWnd)
class CValueEdit : public CValueControl
{
DECLARE_DYNCREATE(CValueEdit);
public:
	int EnableEdit()
	{
		UINT n=PWINDOW->GetStyle();
		if(n&ES_READONLY)
			return 1;
		else
			return 0;
	}
	int EnableEdit(BOOL bSet)
	{
		int nRes=EnableEdit();
		PWINDOW->SetReadOnly(1-bSet);
		return nRes;
	};



	CValueEdit()
	{
		nType=TYPE_STRING;
	}

	virtual CString GetString(void)const
	{
		CString csRes;
		PWINDOW->GetWindowText(csRes);
		csRes.Replace("\r\n","\n");
		return csRes;
	};
	virtual void SetString(CString csParam)
	{
		csParam.Replace("\n","\r\n");
		PWINDOW->SetWindowText(csParam);
	};

};
#undef	PWINDOW

class CValueNumber : public CValueEdit
{
DECLARE_DYNCREATE(CValueNumber);
public:
	CValueNumber()
	{
		nType=TYPE_NUMBER;
	};
	NUMBER GetNumber(void)const
	{
		CString Str;
		Str=GetString();
		Str.Replace(",",".");
		return atof(Str);
	}
	CString GetTypeString(void)const
	{
		if(!pWnd)
			return NOT_DEFINED;//SetError("Не определен указатель на объект диалога");
		return "Число";
	}

};
class CValueInt : public CValueControl
{
DECLARE_DYNCREATE(CValueInt);
public:
	CValueInt()
	{
		nType=TYPE_NUMBER;
	};
	NUMBER GetNumber(void)const
	{
		CString Str;
		Str=GetString();
		Str.Replace(",",".");
		return atof(Str);
	}
	virtual void SetNumber(CString Val)
	{
		SetString(Val);
	};
	CString GetTypeString(void)const
	{
		if(!pWnd)
			return NOT_DEFINED;//SetError("Не определен указатель на объект диалога");
		return "Число";
	}


};


//ПОДДЕРЖКА ТЕКСТА
#define PWINDOW ((CXColorStatic *)pWnd)
class CValueStatic : public CValueControl
{
DECLARE_DYNCREATE(CValueStatic);
public:
	//Заголовок
	CString Caption()
	{
		CString csRes;
		PWINDOW->GetWindowText(csRes);
		return csRes;
	};
	void Caption(CString Str)
	{
		CString csRes=Caption();
		CXColorStatic m;
		
		PWINDOW->SetWindowText(Str);
		PWINDOW->SetPlainBorder(TRUE);
		PWINDOW->SetPlainBorder(FALSE);
	};

	//Цвет
	int Color()
	{
		return PWINDOW->GetTextColor();
	};
	int Color(int nColor)
	{
		int nRes=Color();
		PWINDOW->SetTextColor(nColor);
		PWINDOW->SetPlainBorder(TRUE);
		PWINDOW->SetPlainBorder(FALSE);
		return nRes;
	};
	int Color(int R,int G,int B)
	{
		int nRes=Color();
		Color(RGB(R,G,B));
		return nRes;
	};

public:
	CValueStatic()
	{
		nType=TYPE_STRING;
	}

	virtual CString GetString(void)const
	{
		CString csRes;
		PWINDOW->GetWindowText(csRes);
		return csRes;
	};
	virtual void SetString(CString csParam)
	{
		PWINDOW->SetWindowText(csParam);
		PWINDOW->SetPlainBorder(TRUE);
		PWINDOW->SetPlainBorder(FALSE);
	};
#undef PWINDOW
};




//ПОДДЕРЖКА КНОПОК
#define PWINDOW ((CButton *)pWnd)
class CValueButton:public CValueControl
{
DECLARE_DYNCREATE(CValueButton);
public:
	CValueButton()
	{
		nType=TYPE_NUMBER;
	};

	NUMBER GetNumber(void)const
	{
		CString Str;
		Str=GetString();
		Str.Replace(",",".");
		return atof(Str);
	}
	virtual CString GetString(void)const
	{
		CString csRes;
		int nCheck=PWINDOW->GetCheck();
		csRes.Format("%d",nCheck);
		return csRes;
	};
	virtual void SetString(CString csParam)
	{
		int nValue=atoi(csParam);
		if(nValue)
			PWINDOW->SetCheck(true);
		else
			PWINDOW->SetCheck(false);
	};
#undef PWINDOW
};



#define PWINDOW ((CButton *)pWnd)
 
//ПОДДЕРЖКА РАМКИ ДИАЛОГА
class CValueGroup:public CValueButton
{
	DECLARE_DYNCREATE(CValueGroup);
};
//ПОДДЕРЖКА ФЛАЖКОВ
class CValueCheck:public CValueButton
{
	DECLARE_DYNCREATE(CValueCheck);
public:
};

//ПОДДЕРЖКА ПЕРЕКЛЮЧАТЕЛЕЙ
#define MAX_RADIO  100 // максимальное количество элементов у переключателя (RADIO)
class CValueRadio:public CValueControl
{
DECLARE_DYNCREATE(CValueRadio);
public:
	//Доступность
	int Enable()
	{
		return PWINDOW->IsWindowEnabled();
	};
	int Enable(BOOL bStat)
	{
		int nRes=Enable();

		CWnd *hParent=PWINDOW->GetParent();
		if(hParent)
		{
			PWINDOW->EnableWindow(bStat);
			int n1=PWINDOW->GetDlgCtrlID();
		    for(int nID=n1+1;nID<n1+MAX_RADIO;nID++)
			{
				CWnd* pWnd=hParent->GetDlgItem(nID);
				if(!pWnd)
					break;

				CRuntimeClass* prt = pWnd->GetRuntimeClass();
				if(prt->m_lpszClassName!="CRadio")
					break;
				if((1<<17)&pWnd->GetStyle())
					break;
				pWnd->EnableWindow(bStat);
			}
		}
		return nRes;
	};
	
};
#undef PWINDOW




//ДАТА
#define PWINDOW ((CDateTimeCtrl *)pWnd)
class CValueDate : public CValueControl
{
DECLARE_DYNCREATE(CValueDate);
public:
	CValueDate();
	virtual ~CValueDate();
	virtual int GetDate(void)const;
	CString GetString(void)const
	{
		CString csRes;
		PWINDOW->GetWindowText(csRes);
		return csRes;
	}
	void SetString(CString csParam)
	{
		int nYear;
		int nMonth;
		int nDay;
		CValue dParam;
		dParam.SetString(csParam);
		Date(dParam,nYear,nMonth,nDay,true);
		// set with a COleDateTime object
		COleDateTime oleTime(nYear, nMonth, nDay, 0, 0, 0);
		PWINDOW->SetTime(oleTime);
	}
};
#undef PWINDOW

#endif // !defined(AFX_VALUECONTROL_H__C05903D7_64B8_4117_BA65_7C1CF59AD752__INCLUDED_)
