//Авторские права - VTOOLS.RU (info@vtools.ru)
// MessageBar.cpp : implementation file
//

#include "stdafx.h"
#include "enterprise.h"
#include "MessageBar.h"
#include "SourceUT\OXMDIFloatWnd.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CMessageBar dialog
BEGIN_MESSAGE_MAP(CMessageBar, CExtWFF<CMyEdit>)
	//{{AFX_MSG_MAP(CMessageBar)
	ON_WM_LBUTTONDBLCLK()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

void ActivateConfig(CString csText,int nErrorLine);


BOOL ParseTextLines(CString csText)
{
	int nRes=csText.Find("(#Строка");
	if(nRes>0)
	{
		int nErrorLine=atoi(csText.Mid(nRes+8));
		csText=csText.Left(nRes);
		csText.TrimRight();
		csText.TrimLeft();

		ActivateConfig(csText,nErrorLine);
		return 1;
	}
	return 0;
}
CMessageBar::CMessageBar()
{

}

void CMessageBar::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
/*
Общие модули\Глобальный модуль	(#Строка 10):
	Для А=1 По Мета.ОбщиеФормы( <<?>> ) Цикл
Поле агрегатного объекта не обнаружено 'ОбщиеФормы'
*/
	int nLine=LineFromChar();

	if(nLine>=0)
	{
		CString csText;
		for(int i=0;i<3;i++)
		{
			int nRes=GetLine(nLine-i,csText.GetBuffer(1000),1000);
			csText.ReleaseBuffer();
			if(nRes)
			{
				if(ParseTextLines(csText))
					return;
			}
			else
			{
				break;
			}
		}

	}



	CExtWFF<CMyEdit>::OnLButtonDblClk(nFlags, point);
}



void CMessageBar::Init()
{
	pMessageWindow=this;
	//устанавливаем шрифт для окна
	SetLimitText(0);
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
	m_EditLogger.SetEditCtrl(m_hWnd );
	::SetWindowLong(m_hWnd,GWL_STYLE, ::GetWindowLong(m_hWnd,GWL_STYLE) | (
		ES_MULTILINE | ES_AUTOVSCROLL | ES_AUTOHSCROLL | ES_NOHIDESEL | ES_WANTRETURN | WS_VSCROLL
		) );
	//SetReadOnly(1);
}
 
void CMessageBar::OnClear()
{
	SetWindowText("");
};
void CMessageBar::Message(CString Str)
{
	if(!IsWindowVisible())
	{
		CFrameWnd* pParentFrameWnd = GetParentFrame();
		if(pParentFrameWnd)
		{
			static bool bIsStart=1;
			if(bIsStart || pParentFrameWnd->IsWindowVisible())
			{
				OnClear();
				((CMDIFrameWnd*)pParentFrameWnd)->ShowControlBar((CControlBar*)GetParent(), TRUE, 0);
			}
			bIsStart=0;
		}
	}
	Str.Replace("\r\n","\n");
	Str.Replace("\r","\n");
	Str.Replace("\n","\r\n");
	m_EditLogger.AddText( Str+"\r\n");
}
