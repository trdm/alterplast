// DerivClsFolder.h: interface for the CDerivClsFolder class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DERIVCLSFOLDER_H__BD95379E_A3F0_4A19_A9CB_302FD1FC34C8__INCLUDED_)
#define AFX_DERIVCLSFOLDER_H__BD95379E_A3F0_4A19_A9CB_302FD1FC34C8__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Info.h"

class CDerivClsFolderInfo : public CInfo  
{
public:
	CDerivClsFolderInfo():CInfo("Производные_классы",TYPE_DERIV_CLASS_FOLDER){}
	virtual ~CDerivClsFolderInfo();
	HTREEITEM InsertInTree(HTREEITEM parent,HTREEITEM after=TVI_FIRST);

};

#endif // !defined(AFX_DERIVCLSFOLDER_H__BD95379E_A3F0_4A19_A9CB_302FD1FC34C8__INCLUDED_)
