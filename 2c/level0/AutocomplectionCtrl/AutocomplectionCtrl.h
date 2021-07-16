// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
#ifndef __AUTOCOMPLECTIONCTRL_H__
#define __AUTOCOMPLECTIONCTRL_H__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// AutocomplectionCtrl.h : header file
//
#include "../SyntaxColor/CCrystalEditView.h"
#include "../IconListBox.h"
#include "../GridCtrl_src/TitleTip.h"

#define MOUSE_SHOW_TIMER_ID	2001
#define LINE_SHOW_TIMER_ID	2002

//#define CPARENTWND CRichEditCtrl
#define CPARENTWND CCrystalEditView
/////////////////////////////////////////////////////////////////////////////
// CAutocomplectionCtrl window

struct CModuleElementInfo
{
	CModuleElementInfo()
	{
		nImage=0;
		nLine=-1;
	};
	CString csName;//имя элемента
	CString csDescription;//тип объекта
	CString csType;//тип объекта
	int nImage;//номер картинки
	int nLine;//номер строки кода, где находится элемент
	CString csModuleName;//Имя модуля

	CString csNameFind;
};
class CModuleInfo:public CArray <CModuleElementInfo,CModuleElementInfo&>
{
public:
	int Find(CString csName)
	{
		csName=mUpper(csName);
		for(int i=0;i<GetSize();i++)
		{
			if(GetAt(i).csNameFind==csName)//identical 
				return i;
		}
		return -1;
		//return Find(csName.GetBuffer(0));
	}
	CString GetDesription(CString csName)
	{
		int i=Find(csName);
		if(i>0)
		{
			return GetAt(i).csDescription;
		}
		return "";
	}
	int GetLine(CString csName)
	{
		int i=Find(csName);
		if(i>0)
		{
			return GetAt(i).nLine;
		}
		return -1;
	}
	CString GetModuleName(CString csName)
	{
		int i=Find(csName);
		if(i>0)
		{
			return GetAt(i).csModuleName;
		}
		return "";
	}

	int Add(CModuleElementInfo &data)
	{
		data.csNameFind=mUpper(data.csName);
		return CArray <CModuleElementInfo,CModuleElementInfo&>::Add(data);
	};
};

extern BOOL bNotUseUndo;
class CAutocomplectionCtrl : public CPARENTWND
{
// Construction
public:
	CAutocomplectionCtrl();
	
// Attributes
public:
	CTitleTip m_TitleTip;
	CPoint pointShowTitle;
	CString csCurTitleName;

	CModuleInfo aElements;
//	CMapStringToPtr aListDescription;//описание
//	CMapStringToPtr aListLineNumber;//номера строк
//	CString			csModuleName;//имя(путь) модуля

	BOOL bUseCashContextData;
	CMapStringToPtr aImages;
/*	CArray <CString,CString> aProcName;//название процедур и функций
	CArray <UINT,UINT> aListLine;//номер строки в модуле
	CArray <CString,CString> aProcDescription;
*/

// Operations
protected:

	int AddKeywordFromObjectName(CString csTypeWord,int nRecursiveCounter);
	void AddMetaFormVariable();//CArray <CString,CString> &aName,CArray <CString,CString> &aType,CArray <UINT,UINT> &aListImagez);
	void LoadContextData(BOOL bReload);
	void IntelliComplete();
	int IntelliPoint(UINT nChar, UINT nRepCnt, UINT nFlags);
	void Autocomplete(BOOL givenext = TRUE);
	CString complete(const CString& str, const CString& actual, CStringArray &matching);
	inline void RemoveListbox();
	void LoadSysKeyword();
	CString GetCurrentContextText(int nYPos,CMapStringToString &listVar);
	void AddToListVariablesFromCode(CString StrCode,CMapStringToString &listVar);
	void AddToListVariablesFromComment(CString StrCode,CMapStringToString &listVar,BOOL nMoveToDown);

	void ShowDescription();
	BOOL LoadIntelliList(CPoint point,BOOL bUsePoint=0,BOOL bAllColor=0);
	void RefreshContextList();

// Data
	BOOL m_autocompleted;
	CStringArray m_keywords;
	BOOL m_bInForcedChange;

	CIconListBox m_listbox;

// Preferences
	BOOL m_learn;
	BOOL m_showlistbox;
	BOOL m_trapenter;
	BOOL m_learnondblclick;
	BOOL m_listallwords;
	BOOL m_myauto;
	BOOL m_fast_mode;

// Comprare funcions
	int (*compare)(const char*, const char*);
	int (*comparen)(const char*, const char*, size_t n);

	
public:

	//пооддержка методов как в RichEdit-е
	void GetSel(CHARRANGE &curSel)
	{
		CPoint ptStart;
		CPoint ptEnd;
		GetSelection(ptStart,ptEnd);
		curSel.cpMin=min(ptStart.x,ptEnd.x);
		curSel.cpMax=max(ptStart.x,ptEnd.x);
	};
	void SetSel(int n1, int n2)
	{
		CPoint ptCursorPos=GetCursorPos();
		CPoint ptCursorPos2=ptCursorPos;
		int nLength = GetLineLength(ptCursorPos.y);
		ptCursorPos.x=min(n1,nLength);
		ptCursorPos2.x=min(n2,nLength);
		SetSelection(ptCursorPos, ptCursorPos2);
	};
	void ReplaceSel(CString csTect,BOOL b=FALSE)
	{
		ReplaceSelection(csTect);
	};
	CString GetSelText()
	{
		CPoint ptStart;
		CPoint ptEnd;
		GetSelection(ptStart,ptEnd);
		if(ptStart.y>=0)
		{
			CString Str = GetLineChars(ptStart.y);
			int nLength = GetLineLength(ptStart.y);
			int n1=min(ptStart.x,ptEnd.x);
			int n2=max(ptStart.x,ptEnd.x);
			return Str.Mid(n1,n2-n1);
		}
		return "";
	};


	void Enable(BOOL enable = TRUE);
	/* Preferences */
	void CaseSensitive(BOOL casesens = TRUE);
	void TrapEnter(BOOL trap = TRUE);
	void Learn(BOOL learn = TRUE);
	void ShowListBox(BOOL show = TRUE);
	void ListAllWords(BOOL show = TRUE);
	void LearnOnDblClick(BOOL learnondbl = TRUE);

	/* Dictionary */
	void GetDictionary(CStringArray& dictionary);
	void ResetDictionary();
	BOOL AddKeyword(const CString& str);
	BOOL IsKeyword(const CString& str);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAutocomplectionCtrl)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
	virtual ~CAutocomplectionCtrl();

	// Generated message map functions
protected:
	BOOL m_enabled;
	CString GetCurWord(CPoint &ptStart,BOOL bUsePoint,BOOL bToRight=1);
	BOOL GetCurWord(CString &curword);
	//{{AFX_MSG(CAutocomplectionCtrl)
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnListBoxChanged();
	afx_msg void OnListBoxDblClick();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnCharRight();
	afx_msg void OnCharLeft();
	afx_msg void OnCharUp();
	afx_msg void OnCharDown();

	afx_msg void OnPageUp();
	afx_msg void OnPageDown();
	afx_msg void OnHome();
	afx_msg void OnHome2();
	afx_msg void OnTextEnd();
	afx_msg void OnLineEnd();
	afx_msg void OnEditTab();
	afx_msg void OnEditDelete();
	afx_msg void OnEditDeleteBack();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //__AUTOCOMPLECTIONCTRL_H__
