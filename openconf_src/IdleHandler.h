// IdleHandler.h: interface for the CIdleHandler class.
#if !defined(AFX_IDLEHANDLER_H__91DF20CF_7756_4A6F_A263_90A75AF35899__INCLUDED_)
#define AFX_IDLEHANDLER_H__91DF20CF_7756_4A6F_A263_90A75AF35899__INCLUDED_
#include "HackVirtual.h"

class CIdleHandler:public CWinApp
{
public:
	CRecentFileList* GetRFL(){return m_pRecentFileList;}
	BOOL OnIdleWrap(LONG lCount);
	static void SetIdleHandler(void(*pFunc)());
	static void RemoveIdleHandler(void(*pFunc)());

	static CTrapVirtual m_trap;
	static void Init();
	static void Done();
	static CMapPtrToPtr m_IdleFunc;
};

#endif // !defined(AFX_IDLEHANDLER_H__91DF20CF_7756_4A6F_A263_90A75AF35899__INCLUDED_)
