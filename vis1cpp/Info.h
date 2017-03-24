// Info.h: interface for the CInfo class.

#if !defined(AFX_INFO_H__B3A69F75_449E_4520_B70C_74D79ECE565B__INCLUDED_)
#define AFX_INFO_H__B3A69F75_449E_4520_B70C_74D79ECE565B__INCLUDED_

#if _MSC_VER > 1000
	#pragma once
#endif // _MSC_VER > 1000

#include "cppview.h"

extern CDocument* OpenDocumentByPath(const CString& szPath);

// TODO номера констант используются для сортировки на одном уровне вложенности
 
enum InfoType
{
	TYPE_WRCSPACE_FOLDER		=0,
	TYPE_DEF_FILE_FOLDER		=1,
	TYPE_CLASS_FOLDER			=2,
	
	TYPE_BASE_CLASS_FOLDER		=3,
	TYPE_DECLARED_PROC			=4,
	TYPE_MODULE_PROC_EXPORT		=5,
	TYPE_MODULE_PROC_PRIVATE	=6,
	TYPE_MEMBER_EXPORT			=7,
	TYPE_MEMBER_PRIVATE			=8,
	
	TYPE_FROM_ALS_CLASS			=9,
	TYPE_FROM_MD_CLASS			=10,
	TYPE_HARD_PROC				=11,
	TYPE_HARD_MEMBER			=12,
	TYPE_FILE_FOLDER			=13,
	
	TYPE_LAST					=14,
};

// need specialized version for CPtrList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CPtrList, DWORD>;
typedef CTypedPtrList<CPtrList, DWORD> DWORDPtrList;

class CInfo  
{
public:
	//CInfo(const CString& name, UINT icon1, UINT icon2=-1);
	CInfo(const CString& name, InfoType icon1);
	virtual ~CInfo();

	void ShowProperties();
	
	virtual HTREEITEM InsertInTree(HTREEITEM parent);
	virtual void OnSelect() {}
	//virtual void OnUpdate(){}
	//virtual void OnExpanding(){}

protected:
	virtual int  GetContextMenu(CStringList& list, DWORDPtrList& idflags) { return -1; }
	virtual void OnContextMenu(UINT cmd) {}

	typedef boost::scoped_ptr<CPropertyPage> CPropertyPagePtr;
	virtual void GetPropertyPages(CPropertyPagePtr& pPropPage) {}

	void OpenKeysForClassInfoFromCurItem(const CString& strItemID);
	void AddSeparator(CStringList& list, DWORDPtrList& idflags);

public:
	static CInfo* GetData(HTREEITEM item);
	static PFNTVCOMPARE GetCallbackCompareFunction() { return m_IsSort ? CallbackCompare : CallbackCompareWithoutSort; }

	static bool IsSort() { return m_IsSort; }
	static void SortAllTree(bool bNeedSort);
	
	void ShowContextMenu(CPoint pt);
	
	const CString& GetDisplayName() const {return m_DisplayName;}

	InfoType GetType() const { return m_Type; }
	
	bool IsClassMember() const { return (GetType() > TYPE_DECLARED_PROC && GetType() < TYPE_FROM_ALS_CLASS); }
	
	static HTREEITEM GetCurItem() { return m_CurItem; }
	
	int Compare(const CInfo* pItem2) const
		{
			assert(pItem2);
			if(GetType() == pItem2->GetType())
				return GetDisplayName().CollateNoCase(pItem2->GetDisplayName());
			
			return GetType() - pItem2->GetType();
		}
	
	int CompareWithoutSort(const CInfo* pItem2) const // TODO подобрать более точное имя
		{
			assert(pItem2);
			if(GetType() > 2 && GetType() < 8) // TODO здесь явно должны быть константы типа EnumType и возможно, что <= 8 !!
				return GetDisplayName().CollateNoCase(pItem2->GetDisplayName());

			return Compare(pItem2);
		}

protected:
	CString m_DisplayName;
	
private:
	InfoType m_Type; //UINT m_Type;

	InfoType m_IconExpand; //UINT m_IconExpand;

	static HTREEITEM m_CurItem;
	static bool m_IsSort;
	
	static CInfo* GetParentData(HTREEITEM item)
	{
		CTreeCtrl& pCtrl = CCPPView::GetMyTreeCtrl();
		return GetData(pCtrl.GetParentItem(item));
	}
	
	static int CALLBACK CallbackCompareWithoutSort(LPARAM lParam1, LPARAM lParam2, 
		LPARAM lParamSort); // TODO подобрать более точное имя
	static int CALLBACK CallbackCompare(LPARAM lParam1, LPARAM lParam2, 
		LPARAM lParamSort);
};

class CClassMemberInfo : public CInfo  
{
public:
	//CClassMemberInfo(const CString& name, UINT icon1) : CInfo(name, icon1) {}
	CClassMemberInfo(const CString& name, InfoType icon1) : CInfo(name, icon1) {}
	~CClassMemberInfo() {}
	
	virtual void OnSelect();
};

// need specialized version for CPtrList because of AddHead/Tail ambiguity
template<> class CTypedPtrList<CPtrList, DWORD>
: public _CTypedPtrList<CPtrList, DWORD>
{
public:
	// Construction
	CTypedPtrList(int nBlockSize = 10)
		: _CTypedPtrList<CPtrList, DWORD>(nBlockSize) { }
	
	// add before head or after tail
	POSITION AddHead(TYPE newElement)
	{ return _CTypedPtrList<CPtrList, DWORD>::AddHead((void*)newElement); }
	POSITION AddTail(TYPE newElement)
	{ return _CTypedPtrList<CPtrList, DWORD>::AddTail((void*)newElement); }
	
	// add another list of elements before head or after tail
	void AddHead(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
	{ _CTypedPtrList<CPtrList, DWORD>::AddHead(pNewList); }
	void AddTail(CTypedPtrList<BASE_CLASS, TYPE>* pNewList)
	{ _CTypedPtrList<CPtrList, DWORD>::AddTail(pNewList); }
};

#endif // !defined(AFX_INFO_H__B3A69F75_449E_4520_B70C_74D79ECE565B__INCLUDED_)
