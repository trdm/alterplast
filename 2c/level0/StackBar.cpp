// StackBar.cpp: implementation of the CStackBar class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "StackBar.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


CArray <CString,CString> *GetStack();
CImageList *GetImageList();
BOOL ParseTextLines(CString csText);
void SetCurrentStack(int nNumber);
extern CDebugMessageBar *pDebugMessageBar;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BEGIN_MESSAGE_MAP(CStackBar, CDebugMessageBar)
	//{{AFX_MSG_MAP(CStackBar)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

CStackBar::CStackBar()
{
}

void CStackBar::Init()
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
	//Инициализация
	InsertFColumn(0, "",	200, FL_LEFT);
	InsertFColumn(1, "",	300, FL_LEFT);
	SetImageList(::GetImageList());


	Refresh();
}

CStackBar::~CStackBar()
{

}


void CStackBar::Refresh()
{
	CArray <CString,CString> *pList=GetStack();
	DeleteAllFRows();
	for(int i=0;i<pList->GetSize();i++)
	{
		int nImage=0;
		if(i==0)
			nImage=378;

		CString Str=pList->GetAt(i);
		int nIndex=Str.Find("::");
		CString Str1=Str.Left(nIndex);
		CString Str2=Str.Mid(nIndex+2);
		Str1.TrimLeft();
		Str2.TrimLeft();

		HFROW hNextFRow=InsertFRow(FL_ROOT,FL_LAST,Str2,nImage);
		SetFItemText(hNextFRow,1,Str1);
	}
	pList->RemoveAll();

}
void CStackBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	HFROW hFRow;
	int iFColumn;
	if(HitTestMouseClick(point, hFRow, iFColumn))
	{
		//обновляем картинки
		for(int i=1;i<GetFRowCount();i++)
		{
			HFROW hCurFRow=FindIndex(FL_ROOT,i);
			int nImage=0;
			if(hCurFRow==hFRow)
				nImage=379;
			SetFItemImage(hCurFRow, 0, nImage);
		}
		CString Str=GetFItemText(hFRow,1);
		ParseTextLines(Str);
		SetCurrentStack(GetFRowIndex(hFRow));
		if(pDebugMessageBar)
			pDebugMessageBar->Refresh();
	}
}
void CStackBar::OnDestroy() 
{
	//переопределяем (что бы не было перезаписи файла состояния родителя)
	//и ... ничнего не делаем
}
