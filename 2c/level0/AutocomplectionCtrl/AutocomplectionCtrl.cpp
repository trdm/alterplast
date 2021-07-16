// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// AutocomplectionCtrl.cpp : implementation file
//

#include "stdafx.h"
#include "../resource.h"
#include "AutocomplectionCtrl.h"
#include "../SyntaxColor/editcmd.h"
#include "../ModuleView.h"
#include "../Compiler/TranslateModule.h"
#include "../ModuleDoc.h"
#include "../EditForm/FormEditor.h"
#include "../Compiler/Definition.h"
#include "../MetadataTree.h"

CImageList *GetImageList();

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


/////////////////////////////////////////////////////////////////////////////
// CAutocomplectionCtrl

BOOL isalpha2(unsigned int c)
{
	if(c<'A'&&(!(c>='0'&&c<='9')))
		return FALSE;
	return TRUE;
}


int compare2(const char* s1, const char* s2)
{
	CString Str1=s1;
	CString Str2=s2;
	Str1.MakeLower();
	Str2.MakeLower();
	return Str1.Compare(Str2);
}
int comparen2(const char*s1, const char*s2, size_t n)
{
	CString Str1;
	char *p1=Str1.GetBuffer(n);
	memcpy(p1,s1,n);

	CString Str2;
	char *p2=Str2.GetBuffer(n);
	memcpy(p2,s2,n);

	Str1.MakeLower();
	Str2.MakeLower();
	return Str1.Compare(Str2);
}

CAutocomplectionCtrl::CAutocomplectionCtrl()
{
	bUseCashContextData=0;

	m_autocompleted=FALSE;
	
	/* preferences */
	m_enabled = TRUE;
	m_showlistbox = TRUE;
	m_learn = FALSE;
	m_trapenter = TRUE;
	m_learnondblclick = FALSE;
	m_listallwords = FALSE;

	m_myauto = TRUE;
	m_fast_mode=0;
	
	/* compare function*/
	compare = compare2;
	comparen = comparen2;

	//CaseSensitive(0);

	LoadSysKeyword();

	m_listbox.SetImageList(GetImageList());
}

CAutocomplectionCtrl::~CAutocomplectionCtrl()
{
    if (::IsWindow(m_TitleTip.GetSafeHwnd())) 
        m_TitleTip.DestroyWindow();
}


BEGIN_MESSAGE_MAP(CAutocomplectionCtrl, CPARENTWND)
	//{{AFX_MSG_MAP(CAutocomplectionCtrl)
	ON_WM_CHAR()
	ON_LBN_SELCHANGE(IDC_LIST, OnListBoxChanged)
	ON_LBN_DBLCLK(IDC_LIST, OnListBoxDblClick)
	ON_WM_KEYDOWN()
	ON_WM_LBUTTONDOWN()
	ON_WM_RBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_KILLFOCUS()
	ON_COMMAND(ID_EDIT_CHAR_RIGHT, OnCharRight)
	ON_COMMAND(ID_EDIT_CHAR_LEFT, OnCharLeft)
	ON_COMMAND(ID_EDIT_LINE_UP, OnCharUp)
	ON_COMMAND(ID_EDIT_LINE_DOWN, OnCharDown)

	ON_COMMAND(ID_EDIT_PAGE_UP, OnPageUp)
	ON_COMMAND(ID_EDIT_PAGE_DOWN, OnPageDown)
 	ON_COMMAND(ID_EDIT_HOME, OnHome)
 	ON_COMMAND(ID_EDIT_TEXT_BEGIN, OnHome2)
	
	ON_COMMAND(ID_EDIT_LINE_END, OnLineEnd)
	ON_COMMAND(ID_EDIT_TEXT_END, OnTextEnd)
	ON_COMMAND(ID_EDIT_TAB, OnEditTab)

	ON_COMMAND(ID_EDIT_DELETE, OnEditDelete)
	ON_COMMAND(ID_EDIT_DELETE_BACK, OnEditDeleteBack)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
 
/////////////////////////////////////////////////////////////////////////////

extern struct aKeyWordsDef aKeyWords[];
extern CArray <CString,CString> aSysFunction;
extern CArray <CString,CString> aGlVariables;

void CAutocomplectionCtrl::LoadSysKeyword()
{
	ResetDictionary();
	
	for(int i=0;i<LastKeyWord;i++)
	{
		CString csRus=aKeyWords[i].Rus;
		CString csEng=aKeyWords[i].Eng;
		AddKeyword(csRus);
		aImages[csRus]=(void*)355;
		//AddKeyword(csEng);
		//aImages[csEng]=(void*)355;
	}

	for(	i=0;i<aSysFunction.GetSize();i++)
	{
		AddKeyword(aSysFunction[i]);
		aImages[aSysFunction[i]]=(void*)353;
	}

	
	AddKeyword("Контекст");
	aImages["Контекст"]=(void*)354;

	AddKeyword("БазовыйОбъект");
	aImages["БазовыйОбъект"]=(void*)354;

	for(	i=0;i<aGlVariables.GetSize();i++)
	{
		AddKeyword(aGlVariables[i]);
		aImages[aGlVariables[i]]=(void*)354;
	}
	
}


CString CAutocomplectionCtrl::complete(const CString& str, const CString& actual, CStringArray &matching)
{
	int tot=m_keywords.GetSize();
	CString ret;
	int foundindex=-1;
	int comp;
	BOOL found=FALSE;
	int strln = str.GetLength();
	
	int i;
	
	/* find first matching */
	
	int bottom=0, top=tot, middle;
	
	while(1)
	{
		if (bottom >= top)
		{
			i= -1;
			break;
		}
		
		middle = top + bottom;
		middle >>= 1;
		
		comp = comparen(m_keywords[middle],str, strln);
		
		if (!comp)
		{
			while (middle>=0 && !comparen(m_keywords[middle],str, strln))
				middle--;
			
			i = middle + 1;
			break;
		};
		
		if (comp < 0)
			bottom=middle+1;
		else
			top=middle;
	}
	
	/* look for next items matching */
	if (i!=-1)
	{
		for (; i<tot; i++)
		{
			if (comparen(m_keywords[i], str, strln)==0)
			{
				matching.Add(m_keywords[i]);
				
				if (!found)
				{
					ret=m_keywords[i].Mid(strln);
					comp=compare(ret,actual);
					if (comp>=0)//Y.N.I.
						found = TRUE;
					
					if (comp<0 && foundindex==-1)
						foundindex=i;
				}
			}
		}
	}
	
	if (found)
		return ret;
	
	if (foundindex!=-1)
		return m_keywords[foundindex].Mid(str.GetLength());
	
	if (matching.GetSize()>1)
		return matching[0];
	
	return "";
}

inline void CAutocomplectionCtrl::RemoveListbox()
{
	//bNotUseUndo=0;

	if (m_listbox.m_hWnd)
	{
		m_listbox.DestroyWindow();
		if(m_pTextBuffer)
		if(m_pTextBuffer->m_bUndoGroup)
			m_pTextBuffer->FlushUndoGroup(this);
	}
	m_listbox.m_hWnd=0;
	
	m_autocompleted = FALSE;


}

void CAutocomplectionCtrl::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(m_pTextBuffer)
	if(m_pTextBuffer->GetReadOnly())
		return;

	if (nChar == '(')
	{
		SetTimer(LINE_SHOW_TIMER_ID,10,0);
	}

	if (!m_enabled)
	{
		CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
		return;
	}

	if (nChar == ' ' && GetKeyState(VK_CONTROL)& 0xE000)
	{
		//смотрим - может слева есть точка
		CPoint point=GetCursorPos();
		CString csWord=GetCurWord(point,1,0);
		csWord.TrimRight();
		if(csWord.GetLength())
		if(csWord.Right(1)==".")
		{
			//OutToDebug("csWord=%s\n",csWord);
			point=GetCursorPos();
			point.x--;
			LoadIntelliList(point,1,1);
			Autocomplete();
			return;
		}


		m_fast_mode=0;
		LoadSysKeyword();
		LoadContextData(1);
		Autocomplete();
		return;
	}
	else if (nChar == VK_RETURN && m_trapenter && m_listbox.m_hWnd)
	{
		CHARRANGE curSel;
		GetSel(curSel);

		CString text;
		m_listbox.GetText(m_listbox.GetCurSel(),text);
		SetSel(curSel.cpMax-text.GetLength(), curSel.cpMax);
		//bNotUseUndo=0;
		ReplaceSelection(text);

		SetSel(curSel.cpMax,curSel.cpMax);
		RemoveListbox();
		
	    m_TitleTip.Hide();  // hide any titletips

		m_fast_mode=0;
		return;
	}
	else if (m_autocompleted)
	{

		CHARRANGE curSel;
		GetSel(curSel);
		
		if (nChar == VK_BACK)
		{
			CString cw;
			GetCurWord(cw);
			m_autocompleted=FALSE;
			if(!cw.IsEmpty())
			{
				SetSel(curSel.cpMin-1,curSel.cpMax);
				CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
				Autocomplete();
			}
			else
			{
				CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
				RemoveListbox();
			}

		}
		else if (!isalpha2(nChar))
		{
			if(m_fast_mode)//&&nChar==' ')
			{
				RemoveListbox();
				ReplaceSel("");
				CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
				m_autocompleted=0;
				return;
			}


			SetSel(curSel.cpMax,curSel.cpMax);

			m_autocompleted=FALSE;
			
			RemoveListbox();
			
			CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
		}
		else
		{
			m_autocompleted=FALSE;
			CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
			
			Autocomplete();
		}
	}
	else
	{
		if (nChar == ' ' && m_learn)
		{
			CString cw;
			GetCurWord(cw);

			/* Let's trim non alpha characters */
			int start=0, end =cw.GetLength()-1;
			while (start<end && !isalpha2(cw[start]))
				start++;

			while (start<end && !isalpha2(cw[end]))
				end--;

			if (start!=end)
			{
				cw=cw.Mid(start,end-start+1);

				AddKeyword(cw);
			}
		}
		else
		if (m_myauto&&m_autocompleted==0)
		{
			if(nCursorColor==COLORINDEX_NORMALTEXT||nCursorColor==COLORINDEX_KEYWORD)
			{
				if(isalpha2(nChar))
				{
					CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
					IntelliComplete();
					return;
				}
			}
		}

		if (nChar=='.'&&m_autocompleted==0)//список методов агрегатных объектов
		{
			try
			{
			if(IntelliPoint(nChar, nRepCnt, nFlags))
				return;
			}
			catch(...)
			{
				OutToDebug("Error CAutocomplectionCtrl\n");
				return;
			}
		}

		m_autocompleted=FALSE;
		CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
	}
}

void CAutocomplectionCtrl::OnMouseMove(UINT nFlags, CPoint point) 
{
	CPARENTWND::OnMouseMove(nFlags, point);
}

//________________________________________________________________________________
//выводим описание ключевого слова, функции или метода
void CAutocomplectionCtrl::ShowDescription()
{
	if(m_listbox.m_hWnd)
	{
		int nSel=m_listbox.GetCurSel();
		if(nSel>=0)
		{
			CRect ItemRect,Rect;
			m_listbox.GetItemRect(nSel,&ItemRect);
			m_listbox.GetWindowRect(&Rect);
			ScreenToClient(Rect);


			pointShowTitle.x=Rect.right;
			pointShowTitle.y=Rect.top+ItemRect.bottom;
			pointShowTitle.y-=37;
			m_listbox.GetText(nSel,csCurTitleName);
			SetTimer(MOUSE_SHOW_TIMER_ID,10,0);
		}
	}
}
//________________________________________________________________________________
void CAutocomplectionCtrl::OnListBoxChanged()
{
	CString temp, curword;
	m_listbox.GetText(m_listbox.GetCurSel(),temp);


	ShowDescription();

	
	GetCurWord(curword);

	CHARRANGE curSel;
	GetSel(curSel);
	temp=temp.Right(temp.GetLength()-curword.GetLength());
	ReplaceSel(temp, TRUE);
	SetSel(curSel.cpMin, curSel.cpMin + temp.GetLength());

	SetFocus();
}


BOOL CAutocomplectionCtrl::GetCurWord(CString &curword)
{
	CPoint ptStart;
	CPoint ptEnd;
	GetSelection(ptStart,ptEnd);

	CString Str = GetLineChars(ptStart.y);
	if(Str.IsEmpty())
		return 0;

	int n2=ptStart.x;
	int n1=n2-1;
	while(n1>=0)
	{
		unsigned char c=Str[n1];
		if(c!='#'&&c<'A'&&(c<'0'||c>'9')&&(c!='.'||m_fast_mode==0))
			break;
		n1--;
	}

	curword=Str.Mid(n1+1,n2-n1-1);
	return TRUE;//!curword.IsEmpty();
}
CString CAutocomplectionCtrl::GetCurWord(CPoint &ptStart,BOOL bUsePoint,BOOL bToRight)
{
	CString Str = GetLineChars(ptStart.y);
	int nLineLength = GetLineLength(ptStart.y);
	int n2=ptStart.x;
	int n1=n2-1;
	while(n1>=0)
	{
		unsigned char c=Str[n1];
		if(c!='#'&&c<'A'&&(c<'0'||c>'9')&&(c!='.'||bUsePoint==0))
			break;
		n1--;
	}
	if(bToRight)
	while(n2<nLineLength)
	{
		unsigned char c=Str[n2];
		if(c<'A'&&(c<'0'||c>'9')&&(c!='.'||bUsePoint==0))
			break;
		n2++;
	}

	ptStart.x=n1+1;
	return Str.Mid(n1+1,n2-n1-1);
}


void CAutocomplectionCtrl::OnCharRight()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_RIGHT, 1, 57);
	else
		CPARENTWND::OnCharRight();
}
void CAutocomplectionCtrl::OnCharLeft()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_LEFT, 1, 57);
	else
		CPARENTWND::OnCharLeft();
}
void CAutocomplectionCtrl::OnCharUp()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_UP, 1, 57);
	else
	{
		m_TitleTip.Hide();
		CPARENTWND::OnLineUp();
	}
}
void CAutocomplectionCtrl::OnCharDown()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_DOWN, 1, 57);
	else
	{
		m_TitleTip.Hide();
		CPARENTWND::OnLineDown();
	}
}
void CAutocomplectionCtrl::OnPageDown()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_NEXT, 1, 57);
	else
		CPARENTWND::OnPageDown();
}
void CAutocomplectionCtrl::OnPageUp()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_PRIOR, 1, 57);
	else
		CPARENTWND::OnPageUp();
}
void CAutocomplectionCtrl::OnHome()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_HOME, 1, 57);
	else
		CPARENTWND::OnHome();
}
void CAutocomplectionCtrl::OnHome2()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_HOME, 1, 57);
	else
		CPARENTWND::OnTextBegin();
}

void CAutocomplectionCtrl::OnLineEnd()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_END, 1, 57);
	else
		CPARENTWND::OnLineEnd();
}
void CAutocomplectionCtrl::OnTextEnd()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_END, 1, 57);
	else
		CPARENTWND::OnTextEnd();
}
void CAutocomplectionCtrl::OnEditTab()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_TAB, 1, 57);
	else
		CPARENTWND::OnEditTab();
}
void CAutocomplectionCtrl::OnEditDelete()
{
	if(m_listbox.m_hWnd)
	{
		OnKeyDown(VK_ESCAPE, 1, 57);
	}
	else
		CPARENTWND::OnEditDelete();
}
void CAutocomplectionCtrl::OnEditDeleteBack()
{
	if(m_listbox.m_hWnd)
		OnKeyDown(VK_LEFT, 1, 57);
	else
	{
		CPARENTWND::OnEditDeleteBack();
		if (m_myauto&&m_autocompleted==0)
		if(nCursorColor==COLORINDEX_NORMALTEXT||nCursorColor==COLORINDEX_KEYWORD)
		{
			IntelliComplete();
		}
	}
}

void CAutocomplectionCtrl::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	/* Check for virtual keys if the listbox is open*/
	if(m_listbox.m_hWnd)
	{
		switch (nChar)
		{
		case (VK_UP):
		case (VK_DOWN):
		case (VK_PRIOR):
		case (VK_NEXT):
		case (VK_HOME):
		case (VK_END):
			{
				m_listbox.SendMessage(WM_KEYDOWN, nChar, MAKELPARAM(nRepCnt, nFlags));
				return;
			}
		case (VK_TAB):
			{
				CHARRANGE curSel;
				GetSel(curSel);
				SetSel(curSel.cpMax,curSel.cpMax);
				
				RemoveListbox();
				
				return;
			} 
		case (VK_RIGHT):
			{
				CHARRANGE curSel;
				GetSel(curSel);
				SetSel(curSel.cpMin+1, curSel.cpMax);
				
				Autocomplete( FALSE );
				
				return;
			}
		case (VK_LEFT):
			{
				CString cword;
				GetCurWord(cword);
				
				if (!cword.IsEmpty())
				{
					CHARRANGE curSel;
					GetSel(curSel);
					SetSel(curSel.cpMin-1, curSel.cpMax);
					
					Autocomplete( FALSE );
				}
				else
				{
					ReplaceSel("");
					RemoveListbox();
				}
				return;
			}
		case (VK_ESCAPE):
			{
				RemoveListbox();
				
				ReplaceSel("");
				
				return;
			}
		}
	}
	
	CPARENTWND::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CAutocomplectionCtrl::Autocomplete(BOOL givenext)
{
	CHARRANGE curSel;
	CString text, act, rep;
	CStringArray strings;
	
	if(!GetCurWord(text))
		return;
	
	GetSel(curSel);
	
	if (curSel.cpMax != curSel.cpMin)
		act = GetSelText();
	
	rep=complete(text, act, strings);
	if (!givenext)
		rep= act;

	if(m_fast_mode&&rep.IsEmpty())
	{
		m_autocompleted=0;
		RemoveListbox();
		return;
	}


	m_autocompleted=TRUE;

	if (strings.GetSize()!=0)
	{
		if (m_showlistbox)
			if (!m_listbox.m_hWnd)
			{
				if(m_pTextBuffer)
				if(m_pTextBuffer->m_bUndoGroup)
					m_pTextBuffer->FlushUndoGroup(this);
				m_pTextBuffer->BeginUndoGroup();
			}

		ReplaceSel(rep,TRUE);
		SetSel(curSel.cpMin, curSel.cpMin + rep.GetLength());

		if (m_showlistbox)
		{
			int nTextLength=rep.GetLength()+text.GetLength();
			CPoint ptCursorPos=GetCursorPos();
			int nOffset=CalculateActualOffset(ptCursorPos.y, ptCursorPos.x);

			CRect p;
			p.top=(ptCursorPos.y-m_nTopLine+1) * GetLineHeight();
			p.left=(2+nOffset-m_nOffsetChar-nTextLength) * GetCharWidth();
			CRect curRect;
			GetWindowRect(curRect);

			p.right=p.left+200;
			
			if (!m_listbox.m_hWnd)
			{
				m_listbox.Create(LBS_OWNERDRAWFIXED |  LBS_NOINTEGRALHEIGHT | WS_VSCROLL | WS_TABSTOP | WS_VSCROLL | WS_CHILD | WS_VISIBLE | LBS_STANDARD | LBS_HASSTRINGS , p, this,IDC_LIST);
				m_listbox.SetFont(GetFont());

				if (m_listallwords)
				{
					for (int i=0; i<m_keywords.GetSize(); i++)
					{
						int nImage=(int)aImages[m_keywords[i]];
						m_listbox.AddString(m_keywords[i],nImage);
					}

					p.bottom = p.top + m_listbox.GetItemHeight(0) * min(m_keywords.GetSize()+1,10);	// The cbox must not be too long
					if(curRect.bottom<p.bottom)
						p.OffsetRect(0,-p.Height()-GetLineHeight());
					m_listbox.MoveWindow(&p);
				}
			}
			else  
			{
				if (!m_listallwords)
					m_listbox.ResetContent();
			} 
				

			if (!m_listallwords)
			{
				p.bottom = p.top + m_listbox.GetItemHeight(0) * min(strings.GetSize()+1,10);	// The cbox must not be too long
				if(p.bottom>curRect.Height())
					p.OffsetRect(0,-p.Height()-GetLineHeight());

				m_listbox.MoveWindow(&p);
				for (int i=0; i<strings.GetSize(); i++)
				{
					int nImage=(int)aImages[strings[i]];
					m_listbox.AddString(strings[i],nImage);
				}
			}
			
			m_listbox.SelectString(-1,text + rep);
			ShowDescription();
		}
	}
	else
	{
		if (!m_listbox.m_hWnd)
			return;

		m_listbox.SetCurSel(-1);
		
		/*looking for nearest*/
		int nofitems=m_listbox.GetCount();
		CString temp;

		for (int i=0; i < nofitems; i++)
		{
			m_listbox.GetText(i,temp);
			if (temp.CompareNoCase(text)>0)
				break;
		}

		m_listbox.SetTopIndex(i--);
	}
		
	return;
}

/************************************************************/
/*				Forwarding some messages					*/
/************************************************************/

BOOL CAutocomplectionCtrl::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg)
	if (pMsg->hwnd==m_listbox.m_hWnd && pMsg->message==WM_CHAR)
		SetFocus();
	
	return CPARENTWND::PreTranslateMessage(pMsg);
}

BOOL CAutocomplectionCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt) 
{
	if (m_listbox.m_hWnd)
		return m_listbox.SendMessage(WM_MOUSEWHEEL, MAKEWPARAM(nFlags, zDelta), MAKELPARAM( pt.x, pt.y));
	
	return CPARENTWND::OnMouseWheel(nFlags, zDelta, pt);
}

/************************************************************/
/*			Some minor overloaded funcions					*/
/************************************************************/

void CAutocomplectionCtrl::OnKillFocus(CWnd* pNewWnd) 
{
//	RemoveListbox(); 
//	CPARENTWND::OnKillFocus(pNewWnd);
}

void CAutocomplectionCtrl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	RemoveListbox();
	CPARENTWND::OnLButtonDown(nFlags, point);
}

void CAutocomplectionCtrl::OnRButtonDown(UINT nFlags, CPoint point) 
{
	RemoveListbox();
	CPARENTWND::OnRButtonDown(nFlags, point);
}
void CAutocomplectionCtrl::OnListBoxDblClick() 
{
	OnChar(VK_RETURN,0,0);
}


void CAutocomplectionCtrl::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	CPARENTWND::OnLButtonDblClk(nFlags, point);

	if (m_learnondblclick)
	{
		CString temp;
		temp=GetSelText();
		temp.TrimRight(" ()=");
		
		AddKeyword(temp);
	}
}

/************************************************************/
/*						Dictionary							*/
/************************************************************/

void CAutocomplectionCtrl::ResetDictionary()
{
	aImages.RemoveAll();
	aElements.RemoveAll();
	m_keywords.RemoveAll();

}


void CAutocomplectionCtrl::GetDictionary(CStringArray &dictionary)
{
	dictionary.RemoveAll();
	dictionary.Copy(m_keywords);
}

BOOL CAutocomplectionCtrl::AddKeyword(const CString& str)
{
	if (str.IsEmpty())
		return FALSE;

	if (!IsKeyword(str))
	{
		int i, tot = m_keywords.GetSize();
		
		for (i = 0 ;i < tot; i++)
		{
			if (compare(m_keywords[i],str)>0)
				break;
		}

		m_keywords.InsertAt(i,str);
		return TRUE;
	}
	return FALSE;
}

BOOL CAutocomplectionCtrl::IsKeyword(const CString& str)
{
	int bottom=0, top=m_keywords.GetSize(), middle, comp;

		while(1)
		{
			if (bottom >= top)
				return FALSE;

			middle = top + bottom;
			middle >>= 1;

			comp=compare(m_keywords[middle],str);

			if (!comp)
				return TRUE;
		
			if (comp < 0)
				bottom=middle+1;
			else
				top=middle;
		}

}

/********************************************************/
/*							OPTIONS						*/
/********************************************************/

void CAutocomplectionCtrl::CaseSensitive(BOOL casesens)
{
	/* I use 2 function pointers */

	if (casesens)
	{
		compare = strcmp;
		comparen = strncmp;
	}
	else
	{
		compare = stricmp;
		comparen = strnicmp;
	}
}

void CAutocomplectionCtrl::TrapEnter(BOOL trap)
{
	m_trapenter = trap;
}

void CAutocomplectionCtrl::ShowListBox(BOOL show)
{
	m_showlistbox = show;
	
	if (!m_showlistbox)
		RemoveListbox();
}

void CAutocomplectionCtrl::Learn(BOOL learn)
{
	m_learn = learn;
}

void CAutocomplectionCtrl::LearnOnDblClick(BOOL learnondbl)
{
	m_learnondblclick = learnondbl;
}

void CAutocomplectionCtrl::ListAllWords(BOOL show)
{
	m_listallwords = show;
}

void CAutocomplectionCtrl::Enable(BOOL enable)
{
	m_enabled = enable;

	if (!m_enabled)
		RemoveListbox();

}

void CAutocomplectionCtrl::IntelliComplete()
{
	static int nPrevY=-1;
	CPoint point=GetCursorPos();
	if(nPrevY!=point.y)
		bUseCashContextData=0;
	nPrevY=point.y;

	if(!bUseCashContextData)
	{
		LoadSysKeyword();
		LoadContextData(0);
	}
	CString text;
	GetCurWord(text);
	if(!text.IsEmpty())
	{
		m_fast_mode=1;
		Autocomplete();
	}
}

//шаблон получения следующей лексемы + контроль		
#define NEXT(i)	i++;	if(i>=translate.LexemList.GetSize()) break; else pLex=&translate.LexemList[i];

#define LAST_NAME "@last"

//________________________________________________________________________________
BOOL CAutocomplectionCtrl::LoadIntelliList(CPoint point,BOOL bUsePoint,BOOL bAllColor)
{
	m_fast_mode=0;
	ResetDictionary();

	if(point.y<0)
		return 0;
	CDC dc;CRect rc;
	DrawSingleLine(&dc,rc,point.y);
	
	
	CString csWord;//текущая переменная
	CString csTypeWord;//тип текущей переменной

	CPoint point_temp=point;
	csWord=GetCurWord(point_temp,bUsePoint,0);
	if(mUpper(csWord)=="КОНТЕКСТ")//Контекст
	{
		bUseCashContextData=0;
		LoadContextData(0);
		return 1;
	}
	else
	if(mUpper(csWord)=="БАЗОВЫЙОБЪЕКТ")//БазовыйОбъект
	{
		CMetaDocument *pDoc=(CMetaDocument *)GetDocument();
		if(pDoc)
		{
		CMetaObject *pObject=pDoc->GetObj();
		if(pObject)
		{
			int nType;
			CString csType=AfxGetModuleManager()->GetObjectName(pObject->csPrevFileName,nType);
			pObject=AfxGetModuleManager()->pMetadata->GetMetaObject(OBJECTSNAME+"\\"+csType,"",1);
			if(pObject)
			if(!pObject->csParam1.IsEmpty())//если есть родитель
			{
				aElements.RemoveAll();
				return AddKeywordFromObjectName(pObject->csParam1,MAX_OBJECTS_LEVEL);
			}
		}
		}
	}
	else
	if(mUpper(csWord)=="ФОРМА")//Форма
	{
		aElements.RemoveAll();
		return AddKeywordFromObjectName("CValueForm",MAX_OBJECTS_LEVEL);
	}


	
	//1. Сначала собираем текст
	CString text;
	
	text=GetLineChars(point.y);
	text=text.Left(point.x);
	
	char c=0;
	if(text.GetLength()>0)
		c=text.GetAt(text.GetLength()-1);
	
	if(COLORINDEX_COMMENT!=nCursorColor&&(bAllColor||COLORINDEX_NORMALTEXT==nCursorColor||c==')'))
	{
		CMapStringToString listVar;//список типизированных переменных

		//загрузка атрибутов формы и метаданных
		AddMetaFormVariable();
		for(int i=0;i<aElements.GetSize();i++)
			if(!aElements[i].csType.IsEmpty())
				listVar[mUpper(CString(aElements[i].csName))]=aElements[i].csType;

		text=GetCurrentContextText(point.y,listVar)+text;
		//OutToDebug("%s",text);
		AddToListVariablesFromCode(text,listVar);
	

		if(csWord.IsEmpty())
		{
			if(bAllColor)//требуется переменная - типа объект
				return 0;
			csTypeWord=listVar[LAST_NAME ];
		}
		else
			csTypeWord=listVar[mUpper(csWord)];
		
		//4. Ищем атрибуты и функции найденного типа
		if(!csTypeWord.IsEmpty())
		{
			if(mUpper(csWord)=="КОНТЕКСТ")//Контекст
				LoadContextData(0);
			else
			{
				aElements.RemoveAll();
				return AddKeywordFromObjectName(csTypeWord,MAX_OBJECTS_LEVEL);
			}
		}
	}
	return 0;
}


//________________________________________________________________________________
//Вывод списка методов агрегатного объекта (при нажатии точки)
int CAutocomplectionCtrl::IntelliPoint(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	CPoint point=GetCursorPos();
	LoadIntelliList(point,!m_fast_mode);

	if(m_keywords.GetSize()>0)
	{
		CPARENTWND::OnChar(nChar, nRepCnt, nFlags);
		Autocomplete();
		return 1;
	}
	return 0;
}

//________________________________________________________________________________
//загрузка переменных связанных с метаданными и с формой
void CAutocomplectionCtrl::AddMetaFormVariable()//CArray <CString,CString> &aName,CArray <CString,CString> &aType,CArray <UINT,UINT> &aListImage)
{
	//поддержка атрибутов формы
	CModuleDoc *pDoc2=(CModuleDoc*)GetDocument();
	CFormEditor *pForm=pDoc2->GetForm();
	if(pDoc2)
	if(pForm)
	{
		CString csText;
		pForm->GetWindowText(csText);
		CValue AllElements=pForm->AllElements;
		for(int i=1;i<=AllElements.GetSizeArray();i++)
		{
			CValue Val=AllElements.GetAt(i);
			CString csName=Val.GetAt("Идентификатор").GetString();
			CString csType=Val.GetAt("Тип значения").GetString();
			if(!csName.IsEmpty())
			{
				csName.TrimRight();
				CModuleElementInfo data;
				data.csName=csName;
				data.csType=csType;
				data.nImage=360;
				aElements.Add(data);
			}
		}
	}

	//поддержка атрибутов метаданных
	CMetaDocument *pDoc=(CMetaDocument *)GetDocument();
	if(pDoc)
	if(pDoc->GetObj())
	{
		CArray<CString,CString> aVariable;
		CArray<CString,CString> aType2;
		if(AfxGetModuleManager())
		if(AfxGetModuleManager()->GetAttrVariable(pDoc->GetObj()->csPrevFileName,aVariable,aType2))
		{
			for(int i=0;i<aVariable.GetSize();i++)
				if(!aVariable[i].IsEmpty())
				{
					aVariable[i].TrimRight();

					CModuleElementInfo data;
					data.csName=aVariable[i];
					data.csType=aType2[i];
					data.nImage=359;
					aElements.Add(data);
				}
		}
	}
}
//________________________________________________________________________________
void CAutocomplectionCtrl::LoadContextData(BOOL bReload)
{
	CModuleView *pModule=(CModuleView *)this;

	CArray <UINT,UINT> aListImage;

	if(pModule)
	if(!bUseCashContextData||bReload)
	{
		aElements.RemoveAll();
		//ResetDictionary();

		pModule->GetContextProc();//aProcName,aProcDescription,aListImage,aListLine);
		AddMetaFormVariable();

		for(int i=0;i<aElements.GetSize();i++)
		{
			if(aElements[i].nImage==351)
				aElements[i].nImage=356;
			else
			if(aElements[i].nImage==352)
				aElements[i].nImage=357;
		}

		//список локальных переменных
		CPoint point=GetCursorPos();
		CMapStringToString listVar;//рудимент
		CString text=GetCurrentContextText(point.y,listVar);
		AddToListVariablesFromCode(text,listVar);

		if(!GetDocument())
			return;

		//Родительские модули
		CMetaObject *m_MetaObj=((CMetaDocument*)GetDocument())->GetObj();
		CMetadataTree *m_MetaTree=((CMetaDocument*)GetDocument())->GetTree();
		CString csModuleName;
		if(m_MetaTree)
		if(AfxGetModuleManager())
		{
			csModuleName=m_MetaTree->GetPath(m_MetaObj->nTempID);
		}
		try
		{
			if(!AfxGetModuleManager())
				return;

			CCompileModule *pStopParent=0;
			CCompileModule *pContinueParent=0;
			BOOL bStop=0;
			
			CCompileModule *pParent=AfxGetModuleManager()->GetParent(csModuleName,pStopParent,pContinueParent);
			while(pParent)
			{
				if(pParent==pStopParent)
					bStop=1;
				if(pParent==pContinueParent)
					bStop=0;
				if(!bStop)
					for(int i=0;i<pParent->cByteCode.aExportList.GetSize();i++)
					{
						CModuleElementInfo data;
						data.csModuleName=pParent->cByteCode.csModuleName;
						data.csName=pParent->cByteCode.aExportList[i];
						int nLine=(int)pParent->cByteCode.ExportFuncList[mUpper(data.csName)]-1;
						if(nLine>=0)
						{
							data.nLine=pParent->cByteCode.CodeList[nLine].nNumberLine;
							data.nImage=356;
							aElements.Add(data);
						}
					}
					pParent=pParent->pParent;
			}
		}
		catch(...)
		{
		}


	}
	bUseCashContextData=1;

//	m_keywords.RemoveAll();
//	aImages.RemoveAll();
	RefreshContextList();
	AddKeyword("Форма");
}

void CAutocomplectionCtrl::RefreshContextList()
{
	for(int i=0;i<aElements.GetSize();i++)
	{
		AddKeyword(aElements[i].csName);
		aImages[aElements[i].csName]=(void*)aElements[i].nImage;
	}
}

//________________________________________________________________________________
//получение текста текущего контекста - от начала процедуры до текущего контекста
CString CAutocomplectionCtrl::GetCurrentContextText(int nYPos,CMapStringToString &listVar)
{
	//учитываем локальный контекст (идем снизу - вверх)
	CString csLocalText;
	for(int nLC=nYPos-1;nLC>=0;nLC--)
	{
		CString Str = GetLineChars(nLC);
		int nLength = GetLineLength(nLC);
		Str=Str.Left(nLength);
		CString Str0=Str;
		Str.TrimLeft();
		Str.MakeUpper();
		//т.к. нас также интересуют комментарии до процедуры, то неограничиваемся только объявлениями проц...
		if(Str.Left(12)=="КОНЕЦФУНКЦИИ")
			break;
		if(Str.Left(14)=="КОНЕЦПРОЦЕДУРЫ")
			break;
		if(Str.Left(11)=="ENDFUNCTION")
			break;
		if(Str.Left(12)=="ENDPROCEDURE")
			break;

		AddToListVariablesFromComment(Str,listVar,0);

		csLocalText=Str0.Left(nLength)+"\n"+csLocalText;
	}

	//учитываем объявление переменных в начале модуля (теперь идем сверху - вниз)
	CString csHeaderText;
	for(int i=0;i<nLC;i++)
	{
		CString Str = GetLineChars(i);
		int nLength = GetLineLength(i);
		Str=Str.Left(nLength);
		CString Str0=Str;
		Str.TrimLeft();
		Str.MakeUpper();
		if(Str.Left(7)=="ФУНКЦИЯ")
			break;
		if(Str.Left(9)=="ПРОЦЕДУРА")
			break;
		if(Str.Left(8)=="FUNCTION")
			break;
		if(Str.Left(9)=="PROCEDURE")
			break;


		AddToListVariablesFromComment(Str,listVar,1);
		csHeaderText=csHeaderText+Str0.Left(nLength)+"\n";
	}

	//вставляем пустые строки вместо пропущенного текста - для правильного перехода на строки
	CString csSkipText;
	csSkipText.GetBuffer(10+(nLC-i)*2);//должно хватить
	for(;i<=nLC;i++)
		csSkipText+="\n";

	return csHeaderText+csSkipText+csLocalText;
}

//________________________________________________________________________________
//Парсинг строки кода
void CAutocomplectionCtrl::AddToListVariablesFromCode(CString text,CMapStringToString &listVar)
{
//	text.TrimRight();
//	text.TrimLeft();
	listVar[LAST_NAME]="";

	CMapStringToPtr aListVarDef;//список переменных объявленных через оператор Перем (Var)


	//2. Затем разбиваем на лексемы
	if(!text.IsEmpty())
	{
		CTranslateModule translate;
		translate.Clear();
		translate.Load(text);
		
		try
		{
			translate.PrepareLexem();
		}
		catch(CTranslateError *)
		{
			return;
		};
		
		//3. Определяем тип текущей переменной:
		//ищем сист. функцию СоздатьОбъект и присваивание переменных
		for(int i=0;i<translate.LexemList.GetSize();i++)
		{
			CLexem* pLex=&translate.LexemList[i];
			
			//пропускаем условие
			if(pLex->nType==KEYWORD&&pLex->nData==KEY_IF)
			{
				while(true)
				{
					NEXT(i);
					if(pLex->nType==KEYWORD&&pLex->nData==KEY_THEN)
						break;
				}
				NEXT(i);
			}
			//пропускаем заголовок циклов WHILE
			if(pLex->nType==KEYWORD&&pLex->nData==KEY_WHILE)
			{
				while(true)
				{
					NEXT(i);
					if(pLex->nType==KEYWORD&&pLex->nData==KEY_DO)
						break;
				}
				NEXT(i);
			}
			//пропускаем тернерное выражение
			if(pLex->nType==DELIMETER&&pLex->nData=='?')
			{
				while(true)
				{
					NEXT(i);
					if(pLex->nType==DELIMETER&&pLex->nData==';')
						break;
				}
				NEXT(i);
			}

			//Объявление переменных
			if(KEYWORD==pLex->nType&&KEY_VAR==pLex->nData)
			{
				while(true)
				{
					NEXT(i);
					if(DELIMETER==pLex->nType&&pLex->nData!=',')
						break;
					if(IDENTIFIER==pLex->nType)
					{
						CModuleElementInfo data;
						data.csName=pLex->vData.sData;
						data.nLine=pLex->nNumberLine;
						data.nImage=358;
						aElements.Add(data);

						aListVarDef[mUpper(data.csName)]=(void*)1;
					}
				}
				//NEXT(i);
				continue;
			}
			//Объявление функций
			//if(0)
			if(KEYWORD==pLex->nType&&(KEY_FUNCTION==pLex->nData||KEY_PROCEDURE==pLex->nData))
			{
				NEXT(i);
				NEXT(i);

				while(true)
				{
					NEXT(i);
					if(DELIMETER==pLex->nType&&pLex->nData!=',')
						break;
					if(IDENTIFIER==pLex->nType)
					{
						CModuleElementInfo data;
						data.csName=pLex->vData.sData;
						data.nLine=pLex->nNumberLine;
						data.nImage=356;
						aElements.Add(data);

						aListVarDef[mUpper(data.csName)]=(void*)1;

						NEXT(i);
						if(DELIMETER==pLex->nType&&'='==pLex->nData)
						{
							NEXT(i);
						}
						else
						{
							i--;
						}
					}
				}
				continue;
			}
			
			
			CString csVarName;
			if(pLex->nType!=IDENTIFIER)
				continue;
			csVarName=pLex->vData.sData;
			int nNumberLine=pLex->nNumberLine;

			if(mUpper(csVarName)=="СОЗДАТЬОБЪЕКТ"||mUpper(csVarName)=="CREATEOBJECT")//СоздатьОбъек("Метаданные")
			{
				csVarName="";
			}
			else
			{
				NEXT(i);
				
				if(pLex->nType!=DELIMETER||pLex->nData!='=')
					continue;
				NEXT(i);

				//итак было присваивание!
				void *p;
				if(!aListVarDef.Lookup(mUpper(csVarName),p))
				{
					CModuleElementInfo data;
					data.csName=csVarName;
					data.nLine=nNumberLine;
					data.nImage=358;
					aElements.Add(data);

				}
				
				if(pLex->nType!=IDENTIFIER)
					continue;
				
			}



			
			CString csFuncName=pLex->vData.sData;
			if(mUpper(csFuncName)=="СОЗДАТЬОБЪЕКТ"||mUpper(csFuncName)=="CREATEOBJECT")//СоздатьОбъект("Метаданные")
			{
				NEXT(i);
				if(pLex->nType!=DELIMETER||pLex->nData!='(')
					continue;
				NEXT(i);
				if(pLex->nType!=CONSTANT)
					continue;
				csFuncName=pLex->vData.sData;
				listVar[LAST_NAME ]=csFuncName;
				listVar[mUpper(csVarName)]=csFuncName;
				
				NEXT(i);
				if(pLex->nType!=DELIMETER||pLex->nData!=')')
					continue;
				
				NEXT(i);
				if(pLex->nType==DELIMETER&&pLex->nData=='.')//СоздатьОбъект("Метаданные").Массив
				{
					listVar[mUpper(csVarName)]="";
					continue;
				}
				i--;
				
				
			}
			else
			{
				NEXT(i);
				if(pLex->nType==DELIMETER&&pLex->nData==';')
					listVar[mUpper(csVarName)]=listVar[mUpper(csFuncName)];
				continue;
			}
			
			NEXT(i);
		}
	}
}
//________________________________________________________________________________
//Парсинг строки кода и выискивание там спец. комментария типа "//Спр:Справочник" - информации о типе
void CAutocomplectionCtrl::AddToListVariablesFromComment(CString StrCode,CMapStringToString &listVar,BOOL nMoveToDown)
{
	int nIndex=StrCode.Find("//");
	if(nIndex>=0)
	{
		StrCode=StrCode.Mid(nIndex+2);
		nIndex=StrCode.Find(":");
		if(nIndex>0)
		{
			CString csVarName=StrCode.Left(nIndex);
			StrCode=StrCode.Mid(nIndex+1);
			StrCode.TrimLeft();
			nIndex=StrCode.Find(" ");
			if(nIndex>0)
				StrCode=StrCode.Left(nIndex);
			
			csVarName.TrimRight();
			if(nMoveToDown==0)//т.к. идем снизу вверх, то проверяем стоит ли добавлять менее свежие данные
			{
				if(!listVar[csVarName].IsEmpty())
					return;
			}

			listVar[csVarName]=StrCode;

		}
	}
}

//________________________________________________________________________________
//Добавление списка методов по названию типа
int CAutocomplectionCtrl::AddKeywordFromObjectName(CString csTypeWord,int nRecursiveCounter)
{
	nRecursiveCounter--;
	if(nRecursiveCounter<0)
		return 1;

	try
	{
		CCompileModule *pModule;
		CString csTypeWord2=csTypeWord;
		int nIndex=csTypeWord2.Find(".");
		if(nIndex>0)
		{
			csTypeWord2=csTypeWord2.Left(nIndex);
		}
		
		if(!AfxGetModuleManager())
			return 0;

		CString csModuleName1=OBJECTSNAME+"\\"+csTypeWord2+"\\"+OBJMODULE+"\\"+ENTERPRISEMODULENAME;
		pModule=AfxGetModuleManager()->GetCompileModule(csModuleName1,0,0,1);
		
		if(pModule)
		{
			CMapStringToPtr *pList=&pModule->cContext.cFunctions;//методы

			//список атрибутов (экспортных переменных)
			CArray <CString,CString&> &aExportList=pModule->cByteCode.aExportList;
			for(int i=0;i<aExportList.GetSize();i++)
			{
				void *p;
				if(!pList->Lookup(mUpper(aExportList[i]),p))
				{
					//AddKeyword(aExportList[i]);
					CModuleElementInfo data;
					data.csName=aExportList[i];
					data.csDescription=aExportList[i];
					data.nImage=358;
					data.nLine=0;
					data.csModuleName=csModuleName1;
					aElements.Add(data);
				}
			}

			//список методов
			CFunction *pFunction;
			POSITION pos;
			CString csKey;
			for( pos = pList->GetStartPosition(); pos != NULL; )
			{
				pList->GetNextAssoc( pos, csKey, (void*&)pFunction );
				if(pFunction)
					if(pFunction->bExport)
					{
						//AddKeyword(pFunction->csRealName);
						CModuleElementInfo data;
						data.csName=pFunction->csRealName;
						data.csDescription=pFunction->csShortDescription;
						data.nImage=356;
						data.nLine=pFunction->nNumberLine;
						data.csModuleName=csModuleName1;
						aElements.Add(data);
					}
			}
			RefreshContextList();


			CString csModuleName2=OBJECTSNAME+"\\"+csTypeWord2;
			CMetaObject *pObject=AfxGetModuleManager()->pMetadata->GetMetaObject(csModuleName2,"",1);
			if(pObject)
			{
				if(!pObject->csParam1.IsEmpty())//если есть родитель
				{
					return AddKeywordFromObjectName(pObject->csParam1,nRecursiveCounter);
				}
			}
		}
		else//это системный объект или ОЛЕ или нет такого объекта вообще...
		{
			CValue vObject;
			vObject.CreateObject(csTypeWord);
			for(int i=0;i<vObject.GetNMethods();i++)
			{
				CString csName=vObject.GetMethodName(i,1);
				CString csDescription=vObject.GetMethodDescription(i,1);
				//AddKeyword(csName);
				CModuleElementInfo data;
				data.csName=csName;
				data.csDescription=vObject.GetMethodDescription(i,1);
				aElements.Add(data);
			}
			RefreshContextList();
		}
		return 1;
	}
	catch(...)
	{
		//значит нет такого объекта вообще...
	}
	return 0;
}