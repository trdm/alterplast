
#include "stdafx.h"
 
#ifdef _SECDLL
#undef AFXAPI_DATA
#define AFXAPI_DATA __based(__segname("_DATA"))
#endif //_SECDLL

#ifndef __SECAUX_H__
#include "secaux.h"
#endif

#ifdef _SECDLL
#undef AFXAPP_DATA
#define AFXAPP_DATA AFXAPI_DATA
#endif //_SECDLL

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define new DEBUG_NEW

/////////////////////////////////////////////////////////////////////////////
// Cached system metrics, etc

SEC_AUX_DATA secData;

// Initialization code
// Initialization code
SEC_AUX_DATA::SEC_AUX_DATA()
{
	// Cache various target platform version information
	DWORD dwVersion = ::GetVersion();
	bWin4 = (BYTE)dwVersion >= 4;

	// Cached system values (updated in CWnd::OnSysColorChange)
	UpdateSysColors();

	// Standard cursors
	hcurHSplit = NULL;
	hcurVSplit = NULL;
	hCurCrossHairs = NULL;
	hcurArrow = ::LoadCursor(NULL, IDC_ARROW);
	ASSERT(hcurArrow != NULL);

	// cxBorder2 and cyBorder are 2x borders for Win4
	cxBorder2 = bWin4 ? CX_BORDER*2 : CX_BORDER;
	cyBorder2 = bWin4 ? CY_BORDER*2 : CY_BORDER;
}

// Termination code
SEC_AUX_DATA::~SEC_AUX_DATA()
{
}

void SEC_AUX_DATA::UpdateSysColors()
{
	clrBtnFace       = ::GetSysColor(COLOR_BTNFACE);
	clrBtnShadow     = ::GetSysColor(COLOR_BTNSHADOW);
	clrBtnHilite     = ::GetSysColor(COLOR_BTNHIGHLIGHT);
	clrWindowFrame   = ::GetSysColor(COLOR_WINDOWFRAME);
	clrText          = ::GetSysColor(COLOR_WINDOWTEXT);
	clrWindow        = ::GetSysColor(COLOR_WINDOW);
	clrHighlight     = ::GetSysColor(COLOR_HIGHLIGHT);
	clrHighlightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
#ifdef WIN32
	clr3DLight       = ::GetSysColor(COLOR_3DLIGHT);
	clr3DDkShadow    = ::GetSysColor(COLOR_3DDKSHADOW);
#else
	clr3DLight       = ::GetSysColor(COLOR_BTNFACE);
	clr3DDkShadow    = ::GetSysColor(COLOR_WINDOWFRAME);
#endif
}

// helper functions called by macro SEC_TRACE_WINDOW (see secaux.h for more details)
#ifdef WIN32
#if defined _DEBUG
void __secTraceWindow ( FILE * pFile, TCHAR * pszPrefix, CWnd * pwndTrace, TCHAR * pszSuffix )
{
     HWND hwndTrace = 0;
     if (pwndTrace) hwndTrace = *pwndTrace;
     __secTraceWindow(pFile, pszPrefix, hwndTrace, pszSuffix);
}
void __secTraceWindow ( FILE * pFile, TCHAR * pszPrefix, HWND hwndTrace, TCHAR * pszSuffix )
{
     if (! pszPrefix) pszPrefix = _T("");
     if (! pszSuffix) pszSuffix = _T("\n");
     if (! hwndTrace)
          if (pFile)
               _ftprintf(pFile, _T("%snull%s"), pszPrefix, pszSuffix);
          else
               TRACE(_T("%snull%s"), pszPrefix, pszSuffix);
     else {
          TCHAR szCaption[256] = { '\0' }, szWindowChain[3192] = { '\0' };
          //const TCHAR * pszClassName = _T("?unknown?");
          if (hwndTrace) GetWindowText(hwndTrace, szCaption, sizeof szCaption);
          for ( HWND h = hwndTrace;  h;  h = GetParent(h) ) {
               if (szWindowChain[0]) _tcscat(szWindowChain, _T(":"));
               CWnd * w = CWnd::FromHandlePermanent(h);
               const TCHAR * s = _T("???");
               if (w) s = (TCHAR *)w->GetRuntimeClass()->m_lpszClassName;
               _tcscat(szWindowChain, s);
          }
          if (pFile)
               _ftprintf(pFile, _T("%s<hwnd=0x%lx, caption='%s', winchain='%s'>%s"), 
                              pszPrefix, (long)hwndTrace, szCaption, szWindowChain, pszSuffix);
          else
               TRACE(_T("%s<hwnd=0x%lx, caption='%s', winchain='%s'>%s"), 
                     pszPrefix, (long)hwndTrace, szCaption, szWindowChain, pszSuffix);
     }
}
#endif // #if defined _DEBUG
#endif // #ifdef WIN32

/////////////////////////////////////////////////////////////////////////////
