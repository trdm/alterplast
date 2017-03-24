// DeclProcInfo.h: interface for the CDeclProcInfo class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DECLPROCINFO_H__4002F420_6A3D_495F_8844_AF99FB62CFDC__INCLUDED_)
#define AFX_DECLPROCINFO_H__4002F420_6A3D_495F_8844_AF99FB62CFDC__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CDeclProcInfo : public CInfo  
{
public:
	CDeclProcInfo(const CString& proc, CMetaDataOfClasses::ARR_TYPE_NAME* pArr);
	virtual ~CDeclProcInfo();

	virtual void OnSelect();
	//virtual HTREEITEM InsertInTree(HTREEITEM parent);

private:
	CString m_ProcName;
	bool m_HasRetVal;
};

#endif // !defined(AFX_DECLPROCINFO_H__4002F420_6A3D_495F_8844_AF99FB62CFDC__INCLUDED_)
