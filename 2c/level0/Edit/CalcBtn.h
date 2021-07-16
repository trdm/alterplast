/////////////////////////////////////////////////////////////////////////////
// CalcBtn.h : header file for SECCalcBtn - the base class for SECCalculator
//             buttons.
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

#ifndef __CALCBTN_H__
#define __CALCBTN_H__

//
// Header include diagnostics
//
#if defined(WIN32) && defined(SEC_DBG_HEADER_DIAGNOSTICS)
	#pragma message ( __TIME__ ":  include " __FILE__ )
#endif

#ifdef WIN32

#ifndef __SECCALC_H__
#include "SecCalc.h"
#endif

#ifdef _SECDLL
	#undef AFXAPP_DATA
	#define AFXAPP_DATA AFXAPI_DATA
	#undef AFX_DATA
	#define AFX_DATA    SEC_DATAEXT
#endif //_SECDLL


/////////////////////////////////////////////////////////////////////////////
// SECCalcBtn object
//
/////////////////////////////
// AutoDuck tag block block for SECCalcBtn 

//@doc SECCalcBtn
//@mdata Attrib | SECCalcBtn | m_attribs | Button attributes.

//@doc SECCalcBtn
//@mdata CString | SECCalcBtn | m_strCaption | Button caption.

//@doc SECCalcBtn
//@mdata CString | SECCalcBtn | m_strAccels | Standard keyboard accelerators.

//@doc SECCalcBtn
//@mdata UINT | SECCalcBtn | m_nKeyDownAccel | WM_KEYDOWN accelerators.

//@doc SECCalcBtn
//@mdata int | SECCalcBtn | m_nWidth | Width of button.

//@doc SECCalcBtn
//@mdata CRect | SECCalcBtn | m_rect | Rectangle of button (relative to calculators client).

//@doc SECCalcBtn
//@mdata int | SECCalcBtn | m_nState | State flags (see state bits).

//@doc SECCalcBtn
//@mdata SECCalculator::SECCalcOp  | SECCalcBtn | m_op | The operation of this button.

// adinsert AutoDuck insertion point for SECCalcBtn 
//@doc SECCalcBtn 
//@class SECCalcBtn is the base class for the buttons on a calculator edit 
// control created by class SECCalculator.  
//@comm An associated class, <c Attrib>, 
// defines the attributes used for drawing a button on the calculator.
//
// The SECCalculator class implements a calculator edit control capable of decimal 
// arithmetic and other standard operations.  The popup window for the calculator 
// is created by the SECPopupCalculator class.
//
// See the sample CALC in the \OT\SAMPLES\STANDARD\CONTROLS\CALC directory for a demonstration 
// of this class.
//@base public | CObject
class SECCalcBtn : public CObject
{

	//@access Creation/initialization

public:
	//@cmember
	/* Constructs a SECCalcBtn object.*/
	SECCalcBtn();

	virtual ~SECCalcBtn();


	// Defines the attributes used for drawing a calculator button
	/////////////////////////////
	// AutoDuck tag block block for Attrib
	//@doc SECCalcBtn
	//@mdata HFONT     | SECCalcBtn | m_hFont | The font handle attribute.

	//@doc SECCalcBtn
	//@mdata HBITMAP*  | SECCalcBtn | m_lphBitmap | The bitmap handle attribute.

	//@doc SECCalcBtn
	//@mdata COLORREF  | SECCalcBtn | m_textColor | The text color attribute.

	//@doc SECCalcBtn
	//@mdata DWORD     | SECCalcBtn | m_dwStyle | The style attribute (not currently used).

	// adinsert AutoDuck insertion point for Attrib
	//@doc Attrib
	//@class This is a helper class that defines the attributes used for 
	// drawing a calculator button.
	//@comm
	// The class Attrib is associated with the class <c SECalcBtn>, which 
	// is the base class for the buttons on a calculator edit control.  Attrib 
	// defines the attributes used for drawing a button on the calculator.
	//
	// The SECCalculator class implements a calculator edit control capable of 
	// decimal arithmetic and other standard operations.  The popup window for 
	// the calculator is created by the <c SECPopupCalculator> class.
	//
	// See the sample CALC in the \OT\SAMPLES\STANDARD\CONTROLS\CALC directory for a demonstration 
	// of this class.
	// @base public | CObject
	class Attrib
	{
		//@access Creation/initialization

	public:
		//@cmember
		/* Constructs an Attrib object.*/
		Attrib();

		//@cmember
		/* Constructs an Attrib object.*/
		Attrib(DWORD dwStyle, COLORREF textColor = 0L, HFONT hFont = NULL, 
			   HBITMAP* lphBitmap = NULL);

		//@cmember
		/* Constructs an Attrib object.*/
		Attrib(const Attrib& attrib);

		//@cmember
		/* Attrib destructor.*/
		~Attrib();

		//@access Attributes

		//@cmember
		/* Retrieves the font attribute.*/
		HFONT GetFont() const;

		//@cmember
		/* Retrieves the text color attribute.*/
		COLORREF GetTextColor() const;

		//@cmember
		/* Retrieves the bitmap attribute.*/
		HBITMAP GetBitmap() const;
	
		//@access Operations

		//@cmember
		/* Equals operator.*/
		const Attrib& operator=(const Attrib& attrib);

		//@access Protected data members

	protected:
		//@cmember
		/* The font handle attribute.*/
		HFONT    m_hFont;

		//@cmember
		/* The bitmap handle attribute.*/
		HBITMAP* m_lphBitmap;

		//@cmember
		/* The text color attribute.*/
		COLORREF m_textColor;

		//@cmember
		/* The style attribute (not currently used).*/
		DWORD    m_dwStyle;
	};
	
public:

	// Initialise button with accelerators same as button text
	BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
					LPCTSTR lpszCaption, int nWidth = 1, 
					BOOL bStartsRow = FALSE);

	// Initialise button with single character accelerator
	BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
					LPCTSTR lpszCaption, TCHAR cAccel, int nWidth = 1, 
					BOOL bStartsRow = FALSE);

	// Initialise button with different accelerators to button text
	BOOL Initialise(const Attrib& attrib, SECCalculator::SECCalcOp op, 
					LPCTSTR lpszCaption, LPCTSTR lpszAccels, int nWidth = 1, 
					BOOL bStartsRow = FALSE);

	//@access Attributes

public:
	
	//@cmember
	/* Returns the width of the button.*/
	BOOL GetWidth() const;

	//@cmember
	/* Returns the operation code for this button.*/
	SECCalculator::SECCalcOp GetOp() const;

	//@cmember
	/* Get the button state.*/
	int  GetState() const;

	//@cmember
	/* Set the button state.*/
	void SetState(int nState);

	//@cmember
	/* Tests whether a character is an accelerator for the button.*/
	BOOL IsAccelerator(LPCTSTR lpszChar) const;

	//@cmember
	/* Tests whether the WM_KEYDOWN char is an accelerator for the button.*/
	BOOL IsKeyDownAccelerator(UINT nChar) const;

	//@cmember
	/* Sets the WM_KEYDOWN accelerator for this button.*/
	void SetKeyDownAccel(UINT nChar);

	//@cmember
	/* Get the rectange of the button (relative to calculators client).*/
	const CRect& GetRect() const;

	//@cmember
	/* Set the rectange of the button (relative to calculators client).*/
	void SetRect(const RECT& rect);


	//@access Overrides
public:
	
	//@cmember
	/* Draws the button!*/
	virtual void Draw(CDC& dc);

// Implementation

#ifdef _DEBUG
	virtual void Dump(CDumpContext& dc) const;
	virtual void AssertValid() const;
#endif

	//@access Public data members

public:
	// enum State values
	//@cmember,menum
	/* State values*/
	enum
	{
		Down      = 0x0001,						//@@emem Button is currently down
		StartsRow = 0x0002						//@@emem Button starts a new row
	};

	//@access Protected data members

protected:
	
	//@cmember
	/* Button attributes.*/
	Attrib		             m_attribs;
	
	//@cmember
	/* Button caption.*/
	CString                  m_strCaption;
	
	//@cmember
	/* Standard keyboard accelerators.*/
	CString                  m_strAccels;
	
	//@cmember
	/* WM_KEYDOWN accelerators.*/
	UINT		             m_nKeyDownAccel;
	
	//@cmember
	/* Width of button.*/
	int                      m_nWidth;
	
	//@cmember
	/* Rectangle of button (relative to calculators client).*/
	CRect		             m_rect;

	//@cmember
	/* State flags (see state bits).*/
	int                      m_nState;
	
	//@cmember
	/* The operation of this button.*/
	SECCalculator::SECCalcOp m_op;
};

// SEC Extension DLL
// Reset declaration context
//

#undef AFX_DATA
#define AFX_DATA
#undef AFXAPP_DATA
#define AFXAPP_DATA NEAR

#endif //WIN32

#endif //__CALCBTN_H__

