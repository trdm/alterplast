// ¿‚ÚÓÒÍËÂ Ô‡‚‡: VTOOLS.RU (info@vtools.ru) 2002,2003„.
// 1CCtrl.cpp: implementation of the C1CCtrl class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "enterprise.h"
#include "1CCtrl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//œŒƒƒ≈–∆ ¿ “≈ —“¿
void SetStaticParams(CXColorStatic &m_StaticText,int hAlign=0,int vAlign=0,CString csFont="MS Sans Serif",bool bBold=0,bool bItalic=0,bool bUnderL=0,int nHeight=15,int nColor=0)
{
	
	if(hAlign==0)
	m_StaticText.SetHorzAlignment(X_ALIGNHORZ_LEFT,TRUE);
	else if(hAlign==1)
	m_StaticText.SetHorzAlignment(X_ALIGNHORZ_CENTER,TRUE);
	else if(hAlign==2)
	m_StaticText.SetHorzAlignment(X_ALIGNHORZ_RIGHT,TRUE);

	if(vAlign==0)
	m_StaticText.SetVertAlignment(X_ALIGNVERT_TOP,TRUE);
	else if(vAlign==1)
	m_StaticText.SetVertAlignment(X_ALIGNVERT_CENTER,TRUE);
	else if(vAlign==2)
	m_StaticText.SetVertAlignment(X_ALIGNVERT_BOTTOM,TRUE);
	

	m_StaticText.SetFont(csFont,nHeight);
	m_StaticText.SetBold(bBold,TRUE);
	m_StaticText.SetItalic(bItalic,TRUE);
	m_StaticText.SetUnderline(bUnderL,TRUE);
	//m_StaticText.SetFontAttr(OX_STRIKED_OUT_FONT, 0);
	//m_StaticText.SetFontHeight(nHeight);
	

	//m_StaticText.Set3Doffset(0);
	//m_StaticText.SetTextColor(RGB(Red, Green, Blue));
	m_StaticText.SetTextColor(nColor);
	

	
	m_StaticText.SetBackgroundColor(::GetSysColor(COLOR_3DFACE));
	m_StaticText.SetPlainBorder(TRUE);
	m_StaticText.SetPlainBorder(FALSE);
}


//œŒƒƒ≈–∆ ¿ œŒÀ≈… –≈ƒ¿ “»–Œ¬¿Õ»ﬂ
IMPLEMENT_DYNCREATE(CDropEdit,SECDropEdit);
BEGIN_MESSAGE_MAP(CDropEdit, SECDropEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()
void CDropEdit::OnClicked()
{
	SetFocus();
	UINT wParam = GetDlgCtrlID();
	
	NMHDR	   hdr;
	hdr.code	   = WM_ONSELECT;
	hdr.hwndFrom = m_hWnd;
	hdr.idFrom   = GetDlgCtrlID();
	if(GetParent())
	{
		int nRet=GetParent()->SendMessage(WM_NOTIFY,  wParam, (LPARAM) &hdr);
		if(nRet==-1)
			return;
	}
}
IMPLEMENT_DYNCREATE(CCalcEdit,CDropEdit);
BEGIN_MESSAGE_MAP(CCalcEdit, CDropEdit)
	ON_WM_CHAR()
END_MESSAGE_MAP()

void CCalcEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	DWORD dwStyle= GetStyle();
	if ((dwStyle & ES_READONLY)== ES_READONLY) 
	{
		return ;
	}

	if(nUseFilter==TYPE_NUMBER)
	{
		CString Str;
		GetWindowText(Str);
		switch(nChar) 
		{
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				CDropEdit::OnChar(nChar,nRepCnt,nFlags);
				GetWindowText(Str);
				if(Str.Find("-")>0)
				{
					Str.Replace("-","");
					Str=CString("-")+Str;
					SetWindowText(Str);
				}
				break;
			
			case '+':
				Str.TrimLeft('-');
				SetWindowText(Str);
				break;

			case '-':
				if(Str.Left(1)=="-")
					Str.TrimLeft('-');
				else
					Str=CString("-")+Str;
				SetWindowText(Str);
				break;

			case '.':
				if(Str.Find(".")==-1)
					CDropEdit::OnChar(nChar,nRepCnt,nFlags);
				break;

			case VK_BACK:
				CDropEdit::OnChar(nChar,nRepCnt,nFlags);
				break;
		}
	}
	else
	if(nUseFilter==TYPE_DATE)
	{
		SetMask("##.##.####");
		CDropEdit::OnChar(nChar,nRepCnt,nFlags);
	}
	else
	if(nUseFilter==TYPE_STRING)
	{
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return ;
	}

}
void CCalcEdit::OnClicked()
{
	SetFocus();
//	if(nUseFilter==1)
	if(GetStyle() & ES_READONLY)
		return;

	UINT wParam = GetDlgCtrlID();
	
	NMHDR	   hdr;
	hdr.code	   = WM_ONSELECT;
	hdr.hwndFrom = m_hWnd;
	hdr.idFrom   = GetDlgCtrlID();
	if(GetParent())
	{
		int nRet=GetParent()->SendMessage(WM_NOTIFY,  wParam, (LPARAM) &hdr);
		if(nRet==-1)
			return;
	}

	if(nUseFilter!=TYPE_NUMBER)
		return;

	// Parse current value
	double val = 0.0;
	CString temp;
	GetWindowText(temp);
	temp.Replace(",",".");
	_stscanf(temp, _T("%lf"), &val);

	SECPopupCalculator* pCalc = new SECPopupCalculator;

	// We want the calculator to self-destruct when it has been dismissed
	pCalc->m_bAutoDelete = TRUE;

	// Set window to receive result as WM_SETTEXT
	pCalc->SetOwner(this);


	CRect r;
	GetWindowRect(&r);

	DWORD dwStyle = WS_VISIBLE|SEC_CS_RAISEDEDGE|SEC_PCS_DESTROY_ON_EQUALS;
	//if(GetParent()->IsDlgButtonChecked(IDC_HASDIVIDER))
	dwStyle |= SEC_CS_DIVIDER;

	VERIFY(pCalc->Create(dwStyle, r, this));
	// Set the initial value
	pCalc->SetValue(val);
}

//œŒƒƒ≈–∆ ¿ «¿ À¿ƒŒ 
void CMyTabCtrl::RemoveAll()
{
	aLayer.RemoveAll();
	CTabCtrl::DeleteAllItems();

}
void CMyTabCtrl::AddValue(CString csName,CString csPresent)
{
	TCITEM tcItem;
	tcItem.mask = TCIF_TEXT;
	tcItem.pszText = csPresent.GetBuffer(0);
	CTabCtrl::InsertItem(aLayer.GetSize(), &tcItem);
	csPresent.ReleaseBuffer();

	CLayerTabCtrl data;
	data.Name=csName;
	data.Present=csPresent;
	aLayer.Add(data);
}

void CMyTabCtrl::Add(CString csLayer,UINT nID)
{
	CElementTabCtrl data;
	csLayer.Replace(" ","");
	if(csLayer.IsEmpty())
		csLayer=DEFAULT_LAYER_NAME;
	data.Name=mUpper(csLayer);
	data.ID=nID;
	aList.Add(data);
}

void CMyTabCtrl::UseLayer(CString csName,int nMode)
{
	int nCmdShow=SW_NORMAL;//SW_SHOW;
	if(nMode==0)
		nCmdShow=SW_HIDE;
	else
	if(nMode==2)
		UseLayer("",0);

	csName=csName+",";
	csName.MakeUpper();
	csName.Replace(" ","");

	ASSERT(GetParent());
	for(int i=0;i<aList.GetSize();i++)
	{
		if(csName==",")
			GetParent()->GetDlgItem(aList[i].ID)->ShowWindow(nCmdShow);
		else
		{
			CString csFind=aList[i].Name;
			csFind+=",";
			if(csName.Find(csFind)>=0)
			{
				GetParent()->GetDlgItem(aList[i].ID)->ShowWindow(nCmdShow);
			}
		}
	}
}
void CMyTabCtrl::UseLayer(int nIndex)
{
	ASSERT(nIndex<aLayer.GetSize());
	ASSERT(nIndex>=0);
	UseLayer(aLayer[nIndex].Name);

}


IMPLEMENT_DYNCREATE(CRadio,CButton);
IMPLEMENT_DYNCREATE(CGroup,CButton);
