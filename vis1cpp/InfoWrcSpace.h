// InfoWrcSpace.h: interface for the CInfoWrcSpace class.
#if !defined(AFX_INFOWRCSPACE_H__1FC854E4_039E_4C6D_B1CF_D5DC0223A4D9__INCLUDED_)
#define AFX_INFOWRCSPACE_H__1FC854E4_039E_4C6D_B1CF_D5DC0223A4D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <boost\shared_ptr.hpp>

#include "Info.h"
#include "DefFileInfo.h"

class CInfoWrcSpace : public CInfo  
{
public:
	CInfoWrcSpace();
	virtual ~CInfoWrcSpace();

	virtual HTREEITEM InsertInTree(HTREEITEM parent);
	
protected:
	virtual int  GetContextMenu(CStringList& list, DWORDPtrList& idflags);
	virtual void OnContextMenu(UINT cmd);
	virtual void GetPropertyPages(CPropertyPagePtr& pPropPage);

private:
	// TODO эти 3 функции, скорее всего, нужно перенести в другой класс типа CMetaDataOfClasses - посмотреть, как сделано в 1С++
	static void ReadAllAliasesFrom_OxyIniFile();
	static void ReadDefclsTextToModuleString( const CString& strDefClsName, CModuleString &strDefclsModuleText );
	static void AnalyzeAllClassesInfo( CModuleString& strDefclsModuleText, const CString& strDefClsName );
};

#endif // !defined(AFX_INFOWRCSPACE_H__1FC854E4_039E_4C6D_B1CF_D5DC0223A4D9__INCLUDED_)
