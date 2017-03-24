// XTList.h: interface for the CXTList class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_XTLIST_H__4181FA9A_992E_4E63_9D69_B0826950AE07__INCLUDED_)
#define AFX_XTLIST_H__4181FA9A_992E_4E63_9D69_B0826950AE07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "config.h"

class CXTNode
{
protected:
	class CXTNode*	m_Succ;
	class CXTNode*	m_Pred;
public:
	const char*		m_Name;
public:
	CXTNode(const char* name = NULL);

	virtual ~CXTNode();
	friend class CXTList;


	//operations
	virtual void	Remove();
	//iteration
	CXTNode*		GetNext();
	CXTNode*		GetPrev();
};


//hook function prototype
typedef int ListHook(CXTNode *node, ULONG data);

class CXTList  
{
protected:
	class CXTNode		m_Head;
	class CXTNode		m_Tail;
public:
	CXTList(UINT mode = 0);
	virtual ~CXTList();

	//list initialisation
	virtual void		Init();
	//operations
	virtual void		AddHead(CXTNode* nNode);
	virtual void		AddTail(CXTNode* nNode);
	virtual CXTNode*	RemoveHead();
	virtual CXTNode*	RemoveTail();
	virtual void		RemoveAll();
	virtual void		DeleteAll();
	//insertion
	virtual void		InsertBefore(CXTNode* nNode, CXTNode* oldNode);
	virtual void		InsertAfter(CXTNode* nNode, CXTNode* oldNode);
	//searching
	virtual CXTNode*	FindName(const char *name, int *pIndex = NULL);
	virtual CXTNode*	GetAt(int index);
	//iteration
	virtual CXTNode*	GetHead();
	virtual CXTNode*	GetTail();
	virtual int			ApplyListHook(ListHook *hook, ULONG data);
	//status
	virtual int			IsEmpty();

	UINT				m_mode;
	//0x00000001	destroy childs on list destruction (otherwise only remove them)

#ifdef _DEBUG
	virtual void		Dump();
#endif
};

#endif // !defined(AFX_XTLIST_H__4181FA9A_992E_4E63_9D69_B0826950AE07__INCLUDED_)
