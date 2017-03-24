// MyModule.h: interface for the CMyModule class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MYMODULE_H__8B8230EA_F4A9_42AB_B82C_5CF169D45856__INCLUDED_)
#define AFX_MYMODULE_H__8B8230EA_F4A9_42AB_B82C_5CF169D45856__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CClassInfo;
class CMyModule : public CBLModule7  
{
public:
	CMyModule():CBLModule7(NULL, NULL),m_pInfo(NULL){};
	CMyModule(const char* pSrc, CClassInfo* m_pInfoA = NULL):CBLModule7(NULL,pSrc),m_pInfo(m_pInfoA){};
	virtual ~CMyModule();

	
	int				OnSyntaxError(void);

	CClassInfo* m_pInfo;
};

#endif // !defined(AFX_MYMODULE_H__8B8230EA_F4A9_42AB_B82C_5CF169D45856__INCLUDED_)
