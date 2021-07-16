// xrnumericedit.cpp : implementation file
//

#include "stdafx.h"
//#include "NumericEdit.h"
#include "xrnumericedit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXrNumericData

IMPLEMENT_DYNCREATE(CXrNumericData, CObject)

CXrNumericData::CXrNumericData() : 
	m_eType  (xrNumericDataTypeDIGIT),
	m_chValue(chNULL)
{
}

CXrNumericData::CXrNumericData(enumNumericDataType eType, TCHAR m_chValue) : 
	m_eType  (eType),
	m_chValue(m_chValue)
{
}

/////////////////////////////////////////////////////////////////////////////
// CXrNumericData operations

void CXrNumericData::operator=(const CXrNumericData& src)
{
	this->m_eType= src.m_eType  ;
	this->m_chValue= src.m_chValue;
}
		
BOOL CXrNumericData::IsInputData(enumNumericDataType eType)
{
	if (m_eType== eType)
		return true;
	
	return false;
}

BOOL CXrNumericData::IsValidInput(TCHAR chNewChar)
{
	BOOL bIsValidInput= FALSE;
	
	switch(m_eType)
	{
	// These are the input types. 
	case xrNumericDataTypeDIGIT             :
		bIsValidInput=_istdigit(chNewChar);
		break;

	case xrNumericDataTypePROMPTSYMBOL:
		bIsValidInput= (chNewChar== chNumericPlaceholderPROMPTSYMBOL);
		break;
	}
	
	return bIsValidInput;
}

#ifdef _DEBUG
void CXrNumericData::AssertValid() const
{
	CObject::AssertValid();
	ASSERT( (m_eType >= 0) && (m_eType < NUMERICDATATYPECOUNT));
	ASSERT( m_chValue != chNULL);
}

void CXrNumericData::Dump(CDumpContext& dc) const
{
	CObject::Dump(dc);
}
#endif

/////////////////////////////////////////////////////////////////////////////
// CXrNumericEdit

CXrNumericEdit::CXrNumericEdit()
	:m_bAutoTab(FALSE),
	m_bInsertMode(TRUE),
	m_bIsNegativeValue(FALSE),
	m_chDecimalSymbol(chPERIOD),
	m_chNegativeSignSymbol(chMINUS),
	m_nCountDigitGrouping(3),
	m_nCountDigitsAfterDecimals(0),
	m_nCountDigitsDecimals(-1),
	m_nCountSpaceSeparator(1),
	m_nDisplayLeadingZero(xrDLZDataTypeFull),
	m_nNegativeNumberFormat(xrNNFDataTypeSRNO),
	m_nNegativeSymbolFormat(xrNSFDataTypeParentheses),
	m_nDisplayDigitsDecimalsFormat(xrDDDFDataTypeOnlyInputs),
	m_rgbNegativeColor(RGB(255, 0, 0)),
	m_strPostfix(""),
	m_strPrefix(""),
	m_strSeparatorGrouping("'"),
	m_nSelectionStart(-1),
	m_nSetTextSemaphor(0)
{
	int nLength;

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, 
		&m_chDecimalSymbol, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL ,
			pchData, nLength);
		memcpy(&m_chDecimalSymbol, pchData, sizeof(m_chDecimalSymbol));
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND ,
			pchData, nLength);
		m_strSeparatorGrouping= pchData;
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SGROUPING,
			pchData, nLength);
		CString strDigitGrouping(pchData);
		nLength= strDigitGrouping.Find(';');
		if (nLength!= -1)
			strDigitGrouping= strDigitGrouping.Left(nLength);
		
		m_nCountDigitGrouping= atoi(strDigitGrouping);
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDIGITS, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_IDIGITS,
			pchData, nLength);
		m_nCountDigitsAfterDecimals= atoi(pchData);
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ILZERO,
			pchData, nLength);
		if (atoi(pchData)== 0)
			m_nDisplayLeadingZero= xrDLZDataTypeParcial;
		else
			m_nDisplayLeadingZero= xrDLZDataTypeFull;
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_INEGNUMBER,
			pchData, nLength);
		switch(atoi(pchData)) {
			case 0:
				m_nNegativeNumberFormat= xrNNFDataTypeRSNSO;
				break;
			case 1:
			case 2:
				m_nNegativeNumberFormat= xrNNFDataTypeRSNO;
				break;
			case 3:
			case 4:
				m_nNegativeNumberFormat= xrNNFDataTypeRNSO;
				break;
		}
		delete pchData;
	}

	nLength=::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, 
		NULL, 0);
	if(nLength)	{
		char* pchData= new char[nLength];
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN,
			pchData, nLength);
		
		memcpy(&m_chNegativeSignSymbol, pchData, sizeof(m_chNegativeSignSymbol));
		delete pchData;
	}

	InitializeListNumericData(IsNumericDataTypeFloat());

	m_rgbCurrentTextColor = GetSysColor(COLOR_WINDOWTEXT);
}

CXrNumericEdit::~CXrNumericEdit()
{
	DeleteContents();
}


BEGIN_MESSAGE_MAP(CXrNumericEdit, CEdit)
	//{{AFX_MSG_MAP(CXrNumericEdit)
	ON_WM_KEYDOWN()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT,OnCut)
	ON_MESSAGE(WM_COPY,OnCopy)
	ON_MESSAGE(WM_PASTE,OnPaste)
	ON_MESSAGE(WM_CLEAR,OnClear)
	ON_MESSAGE(WM_SETTEXT,OnSetText)
	ON_MESSAGE(WM_GETTEXT,OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH,OnGetTextLength)
	ON_MESSAGE(EM_SETSEL,OnSetSel)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CXrNumericEdit message handlers

BOOL CXrNumericEdit::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{
	// We override the create function so that we can update the text 
	// if a mask was provided in the constructor. 
	BOOL bReturn= CEdit::Create(dwStyle, rect, pParentWnd, nID);
/*	if(bReturn)	{		
		// Don't update if there is no data. 
		
		if(!IsEmpty()) {
			FormatNumericData();
			Update();
		}
	}
*/	
	return bReturn;
}

void CXrNumericEdit::PreSubclassWindow() 
{
	CEdit::PreSubclassWindow();
/*	
	if(!IsEmpty()) {
		FormatNumericData();
		Update();
	}
*/
}

LONG CXrNumericEdit::OnCut(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;
	nSelectionEnd= RPtoLP(nSelectionEnd);

	CString strContents= GetNumericInputData(false);
	strContents= strContents.Mid(nSelectionStart, (nSelectionEnd- nSelectionStart));
	SetClipboardData(strContents);
	int nDeleteCount= DeleteRange(nSelectionStart, nSelectionEnd);

	Update(LPtoRP(nSelectionStart));
	return 0L;
}

LONG CXrNumericEdit::OnPaste(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	CString strClipboardText= GetClipboardData();
	if (strClipboardText.IsEmpty())
		return 0L;

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;
	nSelectionEnd= RPtoLP(nSelectionEnd);

	CString strPreviusContents= GetNumericInputData(false);
	CString strLeft= strPreviusContents.Left(nSelectionStart);
	CString strRight= strPreviusContents.Mid(nSelectionEnd);
	CString strNewContents= strLeft+ strClipboardText+ strRight;
	
	if ((!strLeft.IsEmpty() || !strRight.IsEmpty()) && m_bIsNegativeValue)
		strNewContents= "-"+ strNewContents;

	if (!SetNumericInputData(strNewContents, false)) {
		SetNumericInputData(strPreviusContents);
	}

	Update(LPtoRP(nSelectionStart));	
	return 0L;
}

LONG CXrNumericEdit::OnCopy(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;
	nSelectionEnd= RPtoLP(nSelectionEnd);

	CString strContents= GetNumericInputData(false);
	strContents= strContents.Mid(nSelectionStart, (nSelectionEnd- nSelectionStart));
	SetClipboardData(strContents);

	return 0L;
}

LONG CXrNumericEdit::OnClear(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	int nSelectionStart=0;
	int nSelectionEnd=0;
	GetSel(nSelectionStart, nSelectionEnd);

	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;
	nSelectionEnd= RPtoLP(nSelectionEnd);
	int nDeleteCount= DeleteRange(nSelectionStart, nSelectionEnd);

	CEdit::Default();

	Update(LPtoRP(nSelectionStart));
	
	return 0;
}

LONG CXrNumericEdit::OnSetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	CString csNewString=(LPCTSTR)lParam;
	if(csNewString.Compare(GetStringContents())== 0) {
		LONG result= CEdit::Default();
		return result;
	}
	else {
		//ASSERT(m_nSetTextSemaphor==0);
		SetNumericInputData(csNewString, false);
		return TRUE;
	}
}

LONG CXrNumericEdit::OnGetText(UINT wParam, LONG lParam)
{
	UNREFERENCED_PARAMETER(wParam);
	UNREFERENCED_PARAMETER(lParam);

	LPSTR lpszText= (LPSTR)lParam;
	if (lpszText== NULL)
		return CEdit::Default();

	if (IsNegativeValue()) {
		*lpszText= '-';
		lpszText++;
	}

	CString strStringContents= GetNumericInputData(false);
	if (!strStringContents.IsEmpty() && strStringContents[0]== '.')
		strStringContents= "0"+ strStringContents;
	if (strStringContents.GetLength()> (int)wParam) {
		memcpy(lpszText, strStringContents.GetBuffer(0), wParam);
		lpszText[wParam]= '\0';
	}
	else {
		memcpy(lpszText, strStringContents.GetBuffer(0), strStringContents.GetLength());
		lpszText[strStringContents.GetLength()]= '\0';
	}

	return strlen(lpszText) + (IsNegativeValue() ? 1 : 0);
}

LONG CXrNumericEdit::OnGetTextLength(UINT wParam, LONG lParam)
{
	CString strStringContents= GetNumericInputData(false);
	
	return IsNegativeValue() ? strStringContents.GetLength()+ 1 : 
		strStringContents.GetLength();
}

LONG CXrNumericEdit::OnSetSel(UINT wParam, LONG lParam) 
{
	int nSelectionStart= (INT) wParam;
	if (nSelectionStart== -1)
		nSelectionStart= 0;
	
	int nSelectionEnd  = (INT)lParam;
	if (nSelectionEnd== -1 || nSelectionEnd>= LPtoRP(m_listNumericData.GetCount()))
		nSelectionEnd= LPtoRP(m_listNumericData.GetCount());

	int nLogicalStart= RPtoLP(nSelectionStart);
	int nLogicalEnd= RPtoLP(nSelectionEnd);
	if (nLogicalStart< 0)
		nLogicalStart= 0;
	
	if ((nSelectionStart!= LPtoRP(nLogicalStart)) || (nSelectionEnd!= LPtoRP(nLogicalEnd))) {
		SetSel(LPtoRP(nLogicalStart), LPtoRP(nLogicalEnd));
		return 0L;
	}

	return CEdit::Default();
}

void CXrNumericEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	TRACE(_T("CXrNumericEdit::OnKeyDown(\'%c\' (%x), %d, 0x%04x)\n"), 
		nChar, nChar, nRepCnt, nFlags);

	DWORD dwStyle= GetStyle();
	if ((dwStyle & ES_READONLY)== ES_READONLY) {
		CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
		return ;
	}
	
	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)< 0;

	switch(nChar) {
		case VK_DELETE:
			{
				int nSelectionStart=0;
				int nSelectionEnd  =0;
				GetSel(nSelectionStart, nSelectionEnd);

				if (nSelectionStart== nSelectionEnd) {
					/*if (*/DeleteRange(RPtoLP(nSelectionStart), RPtoLP(nSelectionStart)+ 1);/*)*/
						Update(nSelectionStart);
				}
				else {
					if (DeleteRange(RPtoLP(nSelectionStart), RPtoLP(nSelectionEnd)))
						Update(nSelectionStart);
				}
			}
			break;

		case VK_END:
			KeyDownEnd(nChar, nRepCnt, nFlags);
			break;

		case VK_DOWN:
		case VK_RIGHT:
			KeyDownRight(nChar, nRepCnt, nFlags);
			break;
		
		case VK_HOME:
			KeyDownHome(nChar, nRepCnt, nFlags);
			break;

		case VK_LEFT:
		case VK_UP:
			KeyDownLeft(nChar, nRepCnt, nFlags);
			break;

		case VK_INSERT: {
			BOOL bOldInsertMode= IsInsertMode();
			BOOL bNewInsertMode= bOldInsertMode ? FALSE : TRUE;
		
			SetInsertMode(bNewInsertMode);
			break;
		}
	}
}

void CXrNumericEdit::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	TRACE(_T("CXrNumericEdit::OnChar(\'%c\' (%x), %d, 0x%04x)\n"), 
		nChar, nChar, nRepCnt, nFlags);

	DWORD dwStyle= GetStyle();
	if ((dwStyle & ES_READONLY)== ES_READONLY) {
		CEdit::OnChar(nChar, nRepCnt, nFlags);
		return ;
	}

	switch(nChar) {
		case VK_0:
		case VK_1:
		case VK_2:
		case VK_3:
		case VK_4:
		case VK_5:
		case VK_6:
		case VK_7:
		case VK_8:
		case VK_9:
			CharNumeric(nChar, nRepCnt, nFlags);
			break;
		
		case VK_PLUS:
			CharPlus(nChar, nRepCnt, nFlags);
			break;

		case VK_MINUS:
			CharMinus(nChar, nRepCnt, nFlags);
			break;

		case VK_PERIOD:
			CharPointDecimal(nChar, nRepCnt, nFlags);
			break;

		case VK_BACK:
			CharBack(nChar, nRepCnt, nFlags);
			break;
	};
}

void CXrNumericEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CEdit::OnSetFocus(pOldWnd);
	
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);
	
	if((RPtoLP(nSelectionStart)==0) && (RPtoLP(nSelectionEnd)== GetWindowTextLength()))
	{
		UpdateInsertionPoint(0);
	}
}

void CXrNumericEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonDown(nFlags, point);

	int pos = this->CharFromPos(point);
	int nLogicalPos= RPtoLP(pos);

	if (nLogicalPos< 0)
		nLogicalPos= 0;

	pos= LPtoRP(nLogicalPos);
	SetSel(pos, pos);

	m_nSelectionStart= pos;
}

void CXrNumericEdit::OnLButtonUp(UINT nFlags, CPoint point) 
{
	CEdit::OnLButtonUp(nFlags, point);

	m_nSelectionStart= -1;
}

void CXrNumericEdit::OnLButtonDblClk(UINT nFlags, CPoint point) 
{
	int nCountDigits= m_listNumericData.GetCount();
	SetSel(LPtoRP(0), LPtoRP(nCountDigits));
}

void CXrNumericEdit::OnMouseMove(UINT nFlags, CPoint point) 
{
	CRect rectWindow;
	if (((nFlags & MK_LBUTTON)!=MK_LBUTTON) || (m_nSelectionStart== -1))
		return;

	GetClientRect(rectWindow);
	if (!rectWindow.PtInRect(point))
		return;

	int nPosCaret= RPtoLP(CharFromPos(point));
	if (nPosCaret< 0)
		nPosCaret= 0;
	
	nPosCaret= LPtoRP(nPosCaret);
	SetSel(m_nSelectionStart, nPosCaret);
	
	if (nPosCaret== LPtoRP(m_listNumericData.GetCount())) {
		nPosCaret= GetCaretPos().x;
	}
	else {
		nPosCaret= PosFromChar(nPosCaret).x;
		if (nPosCaret< 0) nPosCaret= 1;
	}
	
	SetCaretPos(CPoint(nPosCaret, 1));
}

void CXrNumericEdit::SetInsertMode(BOOL bInsertMode)
{
	m_bInsertMode= bInsertMode;
}

BOOL CXrNumericEdit::IsInsertMode() const
{
	return m_bInsertMode;
}

void CXrNumericEdit::SetAutoTab(BOOL bAutoTab)
{
	m_bAutoTab= bAutoTab;
}

BOOL CXrNumericEdit::IsAutoTab() const
{
	return m_bAutoTab;
}

void CXrNumericEdit::Update(int nSelectionStart)
{
	// Update the edit control if it exists. 
	if(::IsWindow(m_hWnd))
	{
		m_nSetTextSemaphor++;
		CString sText= GetStringContents();
		SetWindowText(sText);
		m_nSetTextSemaphor--;
		
		// We usually need to update the insertion point. 
		if(nSelectionStart >= 0)
			UpdateInsertionPoint(nSelectionStart);
	}
}

void CXrNumericEdit::UpdateInsertionPoint(int nSelectionStart, enumDMCPDataType enumDirection)
{
	switch(enumDirection) {
		case xrDMCPDataTypeDirectionForward: {
				int nNewInsertionPoint= nSelectionStart;//GetNextInputLocation(nSelectionStart);

				if(m_bAutoTab && RPtoLP(nNewInsertionPoint)== m_listNumericData.GetCount())
				{
					CWnd* pParentWnd= GetParent();
					ASSERT(pParentWnd);
					CWnd* pNextTabCtrl= pParentWnd->GetNextDlgTabItem(this);
					if(pNextTabCtrl && pNextTabCtrl!=this)
					{
						pNextTabCtrl->SetFocus();
					}
				}
				else
				{
					SetSel(nNewInsertionPoint, nNewInsertionPoint);
				}
			}
			break;
		
		case xrDMCPDataTypeDirectionBackward: {
				int nNewInsertionPoint= nSelectionStart;
				SetSel(nNewInsertionPoint, nNewInsertionPoint);
			}
			break;
	}
}

CString CXrNumericEdit::GetStringContents() const
{
	CString csInputData= _T("");

	CXrNumericData* pobjData=NULL;
	for(POSITION pos= m_listData.GetHeadPosition(); pos;) {
		pobjData= m_listData.GetNext(pos);
		csInputData += pobjData->m_chValue;
	}
	
	return csInputData;
}

BOOL CXrNumericEdit::IsEmpty() const
{
	return GetStringContents().IsEmpty();
}

void CXrNumericEdit::SetCountSpaceSeparator(INT nCountSpaceSeparator)
{
	m_nCountSpaceSeparator= nCountSpaceSeparator;

	FormatNumericData();
	Update();
}

INT CXrNumericEdit::GetCountSpaceSeparator() const
{
	return m_nCountSpaceSeparator;
}

void CXrNumericEdit::SetCountDigitsDecimals(INT nCountDigitsDecimals)
{
	m_nCountDigitsDecimals= nCountDigitsDecimals;

	CString strPreviusData= GetNumericInputData();
	InitializeListNumericData(IsNumericDataTypeFloat());
	SetNumericInputData(strPreviusData);
}

INT CXrNumericEdit::GetCountDigitsDecimals() const
{
	return m_nCountDigitsDecimals;
}

void CXrNumericEdit::SetCountDigitGrouping(INT nCountDigitGrouping)
{
	m_nCountDigitGrouping= nCountDigitGrouping;
	
	FormatNumericData();
	Update();
}

INT CXrNumericEdit::GetCountDigitGrouping() const
{
	return m_nCountDigitGrouping;
}

void CXrNumericEdit::SetCountDigitsAfterDecimals(INT nCountDigitsAfterDecimals)
{
	m_nCountDigitsAfterDecimals= nCountDigitsAfterDecimals;
	
	CString strPreviusData= GetNumericInputData();
	InitializeListNumericData(IsNumericDataTypeFloat());
	SetNumericInputData(strPreviusData);
}

INT CXrNumericEdit::GetCountDigitsAfterDecimals() const
{
	return m_nCountDigitsAfterDecimals;
}

void CXrNumericEdit::SetNegativeColor(COLORREF rgbNegativeColor)
{
	m_rgbNegativeColor= rgbNegativeColor;
}

COLORREF CXrNumericEdit::GetNegativeColor() const
{
	return m_rgbNegativeColor;
}

void CXrNumericEdit::SetNegativeSignSymbol(TCHAR chNegativeSignSymbol)
{
	m_chNegativeSignSymbol= chNegativeSignSymbol;

	FormatNumericData();
	Update();
}

TCHAR CXrNumericEdit::GetNegativeSignSymbol() const
{
	return m_chNegativeSignSymbol;
}

void CXrNumericEdit::SetDecimalSymbol(TCHAR chDecimalSymbol)
{
	m_chDecimalSymbol= chDecimalSymbol;

	FormatNumericData();
	Update();
}

TCHAR CXrNumericEdit::GetDecimalSymbol() const
{
	return m_chDecimalSymbol;
}

void CXrNumericEdit::SetDisplayLeadingZero(enumDLZDataType nDisplayLeadingZero)
{
	m_nDisplayLeadingZero= nDisplayLeadingZero;

	CString strPreviusData= GetNumericInputData();
	InitializeListNumericData(IsNumericDataTypeFloat());
	SetNumericInputData(strPreviusData);
}

enumDLZDataType CXrNumericEdit::GetDisplayLeadingZero() const
{
	return m_nDisplayLeadingZero;
}

void CXrNumericEdit::SetNegativeSymbolFormat(enumNSFDataType nNegativeSymbolFormat)
{
	m_nNegativeSymbolFormat= nNegativeSymbolFormat;

	FormatNumericData();
	Update();
}

enumNSFDataType CXrNumericEdit::GetNegativeSymbolFormat() const
{
	return m_nNegativeSymbolFormat;
}

void CXrNumericEdit::SetNegativeNumberFormat(enumNNFDataType nNegativeNumberFormat)
{
	m_nNegativeNumberFormat= nNegativeNumberFormat;

	FormatNumericData();
	Update();
}

enumNNFDataType CXrNumericEdit::GetNegativeNumberFormat() const
{
	return m_nNegativeNumberFormat;
}

void CXrNumericEdit::SetNegativeValue(BOOL bNegativeValue)
{
	m_bIsNegativeValue= bNegativeValue;
	if (m_bIsNegativeValue)
		SetTextColor(m_rgbNegativeColor);
	else
		SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	FormatNumericData();
	Update();
}

BOOL CXrNumericEdit::IsNegativeValue() const
{
	BOOL bIsValueZeros= TRUE;
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos; ) {
		CXrNumericData* pobjNumericData= m_listNumericData.GetNext(pos);
		if (pobjNumericData->IsInputData() && pobjNumericData->m_chValue!= '0') {
			bIsValueZeros= FALSE;
			break;
		}
	}

	return m_bIsNegativeValue && !bIsValueZeros;
}

void CXrNumericEdit::SetDisplayDigitsDecimalsFormat(enumDDDFDataType nDisplayDigitsDecimalsFormat)
{
	m_nDisplayDigitsDecimalsFormat= nDisplayDigitsDecimalsFormat;

	CString strPreviusData= GetNumericInputData();
	InitializeListNumericData(IsNumericDataTypeFloat());
	SetNumericInputData(strPreviusData);
}

enumDDDFDataType CXrNumericEdit::GetDisplayDigitsDecimalsFormat() const
{
	return m_nDisplayDigitsDecimalsFormat;
}

void CXrNumericEdit::DeleteContents()
{
	if(m_listData.GetCount()==0) {
		if(::IsWindow(GetSafeHwnd()))
			SetWindowText(_T(""));
	}

	CXrNumericData* pobjData=NULL;
	for(POSITION pos= m_listData.GetHeadPosition(); pos;) {
		pobjData= m_listData.GetNext(pos);
		delete pobjData;
		pobjData= NULL;
	}
	
	m_listData.RemoveAll();

	pobjData=NULL;
	for(pos= m_listNumericData.GetHeadPosition(); pos;) {
		pobjData= m_listNumericData.GetNext(pos);
		delete pobjData;
		pobjData= NULL;
	}
	
	m_listNumericData.RemoveAll();
}

void CXrNumericEdit::SetSeparatorGrouping(CString strSeparatorGrouping)
{
	m_strSeparatorGrouping= strSeparatorGrouping;

	FormatNumericData();
	Update();
}

CString CXrNumericEdit::GetSeparatorGrouping() const
{
	return m_strSeparatorGrouping;
}

int CXrNumericEdit::InsertAt(int nSelectionStart, TCHAR chNewChar) 
{
	CString csPreviousInput= GetNumericInputData();

	CString csInputData= _T("");
	CString csLeftInputData= csPreviousInput.Left(nSelectionStart);
	CString csRightInputData= _T("");
	if (nSelectionStart< csPreviousInput.GetLength()) {
		csRightInputData= csPreviousInput.Mid(nSelectionStart);
	}

	csInputData=  csLeftInputData;
	csInputData+= chNewChar;
	csInputData+= csRightInputData;

	// Now apply the filtered data stream and check if it was successful. 
	if(!SetNumericInputData(csInputData)) {
		
		// If not successful, then restore the previous input and return -1. 
		SetNumericInputData(csPreviousInput);
		return -1;
	}
	
	return nSelectionStart;
}

int CXrNumericEdit::SetAt(int nSelectionStart, TCHAR chNewChar)
{
	CString csPreviousInput= GetNumericInputData();
	CString csInputData= _T(csPreviousInput);
	if (nSelectionStart< 0 || nSelectionStart>= csInputData.GetLength())
		return -1;
	csInputData.SetAt(nSelectionStart, chNewChar);

	// Now apply the filtered data stream and check if it was successful. 
	if(!SetNumericInputData(csInputData)) {
		
		// If not successful, then restore the previous input and return -1. 
		SetNumericInputData(csPreviousInput);
		return -1;
	}
	
	return nSelectionStart;
}

int CXrNumericEdit::RPtoLP(int nRealPos)  
{
	int nLogicalPos= 0;
	int nCharIndex= 0;
	for(POSITION pos= m_listData.GetHeadPosition(); pos; nCharIndex++)
	{
		if (nCharIndex== nRealPos)
			return nLogicalPos;

		CXrNumericData* pobjData= m_listData.GetNext(pos);
		if (IsValidInput(pobjData->m_chValue)) {
			nLogicalPos++;
		}
	}

	if (nCharIndex== nRealPos)
		return nLogicalPos;
	
	return -1;
}

int CXrNumericEdit::LPtoRP(int nLogicalPos)  
{	
	int nRealPos= -1;
	int nCharIndex= 0;
	int nLastLogicalCharPos= 0;

	for(POSITION pos= m_listData.GetHeadPosition(); pos; nCharIndex++)
	{
		CXrNumericData* pobjData= m_listData.GetNext(pos);
		if (IsValidInput(pobjData->m_chValue)) {
			nRealPos++;
			nLastLogicalCharPos= nCharIndex+ 1;
		}

		if (nLogicalPos== nRealPos)
			return nCharIndex;
	}
	
	if (nLogicalPos>= nRealPos)
		return nLastLogicalCharPos;
	
	return -1;
}

void CXrNumericEdit::CharNumeric(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);
	TRACE(_T("  %2d, %2d  Before\n"), nSelectionStart, nSelectionEnd);

	if(nSelectionStart== nSelectionEnd) {
		int nActualInsertionPoint=  RPtoLP(nSelectionStart);
		BOOL bIsPrompt= IsPromptPosition(nActualInsertionPoint);
		
		if(m_bInsertMode && !bIsPrompt)
			nActualInsertionPoint= InsertAt(nActualInsertionPoint, (TCHAR)nChar);
		else if (IsValidInputPosition(nActualInsertionPoint))
			nActualInsertionPoint= SetAt   (nActualInsertionPoint, (TCHAR)nChar);

		if (nActualInsertionPoint>= 0)
			nSelectionStart= LPtoRP(nActualInsertionPoint + 1);

		Update(nSelectionStart);
	}
	else {
		
		int nActualInsertionPoint= RPtoLP(nSelectionStart);
		POSITION pos= m_listNumericData.FindIndex(nActualInsertionPoint);
	
		if (pos== NULL)
			return;

		CXrNumericData* pobjData=new CXrNumericData();
		pobjData->m_chValue= (TCHAR)nChar;
		m_listNumericData.InsertBefore(pos, pobjData);

		if(DeleteRange(RPtoLP(nSelectionStart)+ 1, RPtoLP(nSelectionEnd)+ 1)) {
		
			if(nActualInsertionPoint >= 0)
				nSelectionStart= LPtoRP(nActualInsertionPoint + 1);

			Update(nSelectionStart);
		}
	}
}

CString CXrNumericEdit::GetNumericInputData(BOOL bWithPromptSymbol) const
{
	CString csInputData;

	if(m_listNumericData.GetCount()==0)
	{
		GetWindowText(csInputData);
		return csInputData;
	}

	CXrNumericData* pobjData=NULL;
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos;)
	{
		pobjData= m_listNumericData.GetNext(pos);
		
		// Ignore everything that is not data. 
		if(pobjData->IsInputData() || 
			pobjData->IsInputData(xrNumericDataTypeDECIMALSEPARATOR) ||
			pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL))
		{
			if ((pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL) && bWithPromptSymbol) ||
				!pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL))
			{
				csInputData += pobjData->m_chValue;
			}
			else
				csInputData += "0";
		}
	}
	
	return csInputData;
}

void CXrNumericEdit::InitializeListNumericData(BOOL IsFractionary)
{
	DeleteContents();

	CompleteListNumericData(0, true, true);

	if (IsFractionary) {
		
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypeDECIMALSEPARATOR, chNumericPlaceholderDECIMALSEPARATOR);
		m_listNumericData.AddTail(pobjData);

		CompleteListNumericData(0, false, true);
	}

	if (m_listNumericData.IsEmpty()) {
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePROMPTSYMBOL, chNumericPlaceholderPROMPTSYMBOL);
		m_listNumericData.AddTail(pobjData);
	}

	FormatNumericData();
}

BOOL CXrNumericEdit::IsNumericDataTypeFloat()
{
	return (m_nCountDigitsAfterDecimals> 0);
}

BOOL CXrNumericEdit::SetNumericInputData(LPCTSTR pszInputData, BOOL bIsOnlyDigits, BOOL bCompleteByHeader)
{
	if (pszInputData== NULL)
		return false;

	BOOL bIsNegativeValue= m_bIsNegativeValue;
	if (!bIsOnlyDigits) {
		bIsNegativeValue= FALSE;
		if (*pszInputData== '-') {
			bIsNegativeValue= true;
			pszInputData++;
		}		
	}

	if (!IsValidInput(pszInputData))
		return false;

	DeleteContents();
	m_bIsNegativeValue= bIsNegativeValue;

	BOOL bInsertBeforeDecimal= TRUE;
	int nCountDigits= 0;

	//Eliminar todos los ceros que se encuentre al principio del numero
	for(LPCTSTR pszInsertionPoint= pszInputData; *pszInsertionPoint; pszInsertionPoint++) {
		if (*pszInsertionPoint== '0')
			pszInputData++;
		else
			break;
	}
		
	for(pszInsertionPoint= pszInputData; *pszInsertionPoint; pszInsertionPoint++, nCountDigits++) {
		TCHAR chNew= *pszInsertionPoint;

		//Verificar que no sea mayor que la cantidad de
		//digitos permitidos en la parte decimal
		if (bInsertBeforeDecimal && nCountDigits>= m_nCountDigitsDecimals && m_nCountDigitsDecimals> 0 && chNew!= m_chDecimalSymbol) {
			continue;
		}

		//Verificar que no sea mayor que la cantidad de 
		//de digitos fractionarios
		if (!bInsertBeforeDecimal && nCountDigits> m_nCountDigitsAfterDecimals && m_nCountDigitsAfterDecimals>= 0)
			continue;
		
		CXrNumericData* pobjData=new CXrNumericData();
		if(pobjData) {
			
			pobjData->m_chValue= chNew;
			if (chNew!= m_chDecimalSymbol)
				m_listNumericData.AddTail(pobjData);
			
			if (chNew== chNumericPlaceholderPROMPTSYMBOL) {
				pobjData->m_eType= xrNumericDataTypePROMPTSYMBOL;
				
				continue;
			}
			
			if (chNew== m_chDecimalSymbol) {
				CompleteListNumericData(nCountDigits, true, bCompleteByHeader);
				
				if (IsNumericDataTypeFloat()) {
					m_listNumericData.AddTail(pobjData);
					pobjData->m_eType  =xrNumericDataTypeDECIMALSEPARATOR  ;
				}
				else {
					delete pobjData;
					pobjData= NULL;
				}
				
				bInsertBeforeDecimal= FALSE;
				nCountDigits= 0;
			}
		}
	}

	if (bInsertBeforeDecimal) {
		CompleteListNumericData(nCountDigits, true, bCompleteByHeader);
		
		if (IsNumericDataTypeFloat()) {
			CXrNumericData* pobjData=new CXrNumericData(xrNumericDataTypeDECIMALSEPARATOR, m_chDecimalSymbol );
			m_listNumericData.AddTail(pobjData);
		}

		CompleteListNumericData(0, false, true);
	}
	else {
		if (nCountDigits>0) 
			nCountDigits--;
		
		CompleteListNumericData(nCountDigits, false, true);
	}

	//Eliminar todos los prompt que estan al principio de la
	//lista y deja solo el que esta antes del punto si esta
	//seteado el modo de visualizacion completo
	if (m_nDisplayDigitsDecimalsFormat== xrDDDFDataTypeOnlyInputs) {
		POSITION posCurrent= m_listNumericData.GetHeadPosition();
		POSITION posPrevius= NULL;
		while (posCurrent!= NULL) {
			posPrevius= posCurrent;
			CXrNumericData* pobjData= m_listNumericData.GetNext(posCurrent);
			if (!pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL))
				break;
			
			if (posCurrent== NULL)
				break;
				
			CXrNumericData* pobjDataNext= m_listNumericData.GetAt(posCurrent);
			BOOL bIsPromptNext= pobjDataNext->IsInputData(xrNumericDataTypePROMPTSYMBOL);
			BOOL bIsPointDecimalNext= pobjDataNext->IsInputData(xrNumericDataTypeDECIMALSEPARATOR);

			BOOL bDeleteCurrentPosition= (!bIsPointDecimalNext);
			bDeleteCurrentPosition|= ((m_nDisplayLeadingZero== xrDLZDataTypeParcial) && 
				(bIsPointDecimalNext));

			if (bDeleteCurrentPosition) {
				m_listNumericData.RemoveAt(posPrevius);
				delete pobjData;
			}
		}
	}

	//Si la lista esta vacia llamar al inicializador
	if (m_listNumericData.IsEmpty())
		InitializeListNumericData(IsNumericDataTypeFloat());

	//Si el primer numero es un cero cambiarlo a prompt
	CXrNumericData* pobjData= m_listNumericData.GetHead();
	if (pobjData!= NULL && pobjData->IsInputData() && pobjData->m_chValue == '0') {
		*pobjData= CXrNumericData(xrNumericDataTypePROMPTSYMBOL, chNumericPlaceholderPROMPTSYMBOL);
	}


	FormatNumericData();
	if (m_bIsNegativeValue)
		SetTextColor(m_rgbNegativeColor);
	else
		SetTextColor(GetSysColor(COLOR_WINDOWTEXT));

	Update();
	return true;
}

BOOL CXrNumericEdit::IsValidInput(TCHAR chNewChar)
{
	CXrNumericData objData;
	if (objData.IsValidInput(chNewChar) || 
		(chNewChar== m_chDecimalSymbol) || 
		(chNewChar== chNumericPlaceholderPROMPTSYMBOL))
	{
		return true;
	}

	return false;
}

BOOL CXrNumericEdit::IsValidInput(LPCTSTR pszInputData)
{
	if (pszInputData== NULL)
		return false;

	int nCountsDecimalSeparator= 0;
	for(LPCTSTR pszInsertionPoint= pszInputData; *pszInsertionPoint; pszInsertionPoint++) {
		TCHAR chNew=*pszInsertionPoint;
		
		if (chNew== m_chDecimalSymbol)
			nCountsDecimalSeparator++;

		if (IsValidInput(chNew))
			continue;

		return false;
	}

	return (nCountsDecimalSeparator<= 1);
}

BOOL CXrNumericEdit::IsPromptPosition(int nSelectionStart)
{
	POSITION pos= m_listNumericData.FindIndex(nSelectionStart);
	
	if (pos== NULL)
		return false;

	CXrNumericData* pobjData= m_listNumericData.GetAt(pos);

	return pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL);
}

void CXrNumericEdit::CompleteListNumericData(int nPositionStart, BOOL bBeforePointDecimal, BOOL bCompleteByHeader)
{
	if (bBeforePointDecimal) {
		//Adicionarles los ceros a los lugares decimales si
		//la cantidad es mayor que cero y el modo de visualizacion
		//es a modo completo
		if ((m_nCountDigitsDecimals>0) && (m_nDisplayDigitsDecimalsFormat== xrDDDFDataTypeFull)) {
			for(int nIndex= nPositionStart; ((nIndex< m_nCountDigitsDecimals) && 
				(m_nCountDigitsDecimals> 0)); nIndex++) 
			{
				CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePROMPTSYMBOL, chNumericPlaceholderPROMPTSYMBOL);
				if (bCompleteByHeader)
					m_listNumericData.AddHead(pobjData);
				else
					m_listNumericData.AddTail(pobjData);
			}
		}

		//adicionar un cero si el modo de visualización es a 
		//modo completo
		else if (m_nDisplayLeadingZero== xrDLZDataTypeFull && nPositionStart== 0) {
			CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePROMPTSYMBOL, chNumericPlaceholderPROMPTSYMBOL);
			if (bCompleteByHeader)
				m_listNumericData.AddHead(pobjData);
			else
				m_listNumericData.AddTail(pobjData);
		}
	}
	
	else {
		//Adicionarles los lugares decimales que tiene despues
		//del punto decimal y que no se hayan completados
		for(int nIndex= nPositionStart; ((nIndex< m_nCountDigitsAfterDecimals) && 
			(m_nCountDigitsAfterDecimals> 0)); nIndex++) 
		{
			CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePROMPTSYMBOL, chNumericPlaceholderPROMPTSYMBOL);
			m_listNumericData.AddTail(pobjData);
		}
	}
}

BOOL CXrNumericEdit::IsValidInputPosition(int nSelectionStart)
{
	POSITION pos= m_listNumericData.FindIndex(nSelectionStart);
	
	if (pos== NULL)
		return false;

	CXrNumericData* pobjData= m_listNumericData.GetAt(pos);

	return pobjData->IsInputData(xrNumericDataTypeDIGIT) || 
		pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL);
}

int CXrNumericEdit::DeleteRange(int nSelectionStart, int nSelectionEnd)
{
	int nCharIndex  =0;
	int nDeleteCount=0;
	
	CString csInputData= _T("");
	CXrNumericData* pobjData=NULL;
	BOOL bIsNegativeValue= IsNegativeValue();
	
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos; nCharIndex++)
	{
		pobjData= m_listNumericData.GetNext(pos);
		// Ignore everything that is not data. 
		// This is just like we do in GetInputData except that we 
		// will ignore the input data within the selection range. 
		if(IsValidInput(pobjData->m_chValue))
		{
			if((nCharIndex < nSelectionStart) || 
				(nCharIndex >= nSelectionEnd) || 
				pobjData->IsInputData(xrNumericDataTypeDECIMALSEPARATOR))
			{
				// The SetInputData() function will take care of validating 
				// the shifted characters. 
				csInputData += pobjData->m_chValue;
			}
			else
				nDeleteCount++;
		}
	}

	// Now apply the filtered data stream. 
	SetNumericInputData(csInputData, true, false);
	
	// return the deleted count so that an error can be generated 
	// if none were deleted. 
	return nDeleteCount;
}

void CXrNumericEdit::CharPointDecimal(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);

	//Buscar el indice que corresponde a el punto decimal
	CXrNumericData* pobjData=NULL;
	int nCharIndex= 0;
	
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos; nCharIndex++)
	{
		pobjData= m_listNumericData.GetNext(pos);
		if (pobjData->IsInputData(xrNumericDataTypeDECIMALSEPARATOR))
			break;
	}

	if (nCharIndex< m_listNumericData.GetCount()) {
		UpdateInsertionPoint(LPtoRP(nCharIndex+ 1));
	}
}

void CXrNumericEdit::CharPlus(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);
	m_bIsNegativeValue= FALSE;
	
	nSelectionStart= RPtoLP(nSelectionStart);
	FormatNumericData();

	if (m_bIsNegativeValue)
		SetTextColor(m_rgbNegativeColor);
	else
		SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	Update(LPtoRP(nSelectionStart));
}

void CXrNumericEdit::CharMinus(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);

	m_bIsNegativeValue= m_bIsNegativeValue ? FALSE : TRUE;
	nSelectionStart= RPtoLP(nSelectionStart);
	
	FormatNumericData();
	if (m_bIsNegativeValue)
		SetTextColor(m_rgbNegativeColor);
	else
		SetTextColor(GetSysColor(COLOR_WINDOWTEXT));
	Update(LPtoRP(nSelectionStart));
}

void CXrNumericEdit::CharBack(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);
	
	nSelectionStart= RPtoLP(nSelectionStart);
	nSelectionEnd= RPtoLP(nSelectionEnd);

	if (nSelectionStart== nSelectionEnd) {
		/*if (*/DeleteRange(nSelectionStart- 1, nSelectionStart);/*)*/
			Update(LPtoRP(nSelectionStart- 1));

	}
	else {
		if (DeleteRange(nSelectionStart, nSelectionEnd))
			Update(LPtoRP(nSelectionStart));
	}
}

void CXrNumericEdit::SetPostfix(LPCSTR lpszPostfix)
{
	m_strPostfix= lpszPostfix;

	FormatNumericData();
	Update();
}

CString CXrNumericEdit::GetPostfix() const
{
	return m_strPostfix;
}

void CXrNumericEdit::SetPrefix(LPCSTR lpszPrefix)
{
	m_strPrefix= lpszPrefix;

	FormatNumericData();
	Update();
}

CString CXrNumericEdit::GetPrefix() const
{
	return m_strPrefix;
}

void CXrNumericEdit::FormatNumericData()
{
	//Borrar todos los elementos de la lista
	CXrNumericData* pobjData=NULL;
	for(POSITION pos= m_listData.GetHeadPosition(); pos;) {
		pobjData= m_listData.GetNext(pos);
		delete pobjData;
		pobjData= NULL;
	}
	
	m_listData.RemoveAll();

	if (!IsNegativeValue()) {
		
		FormatNumericDataPositive();
		return;
	}

	switch(m_nNegativeNumberFormat) {
	case xrNNFDataTypeRSNSO:
		FormatNumericDataRSNSO();
		break;
	
	case xrNNFDataTypeSRNSO:
		FormatNumericDataSRNSO();
		break;
	
	case xrNNFDataTypeSRNOS:
		FormatNumericDataSRNOS();
		break;
	
	case xrNNFDataTypeRSNOS:
		FormatNumericDataRSNOS();
		break;
	
	case xrNNFDataTypeRSNO:
		FormatNumericDataRSNO();
		break;
	
	case xrNNFDataTypeSRNO:
		FormatNumericDataSRNO();
		break;
	
	case xrNNFDataTypeRNSO:
		FormatNumericDataRNSO();
		break;
	
	case xrNNFDataTypeRNOS:
		FormatNumericDataRNOS();
		break;
	}
}

void CXrNumericEdit::InsertFormatPrefix()
{
	//Adicionar el prefixo
	for(int nIndex= 0; nIndex< m_strPrefix.GetLength(); nIndex++) {
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePREFIX, m_strPrefix.GetAt(nIndex));
		m_listData.AddTail(pobjData);
	}
}

void CXrNumericEdit::InsertFormatPostfix()
{
	//Adicionar el postfix
	for(int nIndex= 0; nIndex< m_strPostfix.GetLength(); nIndex++) {
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypePOSTFIX, m_strPostfix.GetAt(nIndex));
		m_listData.AddTail(pobjData);
	}
}

void CXrNumericEdit::InsertFormatSeparatorGroup()
{
	//Adicionar el separador de grupo decimales (miles)
	for(int nIndex= 0; nIndex< m_strSeparatorGrouping.GetLength(); nIndex++) {
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypeTHOUSANDSSEPARATOR, m_strSeparatorGrouping.GetAt(nIndex));
		m_listData.AddTail(pobjData);
	}
}

void CXrNumericEdit::InsertFormatSpaceSeparatorGroup()
{
	//Adicionar el espacio entre grupo
	for(int nIndex= 0; nIndex< m_nCountSpaceSeparator; nIndex++) {
		CXrNumericData* pobjData= new CXrNumericData(xrNumericDataTypeGROUPSEPARATOR, ' ');
		m_listData.AddTail(pobjData);
	}
}

void CXrNumericEdit::InsertFormatNumericData()
{
	int nCountDigitsGrouping= 0;
	int nCharIndex= 0;
	int nCountDigitsDecimalReal= GetCountDigitsDecimalsReal();
	if (m_nCountDigitGrouping!= 0 && ((nCountDigitsDecimalReal % m_nCountDigitGrouping)!= 0)) {
		nCountDigitsGrouping= m_nCountDigitGrouping- (nCountDigitsDecimalReal % m_nCountDigitGrouping);
	}
	
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos;)
	{
		nCountDigitsGrouping++; 
		nCharIndex++;

		CXrNumericData* pobjNumericData= m_listNumericData.GetNext(pos);
		CXrNumericData* pobjData= new CXrNumericData(pobjNumericData->m_eType, 
			pobjNumericData->m_chValue);
		
		if (pobjData->IsInputData(xrNumericDataTypePROMPTSYMBOL))
			pobjData->m_chValue= '0';
		else if (pobjData->IsInputData(xrNumericDataTypeDECIMALSEPARATOR))
			pobjData->m_chValue= m_chDecimalSymbol;

		m_listData.AddTail(pobjData);

		//Insertar los separadores de grupo decimales
		if (m_nCountDigitGrouping!= 0 && nCountDigitsGrouping== m_nCountDigitGrouping && nCharIndex< nCountDigitsDecimalReal) {
			nCountDigitsGrouping= 0;
			InsertFormatSeparatorGroup();
		}
	}
}

void CXrNumericEdit::InsertFormatSymbolNegative(BOOL bIsPrefixSymbol)
{
	char chNegativeSymbol= _T('-');
	
	switch(m_nNegativeSymbolFormat) {
		case xrNSFDataTypeParentheses:
			chNegativeSymbol= _T(')');
			if (bIsPrefixSymbol)
				chNegativeSymbol= _T('(');
			break;	
		case xrNSFDataTypeBraces:
			chNegativeSymbol= _T('}');
			if (bIsPrefixSymbol)
				chNegativeSymbol= _T('{');
			break;
		case xrNSFDataTypeBrackets:
			chNegativeSymbol= _T(']');
			if (bIsPrefixSymbol)
				chNegativeSymbol= _T('[');
			break;
	}

	switch(m_nNegativeNumberFormat) {
		case xrNNFDataTypeRSNSO:
		case xrNNFDataTypeSRNSO:
		case xrNNFDataTypeSRNOS:
		case xrNNFDataTypeRSNOS:
			m_listData.AddTail(new CXrNumericData(xrNumericDataTypeNEGATIVESYMBOL, chNegativeSymbol));
			break;
		default:
			m_listData.AddTail(new CXrNumericData(xrNumericDataTypeNEGATIVESYMBOL, m_chNegativeSignSymbol));
	}
}

INT CXrNumericEdit::GetCountDigitsDecimalsReal()
{
	INT nCountDigitsDecimals= 0;
	for(POSITION pos= m_listNumericData.GetHeadPosition(); pos; )
	{
		CXrNumericData* pobjNumericData= m_listNumericData.GetNext(pos);
		if (pobjNumericData->IsInputData(xrNumericDataTypeDECIMALSEPARATOR))
			break;
		
		nCountDigitsDecimals++;
	}

	return nCountDigitsDecimals;
}

void CXrNumericEdit::FormatNumericDataPositive()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}

void CXrNumericEdit::FormatNumericDataRSNSO()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatSymbolNegative(true);
	InsertFormatNumericData();
	InsertFormatSymbolNegative(false);

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}

void CXrNumericEdit::FormatNumericDataSRNSO()
{
	InsertFormatSymbolNegative(true);
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();
	InsertFormatSymbolNegative(false);

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}

void CXrNumericEdit::FormatNumericDataSRNOS()
{
	InsertFormatSymbolNegative(true);
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
	
	InsertFormatSymbolNegative(false);
}

void CXrNumericEdit::FormatNumericDataRSNOS()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}

	InsertFormatSymbolNegative(true);
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
	InsertFormatSymbolNegative(false);
}

void CXrNumericEdit::FormatNumericDataRSNO()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}

	InsertFormatSymbolNegative(true);
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}

void CXrNumericEdit::FormatNumericDataSRNO()
{
	InsertFormatSymbolNegative(true);
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}

void CXrNumericEdit::FormatNumericDataRNOS()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
	InsertFormatSymbolNegative(true);
}

void CXrNumericEdit::FormatNumericDataRNSO()
{
	if (!m_strPrefix.IsEmpty()) {
		InsertFormatPrefix();
		InsertFormatSpaceSeparatorGroup();
	}
	
	InsertFormatNumericData();
	InsertFormatSymbolNegative(true);

	if (!m_strPostfix.IsEmpty()) {
		InsertFormatSpaceSeparatorGroup();
		InsertFormatPostfix();
	}
}


void CXrNumericEdit::SetClipboardData(CString strSource)
{
	if(OpenClipboard()) {
		HGLOBAL clipbuffer;
		char * buffer;
		EmptyClipboard();
		clipbuffer = GlobalAlloc(GMEM_DDESHARE, strSource.GetLength()+ 1);
		buffer = (char*)GlobalLock(clipbuffer);
		strcpy(buffer, LPCSTR(strSource));
		GlobalUnlock(clipbuffer);
		::SetClipboardData(CF_TEXT,clipbuffer);
		CloseClipboard();
	}
}

CString CXrNumericEdit::GetClipboardData()
{
	char * buffer = NULL;
	
	//open the clipboard
	CString fromClipboard= _T("");
	if ( OpenClipboard() ) 
	{
		if (::IsClipboardFormatAvailable(CF_TEXT)) {
			HANDLE hData = ::GetClipboardData( CF_TEXT );
			char * buffer = (char*)GlobalLock( hData );
			fromClipboard = buffer;
			GlobalUnlock( hData );
		}
		
		CloseClipboard();
	}

	return fromClipboard;
}

void CXrNumericEdit::KeyDownLeft(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)< 0;
	
	int nSelectionStart=0;
	int nSelectionEnd  =0;
	GetSel(nSelectionStart, nSelectionEnd);
	nSelectionEnd= RPtoLP(nSelectionEnd);
	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;

	if(!bIsShiftKeyDown) {
		
		UpdateInsertionPoint(LPtoRP(nSelectionStart- 1), xrDMCPDataTypeDirectionBackward);
	}
	else {
		int nCaretPos= CharFromPos(GetCaretPos());
		nCaretPos= RPtoLP(nCaretPos);
		CPoint pointCaret;
		if (nCaretPos== nSelectionStart) {
			nSelectionStart= nSelectionStart- 1;
			if (nSelectionStart< 0)
				nSelectionStart= 0;
			pointCaret= PosFromChar(LPtoRP(nSelectionStart));
		}
		else {
			nSelectionEnd= nSelectionEnd- 1;
			pointCaret= PosFromChar(LPtoRP(nSelectionEnd));
		}
		
		if (pointCaret.x<= 0) pointCaret.x= 1;
		if (pointCaret.y<= 0) pointCaret.y= 1;
		
		SetSel(LPtoRP(nSelectionStart), LPtoRP(nSelectionEnd));
		SetCaretPos(pointCaret);
	}
}

void CXrNumericEdit::KeyDownRight(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)< 0;

	int nSelectionStart= 0;
	int nSelectionEnd  = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	nSelectionEnd= RPtoLP(nSelectionEnd);
	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;

	if(!bIsShiftKeyDown) {
		
		UpdateInsertionPoint(LPtoRP(nSelectionStart+ 1));
	}
	else {
		int nCaretPos= CharFromPos(GetCaretPos());
		nCaretPos= RPtoLP(nCaretPos);
		CPoint pointCaret;
		if (nCaretPos== nSelectionStart) {
			nSelectionStart= LPtoRP(nSelectionStart+ 1);
			nSelectionEnd= LPtoRP(nSelectionEnd);
			pointCaret= PosFromChar(nSelectionStart);
		}
		else {
			nSelectionEnd= LPtoRP(nSelectionEnd+ 1);
			nSelectionStart= LPtoRP(nSelectionStart);
			pointCaret= PosFromChar(nSelectionEnd);
		}
		
		SetSel(nSelectionStart, nSelectionEnd);
		
		if (pointCaret.x<= 0) pointCaret.x= GetCaretPos().x;
		if (pointCaret.y<= 0) pointCaret.y= GetCaretPos().y;
		SetCaretPos(pointCaret);
	}
}

void CXrNumericEdit::KeyDownHome(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)< 0;

	int nSelectionStart= 0;
	int nSelectionEnd  = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	nSelectionEnd= RPtoLP(nSelectionEnd);
	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;

	if(!bIsShiftKeyDown) {
		
		UpdateInsertionPoint(LPtoRP(0));
	}
	else {
		int nCaretPos= CharFromPos(GetCaretPos());
		nCaretPos= RPtoLP(nCaretPos);
		CPoint pointCaret;
		if (nCaretPos== nSelectionStart) {
			nSelectionStart= LPtoRP(0);
			nSelectionEnd= LPtoRP(nSelectionEnd);
			pointCaret= PosFromChar(nSelectionStart);
		}
		else {
			nSelectionEnd= LPtoRP(nSelectionStart);
			nSelectionStart= LPtoRP(0);
			pointCaret= PosFromChar(nSelectionStart);
		}
		
		if (pointCaret.x<= 0) pointCaret.x= 1;
		if (pointCaret.y<= 0) pointCaret.y= 1;
		
		SetSel(nSelectionStart, nSelectionEnd);
		SetCaretPos(pointCaret);
	}
}

void CXrNumericEdit::KeyDownEnd(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	BOOL bIsShiftKeyDown=::GetAsyncKeyState(VK_SHIFT)< 0;

	int nSelectionStart= 0;
	int nSelectionEnd  = 0;
	GetSel(nSelectionStart, nSelectionEnd);
	nSelectionEnd= RPtoLP(nSelectionEnd);
	nSelectionStart= RPtoLP(nSelectionStart);
	if (nSelectionStart< 0)
		nSelectionStart= 0;

	if(!bIsShiftKeyDown) {
		
		UpdateInsertionPoint(LPtoRP(m_listNumericData.GetCount()));
	}
	else {
		int nCaretPos= CharFromPos(GetCaretPos());
		nCaretPos= RPtoLP(nCaretPos);
		CPoint pointCaret;
		if (nCaretPos== nSelectionStart) {
			nSelectionStart= LPtoRP(nSelectionEnd);
			nSelectionEnd= LPtoRP(m_listNumericData.GetCount());
			pointCaret= PosFromChar(nSelectionEnd);
		}
		else {
			nSelectionEnd= LPtoRP(m_listNumericData.GetCount());
			nSelectionStart= LPtoRP(nSelectionStart);
			pointCaret= PosFromChar(nSelectionEnd);
		}
		
		SetSel(nSelectionStart, nSelectionEnd);
		
		if (pointCaret.x<= 0) pointCaret.x= GetCaretPos().x;
		if (pointCaret.y<= 0) pointCaret.y= GetCaretPos().y;
		SetCaretPos(pointCaret);
	}
}

COLORREF CXrNumericEdit::SetTextColor(const COLORREF textColor)
{
	COLORREF prevColor = m_rgbCurrentTextColor;	
	m_rgbCurrentTextColor = textColor;				

	this->Invalidate(TRUE);						

	return prevColor;							
}

COLORREF CXrNumericEdit::GetTextColor() const
{
	return (m_rgbCurrentTextColor);
}

void CXrNumericEdit::GetTextColorRGB(BYTE &Red, BYTE &Green, BYTE &Blue) const
{
	Red = GetRValue(m_rgbCurrentTextColor);
	Green = GetGValue(m_rgbCurrentTextColor);
	Blue = GetBValue(m_rgbCurrentTextColor);
}

HBRUSH CXrNumericEdit::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	HBRUSH hBrush= (HBRUSH)CEdit::Default();
	pDC->SetTextColor(m_rgbCurrentTextColor);
	pDC->SetBkMode(TRANSPARENT);

	return hBrush;
}
