// amsEdit.cpp : implementation file for CEdit-derived classes
// Created by: Alvaro Mendez - 07/17/2000
//

#include "stdafx.h"
#include "amsEdit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#pragma warning (disable:4355)  // disables: 'this': used in base member initializer list

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit

// Constructs the object with the default attributes
CAMSEdit::CAMSEdit() :
	m_rgbText(0),
	m_uInternalFlags(None)
{
}

// Destroys the object (virtual).
CAMSEdit::~CAMSEdit()
{
}

BEGIN_MESSAGE_MAP(CAMSEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSEdit)
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's text.
CString CAMSEdit::GetText() const
{
	CString strText;
	GetWindowText(strText);
	return strText;
}

// Returns the control's text without leading or trailing blanks.
CString CAMSEdit::GetTrimmedText() const
{
    CString strText = GetText();
    strText.TrimLeft();
    strText.TrimRight();
    return strText;
}

// Sets the control's text to the given string value.
void CAMSEdit::SetText(const CString& strText)
{
	SetWindowText(strText);
}

// Sets the background color to the given rgb.
void CAMSEdit::SetBackgroundColor(COLORREF rgb)
{
	m_brushBackground.DeleteObject();
	m_brushBackground.CreateSolidBrush(rgb);
	Invalidate();
}

// Returns the RGB for the background color.
COLORREF CAMSEdit::GetBackgroundColor() const
{
	CAMSEdit* pThis = const_cast<CAMSEdit*>(this);

	if (!m_brushBackground.GetSafeHandle())
	{
		COLORREF rgb = pThis->GetDC()->GetBkColor();
		pThis->m_brushBackground.CreateSolidBrush(rgb);
		return rgb;
	}

	LOGBRUSH lb;
	pThis->m_brushBackground.GetLogBrush(&lb);
	return lb.lbColor;
}

// Sets the text color to the given rgb.
void CAMSEdit::SetTextColor(COLORREF rgb)
{
	m_rgbText = rgb;
	m_uInternalFlags |= TextColorHasBeenSet;
	Invalidate();
}

// Returns the RGB for the text color.
COLORREF CAMSEdit::GetTextColor() const
{
	if (!(m_uInternalFlags & TextColorHasBeenSet))
	{
		CAMSEdit* pThis = const_cast<CAMSEdit*>(this);
		pThis->m_rgbText = pThis->GetDC()->GetTextColor();
		pThis->m_uInternalFlags |= TextColorHasBeenSet;
	}
	return m_rgbText;
}

// Returns true if the control is read only
bool CAMSEdit::IsReadOnly() const
{
	return !!(GetStyle() & ES_READONLY);
}

// Returns the control's value in a valid format.
CString CAMSEdit::GetValidText() const
{
	return GetText();
}

// Redraws the window's text.
void CAMSEdit::Redraw()
{
	if (!::IsWindow(m_hWnd))
		return;

	CString strText = GetValidText();
	if (strText != GetText())
		SetWindowText(strText);
}

// Returns true if the given character should be entered into the control.
bool CAMSEdit::ShouldEnter(TCHAR c) const
{
	return true;
}

// Cuts the current selection into the clipboard.
LONG CAMSEdit::OnCut(UINT, LONG)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nStart < nEnd)
	{
		SendMessage(WM_COPY);				// copy the selection and...
		SendMessage(WM_KEYDOWN, VK_DELETE); // delete it
	}
	
	return 0;
}
	
// Clears the current selection.
LONG CAMSEdit::OnClear(UINT wParam, LONG lParam)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	if (nStart < nEnd)
		SendMessage(WM_KEYDOWN, VK_DELETE); // delete the selection
	
	return 0;
}

// Pastes the text from the clipboard onto the current selection.
LONG CAMSEdit::OnPaste(UINT, LONG)
{
	int nStart, nEnd;
	GetSel(nStart, nEnd);

	CEdit::Default();
	CString strText = GetValidText();

	if (strText != GetText())
	{
		SetWindowText(strText);
		SetSel(nStart, nEnd);
	}

	return 0;
}

// Handles drawing the text and background using the designated colors
BOOL CAMSEdit::OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult) 
{
	if ((message == WM_CTLCOLOREDIT || message == WM_CTLCOLORSTATIC) && (m_brushBackground.GetSafeHandle() || m_uInternalFlags & TextColorHasBeenSet))
	{
		CDC* pDC = CDC::FromHandle((HDC)wParam);

		if (m_rgbText)
			pDC->SetTextColor(m_rgbText);

		// Set the text background to the requested background color
		pDC->SetBkColor(GetBackgroundColor());

		*pLResult = (LRESULT)m_brushBackground.GetSafeHandle();
		return TRUE;
	}

	return CEdit::OnChildNotify(message, wParam, lParam, pLResult);
}

// Handles the WM_SETTEXT message to ensure that text (set via SetWindowText) is valid.
LONG CAMSEdit::OnSetText(UINT wParam, LONG lParam)
{
	LONG nResult = CEdit::Default();

	CString strText = GetValidText();
	if (strText != (LPCTSTR)lParam)
		SetWindowText(strText);

	return nResult;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::SelectionSaver

// Constructs the selection saver object for the given edit control.
// It then saves the edit control's current selection.
CAMSEdit::SelectionSaver::SelectionSaver(CEdit* pEdit) :
	m_pEdit(pEdit)
{
	ASSERT(pEdit);
	pEdit->GetSel(m_nStart, m_nEnd);
}

// Constructs the selection saver object for the given edit control.
// It then saves the given nStart and nEnd values.
CAMSEdit::SelectionSaver::SelectionSaver(CEdit* pEdit, int nStart, int nEnd) :
	m_pEdit(pEdit),
	m_nStart(nStart),
	m_nEnd(nEnd)
{
	ASSERT(pEdit);
	ASSERT(nStart <= nEnd);
}

// Destroys the object and restores the selection to the saved start and end values.
CAMSEdit::SelectionSaver::~SelectionSaver()
{
	if (m_pEdit)
		m_pEdit->SetSel(m_nStart, m_nEnd, TRUE);
}

// Changes the start and end values to nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveTo(int nStart, int nEnd)
{
	ASSERT(nStart <= nEnd);

	m_nStart = nStart;
	m_nEnd = nEnd;
}

// Changes the start and end values by nStart and nEnd respectively.
void CAMSEdit::SelectionSaver::MoveBy(int nStart, int nEnd)
{
	m_nStart += nStart;
	m_nEnd += nEnd;

	ASSERT(m_nStart <= m_nEnd);
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::MoveBy(int nPos)
{
	m_nStart += nPos;
	m_nEnd += nPos;
}

// Changes both the start and end values by nPos.
void CAMSEdit::SelectionSaver::operator+=(int nPos)
{
	MoveBy(nPos);
}

// Returns the value for the selection's start.
int CAMSEdit::SelectionSaver::GetStart() const
{
	return m_nStart;
}

// Returns the value for the selection's end.
int CAMSEdit::SelectionSaver::GetEnd() const
{
	return m_nEnd;
}

// Updates the selection's start and end with the current selection.
void CAMSEdit::SelectionSaver::Update()
{
	if (m_pEdit)
		m_pEdit->GetSel(m_nStart, m_nEnd);
}

// Disables resetting the selection in the destructor
void CAMSEdit::SelectionSaver::Disable()
{
	m_pEdit = NULL;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::Behavior

// Constructs the object from the given control.
CAMSEdit::Behavior::Behavior(CAMSEdit* pEdit) :
	m_pEdit(pEdit),
	m_uFlags(0)
{
	ASSERT(m_pEdit);
}

// Destroys the object (virtual).
CAMSEdit::Behavior::~Behavior()
{
}

// Adds and removes flags from the behavior and then redraws the control
void CAMSEdit::Behavior::ModifyFlags(UINT uAdd, UINT uRemove)
{
	UINT uFlags = (m_uFlags & ~uRemove) | uAdd;

	if (m_uFlags != uFlags)
	{
		m_uFlags = uFlags;
		_Redraw();
	}
}

// Returns the flags
UINT CAMSEdit::Behavior::GetFlags() const
{
	return m_uFlags;
}

// Handles the WM_CHAR message by passing it to the control.
void CAMSEdit::Behavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	m_pEdit->OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message by passing it to the control.
void CAMSEdit::Behavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	m_pEdit->OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message by passing it to the control.
void CAMSEdit::Behavior::_OnKillFocus(CWnd* pNewWnd) 
{
	m_pEdit->OnKillFocus(pNewWnd);
}

// Handles the WM_PASTE message by passing it to the control.
LONG CAMSEdit::Behavior::_OnPaste(UINT wParam, LONG lParam)
{
	return m_pEdit->OnPaste(wParam, lParam);
}

// Calls the default handler for the current message
LRESULT CAMSEdit::Behavior::_Default()
{
	return m_pEdit->Default();
}

// Redraws the control so that its value is valid
void CAMSEdit::Behavior::_Redraw()
{
	m_pEdit->Redraw();
}

// Returns true if the given character should be entered into the control.
bool CAMSEdit::Behavior::_ShouldEnter(TCHAR c) const
{
	return m_pEdit->ShouldEnter(c);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::AlphanumericBehavior

// Constructs the object using the given set of strInvalidChars
CAMSEdit::AlphanumericBehavior::AlphanumericBehavior(CAMSEdit* pEdit, int nMaxChars /*= 0*/, const CString& strInvalidChars /*= _T("%'*\"+?><:\\"")*/) :
	Behavior(pEdit),
	m_nMaxChars(nMaxChars),
	m_strInvalidChars(strInvalidChars)
{
	ASSERT(m_nMaxChars >= 0);
}

// Sets the characters to be considered invalid for text input.
void CAMSEdit::AlphanumericBehavior::SetInvalidCharacters(const CString& strInvalidChars)
{
	if (m_strInvalidChars == strInvalidChars)
		return;

	m_strInvalidChars = strInvalidChars;
	_Redraw();
}

// Returns the characters considered invalid for text input.
const CString& CAMSEdit::AlphanumericBehavior::GetInvalidCharacters() const
{
	return m_strInvalidChars;
}

// Sets the maximum number of characters to allow for input.
void CAMSEdit::AlphanumericBehavior::SetMaxCharacters(int nMaxChars)
{
	if (m_nMaxChars == nMaxChars)
		return;

	m_nMaxChars = nMaxChars;
	_Redraw();
}

// Returns the characters considered invalid for input.
int CAMSEdit::AlphanumericBehavior::GetMaxCharacters() const
{
	return m_nMaxChars;
}

// Returns the control's value in a valid format.
CString CAMSEdit::AlphanumericBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	CString strNewText = strText.Left(m_nMaxChars ? m_nMaxChars : strText.GetLength());

	// Remove any invalid characters from the control's text
	for (int iPos = strNewText.GetLength() - 1; iPos >= 0; iPos--)
	{
		if (m_strInvalidChars.Find(strNewText[iPos]) >= 0)
			strNewText = strNewText.Left(iPos) + strNewText.Mid(iPos + 1);
	}

	return strNewText;
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::AlphanumericBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	if (!m_strInvalidChars.IsEmpty())
	{
		// Check if the character is invalid
		if (m_strInvalidChars.Find((TCHAR)uChar) >= 0)
		{
			MessageBeep(MB_ICONEXCLAMATION);
			return;
		}
	}

	TCHAR c = static_cast<TCHAR>(uChar);
	
	// If the number of characters is already at Max, overwrite
	CString strText = m_pEdit->GetText();
	if (strText.GetLength() == m_nMaxChars && m_nMaxChars && _istprint(c))
	{
		int nStart, nEnd;
		m_pEdit->GetSel(nStart, nEnd);

		if (nStart < m_nMaxChars && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nStart + 1);
			m_pEdit->ReplaceSel(CString(c), TRUE);
		}
		return;
	}

	if (_ShouldEnter(c))
		Behavior::_OnChar(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::MaskedBehavior

// Constructs the object using the given mask.
CAMSEdit::MaskedBehavior::MaskedBehavior(CAMSEdit* pEdit, const CString& strMask /*= _T("")*/) :
	Behavior(pEdit),
	m_strMask(strMask)
{
	m_arraySymbols.Add(Symbol('#', _istdigit));  // default mask symbol
}

// Returns the mask
const CString& CAMSEdit::MaskedBehavior::GetMask() const
{
	return m_strMask;
}

// Sets the mask and redraws the control to accomodate it
void CAMSEdit::MaskedBehavior::SetMask(const CString& strMask)
{
	if (m_strMask == strMask)
		return;

	m_strMask = strMask;
	_Redraw();
}

// Returns the numeric portion of the control's value as a string
CString CAMSEdit::MaskedBehavior::GetNumericText() const
{
	CString strText = m_pEdit->GetText();
	CString strResult;

	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];
		if (_istdigit(c))
			strResult += c;
	}

	return strResult;	
}

// Returns the control's value in a valid format.
CString CAMSEdit::MaskedBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	int nMaskLen = m_strMask.GetLength();
	
	// If the mask is empty, allow anything
	if (!nMaskLen)
		return strText;

	CString strNewText;
	
	// Accomodate the text to the mask as much as possible
	for (int iPos = 0, iMaskPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++, iMaskPos++)
	{
		TCHAR c = strText[iPos];
		TCHAR cMask = (iMaskPos < nMaskLen ? m_strMask[iMaskPos] : 0);

		// If we've reached the end of the mask, break
		if (!cMask)
			break;

		// Match the character to any of the symbols
		for (int iMaskChar = 0, nMaskCharCount = m_arraySymbols.GetSize(); iMaskChar < nMaskCharCount; iMaskChar++)
		{
			const Symbol& mc = m_arraySymbols[iMaskChar];

			// Find the symbol that applies for the given character
			if (!mc.Validate(c))
				continue;

			// Try to add matching characters in the mask until a different symbol is reached
			for (; iMaskPos < nMaskLen; iMaskPos++)
			{
				cMask = m_strMask[iMaskPos];
				if (cMask == mc)
				{
					strNewText += mc.Convert(c);
					break;
				} 
				else
				{
					for (int iMaskChar2 = 0; iMaskChar2 < nMaskCharCount; iMaskChar2++)
					{
						if (cMask == m_arraySymbols[iMaskChar2])
							break;
					}

					if (iMaskChar2 < nMaskCharCount)
						break;

					strNewText += cMask;
				}
			}

			break;
		}

		// If the character was not matched to a symbol, stop
		if (iMaskChar == nMaskCharCount)
		{
			if (c == cMask)
			{
				// Match the character to any of the symbols
				for (iMaskChar = 0, nMaskCharCount = m_arraySymbols.GetSize(); iMaskChar < nMaskCharCount; iMaskChar++)
				{
					if (cMask == m_arraySymbols[iMaskChar])
						break;
				}

				if (iMaskChar == nMaskCharCount)
				{
					strNewText += c;
					continue;
				}
			}

			break;
		}
	}

	return strNewText;
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::MaskedBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	TCHAR c = static_cast<TCHAR>(uChar);

	// If the mask is empty, allow anything
	int nMaskLen = m_strMask.GetLength();
	if (!nMaskLen)
	{
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	// Check that we haven't gone past the mask's length
	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);
	if (nStart >= nMaskLen && c != VK_BACK)
		return;

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	// Check for a non-printable character (such as Ctrl+C)
	if (!_istprint(c))
	{
		if (c == VK_BACK && nStart != nLen)
		{
			m_pEdit->SendMessage(WM_KEYDOWN, VK_LEFT); // move the cursor left
			return;
		}
		
		// Allow backspace only if the cursor is all the way to the right
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	TCHAR cMask = m_strMask[nStart];

	// Check if the mask's character matches with any of the symbols in the array.
	for (int iMaskChar = 0, nMaskCharCount = m_arraySymbols.GetSize(); iMaskChar < nMaskCharCount; iMaskChar++)
	{
		const Symbol& mc = m_arraySymbols[iMaskChar];
		if (cMask == mc)
		{
			if (mc.Validate(c) && _ShouldEnter(c))
			{
				nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
				m_pEdit->SetSel(nStart, nEnd);
				m_pEdit->ReplaceSel(CString(mc.Convert(c)), TRUE);
			}
			return;
		}
	}

	// Check if it's the same character as the mask.
	if (cMask == c && _ShouldEnter(c))
	{
		nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
		m_pEdit->SetSel(nStart, nEnd);
		m_pEdit->ReplaceSel(CString(c), TRUE);
		return;
	}

	// If it's a valid character, find the next symbol on the mask and add any non-mask characters in between.
	for (iMaskChar = 0, nMaskCharCount = m_arraySymbols.GetSize(); iMaskChar < nMaskCharCount; iMaskChar++)
	{
		const Symbol& mc = m_arraySymbols[iMaskChar];

		// See if the character is valid for any other symbols
		if (!mc.Validate(c))
			continue;

		// Find the position of the next symbol 
		CString strMasks;
		for (int iMaskChar2 = 0; iMaskChar2 < nMaskCharCount; iMaskChar2++)
			strMasks += m_arraySymbols[iMaskChar2];
		CString strMaskPortion = m_strMask.Mid(nStart);
		int nMaskPos = strMaskPortion.FindOneOf(strMasks);
			
		// Enter the character if there isn't another symbol before it
		if (nMaskPos >= 0 && strMaskPortion[nMaskPos] == mc && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nStart + nMaskPos);
			m_pEdit->ReplaceSel(strMaskPortion.Left(nMaskPos), TRUE);

			_OnChar(uChar, nRepCnt, nFlags);
		}
		return;
	}
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSEdit::MaskedBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			// If deleting make sure it's the last character or that
			// the selection goes all the way to the end of the text

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			if (nEnd != nLen)
			{
				if (!(nEnd == nStart && nEnd == nLen - 1))
					return;
			}
			break;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Returns a reference to the array of symbols that may be found on the mask.
// This allows adding, editing, or deleting symbols for the mask.
CAMSEdit::MaskedBehavior::SymbolArray& CAMSEdit::MaskedBehavior::GetSymbolArray()
{
	return m_arraySymbols;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::MaskedBehavior::Symbol

// Constructs the object -- needed for CArray
CAMSEdit::MaskedBehavior::Symbol::Symbol() :
	m_cSymbol(0),
	m_fnValidation(NULL),
	m_fnConversion(NULL)
{
}	

// Constructs the object with the given character and set of functions
CAMSEdit::MaskedBehavior::Symbol::Symbol(TCHAR cSymbol, ValidationFunction fnValidation, ConversionFunction fnConversion /*= NULL*/) :
	m_cSymbol(cSymbol),
	m_fnValidation(fnValidation),
	m_fnConversion(fnConversion)
{
}	

// Destroys the object (virtual).
CAMSEdit::MaskedBehavior::Symbol::~Symbol()
{
}

// Returns true if the given character (usually just entered by the user) is a match for self's symbol.
// This is tested by passing it to the validation function passed in the constructor (if valid).
bool CAMSEdit::MaskedBehavior::Symbol::Validate(TCHAR c) const
{
	if (m_fnValidation)
		return (m_fnValidation(c) != 0);
	return true;
}

// Returns the given character converted as a result of calling the conversion function was passed in the constructor.
// If no conversion function was passed, the character is returned intact.
TCHAR CAMSEdit::MaskedBehavior::Symbol::Convert(TCHAR c) const
{
	if (m_fnConversion)
		return (TCHAR)m_fnConversion(c);
	return c;
}

// Sets the character for the symbol to be used in the mask.
void CAMSEdit::MaskedBehavior::Symbol::Set(TCHAR cSymbol)
{
	m_cSymbol = cSymbol;
}

// Returns the character for the symbol to be used in the mask.
TCHAR CAMSEdit::MaskedBehavior::Symbol::Get() const
{
	return m_cSymbol;
}

// Returns the character for the symbol to be used in the mask.
CAMSEdit::MaskedBehavior::Symbol::operator TCHAR() const
{
	return m_cSymbol;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::NumericBehavior

// Constructs the object using the given nMaxWholeDigits and nMaxDecimalPlaces.
CAMSEdit::NumericBehavior::NumericBehavior(CAMSEdit* pEdit, int nMaxWholeDigits /*= 9*/, int nMaxDecimalPlaces /*= 4*/) :
	Behavior(pEdit),
	m_nMaxWholeDigits(nMaxWholeDigits >= 0 ? nMaxWholeDigits : -nMaxWholeDigits),
	m_nMaxDecimalPlaces(nMaxDecimalPlaces),
	m_bAllowNegative(nMaxWholeDigits >= 0),
	m_cNegativeSign('-'),
	m_cDecimalPoint('.'),
	m_cGroupSeparator(','),
	m_nDigitsInGroup(0),
	m_dMin(AMS_MIN_NUMBER),	
	m_dMax(AMS_MAX_NUMBER)	
{
	ASSERT(m_nMaxWholeDigits > 0);		// must have at least 1 digit to the left of the decimal
	ASSERT(m_nMaxDecimalPlaces >= 0);	// decimal places must be positive

	// Get the system's negative sign
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, NULL, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SNEGATIVESIGN, &m_cNegativeSign, sizeof(m_cNegativeSign));

	// Get the system's decimal point	
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, NULL, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDECIMAL, &m_cDecimalPoint, sizeof(m_cDecimalPoint));

	// Get the system's group separator
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, NULL, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STHOUSAND, &m_cGroupSeparator, sizeof(m_cGroupSeparator));

	// Ensure the separators are not the same
	if (m_cDecimalPoint == m_cGroupSeparator)
		m_cGroupSeparator = (m_cDecimalPoint == ',' ? '.' : ',');
}

// Sets the maximum number of digits before the decimal point.
// If nMaxWholeDigits is negative, then negative numbers will NOT be allowed.
void CAMSEdit::NumericBehavior::SetMaxWholeDigits(int nMaxWholeDigits)
{
	ASSERT(nMaxWholeDigits);

	// If nMaxWholeDigits is negative, don't allow negatives
	bool bAllowNegative = (nMaxWholeDigits >= 0);
	if (nMaxWholeDigits < 0)
		nMaxWholeDigits = -nMaxWholeDigits;
		
	if (m_nMaxWholeDigits == nMaxWholeDigits && m_bAllowNegative == bAllowNegative)
		return;

	m_nMaxWholeDigits = nMaxWholeDigits;
	m_bAllowNegative = bAllowNegative;
	_Redraw();
}

// Returns the maximum number of digits before the decimal point.
int CAMSEdit::NumericBehavior::GetMaxWholeDigits() const
{
	return m_nMaxWholeDigits;
}

// Sets the maximum number of digits after the decimal point.
void CAMSEdit::NumericBehavior::SetMaxDecimalPlaces(int nMaxDecimalPlaces)
{
	ASSERT(nMaxDecimalPlaces >= 0);
	if (m_nMaxDecimalPlaces == nMaxDecimalPlaces)
		return;

	m_nMaxDecimalPlaces = nMaxDecimalPlaces;
	_Redraw();
}

// Returns the maximum number of digits after the decimal point.
int CAMSEdit::NumericBehavior::GetMaxDecimalPlaces() const
{
	return m_nMaxDecimalPlaces;
}

// Sets whether the negative sign is allowed in the number or not.
void CAMSEdit::NumericBehavior::AllowNegative(bool bAllowNegative /*= true*/)
{
	if (m_bAllowNegative == bAllowNegative)
		return;

	m_bAllowNegative = bAllowNegative;
	_Redraw();
}

// Returns true if the negative sign is allowed in the number.
bool CAMSEdit::NumericBehavior::IsNegativeAllowed() const
{
	return m_bAllowNegative;
}

// Sets the number of digits to be grouped together (if any).
void CAMSEdit::NumericBehavior::SetDigitsInGroup(int nDigitsInGroup)
{
	ASSERT(nDigitsInGroup >= 0);
	if (m_nDigitsInGroup == nDigitsInGroup)
		return;

	m_nDigitsInGroup = nDigitsInGroup;
	_Redraw();
}

// Returns the number of digits to be grouped together (if any).
int CAMSEdit::NumericBehavior::GetDigitsInGroup() const
{
	return m_nDigitsInGroup;
}

// Sets the character to use for the decimal point and the group separator (thousands)
void CAMSEdit::NumericBehavior::SetSeparators(TCHAR cDecimal, TCHAR cGroup)
{
	ASSERT(cDecimal);
	ASSERT(cGroup);

	// If nothing's changing, leave
	if (m_cDecimalPoint == cDecimal && m_cGroupSeparator == cGroup)
		return;

	// Set them
	m_cDecimalPoint = cDecimal;
	m_cGroupSeparator = cGroup;

	// Ensure they're not the same
	if (m_cDecimalPoint == m_cGroupSeparator)
		m_cGroupSeparator = (m_cDecimalPoint == ',' ? '.' : ',');

	_Redraw();
}

// Retrieves the character being used for the decimal point and group separator (thousands).
void CAMSEdit::NumericBehavior::GetSeparators(TCHAR* pcDecimal, TCHAR* pcGroup) const
{
	if (pcDecimal)
		*pcDecimal = m_cDecimalPoint;
	if (pcGroup)
		*pcGroup = m_cGroupSeparator;
}	

// Sets the text to be automatically inserted in front of the number (such as a currency sign).
void CAMSEdit::NumericBehavior::SetPrefix(const CString& strPrefix)
{
	if (m_strPrefix == strPrefix)
		return;

	m_strPrefix = strPrefix;
	_Redraw();
}

// Returns the text to be automatically inserted in front of the number (such as a currency sign).
const CString& CAMSEdit::NumericBehavior::GetPrefix() const
{
	return m_strPrefix;
}

// Parses the given strMask to set the control's configuration.
void CAMSEdit::NumericBehavior::SetMask(const CString& strMask)
{
	int nDecimalPos = -1;
	int nGroupingPos = -1;
	int nLen = strMask.GetLength();

	m_nMaxWholeDigits = 0;
	m_nMaxDecimalPlaces = 0;
	m_nDigitsInGroup = 0;
	m_bAllowNegative = true;
	m_strPrefix = _T("");

	for (int iPos = nLen - 1; iPos >= 0; iPos--)
	{
		TCHAR c = strMask[iPos];
		if (c == '#')
		{
			if (nDecimalPos >= 0)
				m_nMaxWholeDigits++;
			else
				m_nMaxDecimalPlaces++;
		}
		else if ((c == '.' || c == m_cDecimalPoint) && nDecimalPos < 0)
		{
			nDecimalPos = iPos;
			m_cDecimalPoint = c;
		}
		else if (c == ',' || c == m_cGroupSeparator)
		{
			if (!m_nDigitsInGroup)
			{
				m_nDigitsInGroup = (((nDecimalPos >= 0) ? nDecimalPos : nLen) - iPos) - 1;
				m_cGroupSeparator = c;
			}
		}
		else
		{
			m_strPrefix = strMask.Left(iPos + 1);
			break;
		}
	}

	if (nDecimalPos < 0)
	{
		m_nMaxWholeDigits = m_nMaxDecimalPlaces;
		m_nMaxDecimalPlaces = 0;
	}

	ASSERT(m_nMaxWholeDigits > 0);	// must have at least one digit on left side of decimal point
	_Redraw();
}

// Gets the mask corresponding to the maximum number than can be entered into the control
CString CAMSEdit::NumericBehavior::GetMask() const
{
	CString strMask;

	for (int iDigit = 0; iDigit < m_nMaxWholeDigits; iDigit++)
		strMask += '0';

	if (m_nMaxDecimalPlaces)
		strMask += m_cDecimalPoint;

	for (iDigit = 0; iDigit < m_nMaxDecimalPlaces; iDigit++)
		strMask += '0';

	strMask = GetSeparatedText(strMask);

	for (int iPos = 0, nLen = strMask.GetLength(); iPos < nLen; iPos++)
	{
		if (strMask[iPos] == '0')
			strMask.SetAt(iPos, '#');
	}

	return strMask;
}

// Sets the range of allowed values to the given minimum and maximum double values.
void CAMSEdit::NumericBehavior::SetRange(double dMin, double dMax)
{
	ASSERT(dMin <= dMax);

	m_dMin = dMin;
	m_dMax = dMax;

	_Redraw();
}

// Retrieves the range of allowed values inside the given set of double pointers.
void CAMSEdit::NumericBehavior::GetRange(double* pdMin, double* pdMax) const
{
	if (pdMin)
		*pdMin = m_dMin;
	if (pdMax)
		*pdMax = m_dMax;
}

// Returns the number of group separator characters in the given strText.
int CAMSEdit::NumericBehavior::GetGroupSeparatorCount(const CString& strText) const
{
	for (int iPos = 0, nSepCount = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		if (strText[iPos] == m_cGroupSeparator)
			nSepCount++;
	}

	return nSepCount;
}

// Returns the given strText as a numeric string.
CString CAMSEdit::NumericBehavior::GetNumericText(const CString& strText, bool bConvertToNumericSigns /*= false*/) const
{
	CString strNewText;

	for (int iPos = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];
		if (_istdigit(c))
			strNewText += c;
		else if (c == m_cNegativeSign)
			strNewText += (bConvertToNumericSigns ? '-' : m_cNegativeSign);
		else if (c == m_cDecimalPoint)
			strNewText += (bConvertToNumericSigns ? '.' : m_cDecimalPoint);
	}

	return strNewText;
}

// Returns the current double as a text value.
// If bTrimTrailingZeros is true, any insignificant zeros after the decimal point are removed.
CString CAMSEdit::NumericBehavior::GetDoubleText(double dText, bool bTrimTrailingZeros /*= true*/) const
{
	CString strText;
	strText.Format(_T("%lf"), dText);

	if (bTrimTrailingZeros)
	{
		strText.TrimRight('0');
		strText.TrimRight('.');
	}

	return strText;
}

// Sets the control's text to the given double value.
// If bTrimTrailingZeros is true, any insignificant zeros after the decimal point are removed.
void CAMSEdit::NumericBehavior::SetDouble(double dText, bool bTrimTrailingZeros /*= true*/)
{
	m_pEdit->SetWindowText(GetDoubleText(dText, bTrimTrailingZeros));
}

// Returns the current text as a double value.
double CAMSEdit::NumericBehavior::GetDouble() const
{
	return _tcstod(GetNumericText(m_pEdit->GetText(), true), NULL);
}

// Sets the control's text to the given integer value.
void CAMSEdit::NumericBehavior::SetInt(int nText)
{
	CString strText;
	strText.Format(_T("%d"), nText);
	m_pEdit->SetWindowText(strText);
}

// Returns the current text as an integer value.
int CAMSEdit::NumericBehavior::GetInt() const
{
	return _ttoi(GetNumericText(m_pEdit->GetText(), true));
}

// Adjusts the location of separators based on the nCurrentSeparatorCount.
void CAMSEdit::NumericBehavior::AdjustSeparators(int nCurrentSeparatorCount)
{
	SelectionSaver selection = m_pEdit;

	CString strText = _GetValidText();
	if (strText.IsEmpty() && selection.GetStart() <= m_strPrefix.GetLength())
		m_pEdit->SetWindowText(m_strPrefix.Mid(0, selection.GetStart()));
	else if (strText != m_pEdit->GetText())
		m_pEdit->SetWindowText(strText);

	// Adjust the current selection if separators were added/removed
	int nNewSeparatorCount = GetGroupSeparatorCount(strText);
	if (nCurrentSeparatorCount != nNewSeparatorCount && selection.GetStart() > m_strPrefix.GetLength())
		selection += (nNewSeparatorCount - nCurrentSeparatorCount);
}

// Returns the given text with the group separator characters inserted in the proper places.
CString CAMSEdit::NumericBehavior::GetSeparatedText(const CString& strText) const
{
	CString strNumericText = GetNumericText(strText);
	CString strNewText = strNumericText;
	
	// Retrieve the number without the decimal point
	int nDecimalPos = strNumericText.Find(m_cDecimalPoint);
	if (nDecimalPos >= 0)
		strNewText = strNewText.Left(nDecimalPos);

	if (m_nDigitsInGroup > 0)
	{
		int nLen = strNewText.GetLength();
		BOOL bIsNegative = (!strNewText.IsEmpty() && strNewText[0] == m_cNegativeSign);

		// Loop in reverse and stick the separator every m_nDigitsInGroup digits.
		for (int iPos = nLen - (m_nDigitsInGroup + 1); iPos >= bIsNegative; iPos -= m_nDigitsInGroup)
			strNewText = strNewText.Left(iPos + 1) + m_cGroupSeparator + strNewText.Mid(iPos + 1);
	}

	// Prepend the prefix, if the number is not empty.
	if (!strNewText.IsEmpty() || nDecimalPos >= 0)
	{
		strNewText = m_strPrefix + strNewText;

		if (nDecimalPos >= 0)
			strNewText += strNumericText.Mid(nDecimalPos);
	}

	return strNewText;
}

// Inserts nCount zeros into the given string at the given position.
// If nPos is less than 0, the zeros are appended.
void CAMSEdit::NumericBehavior::InsertZeros(CString* pStrText, int nPos, int nCount)
{
	ASSERT(pStrText);

	if (nPos < 0 && nCount > 0)
		nPos = pStrText->GetLength();

	for (int iZero = 0; iZero < nCount; iZero++)
		pStrText->Insert(nPos, '0');
}

// Returns the control's value in a valid format.
CString CAMSEdit::NumericBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	CString strNewText;
	bool bIsNegative = false;
	int nPrefixLen = m_strPrefix.GetLength();

	// Remove any invalid characters from the number
	for (int iPos = 0, nDecimalPos = -1, nNewLen = 0, nLen = strText.GetLength(); iPos < nLen; iPos++)
	{
		TCHAR c = strText[iPos];

		// Check for a negative sign
		if (iPos == nPrefixLen && c == m_cNegativeSign && m_bAllowNegative)
		{
			strNewText += c;
			nNewLen++;
			bIsNegative = true;
		}
		// Check for a digit
		else if (_istdigit(c))
		{
			// Make sure it doesn't go beyond the limits
			if (nDecimalPos < 0 && nNewLen == m_nMaxWholeDigits + bIsNegative)
				continue;

			if (nDecimalPos >= 0 && nNewLen > nDecimalPos + m_nMaxDecimalPlaces)
				break;

			strNewText += c;
			nNewLen++;
		}
		// Check for a decimal point
		else if (c == m_cDecimalPoint && nDecimalPos < 0)
		{
			if (m_nMaxDecimalPlaces == 0)
				break;

			strNewText += c;
			nDecimalPos = nNewLen;
			nNewLen++;
		}
	}

	return GetSeparatedText(strNewText);
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::NumericBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	TCHAR c = static_cast<TCHAR>(uChar);

	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);

	CString strText = m_pEdit->GetText();
	CString strNumericText = GetNumericText(strText);
	int nDecimalPos = strText.Find(m_cDecimalPoint);
	int nNumericDecimalPos = strNumericText.Find(m_cDecimalPoint);
	int nLen = strText.GetLength();
	int nNumericLen = strNumericText.GetLength();
	int nPrefixLen = m_strPrefix.GetLength();
	int nSepCount = GetGroupSeparatorCount(strText);
	bool bNeedAdjustment = false;

	// Check if we're in the prefix's location
	if (nStart < nPrefixLen && _istprint(c))
	{
		TCHAR cPrefix = m_strPrefix[nStart];

		// Check if it's the same character as the prefix.
		if (cPrefix == c && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
				m_pEdit->SetSel(nStart, nEnd);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		// If it's a part of the number, enter the prefix
		else if ((_istdigit(c) || c == m_cNegativeSign || c == m_cDecimalPoint) && _ShouldEnter(c))
		{
			nEnd = (nEnd == nLen ? nEnd : (nPrefixLen));
			m_pEdit->SetSel(nStart, nEnd);
			m_pEdit->ReplaceSel(m_strPrefix.Mid(nStart), TRUE);

			NumericBehavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		
		return;
	}

	// Check if it's a negative sign
	if (c == m_cNegativeSign && m_bAllowNegative)
	{
		// If it's at the beginning, determine if it should overwritten
		if (nStart == nPrefixLen)
		{
			if (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign && _ShouldEnter(c))
			{
				nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
				m_pEdit->SetSel(nStart, nEnd);
				m_pEdit->ReplaceSel(CString(m_cNegativeSign), TRUE);
				return;
			}
		}
		// If we're not at the beginning, toggle the sign
		else if (_ShouldEnter(c))
		{
			if (strNumericText[0] == m_cNegativeSign)
			{
				m_pEdit->SetSel(nPrefixLen, nPrefixLen + 1);
				m_pEdit->ReplaceSel(_T(""), TRUE);
				m_pEdit->SetSel(nStart - 1, nEnd - 1);
			}
			else
			{
				m_pEdit->SetSel(nPrefixLen, nPrefixLen);
				m_pEdit->ReplaceSel(CString(m_cNegativeSign), TRUE);
				m_pEdit->SetSel(nStart + 1, nEnd + 1);
			}

			return;
		}
	}

	// Check if it's a decimal point (only one is allowed).
	else if (c == m_cDecimalPoint && m_nMaxDecimalPlaces > 0)
	{
		if (nDecimalPos >= 0)
		{
			// Check if we're replacing the decimal point
			if (nDecimalPos >= nStart && nDecimalPos < nEnd)
				bNeedAdjustment = true;
			else
			{	// Otherwise, put the caret on it
				if (_ShouldEnter(c))
					m_pEdit->SetSel(nDecimalPos + 1, nDecimalPos + 1);
				return;
			}
		}
	}

	// Check if it's a digit
	else if (_istdigit(c))
	{
		// Check if we're on the right of the decimal point.
		if (nDecimalPos >= 0 && nDecimalPos < nStart)
		{
			if (strNumericText.Mid(nNumericDecimalPos + 1).GetLength() == m_nMaxDecimalPlaces)
			{
				if (nStart <= nDecimalPos + m_nMaxDecimalPlaces && _ShouldEnter(c))
				{
					nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
					m_pEdit->SetSel(nStart, nEnd);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				return;
			}
		}

		// We're on the left side of the decimal point
		else 
		{
			bool bIsNegative = (!strNumericText.IsEmpty() && strNumericText[0] == m_cNegativeSign);

			// Make sure we can still enter digits.
			if (nStart == m_nMaxWholeDigits + bIsNegative + nSepCount + nPrefixLen)
			{
				if (m_uFlags & AddDecimalAfterMaxWholeDigits && m_nMaxDecimalPlaces > 0)
				{
					nEnd = (nEnd == nLen ? nEnd : (nStart + 2));
					m_pEdit->SetSel(nStart, nEnd);
					m_pEdit->ReplaceSel(CString(m_cDecimalPoint) + c, TRUE);
				}
				return;
			}

			if (strNumericText.Mid(0, nNumericDecimalPos >= 0 ? nNumericDecimalPos : nNumericLen).GetLength() == m_nMaxWholeDigits + bIsNegative)
			{
				if (_ShouldEnter(c))
				{
					if (strText[nStart] == m_cGroupSeparator)
						nStart++;

					nEnd = (nEnd == nLen ? nEnd : (nStart + 1));
					m_pEdit->SetSel(nStart, nEnd);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				return;
			}

			bNeedAdjustment = true;
		}
	}

	// Check if it's a non-printable character, such as Backspace or Ctrl+C
	else if (!_istprint(c))
		bNeedAdjustment = true;
	else
		return;

	// Check if the character should be entered
	if (!_ShouldEnter(c))
		return;
	
	Behavior::_OnChar(uChar, nRepCnt, nFlags);

	// If the decimal point was added/removed or a separator needs adding/removing, adjust the text
	if (bNeedAdjustment || nNumericDecimalPos != GetNumericText(m_pEdit->GetText()).Find(m_cDecimalPoint))
		AdjustSeparators(nSepCount);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSEdit::NumericBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			// If deleting the prefix, don't allow it if there's a number after it.
			int nPrefixLen = m_strPrefix.GetLength();
			if (nStart < nPrefixLen && nLen > nPrefixLen)
			{
				if (nEnd == nLen)
					break;
				return;
			}

			if (nStart < nLen && strText[nStart] == m_cGroupSeparator && nStart == nEnd)
				Behavior::_OnKeyDown(VK_RIGHT, nRepCnt, nFlags);

			// Allow the deletion and then adjust the value
			int nSepCount = GetGroupSeparatorCount(strText);
			Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);

			AdjustSeparators(nSepCount);

			// If everything on the right was deleted, put the selection on the right
			if (nEnd == nLen)
				m_pEdit->SetSel(nStart, nStart);

			return;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
// It's used here to check if zeros need to be added to the value.
void CAMSEdit::NumericBehavior::_OnKillFocus(CWnd* pNewWnd) 
{
	Behavior::_OnKillFocus(pNewWnd);

	// Check if the value is empty and we don't want to touch it
	CString strOriginalText = GetNumericText(m_pEdit->GetText(), true);
	CString strText = strOriginalText;
	int nLen = strText.GetLength();

	// Add a leading zero before the decimal point if necessary
	if (nLen > 0 && strText[0] == '.')
		strText.Insert(0, '0');
	else if (!(m_uFlags & OnKillFocus_Max) || (nLen == 0 && m_uFlags & OnKillFocus_DontPadWithZerosIfEmpty))
		return;

	int nDecimalPos = strText.Find('.');
	int nMaxDecimalPlaces = GetMaxDecimalPlaces();
	int nMaxWholeDigits = GetMaxWholeDigits();

	// Check if we need to pad the number with zeros after the decimal point
	if (m_uFlags & OnKillFocus_PadWithZerosAfterDecimal && nMaxDecimalPlaces > 0)
	{
		if (nDecimalPos < 0)
		{
			if (nLen == 0 || strText == '-')
			{
				strText = '0';
				nLen = 1;
			}
			strText += '.';
			nDecimalPos = nLen++;
		}

		InsertZeros(&strText, -1, nMaxDecimalPlaces - (nLen - nDecimalPos - 1));
	}

	// Check if we need to pad the number with zeros before the decimal point
	if (m_uFlags & OnKillFocus_PadWithZerosBeforeDecimal && nMaxWholeDigits > 0)
	{
		if (nDecimalPos < 0)
			nDecimalPos = nLen;

		if (nLen && strText[0] == '-')
			nDecimalPos--;

		InsertZeros(&strText, (nLen ? strText[0] == '-' : -1), nMaxWholeDigits - nDecimalPos);
	}

	if (strText != strOriginalText)
	{
		SelectionSaver selection = m_pEdit;	// remember the current selection 
		m_pEdit->SetWindowText(strText);
	}

	// If it's empty, take action based on the flag
	if (strText.IsEmpty())
	{
		if (m_uFlags & OnKillFocus_Beep_IfEmpty)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfEmpty)
			AdjustWithinRange();

		if ((m_uFlags & OnKillFocus_ShowMessage_IfEmpty) == OnKillFocus_ShowMessage_IfEmpty)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfEmpty)
			m_pEdit->SetFocus();

		return;
	}
		
	if (!IsValid())
	{
		if (m_uFlags & OnKillFocus_Beep_IfInvalid)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfInvalid)
			AdjustWithinRange();
		
		if ((m_uFlags & OnKillFocus_ShowMessage_IfInvalid) == OnKillFocus_ShowMessage_IfInvalid)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfInvalid)
			m_pEdit->SetFocus();
	}
}

// Returns true if the control's value is valid and falls within the allowed range.
bool CAMSEdit::NumericBehavior::IsValid() const
{
	double dValue = GetDouble();
	return (dValue >= m_dMin && dValue <= m_dMax);
}

// Returns true if the control's value is valid and falls within the allowed range.
// If bShowErrorIfNotValid is true, an error message box is shown and the control gets the focus.
bool CAMSEdit::NumericBehavior::CheckIfValid(bool bShowErrorIfNotValid /*= true*/)
{
	if (!m_pEdit->IsWindowEnabled())
		return true;

	bool bValid = IsValid();
	if (!bValid && bShowErrorIfNotValid)
	{
		ShowErrorMessage();
		m_pEdit->SetFocus();
	}
	
	return bValid;
}

// Shows a message box informing the user to enter a valid value within the allowed range.
void CAMSEdit::NumericBehavior::ShowErrorMessage() const
{
	CString strMessage = _T("Please specify a valid numeric value.");
	
	if (m_dMin > AMS_MIN_NUMBER && m_dMax < AMS_MAX_NUMBER)
		strMessage = _T("Please specify a numeric value between ") + GetDoubleText(m_dMin) + _T(" and ") + GetDoubleText(m_dMax) + _T(".");
	else if (m_dMin > AMS_MIN_NUMBER)
		strMessage = _T("Please specify a numeric value greater than or equal to ") + GetDoubleText(m_dMin) + _T(".");
	else if (m_dMax < AMS_MAX_NUMBER)
		strMessage = _T("Please specify a numeric value less than or equal to ") + GetDoubleText(m_dMax) + _T(".");

	AfxMessageBox(strMessage, MB_ICONEXCLAMATION);
}

// Adjusts the control's value to be within the range of allowed numeric values.
void CAMSEdit::NumericBehavior::AdjustWithinRange()
{
	// Check if it's already within the range
	if (IsValid())
		return;

	// If it's empty, set it a valid number
	if (m_pEdit->GetText().IsEmpty())
		m_pEdit->SetWindowText(_T(" "));
	else
		_Redraw();

	// Make it fall within the range
	double dValue = GetDouble();
    if (dValue < m_dMin)
		SetDouble(m_dMin);
    else if (dValue > m_dMax)
		SetDouble(m_dMax);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::DateBehavior

// Constructs the object with the given control.
CAMSEdit::DateBehavior::DateBehavior(CAMSEdit* pEdit) :
	Behavior(pEdit),
	m_dateMin(AMS_MIN_OLEDATETIME),
	m_dateMax(AMS_MAX_OLEDATETIME),
	m_cSep('/')
{
	// Get the system's date separator
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, NULL, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SDATE, &m_cSep, sizeof(m_cSep));

	// Determine if the day should go before the month
	TCHAR szShortDate[MAX_PATH];
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, NULL, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SSHORTDATE, szShortDate, sizeof(szShortDate));

		for (int iPos = 0; szShortDate[iPos]; iPos++)
		{
			TCHAR c = _totupper(szShortDate[iPos]);
			if (c == 'M')	// see if the month is first
				break;
			if (c == 'D')	// see if the day is first, and then set the flag
			{
				m_uFlags |= DayBeforeMonth;
				break;
			}
		}
	}
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::DateBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	TCHAR c = static_cast<TCHAR>(uChar);

	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	// Check for a non-printable character (such as Ctrl+C)
	if (!_istprint(c))
	{
		if (c == VK_BACK && nStart != nLen)
		{
			m_pEdit->SendMessage(WM_KEYDOWN, VK_LEFT); // move the cursor left
			return;
		}
		
		// Allow backspace only if the cursor is all the way to the right
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	// Add the digit depending on its location
	switch (nStart)
	{
		case 0:		// FIRST DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidDayDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidDay(GetDay()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinDayDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
				// Check if we can insert the digit with a leading zero
				else if (nLen == nStart && GetMinDayDigit(0) == '0' && IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(nStart, nStart + 2);
					m_pEdit->ReplaceSel(CString('0') + c, TRUE);					
				}
			}
			else
			{
				if (IsValidMonthDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidMonth(GetMonth()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinMonthDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
						AdjustMaxDay();
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
				// Check if we can insert the digit with a leading zero
				else if (nLen == nStart && GetMinMonthDigit(0) == '0' && IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(nStart, nStart + 2);
					m_pEdit->ReplaceSel(CString('0') + c, TRUE);					
				}
			}
			break;
		}
		case 1:		// SECOND DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
				// Check if it's a slash and the first digit (preceded by a zero) is a valid month
				else if (c == m_cSep && nLen == nStart && GetMinDayDigit(0) == '0' && IsValidDay(_ttoi(CString('0') + strText[0])) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(0, nStart);
					m_pEdit->ReplaceSel(CString('0') + strText[0] + c, TRUE);					
				}
			}
			else
			{
				if (IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (GetDay() > 0 && AdjustMaxDay())
							m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 2);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}				
				// Check if it's a slash and the first digit (preceded by a zero) is a valid month
				else if (c == m_cSep && nLen == nStart && GetMinMonthDigit(0) == '0' && IsValidMonth(_ttoi(CString('0') + strText[0])) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(0, nStart);
					m_pEdit->ReplaceSel(CString('0') + strText[0] + c, TRUE);					
				}
			}
			break;
		}
		
		case 2:		// FIRST SLASH
		{
			int nSlash = 0;
			if (c == m_cSep)
				nSlash = 1;
			else
			{
				if (m_uFlags & DayBeforeMonth)
					nSlash = (IsValidMonthDigit(c, 0) || (nLen == nStart && GetMinMonthDigit(0) == '0' && IsValidMonthDigit(c, 1))) ? 2 : 0;
				else
					nSlash = (IsValidDayDigit(c, 0) || (nLen == nStart && GetMinDayDigit(0) == '0' && IsValidDayDigit(c, 1))) ? 2 : 0;
			}

			// If we need the slash, enter it
			if (nSlash && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
			}

			// If the slash is to be preceded by a valid digit, "type" it in.
			if (nSlash == 2)
				keybd_event((BYTE)c, 0, 0, 0);
			break;
		}

		case 3:		// THIRD DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidMonthDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidMonth(GetMonth()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinMonthDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);

					AdjustMaxDay();
				}
				// Check if we can insert the digit with a leading zero
				else if (nLen == nStart && GetMinMonthDigit(0) == '0' && IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(nStart, nStart + 2);
					m_pEdit->ReplaceSel(CString('0') + c, TRUE);					
					AdjustMaxDay();
				}
			}
			else
			{
				if (IsValidDayDigit(c, 0) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (nLen > nStart + 1)
						{
							if (!IsValidDay(GetDay()))
							{
								m_pEdit->SetSel(nStart + 1, nStart + 2);
								m_pEdit->ReplaceSel(CString(GetMinDayDigit(1)), TRUE);
								m_pEdit->SetSel(nStart + 1, nStart + 2);
							}
						}
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
				// Check if we can insert the digit with a leading zero
				else if (nLen == nStart && GetMinDayDigit(0) == '0' && IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(nStart, nStart + 2);
					m_pEdit->ReplaceSel(CString('0') + c, TRUE);					
				}
			}
			break;			
		}

		case 4:		// FOURTH DIGIT
		{
			if (m_uFlags & DayBeforeMonth)
			{
				if (IsValidMonthDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);

						if (GetDay() > 0 && AdjustMaxDay())
							m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 2);
					}
					else
					{
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
						AdjustMaxDay();
					}
				}
				// Check if it's a slash and the first digit (preceded by a zero) is a valid month
				else if (c == m_cSep && nLen == nStart && GetMinMonthDigit(0) == '0' && IsValidMonth(_ttoi(CString('0') + strText[3])) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(3, nStart);
					m_pEdit->ReplaceSel(CString('0') + strText[3] + c, TRUE);					
				}
			}
			else
			{
				if (IsValidDayDigit(c, 1) && _ShouldEnter(c))
				{
					if (nLen > nStart)
					{
						m_pEdit->SetSel(nStart, nStart + 1);
						m_pEdit->ReplaceSel(CString(c), TRUE);
					}
					else
						Behavior::_OnChar(uChar, nRepCnt, nFlags);
				}
				// Check if it's a slash and the first digit (preceded by a zero) is a valid month
				else if (c == m_cSep && nLen == nStart && GetMinDayDigit(0) == '0' && IsValidDay(_ttoi(CString('0') + strText[3])) && _ShouldEnter(c))
				{
					m_pEdit->SetSel(3, nStart);
					m_pEdit->ReplaceSel(CString('0') + strText[3] + c, TRUE);					
				}
			}
			break;
		}

		case 5:		// SECOND SLASH	(year's first digit)
		{
			int nSlash = 0;
			if (c == m_cSep)
				nSlash = 1;
			else
				nSlash = (IsValidYearDigit(c, 0) ? 2 : 0);

			// If we need the slash, enter it
			if (nSlash && _ShouldEnter(c))
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
			}

			// If the slash is to be preceded by a valid digit, "type" it in.
			if (nSlash == 2)
				keybd_event((BYTE)c, 0, 0, 0);
			break;			
		}

		case 6:		// YEAR (all 4 digits)
		case 7:
		case 8:
		case 9:
		{
			if (IsValidYearDigit(c, nStart - GetYearStartPosition()) && _ShouldEnter(c))
			{
				if (nLen > nStart)
				{
					m_pEdit->SetSel(nStart, nStart + 1, FALSE);
					m_pEdit->ReplaceSel(CString(c), TRUE);

					for (; nStart + 1 < nLen && nStart < 9; nStart++)
					{
						if (!IsValidYearDigit(strText[nStart + 1], nStart - (GetYearStartPosition() - 1)))
						{
							m_pEdit->SetSel(nStart + 1, 10, FALSE);
							CString strPortion;
							for (int iPos = nStart + 1; iPos < nLen && iPos < 10; iPos++)
								strPortion += GetMinYearDigit(iPos - GetYearStartPosition(), false);
							
							m_pEdit->ReplaceSel(strPortion, TRUE);
							m_pEdit->SetSel(nStart + 1, 10, FALSE);
							break;
						}
					}
				}
				else
					Behavior::_OnChar(uChar, nRepCnt, nFlags);

				if (IsValidYear(GetYear()))
				{
					AdjustMaxDay();			// adjust the day first
					AdjustMaxMonthAndDay();	// then adjust the month and the day, if necessary
				}
			}
			break;
		}
	}
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSEdit::DateBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			// If deleting make sure it's the last character or that
			// the selection goes all the way to the end of the text

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();

			if (nEnd != nLen)
			{
				if (!(nEnd == nStart && nEnd == nLen - 1))
					return;
			}
			break;
		}

		case VK_UP:
		{
			// If pressing the UP arrow, increment the corresponding value.

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			if (nStart >= GetYearStartPosition() && nStart <= GetYearStartPosition() + 4)
			{
				int nYear = GetYear();
				if (nYear >= m_dateMin.GetYear() && nYear < m_dateMax.GetYear())
					SetYear(++nYear);
			}

			else if (nStart >= GetMonthStartPosition() && nStart <= GetMonthStartPosition() + 2)
			{
				int nMonth = GetMonth();
				if (nMonth >= GetMinMonth() && nMonth < GetMaxMonth())
					SetMonth(++nMonth);
			}

			else if (nStart >= GetDayStartPosition() && nStart <= GetDayStartPosition() + 2)
			{
				int nDay = GetDay();
				if (nDay >= GetMinDay() && nDay < GetMaxDay())
					SetDay(++nDay);
			}
			
			return;
		}

		case VK_DOWN:
		{
			// If pressing the DOWN arrow, decrement the corresponding value.

			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);

			if (nStart >= GetYearStartPosition() && nStart <= GetYearStartPosition() + 4)
			{
				int nYear = GetYear();
				if (nYear > m_dateMin.GetYear())
					SetYear(--nYear);
			}

			else if (nStart >= GetMonthStartPosition() && nStart <= GetMonthStartPosition() + 2)
			{
				int nMonth = GetMonth();
				if (nMonth > GetMinMonth())
					SetMonth(--nMonth);
			}

			else if (nStart >= GetDayStartPosition() && nStart <= GetDayStartPosition() + 2)
			{
				int nDay = GetDay();
				if (nDay > GetMinDay())
					SetDay(--nDay);
			}
			
			return;
		}
	}

	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
// It's used here to check if any action needs to be taken based on the control's value.
void CAMSEdit::DateBehavior::_OnKillFocus(CWnd* pNewWnd) 
{
	Behavior::_OnKillFocus(pNewWnd);

	// Check if any of the OnKillFocus flags is set
	if (!(m_uFlags & OnKillFocus_Max))
		return;

	CString strText = m_pEdit->GetText();

	// If it's empty, take action based on the flag
	if (strText.IsEmpty())
	{
		if (m_uFlags & OnKillFocus_Beep_IfEmpty)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfEmpty)
			m_pEdit->SetWindowText(_T(" "));

		if ((m_uFlags & OnKillFocus_ShowMessage_IfEmpty) == OnKillFocus_ShowMessage_IfEmpty)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfEmpty)
			m_pEdit->SetFocus();

		return;
	}
		
	if (!IsValid())
	{
		if (m_uFlags & OnKillFocus_Beep_IfInvalid)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfInvalid)
			_Redraw();
		
		if ((m_uFlags & OnKillFocus_ShowMessage_IfInvalid) == OnKillFocus_ShowMessage_IfInvalid)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfInvalid)
			m_pEdit->SetFocus();
	}
}

// Returns the given value as a string with or without leading zeros.
CString CAMSEdit::DateBehavior::GetString(int nValue, bool bTwoDigitWithLeadingZero /*= true*/)
{
	CString strValue;
	if (bTwoDigitWithLeadingZero)
		strValue.Format(_T("%02d"), nValue);
	else
		strValue.Format(_T("%d"), nValue);
	return strValue;
}

// Returns the zero-based position of the month inside the control.
// This is based on whether the month is shown before or after the day.
int CAMSEdit::DateBehavior::GetMonthStartPosition() const
{
	return ((m_uFlags & DayBeforeMonth) ? 3 : 0);
}

// Returns the zero-based position of the day inside the control.
// This is based on whether the day is shown before or after the month.
int CAMSEdit::DateBehavior::GetDayStartPosition() const
{
	return ((m_uFlags & DayBeforeMonth) ? 0 : 3);
}

// Returns the zero-based position of the year inside the control.
int CAMSEdit::DateBehavior::GetYearStartPosition() const
{
	return 6;
}

// Returns the maximum value for the month based on the allowed range.
int CAMSEdit::DateBehavior::GetMaxMonth() const
{
	if (GetValidYear() == m_dateMax.GetYear())
		return m_dateMax.GetMonth();
	return 12;
}

// Returns the minimum value for the month based on the allowed range.
int CAMSEdit::DateBehavior::GetMinMonth() const
{
	if (GetValidYear() == m_dateMin.GetYear())
		return m_dateMin.GetMonth();
	return 1;
}

// Returns the maximum value for the day based on the allowed range.
int CAMSEdit::DateBehavior::GetMaxDay() const
{
	int nYear = GetValidYear();
	int nMonth = GetValidMonth();

	if (nYear == m_dateMax.GetYear() && nMonth == m_dateMax.GetMonth())
		return m_dateMax.GetDay();

	return GetMaxDayOfMonth(nMonth, nYear);
}

// Returns the minimum value for the day based on the allowed range.
int CAMSEdit::DateBehavior::GetMinDay() const
{
	int nYear = GetValidYear();
	int nMonth = GetValidMonth();

	if (nYear == m_dateMin.GetYear() && nMonth == m_dateMin.GetMonth())
		return m_dateMin.GetDay();

	return 1;
}

// Returns the maximum value for the day based on the given month and year.
int CAMSEdit::DateBehavior::GetMaxDayOfMonth(int nMonth, int nYear)
{
	ASSERT(nMonth >= 1 && nMonth <= 12);

	switch (nMonth)
	{
		case 4:
		case 6:
		case 9:
		case 11:
			return 30;

		case 2:
			return IsLeapYear(nYear) ? 29 : 28;
	}
	return 31;
}

// Returns the digit at the given position (0 or 1) for the maximum value of the month allowed.
TCHAR CAMSEdit::DateBehavior::GetMaxMonthDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nYear = GetValidYear();
	int nMaxMonth = m_dateMax.GetMonth();
	int nMaxYear = m_dateMax.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year is at the max, then use the first digit of the max month
		if (nYear == nMaxYear)
			return GetString(nMaxMonth)[0];

		// Otherwise, it's always '1'
		return '1';
	}

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetMonthStartPosition()) ? strText[GetMonthStartPosition()] : '0';
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// If the year is at the max, then check if the first digits match
	if (nYear == nMaxYear && (IsValidYear(GetYear()) || nMaxYear == m_dateMin.GetYear()))
	{
		// If the first digits match, then use the second digit of the max month
		if (GetString(nMaxMonth)[0] == cFirstDigit)
			return GetString(nMaxMonth)[1];

		// Assuming the logic for the first digit is correct, then it must be '0'
		ASSERT(cFirstDigit == '0');
		return '9';  
	}

	// Use the first digit to determine the second digit's max
	return (cFirstDigit == '1' ? '2' : '9');
}

// Returns the digit at the given position (0 or 1) for the minimum value of the month allowed.
TCHAR CAMSEdit::DateBehavior::GetMinMonthDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nYear = GetValidYear();
	int nMinMonth = m_dateMin.GetMonth();
	int nMinYear = m_dateMin.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year is at the min, then use the first digit of the min month
		if (nYear == nMinYear)
			return GetString(nMinMonth)[0];

		// Otherwise, it's always '0'
		return '0';
	}

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetMonthStartPosition()) ? strText[GetMonthStartPosition()] : '0';
	if (!cFirstDigit)
		return '1';

	// If the year is at the max, then check if the first digits match
	if (nYear == nMinYear && (IsValidYear(GetYear()) || nMinYear == m_dateMax.GetYear()))
	{
		// If the first digits match, then use the second digit of the max month
		if (GetString(nMinMonth)[0] == cFirstDigit)
			return GetString(nMinMonth)[1];

		return '0';  
	}

	// Use the first digit to determine the second digit's min
	return (cFirstDigit == '1' ? '0' : '1');
}

// Returns true if the digit at the given position (0 or 1) is within the allowed range for the month.
bool CAMSEdit::DateBehavior::IsValidMonthDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinMonthDigit(nPos) && c <= GetMaxMonthDigit(nPos));
}

// Returns true if the given month is valid and falls within the range.
bool CAMSEdit::DateBehavior::IsValidMonth(int nMonth) const
{
	int nYear = GetValidYear();
	int nDay = GetValidDay();
	return IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0));
}

// Returns the digit at the given position (0 or 1) for the maximum value of the day allowed.
TCHAR CAMSEdit::DateBehavior::GetMaxDayDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nMonth = GetValidMonth();
	int nYear = GetValidYear();
	int nMaxDay = m_dateMax.GetDay();
	int nMaxMonth = m_dateMax.GetMonth();
	int nMaxYear = m_dateMax.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year and month are at the max, then use the first digit of the max day
		if (nYear == nMaxYear && nMonth == nMaxMonth)
			return GetString(nMaxDay)[0];
		return GetString(GetMaxDayOfMonth(nMonth, nYear))[0];
	}

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetDayStartPosition()) ? strText[GetDayStartPosition()] : '0';
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// If the year and month are at the max, then use the second digit of the max day
	if (nYear == nMaxYear && nMonth == nMaxMonth && GetString(nMaxDay)[0] == cFirstDigit)
		return GetString(nMaxDay)[1];

	if (cFirstDigit == '0' || 
		cFirstDigit == '1' || 
		(cFirstDigit == '2' && nMonth != 2) || 
		(nMonth == 2 && !IsValidYear(GetYear())))
		return '9';
	return GetString(GetMaxDayOfMonth(nMonth, nYear))[1];
}

// Returns the digit at the given position (0 or 1) for the minimum value of the day allowed.
TCHAR CAMSEdit::DateBehavior::GetMinDayDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	int nMonth = GetValidMonth();
	int nYear = GetValidYear();
	int nMinDay = m_dateMin.GetDay();
	int nMinMonth = m_dateMin.GetMonth();
	int nMinYear = m_dateMin.GetYear();

	// First digit
	if (nPos == 0)
	{
		// If the year and month are at the min, then use the first digit of the min day
		if (nYear == nMinYear && nMonth == nMinMonth)
			return GetString(nMinDay)[0];
		return '0';
	}

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetDayStartPosition()) ? strText[GetDayStartPosition()] : '0';
	if (!cFirstDigit)  // must have a valid first digit at this point
		return '1';

	// If the year and month are at the max, then use the first second of the max day
	if (nYear == nMinYear && nMonth == nMinMonth && GetString(nMinDay)[0] == cFirstDigit)
		return GetString(nMinDay)[1];

	// Use the first digit to determine the second digit's min
	return (cFirstDigit == '0' ? '1' : '0');
}

// Returns true if the digit at the given position (0 or 1) is within the allowed range for the day.
bool CAMSEdit::DateBehavior::IsValidDayDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinDayDigit(nPos) && c <= GetMaxDayDigit(nPos));
}

// Returns true if the given day is valid and falls within the range.
bool CAMSEdit::DateBehavior::IsValidDay(int nDay) const
{
	return IsValid(COleDateTime(GetValidYear(), GetValidMonth(), nDay, 0, 0, 0));
}

// Returns true if the given year is valid and falls within the range.
bool CAMSEdit::DateBehavior::IsValidYear(int nYear) const
{
	return (nYear >= m_dateMin.GetYear() && nYear <= m_dateMax.GetYear());
}

// Adjusts the month (to the minimum) if not valid; otherwise adjusts the day (to the maximum) if not valid.
bool CAMSEdit::DateBehavior::AdjustMaxMonthAndDay()
{
	int nMonth = GetMonth();	
	if (nMonth && !IsValidMonth(nMonth))
	{
		SetMonth(GetMinMonth());  // this adjusts the day automatically
		return true;
	}

	return AdjustMaxDay();
}

// Adjusts the day (to the maximum) if not valid.
bool CAMSEdit::DateBehavior::AdjustMaxDay()
{
	int nDay = GetDay();
	if (nDay && !IsValidDay(nDay))
	{
		SetDay(GetMaxDay());
		return true;
	}
	
	return false;	// nothing had to be adjusted
}

// Returns the digit at the given position (0 to 3) for the maximum value of the year allowed.
TCHAR CAMSEdit::DateBehavior::GetMaxYearDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 3);

	CString strYear = GetString(GetYear(), false);
	CString strMaxYear = GetString(m_dateMax.GetYear(), false);

	if (nPos == 0 || _ttoi(strMaxYear.Left(nPos)) <= _ttoi(strYear.Left(nPos)))
		return strMaxYear[nPos];
	return '9';
}

// Returns the digit at the given position (0 to 3) for the minimum value of the year allowed.
// If bValidYear is true, the current year is made sure to be valid.
TCHAR CAMSEdit::DateBehavior::GetMinYearDigit(int nPos, bool bValidYear /*= false*/) const
{
	ASSERT(nPos >= 0 && nPos <= 3);

	int nYear = GetYear();
	if (!IsValidYear(nYear) && bValidYear)
		nYear = GetValidYear();

	CString strYear = GetString(nYear, false);
	CString strMinYear = GetString(m_dateMin.GetYear(), false);

	if (nPos == 0 || _ttoi(strMinYear.Left(nPos)) >= _ttoi(strYear.Left(nPos)))
		return strMinYear[nPos];
	return '0';
}

// Returns true if the digit at the given position (0 to 3) is within the allowed range for the year.
bool CAMSEdit::DateBehavior::IsValidYearDigit(TCHAR c, int nPos) const
{
	return (c >= GetMinYearDigit(nPos) && c <= GetMaxYearDigit(nPos));
}

// Returns the month currently shown on the control or 0.
int CAMSEdit::DateBehavior::GetMonth() const
{
	CString strText = m_pEdit->GetText();

	int nStartPos = GetMonthStartPosition();
	if (strText.GetLength() >= nStartPos + 2)
		return _ttoi(strText.Mid(nStartPos, nStartPos + 2));
	return 0;
}

// Returns the current month as a valid value.  
// If it is less than the minimum allowed, the minimum is returned; 
// if it is more than the maximum allowed, the maximum is returned.
int CAMSEdit::DateBehavior::GetValidMonth() const
{
	int nMonth = GetMonth();
	
	// It it's outside the range, fix it
	if (nMonth < GetMinMonth())
		nMonth = GetMinMonth();
	else if (nMonth > GetMaxMonth())
		nMonth = GetMaxMonth();

	return nMonth;
}

// Returns the day currently shown on the control or 0.
int CAMSEdit::DateBehavior::GetDay() const
{
	CString strText = m_pEdit->GetText();

	int nStartPos = GetDayStartPosition();
	if (strText.GetLength() >= nStartPos + 2)
		return _ttoi(strText.Mid(nStartPos, nStartPos + 2));
	return 0;
}

// Returns the current day as a valid value.  
// If it is less than the minimum allowed, the minimum is returned; 
// if it is more than the maximum allowed, the maximum is returned.
int CAMSEdit::DateBehavior::GetValidDay() const
{
	int nDay = GetDay();

	// It it's outside the range, fix it
	if (nDay < GetMinDay())
		nDay = GetMinDay();
	else if (nDay > GetMaxDay())
		nDay = GetMaxDay();

	return nDay;
}

// Returns the year currently shown on the control or 0.
int CAMSEdit::DateBehavior::GetYear() const
{
	CString strText = m_pEdit->GetText();

	int nSlash = strText.ReverseFind(m_cSep);
	if (nSlash > 0)
		return _ttoi(strText.Mid(nSlash + 1));
	return 0;
}

// Returns the current year as a valid value.  
// If it is less than the minimum allowed, the minimum is returned; 
// if it is more than the maximum allowed, the maximum is returned.
int CAMSEdit::DateBehavior::GetValidYear() const
{
	int nYear = GetYear();
	if (nYear < m_dateMin.GetYear())
	{
		nYear = COleDateTime::GetCurrentTime().GetYear();
		if (nYear < m_dateMin.GetYear())
			nYear = m_dateMin.GetYear();
	}
	if (nYear > m_dateMax.GetYear())
		nYear = m_dateMax.GetYear();

	return nYear;
}

// Sets the control's month to the given value, which must be valid.
void CAMSEdit::DateBehavior::SetMonth(int nMonth)
{
	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetMonth() > 0)		// see if there's already a month
		m_pEdit->SetSel(GetMonthStartPosition(), GetMonthStartPosition() + 3);

	CString strText;
	strText.Format(_T("%02d%c"), nMonth, m_cSep);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the month

	AdjustMaxDay();	// adjust the day if it's out of range
	ASSERT(IsValidMonth(nMonth));
}

// Sets the control's day to the given value, which must be valid.
void CAMSEdit::DateBehavior::SetDay(int nDay)
{
	ASSERT(IsValidDay(nDay));

	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetDay() > 0)		// see if there's already a day
		m_pEdit->SetSel(GetDayStartPosition(), GetDayStartPosition() + 3);

	CString strText;
	strText.Format(_T("%02d%c"), nDay, m_cSep);

	m_pEdit->ReplaceSel(strText, TRUE);	// set the day
}

// Sets the control's year to the given value, which must be valid.
void CAMSEdit::DateBehavior::SetYear(int nYear)
{
	ASSERT(IsValidYear(nYear));

	SelectionSaver selection = m_pEdit;	// remember the current selection

	if (GetYear() > 0)		// see if there's already a year
		m_pEdit->SetSel(GetYearStartPosition(), GetYearStartPosition() + 4);

	CString strText;
	strText.Format(_T("%4d"), nYear);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the year
	
	AdjustMaxMonthAndDay();	// adjust the month and/or day if they're out of range
}

// Returns the date on the control as a CTime object.
CTime CAMSEdit::DateBehavior::GetDate() const
{
	return CTime(GetYear(), GetMonth(), GetDay(), 0, 0, 0);
}

// Returns the date on the control as a COleDateTime object.
COleDateTime CAMSEdit::DateBehavior::GetOleDate() const
{
	return COleDateTime(GetYear(), GetMonth(), GetDay(), 0, 0, 0);
}

// Sets the month, day, and year on the control to the given values, which must be valid.
void CAMSEdit::DateBehavior::SetDate(int nYear, int nMonth, int nDay)
{
	ASSERT(IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0))); 
	m_pEdit->SetWindowText(GetFormattedDate(nYear, nMonth, nDay));
}

// Sets the month, day, and year on the control based on the given CTime object, which must be valid.
void CAMSEdit::DateBehavior::SetDate(const CTime& date)
{
	SetDate(date.GetYear(), date.GetMonth(), date.GetDay());
}

// Sets the month, day, and year on the control based on the given COleDateTime object, which must be valid.
void CAMSEdit::DateBehavior::SetDate(const COleDateTime& date)
{
	SetDate(date.GetYear(), date.GetMonth(), date.GetDay());
}

// Sets the month, day, and year on the control to today's date.
void CAMSEdit::DateBehavior::SetDateToToday()
{
	SetDate(COleDateTime::GetCurrentTime());
}

// Returns true if the given year is a leap year.
bool CAMSEdit::DateBehavior::IsLeapYear(int nYear)
{
	return (nYear & 3) == 0 && (nYear % 100 != 0 || nYear % 400 == 0); 
}

// Returns true if the control's date is valid and falls within the allowed range.
bool CAMSEdit::DateBehavior::IsValid() const
{
	return IsValid(COleDateTime(GetYear(), GetMonth(), GetDay(), 0, 0, 0));
}

// Returns true if the control's date is valid and falls within the allowed range.
// If bShowErrorIfNotValid is true, an error message box is shown and the control gets the focus.
bool CAMSEdit::DateBehavior::CheckIfValid(bool bShowErrorIfNotValid /*= true*/)
{
	if (!m_pEdit->IsWindowEnabled())
		return true;

	bool bValid = IsValid();
	if (!bValid && bShowErrorIfNotValid)
	{
		ShowErrorMessage();
		m_pEdit->SetFocus();
	}
	
	return bValid;
}

// Shows a message box informing the user to enter a valid date within the allowed range.
void CAMSEdit::DateBehavior::ShowErrorMessage() const
{
	AfxMessageBox(_T("Please specify a date between ") + GetFormattedDate(m_dateMin.GetYear(), m_dateMin.GetMonth(), m_dateMin.GetDay()) + _T(" and ") + GetFormattedDate(m_dateMax.GetYear(), m_dateMax.GetMonth(), m_dateMax.GetDay()) + '.', MB_ICONEXCLAMATION);
}

// Sets the range of allowed date values to the given minimum and maximum CTime values.
void CAMSEdit::DateBehavior::SetRange(const CTime& dateMin, const CTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_CTIME);
	ASSERT(dateMax <= AMS_MAX_CTIME);
	ASSERT(dateMin <= dateMax);

	m_dateMin.SetDate(dateMin.GetYear(), dateMin.GetMonth(), dateMin.GetDay());
	m_dateMax.SetDate(dateMax.GetYear(), dateMax.GetMonth(), dateMax.GetDay());

	_Redraw();
}

// Sets the range of allowed date values to the given minimum and maximum COleDateTime values.
void CAMSEdit::DateBehavior::SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_OLEDATETIME);
	ASSERT(dateMax <= AMS_MAX_OLEDATETIME);
	ASSERT(dateMin <= dateMax);

	m_dateMin = dateMin;
	m_dateMax = dateMax;
	_Redraw();
}

// Retrieves the range of allowed date values inside the given set of CTime pointers.
void CAMSEdit::DateBehavior::GetRange(CTime* pDateMin, CTime* pDateMax) const
{
	if (pDateMin)
		*pDateMin = CTime(m_dateMin.GetYear(), m_dateMin.GetMonth(), m_dateMin.GetDay(), 0, 0, 0);
	if (pDateMax)
		*pDateMax = CTime(m_dateMax.GetYear(), m_dateMax.GetMonth(), m_dateMax.GetDay(), 0, 0, 0);
}

// Retrieves the range of allowed date values inside the given set of COleDateTime pointers.
void CAMSEdit::DateBehavior::GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const
{
	if (pDateMin)
		*pDateMin = m_dateMin;
	if (pDateMax)
		*pDateMax = m_dateMax;
}

// Returns true if the given date is valid and falls within the range.
bool CAMSEdit::DateBehavior::IsValid(const COleDateTime& date, bool bDateOnly /*= true*/) const
{
	return (date.GetStatus() == COleDateTime::valid && date >= m_dateMin && date <= m_dateMax);
}

// Sets the character used to separate the month, day, and year values.
void CAMSEdit::DateBehavior::SetSeparator(TCHAR cSep)
{
	ASSERT(cSep);
	ASSERT(!_istdigit(cSep));

	if (m_cSep != cSep)
	{
		m_cSep = cSep;
		_Redraw();
	}
}

// Returns the character used to separate the month, day, and year values.
TCHAR CAMSEdit::DateBehavior::GetSeparator() const
{
	return m_cSep;
}

// Sets whether the day should be shown before the month or after it.
void CAMSEdit::DateBehavior::ShowDayBeforeMonth(bool bDayBeforeMonth /*= true*/)
{
	ModifyFlags(bDayBeforeMonth ? DayBeforeMonth : 0, bDayBeforeMonth ? 0 : DayBeforeMonth);
}

// Returns true if the day will be shown before the month (instead of after it).
bool CAMSEdit::DateBehavior::IsDayShownBeforeMonth() const
{
	return (m_uFlags & DayBeforeMonth) ? true : false;
}

// Returns the control's value in a valid format.
CString CAMSEdit::DateBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();

	if (strText.IsEmpty())
		return strText;

	if (IsValid())
		return GetFormattedDate(GetYear(), GetMonth(), GetDay());

	// If the date is empty, try using today
	if (GetYear() == 0 && GetMonth() == 0 && GetDay() == 0)
		((CAMSEdit::DateBehavior*)this)->SetDateToToday();

	int nYear = GetValidYear();
	int nMonth = GetValidMonth();
	int nDay = GetValidDay();

	if (!IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0)))
		nMonth = GetMinMonth();

	if (!IsValid(COleDateTime(nYear, nMonth, nDay, 0, 0, 0)))
		nDay = GetMaxDay();

	return GetFormattedDate(nYear, nMonth, nDay);
}

// Formats the given year, month, and day values into a string based on the proper format.
CString CAMSEdit::DateBehavior::GetFormattedDate(int nYear, int nMonth, int nDay) const
{
	CString strText;	
	if (m_uFlags & DayBeforeMonth)
		strText.Format(_T("%02d%c%02d%c%4d"), nDay, m_cSep, nMonth, m_cSep, nYear);
	else
		strText.Format(_T("%02d%c%02d%c%4d"), nMonth, m_cSep, nDay, m_cSep, nYear);

	return strText;
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::TimeBehavior

#define AMS_DEFAULT_DATE		1899, 12, 30

// Constructs the object with the given control.
CAMSEdit::TimeBehavior::TimeBehavior(CAMSEdit* pEdit) :
	Behavior(pEdit),
	m_timeMin(AMS_DEFAULT_DATE, 0, 0, 0),
	m_timeMax(AMS_DEFAULT_DATE, 23, 59, 59),
	m_cSep(':'),
	m_strAM(AMS_AM_SYMBOL),
	m_strPM(AMS_PM_SYMBOL),
	m_nAMPMLength(m_strAM.GetLength()),
	m_nHourStart(0)
{
	// Get the system's time separator
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, NULL, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_STIME, &m_cSep, sizeof(m_cSep));

	// Determine if it's in 24-hour format
	TCHAR szBuffer[3];
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, NULL, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ITIME, szBuffer, sizeof(szBuffer));
		if (_ttoi(szBuffer))
			m_uFlags |= TwentyFourHourFormat;
	}

	// Get the AM symbol
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, NULL, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S1159, szBuffer, sizeof(szBuffer));
		m_strAM = szBuffer;
	}

	// Get the PM symbol
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, NULL, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_S2359, szBuffer, sizeof(szBuffer));
		m_strPM = szBuffer;
	}

	m_nAMPMLength = m_strAM.GetLength();

	// Verify the lengths are the same; otherwise use the default
	if (m_nAMPMLength == 0 || m_nAMPMLength != m_strPM.GetLength())
	{
		m_strAM = AMS_AM_SYMBOL;
		m_strPM = AMS_PM_SYMBOL;
		m_nAMPMLength = m_strAM.GetLength();
	}
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::TimeBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	TCHAR c = static_cast<TCHAR>(uChar);
	
	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);
	
	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();
	
	// Check for a non-printable character (such as Ctrl+C)
	if (!_istprint(c))
	{
		if (c == VK_BACK && nStart != nLen)
		{
			m_pEdit->SendMessage(WM_KEYDOWN, VK_LEFT); // move the cursor left
			return;
		}
		
		// Allow backspace only if the cursor is all the way to the right
		if (_ShouldEnter(c))
			Behavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}
	
	// Add the digit depending on its location
	if (nStart == m_nHourStart)		// FIRST DIGIT
	{
		if (IsValidHourDigit(c, 0) && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
				
				if (nLen > nStart + 1)
				{
					// If the second digit is no longer valid, correct and select it
					if (!IsValidHour(GetHour()))
					{
						m_pEdit->SetSel(nStart + 1, nStart + 2);
						m_pEdit->ReplaceSel(CString(GetMinHourDigit(1)), TRUE);
						m_pEdit->SetSel(nStart + 1, nStart + 2);
					}
				}
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		else if (nLen == nStart && IsValidHourDigit(c, 1) && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nStart + 2);
			m_pEdit->ReplaceSel(CString('0') + c, TRUE);					
		}
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}

	else if (nStart == m_nHourStart + 1)	// SECOND DIGIT
	{
		if (IsValidHourDigit(c, 1) && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);					
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		else if (c == m_cSep && nLen == nStart && IsValidHour(_ttoi(CString('0') + strText[m_nHourStart])) && _ShouldEnter(c))
		{
			m_pEdit->SetSel(m_nHourStart, nStart);
			m_pEdit->ReplaceSel(CString('0') + strText[m_nHourStart] + c, TRUE);					
		}
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
	
	else if (nStart == m_nHourStart + 2)	// FIRST COLON
	{
		int nColon = 0;
		if (c == m_cSep)
			nColon = 1;
		else
			nColon = (IsValidMinuteDigit(c, 0) ? 2 : 0);
		
		// If we need the colon, enter it
		if (nColon && _ShouldEnter(c))
		{
			m_pEdit->SetSel(nStart, nStart + 1, FALSE);
			m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
		}
		
		// If the colon is to be preceded by a valid digit, "type" it in.
		if (nColon == 2)
			keybd_event((BYTE)c, 0, 0, 0);
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
			
	else if (nStart == m_nHourStart + 3)	// THIRD DIGIT
	{
		if (IsValidMinuteDigit(c, 0) && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
				
				if (nLen > nStart + 1)
				{
					if (!IsValidMinute(GetMinute()))
					{
						m_pEdit->SetSel(nStart + 1, nStart + 2);
						m_pEdit->ReplaceSel(CString(GetMinMinuteDigit(1)), TRUE);
						m_pEdit->SetSel(nStart + 1, nStart + 2);
					}
				}
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
		}
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
	
	else if (nStart == m_nHourStart + 4)	// FOURTH DIGIT
	{
		if (IsValidMinuteDigit(c, 1) && _ShouldEnter(c))
		{
			if (nLen > nStart)
			{
				m_pEdit->SetSel(nStart, nStart + 1);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			else
				Behavior::_OnChar(uChar, nRepCnt, nFlags);
			
			// Show the AM/PM symbol if we're not showing seconds
			if (!IsShowingSeconds())
				ShowAMPM();
		}				
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
			
	else if (nStart == m_nHourStart + 5)	// SECOND COLON	OR FIRST SPACE (seconds' first digit or AM/PM)
	{
		if (IsShowingSeconds())
		{
			int nColon = 0;
			if (c == m_cSep)
				nColon = 1;
			else
				nColon = (IsValidSecondDigit(c, 0) ? 2 : 0);
			
			// If we need the slash, enter it
			if (nColon && _ShouldEnter(c))
			{					
				int nReplace = (nStart < nLen && strText[nStart] != ' ');
				m_pEdit->SetSel(nStart, nStart + nReplace, FALSE);
				m_pEdit->ReplaceSel(CString(m_cSep), TRUE);
			}
			
			// If the colon is to be preceded by a valid digit, "type" it in.
			if (nColon == 2)
				keybd_event((BYTE)c, 0, 0, 0);
		}
		else if (!IsShowing24HourFormat())
		{
			if (c == ' ')
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(c), TRUE);
			}
			ShowAMPM();
		}

		ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}

	else if (nStart == m_nHourStart + 6)	// FIFTH DIGIT - first digit of seconds or AM/PM
	{
		if (IsShowingSeconds())
		{
			if (IsValidSecondDigit(c, 0) && _ShouldEnter(c))
			{
				if (nLen > nStart)
				{
					int nReplace = (nStart < nLen && strText[nStart] != ' ');
					m_pEdit->SetSel(nStart, nStart + nReplace, FALSE);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				else
					Behavior::_OnChar(uChar, nRepCnt, nFlags);
			}
		}
		
		ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}

	else if (nStart == m_nHourStart + 7)	// SIXTH DIGIT - second digit of seconds or AM/PM
	{
		if (IsShowingSeconds())
		{
			if (IsValidSecondDigit(c, 1) && _ShouldEnter(c))
			{
				if (nLen > nStart)
				{
					int nReplace = (nStart < nLen && strText[nStart] != ' ');
					m_pEdit->SetSel(nStart, nStart + nReplace, FALSE);
					m_pEdit->ReplaceSel(CString(c), TRUE);
				}
				else
					Behavior::_OnChar(uChar, nRepCnt, nFlags);

				// Show the AM/PM symbol if we're not in 24-hour format
				ShowAMPM();
			}
		}

		ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
		
	else if (nStart == m_nHourStart + 8)	// FIRST SPACE (with seconds showing)
	{
		if (IsShowingSeconds() && !IsShowing24HourFormat())
		{
			if (c == ' ')
			{
				m_pEdit->SetSel(nStart, nStart + 1, FALSE);
				m_pEdit->ReplaceSel(CString(c), TRUE);
				ShowAMPM();
			}
		} 

		ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}

	else 		// AM/PM
		ChangeAMPM(c);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSEdit::TimeBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	switch (uChar)
	{
		case VK_DELETE:
		{
			// If deleting make sure it's the last character or that
			// the selection goes all the way to the end of the text
			
			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);
			
			CString strText = m_pEdit->GetText();
			int nLen = strText.GetLength();
			
			if (nEnd != nLen)
			{
				if (!(nEnd == nStart && nEnd == nLen - 1))
					return;
			}
			break;
		}
			
		case VK_UP:
		{
			// If pressing the UP arrow, increment the corresponding value.
			
			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);
			
			if (nStart >= GetHourStartPosition() && nStart <= GetHourStartPosition() + 2)
			{
				int nHour = GetHour();
				if (nHour >= GetMinHour())
				{
					// Handle moving up through the noon hour
					CString strAMPM = GetAMPM();
					if (IsValidAMPM(strAMPM))
					{
						if (nHour == 11)
						{
							if (strAMPM == m_strPM)  // stop at midnight
								return;
							SetAMPM(false); 
						}
						else if (nHour == 12)
							nHour = 0;
					}

					if (nHour < GetMaxHour())
						SetHour(++nHour);
				}
			}			
			else if (nStart >= GetMinuteStartPosition() && nStart <= GetMinuteStartPosition() + 2)
			{
				int nMinute = GetMinute();
				if (nMinute >= GetMinMinute() && nMinute < GetMaxMinute())
					SetMinute(++nMinute);
			}
			else if (nStart >= GetAMPMStartPosition() && nStart <= GetAMPMStartPosition() + m_nAMPMLength)
			{
				CString strAMPM = GetAMPM();
				SetAMPM(!IsValidAMPM(strAMPM) || strAMPM == m_strPM);
			}
			else if (nStart >= GetSecondStartPosition() && nStart <= GetSecondStartPosition() + 2)
			{
				int nSecond = GetSecond();
				if (nSecond >= GetMinSecond() && nSecond < GetMaxSecond())
					SetSecond(++nSecond);
			}

			return;
		}
			
		case VK_DOWN:
		{
			// If pressing the DOWN arrow, decrement the corresponding value.
			
			int nStart, nEnd;
			m_pEdit->GetSel(nStart, nEnd);
			
			if (nStart >= GetHourStartPosition() && nStart <= GetHourStartPosition() + 2)
			{
				int nHour = GetHour();
				if (nHour <= GetMaxHour())
				{
					// Handle moving up through the noon hour
					CString strAMPM = GetAMPM();
					if (IsValidAMPM(strAMPM))
					{
						if (nHour == 12)
						{
							if (strAMPM == m_strAM)	// stop at midnight
								return;
							SetAMPM(true);
						}
						else if (nHour == 1)
							nHour = 13;
					}

					if (nHour > GetMinHour())
						SetHour(--nHour);
				}
			}			
			else if (nStart >= GetMinuteStartPosition() && nStart <= GetMinuteStartPosition() + 2)
			{
				int nMinute = GetMinute();
				if (nMinute > GetMinMinute() && nMinute <= GetMaxMinute())
					SetMinute(--nMinute);
			}
			else if (nStart >= GetAMPMStartPosition() && nStart <= GetAMPMStartPosition() + m_nAMPMLength)
			{
				CString strAMPM = GetAMPM();
				SetAMPM(!IsValidAMPM(strAMPM) || strAMPM == m_strPM);
			}
			else if (nStart >= GetSecondStartPosition() && nStart <= GetSecondStartPosition() + 2)
			{
				int nSecond = GetSecond();
				if (nSecond > GetMinSecond() && nSecond <= GetMaxSecond())
					SetSecond(--nSecond);
			}
			return;
		}
	}
	
	Behavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
// It's used here to check if any action needs to be taken based on the control's value.
void CAMSDateEdit::TimeBehavior::_OnKillFocus(CWnd* pNewWnd) 
{
	Behavior::_OnKillFocus(pNewWnd);

	// Check if any of the OnKillFocus flags is set
	if (!(m_uFlags & OnKillFocus_Max))
		return;

	CString strText = m_pEdit->GetText();

	// If it's empty, take action based on the flag
	if (strText.IsEmpty())
	{
		if (m_uFlags & OnKillFocus_Beep_IfEmpty)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfEmpty)
			AdjustWithinRange();

		if ((m_uFlags & OnKillFocus_ShowMessage_IfEmpty) == OnKillFocus_ShowMessage_IfEmpty)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfEmpty)
			m_pEdit->SetFocus();

		return;
	}
		
	if (!IsValid())
	{
		if (m_uFlags & OnKillFocus_Beep_IfInvalid)
			MessageBeep(MB_ICONEXCLAMATION);
			
		if (m_uFlags & OnKillFocus_SetValid_IfInvalid)
			AdjustWithinRange();
		
		if ((m_uFlags & OnKillFocus_ShowMessage_IfInvalid) == OnKillFocus_ShowMessage_IfInvalid)
			ShowErrorMessage();		

		if (m_uFlags & OnKillFocus_SetFocus_IfInvalid)
			m_pEdit->SetFocus();
	}
}

// Returns the zero-based position of the hour inside the control.
int CAMSEdit::TimeBehavior::GetHourStartPosition() const
{
	return m_nHourStart;
}

// Returns the zero-based position of the minute inside the control.
int CAMSEdit::TimeBehavior::GetMinuteStartPosition() const
{
	return m_nHourStart + 3;
}

// Returns the zero-based position of the second inside the control.
int CAMSEdit::TimeBehavior::GetSecondStartPosition() const
{
	return m_nHourStart + 6;
}

// Returns the zero-based position of the AM/PM symbols inside the control.
// This is based on whether the seconds are being shown or not.
int CAMSEdit::TimeBehavior::GetAMPMStartPosition() const
{
    return m_nHourStart + (IsShowingSeconds() ? 9 : 6);
}

// Returns the maximum value for the hour based on whether we're in 24-Hour format (or b24HourFormat is true) or not.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMaxHour(bool b24HourFormat /*= false*/) const
{
	return (b24HourFormat || IsShowing24HourFormat()) ? 23 : 12;
}

// Returns the minimum value for the hour based on whether we're in 24-Hour format (or b24HourFormat is true) or not.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMinHour(bool b24HourFormat /*= false*/) const
{
	return (b24HourFormat || IsShowing24HourFormat()) ? 0 : 1;
}

// Returns the maximum value for the minute.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMaxMinute() const
{
	return 59;
}

// Returns the minimum value for the minute.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMinMinute() const
{
	return 0;
}

// Returns the maximum value for the second.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMaxSecond() const
{
	return 59;
}

// Returns the minimum value for the second.
// Note: This value is not based on the allowed range.
int CAMSEdit::TimeBehavior::GetMinSecond() const
{
	return 0;
}

// Returns the digit at the given position (0 or 1) for the maximum value of the hour.
TCHAR CAMSEdit::TimeBehavior::GetMaxHourDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	// First digit
	if (nPos == 0)
		return IsShowing24HourFormat() ? '2' : '1';

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetHourStartPosition()) ? strText[GetHourStartPosition()] : '0';
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// Use the first digit to determine the second digit's max
	if (cFirstDigit == '2')
		return '3';
	if (cFirstDigit == '1' && !IsShowing24HourFormat())
		return '2';
	return '9';
}

// Returns the digit at the given position (0 or 1) for the minimum value of the hour.
TCHAR CAMSEdit::TimeBehavior::GetMinHourDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);

	// First digit
	if (nPos == 0)
		return '0';

	// Second digit
	CString strText = m_pEdit->GetText();
	TCHAR cFirstDigit = (strText.GetLength() > GetHourStartPosition()) ? strText[GetHourStartPosition()] : '0';
	ASSERT(cFirstDigit);  // must have a valid first digit at this point

	// If the first digit is a 0 and we're not in 24-hour format, don't allow 0
	if (cFirstDigit == '0' && !IsShowing24HourFormat())
		return '1';

	// For all other cases it's always 0
	return '0';
}

// Returns true if the digit at the given position (0 or 1) is valid for the hour.
bool CAMSEdit::TimeBehavior::IsValidHourDigit(TCHAR c, int nPos) const
{
    return (c >= GetMinHourDigit(nPos) && c <= GetMaxHourDigit(nPos));
}

// Returns true if the given hour is valid.
// If b24HourFormat is true, the value is checked from 0 to 23 regardless of the format currently being shown.
bool CAMSEdit::TimeBehavior::IsValidHour(int nHour, bool b24HourFormat /*= false*/) const
{
	return (nHour >= GetMinHour(b24HourFormat) && nHour <= GetMaxHour(b24HourFormat));
}

// Returns the digit at the given position (0 or 1) for the maximum value of the minute.
TCHAR CAMSEdit::TimeBehavior::GetMaxMinuteDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);
	return (nPos == 0 ? '5' : '9');
}

// Returns the digit at the given position (0 or 1) for the maximum value of the minute.
TCHAR CAMSEdit::TimeBehavior::GetMinMinuteDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);	
    return '0';
}

// Returns true if the digit at the given position (0 or 1) is valid for the minute.
bool CAMSEdit::TimeBehavior::IsValidMinuteDigit(TCHAR c, int nPos) const
{
    return (c >= GetMinMinuteDigit(nPos) && c <= GetMaxMinuteDigit(nPos));
}

// Returns true if the given minute is valid.
bool CAMSEdit::TimeBehavior::IsValidMinute(int nMinute) const
{
	return (nMinute >= GetMinMinute() && nMinute <= GetMaxMinute());
}

// Returns the digit at the given position (0 or 1) for the maximum value of the second.
TCHAR CAMSEdit::TimeBehavior::GetMaxSecondDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);
	return (nPos == 0 ? '5' : '9');
}

// Returns the digit at the given position (0 or 1) for the maximum value of the second.
TCHAR CAMSEdit::TimeBehavior::GetMinSecondDigit(int nPos) const
{
	ASSERT(nPos >= 0 && nPos <= 1);	
    return '0';
}

// Returns true if the digit at the given position (0 or 1) is valid for the second.
bool CAMSEdit::TimeBehavior::IsValidSecondDigit(TCHAR c, int nPos) const
{
    return (c >= GetMinSecondDigit(nPos) && c <= GetMaxSecondDigit(nPos));
}

// Returns true if the given second is valid.
bool CAMSEdit::TimeBehavior::IsValidSecond(int nSecond) const
{
	return (nSecond >= GetMinSecond() && nSecond <= GetMaxSecond());
}

// Shows the AM symbol if not in 24-hour format and they're not already shown.
void CAMSEdit::TimeBehavior::ShowAMPM()
{
	if (!IsShowing24HourFormat() && !IsValidAMPM(GetAMPM()))
		SetAMPM(true);
}

// Sets the AM or PM symbol if not in 24-hour format.
void CAMSEdit::TimeBehavior::SetAMPM(bool bAM)
{
	if (IsShowing24HourFormat())
		return;

	SelectionSaver selection = m_pEdit;	// remember the current selection
	m_pEdit->SetSel(GetAMPMStartPosition() - 1, GetAMPMStartPosition() + m_nAMPMLength);
	
	CString strText;
	strText.Format(_T(" %s"), bAM ? m_strAM : m_strPM);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the AM/PM
}

// Changes the AM/PM symbol based on the given character (entered by the user).
bool CAMSEdit::TimeBehavior::ChangeAMPM(TCHAR c)
{
	if (IsShowing24HourFormat())
		return false;

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	int nPos = GetAMPMPosition(strText);
	if (nPos == 0)
		return false;

	int nStart, nEnd;
	m_pEdit->GetSel(nStart, nEnd);

	CString strAMPM = GetAMPM();
	TCHAR cUpper = _totupper(c);

	switch (cUpper)
	{
		case 'A':
		case 'P':
			SetAMPM(cUpper == 'A');

			if (cUpper == (TCHAR)_totupper(m_strAM[0]) || cUpper == (TCHAR)_totupper(m_strPM[0]))
			{
				// Move the cursor right, if we're in front of the AM/PM symbols
				if (nStart == nPos)
					m_pEdit->PostMessage(WM_KEYDOWN, VK_RIGHT);

				// Move the cursor right twice, if we're in front of the space in front of the AM/PM symbols
				if (nStart + 1 == nPos)
				{
					m_pEdit->PostMessage(WM_KEYDOWN, VK_RIGHT);
					m_pEdit->PostMessage(WM_KEYDOWN, VK_RIGHT);
				}
			}
			return true;

		default:
			// Handle entries after the first character of the AM/PM symbol -- allow the user to enter each character
			if (nStart > nPos)
			{
				// Check if we're adding a character of the AM/PM symbol (after the first one)
				if ((nLen == nStart && !IsValidAMPM(strAMPM)) || (nLen == nEnd && nEnd != nStart))
				{
					const CString& strAMPMToUse = _totupper(strText[nPos]) == _totupper(m_strAM[0]) ? m_strAM : m_strPM;
					if (cUpper == (TCHAR)_totupper(strAMPMToUse[nStart - nPos]))
					{
						m_pEdit->ReplaceSel(strAMPMToUse.Mid(nStart - nPos), TRUE);	// set the rest of the AM/PM
						m_pEdit->SetSel(nStart, nStart, TRUE);  // Reset the selection so that the cursor can be moved
						return ChangeAMPM(c); // move the cursor (below)
					}
				}

				// Check if the AM/PM symbol is OK and we just need to move over one
				if (nLen > nStart && nEnd == nStart && cUpper == (TCHAR)_totupper(strText[nStart]))
				{
					m_pEdit->PostMessage(WM_KEYDOWN, VK_RIGHT);
					return true;
				}
			}
	}

	return false;
}

// Returns the zero-based position of the AM/PM symbol shown on the control.
int CAMSEdit::TimeBehavior::GetAMPMPosition(const CString& strText) const
{
	int nPos = strText.Find(' ' + m_strAM);
	return ((nPos < 0) ? strText.Find(' ' + m_strPM) : nPos) + 1;
}

// Returns true if the given AM/PM symbol is valid.
bool CAMSEdit::TimeBehavior::IsValidAMPM(const CString& strAMPM) const
{
	return (strAMPM == m_strAM || strAMPM == m_strPM);
}

// Returns the hour currently shown on the control or -1.
int CAMSEdit::TimeBehavior::GetHour() const
{
	CString strText = m_pEdit->GetText();
	
	int nStartPos = GetHourStartPosition();
	if (strText.GetLength() >= nStartPos + 2)
		return _ttoi(strText.Mid(nStartPos, nStartPos + 2));

	return -1;
}

// Returns the current hour as a valid value.  
int CAMSEdit::TimeBehavior::GetValidHour(bool b24HourFormat /*= false*/) const
{
	int nHour = GetHour();
	
	// It it's outside the range, fix it
	if (nHour < GetMinHour(b24HourFormat))
		nHour = GetMinHour(b24HourFormat);
	else if (nHour > GetMaxHour(b24HourFormat))
		nHour = GetMaxHour(b24HourFormat);
	
	return nHour;
}

// Returns the minute currently shown on the control or -1.
int CAMSEdit::TimeBehavior::GetMinute() const
{
	CString strText = m_pEdit->GetText();
	
	int nStartPos = GetMinuteStartPosition();
	if (strText.GetLength() >= nStartPos + 2)
		return _ttoi(strText.Mid(nStartPos, nStartPos + 2));
	
	return -1;
}

// Returns the current minute as a valid value.  
int CAMSEdit::TimeBehavior::GetValidMinute() const
{
	int nMinute = GetMinute();
	
	// It it's outside the range, fix it
	if (nMinute < GetMinMinute())
		nMinute = GetMinMinute();
	else if (nMinute > GetMaxMinute())
		nMinute = GetMaxMinute();
	
	return nMinute;
}

// Returns the second currently shown on the control or -1.
int CAMSEdit::TimeBehavior::GetSecond() const
{
	CString strText = m_pEdit->GetText();

	int nStartPos = GetSecondStartPosition();
	if (strText.GetLength() >= nStartPos + 2 && _istdigit(strText[nStartPos]) && _istdigit(strText[nStartPos + 1]))
		return _ttoi(strText.Mid(nStartPos, nStartPos + 2));

	return -1;
}

// Returns the current second as a valid value.  
int CAMSEdit::TimeBehavior::GetValidSecond() const
{
	int nSecond = GetSecond();
	if (nSecond < GetMinSecond())
		nSecond = GetMinSecond();
	else if (nSecond > GetMaxSecond())
		nSecond = GetMaxSecond();
	
	return nSecond;
}

// Returns the AM/PM symbol currently shown on the control or an empty string.
CString CAMSEdit::TimeBehavior::GetAMPM() const
{
	CString strText = m_pEdit->GetText();
	int nPos = GetAMPMPosition(strText);
	if (nPos > 0)
		return strText.Mid(nPos);
	return _T("");
}

// Returns the current AM/PM symbol as a valid value.
CString CAMSEdit::TimeBehavior::GetValidAMPM() const
{
	CString strAMPM = GetAMPM();
	if (!IsValidAMPM(strAMPM))
		return m_strAM;			
	
	return strAMPM;
}

// Sets the control's hour to the given value, which must be valid.
void CAMSEdit::TimeBehavior::SetHour(int nHour)
{
	ASSERT(IsValidHour(nHour, false));
	
	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetHour() >= 0)		// see if there's already an hour
		m_pEdit->SetSel(GetHourStartPosition(), GetHourStartPosition() + 3);
	
	// Convert it to AM/PM hour if necessary
	CString strAMPM;
	if (!IsShowing24HourFormat() && nHour > 12)
		nHour = ConvertToAMPMHour(nHour, &strAMPM);

	CString strText;
	strText.Format(_T("%02d%c"), nHour, m_cSep);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the hour

	// Change the AM/PM if it's present
	if (!strAMPM.IsEmpty() && IsValidAMPM(GetAMPM()))
		SetAMPM(strAMPM == m_strAM);
}

// Sets the control's minute to the given value, which must be valid.
void CAMSEdit::TimeBehavior::SetMinute(int nMinute)
{
	ASSERT(IsValidMinute(nMinute));
	
	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetMinute() >= 0)		// see if there's already a minute
		m_pEdit->SetSel(GetMinuteStartPosition(), GetMinuteStartPosition() + 2 + IsShowingSeconds());
	
	CString strText;
	strText.Format(_T("%02d"), nMinute);
	if (IsShowingSeconds())
		strText += m_cSep;
	
	m_pEdit->ReplaceSel(strText, TRUE);	// set the minute

	// Append the AM/PM if no seconds come after and it's not in 24-hour format
	if (!IsShowingSeconds())
		ShowAMPM();
}

// Sets the control's second to the given value, which must be valid.
void CAMSEdit::TimeBehavior::SetSecond(int nSecond)
{
	ASSERT(IsValidSecond(nSecond));
	
	if (!IsShowingSeconds())
		return;

	SelectionSaver selection = m_pEdit;	// remember the current selection
	
	if (GetSecond() >= 0)		// see if there's already a second
		m_pEdit->SetSel(GetSecondStartPosition(), GetSecondStartPosition() + 2);
	
	CString strText;
	strText.Format(_T("%02d"), nSecond);
	m_pEdit->ReplaceSel(strText, TRUE);	// set the second

	// Append the AM/PM if it's not in 24-hour format
	ShowAMPM();
}

// Returns the combination of the given hour and AM/PM symbol converted to 24-hour format.
int CAMSEdit::TimeBehavior::ConvertTo24Hour(int nHour, const CString& strAMPM) const
{
	if (strAMPM == m_strPM && nHour >= 1 && nHour <= 11)
		nHour += 12;
	else if (strAMPM == m_strAM && nHour == 12)
		nHour = 0;
	return nHour;
}

// Returns the given hour (in 24-hour format) to 12-Hour format and sets the optional 
// string pointer to the resulting AM/PM symbol (if not NULL).
int CAMSEdit::TimeBehavior::ConvertToAMPMHour(int nHour, CString* pStrAMPM /*= NULL*/) const
{
	CString strAMPM = m_strAM;

	if (nHour >= 12)
	{
		nHour -= 12;
		strAMPM = m_strPM;
	}
	if (nHour == 0)
		nHour = 12;

	if (pStrAMPM)
		*pStrAMPM = strAMPM;

	return nHour;
}

// Returns the time on the control as a CTime object.
CTime CAMSEdit::TimeBehavior::GetTime() const
{
    if (IsShowing24HourFormat())
		return CTime(1970, 1, 1, GetHour(), GetMinute(), GetValidSecond());
	return CTime(1970, 1, 1, ConvertTo24Hour(GetHour(), GetAMPM()), GetMinute(), GetValidSecond());
}

// Returns the time on the control as a COleDateTime object.
COleDateTime CAMSEdit::TimeBehavior::GetOleTime() const
{
    if (IsShowing24HourFormat())
		return COleDateTime(AMS_DEFAULT_DATE, GetHour(), GetMinute(), GetValidSecond());
	return COleDateTime(AMS_DEFAULT_DATE, ConvertTo24Hour(GetHour(), GetAMPM()), GetMinute(), GetValidSecond());
}

// Sets the hour, minute, and second on the control to the given values, which must be valid.
void CAMSEdit::TimeBehavior::SetTime(int nHour, int nMinute, int nSecond /*= 0*/)
{
	ASSERT(IsValidHour(nHour, true));
	ASSERT(IsValidMinute(nMinute));
	ASSERT(IsValidSecond(nSecond));
	m_pEdit->SetWindowText(GetFormattedTime(nHour, nMinute, nSecond));
}

// Sets the hour, minute, and second on the control based on the given CTime object, which must be valid.
void CAMSEdit::TimeBehavior::SetTime(const CTime& time)
{
	SetTime(time.GetHour(), time.GetMinute(), time.GetSecond());
}

// Sets the hour, minute, and second on the control based on the given COleDateTime object, which must be valid.
void CAMSEdit::TimeBehavior::SetTime(const COleDateTime& time)
{
	SetTime(time.GetHour(), time.GetMinute(), time.GetSecond());
}

// Sets the hour, minute, and second on the control to the current time.
void CAMSEdit::TimeBehavior::SetTimeToNow()
{
	SetTime(CTime::GetCurrentTime());
}

// Returns true if the control's time is valid and falls within allowed range.
bool CAMSEdit::TimeBehavior::IsValid() const
{
	return IsValid(true);
}

// Returns true if the control's time is valid.
// If bCheckRangeAlso is true, the time is also checked that it falls within allowed range.
bool CAMSEdit::TimeBehavior::IsValid(bool bCheckRangeAlso) const
{
	// Check that we have a valid hour and minute
    int nHour = GetHour();
    int nMinute = GetMinute();
	if (nHour < 0 || nMinute < 0)
		return false;
    
	// Check that the seconds are valid if being shown
	int nSecond = GetSecond();
	bool bShowingSeconds = IsShowingSeconds();
	if (bShowingSeconds != (nSecond >= 0))
		return false;

	// Check the AM/PM portion
	CString strAMPM = GetAMPM();
	bool b24HourFormat = IsShowing24HourFormat();
    if ((b24HourFormat && !strAMPM.IsEmpty()) ||
		(!b24HourFormat && (strAMPM != m_strAM && strAMPM != m_strPM)))
		return false;

	if (!b24HourFormat && strAMPM == m_strPM)
	{
		nHour += 12;
		if (nHour == 24)
			nHour = 0;
	}
	if (!bShowingSeconds)
		nSecond = m_timeMin.GetSecond(); // avoids possible problem when checking range below

	// Check that it's valid
	COleDateTime date(AMS_DEFAULT_DATE, nHour, nMinute, nSecond);
	if (date.GetStatus() != COleDateTime::valid)
		return false;

	// Check the range if desired
	if (bCheckRangeAlso)
		return IsValid(date, false);
	return true;
}

// Returns true if the control's time is valid and falls within the allowed range.
// If bShowErrorIfNotValid is true, an error message box is shown and the control gets the focus.
bool CAMSEdit::TimeBehavior::CheckIfValid(bool bShowErrorIfNotValid /*= true*/)
{
	if (!m_pEdit->IsWindowEnabled())
		return true;
	
	bool bValid = IsValid();
	if (!bValid && bShowErrorIfNotValid)
	{
		ShowErrorMessage();
		m_pEdit->SetFocus();
	}
	
	return bValid;
}

// Shows a message box informing the user to enter a valid time within the allowed range.
void CAMSEdit::TimeBehavior::ShowErrorMessage() const
{
	AfxMessageBox(_T("Please specify a time between ") + GetFormattedTime(m_timeMin.GetHour(), m_timeMin.GetMinute(), m_timeMin.GetSecond()) + _T(" and ") + GetFormattedTime(m_timeMax.GetHour(), m_timeMax.GetMinute(), m_timeMax.GetSecond() + '.'), MB_ICONEXCLAMATION);
}

// Sets the range of allowed time values to the given minimum and maximum CTime values.
void CAMSEdit::TimeBehavior::SetRange(const CTime& dateMin, const CTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_CTIME);
	ASSERT(dateMax <= AMS_MAX_CTIME);
	
	m_timeMin.SetTime(dateMin.GetHour(), dateMin.GetMinute(), dateMin.GetSecond());
	m_timeMax.SetTime(dateMax.GetHour(), dateMax.GetMinute(), dateMin.GetSecond());
	ASSERT(m_timeMin <= m_timeMax);
	
	_Redraw();
}

// Sets the range of allowed time values to the given minimum and maximum COleDateTime values.
void CAMSEdit::TimeBehavior::SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax)
{
	ASSERT(dateMin >= AMS_MIN_OLEDATETIME);
	ASSERT(dateMax <= AMS_MAX_OLEDATETIME);
	
	m_timeMin.SetTime(dateMin.GetHour(), dateMin.GetMinute(), dateMin.GetSecond());
	m_timeMax.SetTime(dateMax.GetHour(), dateMax.GetMinute(), dateMin.GetSecond());
	ASSERT(m_timeMin <= m_timeMax);

	_Redraw();
}

// Retrieves the range of allowed time values inside the given set of CTime pointers.
void CAMSEdit::TimeBehavior::GetRange(CTime* pDateMin, CTime* pDateMax) const
{
	if (pDateMin)
		*pDateMin = CTime(pDateMin->GetYear(), pDateMin->GetMonth(), pDateMin->GetDay(), m_timeMin.GetHour(), m_timeMin.GetMinute(), m_timeMin.GetSecond());
	if (pDateMax)
		*pDateMax = CTime(pDateMin->GetYear(), pDateMin->GetMonth(), pDateMin->GetDay(), m_timeMax.GetHour(), m_timeMax.GetMinute(), m_timeMax.GetSecond());
}

// Retrieves the range of allowed time values inside the given set of COleDateTime pointers.
void CAMSEdit::TimeBehavior::GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const
{
	if (pDateMin)
		*pDateMin = COleDateTime(pDateMin->GetYear(), pDateMin->GetMonth(), pDateMin->GetDay(), m_timeMin.GetHour(), m_timeMin.GetMinute(), m_timeMin.GetSecond());
	if (pDateMax)
		*pDateMax = COleDateTime(pDateMin->GetYear(), pDateMin->GetMonth(), pDateMin->GetDay(), m_timeMax.GetHour(), m_timeMax.GetMinute(), m_timeMax.GetSecond());
}

// Returns true if the given date is valid and falls within the range.
bool CAMSEdit::TimeBehavior::IsValid(const COleDateTime& date, bool bDateOnly /*= true*/) const
{
	return (date.GetStatus() == COleDateTime::valid && date >= m_timeMin && date <= m_timeMax);
}

// Sets the character used to separate the hour, minute, and second values.
void CAMSEdit::TimeBehavior::SetSeparator(TCHAR cSep)
{
	ASSERT(cSep);
	ASSERT(!_istdigit(cSep));
	
	if (m_cSep != cSep)
	{
		m_cSep = cSep;
		_Redraw();
	}
}

// Returns the character used to separate the hour, minute, and second values.
TCHAR CAMSEdit::TimeBehavior::GetSeparator() const
{
	return m_cSep;
}

// Sets whether the hour should be shown in 24-Hour format or in 12-Hour format with the AM/PM symbols.
// Note: By default, this is based on the user's system configuration.
void CAMSEdit::TimeBehavior::Show24HourFormat(bool bShow24HourFormat /*= true*/)
{
	ModifyFlags(bShow24HourFormat ? TwentyFourHourFormat : 0, bShow24HourFormat ? 0 : TwentyFourHourFormat);
}

// Returns true if the hour will be shown in 24-Hour format (instead of 12-Hour format with the AM/PM symbols).
bool CAMSEdit::TimeBehavior::IsShowing24HourFormat() const
{
	return (m_uFlags & TwentyFourHourFormat) ? true : false;
}

// Sets whether the seconds will be shown.
void CAMSEdit::TimeBehavior::ShowSeconds(bool bShowSeconds /*= true*/)
{
	ModifyFlags(bShowSeconds ? WithSeconds : 0, bShowSeconds ? 0 : WithSeconds);
}

// Returns true if the seconds will be shown.
bool CAMSEdit::TimeBehavior::IsShowingSeconds() const
{
	return (m_uFlags & WithSeconds) ? true : false;
}

// Sets the symbols to display for AM and PM, which must each be of the same length.
// Note: By default, these symbols are retrieved from the user's system configuration.
void CAMSEdit::TimeBehavior::SetAMPMSymbols(const CString& strAM, const CString& strPM)
{
	ASSERT(strAM.GetLength() == strPM.GetLength());  // make sure they're the same length

	m_strAM = strAM;
	m_strPM = strPM;

	if (m_strAM == _T(""))
		m_strAM = AMS_AM_SYMBOL;
	if (m_strPM == _T(""))
		m_strPM = AMS_PM_SYMBOL;

	m_nAMPMLength = m_strAM.GetLength();
	_Redraw();
}

// Retrieves the symbols to display for AM and PM into the given string pointers, which must be valid.
void CAMSEdit::TimeBehavior::GetAMPMSymbols(CString* pStrAM, CString* pStrPM) const
{
	ASSERT(pStrAM);
	ASSERT(pStrPM);
	*pStrAM = m_strAM;
	*pStrPM = m_strPM;
}

// Returns the control's value in a valid format.
CString CAMSEdit::TimeBehavior::_GetValidText() const
{
	CString strText = m_pEdit->GetText();
	
	// If it's empty or has a valid time, return it
	if (strText.IsEmpty())
		return strText;

	if (IsValid(false))
		return GetFormattedTime(GetHour(), GetMinute(), GetSecond(), GetAMPM());

	// If the hour, minute, and second are invalid, set it to the current time
	if (GetHour() < 0 && GetMinute() < 0 && GetSecond() < 0)
	{
		CTime time = CTime::GetCurrentTime();
		return GetFormattedTime(time.GetHour(), time.GetMinute(), time.GetSecond());
	}

	// Otherwise retrieve the validated time
	return GetFormattedTime(GetValidHour(true), GetValidMinute(), GetValidSecond(), GetAMPM());
}

// Formats the given hour, minute, second, and optional AM/PM symbol into a string based on the proper format.
CString CAMSEdit::TimeBehavior::GetFormattedTime(int nHour, int nMinute, int nSecond, const CString& strAMPM /*= _T("")*/) const
{
	if (IsShowing24HourFormat())
	{
		// Handle switching from AM/PM to 24-hour format
		if (IsValidAMPM(strAMPM))
			nHour = ConvertTo24Hour(nHour, strAMPM);
	}
	else
	{
		// Handle switching from 24-hour format to AM/PM
		if (!IsValidAMPM(strAMPM))
			nHour = ConvertToAMPMHour(nHour, const_cast<CString*>(&strAMPM));
	}

	CString strText;	
    if (IsShowingSeconds())
	{
        if (IsShowing24HourFormat())
            strText.Format(_T("%02d%c%02d%c%02d"), nHour, m_cSep, nMinute, m_cSep, nSecond);
		else 
            strText.Format(_T("%02d%c%02d%c%02d %s"), nHour, m_cSep, nMinute, m_cSep, nSecond, strAMPM);
    }
	else 
	{
        if (IsShowing24HourFormat())
            strText.Format(_T("%02d%c%02d"), nHour, m_cSep, nMinute);
		else 
            strText.Format(_T("%02d%c%02d %s"), nHour, m_cSep, nMinute, strAMPM);
    }
	
	return strText;
}

// Adjusts the control's value to be within the range of allowed time values.
void CAMSEdit::TimeBehavior::AdjustWithinRange()
{
	// Check if it's already within the range
	if (IsValid())
		return;

	// If it's empty, set it to the current time
	if (m_pEdit->GetText().IsEmpty())
		m_pEdit->SetWindowText(_T(" "));
	else
		_Redraw();

	// Make it fall within the range
	COleDateTime date = GetOleTime();
	if (date < m_timeMin)
		SetTime(m_timeMin);
	else if (date > m_timeMax)
		SetTime(m_timeMax);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSEdit::DateTimeBehavior

CAMSEdit::DateTimeBehavior::DateTimeBehavior(CAMSEdit* pEdit) : 
	DateBehavior(pEdit), 
	TimeBehavior(pEdit),
	Behavior(pEdit)
{ 
	m_nHourStart = 11;
}

// Sets the month, day, year, hour, minute, and second on the control to the given values, which must be valid.
void CAMSEdit::DateTimeBehavior::SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond /*= 0*/)
{
	if (m_uFlags & DateOnly)
		SetDate(nYear, nMonth, nDay);
	else if (m_uFlags & TimeOnly)
		SetTime(nHour, nMinute, nSecond);
	else
	{
		ASSERT(IsValid(COleDateTime(nYear, nMonth, nDay, nHour, nMinute, nSecond))); 
		m_pEdit->SetWindowText(GetFormattedDate(nYear, nMonth, nDay) + ' ' + GetFormattedTime(nHour, nMinute, nSecond));
	}
}

// Sets the month, day, year, hour, minute, and second on the control to the CTime object, which must be valid.
void CAMSEdit::DateTimeBehavior::SetDateTime(const CTime& dt)
{
	SetDateTime(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
}

// Sets the month, day, year, hour, minute, and second on the control to the COleDateTime object, which must be valid.
void CAMSEdit::DateTimeBehavior::SetDateTime(const COleDateTime& dt)
{
	SetDateTime(dt.GetYear(), dt.GetMonth(), dt.GetDay(), dt.GetHour(), dt.GetMinute(), dt.GetSecond());
}

// Sets the month, day, year, hour, minute, and second on the control to the current date and time.
void CAMSEdit::DateTimeBehavior::SetToNow()
{
	SetDateTime(COleDateTime::GetCurrentTime());
}

// Returns the date and time on the control as a CTime object.
CTime CAMSEdit::DateTimeBehavior::GetDateTime() const
{
	CTime date = GetDate();
	CTime time = GetTime();
	
	if (m_uFlags & DateOnly)
		return date;
	if (m_uFlags & TimeOnly)
		return time;	
	return CTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
}

// Returns the date and time on the control as a COleDateTime object.
COleDateTime CAMSEdit::DateTimeBehavior::GetOleDateTime() const
{
	COleDateTime date = GetOleDate();
	COleDateTime time = GetOleTime();
	
	if (m_uFlags & DateOnly)
		return date;
	if (m_uFlags & TimeOnly)
		return time;	
	return COleDateTime(date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond());
}

// Returns true if the control's date and time is valid and falls within the allowed range.
bool CAMSEdit::DateTimeBehavior::IsValid() const
{
	if (m_uFlags & DateOnly)
		return DateBehavior::IsValid();
	if (m_uFlags & TimeOnly)
		return TimeBehavior::IsValid();
	return (DateBehavior::IsValid() && TimeBehavior::IsValid());
}

// Sets the range of allowed date values to the given minimum and maximum CTime values.
void CAMSEdit::DateTimeBehavior::SetRange(const CTime& dateMin, const CTime& dateMax)
{
	DateBehavior::SetRange(dateMin, dateMax);
	TimeBehavior::SetRange(dateMin, dateMax);
}

// Sets the range of allowed date values to the given minimum and maximum COleDateTime values.
void CAMSEdit::DateTimeBehavior::SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax)
{
	DateBehavior::SetRange(dateMin, dateMax);
	TimeBehavior::SetRange(dateMin, dateMax);
}

// Retrieves the range of allowed date and time values inside the given set of CTime pointers.
void CAMSEdit::DateTimeBehavior::GetRange(CTime* pDateMin, CTime* pDateMax) const
{
	DateBehavior::GetRange(pDateMin, pDateMax);
	TimeBehavior::GetRange(pDateMin, pDateMax);
}

// Retrieves the range of allowed date and time values inside the given set of COleDateTime pointers.
void CAMSEdit::DateTimeBehavior::GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const
{
	DateBehavior::GetRange(pDateMin, pDateMax);
	TimeBehavior::GetRange(pDateMin, pDateMax);
}

// Returns true if the given date and time is valid and falls within the range.
bool CAMSEdit::DateTimeBehavior::IsValid(const COleDateTime& date, bool bDateOnly /*= true*/) const
{
	if (bDateOnly)
		return DateBehavior::IsValid(date);

	COleDateTime dt(GetYear(), GetMonth(), GetDay(), date.GetHour(), date.GetMinute(), date.GetSecond());
	COleDateTime dtMin(m_dateMin.GetYear(), m_dateMin.GetMonth(), m_dateMin.GetDay(), m_timeMin.GetHour(), m_timeMin.GetMinute(), m_timeMin.GetSecond());
	COleDateTime dtMax(m_dateMax.GetYear(), m_dateMax.GetMonth(), m_dateMax.GetDay(), m_timeMax.GetHour(), m_timeMax.GetMinute(), m_timeMax.GetSecond());
	return (dt.GetStatus() == COleDateTime::valid && dt >= dtMin && dt <= dtMax);
}

// Sets the character used to separate the date or time values, depending on the flag.
void CAMSEdit::DateTimeBehavior::SetSeparator(TCHAR cSep, bool bDate)
{
	if (bDate)
		DateBehavior::SetSeparator(cSep);
	else
		TimeBehavior::SetSeparator(cSep);
}

// Returns the character used to separate the date or time values, depending on the flag.
TCHAR CAMSEdit::DateTimeBehavior::GetSeparator(bool bDate) const
{
	return (bDate ? DateBehavior::GetSeparator() : TimeBehavior::GetSeparator());
}

// Adds and removes flags from the behavior and then redraws the control
void CAMSEdit::DateTimeBehavior::ModifyFlags(UINT uAdd, UINT uRemove)
{
	UINT uFlags = (m_uFlags & ~uRemove) | uAdd;
	m_nHourStart = (uFlags & TimeOnly) ? 0 : 11;

	Behavior::ModifyFlags(uAdd, uRemove);
}

// Returns the control's value in a valid format.
CString CAMSEdit::DateTimeBehavior::_GetValidText() const
{
	// Check if we're showing the date only
	CString strDate = DateBehavior::_GetValidText();
	if (m_uFlags & DateOnly)
		return strDate;

	// Check if we're showing the time only
	CString strTime = TimeBehavior::_GetValidText();
	if (m_uFlags & TimeOnly)
		return strTime;

	CString strSpace = (!strDate.IsEmpty() && !strTime.IsEmpty() ? _T(" ") : _T(""));
	return strDate + strSpace + strTime;
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSEdit::DateTimeBehavior::_OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	// Check to see if it's read only
	if (m_pEdit->IsReadOnly())
		return;

	// Check if we're showing the date or the time only
	if (m_uFlags & DateOnly)
	{
		DateBehavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}
	if (m_uFlags & TimeOnly)
	{
		TimeBehavior::_OnChar(uChar, nRepCnt, nFlags);
		return;
	}

	TCHAR c = static_cast<TCHAR>(uChar);
	
	int nStart, nEnd;
	DateBehavior::m_pEdit->GetSel(nStart, nEnd);

	CString strText = m_pEdit->GetText();
	int nLen = strText.GetLength();

	if (nStart >= 0 && nStart <= 9)
	{
		DateBehavior::_OnChar(uChar, nRepCnt, nFlags);
		ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
	else if (nStart == 10)
	{
		DateBehavior::_OnChar(uChar, nRepCnt, nFlags);

		int nSpace = 0;
		if (c == ' ')
			nSpace = 1;
		else
			nSpace = (IsValidHourDigit(c, 0) || (IsValidHourDigit(c, 1) && nLen <= 11) ? 2 : 0);
		
		// If we need the space, enter it
		if (nSpace && _ShouldEnter(c))
		{
			DateBehavior::m_pEdit->SetSel(nStart, nStart + 1, FALSE);
			DateBehavior::m_pEdit->ReplaceSel(CString(' '), TRUE);
		}
		
		// If the space is to be preceded by a valid digit, "type" it in.
		if (nSpace == 2)
			keybd_event((BYTE)c, 0, 0, 0);
		else
			ChangeAMPM(c);	// allow changing AM/PM (if it's being shown) by pressing A or P
	}
	else
		TimeBehavior::_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSEdit::DateTimeBehavior::_OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	// Check if we're showing the time only
	if (m_uFlags & TimeOnly)
	{
		TimeBehavior::_OnKeyDown(uChar, nRepCnt, nFlags);
		return;
	}

	DateBehavior::_OnKeyDown(uChar, nRepCnt, nFlags);
	if ((uChar == VK_UP || uChar == VK_DOWN) && !(m_uFlags & DateOnly))
		TimeBehavior::_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
// It's used here to check if any action needs to be taken based on the control's value.
void CAMSEdit::DateTimeBehavior::_OnKillFocus(CWnd* pNewWnd)
{
	DateBehavior::_OnKillFocus(pNewWnd);
}

// Shows a message box informing the user to enter a valid date and time within the allowed range.
void CAMSEdit::DateTimeBehavior::ShowErrorMessage() const
{
	// Show the message depending on what we're showing
	if (m_uFlags & DateOnly)
		DateBehavior::ShowErrorMessage();
	else if (m_uFlags & TimeOnly)
		TimeBehavior::ShowErrorMessage();
	else
	{
		CString strMinDateTime = 
			GetFormattedDate(m_dateMin.GetYear(), m_dateMin.GetMonth(), m_dateMin.GetDay()) + ' ' + 
			GetFormattedTime(m_timeMin.GetHour(), m_timeMin.GetMinute(), m_timeMin.GetSecond());
		CString strMaxDateTime = 
			GetFormattedDate(m_dateMax.GetYear(), m_dateMax.GetMonth(), m_dateMax.GetDay()) + ' ' + 
			GetFormattedTime(m_timeMax.GetHour(), m_timeMax.GetMinute(), m_timeMax.GetSecond());
		AfxMessageBox(_T("Please specify a date and time between ") + strMinDateTime + _T(" and ") + strMaxDateTime + '.', MB_ICONEXCLAMATION);
	}
}


/////////////////////////////////////////////////////////////////////////////
// CAMSAlphanumericEdit window

// Constructs the object using the given set of strInvalidChars
CAMSAlphanumericEdit::CAMSAlphanumericEdit(int nMaxChars /*= 0*/, const CString& strInvalidChars /*= _T("%'*\"+?><:\\"")*/) :
	AlphanumericBehavior(this, nMaxChars, strInvalidChars)
{
}

BEGIN_MESSAGE_MAP(CAMSAlphanumericEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSAlphanumericEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSAlphanumericEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSAlphanumericEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSMaskedEdit window

// Constructs the object using the given numeric strMask.
CAMSMaskedEdit::CAMSMaskedEdit(const CString& strMask /*= _T("")*/) :
	MaskedBehavior(this, strMask)
{
}

BEGIN_MESSAGE_MAP(CAMSMaskedEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSMaskedEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSMaskedEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSMaskedEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSMaskedEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// Constructs the object using the given nMaxWholeDigits and nMaxDecimalPlaces.
CAMSNumericEdit::CAMSNumericEdit(int nMaxWholeDigits /*= 9*/, int nMaxDecimalPlaces /*= 4*/) :
	NumericBehavior(this, nMaxWholeDigits, nMaxDecimalPlaces)
{
}

BEGIN_MESSAGE_MAP(CAMSNumericEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSNumericEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSNumericEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSNumericEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSNumericEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
void CAMSNumericEdit::OnKillFocus(CWnd* pNewWnd) 
{
	_OnKillFocus(pNewWnd);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSIntegerEdit window

// Constructs the object allowing the negative sign or not.
CAMSIntegerEdit::CAMSIntegerEdit(int nMaxWholeDigits /*= 9*/) :
	CAMSNumericEdit(nMaxWholeDigits, 0)
{
}

BEGIN_MESSAGE_MAP(CAMSIntegerEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSIntegerEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// Constructs the object using the given numeric strMask.
CAMSCurrencyEdit::CAMSCurrencyEdit()
{
	m_nDigitsInGroup = 3;
	m_nMaxDecimalPlaces = 2;
	m_strPrefix = _T("$");
	m_uFlags |= OnKillFocus_PadWithZerosAfterDecimal | OnKillFocus_DontPadWithZerosIfEmpty;
	
	// Get the system's current settings
	TCHAR szValue[10];
	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SCURRENCY, szValue, sizeof(szValue));
		m_strPrefix = szValue;
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONGROUPING, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONGROUPING, szValue, sizeof(szValue));
		m_nDigitsInGroup = _ttoi(szValue);
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRDIGITS, szValue, 0))
	{
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_ICURRDIGITS, szValue, sizeof(szValue));
		m_nMaxDecimalPlaces = _ttoi(szValue);
	}

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, &m_cDecimalPoint, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONDECIMALSEP, &m_cDecimalPoint, sizeof(m_cDecimalPoint));

	if (::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, &m_cGroupSeparator, 0))
		::GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SMONTHOUSANDSEP, &m_cGroupSeparator, sizeof(m_cGroupSeparator));
}
	
BEGIN_MESSAGE_MAP(CAMSCurrencyEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSCurrencyEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CAMSDateEdit

CAMSDateEdit::CAMSDateEdit() :
	DateBehavior(this),
	Behavior(this) // required because DateBehavior derives virtually from Behavior
{
}


BEGIN_MESSAGE_MAP(CAMSDateEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSDateEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSDateEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSDateEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSDateEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
void CAMSDateEdit::OnKillFocus(CWnd* pNewWnd) 
{
	_OnKillFocus(pNewWnd);
}

// Handles the WM_PASTE message to ensure that the text being pasted is a valid date.
LONG CAMSDateEdit::OnPaste(UINT, LONG)
{
	return _OnPaste(0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSTimeEdit

CAMSTimeEdit::CAMSTimeEdit() :
	TimeBehavior(this),
	Behavior(this)  // required because TimeBehavior derives virtually from Behavior
{
}


BEGIN_MESSAGE_MAP(CAMSTimeEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSTimeEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSTimeEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSTimeEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSTimeEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
void CAMSTimeEdit::OnKillFocus(CWnd* pNewWnd) 
{
	_OnKillFocus(pNewWnd);
}

// Handles the WM_PASTE message to ensure that the text being pasted is a valid time.
LONG CAMSTimeEdit::OnPaste(UINT, LONG)
{
	return _OnPaste(0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSDateTimeEdit

CAMSDateTimeEdit::CAMSDateTimeEdit() :
	DateTimeBehavior(this),
	Behavior(this) // required because DateTimeBehavior derives virtually from Behavior
{
}


BEGIN_MESSAGE_MAP(CAMSDateTimeEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSDateTimeEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Returns the control's value in a valid format.
CString CAMSDateTimeEdit::GetValidText() const
{
	return _GetValidText();
}

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSDateTimeEdit::OnChar(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	_OnChar(uChar, nRepCnt, nFlags);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSDateTimeEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags) 
{
	_OnKeyDown(uChar, nRepCnt, nFlags);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
void CAMSDateTimeEdit::OnKillFocus(CWnd* pNewWnd) 
{
	_OnKillFocus(pNewWnd);
}

// Handles the WM_PASTE message to ensure that the text being pasted is a valid date.
LONG CAMSDateTimeEdit::OnPaste(UINT, LONG)
{
	return _OnPaste(0, 0);
}


/////////////////////////////////////////////////////////////////////////////
// CAMSMultiMaskedEdit

CAMSMultiMaskedEdit::CAMSMultiMaskedEdit() :
	AlphanumericBehavior(this),
	NumericBehavior(this),
	MaskedBehavior(this),
	DateTimeBehavior(this),
	Behavior(this),
	m_pCurrentBehavior((AlphanumericBehavior*)this)
{
}

// Returns the mask
const CString& CAMSMultiMaskedEdit::GetMask() const
{
	return MaskedBehavior::m_strMask;
}

// Sets the mask and thereby the edit box's behavior
void CAMSMultiMaskedEdit::SetMask(const CString& strMask)
{
	int nLen = strMask.GetLength();
	MaskedBehavior::m_strMask = strMask;

	// If it doesn't have numeric place holders then it's alphanumeric
	int nPos = strMask.Find('#');
	if (nPos < 0)
	{
		m_pCurrentBehavior = (AlphanumericBehavior*)this;
		SetMaxCharacters(nLen);
		return;
	}

	// If it's exactly like the date mask, then it's a date
	if (strMask == _T("##/##/#### ##:##:##"))
	{
		m_pCurrentBehavior = (DateTimeBehavior*)this;
		m_pCurrentBehavior->ModifyFlags(DateTimeBehavior::WithSeconds, 0);
		return;
	}

	// If it's exactly like the date mask, then it's a date
	else if (strMask == _T("##/##/#### ##:##"))
	{
		m_pCurrentBehavior = (DateTimeBehavior*)this;
		return;
	}

	// If it's exactly like the date mask, then it's a date
	else if (strMask == _T("##/##/####"))
	{
		m_pCurrentBehavior = (DateTimeBehavior*)this;
		m_pCurrentBehavior->ModifyFlags(DateTimeBehavior::DateOnly, 0);
		return;
	}

	// If it's exactly like the time mask with seconds, then it's a time
	else if (strMask == _T("##:##:##"))
	{
		m_pCurrentBehavior = (DateTimeBehavior*)this;
		m_pCurrentBehavior->ModifyFlags(DateTimeBehavior::TimeOnly | DateTimeBehavior::WithSeconds, 0);
		return;
	}

	// If it's exactly like the time mask, then it's a time
	else if (strMask == _T("##:##"))
	{
		m_pCurrentBehavior = (DateTimeBehavior*)this;
		m_pCurrentBehavior->ModifyFlags(DateTimeBehavior::TimeOnly, 0);
		return;
	}

	// If after the first numeric placeholder, we don't find any foreign characters,
	// then it's numeric, otherwise it's masked numeric.
	CString strSmallMask = strMask.Mid(nPos + 1);
	for (int iPos = 0, nSmallLen = strSmallMask.GetLength(); iPos < nSmallLen; iPos++)
	{
		TCHAR c = strSmallMask[iPos];
		if (c != '#' && c != m_cDecimalPoint && c != m_cGroupSeparator)
		{
			m_pCurrentBehavior = (MaskedBehavior*)this;
			MaskedBehavior::m_strMask = _T("");
			MaskedBehavior::SetMask(strMask);
			return;
		}
	}

	// Verify that it ends in a number; otherwise it's a masked numeric
	if (nSmallLen && strSmallMask[nSmallLen - 1] != '#')
	{
		m_pCurrentBehavior = (MaskedBehavior*)this;
		MaskedBehavior::m_strMask = _T("");
		MaskedBehavior::SetMask(strMask);
	}
	else
	{
		m_pCurrentBehavior = (NumericBehavior*)this;
		NumericBehavior::SetMask(strMask);
	}
}

// Returns the control's value in a valid format.
CString CAMSMultiMaskedEdit::GetValidText() const
{
	ASSERT(m_pCurrentBehavior);
	return m_pCurrentBehavior->_GetValidText();
}

BEGIN_MESSAGE_MAP(CAMSMultiMaskedEdit, CEdit)
	//{{AFX_MSG_MAP(CAMSMultiMaskedEdit)
	ON_WM_CHAR()
	ON_WM_KEYDOWN()
	ON_WM_KILLFOCUS()
	//}}AFX_MSG_MAP
	ON_MESSAGE(WM_CUT, OnCut)
	ON_MESSAGE(WM_PASTE, OnPaste)
	ON_MESSAGE(WM_CLEAR, OnClear)
	ON_MESSAGE(WM_SETTEXT, OnSetText)
END_MESSAGE_MAP()

// Handles the WM_CHAR message, which is called when the user enters a regular character or Backspace
void CAMSMultiMaskedEdit::OnChar(UINT uChar, UINT nRepCnt, UINT c)
{
	ASSERT(m_pCurrentBehavior);
	m_pCurrentBehavior->_OnChar(uChar, nRepCnt, nRepCnt);
}

// Handles the WM_KEYDOWN message, which is called when the user enters a special character such as Delete or the arrow keys.
void CAMSMultiMaskedEdit::OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags)
{
	ASSERT(m_pCurrentBehavior);
	m_pCurrentBehavior->_OnKeyDown(uChar, nRepCnt, nRepCnt);
}

// Handles the WM_KILLFOCUS message, which is called when the user leaves the control.
void CAMSMultiMaskedEdit::OnKillFocus(CWnd* pNewWnd) 
{
	ASSERT(m_pCurrentBehavior);
	m_pCurrentBehavior->_OnKillFocus(pNewWnd);
}

// Handles the WM_PASTE message to ensure that the text being pasted is properly shown.
LONG CAMSMultiMaskedEdit::OnPaste(UINT wParam, LONG lParam)
{
	ASSERT(m_pCurrentBehavior);
	return m_pCurrentBehavior->_OnPaste(wParam, lParam);
}
