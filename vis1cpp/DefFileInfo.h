// DefFileInfo.h: interface for the CDefFileInfo class.
#if !defined(AFX_DEFFILEINFO_H__183C9F0E_8992_4F34_B0FB_D09AEF82B32B__INCLUDED_)
#define AFX_DEFFILEINFO_H__183C9F0E_8992_4F34_B0FB_D09AEF82B32B__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

#include "Info.h"
#include "ClassInfo.h"

class CDefFileInfo : public CInfo  
{
public:
	typedef boost::shared_ptr<CDefFileInfo> CPtr; // TODO перенести в другой модуль или класс
	typedef boost::weak_ptr<CDefFileInfo> CWeakPtr; // TODO перенести в другой модуль или класс

	static CPtr FindElem(const CString& strDefPath);

	virtual ~CDefFileInfo();

	virtual void OnSelect();
	
	virtual HTREEITEM InsertInTree(HTREEITEM parent);
	
	CDocument* Open() const;
	
	const CString& GetPath() const { return m_path; }
	
	// TODO artbear - к сожалению, в VC 6 не работает способ определения подобной функции-шаблона вне тела класса
	template<typename UnaryOp>
		void ForEachChildClassInfoIntoDefFile(UnaryOp op)
	{
		m_ChildClasses.ForEachValue(op);
	}

	// TODO artbear - к сожалению, в VC 6 не работает способ определения подобной функции-шаблона вне тела класса
	template<typename UnaryOp>
		static void ForEach(UnaryOp op)
	{
		m_ElementsMap.ForEachValue(op);
	}

	static void CreateListOfClassesNameForEachDefFile();
	static void CreateAndInsertInTreeForEachDefFile( HTREEITEM item );
	static void RemoveAllDefFiles();

	static void UpdateLinksForEachDefFile();
	
protected:
	virtual void GetPropertyPages(CPropertyPagePtr& pPropPage);
	virtual int  GetContextMenu(CStringList& list, DWORDPtrList& idflags);
	virtual void OnContextMenu(UINT cmd);

private:
	typedef boost::shared_ptr<CStringList> CStringListPtr;

	CString m_path;
	CStringListPtr m_pClassesNameList;
	
	CIStringMap<CClassInfo::CWeakPtr, CClassInfo::CWeakPtr&> m_ChildClasses;

	static CIStringMap<CStringListPtr, CStringListPtr&> m_mapDef;
	static CIStringMap<CPtr, CPtr&> m_ElementsMap;

	static CPtr CreateElem( const CString& DefFilePath, const CStringListPtr& pClassesNameList );

	CDefFileInfo(const CString& path, const CStringListPtr& pClassesNameList);

	CString EvalDisplayName(const CString& path);
	void CreateAndInsertInTreeForEachClassInDefFile( HTREEITEM item );

	static void UpdateLinksForEachDefFileInner( const CDefFileInfo::CPtr& pClassInfo );
	void UpdateLinks();

	static void UpdateLinksForClassInfo(const CClassInfo::CWeakPtr& pClassInfo);
};

#endif // !defined(AFX_DEFFILEINFO_H__183C9F0E_8992_4F34_B0FB_D09AEF82B32B__INCLUDED_)
