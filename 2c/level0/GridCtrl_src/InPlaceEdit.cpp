// InPlaceEdit.cpp : implementation file
//
// Adapted by Chris Maunder <cmaunder@mail.com>
// Copyright (c) 1998-2000. All Rights Reserved.


#include "stdafx.h"
#include "TCHAR.h"
#include "InPlaceEdit.h"

#include "GridCtrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


extern CRect	afxCurrentRect;//текущий координаты диалогового окна, который инициирует выборку из списка значений (CButton,CEdit)
extern CWnd	*afxCurrentWnd;
CString FuncFormat(CValue &v,CString &fmt);

BOOL IsPressed(int nKey)
{
	return ((GetKeyState(nKey) & (1 << (sizeof(SHORT)*8-1))) != 0 );
}


/////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit

CInPlaceEdit::CInPlaceEdit(CWnd* pParent, CRect& rect, DWORD dwStyle, UINT nID,
                           int nRow, int nColumn, CString sInitText, 
                           UINT nFirstChar,int nType,int nSetFormat):
						   Behavior(this)
{
	m_bLockClose=0;
	if(IsPressed(VK_F2)||IsPressed(VK_RETURN))
		nFirstChar=VK_LBUTTON;

    m_bAlreadyEnding = FALSE;
	bWasClicked=0;
    m_sInitText     = sInitText;
    m_nRow          = nRow;
    m_nColumn       = nColumn;
    m_nLastChar     = 0; 
	m_nFormat		= nSetFormat;
    m_bExitOnArrows = (nFirstChar != VK_LBUTTON);    // If mouse click brought us here,
                                                     // then no exit on arrows

    m_Rect = rect;  // For bizarre CE bug.
	m_pGrid=0;
    
    DWORD dwEditStyle = WS_BORDER|WS_CHILD|WS_VISIBLE| ES_AUTOHSCROLL |ES_MULTILINE
        | dwStyle;
    if (!Create(dwEditStyle, rect, pParent, nID)) return;
    
    SetFont(pParent->GetFont());
	m_pGrid = (CGridCtrl*)pParent;
    
	nUseFilter=nType;
	if(nType==TYPE_NUMBER)
	{
		SetBitmap(IDB_CALCBTN);
		SetRightAlign(1);

		ASSERT(m_pGrid);
		ASSERT(nColumn<m_pGrid->aColumns.GetSize());
		int nLength=m_pGrid->aColumns[nColumn].nLength;
		int nPrec=m_pGrid->aColumns[nColumn].nPrec;
		if(nLength || nPrec)
		{
			if(nPrec>10)
				nPrec=10;
			if(nLength<=0)
				nLength=20;


			CString csMask;
			for(int i=0;i<nLength;i++)
				csMask+="9";
			csMask+=".";
			for(int j=0;j<nPrec;j++)
				csMask+="9";
			SetMask(csMask);
			
			CString csFrm;
			csFrm.Format("„%d.%d",nLength,nPrec);
			m_sInitText=FuncFormat(CValue(m_sInitText),csFrm);
			m_sInitText.TrimLeft();

		}
	}
	else
	if(nType==TYPE_DATE)
	{
		SetMask(_T("##/##/####"));
	}
	else
	if(nType>TYPE_DATE)
	{
		SetBitmap(IDB_CHOSECBTN);
		::SetWindowLong(m_hWnd, GWL_STYLE, GetStyle() ^ ES_READONLY);
	}
    SetWindowText(m_sInitText);
    SetFocus();

    switch (nFirstChar){
        case VK_LBUTTON: 
        case VK_RETURN:   SetSel(0, -1); ResizeControl();return;
        case VK_BACK:     SetSel((int)_tcslen(m_sInitText), -1); break;
        case VK_TAB:
        case VK_DOWN: 
        case VK_UP:   
        case VK_RIGHT:
        case VK_LEFT:  
        case VK_NEXT:  
        case VK_PRIOR: 
        case VK_HOME:
        case VK_SPACE:
        case VK_END:      SetSel(0,-1); ResizeControl();return;
        default:          
//		    SetWindowText(CString(nFirstChar));
//			return;
			SetSel(0,-1);
    }
    
    SendMessage(WM_CHAR, nFirstChar);
}

CInPlaceEdit::~CInPlaceEdit()
{
	m_vEdit.Detach();
	if(m_Value.FindMethod("ƒе»нициализаци€ќбъекта")>=0)
		m_Value.CallFunction("ƒе»нициализаци€ќбъекта");
}

BEGIN_MESSAGE_MAP(CInPlaceEdit, PARENTEDIT)
    //{{AFX_MSG_MAP(CInPlaceEdit)
    ON_WM_KILLFOCUS()
    ON_WM_CHAR()
    ON_WM_KEYDOWN()
    ON_WM_GETDLGCODE()
    ON_WM_CREATE()
	ON_WM_SYSKEYDOWN()
//	ON_MESSAGE(WM_SETTEXT, OnSetText)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit message handlers

// If an arrow key (or associated) is pressed, then exit if
//  a) The Ctrl key was down, or
//  b) m_bExitOnArrows == TRUE
void CInPlaceEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
    if ((nChar == VK_PRIOR || nChar == VK_NEXT ||
        nChar == VK_DOWN  || nChar == VK_UP   ||
        nChar == VK_RIGHT || nChar == VK_LEFT) &&
        (m_bExitOnArrows))// || GetKeyState(VK_CONTROL) < 0))
    {
		bWasClicked=0;
        m_nLastChar = nChar;
		//GetParent()->PostMessage(WM_SETFOCUS);
		//EndEdit();
        GetParent()->SetFocus();
        return;
    }
	else
    if (nChar == VK_TAB || nChar == VK_RETURN)
    {
		bWasClicked=0;
        m_nLastChar = nChar;
		//GetParent()->PostMessage(WM_SETFOCUS);
		//EndEdit();
        GetParent()->SetFocus();    // This will destroy this window
        return;
    }
	else
    if (nChar == VK_ESCAPE) 
    {
		bWasClicked=0;
        SetWindowText(m_sInitText);    // restore previous text
        m_nLastChar = nChar;
		//GetParent()->PostMessage(WM_SETFOCUS);
		//EndEdit();
        GetParent()->SetFocus();
        return;
    }
    if (nChar == VK_DELETE)
	if(m_Value.nType>TYPE_DATE)
	{
		OnClicked();
		return;
	}
    
    PARENTEDIT::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CInPlaceEdit::SetValue(CValue Val)
{
	m_Value=Val;
	bWasClicked=0;
/*
	if(m_Value.FindMethod("”становитьƒиалоговыйЁлемент")>=0)
	{
		CValueControl *pVal=new CValueControl();
		pVal->Attach(this);
		m_vEdit=ValueByRef(pVal);

		m_Value.CallFunction("”становитьƒиалоговыйЁлемент",&m_vEdit);
	}
*/
	
}
CValue CInPlaceEdit::GetValue()
{
	return m_Value;
}

void CInPlaceEdit::OnClicked()
{
	ASSERT(IsWindow(m_hWnd));
	if( !IsWindow(m_hWnd) )
		return;

	if(!m_pGrid)
		return;

	bWasClicked=1;
	if(m_Value.nType>TYPE_DATE)
	{
		//try
		{
			afxCurrentWnd=this;

			CValue vColName;

			

			//ѕри начале выбора значени€
			if(0)
			{
				CValue vColName=String(m_pGrid->aColumns[m_nColumn].csName);

				CValue vFlag=1;
				CallFunction("ѕриЌачале¬ыбора«начени€",vColName,vFlag,CValue(0));
				CallFunction("OnStartValueChoice",vColName,vFlag,CValue(0));
				if(!vFlag.GetNumber())
					return;
			}


			CString csPrevVal;
			m_Value.SaveToString(csPrevVal);

			bWasClicked=2;
			int nRes=0;
			m_bLockClose=1;//запрещаем закрытие окна редактировани€
			if(m_Value.FindMethod("ќбработка—обыти€")>=0)
				nRes=m_Value.CallFunction("ќбработка—обыти€",&String("¬ыбор")).GetNumber();
			if(m_bLockClose==2)
			{
				SendMessage(WM_CLOSE, 0, 0);
				return;
			}
			m_bLockClose=0;

			if(nRes)
			{
				bWasClicked=0;
				return;
			}

			//ѕри окончании выбора значени€
			if(0)
			{
				CValue vFlag=1;
				CallFunction("ќбработка¬ыбора«начени€",m_Value,vColName,vFlag);
				CallFunction("ProcessValueChoice",m_Value,vColName,vFlag);
				if(!vFlag.GetNumber())
				{
					m_Value.LoadFromString(csPrevVal);
					bWasClicked=0;
					return;
				}
			}

			SetWindowText(m_Value.GetString());
			bWasClicked=0;
		}
/*		catch(CTranslateError *)
		{
				DoError();
		}*/
		return;
	}
	CCalcEdit::OnClicked();
}

// As soon as this edit loses focus, kill it.
void CInPlaceEdit::OnKillFocus(CWnd* pNewWnd)
{
    PARENTEDIT::OnKillFocus(pNewWnd);
	if(bWasClicked==2)
		return;
	if(bWasClicked==1)
	{
		bWasClicked=0;
		return;
	}
	//Message("OnKillFocus");
	EndEdit();
}

// Resize edit control if needed
void CInPlaceEdit::ResizeControl()
{
    // Get text extent
    CString str;
    GetWindowText( str );

    // add some extra buffer
    str += _T("  ");
    
    CWindowDC dc(this);
    CFont *pFontDC = dc.SelectObject(GetFont());
    CSize size = dc.GetTextExtent( str );
    dc.SelectObject( pFontDC );
       
    // Get client rect
    CRect ParentRect;
    GetParent()->GetClientRect( &ParentRect );
    
    // Check whether control needs to be resized
    // and whether there is space to grow
    if (size.cx > m_Rect.Width())
    {
        if( size.cx + m_Rect.left < ParentRect.right )
		{
			if(1==m_nFormat)//авторасширение влево
	            m_Rect.left-= size.cx - m_Rect.Width();
			else
	            m_Rect.right = m_Rect.left + size.cx;
		}
        else
            m_Rect.right = ParentRect.right;
        MoveWindow( &m_Rect );
    }
}
void CInPlaceEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
   
    PARENTEDIT::OnChar(nChar, nRepCnt, nFlags);
    
    // Resize edit control if needed
	ResizeControl();
}

UINT CInPlaceEdit::OnGetDlgCode() 
{
    return DLGC_WANTALLKEYS;
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit overrides

// Stoopid win95 accelerator key problem workaround - Matt Weagle.
BOOL CInPlaceEdit::PreTranslateMessage(MSG* pMsg) 
{
    // Catch the Alt key so we don't choke if focus is going to an owner drawn button
    if (pMsg->message == WM_SYSCHAR)
        return TRUE;
 
    return CWnd::PreTranslateMessage(pMsg);
}

// Auto delete
void CInPlaceEdit::PostNcDestroy() 
{
    PARENTEDIT::PostNcDestroy();
    
    delete this;	
}

////////////////////////////////////////////////////////////////////////////
// CInPlaceEdit implementation

void CInPlaceEdit::EndEdit()
{
    CString str;

    // EFW - BUG FIX - Clicking on a grid scroll bar in a derived class
    // that validates input can cause this to get called multiple times
    // causing assertions because the edit control goes away the first time.
    if(m_bAlreadyEnding)
        return;

	//SetWindowText(m_Value.GetString());

    m_bAlreadyEnding = TRUE;
    GetWindowText(str);
	if(m_Value.GetType()<=TYPE_DATE)
		m_Value.SetData(String(str));

    // Send Notification to parent
    GV_DISPINFO dispinfo;

    dispinfo.hdr.hwndFrom = GetSafeHwnd();
    dispinfo.hdr.idFrom   = GetDlgCtrlID();
    dispinfo.hdr.code     = GVN_ENDLABELEDIT;

    dispinfo.item.mask    = LVIF_TEXT|LVIF_PARAM;
    dispinfo.item.row     = m_nRow;
    dispinfo.item.col     = m_nColumn;
    dispinfo.item.strText  = str;
    dispinfo.item.lParam  = (LPARAM) m_nLastChar;
	dispinfo.Val=m_Value;

	//Message("EndEdit");

    // Close this window (PostNcDestroy will delete this)
    if (IsWindow(GetSafeHwnd()))
	{
		CWnd* pOwner = GetOwner();
		if (pOwner)
			pOwner->SendMessage(WM_NOTIFY, GetDlgCtrlID(), (LPARAM)&dispinfo );

		if(m_bLockClose)
			m_bLockClose=2;//PostMessage(WM_CLOSE, 0, 0);
		else
			SendMessage(WM_CLOSE, 0, 0);
	}
    m_bAlreadyEnding = FALSE;
}

void CInPlaceEdit::OnSysKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	// TODO: Add your message handler code here and/or call default
	
	PARENTEDIT::OnSysKeyDown(nChar, nRepCnt, nFlags);
}



void CInPlaceEdit::CallFunction(CString csFunction,CValue &vParam1,CValue &vParam2,CValue &vParam3)
{
	if(m_pGrid)
	if(m_pGrid->pRunContext)
	if(m_pGrid->pRunContext->pProcUnit)
	{
		try
		{
			m_pGrid->pRunContext->pProcUnit->CallFunction(csFunction,vParam1,vParam2,vParam3);
		}
		catch(CTranslateError *)
		{
			DoError();
			return;
		};
	}
}
