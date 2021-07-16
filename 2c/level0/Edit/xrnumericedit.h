#if !defined(AFX_XRNUMERICEDIT_H__580FBE62_8B54_4D83_A1E8_6B9E6505837B__INCLUDED_)
#define AFX_XRNUMERICEDIT_H__580FBE62_8B54_4D83_A1E8_6B9E6505837B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// xrnumericedit.h : header file
//

#ifndef __AFXTEMPL_H__
#include <afxtempl.h>
#define __AFXTEMPL_H__
#endif

#define VK_0		0x30
#define VK_1		0x31
#define VK_2		0x32
#define VK_3		0x33
#define VK_4		0x34
#define VK_5		0x35
#define VK_6		0x36
#define VK_7		0x37
#define VK_8		0x38
#define VK_9		0x39
#define VK_PERIOD	0x2E
#define VK_MINUS	0x2D
#define VK_PLUS		0x2B

enum enumNNFDataType //Negative Number Format Data Type
{
	xrNNFDataTypeRSNSO= 0,  // r(1.1)o --> Prefix+ Sign symbol+ Number+ Sign symbol+ Postfix
	xrNNFDataTypeSRNSO,		// (r1.1)o --> Sign symbol+ Prefix+ Number+ Sign symbol+ Postfix
	xrNNFDataTypeSRNOS,		// (r1.1o) --> Sign symbol+ Prefix+ Number+ Postfix+ Sign symbol
	xrNNFDataTypeRSNOS,		// r(1.1o) --> Prefix+ Sign symbol+ Number+ Postfix+ Sign symbol
	xrNNFDataTypeRSNO,		// r-1.1o  --> Prefix+ Sign symbol+ Number+ Postfix
	xrNNFDataTypeSRNO,		// -r1.1o   --> Sign symbol+ Prefix+ Number+ Postfix
	xrNNFDataTypeRNSO,		// r1.1-o  --> Prefix+ Number+ Sign symbol+ Postfix
	xrNNFDataTypeRNOS		// r1.1o-   --> Prefix+ Number+ Postfix+ Sign symbol
};

enum enumNSFDataType // Negative Symbol Format Data Type
{
	xrNSFDataTypeParentheses= 10,			// () --> show parentheses with sign symbol
	xrNSFDataTypeBraces,					// {} --> show braces with sign symbol 
	xrNSFDataTypeBrackets					// [] --> show brackets with sign symbol 
};

enum enumDLZDataType // Display leading Zero Data Type
{
	xrDLZDataTypeParcial= 20,	// .7 not show the zero
	xrDLZDataTypeFull			// 0.7 show the zero
};

enum enumDMCPDataType	//Direction of movement of caret position
{
	xrDMCPDataTypeActualPosition= 30,
	xrDMCPDataTypeDirectionForward,
	xrDMCPDataTypeDirectionBackward
};

enum enumDDDFDataType  //Display digits decimals format 
{
	xrDDDFDataTypeOnlyInputs= 33,		//Show only input
	xrDDDFDataTypeFull					//Display all digits 
};

enum enumNumericDataType
{
	xrNumericDataTypeDECIMALSEPARATOR=0,  // Decimal separator. 
	xrNumericDataTypeTHOUSANDSSEPARATOR,  // Thousands separator. 
	xrNumericDataTypePROMPTSYMBOL	   ,  // Prompy symbol	
	xrNumericDataTypeDIGIT             ,  // # digit placeholder.
	xrNumericDataTypePREFIX			   ,  // Prefix digit placeholder
	xrNumericDataTypePOSTFIX		   ,  // Postfix digit placeholder
	xrNumericDataTypeGROUPSEPARATOR	   ,  // Group separator
	xrNumericDataTypeNEGATIVESYMBOL	   ,  // negative symbol

	NUMERICDATATYPECOUNT
} ;

const TCHAR chNULL   = _T('\0') ;
const TCHAR chCR     = _T('\r') ;
const TCHAR chLF     = _T('\n') ;
const TCHAR chSPACE  = _T(' ' ) ;
const TCHAR chPERIOD = _T('.' ) ;
const TCHAR chCOMMA  = _T(',' ) ;
const TCHAR chMINUS	 = _T('-' );

const TCHAR chNumericPlaceholderDECIMALSEPARATOR   = _T('.')  ;
const TCHAR chNumericPlaceholderTHOUSANDSSEPARATOR = _T(',')  ;
//const TCHAR chNumericPlaceholderDIGIT              = _T('#')  ;
const TCHAR chNumericPlaceholderPROMPTSYMBOL       = _T('#')  ;


class CXrNumericData : public CObject
{
	DECLARE_DYNCREATE(CXrNumericData)
	
public:
	CXrNumericData() ;
	CXrNumericData(enumNumericDataType eType, TCHAR m_chValue) ;

// Attributes
public:
	enumNumericDataType m_eType   ;
	TCHAR				m_chValue ;
	
// Operations
public:
	void  operator=(const CXrNumericData& src) ;
	BOOL  IsInputData   (enumNumericDataType eType= xrNumericDataTypeDIGIT) ;
	BOOL  IsValidInput  (TCHAR chNewChar) ;
	
	// Diagnostic Support
#ifdef _DEBUG
public:
	virtual void AssertValid() const ;
	virtual void Dump(CDumpContext& dc) const ;
#endif
};

/////////////////////////////////////////////////////////////////////////////
// CXrNumericEdit window

class CXrNumericEdit : public CEdit
{
// Construction
public:
	CXrNumericEdit();
	virtual ~CXrNumericEdit();

	BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) ;
// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXrNumericEdit)
	protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
public:
	
	void GetTextColorRGB(BYTE& Red, BYTE& Green, BYTE& Blue) const;
	COLORREF GetTextColor() const;
	COLORREF SetTextColor(const COLORREF textColor);

	INT GetCountDigitsDecimalsReal();
	
	CString GetPrefix() const;
	void SetPrefix(LPCSTR lpszPrefix);

	CString GetPostfix() const;
	void SetPostfix(LPCSTR lpszPostfix);

	CString GetSeparatorGrouping() const;
	void SetSeparatorGrouping(CString strSeparatorGrouping);
	
	enumDDDFDataType GetDisplayDigitsDecimalsFormat() const;
	void SetDisplayDigitsDecimalsFormat(enumDDDFDataType nDisplayDigitsDecimalsFormat);
	
	BOOL IsNegativeValue() const;
	void SetNegativeValue(BOOL bNegativeValue);
	
	enumNNFDataType GetNegativeNumberFormat() const;
	void SetNegativeNumberFormat( enumNNFDataType nNegativeNumberFormat);
	
	enumNSFDataType GetNegativeSymbolFormat() const;
	void SetNegativeSymbolFormat( enumNSFDataType nNegativeSymbolFormat );
	
	enumDLZDataType GetDisplayLeadingZero() const;
	void SetDisplayLeadingZero(enumDLZDataType nDisplayLeadingZero);
	
	TCHAR GetDecimalSymbol() const;
	void SetDecimalSymbol(TCHAR chDecimalSymbol);
	
	TCHAR GetNegativeSignSymbol() const;
	void SetNegativeSignSymbol(TCHAR chNegativeSignSymbol);
	
	COLORREF GetNegativeColor() const;
	void SetNegativeColor(COLORREF rgbNegativeColor);
	
	INT GetCountDigitsAfterDecimals() const;
	void SetCountDigitsAfterDecimals(INT nCountDigitsAfterDecimals);
	
	INT GetCountDigitGrouping() const;
	void SetCountDigitGrouping(INT nCountDigitGrouping);
	
	INT GetCountDigitsDecimals() const;
	void SetCountDigitsDecimals(INT nCountDigitsDecimals);
	
	INT GetCountSpaceSeparator() const;
	void SetCountSpaceSeparator(INT nCountSpaceSeparator);
	
	CString GetStringContents() const;
	BOOL IsEmpty() const;

	BOOL IsAutoTab() const;
	void SetAutoTab(BOOL bAutoTab);
	
	BOOL IsInsertMode() const;
	void SetInsertMode(BOOL bInsertMode);

	int  RPtoLP(int nRealPos) ;
	int  LPtoRP(int nLogicalPos) ;
	
	// Generated message map functions
protected:
	//{{AFX_MSG(CXrNumericEdit)
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg LONG OnCut(UINT wParam, LONG lParam) ;
	afx_msg LONG OnCopy(UINT wParam, LONG lParam) ;
	afx_msg LONG OnPaste(UINT wParam, LONG lParam) ;
	afx_msg LONG OnClear(UINT wParam, LONG lParam) ;
	afx_msg LONG OnSetText(UINT wParam, LONG lParam) ;
	afx_msg LONG OnGetText(UINT wParam, LONG lParam) ;
	afx_msg LONG OnGetTextLength(UINT wParam, LONG lParam) ;
	afx_msg LONG OnSetSel(UINT wParam, LONG lParam) ;

	DECLARE_MESSAGE_MAP()

protected:
	void Update					(int nSelectionStart= 0);
	void UpdateInsertionPoint	(int nSelectionStart, enumDMCPDataType enumDirection= xrDMCPDataTypeDirectionForward);

	void DeleteContents			();
	int  DeleteRange            (int nSelectionStart, int nSelectionEnd);

	int  InsertAt				(int nSelectionStart, TCHAR chNewChar  ) ;
	int  SetAt					(int nSelectionStart, TCHAR chNewChar  ) ;

	BOOL SetNumericInputData	(LPCTSTR pszInputData, BOOL bIsOnlyDigits= TRUE, BOOL bCompleteByHeader= true);
	CString GetNumericInputData	(BOOL bWithPromptSymbol= true) const;
	
	BOOL IsNumericDataTypeFloat();
	
	void InitializeListNumericData(BOOL IsFractionary);
	
	void CharNumeric			(UINT nChar, UINT nRepCnt, UINT nFlags);
	void CharMinus				(UINT nChar, UINT nRepCnt, UINT nFlags);
	void CharPlus				(UINT nChar, UINT nRepCnt, UINT nFlags);
	void CharPointDecimal		(UINT nChar, UINT nRepCnt, UINT nFlags);
	void CharBack				(UINT nChar, UINT nRepCnt, UINT nFlags);
	void CompleteListNumericData(int nPositionStart, BOOL bBeforePointDecimal, BOOL bCompleteByHeader);

	void KeyDownEnd				(UINT nChar, UINT nRepCnt, UINT nFlags);
	void KeyDownHome			(UINT nChar, UINT nRepCnt, UINT nFlags);
	void KeyDownRight			(UINT nChar, UINT nRepCnt, UINT nFlags);
	void KeyDownLeft			(UINT nChar, UINT nRepCnt, UINT nFlags);
	
	BOOL IsValidInputPosition	(int nSelectionStart);
	BOOL IsPromptPosition		(int nSelectionStart);
	BOOL IsValidInput			(LPCTSTR pszInputData);
	BOOL IsValidInput			(TCHAR chNewChar);

	void InsertFormatNumericData();
	void InsertFormatSpaceSeparatorGroup();
	void InsertFormatSeparatorGroup();
	void InsertFormatPostfix();
	void InsertFormatPrefix();
	void InsertFormatSymbolNegative(BOOL bIsPrefixSymbol);
	
	void FormatNumericDataRNSO();
	void FormatNumericDataRNOS();
	void FormatNumericDataSRNO();
	void FormatNumericDataRSNO();
	void FormatNumericDataRSNOS();
	void FormatNumericDataSRNOS();
	void FormatNumericDataSRNSO();
	void FormatNumericDataRSNSO();
	void FormatNumericDataPositive();
	void FormatNumericData();

	void SetClipboardData(CString strSource);
	CString GetClipboardData();

protected:
	int m_nSetTextSemaphor;
	int m_nSelectionStart;
	
	CTypedPtrList<CObList, CXrNumericData*> m_listData;
	CTypedPtrList<CObList, CXrNumericData*> m_listNumericData;

	CString				m_strPrefix;
	CString				m_strPostfix;
	CString				m_strSeparatorGrouping;

	BOOL				m_bAutoTab;
	BOOL				m_bInsertMode;
	BOOL				m_bIsNegativeValue;

	INT					m_nCountSpaceSeparator;
	INT					m_nCountDigitsDecimals;
	INT					m_nCountDigitGrouping;
	INT					m_nCountDigitsAfterDecimals;
	COLORREF			m_rgbNegativeColor;
	COLORREF			m_rgbCurrentTextColor;
	TCHAR				m_chNegativeSignSymbol;
	TCHAR				m_chDecimalSymbol;
	
	enumDLZDataType		m_nDisplayLeadingZero;
	enumNSFDataType		m_nNegativeSymbolFormat;
	enumNNFDataType		m_nNegativeNumberFormat;
	enumDDDFDataType	m_nDisplayDigitsDecimalsFormat;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_XRNUMERICEDIT_H__580FBE62_8B54_4D83_A1E8_6B9E6505837B__INCLUDED_)
