// RBTree.h:		interface for the CRBTree template.
// Inventor Name:	Hatem Mostafa
// Created:			4/1/2005
//
// Взято с http://www.codeproject.com/cpp/BinaryTree.asp
//////////////////////////////////////////////////////////////////////
// Red/Black Tree is a binary trees with the following properties:
// 1- Every node is either red or black
// 2- Every leaf (nil) is black
// 3- If a node is red, then both its children are black
// 4- Every simple path from a node to a descendant leaf contains the same number of black nodes

#pragma once

#include "BinaryTree.h"

#define RBTREENODE CRBTreeNode<KEY>
#define Red		false
#define Black	true
#define GetColor(x) ((RBTREENODE*)x)->Color
#define SetColor(x, c) ((RBTREENODE*)x)->Color = c
#define IsBlack(x) GetColor(x) == Black
#define IsRed(x) GetColor(x) == Red

template <class KEY> class CRBTreeNode : public CBinaryTreeNode<KEY>
{
public:
	CRBTreeNode():CBinaryTreeNode<KEY>()
	{
		Color = Black;
	}
public:
	// red black flag
	bool Color;
};

template <class KEY, class ARG_KEY> class CRBTree : public CBinaryTree<KEY, ARG_KEY>
{
public:
	CRBTree():CBinaryTree<KEY, ARG_KEY>()
	{
		Root = Nil = NewNode();
	}
	~CRBTree()
	{
		DeleteNode(Nil);
	}

public:
	//     |                       |
	//     y     RightRotate       x
	//	  / \      ---->          / \
	//   x   c	   <----         a   y
	//  / \	      LeftRotate        / \
	// a   b                       b   c
	void LeftRotate(TREENODE* x)
	{
		TREENODE* y = x->RIGHT;
		x->RIGHT = y->LEFT;
		if(y->LEFT != Nil)
			y->LEFT->Parent = x;
		y->Parent = x->Parent;
		if(x->Parent == Nil)
			Root = y;
		else
			x->Parent->Childs[x == x->Parent->RIGHT] = y;
		y->LEFT = x;
		x->Parent = y;
	}

	void RightRotate(TREENODE* y)
	{
		TREENODE* x = y->LEFT;
		y->LEFT = x->RIGHT;
		if(x->RIGHT != Nil)
			x->RIGHT->Parent = y;
		x->Parent = y->Parent;
		if(y->Parent == Nil)
			Root = x;
		else
			y->Parent->Childs[y == y->Parent->RIGHT] = x;
		x->RIGHT = y;
		y->Parent = x;
	}
	// to kep the tree balanced use RBInsert
	inline TREENODE* Insert(ARG_KEY key, int nID = -1, TREENODE* node = NULL)
	{
		node = CBinaryTree<KEY, ARG_KEY>::Insert(key, nID, node);
		if(node->Count > 1)
			return node;
		TREENODE* x = node, *y;
		SetColor(x, Red);
		while(x != Root && IsRed(x->Parent))
		{
			bool bRIGHT = x->Parent == x->Parent->Parent->LEFT;
			y = x->Parent->Parent->Childs[bRIGHT];
			if(IsRed(y))
			{
				SetColor(x->Parent, Black);
				SetColor(y, Black);
				SetColor(x->Parent->Parent, Red);
				x = x->Parent->Parent;
			}
			else
			{
				if(x == x->Parent->Childs[bRIGHT])
				{
					x = x->Parent;
					bRIGHT ? LeftRotate(x) : RightRotate(x);
				}
				SetColor(x->Parent, Black);
				SetColor(x->Parent->Parent, Red);
				bRIGHT ? RightRotate(x->Parent->Parent) : LeftRotate(x->Parent->Parent);
			}				
		}
		SetColor(Root, Black);
		return node;
	}

	void Delete(TREENODE* node)
	{	
		TREENODE *pSpliced = (node->LEFT == Nil || node->RIGHT == Nil)?node:Successor(node);
		TREENODE *pChild = pSpliced->Childs[pSpliced->LEFT == Nil];
		// connect child to spliced node parent
		pChild->Parent = pSpliced->Parent;
		// connect spliced node parent to child
		if(pSpliced->Parent == Nil)
			Root = pChild;
		else
			pSpliced->Parent->Childs[pSpliced != pSpliced->Parent->LEFT] = pChild;
		if(IsBlack(pSpliced))
			DeleteFixUp(pChild);
		// put spliced node in place of node (if required)
		if(pSpliced != node)
		{	// copy spliced node
			*node = *pSpliced;
			// delete the spliced node
			DeleteNode(pSpliced);
		}
		else
			// delete the node
			DeleteNode(node);
		Count--;
		UniqueCount--;
	}
protected:
	void DeleteFixUp(TREENODE* x)
	{
		TREENODE* w;
		while (x != Root && IsBlack(x))
		{
			bool bRIGHT = x == x->Parent->LEFT;
			w = x->Parent->Childs[bRIGHT];
			if(IsRed(w))
			{
				SetColor(w, Black);
				SetColor(x->Parent, Red);
				bRIGHT ? LeftRotate(x->Parent) : RightRotate(x->Parent);
				w = x->Parent->Childs[bRIGHT];
			}
			if(IsBlack(w->Childs[!bRIGHT]) && IsBlack(w->Childs[bRIGHT]))
			{
				SetColor(w, Red);
				x = x->Parent;
			}
			else	
			{
				if (IsBlack(w->Childs[bRIGHT]))
				{
					SetColor(w->Childs[!bRIGHT], Black);
					SetColor(w, Red);
					bRIGHT ? RightRotate(w) : LeftRotate(w);
					w = x->Parent->Childs[bRIGHT];
				}
				SetColor(w, GetColor(x->Parent));
				SetColor(x->Parent, Black);
				SetColor(w->Childs[bRIGHT], Black);
				bRIGHT ? LeftRotate(x->Parent) : RightRotate(x->Parent);
				x = Root;
			}
		}
		SetColor(x, Black);
	}
	virtual TREENODE* NewNode()
	{
		HANDLE hHeap = CVTExtHeap::Get();
		RBTREENODE* node = (RBTREENODE*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(RBTREENODE));
		new( node ) RBTREENODE;
		node->Parent = node->LEFT = node->RIGHT = Nil;
		return node;
	}

	virtual void DeleteNode(TREENODE* node)
	{
		HANDLE hHeap = CVTExtHeap::Get();
		node->~CBinaryTreeNode();
		HeapFree(hHeap, HEAP_NO_SERIALIZE, (LPVOID)node);
	}
};