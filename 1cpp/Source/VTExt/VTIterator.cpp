//////////////////////////////////////////////////////////////////////
// Классы и методы для работы с итераторами для CVTExtended
//
// Автор: Диркс Алексей aka ADirks
// Контакты: e-mail: adirks@ngs.ru, ICQ: 13472890
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VTExtended.h"
#include "../StringArrayEx.h"

CVTExtIterator::CVTExtIterator(CVTExtIndex* pIndex, bool bUnique)
{
	this->pIndex = pIndex;
	this->m_bUnique = bUnique;

	if( pIndex )
		pVT = pIndex->pVT;
	else
		pVT = NULL;
	SelStarted = false;
	ResetPosition();
}

void CVTExtIterator::SetPosition(CVTExtIndexTreeNode* pNode, int nIndexInNode, CVTExtRow* pRow, int nRow)
{
	pCurrentNode = pNode;
	IndexInNode = nIndexInNode;
	pCurrentRow = pRow;
	nCurrentRow = nRow;
	SelStarted = false;
}

void CVTExtIterator::ResetPosition()
{
	nCurrentRow = -1;
	pCurrentRow = NULL;
	pCurrentNode = NULL;
	IndexInNode = -1;
}

void CVTExtIterator::SetCurrentRow() 
{
	pCurrentRow = pVT->GetRow(nCurrentRow); 
}

//  Простой итератор - без индекса
CVTExtIteratorSimple::CVTExtIteratorSimple(CVTExtended* pVT)
: CVTExtIterator(NULL, false)
{
	this->pVT = pVT; 
}

bool CVTExtIteratorSimple::First()
{
	if( pVT->GetRowsCount() <= 0 ) return false;
	SelStarted = true;
	ResetPosition();
	nCurrentRow = 0;
	SetCurrentRow();
	return true;
}

bool CVTExtIteratorSimple::Last()
{
	if( pVT->GetRowsCount() <= 0 ) return false;
	SelStarted = true;
	ResetPosition();
	nCurrentRow = pVT->GetRowsCount() - 1;
	SetCurrentRow();
	return true;
}

bool CVTExtIteratorSimple::Next()
{
	if( SelStarted )
	{
		SelStarted = false;
	}
	else
	{
		nCurrentRow++;
		if( nCurrentRow >= pVT->GetRowsCount() )
		{
			ResetPosition();
			return false;
		}
		SetCurrentRow();
	}
	return true;
}

bool CVTExtIteratorSimple::Prev()
{
	if( SelStarted )
	{
		SelStarted = false;
	}
	else
	{
		nCurrentRow--;
		if( nCurrentRow < 0 )
		{
			ResetPosition();
			return false;
		}
		SetCurrentRow();
	}
	return true;
}

// Итератор по индексу без фильтра
CVTExtIteratorIndex::CVTExtIteratorIndex(CVTExtIndex* pIndex, bool bUnique)
:CVTExtIterator(pIndex, bUnique)
{
}

bool CVTExtIteratorIndex::First()
{
	SelStarted = true;
	ResetPosition();
	pCurrentNode = pIndex->Min(pIndex->Root);
	if( pCurrentNode == NULL ) return false;
	nCurrentRow = pCurrentNode->ID;
	IndexInNode = 0;
	return true;
}

bool CVTExtIteratorIndex::NextNode()
{
	if( SelStarted )
	{
		SelStarted = false;
	}
	else if( pCurrentNode )
	{
		pCurrentNode = pIndex->Successor(pCurrentNode);
	}

	if( !pCurrentNode )
	{
		ResetPosition();
		return false;
	}
	IndexInNode = 0;
	nCurrentRow = pCurrentNode->ID;
	SetCurrentRow();
	return true;
}

bool CVTExtIteratorIndex::Next()
{
	if ( m_bUnique )
		return NextNode();

	if( !pCurrentNode )
	{
		nCurrentRow = -1;
		SelStarted = false;
	}
	else if( SelStarted )
	{
		SelStarted = false;
	}
	else
	{
		if( pCurrentNode->ArrayEqualIDs && IndexInNode < pCurrentNode->ArrayEqualIDs->size() )
		{
			nCurrentRow = pCurrentNode->ArrayEqualIDs->operator[](IndexInNode++);
		}
		else
		{
			if( NextNode() ) return true;
		}
	}

	if( nCurrentRow < 0 )
	{
		ResetPosition();
		return false;
	}

	SetCurrentRow();

	return true;
}

void CVTExtIteratorIndex::GoToNode_Backward()
{
	if( pCurrentNode->ArrayEqualIDs )
	{
		IndexInNode = pCurrentNode->ArrayEqualIDs->size();
		nCurrentRow = pCurrentNode->ArrayEqualIDs->operator[](IndexInNode - 1);
	}
	else
	{
		nCurrentRow = pCurrentNode->ID;
		IndexInNode = 0;
	}
}

bool CVTExtIteratorIndex::Last()
{
	SelStarted = true;
	ResetPosition();
	pCurrentNode = pIndex->Max(pIndex->Root);
	if( pCurrentNode == NULL ) return false;
	GoToNode_Backward();
	return true;
}

bool CVTExtIteratorIndex::PrevNode()
{
	if( SelStarted )
	{
		SelStarted = false;
	}
	else if( pCurrentNode )
	{
		pCurrentNode = pIndex->Predecessor(pCurrentNode);
	}

	if( !pCurrentNode )
	{
		ResetPosition();
		return false;
	}
	IndexInNode = 0;
	nCurrentRow = pCurrentNode->ID;
	SetCurrentRow();
	return true;
}

bool CVTExtIteratorIndex::Prev()
{
	if ( m_bUnique )
		return PrevNode();

	if( !pCurrentNode )
	{
		nCurrentRow = -1;
	}
	else if( SelStarted )
	{
		SelStarted = false;
	}
	else if( pCurrentNode->ArrayEqualIDs )
	{
		if( IndexInNode > 0 )
		{
			if ( --IndexInNode )
				nCurrentRow = pCurrentNode->ArrayEqualIDs->operator[](IndexInNode - 1);
			else
				nCurrentRow = pCurrentNode->ID;

		}
		else
		{
			if( !PrevNode() ) return false;
			GoToNode_Backward();
		}
	}
	else
	{
		if( !PrevNode() ) return false;
		GoToNode_Backward();
	}

	if( nCurrentRow < 0 )
	{
		ResetPosition();
		return false;
	}

	SetCurrentRow();

	return true;
}

//-----  Filtered iterator -----
CVTExtIteratorFilter::CVTExtIteratorFilter(CVTExtIndex* pIndex, CVTExtFilter* pFilter, bool bUnique)
:CVTExtIteratorIndex(pIndex, bUnique)
{
	this->pFilter = pFilter;
}

void CVTExtIteratorFilter::SecondDiapasonBegin()
{
	pCurrentNode = pIndex->SearchNearestGE(pFilter->Rec_max);
	if( pCurrentNode )
	{
		if( !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
			pCurrentNode = pIndex->Successor(pCurrentNode);
	}
}

void CVTExtIteratorFilter::FirstDiapasonEnd()
{
	pCurrentNode = pIndex->SearchNearestLE(pFilter->Rec_min);
	if( pCurrentNode )
	{
		if( !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
			pCurrentNode = pIndex->Predecessor(pCurrentNode);
	}
}

bool CVTExtIteratorFilter::First()
{
	SelStarted = true;
	ResetPosition();
	if( pFilter->bInverse )
	{
		pCurrentNode = pIndex->Min(pIndex->Root);
		if( pCurrentNode && !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
		{
			SecondDiapasonBegin();
		}
	}
	else
	{
		pCurrentNode = pIndex->SearchNearestGE(pFilter->Rec_min);
		if( pCurrentNode && !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
			pCurrentNode = NULL;
	}
	if( pCurrentNode == NULL ) return false;
	nCurrentRow = pCurrentNode->ID;
	IndexInNode = 0;
	return true;
}

bool CVTExtIteratorFilter::NextNode()
{
	if( !CVTExtIteratorIndex::NextNode() ) return false;

	pFilter->compare_rec.Row = pVT->GetRow(nCurrentRow);
	if( pFilter->bInverse )
	{
		if( pFilter->compare_rec.compare(pFilter->Rec_min) >= 0 && 
			pFilter->compare_rec.compare(pFilter->Rec_max) <= 0 )
		{
			SecondDiapasonBegin();
			if( pCurrentNode )
			{
				nCurrentRow = pCurrentNode->ID;
				IndexInNode = 0;
				SetCurrentRow();
			}
		}
	}
	else
	{
		if( pFilter->compare_rec.compare(pFilter->Rec_max) > 0 )
		{
			pCurrentNode = NULL;
		}
	}

	if( !pCurrentNode )
	{
		ResetPosition();
		return false;
	}
	return true;
}

bool CVTExtIteratorFilter::Next()
{
	if( m_bUnique || pFilter->bOnlyUnique )
		return NextNode();
	// return CVTExtIteratorIndex::NextNode();
	else
		return CVTExtIteratorIndex::Next();
}

bool CVTExtIteratorFilter::Last()
{
	SelStarted = true;
	ResetPosition();
	if( pFilter->bInverse )
	{
		pCurrentNode = pIndex->Max(pIndex->Root);
		if( pCurrentNode && !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
		{
			FirstDiapasonEnd();
		}
	}
	else
	{
		pCurrentNode = pIndex->SearchNearestLE(pFilter->Rec_max);
		if( pCurrentNode && !pFilter->CheckRow(pVT->GetRow(pCurrentNode->ID)) )
			pCurrentNode = NULL;
	}
	if( pCurrentNode == NULL ) return false;

	if( pCurrentNode->ArrayEqualIDs )
	{
		IndexInNode = pCurrentNode->ArrayEqualIDs->size();
		nCurrentRow = pCurrentNode->ArrayEqualIDs->operator[](IndexInNode-1);
	}
	else
	{
		nCurrentRow = pCurrentNode->ID;
		IndexInNode = 0;
	}
		
	return true;
}

bool CVTExtIteratorFilter::PrevNode()
{
	if( !CVTExtIteratorIndex::PrevNode() ) return false;

	pFilter->compare_rec.Row = pVT->GetRow(nCurrentRow);
	if( pFilter->bInverse )
	{
		if( pFilter->compare_rec.compare(pFilter->Rec_max) <= 0 &&
			pFilter->compare_rec.compare(pFilter->Rec_min) >= 0 )
		{
			FirstDiapasonEnd();
			if( pCurrentNode )
			{
				nCurrentRow = pCurrentNode->ID;
				IndexInNode = 0;
				SetCurrentRow();
			}
		}
	}
	else
	{
		if( pFilter->compare_rec.compare(pFilter->Rec_min) < 0 )
		{
			pCurrentNode = NULL;
		}
	}

	if( !pCurrentNode )
	{
		ResetPosition();
		return false;
	}
	return true;
}

bool CVTExtIteratorFilter::Prev()
{
	if( m_bUnique || pFilter->bOnlyUnique )
		return PrevNode();
//		return CVTExtIteratorIndex::PrevNode();
	else
		return CVTExtIteratorIndex::Prev();
}
