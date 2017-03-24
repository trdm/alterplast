// XTList.cpp: implementation of the CXTList class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "xt_list.h"

//////////////////////////////////////////////////////////////////////
// CXTNode Implementation
//////////////////////////////////////////////////////////////////////

#define NOT_IMPL OutputDebugString("CXTList:: *** unimplemented function called");

#undef DEBUG_THIS

#ifdef _DEBUG
	#undef THIS_FILE
	static char THIS_FILE[]=__FILE__;
	#define new DEBUG_NEW
#endif

//#define DEBUG_THIS

#ifdef DEBUG_THIS
	static char dbg[512];
	#define DBG OutputDebugString(dbg)
#endif

CXTNode::CXTNode(const char *name)
{
#ifdef DEBUG_THIS
	sprintf(dbg,"CXTNode()%08X",this);
	DBG;
#endif
	m_Succ = m_Pred = 0;
	m_Name = name;
}

CXTNode::~CXTNode()
{
#ifdef _DEBUG
//	sprintf(dbg,"~CXTNode() %08X",this);
//	DBG
#endif
	if(m_Succ && m_Pred)
	{
		Remove();
	}
}

void CXTNode::Remove()
{
#ifdef _DEBUG
//	sprintf(dbg,"CXTNode::Remove() %08X",this);
//	DBG
#endif
	if(m_Succ && m_Pred)
	{
		m_Pred->m_Succ = m_Succ;
		m_Succ->m_Pred = m_Pred;
		m_Succ = m_Pred = 0;
	}
}

CXTNode* CXTNode::GetNext()
{
	if(m_Succ && m_Pred)
	{
		return m_Succ->m_Succ ? m_Succ : NULL;
	}
	return 0;//not linked
}

CXTNode* CXTNode::GetPrev()
{
	if(m_Succ && m_Pred)
	{
		return m_Pred->m_Pred ? m_Pred : NULL;
	}
	return 0;//not linked
}

//////////////////////////////////////////////////////////////////////
// CXTList Implementation
//////////////////////////////////////////////////////////////////////

CXTList::CXTList(UINT mode)
{
	m_mode = mode;
	Init();
}

CXTList::~CXTList()
{
	if(m_mode & 0x00000001)
	{
		DeleteAll();
	}
	else
	{
		RemoveAll();
	}
}

void CXTList::Init()
{
	m_Head.m_Succ = &m_Tail;
	m_Head.m_Pred = 0;
	m_Tail.m_Succ = 0;
	m_Tail.m_Pred = &m_Head;
}

void CXTList::AddHead(CXTNode *nNode)
{
	NOT_IMPL
}

void CXTList::AddTail(CXTNode *nNode)
{
	m_Tail.m_Pred->m_Succ = nNode;
	nNode->m_Pred = m_Tail.m_Pred;
	nNode->m_Succ = &m_Tail;
	m_Tail.m_Pred = nNode;
}

CXTNode* CXTList::RemoveHead()
{
	CXTNode *n;
	if(n = GetHead())
	{
		n->Remove();
	}
	return n;
}


CXTNode* CXTList::RemoveTail()
{
	CXTNode *n;
	if(n = GetTail())
	{
		n->Remove();
	}
	return n;
}

void CXTList::RemoveAll()
{
	CXTNode *n;
	while(n = GetHead())
	{
		n->Remove();
	}
}

void CXTList::DeleteAll()
{
	CXTNode *n;
	while(n = RemoveTail())
	{
		delete n;
	}
}

void CXTList::InsertBefore(CXTNode* nNode, CXTNode* oldNode)
{
	if(nNode && oldNode)
	{
		oldNode->m_Pred->m_Succ = nNode;
		nNode->m_Pred = oldNode->m_Pred;
		nNode->m_Succ = oldNode;
		oldNode->m_Pred = nNode;
	}
}

void CXTList::InsertAfter(CXTNode* nNode, CXTNode* oldNode)
{
	NOT_IMPL

}

CXTNode* CXTList::FindName(const char *name,int *pIndex /*= NULL*/)
{
	int pos = 0;
	CXTNode *n = GetHead();
	while(n)
	{
		if(n->m_Name)
		{
			if(stricmp(n->m_Name,name)==0)
			{
				if(pIndex) {*pIndex = pos;}
				return n;
			}
		}
		n = n->GetNext();
		pos++;
	}
	if(pIndex){*pIndex = -1;}
	return 0;
}

CXTNode* CXTList::GetAt(int index)
{
	CXTNode *n = GetHead();
	while(n)
	{
		if(index == 0)
		{return n;}
		n = n->GetNext();
		index--;
	}
	return 0;
}

CXTNode* CXTList::GetHead()
{
	return m_Head.m_Succ->m_Succ ? m_Head.m_Succ : 0;
}

CXTNode* CXTList::GetTail()
{
	return m_Tail.m_Pred->m_Pred ? m_Tail.m_Pred : 0;
}

int CXTList::ApplyListHook(ListHook hook, ULONG data)
{
	NOT_IMPL
	return 0;
}

int CXTList::IsEmpty()
{
	return m_Head.m_Succ->m_Succ ? 0 : 1;
}


#ifdef _DEBUG
void CXTList::Dump()
{
#ifdef DEBUG_THIS
	sprintf(dbg,"---- Dump of CXTList object at 0x%08X",this);
	DBG;
	CXTNode *n = &m_Head;
	const char *name;
	while(n)
	{
		if(n->m_Pred == 0)
			name = "m_Head";
		else if(n->m_Succ == 0)
			name = "m_Tail";
		else if(n->m_Name == 0)
			name = "unnamed";
		else
			name = n->m_Name;

		sprintf(dbg,"| node <%s> at 0x%08X; Successor is 0x%08X Predecessor is 0x%08X",name,n,n->m_Succ,n->m_Pred);
		DBG;
		n = n->m_Succ;
	}
	OutputDebugString("---- end of dump");
#endif
}
#endif