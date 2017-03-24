// ClassInfo.h: interface for the CClassInfo class.
#if !defined(AFX_CLASSINFO_H__B17668A4_C46F_46F2_9ECF_600073C316E1__INCLUDED_)
	#define AFX_CLASSINFO_H__B17668A4_C46F_46F2_9ECF_600073C316E1__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include <boost\shared_ptr.hpp>
#include <boost\weak_ptr.hpp>

#include "Info.h"
#include "cppview.h"
#include "moduleparser.h"

//#include "DefFileInfo.h"

// TODO отрефакторить MAP_Str_Info
typedef std::map<CString,CInfo*,CompNoCase> MAP_Str_Info;
typedef MAP_Str_Info::iterator Str_Info_ITER;

class CDefFileInfo;

class CClassInfo : public CInfo  
{
private:
	CClassInfo(const CString& name);
public:	
	typedef boost::shared_ptr<CClassInfo> CPtr; // TODO перенести в другой модуль или класс
	typedef boost::weak_ptr<CClassInfo> CWeakPtr; // TODO перенести в другой модуль или класс
	
	typedef boost::shared_ptr<CDefFileInfo> CDefFileInfoPtr; // TODO перенести в другой модуль или класс
	typedef boost::weak_ptr<CDefFileInfo> CDefFileInfoWeakPtr; // TODO перенести в другой модуль или класс

	static CClassInfo::CPtr FindElem(const CString& ClassName);

	virtual ~CClassInfo();

	void UpdateLinks();

	void ClearRemoved();
	void InsertKeys(HTREEITEM item);
	void PrepareKeys();
	void ClearKeys();
	void OpenKeys(const CString& name);

	CDocument* OpenClass() const;
	virtual HTREEITEM InsertInTree(HTREEITEM parent);
	
	const CString& GetPath() const { return m_ImplementPath; }
	const CString& GetDefFilePath() const { return m_DefinePath; }
	const CPtrList& GetNodesList() const { return m_nodes; }

	CDefFileInfoPtr GetDefFile() const { return m_pDefFileInfo.lock(); }
	
	// TODO artbear - к сожалению, в VC 6 не работает способ определения подобной функции-шаблона вне тела класса
	template<typename UnaryOp>
		void ForEachChildClass(UnaryOp op)
	{
		m_ChildClasses.ForEachValue(op);
	}
	
	// TODO artbear - к сожалению, в VC 6 не работает способ определения подобной функции-шаблона вне тела класса
	template<typename UnaryOp>
		static void ForEachClassInfo(UnaryOp op)
		{
			m_ClassInfoMap.ForEachValue(op);
		}

	static void RemoveAllFileNameInMap(void)
		{
			m_ClassFileNameMap.RemoveAll();
			m_ClassInfoMap.RemoveAll();
		}
	static int LookupByFileName(const CString& strDocFullPath, CClassInfo*& pInfo)
		{
			return CClassInfo::m_ClassFileNameMap.Lookup(strDocFullPath, pInfo);
		}

	void SetSourceText(const CString& txt)
		{
			m_SourceText = txt;
		}
	
	CString GetSourceText(void)
		{
			if (!m_SourceText.IsEmpty())
				return m_SourceText;
			
			return m_mod.GetSourceText();
		}
	
	void CompileSomeClassesWithFlagCompileAll();

private:
	virtual void GetPropertyPages(CPropertyPagePtr& pPropPage);

	void OpenDef();
	virtual void OnSelect();
	
	virtual int  GetContextMenu(CStringList& list, DWORDPtrList& idflags);
	virtual void OnContextMenu(UINT cmd);

	void InsertBaseClassesInTree( HTREEITEM item );

	CString m_ImplementPath;
	CString m_DefinePath;
	
	MAP_Str_Info m_keys;
	MAP_Str_Info m_removedKeys;
	MAP_Str_Info m_declProc;
	
	CModuleParser m_mod;
	CPtrList m_nodes;

	static CIStringMap<CPtr, CPtr&> m_ClassInfoMap;

	static CIStringMap<CClassInfo*, CClassInfo*> m_ClassFileNameMap;

	// если пусто, значит, в файле
	// если нет, значит, брать текст класса из этого поля
	// используется для разделения текста с помощью препроцессора 
	//	(например, несколько классов в одном файле)
	CString m_SourceText;

	CDefFileInfoWeakPtr m_pDefFileInfo;

	CMetaDataOfClasses::CBaseClassesListPtr m_BaseClassList;
	CIStringMap<CWeakPtr, CWeakPtr&> m_ChildClasses;
};

#endif // !defined(AFX_CLASSINFO_H__B17668A4_C46F_46F2_9ECF_600073C316E1__INCLUDED_)
