// IAllTypedObj.h: interface for the CIAllTypedObj class.
#if !defined(AFX_IALLTYPEDOBJ_H__E155962D_61C5_44E4_910C_B73D73E9831D__INCLUDED_)
#define AFX_IALLTYPEDOBJ_H__E155962D_61C5_44E4_910C_B73D73E9831D__INCLUDED_
#include "interfaces.h"
#include "dispimpl.h"

class CIAllMetaInfo{
public:
	CIAllMetaInfo():parent(NULL),child(NULL),next(NULL),id(0),kind(0){}
	CIAllMetaInfo(long i,int k,CString n):parent(NULL),child(NULL),next(NULL),id(i),kind(k),name(n){}
	~CIAllMetaInfo()
	{
		if(child)
			delete child;
		if(next)
			delete next;
	}
	long id;
	int kind;
	CString name;
	CIAllMetaInfo* parent;
	CIAllMetaInfo* child;
	CIAllMetaInfo* next;
	CString GetFullName()
	{
		CString n;
		CIAllMetaInfo* pItem=this;
		while(1)
		{
			n.Insert(0,pItem->name);
			pItem=pItem->parent;
			if(!pItem->parent)
				break;
			n.Insert(0,".");
		}
		return n;
	}
	CIAllMetaInfo* InsertObj(CString _sname,long id);
};


class CIAllTypedObj : public CDispImpl<IAllTypedObj, AllTypedObj>  
{
public:
	// Пустышки макросов, чтобы VC++ унутре себя связал интерфейс с классом реализации
	BEGIN_COM_MAP(Empty)
		COM_INTERFACE_ENTRY(IAllTypedObj)
	END_COM_MAP()

	CIAllTypedObj();
	virtual ~CIAllTypedObj();
	STDMETHOD(get_item)(VARIANT idx,ICfgDoc **pVal);
	STDMETHOD(get_Count)(long *pVal);
	STDMETHOD(get_ChildsCount)(BSTR Parent,long *pVal);
	STDMETHOD(get_ChildName)(BSTR Parent,long idx,BSTR *pVal);
	STDMETHOD(get_ObjectCount)(BSTR Parent,long *pVal);

	CIAllMetaInfo* m_pRoot;
	CPtrArray m_allObjects;
	int m_count;
	static int EnumTypedObj(class CString &name,long id,long l2,enum PageType pt);
};


#endif // !defined(AFX_IALLTYPEDOBJ_H__E155962D_61C5_44E4_910C_B73D73E9831D__INCLUDED_)
