// CodeJump.h: interface for the CCodeJump class.
#ifndef AFX_CODEJUMP_H
#define AFX_CODEJUMP_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CTxtTrap;

class CCodeJump  
{
public:
	void JumpToLine(CTextDocument* pDoc,int line);
	void SaveCurrentPos(CTxtTrap* pEditor);
	void JumpFromTo(CTxtTrap* pEditor,CTextDocument* pDoc,int line);
	void Jump(CTextDocument* pDoc,CPoint pt1,CPoint pt2);
	void Jump(int pos);
	void ShowStack();
	void Forward();
	void Back();
	static CCodeJump* GetCodeJump();
	void Jump(CTxtTrap* pEditor);
	CCodeJump();
	virtual ~CCodeJump();
	struct SBackState{
		CString title;
		CString word;
		int modulID;
		CString data;
		HWND hWnd;
		CPoint point;
	};
	CPtrArray m_stack;
	int m_CurPos;
	int m_TopPos;

protected:
	static CCodeJump* m_OneCodeJump;
	static CMapStringToPtr m_TypesOfModuls;
};

#endif
