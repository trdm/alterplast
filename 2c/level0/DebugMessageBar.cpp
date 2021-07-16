//Авторские права - VTOOLS.RU (info@vtools.ru)
// DebugMessageBar.cpp: implementation of the CDebugMessageBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "DebugMessageBar.h"
#include "MyDataExchange.h"
#include "Resource.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



CValue ValueFromFile(CString csFileName);
void ValueToFile(CValue *Val,CString csFileName);
CImageList *GetImageList();
extern CString csIBDir;
extern CString csUserPath;//каталог пользователя для записи настроек
extern CArray <void*,void*> aPList;
int SendStringArray(HWND hwnd,CString csName,CArray <CString,CString> &aList,int nMode,int bPost);
void ClearPList();
void LoadStringArray(CArray <CString,CString> *pList);

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
#define FILE_LIST_NAME csUserPath+"\\debug_expr.lst"
CDebugMessageBar::CDebugMessageBar()
{

}

CDebugMessageBar::~CDebugMessageBar()
{
}

BEGIN_MESSAGE_MAP(CDebugMessageBar, CExtWFF<CFListCtrl>)
	//{{AFX_MSG_MAP(CDebugMessageBar)
	ON_WM_DESTROY()
	ON_MESSAGE(WM_SETTEXT, OnSetText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


void CDebugMessageBar::Init()
{
	DWORD dwExStyle=GetExtendedStyle();
	dwExStyle|=LVS_EX_GRIDLINES;//LVS_EX_FULLROWSELECT 
	SetExtendedStyle(dwExStyle);

	//SetFStyle(FL_LINEAR_TREE, FL_LINEAR_TREE);
	SetReadOnly(0);

	//устанавливаем шрифт
	static CFont Font;
	VERIFY(Font.CreateFont(
	   14,                        // nHeight
	   0,                         // nWidth
	   0,                         // nEscapement
	   0,                         // nOrientation
	   FW_NORMAL,                 // nWeight
	   FALSE,                     // bItalic
	   FALSE,                     // bUnderline
	   0,                         // cStrikeOut
	   DEFAULT_CHARSET,					  // nCharSet
	   OUT_DEFAULT_PRECIS,        // nOutPrecision
	   CLIP_DEFAULT_PRECIS,       // nClipPrecision
	   DEFAULT_QUALITY,           // nQuality
	   DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
	   _T("MS Sans Serif")));                 // lpszFacename
	SetFont(&Font);


	int nCol1=0;
	int nCol2=0;
	int nCol3=0;
	csIBDir.TrimRight('\\');

	CValue Val=ValueFromFile(FILE_LIST_NAME);
	if(Val.GetTypeString()=="Структура")
	{
		nCol1=Val.GetAt("nCol1").GetNumber();
		nCol2=Val.GetAt("nCol2").GetNumber();
		nCol3=Val.GetAt("nCol3").GetNumber();
	}
	if(nCol1<100)
		nCol1=200;
	if(nCol2<50)
		nCol2=80;
	if(nCol3<100)
		nCol3=300;

	//Инициализация
	InsertFColumn(0, "Выражение",	nCol1, FL_LEFT);
	InsertFColumn(1, "Тип",			nCol2, FL_LEFT);
	InsertFColumn(2, "Значение",	nCol3, FL_LEFT);
	SetTree(0);	
	SetReadOnly(FALSE);


	//SetImageList(::GetImageList());
	//добавляем профили контролов редактирования
	iControl=AddControl(FL_EDIT);
	HFROW hFRow;

	for(int i=1;i<=Val.GetSizeArray();i++)
	{
		hFRow=InsertFRow(FL_ROOT, FL_LAST, Val.GetAt(i).GetString(), 0);
		SetFItemControl(hFRow, 0, iControl);
		SetFItemControl(hFRow, 1, iControl);
		SetFItemControl(hFRow, 2, iControl);
	}

	hFRow=InsertFRow(FL_ROOT, FL_LAST, "", 0);
	SetFItemControl(hFRow, 0, iControl);
	SetFItemControl(hFRow, 1, iControl);
	SetFItemControl(hFRow, 2, iControl);

#ifndef TEST_SPEED
#ifndef _DEBUG
    m_droptarget.Register ( this );
#endif
#endif
}

CString GetVariable(CString csVariable);
//списки атрибутов объекта
extern CArray <CString,CString> aAttributeName;
extern CArray <CString,CString> aAttributeType;
extern CArray <CString,CString> aAttributeValue;
HWND Get2C();


CString CDebugMessageBar::GetPath(HFROW hFRow)
{
	HFROW hParent=GetNextFRow(hFRow,FL_PARENT);
	if(((UINT)hParent)>50)
	{
		CString csVariable;
		if(GetFItemText(hParent,1)=="Массив"||GetFItemText(hParent,1)=="Структура")
			csVariable.Format("%s%s",GetPath(hParent),GetFItemText(hFRow,0));
		else
			csVariable.Format("%s.%s",GetPath(hParent),GetFItemText(hFRow,0));
		return csVariable;
	}
	return GetFItemText(hFRow,0);
}
void CDebugMessageBar::OnEvent(FL_NOTIFY &notify, HFROW hFRow, int iFColumn, int code)
{
	if(code==FLNM_COLLAPSE)
	{
		BOOL bClose=IsCollapsed(hFRow);
		HWND hwnd=Get2C();
		if(hwnd)
		if(bClose==0)
		{
			//обход всех подчиненных элементов
			CArray <CString,CString> aList;
			HFROW hNextChild, hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
			for(;hChild!=NULL; hChild=hNextChild)
			{
				hNextChild=GetNextFRow(hChild, FL_NEXT);
				aList.Add(GetPath(hChild));
			}		

			SendStringArray(hwnd,"-",aList,GET_VAR_STRING_ARRAY,0);

			int i=0;
			hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
			for(;hChild!=NULL; hChild=hNextChild)
			{
				if(i>=aPList.GetSize())
					return;
				hNextChild=GetNextFRow(hChild, FL_NEXT);

				CArray <CString,CString> *pList=(CArray <CString,CString>*)aPList[i];
				LoadStringArray(pList);
				SetItem(hChild);

				i++;
				if(i>=aPList.GetSize())
					break;
			}		
			ClearPList();


			Invalidate();
		}
	}
	else
	if(code==FLNM_EDIT_END&&iFColumn==0)
	{
		HFROW hParent=GetNextFRow(hFRow,FL_PARENT);
		if(((UINT)hParent)>50)
		{
			return;
		}

		Collapse(hFRow,1);
		SetItem(hFRow,notify.strText);

		if(GetFRowIndex(hFRow)+1>=GetFRowCount())
		{
			hFRow=InsertFRow(FL_ROOT, FL_LAST, "", 0);
			SetFItemControl(hFRow, 0, iControl);
			SetFItemControl(hFRow, 1, iControl);
			SetFItemControl(hFRow, 2, iControl);
		}
	}
}

void CDebugMessageBar::SetItem(HFROW hFRow)
{
	//удаляем все подчиненные элементы
	//*
	HFROW hNextChild, hChild=GetNextFRow(hFRow, FL_FIRST_CHILD);
	for(;hChild!=NULL; hChild=hNextChild){				
		hNextChild=GetNextFRow(hChild, FL_NEXT);
		DeleteFRow(hChild);
	}		
	//*/
	//здесь возможна утечка памяти!!! (но так не тормозит!)
	//GetFRow(hFRow)->m_lstChildFRows.RemoveAll();//зато глючит :)
	

	if(aAttributeName.GetSize()==0)
	{
		SetFItemText(hFRow,1,"");
		SetFItemText(hFRow,2,"");
		return;
	}

	CString csType=aAttributeType[0];
	CString csResult=aAttributeValue[0];

	if(GetFItemText(hFRow,0)=="<...>")
	{
		csType="";
		csResult="";
	}

	SetFItemText(hFRow,1,csType);
	SetFItemText(hFRow,2,csResult);

	for(int i=1;i<min(aAttributeName.GetSize(),MAX_LIST);i++)
	{
		HFROW hNextFRow=InsertFRow(hFRow, FL_LAST, aAttributeName[i], 0, iControl, 1);

		SetFItemText(hNextFRow,1,aAttributeType[i]);
		SetFItemControl(hNextFRow, 1, iControl);

		SetFItemText(hNextFRow,2,aAttributeValue[i]);
		SetFItemControl(hNextFRow, 2, iControl);
	}
	if(MAX_LIST<aAttributeName.GetSize())
	{
		HFROW hNextFRow=InsertFRow(hFRow, FL_LAST, "<...>", 0, -1, 1);
	}


	aAttributeName.RemoveAll();
	aAttributeType.RemoveAll();
	aAttributeValue.RemoveAll();
}
void CDebugMessageBar::SetItem(HFROW hFRow,CString csVariable)
{
	BOOL bClose=IsCollapsed(hFRow);
	if(!bClose)
		Collapse(hFRow,1);

	csVariable.TrimRight();
	if(csVariable.IsEmpty())
	{
		aAttributeName.RemoveAll();
		aAttributeType.RemoveAll();
		aAttributeValue.RemoveAll();

		SetItem(hFRow);
		return;
	}
	GetVariable(csVariable);
	SetItem(hFRow);

	if(!bClose)
		Collapse(hFRow,0);

	Invalidate();
}

void CDebugMessageBar::Refresh()
{
	for(int i=0;i<GetFRowCount()-1;i++)
	{
		HFROW hFRow=FindIndex(FL_ROOT,i);

		CString csVariable=GetFItemText(hFRow,0);
		SetItem(hFRow,csVariable);
	}
}

void CDebugMessageBar::OnDestroy() 
{
	csIBDir.TrimRight('\\');
	CValue Val;
	Val.CreateObject("Структура");
	int n=0;
	for(int i=0;i<GetFRowCount()-1;i++)
	{
		HFROW hFRow=FindIndex(FL_ROOT,i);

		CString csVariable=GetFItemText(hFRow,0);
		csVariable.TrimRight();
		if(!csVariable.IsEmpty())
		{
			n++;
			Val.SetAt(n,String(csVariable));
		}
	}

	Val.SetAt("nCol1",GetFColumnWidth(0));
	Val.SetAt("nCol2",GetFColumnWidth(1));
	Val.SetAt("nCol3",GetFColumnWidth(2));

	ValueToFile(&Val,FILE_LIST_NAME );
}



int CDebugMessageBar::OnSetText(WPARAM wParam, LPARAM lParam)
{
	LPCTSTR lpszString=LPCTSTR(lParam);

	HFROW hPrev=FL_LAST;
	if(GetFRowCount()==1)
		hPrev=FL_FIRST;
	if(GetFRowCount()>1)
		hPrev=FindIndex(FL_ROOT,GetFRowCount()-2);

	HFROW hFRow=InsertFRow(FL_ROOT, hPrev, lpszString, 0);
	SetFItemControl(hFRow, 0, iControl);
	SetFItemControl(hFRow, 1, iControl);
	SetFItemControl(hFRow, 2, iControl);
	SetItem(hFRow,lpszString);
	return 1;
}
