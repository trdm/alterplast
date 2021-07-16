#if !defined(__PROMPTEDIT_H__)
#define __PROMPTEDIT_H__

/////////////////////////////////////////////////////////////////////////
// created:		2002/08/22																							
// created:		22:8:2002   21:39																				
// filename:	H:\MyProjects\C++\Benchmark\ConsoleTest\PromptEdit.h
// file path:	H:\MyProjects\C++\Benchmark\ConsoleTest									
// file base:	PromptEdit																							
// file ext:	h																												
// author:		Tanzim Husain [tanzim@mail.com]													
//																																		
// purpose:		A simple but handy edit that contains a prompt that 		
//						goes away when the edit gains focus
//.						
//						Permission to freely distribute, modify and blah blah...
//						as long as this text/header is included. 
//						If this is used in a commercial project, just drop me a line 
//						about the project :)
/////////////////////////////////////////////////////////////////////////

// The way prompt text will be displayed, the foreground color
enum PromptShowModeEnum
{
	pmNormal, // COLOR_WINDOWTEXT
	pmHighlight, // COLOR_HIGHLIGHT
	pmGray, // COLOR_GRAYTEXT
	pmHot // COLOR_HOTLIGHT or BLUE depending on the Platform
};

/////////////////////////////////////////////////////////////////////////////
// CCEdit window

class CPromptEdit : public CEdit
{
public:		
	CPromptEdit(LPCTSTR lpszPrompt = _T("<Enter Text Here>"), 
							bool bRegainPrompt = true,
							PromptShowModeEnum PromptShowMode = pmNormal);
	virtual ~CPromptEdit(){};

public:	
	// Set the prompt text
	virtual void SetPromptText(LPCTSTR lpszPrompt);
	virtual void SetPromptText(LPCTSTR lpszPrompt, bool bRegainPrompt, PromptShowModeEnum PromptShowMode);
	
	// Get the current prompt text
	virtual CString GetPromptText() const;
	
	// Check whether the edit is currently displaying the prompt text
	virtual bool GetPromptTextActive();
	
	// Specify whether the prompt text will be re shown, when focus is 
	// lost, if the user did not change the content of the edit
	virtual void SetRegainPrompt(bool bRegainPrompt = true);
	
	// Check whether prompt regaining is enabled
	virtual bool GetRegainPrompt() const;
	
	// Set the prompt mode
	virtual void SetPromptShowMode(PromptShowModeEnum PromptShowMode);
	
	// Get the prompt mode
	virtual PromptShowModeEnum GetPromptShowMode() const;

	// Get a the edit text trimmed, returns the length of the trimmed text.
	// Although this method should not be public, I've found it very handy
	// and expect other users will find it handy too!
	virtual int GetWindowTextTrimmed(CString& strWindowTextTrimmed);

protected:
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CPromptEdit)
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL	

protected:
	//{{AFX_MSG(CPromptEdit)
	afx_msg BOOL OnSetfocus();
	afx_msg BOOL OnKillfocus();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void OnSysColorChange();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()	

private:
	void UpdatePromptShowMode();

private:
	CString m_strPrompt;
	bool m_bRegainPrompt;
	PromptShowModeEnum m_PromptShowMode;	
	COLORREF m_crForeground;
};

#endif
