#if !defined(AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_)
#define AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// Edit.h : header file
// Created by: Alvaro Mendez - 07/17/2000
//

#include <afxwin.h>
#include <afxtempl.h>
#include "DropEdit.h"

/////////////////////////////////////////////////////////////////////////////
// CAMSEdit window

// Class CAMSEdit is the base class for all the other AMS CEdit classes.  
// It provides some base functionality to set and get the text and change
// its text and background color.
//
class CAMSEdit : public SECDropEdit
{
public:
	// Construction/destruction
	CAMSEdit();
	virtual ~CAMSEdit();

	// Operations
	void SetText(const CString& strText);
	CString GetText() const;
	CString GetTrimmedText() const;

	void SetBackgroundColor(COLORREF rgb);
	COLORREF GetBackgroundColor() const;

	void SetTextColor(COLORREF rgb);
	COLORREF GetTextColor() const;

	bool IsReadOnly() const;
	
protected:
	virtual void Redraw();
	virtual CString GetValidText() const;
	virtual BOOL OnChildNotify(UINT message, WPARAM wParam, LPARAM lParam, LRESULT* pLResult);
	virtual bool ShouldEnter(TCHAR c) const;

protected:
	CBrush	m_brushBackground;
	COLORREF m_rgbText;

private:
	enum InternalFlags
	{
		None				= 0x0000,
		TextColorHasBeenSet = 0x0001
	};
	UINT m_uInternalFlags;

public:
	// Class SelectionSaver is used to save an edit box's current
	// selection and then restore it on destruction.
	class SelectionSaver
	{
	public:
		SelectionSaver(CEdit* pEdit);
		SelectionSaver(CEdit* pEdit, int nStart, int nEnd);
		~SelectionSaver();

		void MoveTo(int nStart, int nEnd);
		void MoveBy(int nStart, int nEnd);
		void MoveBy(int nPos);
		void operator+=(int nPos);

		int GetStart() const;
		int GetEnd() const;

		void Update();
		void Disable();

	protected:
		CEdit* m_pEdit;
		int m_nStart, m_nEnd;
	};

	// Class Behavior is an abstract base class used to define how an edit
	// box will behave when it is used.   Note that its virtual member functions start
	// with an underscore; this avoids naming conflicts when multiply inheriting.
	class Behavior
	{
	protected:
		Behavior(CAMSEdit* pEdit);
		virtual ~Behavior();

	public:
		void ModifyFlags(UINT uAdd, UINT uRemove);
		UINT GetFlags() const;

	public:
		virtual CString _GetValidText() const = 0;

		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags) = 0;
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);
		virtual LONG _OnPaste(UINT wParam, LONG lParam);

	protected:
		// Wrappers to allow access to protected members of CAMSEdit
		virtual LRESULT _Default();		
		virtual void _Redraw();
		virtual bool _ShouldEnter(TCHAR c) const;

	protected:
		CAMSEdit* m_pEdit;
		UINT m_uFlags;
	};
	friend class Behavior;

	// The AlphanumericBehavior class is used to allow entry of alphanumeric
	// characters.  It can be restricted in terms of what characters cannot 
	// be inputed as well as how many are allowed altogether.
	class AlphanumericBehavior : public Behavior
	{
	public:
		AlphanumericBehavior(CAMSEdit* pEdit, int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

		// Operations
		void SetInvalidCharacters(const CString& strInvalidChars);
		const CString& GetInvalidCharacters() const;

		void SetMaxCharacters(int nMaxChars);
		int GetMaxCharacters() const;

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		int m_nMaxChars;
		CString m_strInvalidChars;
	};

	// The MaskedBehavior class is used to allow entry of numeric characters
	// based on a given mask containing '#' characters to hold digits.
	class MaskedBehavior : public Behavior
	{
	public:
		// Construction
		MaskedBehavior(CAMSEdit* pEdit, const CString& strMask = _T(""));

	public:
		// Operations
		void SetMask(const CString& strMask);
		const CString& GetMask() const;

		CString GetNumericText() const;
	
		// The Symbol class represents a character which may be added to the mask and then interpreted by the 
		// MaskedBehavior class to validate the input from the user and possibly convert it to something else.
		class Symbol
		{
		public:
			#ifndef _UNICODE
				typedef int (*ValidationFunction)(UINT);	// designed for functions such as _istdigit, _istalpha
				typedef UINT (*ConversionFunction)(UINT);	// designed for functions such as _totupper, _totlower
			#else
				typedef int (*ValidationFunction)(WCHAR);		
				typedef WCHAR (*ConversionFunction)(WCHAR);		
			#endif
			
			Symbol();
			Symbol(TCHAR cSymbol, ValidationFunction fnValidation, ConversionFunction fnConversion = NULL);
			virtual ~Symbol();

			virtual bool Validate(TCHAR c) const;
			virtual TCHAR Convert(TCHAR c) const;

			void Set(TCHAR cSymbol);
			TCHAR Get() const;
			operator TCHAR() const;

		protected:
			TCHAR m_cSymbol;
			ValidationFunction m_fnValidation;
			ConversionFunction m_fnConversion;
		};

		typedef CArray<Symbol, Symbol&> SymbolArray;

		SymbolArray& GetSymbolArray();

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);

	protected:
		// Attributes
		CString m_strMask;
		SymbolArray m_arraySymbols;
	};

	// The NumericBehavior class is used to allow the entry of an actual numeric
	// value into the edit control.  It may be restricted by the number of digits
	// before or after the decimal point (if any).  If can also be set to use
	// commas to separate and group thousands.
	class NumericBehavior : public Behavior
	{
	public:
		// Construction
		NumericBehavior(CAMSEdit* pEdit, int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

	public:
		// Operations
		void SetDouble(double dText, bool bTrimTrailingZeros = true);
		double GetDouble() const;

		void SetInt(int nText);
		int GetInt() const;
		
		void SetMaxWholeDigits(int nMaxWholeDigits);
		int GetMaxWholeDigits() const;
		
		void SetMaxDecimalPlaces(int nMaxDecimalPlaces);
		int GetMaxDecimalPlaces() const;
		
		void AllowNegative(bool bAllowNegative = true);
		bool IsNegativeAllowed() const;
		
		void SetDigitsInGroup(int nDigitsInGroup);
		int GetDigitsInGroup() const;
		
		void SetSeparators(TCHAR cDecimal, TCHAR cGroup);
		void GetSeparators(TCHAR* pcDecimal, TCHAR* pcGroup) const;

		void SetPrefix(const CString& strPrefix);
		const CString& GetPrefix() const;

		void SetMask(const CString& strMask);
		CString GetMask() const;

		void SetRange(double dMin, double dMax);
		void GetRange(double* pdMin, double* pdMax) const;

		virtual bool IsValid() const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		enum Flags
		{
			None										= 0x0000,
			AddDecimalAfterMaxWholeDigits				= 0x1000,

			OnKillFocus_Beep_IfInvalid					= 0x0001,
			OnKillFocus_Beep_IfEmpty					= 0x0002,
			OnKillFocus_Beep							= 0x0003,
			OnKillFocus_SetValid_IfInvalid				= 0x0004,
			OnKillFocus_SetValid_IfEmpty				= 0x0008,
			OnKillFocus_SetValid						= 0x000C,
			OnKillFocus_SetFocus_IfInvalid				= 0x0010,
			OnKillFocus_SetFocus_IfEmpty				= 0x0020,
			OnKillFocus_SetFocus						= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid			= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty				= 0x00A0,
			OnKillFocus_ShowMessage						= 0x00F0,

			OnKillFocus_PadWithZerosBeforeDecimal		= 0x0100,
			OnKillFocus_PadWithZerosAfterDecimal		= 0x0200,
			OnKillFocus_DontPadWithZerosIfEmpty			= 0x0400,
			OnKillFocus_Max								= 0x0FFF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

		int GetGroupSeparatorCount(const CString& strText) const;
		
		CString GetNumericText(const CString& strText, bool bConvertToNumericSigns = false) const;
		CString GetDoubleText(double dText, bool bTrimTrailingZeros = true) const;
		CString GetSeparatedText(const CString& strText) const;
		void AdjustSeparators(int nCurrentSeparatorCount);
		static void InsertZeros(CString* pStrText, int nPos, int nCount);

		virtual void ShowErrorMessage() const;
		void AdjustWithinRange();

	protected:
		// Attributes
		int m_nMaxWholeDigits;
		int m_nMaxDecimalPlaces;
		bool m_bAllowNegative;
		TCHAR m_cNegativeSign;
		TCHAR m_cDecimalPoint;
		TCHAR m_cGroupSeparator;
		int m_nDigitsInGroup;
		CString m_strPrefix;
		double m_dMin;
		double m_dMax;
	};

	// The DateBehavior class is used to allow the entry of date values.
	class DateBehavior : virtual public Behavior
	{
	public:
		// Construction
		DateBehavior(CAMSEdit* pEdit);

	public:
		// Operations
		void SetDate(int nYear, int nMonth, int nDay);
		void SetDate(const CTime& date);
		void SetDate(const COleDateTime& date);
		void SetDateToToday();

		CTime GetDate() const;
		COleDateTime GetOleDate() const;

		int GetYear() const;
		int GetMonth() const;
		int GetDay() const;
		void SetYear(int nYear);
		void SetMonth(int nMonth);
		void SetDay(int nDay);
		virtual bool IsValid() const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
		void SetSeparator(TCHAR cSep);
		TCHAR GetSeparator() const;

		void ShowDayBeforeMonth(bool bDayBeforeMonth = true);
		bool IsDayShownBeforeMonth() const;

		enum Flags
		{
			None								= 0x0000,
			DayBeforeMonth						= 0x1000,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		// Helpers
		bool AdjustMaxMonthAndDay();
		bool AdjustMaxDay();

		int GetValidMonth() const;
		int GetMaxMonth() const;
		int GetMinMonth() const;
		int GetMonthStartPosition() const;
		TCHAR GetMaxMonthDigit(int nPos) const;
		TCHAR GetMinMonthDigit(int nPos) const;
		bool IsValidMonthDigit(TCHAR c, int nPos) const;
		bool IsValidMonth(int nMonth) const;

		int GetValidDay() const;
		int GetMaxDay() const;
		int GetMinDay() const;
		int GetDayStartPosition() const;
		TCHAR GetMaxDayDigit(int nPos) const;
		TCHAR GetMinDayDigit(int nPos) const;
		bool IsValidDayDigit(TCHAR c, int nPos) const;
		bool IsValidDay(int nDay) const;
		
		int GetValidYear() const;
		int GetYearStartPosition() const;
		TCHAR GetMaxYearDigit(int nPos) const;
		TCHAR GetMinYearDigit(int nPos, bool bValidYear = false) const;
		bool IsValidYearDigit(TCHAR c, int nPos) const;
		bool IsValidYear(int nYear) const;

		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
		CString GetFormattedDate(int nYear, int nMonth, int nDay) const;

	public:
		static bool IsLeapYear(int nYear);
		static CString GetString(int nValue, bool bTwoDigitWithLeadingZero = true);
		static int GetMaxDayOfMonth(int nMonth, int nYear);

	protected:
		// Attributes
		COleDateTime m_dateMin;
		COleDateTime m_dateMax;
		TCHAR m_cSep;
	};

	// The TimeBehavior class is used to allow the entry of time values.
	class TimeBehavior : virtual public Behavior
	{
	public:
		// Construction
		TimeBehavior(CAMSEdit* pEdit);

	public:
		// Operations
		void SetTime(int nHour, int nMinute, int nSecond = 0);
		void SetTime(const CTime& time);
		void SetTime(const COleDateTime& time);
		void SetTimeToNow();

		CTime GetTime() const;
		COleDateTime GetOleTime() const;

		int GetHour() const;
		int GetMinute() const;
		int GetSecond() const;
        CString GetAMPM() const;
		void SetHour(int nYear);
		void SetMinute(int nMonth);
		void SetSecond(int nDay);
		void SetAMPM(bool bAM);
		virtual bool IsValid() const;
		bool IsValid(bool bCheckRangeAlso) const;
		bool CheckIfValid(bool bShowErrorIfNotValid = true);

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
	
		void SetSeparator(TCHAR cSep);
		TCHAR GetSeparator() const;

		void Show24HourFormat(bool bShow24HourFormat = true);
		bool IsShowing24HourFormat() const;
		void ShowSeconds(bool bShowSeconds = true);
		bool IsShowingSeconds() const;
		void SetAMPMSymbols(const CString& strAM, const CString& strPM);
		void GetAMPMSymbols(CString* pStrAM, CString* pStrPM) const;
		
		enum Flags
		{
			None								= 0x0000,
			TwentyFourHourFormat				= 0x2000,
			WithSeconds							= 0x4000,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		// Helpers
		int GetValidHour(bool b24HourFormat = false) const;
		int GetMaxHour(bool b24HourFormat = false) const;
		int GetMinHour(bool b24HourFormat = false) const;
		int GetHourStartPosition() const;
		TCHAR GetMaxHourDigit(int nPos) const;
		TCHAR GetMinHourDigit(int nPos) const;
		bool IsValidHourDigit(TCHAR c, int nPos) const;
		bool IsValidHour(int nHour, bool b24HourFormat = false) const;
		int ConvertTo24Hour(int nHour, const CString& strAMPM) const;
		int ConvertToAMPMHour(int nHour, CString* pStrAMPM = NULL) const;

		int GetValidMinute() const;
		int GetMaxMinute() const;
		int GetMinMinute() const;
		int GetMaxSecond() const;
		int GetMinSecond() const;
		int GetMinuteStartPosition() const;
		TCHAR GetMaxMinuteDigit(int nPos) const;
		TCHAR GetMinMinuteDigit(int nPos) const;
		bool IsValidMinuteDigit(TCHAR c, int nPos) const;
		bool IsValidMinute(int nMinute) const;
		
		int GetValidSecond() const;
		int GetSecondStartPosition() const;
		TCHAR GetMaxSecondDigit(int nPos) const;
		TCHAR GetMinSecondDigit(int nPos) const;
		bool IsValidSecondDigit(TCHAR c, int nPos) const;
		bool IsValidSecond(int nSecond) const;

		void ShowAMPM();
		bool ChangeAMPM(TCHAR c);
		CString GetValidAMPM() const;
		int GetAMPMStartPosition() const;
		bool IsValidAMPM(const CString& strAMPM) const;
		int GetAMPMPosition(const CString& strText) const;

		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
		CString GetFormattedTime(int nHour, int nMinute, int nSecond, const CString& strAMPM = _T("")) const;
		void AdjustWithinRange();
		
	protected:
		// Attributes
		COleDateTime m_timeMin;
		COleDateTime m_timeMax;
		TCHAR m_cSep;
		CString m_strAM;
		CString m_strPM;
		int m_nAMPMLength;
		int m_nHourStart;
	};

	// The DateTimeBehavior class is used to allow the entry of date and time values.
	class DateTimeBehavior : public DateBehavior, 
							 public TimeBehavior
	{
	public:
		// Construction
		DateTimeBehavior(CAMSEdit* pEdit);

		void SetDateTime(int nYear, int nMonth, int nDay, int nHour, int nMinute, int nSecond = 0);
		void SetDateTime(const CTime& dt);
		void SetDateTime(const COleDateTime& dt);
		void SetToNow();

		CTime GetDateTime() const;
		COleDateTime GetOleDateTime() const;

		virtual bool IsValid() const;

		void SetRange(const CTime& dateMin, const CTime& dateMax);
		void SetRange(const COleDateTime& dateMin, const COleDateTime& dateMax);
		void GetRange(CTime* pDateMin, CTime* pDateMax) const;
		void GetRange(COleDateTime* pDateMin, COleDateTime* pDateMax) const;
	
		void SetSeparator(TCHAR cSep, bool bDate);
		TCHAR GetSeparator(bool bDate) const;

		void ModifyFlags(UINT uAdd, UINT uRemove);

		enum Flags
		{
			DateOnly							= 0x0100,
			TimeOnly							= 0x0200,

			OnKillFocus_Beep_IfInvalid			= 0x0001,
			OnKillFocus_Beep_IfEmpty			= 0x0002,
			OnKillFocus_Beep					= 0x0003,
			OnKillFocus_SetValid_IfInvalid		= 0x0004,
			OnKillFocus_SetValid_IfEmpty		= 0x0008,
			OnKillFocus_SetValid				= 0x000C,
			OnKillFocus_SetFocus_IfInvalid		= 0x0010,
			OnKillFocus_SetFocus_IfEmpty		= 0x0020,
			OnKillFocus_SetFocus				= 0x0030,
			OnKillFocus_ShowMessage_IfInvalid	= 0x0050,
			OnKillFocus_ShowMessage_IfEmpty		= 0x00A0,
			OnKillFocus_ShowMessage				= 0x00F0,
			OnKillFocus_Max						= 0x00FF
		};

	protected:
		virtual CString _GetValidText() const;
		virtual void _OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
		virtual void _OnKillFocus(CWnd* pNewWnd);

	protected:
		virtual bool IsValid(const COleDateTime& date, bool bDateOnly = true) const;
		virtual void ShowErrorMessage() const;
	};


	// Generated message map functions (for CAMSEdit)
protected:
	//{{AFX_MSG(CAMSEdit)
	//}}AFX_MSG
	afx_msg LONG OnCut(UINT wParam, LONG lParam);
	afx_msg LONG OnPaste(UINT wParam, LONG lParam);
	afx_msg LONG OnClear(UINT wParam, LONG lParam);
	afx_msg LONG OnSetText(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSAlphanumericEdit window

// The CAMSAlphanumericEdit is a CAMSEdit control which supports the AlphanumericBehavior class.
//
class CAMSAlphanumericEdit : public CAMSEdit, 
                             public CAMSEdit::AlphanumericBehavior
{
public:
	// Construction
	CAMSAlphanumericEdit(int nMaxChars = 0, const CString& strInvalidChars = _T("%'*\"+?><:\\"));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSAlphanumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSAlphanumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSMaskedEdit window

// The CAMSMaskedEdit is a CAMSEdit control which supports the MaskedBehavior class.
//
class CAMSMaskedEdit : public CAMSEdit, 
                       public CAMSEdit::MaskedBehavior
{
public:
	// Construction
	CAMSMaskedEdit(const CString& strMask = _T(""));

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSNumericEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class.
//
class CAMSNumericEdit : public CAMSEdit, 
                        public CAMSEdit::NumericBehavior
{
public:
	CAMSNumericEdit(int nMaxWholeDigits = 9, int nMaxDecimalPlaces = 4);

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSNumericEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSNumericEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSIntegerEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class
// restricted to only allow integer values.
//
class CAMSIntegerEdit : public CAMSNumericEdit
{
public:
	// Construction
	CAMSIntegerEdit(int nMaxWholeDigits = 9);

private:
	// Hidden members -- they don't make sense here
	void SetDouble(double dText, bool bTrimTrailingZeros = true);
	double GetDouble() const;
	void SetMaxDecimalPlaces(int nMaxDecimalPlaces);	// always 0

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSIntegerEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSIntegerEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSCurrencyEdit window

// The CAMSNumericEdit is a CAMSEdit control which supports the NumericBehavior class
// modified to put the '$' character in front of the value and use commas to separate the thousands.
//
class CAMSCurrencyEdit : public CAMSNumericEdit
{
public:
	// Construction
	CAMSCurrencyEdit();

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSCurrencyEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSCurrencyEdit)
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSDateEdit window

// The CAMSDateEdit is a CAMSEdit control which supports the DateBehavior class.
//
class CAMSDateEdit : public CAMSEdit, 
                     public CAMSEdit::DateBehavior
{
public:
	// Construction
	CAMSDateEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSDateEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSDateEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSTimeEdit window

// The CAMSTimeEdit is a CAMSEdit control which supports the TimeBehavior class.
//
class CAMSTimeEdit : public CAMSEdit, 
                     public CAMSEdit::TimeBehavior
{
public:
	// Construction
	CAMSTimeEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSTimeEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSTimeEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSDateTimeEdit window

// The CAMSDateTimeEdit is a CAMSEdit control which supports the 
// DateBehavior and TimeBehavior classes.
//
class CAMSDateTimeEdit : public CAMSEdit, 
						 public CAMSEdit::DateTimeBehavior
{
public:
	// Construction
	CAMSDateTimeEdit();

protected:
	virtual CString GetValidText() const;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSDateTimeEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSDateTimeEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////
// CAMSMultiMaskedEdit window

// The CAMSMultiMaskedEdit class is a CAMSEdit control which can support the 
// AlphanumericBehavior, NumericBehavior, MaskedBehavior, DateBehavior, or 
// TimeBehavior behavior classes.  It uses the mask to determine the current behavior.
//
class CAMSMultiMaskedEdit : public CAMSEdit, 
                            public CAMSEdit::AlphanumericBehavior,
                            public CAMSEdit::NumericBehavior,
                            public CAMSEdit::MaskedBehavior,
                            public CAMSEdit::DateTimeBehavior
{
public:
	CAMSMultiMaskedEdit();

	const CString& GetMask() const;
	void SetMask(const CString& strMask);
	
protected:
	virtual CString GetValidText() const;

	// Attributes
	Behavior* m_pCurrentBehavior;

public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAMSMultiMaskedEdit)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CAMSMultiMaskedEdit)
	afx_msg void OnChar(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyDown(UINT uChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	//}}AFX_MSG

	afx_msg LONG OnPaste(UINT wParam, LONG lParam);

	DECLARE_MESSAGE_MAP()
};


#define AMS_MIN_NUMBER			-1.7976931348623158e+308
#define AMS_MAX_NUMBER			 1.7976931348623158e+308
#define AMS_MIN_CTIME			CTime(1970, 1, 1, 0, 0, 0)
#define AMS_MAX_CTIME			CTime(2037, 12, 31, 23, 59, 59)
#define AMS_MIN_OLEDATETIME		COleDateTime(1900, 1, 1, 0, 0, 0)
#define AMS_MAX_OLEDATETIME		COleDateTime(9998, 12, 31, 23, 59, 59)
#define AMS_AM_SYMBOL			_T("AM")
#define AMS_PM_SYMBOL			_T("PM")


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_AMS_EDIT_H__AC5ACB94_4363_11D3_9123_00105A6E5DE4__INCLUDED_)
