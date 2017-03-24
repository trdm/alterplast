// BinaryTree.h:	interface for the CBinaryTree template.
// Inventor Name:	Hatem Mostafa
// Created:			18/1/2003
// Modified:		20/12/2004
//
// ¬з€то с http://www.codeproject.com/cpp/BinaryTree.asp
//
//////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
using namespace std;

#define TREENODE CBinaryTreeNode<KEY>
#define LEFT	Childs[0]
#define RIGHT	Childs[1]

template <class KEY> class CBinaryTreeNode
{
public:
	CBinaryTreeNode()
	{
		Parent = LEFT = RIGHT = NULL;
		Count = ID = 0;
		ArrayEqualIDs = NULL;
	}
	~CBinaryTreeNode()
	{
		if( ArrayEqualIDs )
		{
			delete ArrayEqualIDs;
		}
	}
public:
	// node parent, left, right respectively
	TREENODE *Parent, *Childs[2];
	// node key
	KEY Key;
	// node repetition count
	int Count;
	// node ID
	int ID;
	// node repeated keys' IDs
	vector<int> *ArrayEqualIDs;
	const TREENODE& operator=(const TREENODE& node)
	{
		Key = node.Key;
		Count = node.Count;

		ID = node.ID;
		if(node.ArrayEqualIDs)
		{
			if(ArrayEqualIDs == NULL)
				ArrayEqualIDs = new vector<int>;
			*ArrayEqualIDs = *node.ArrayEqualIDs;
		}
		return *this;
	}
};

template <class KEY, class ARG_KEY> class CBinaryTree
{
public:
	CBinaryTree()
	{
		Root = Nil = NULL;
		Count = NodesCount = UniqueCount = 0;
		Modified = NoRepeat = false;
	}
	~CBinaryTree()
	{
		RemoveAll();
	}
public:
	// tree root node
	TREENODE* Root, * Nil;
	// tree nodes count
	int Count, NodesCount, UniqueCount;
	// flag to indicate if the tree is modified or not
	bool Modified;
	// ignore repeated keys in the Add function
	bool NoRepeat;

	// return tree nodes count
	inline int GetCount() const	{	return Count;	}
	// check if the tree is empty or not
	inline bool IsEmpty() const	{	return Count == 0;	}
	// remove all tree nodes
	void RemoveAll()
	{
		TREENODE *node = Root, *pTemp;
		while(node != Nil)
		{
			// check for left child
			if(node->LEFT != Nil)
				node = node->LEFT;
			// check for right child
			else	if(node->RIGHT != Nil)
				node = node->RIGHT;
			else	// node has no childs
			{	// save node pointer
				pTemp = node;
				// set node pointer at its parent to NULL
				if(node->Parent != Nil)
					node->Parent->Childs[node != node->Parent->LEFT] = Nil;
				// update pointer node to its parent
				node = node->Parent;
				// delete the saved node
				DeleteNode(pTemp);
			}
		}
		Count = NodesCount = UniqueCount = 0;
		Root = Nil;
		Modified = false;
	}
	// insert key in the tree
	inline TREENODE* Insert(ARG_KEY key, int nID = -1, TREENODE* node = NULL)
	{
		if(Root == Nil)
		{
			Root = NewNode();
			node = Root;
			NodesCount++;
		}
		else	
		{
			if(node == NULL)
				node = Root;
			int nResult;
			while(true)
			{
				nResult = node->Key.compare(key);
				if(nResult == 0)
				{
					//повтор€ющеес€ значение ключа
					if( NoRepeat == false )
					{
						if(node->ArrayEqualIDs == NULL)
						{
							node->ArrayEqualIDs = new vector<int>;
							UniqueCount--; //количество _уникальных_ ключей уменьшилось!
						}
						node->ArrayEqualIDs->push_back(nID);
						node->Count++;
						Count++;
					}
					return node;
				}
				nResult = nResult > 0 ? 0 : 1;
				if(node->Childs[nResult] == Nil)
				{
					node->Childs[nResult] = NewNode();
					node->Childs[nResult]->Parent = node;
					node = node->Childs[nResult];
					NodesCount++;
					break;
				}
				node = node->Childs[nResult];
			}	
		}

		node->Key = key;
		node->ID = nID;
		node->Count++;
		Count++;
		UniqueCount++;
		Modified = true;
		
		return node;
	}	
	// search for a key in the tree
	inline TREENODE* Search(ARG_KEY key, TREENODE* node = NULL) const
	{
		if(node == NULL)
			node = Root;
		int nResult;
		while(node != Nil && (nResult = node->Key.compare(key)) != 0)
			node = node->Childs[nResult < 0];
		return node == Nil ? NULL : node;
	}	
	// search for a nearest 'greater then' value in the tree
	inline TREENODE* SearchNearestGE(ARG_KEY key, TREENODE* node = NULL) const
	{
		if(node == NULL) node = Root;
		TREENODE* nearest = NULL;
		int nResult;
		while(node != Nil && (nResult = node->Key.compare(key)) != 0)
		{
			if( nResult > 0 ) nearest = node;
			node = node->Childs[nResult < 0];
		}
		return node == Nil ? nearest : node;
	}	
	// search for a nearest 'less then' value in the tree
	inline TREENODE* SearchNearestLE(ARG_KEY key, TREENODE* node = NULL) const
	{
		if(node == NULL) node = Root;
		TREENODE* nearest = NULL;
		int nResult;
		while(node != Nil && (nResult = node->Key.compare(key)) != 0)
		{
			if( nResult < 0 ) nearest = node;
			node = node->Childs[nResult < 0];
		}
		return node == Nil ? nearest : node;
	}	
	// return minimum key in the tree
	TREENODE* Min(TREENODE* node) const
	{	
		// iterate in the left branch
		while(node != Nil && node->LEFT != Nil)
			node = node->LEFT;
		
		if( node == Nil )
			return NULL;
		else
			return node;
	}
	// return maximum key in the tree
	TREENODE* Max(TREENODE* node) const
	{	
		// iterate in the right branch
		while(node != Nil && node->RIGHT != Nil)
			node = node->RIGHT;

		if( node == Nil )
			return NULL;
		else
			return node;
	}
	// return node successor
	TREENODE* Successor(TREENODE* node) const
	{
		// return the left most node in the right subtree
		if(node->RIGHT != Nil)
			return Min(node->RIGHT);
		// go up from node until we find a node that is the left of its parent
		TREENODE* Parent = node->Parent;
		while(Parent != Nil && node == Parent->RIGHT)
		{
			node = Parent;
			Parent = node->Parent;
		}
		if( Parent == Nil ) return NULL;
		return Parent;
	}
	// return node predecessor
	TREENODE* Predecessor(TREENODE* node) const
	{	
		// return the right most node in the left subtree
		if(node->LEFT != Nil)
			return Max(node->LEFT);
		// go up from node until we find a node that is the right of its parent
		TREENODE* Parent = node->Parent;
		while(Parent != Nil && node == Parent->LEFT)
		{
			node = Parent;
			Parent = node->Parent;
		}
		if( Parent == Nil ) return NULL;
		return Parent;
	}

	// delete node
	// 1- node has no child, remove it
	// 2- node has one child, splice it (connect its parent and child)
	// 3- node has two childs, splice its successor and put it in its place
	void Delete(TREENODE* node)
	{	
		TREENODE *pSplice = (node->LEFT == Nil || node->RIGHT == Nil)?node:Successor(node);
		TREENODE *pChild = pSplice->Childs[pSplice->LEFT == Nil];
		// connect child to spliced node parent
		if(pChild != Nil)
			pChild->Parent = pSplice->Parent;
		// connect spliced node parent to child
		if(pSplice->Parent == Nil)
			Root = pChild;
		else
			pSplice->Parent->Childs[pSplice != pSplice->Parent->LEFT] = pChild;
		// put spliced node in place of node (if required)
		if(pSplice != node)
		{	// copy spliced node
			*node = *pSplice;
			// delete the spliced node
			DeleteNode(pSplice);
			NodesCount--;
		}
		else
		{
			// delete the node
			DeleteNode(node);
			NodesCount--;
		}
		Count--;
		UniqueCount--;
	}

	// save all tree nodes in a vector of integers
	void Save(vector<int> &nArraySort, bool bAscending = true, bool (* lpfn)(int, int) = NULL)
	{
		nArraySort.resize(Count);
		int nIndex = 0, *pArray = &*nArraySort.begin();

		TREENODE* node = bAscending ? Min(Root) : Max(Root);
		while(node != Nil)
		{
			if(lpfn)
				(*lpfn)(nIndex++, Count);
			SaveNode(node, pArray);
			node = bAscending ? Successor(node) : Predecessor(node);
		}
		Modified = false;
	}
	// add one node to a vector of integers
	void SaveNode(TREENODE* node, int*& pArray)
	{
		*pArray++ = node->ID;
		if(node->ArrayEqualIDs)
		{
			memcpy(pArray, &*node->ArrayEqualIDs->begin(), node->ArrayEqualIDs->size()*sizeof(int));
			pArray += node->ArrayEqualIDs->size();
		}
	}
protected:
	virtual TREENODE* NewNode()
	{
		HANDLE hHeap = CVTExtHeap::Get();
		TREENODE* node = (TREENODE*)HeapAlloc(hHeap, HEAP_NO_SERIALIZE, sizeof(TREENODE));
		new( node ) TREENODE;
		return node;
	}

	virtual void DeleteNode(TREENODE* node)
	{
		HANDLE hHeap = CVTExtHeap::Get();
		node->~CBinaryTreeNode();
		HeapFree(hHeap, HEAP_NO_SERIALIZE, (LPVOID)node);
	}
};

