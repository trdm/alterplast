// TeleCont.h: interface for the CTeleCont class.
#ifndef TELECONT_H
#define TELECONT_H
#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
class CParser;
#include "telegroup.h"

class CTeleCont  
{
public:
	CTeleCont(CParser* pParser);
	void InitGroups();
	virtual ~CTeleCont();
	
	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const=0;
	virtual LPCTSTR*	GetGroupNames() const=0;

	virtual void Clear();
	virtual void FillGroup(CPtrArray& arr)	= 0;
	
	CParser* m_pParser;
	bool m_builded;
	CTeleGroup** m_Group;
};


#endif // !defined(AFX_TELECONT_H__1978ECB0_8F8A_4B52_930D_6C39BA3172A0__INCLUDED_)
