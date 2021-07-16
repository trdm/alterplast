/////////////////////////////////////////////////////////////////////////////
// SecCalc.h : header file for the SECCalculator and SECPopupCalculator
//             custom controls
//
// Stingray Software Extension Classes
// Copyright (C) 1996-1997 Stingray Software Inc,
// All rights reserved.
//
// This source code is only intended as a supplement to the
// Stingray Extension Classes 
// See the Stingray Extension Classes help files for 
// detailed information regarding using SEC classes.
//
//		Author:		Don			20/01/97
//
//  Updating Author:	Robert Oliver
//  Update Description: AutoDuck comments

#ifndef __SECCALC_H__
#define __SECCALC_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

// SEC Extension DLL
// Initialize declaration context
//

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


// SECCalculator styles
#define SEC_CS_RAISEDEDGE			0x0001L		// Same look as WS_EX_WINDOWEDGE
												// except compatible with Win 3.
#define SEC_CS_DIVIDER				0x0002L		// Divider at top of calculator

// SECPopupCalculator style
#define SEC_PCS_DESTROY_ON_EQUALS	0x0010L		// Popup calculator destroyed
												// everytime equals is pressed.

// Forward references ...
class SECCalcBtn;


/////////////////////////////////////////////////////////////////////////////
// SECCalculator window
//

/////////////////////////////
// AutoDuck tag block block for SECCalculator 

//@doc SECCalculator
//@mdata TCHAR      | SECCalculator | m_cLeadByte | Used by WM_CHAR for saving 
// first byte of multi-byte pair.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nState | Holds the current state 
//@xref <c SECCalculator> <md SECCalculator::StateFlags> 

//@doc SECCalculator
//@mdata double     | SECCalculator | m_val | The currently displayed value.
//@xref <c SECCalculator> <mf SECCalculator::SetValue> <mf SECCalculator::GetValue>

//@doc SECCalculator
//@mdata SECCalcOp  | SECCalculator | m_lastOp | The last operator entered. 
// This will be actioned when the next operator is entered.
//@xref <c SECCalculator> <md SECCalculator::SECCalcOp> 

//@doc SECCalculator
//@mdata BOOL       | SECCalculator | m_bReplace | The next digit entered 
// will replace the current displayed number
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nDP | Maximum number of decimal places 
// to display.
//@xref <c SECCalculator> <mf SECCalculator::SetDecimalPlaces> <mf SECCalculator::GetDecimalPlaces>

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nTotalDigits | Total number of 
// displayable digits.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata CRect      | SECCalculator | m_panelRect | The outer rectange of the 
// panel.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata CString    | SECCalculator | m_strDisplay | The current display 
// string for the panel.
//@xref <c SECCalculator> <mf SECCalculator::SetDisplayString>

//@doc SECCalculator
//@mdata CString    | SECCalculator | m_strDecSep | The decimal separator.
//@xref <c SECCalculator> <mf SECCalculator::LoadDecSeparator>

//@doc SECCalculator
//@mdata CFont      | SECCalculator | m_btnFont | The font used for drawing 
// buttons.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata CFont      | SECCalculator | m_panelFont | The font used for drawing 
// the panel.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata HBITMAP    | SECCalculator | m_hEqualsBmp | The bitmap handle used 
// to draw the equals button.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nBtnWidth | The width of a button (a 
// column) in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nBtnHeight | The height of a button 
// in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nVIndent | The vertical indent of 
// button matrix in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nHIndent | The horizontal indent of 
// button matrix in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nVSpace | The amount of space between 
// button rows in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nHSpace | The amount of space between 
// button columns in pixels.
//@xref <c SECCalculator> 

//@doc SECCalculator
//@mdata int        | SECCalculator | m_nCapture | The index of the button 
// with capture.
//@xref <c SECCalculator> 

// adinsert AutoDuck insertion point for SECCalculator 
//@doc SECCalculator 
//@class The SECCalculator class implements a calculator edit control capable 
// of decimal arithmetic and other standard operations.
//
//@comm See the sample CALC in the \OT\SAMPLES\STANDARD\CONTROLS\CALC directory 
// for a demonstration of this class.
//@base public | CWnd
class SECCalculator : public CWnd
{
	DECLARE_DYNAMIC(SECCalculator)

// Construction
public:

	//@access Creation/initialization

	//@cmember
	/* Constructs an SECCalculator object.*/
	SECCalculator();

	// Destructor.
	virtual ~SECCalculator();

	//@cmember
	/* Creates a window for the calculator and attaches it to the */
	// SECCalculator object.
	BOOL Create(DWORD dwStyle, int x, int y, CWnd* pParentWnd, UINT nID, 
				CCreateContext* pContext = NULL);

	//@cmember
	/* Creates a window with extended style attributes for the */
	// calculator and attaches it to the SECalculator object.
	BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, int x, int y, 
				  CWnd* pParentWnd, UINT nID, CCreateContext* pContext = NULL);

	//@access Attributes

public:
	
	//@cmember,menum
	/* List of calculator functions/buttons*/
	enum SECCalcOp
	{
		OpNull     = 0,			//@@emem No operation
		OpMultiply = 1,			//@@emem Multiply operator
		OpPlus     = 2,			//@@emem Plus operator
		OpMinus    = 3,			//@@emem Minus operator
		OpDivide   = 4,			//@@emem Divide operator
		OpPercent  = 5,			//@@emem Percentage operator
		OpEquals   = 6,			//@@emem Equals operator
		OpC        = 7,			//@@emem Clear function
		OpCE	   = 8,			//@@emem Clear error function
		Op0		   = 9,			//@@emem Decimal digit 0
		Op1        = 10,		//@@emem Decimal digit 1
		Op2        = 11,		//@@emem Decimal digit 2
		Op3        = 12,		//@@emem Decimal digit 3
		Op4        = 13,		//@@emem Decimal digit 4
		Op5        = 14,		//@@emem Decimal digit 5
		Op6        = 15,		//@@emem Decimal digit 6
		Op7        = 16,		//@@emem Decimal digit 7
		Op8        = 17,		//@@emem Decimal digit 8
		Op9        = 18,		//@@emem Decimal digit 9
		OpDP       = 19,		//@@emem Decimal place
		OpBack     = 20,		//@@emem Delete last character

		// Operations not yet implemented
		OpMPlus	   = 21,		// Memory add
		OpMMinus   = 22,		// Memory subtract
		OpM        = 23,		// Places current number into memory
		OpMRecall  = 24,		// Memory recall
		OpSqrt     = 25,		// Square root

		// Operations for user use only
		OpUser1    = 100,		//@@emem User operator 1
		OpUser2    = 101,		//@@emem User operator 2
		OpUser3    = 102,		//@@emem User operator 3
		OpUser4    = 103,		//@@emem User operator 4
		OpUser5    = 104,		//@@emem User operator 5
		OpUser6    = 105,		//@@emem User operator 6
		OpUser7    = 106,		//@@emem User operator 7
		OpUser8    = 107		//@@emem User operator 8
	} ;


	//@access Public Operations
		
public:

	//@cmember
	/* Returns the index of the button under the given point.*/
	int HitTest(CPoint point) const;

	//@cmember
	/* Invalidates the button at the given index.*/
	void InvalidateBtn(int nIndex, BOOL bErase = FALSE);

	//@cmember
	/* Invalidates the LCD panel.*/
	void InvalidatePanel(BOOL bErase = FALSE);

	//@cmember
	/* Resets the calculator back to a "zero" state.*/
	void ResetCalc();

	// 
	//@cmember
	/* Set the current calculator value.*/
	void   SetValue(double val);
	//@cmember
	/* Get the current calculator value.*/
	double GetValue() const;

	// 
	//@cmember
	/* Set the maximum number of decimal places to be displayed.*/
	void SetDecimalPlaces(int nDP);
	//@cmember
	/* Get the maximum number of decimal places to be displayed.*/
	int  GetDecimalPlaces() const;

	//@access Overridable

protected:

	//@cmember
	/* Creates the calculator’s buttons.*/
	virtual BOOL CreateBtns(CWnd* pParentWnd);
	
	//@cmember
	/* Creates the calculator’s LCD panel.*/
	virtual BOOL CreatePanel(CWnd* pParentWnd);
	
	//@cmember
	/* Handles events related to the calculator’s operations (add, */
	// subtract, etc.).
	virtual BOOL HandleEvent(SECCalcOp op);
	
	//@cmember
	/* Override to change the decimal separator displayed.*/
	virtual CString LoadDecSeparator();

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(SECCalculator)
	//}}AFX_VIRTUAL

	// Called when Windows or an application makes a request to repaint.
	afx_msg void OnPaint();

	// Called when a change is made in the system color setting. 
	afx_msg void OnSysColorChange();

	// Called when the user presses the left mouse button.
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);

	// Called when the user releases teh left mouse button.
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

	// Called when the mouse cursor moves. 
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// Called to inform CWnd to cancel any internal mode. 
	afx_msg void OnCancelMode();

	// Override to control the particular types of input to process.
	afx_msg UINT OnGetDlgCode();

	// Called when a keystroke translates to a nonsystem character. 
	afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Called when a nonsystem key is pressed.
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Called when the nonclient area needs to be painted.
	afx_msg void OnNcPaint();

	// Called when the size and position of the client area needs 
    // to be calculated.
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS FAR* lpncsp);

	//@access Protected Operations


#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

protected:	

	//@cmember
	/* Lays out the calculator buttons.*/
	CSize CalcLayout(DWORD dwStyle);

	//@cmember
	/* Formats the value into a displayable string.*/
	void SetDisplayString();

	//@cmember
	/* Presses the given button.*/
	void PressBtn(int nIndex);

	//@cmember
	/* Sizes the panel to the width of the calculator.*/
	void SizePanel();

	//@cmember
	/* Formats the current number to the supplied number */
	// of decimal places into the supplied buffer.
	void FormatNo(LPTSTR ptr, int nDP);

	//@cmember
	/* Returns the class style that this window "will" be registered with.*/
	virtual UINT GetClassStyle() const;

	//@cmember
	/* Draws the calculator.*/
	virtual void Draw(CDC& dc);

	//@cmember
	/* Draws the panel.*/
	void DrawPanel(CDC& dc);

	// Functions used for handling inputs
	//@cmember
	/* Handles a digit input.*/
	BOOL HandleDigit(SECCalcOp op);

	//@cmember
	/* Handles a decimal point input.*/
	BOOL HandleDecimalPoint(SECCalcOp op);
	
	//@cmember
	/* Handles a backspace input.*/
	BOOL HandleBackspace(SECCalcOp op);
	
	//@cmember
	/* Handles an operation (add, subtract, etc.).*/
	BOOL HandleOperator(SECCalcOp op);
	
	//@cmember
	/* Handles the 'Clear' operation.*/
	BOOL HandleClear(SECCalcOp op);
	
	//@cmember
	/* Handles the 'Clear Error' operation.*/
	BOOL HandleClearError(SECCalcOp op);

	//@cmember
	/* Returns the decimal character used by scanf, printf.*/
	TCHAR GetFormatDecimal();

	
	//@access Protected data members

protected:

	//@cmember,menum
	/* State flags*/
	enum StateFlags
	{
		Capture         = 0x0001,	//@@emem We currently have capture
		HaveLead        = 0x0002,	//@@emem Have received the first byte of a 
									// multi-byte pair
		HasInitialValue = 0x0004,	//@@emem Reset won't clear current value
		CalcError       = 0x0008	//@@emem Calculator in error (divide by zero etc)
	} ;

	// General data members
	//@cmember
	/* First input byte of multi-byte pair.*/
	TCHAR     m_cLeadByte;

	//@cmember
	/* The current state.*/
	int       m_nState;

	// Calculation engine data members
	//@cmember
	/* The currently displayed value.*/
	double    m_val;
	
	//@cmember
	/* The last operation entered.*/
	SECCalcOp m_lastOp;

	//@cmember
	/* Next digit will replace current display.*/
	BOOL      m_bReplace;

	//@cmember
	/* Max number of decimal places.*/
	int       m_nDP;

	//@cmember
	/* Total number of displayable digits.*/
	int       m_nTotalDigits;
	
	// Panel (LCD display) data members
	//@cmember
	/* The outer rectange of the panel.*/
	CRect     m_panelRect;

	//@cmember
	/* The current display string for the panel.*/
	CString   m_strDisplay;

	//@cmember
	/* The decimal separator.*/
	CString   m_strDecSep;

	// Drawing/layout data members
	//@cmember
	/* Font used for drawing buttons.*/
	CFont     m_btnFont;

	//@cmember
	/* Font used for drawing the panel.*/
	CFont     m_panelFont;

	//@cmember
	/* Handle of bitmap used on equals button.*/
	HBITMAP   m_hEqualsBmp;

	//@cmember
	/* Width of a button (a column).*/
	int       m_nBtnWidth;

	//@cmember
	/* Height of a button.*/
	int       m_nBtnHeight;

	//@cmember
	/* Vertical Indent of button matrix.*/
	int       m_nVIndent;

	//@cmember
	/* Horizontal Indent of button matrix.*/
	int       m_nHIndent;

	//@cmember
	/* Space between button rows.*/
	int       m_nVSpace;

	//@cmember
	/* Space between button columns*/
	int       m_nHSpace;

	//@cmember
	/* Index of button with capture*/
	int       m_nCapture;

	// We maintain an array of the calculator buttons, m_btns. This would
	// normally be defined with CTypedPtrArray, but templates are not supported
	// by the Microsoft 16-Bit compiler and some Unix compilers. The following
	// is what CTypedPtrArray would have defined.
	/////////////////////////////
	// AutoDuck tag block block for class 

	//@doc SECCalcBtnArray
	//@mfunc SECCalcBtn*  | SECCalcBtnArray | GetAt | Returns the SECCalcBtn 
	// at the specified index.
	//@parm int  | nIndex | Zero-based array index.
	//@xref <c SECCalcBtnArray>


	//@doc SECCalcBtnArray
	//@mfunc SECCalcBtn*&  | SECCalcBtnArray | ElementAt | Returns a temporary 
	// reference to the SECCalcBtn pointer within the array.
	//@parm int  | nIndex | Zero-based array index.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc void  | SECCalcBtnArray | SetAt | Sets the SECCalcBtn at the 
	// specified index. 
	//@parm int  | nIndex | Zerp-based array index;
	//@parm  SECCalcBtn*  | ptr | A pointer to a SECCalcBtn object.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc void  | SECCalcBtnArray | SetAtGrow | Sets the SECCalcBtn at 
	// the specified index, growing the array if necessary.
	//@parm int  | nIndex | Zero-based array index.
	//@parm  SECCalcBtn*  | newElement | SECCalcBtn object to add.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc int  | SECCalcBtnArray | Add | Adds a new SECCalcBtn to the end, 
	// growing the array by 1. 
	//@parm SECCalcBtn*  | newElement | SECCalcBtn object to add.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc int  | SECCalcBtnArray | Append | Add the contents of another 
	// SECCalcBtnArray to the end of this array.
	//@devnote This is a 32-bit api.
	//@parm const SECCalcBtnArray&  | src | SECCalcBtnArray object to append.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc void  | SECCalcBtnArray | Copy | Overwrites the elements with 
	// those of another SECCalcBtnArray.
	//@devnote  This is a 32-bit api.
	//@parm const SECCalcBtnArray&  | src | SECCalcBtnArray object to copy.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc void  | SECCalcBtnArray | InsertAt | Inserts a SECCalcBtn (or 
	// multiple copies one) at a specified index in an array. 
	//@parm int  | nIndex | Zero-based array index to insert at.
	//@parm  SECCalcBtn*  | newElement | SECCalcBtn object to insert.
	//@parmopt  int | nCount | 1 | Number of copies of this object to insert.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc void  | SECCalcBtnArray | InsertAt | Inserts all the elements 
	// from another SECCalcBtnArray collection, starting at the nStartIndex 
	// position. 
	//@parm int  | nStartIndex | Zero-based index to insert at.
	//@parm  SECCalcBtnArray*  | pNewArray | Pointer to SECCalcBtnArray 
	// source object.
	//@xref <c SECCalcBtnArray>

	//@doc SECCalcBtnArray
	//@mfunc SECCalcBtn*  | SECCalcBtnArray | operator[] | Index operator. 
	//@parm int  | nIndex | Zero-based array index. 
	//@xref <c SECCalcBtnArray>


	//@doc SECCalcBtnArray
	//@mfunc SECCalcBtn*&  | SECCalcBtnArray | operator[] | Index operator.
	//@parm int  | nIndex | Zero-based array index.
	//@xref <c SECCalcBtnArray>

	// adinsert AutoDuck insertion point for class 
	//@doc class 
	//@class SECCalcBtn is the base class for the buttons on a calculator edit 
	// control created by class SECCalculator.  An associated class, Attrib, 
	// defines the attributes used for drawing a button on the calculator.
	//
	// The SECCalculator class implements a calculator edit control capable of 
	// decimal arithmetic and other standard operations.  The popup window for 
	// the calculator is created by the SECPopupCalculator class.
	//
	//@comm See the sample CALC in the \OT\SAMPLES\STANDARD\CONTROLS\CALC 
	// directory for a demonstration of this class.
	//@base public | CObArray
	class SECCalcBtnArray : public CObArray
	{
	public:
		// Accessing elements

		//@cmember
		/* Returns the SECCalcBtn at the specified index.*/
		SECCalcBtn* GetAt(int nIndex) const
			{ return (SECCalcBtn*)CObArray::GetAt(nIndex); }
		
		//@cmember
		/* Returns a temporary reference to the SECCalcBtn pointer within the array. */
		SECCalcBtn*& ElementAt(int nIndex)			
			{ return (SECCalcBtn*&)CObArray::ElementAt(nIndex); }
		
		//@cmember
		/* Sets the SECCalcBtn at the specified index. */
		void SetAt(int nIndex, SECCalcBtn* ptr)			
			{ CObArray::SetAt(nIndex, (CObject*) ptr); }

		// Potentially growing the array
		//@cmember
		/* Sets the SECCalcBtn at the specified index,*/
		// growing the array if necessary.
		void SetAtGrow(int nIndex, SECCalcBtn* newElement)		   
			{ CObArray::SetAtGrow(nIndex, (CObject*) newElement); }
		
		//@cmember
		/* Adds a new SECCalcBtn to the end, growing the array by 1. */
		int Add(SECCalcBtn* newElement)		   
			{ return CObArray::Add((CObject*) newElement); }

#ifdef WIN32
		
		//@cmember
		/* Add the contents of another SECCalcBtnArray to the end of this array. */
		int Append(const SECCalcBtnArray& src)		   
			{ return CObArray::Append(src); }
		
		//@cmember
		/* Overwrites the elements with those of another SECCalcBtnArray.*/
		void Copy(const SECCalcBtnArray& src)			
			{ CObArray::Copy(src); }

#endif

		// Operations that move elements around
		//@cmember
		/* Inserts a SECCalcBtn (or multiple copies of one) at a */
		// specified index in an array. 
		void InsertAt(int nIndex, SECCalcBtn* newElement, int nCount = 1)			
			{ CObArray::InsertAt(nIndex, (CObject*) newElement, nCount); }
		
		//@cmember
		/* Inserts all the elements from another SECCalcBtnArray collection,*/
		// starting at the nStartIndex position. 
		void InsertAt(int nStartIndex, SECCalcBtnArray* pNewArray)		   
			{ CObArray::InsertAt(nStartIndex, pNewArray); }

		// overloaded operator helpers
		//@cmember
		/* Index operator.*/
		SECCalcBtn* operator[](int nIndex) const			
			{ return (SECCalcBtn*)CObArray::operator[](nIndex); }
		
		//@cmember
		/* Index operator.*/
		SECCalcBtn*& operator[](int nIndex)			
			{ return (SECCalcBtn*&)CObArray::operator[](nIndex); }
	
	} m_btns;



	// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

};


/////////////////////////////////////////////////////////////////////////////
// SECPopupCalculator window
//
/////////////////////////////
// AutoDuck tag block block for SECPopupCalculator 

//@doc SECPopupCalculator
//@mdata BOOL  | SECPopupCalculator | m_bAutoDelete | Deletes the calculator 
// object with the window if set to TRUE.


// adinsert AutoDuck insertion point for SECPopupCalculator 
//@doc SECPopupCalculator 
//@class The SECPopupCalculator class creates a popup window for a calculator 
// edit control.  The popup window is destroyed whenever the equals key is 
// pressed.  SECPopCalendar is derived from the SECCalculator class which 
// implements a calculator edit control capable of decimal arithmetic and 
// other standard operations.
//
//@comm The base class of the buttons on the calculator is <c SECCalcBtn>.  The class 
// Attrib defines the attributes used for drawing the calculator button.
//
// See the sample CALC in the \OT\SAMPLES\STANDARD\CONTROLS\CALC directory 
// for a demonstration of this class.
//@base public | SECCalculator
class SECPopupCalculator : public SECCalculator
{
	DECLARE_DYNAMIC(SECPopupCalculator)

	//@access Creation/Initialization

public:

	//@cmember
	/* Creates a SECPopupCalculator object.*/
	SECPopupCalculator();

	// Destructor.
	virtual ~SECPopupCalculator();

	//@cmember
	/* Creates a popup window for the calculator and attaches it */
	// to the SECPopupCalculator object.
	BOOL Create(DWORD dwStyle, const RECT& ctrlRect, CWnd* pParentWnd, 
				CCreateContext* pContext = NULL);

	//@cmember
	/* Creates a popup window with extended styles for the calculator */
	// and attaches it to the SECPopupCalculator object.
	BOOL CreateEx(DWORD dwExStyle, DWORD dwStyle, const RECT& ctrlRect, 
				  CWnd* pParentWnd, CCreateContext* pContext = NULL);


	//@access Attributes

protected:
	//@cmember
	/* Returns the class style that this window "will" be */
	// registered with
	virtual UINT GetClassStyle() const;

	
	//@access Overridable

public:

	//@cmember
	/* Called when a button is pressed on the calculator.*/
	virtual BOOL HandleEvent(SECCalcOp op);

	//@cmember
	/* Handles dispatching the result to interested windows*/
	virtual void Notify();

protected:

	// Called immediately before losing the input focus. 
	afx_msg void OnKillFocus(CWnd* pNewWnd);

	// Called when a nonsystem key is pressed.
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	// Called by the default OnNcDestroy member function after the 
    // window has been destroyed. 
	virtual void PostNcDestroy();

	//@access Public data members

public:
	
	//@cmember
	/* Delete with the window.*/
	BOOL m_bAutoDelete;

public:

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

// Implementation - message map and entries
	DECLARE_MESSAGE_MAP()

};



/////////////////////////////////////////////////////////////////////////////

// Function to convert Dialog Base Units to pixels.
CSize SECConvertDBU(int x, int y, HFONT hFont = NULL);


//
// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //WIN32

#endif //__SECCALC_H__

