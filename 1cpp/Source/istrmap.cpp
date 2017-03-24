#include "stdafx.h"
#include "istrmap.h"

//////////////////////////////////////////////////////////////////////////////////////
// class CIStringMapToIndex
//////////////////////////////////////////////////////////////////////////////////////
CIStringMapToIndex::CIStringMapToIndex( bool bUseExternalKeys ) : CIStringMapToInt(bUseExternalKeys)
{
}

int CIStringMapToIndex::Add(LPCSTR szKey)
{
	int nIndex;
	bool bFound = (Lookup(szKey, nIndex) == TRUE);
	ASSERT( !bFound );

	nIndex = GetCount();
	SetAt(szKey, nIndex);
	return nIndex;
}

void CIStringMapToIndex::ShiftIndexes(int nStartIndex, int nDelta)
{
	LPSTR sHashKey;
	int nHashIndex;
	POSITION pos = GetStartPosition();
	while( pos )
	{
		GetNextAssoc(pos, sHashKey, nHashIndex);
		if( nHashIndex >= nStartIndex )
			SetAt(sHashKey, nHashIndex + nDelta);
	}
}

void CIStringMapToIndex::Insert(int nIndex, LPCSTR szKey)
{
	ASSERT(nIndex >= 0 && nIndex <= GetCount());
	int nHashIndex;
	bool bFound = (Lookup(szKey, nHashIndex) == TRUE);
	ASSERT( !bFound );

	ShiftIndexes(nIndex, 1);
	SetAt(szKey, nIndex);
}

int CIStringMapToIndex::Remove(LPCSTR szKey)
{
	int nIndex;
	bool bFound = (Lookup(szKey, nIndex) == TRUE);
	ASSERT(bFound);

	RemoveKey(szKey);
	ShiftIndexes(nIndex, -1);
	return nIndex;
}

void CIStringMapToIndex::Remove(int nIndex)
{
	ASSERT(nIndex >= 0 && nIndex < GetCount());

	LPSTR sHashKey;
	int nHashIndex;
	POSITION pos = GetStartPosition();
	while( pos )
	{
		GetNextAssoc(pos, sHashKey, nHashIndex);
		if( nHashIndex == nIndex )
		{
			RemoveKey(sHashKey);
			break;
		}
	}
	ShiftIndexes(nIndex, -1);
}

void CIStringMapToIndex::Move(int nIndex, int nOffset)
{
	ASSERT(nIndex >= 0 && nIndex < GetCount());
	ASSERT(nOffset != 0);

	int nNewIndex = nIndex + nOffset;
	ASSERT(nNewIndex >= 0 && nNewIndex < GetCount());

    int nFirst, nLast;
    if( nOffset > 0 )
    {
        nFirst = nIndex;
        nLast = nNewIndex;
    }
    else
    {
        nFirst = nNewIndex;
        nLast = nIndex;
    }

	LPSTR sHashKey;
	int nHashIndex;
	POSITION pos = GetStartPosition();
	while( pos )
	{
		GetNextAssoc(pos, sHashKey, nHashIndex);
		if( nHashIndex == nIndex )
		{
			SetAt(sHashKey, nNewIndex);
		}
		else if( nFirst <= nHashIndex && nHashIndex <= nLast )
		{
			if( nOffset < 0 )
				SetAt(sHashKey, nHashIndex + 1);
			else
				SetAt(sHashKey, nHashIndex - 1);
		}
	}
}


//////////////////////////////////////////////////////////////////////////////////////
// class CCollection
//////////////////////////////////////////////////////////////////////////////////////

CCollectionItem::CCollectionItem()
{
    m_pCollection = NULL;
	m_Name = NULL;
}

CCollectionItem::~CCollectionItem()
{
	if (m_Name) free( m_Name );
}

LPCSTR CCollectionItem::GetName() const
{
    return m_Name;
}

void CCollectionItem::SetName(LPCSTR szName)
{
    if (m_pCollection)
    {
        int nIndex = m_pCollection->m_ItemsHash[m_Name];
        m_pCollection->m_ItemsHash.RemoveKey(m_Name);
        m_pCollection->m_ItemsHash[szName] = nIndex;
        m_pCollection->OnRename(this);
    }

	size_t size = (strlen(szName) + 1) * sizeof(char);
// 	if ( m_Name )
// 	{
// 		realloc( m_Name, size );
// 	}
// 	else
// 	{
// 		m_Name = static_cast<LPSTR>(malloc( size ));
// 	}
	m_Name = static_cast<LPSTR>(m_Name ? realloc(m_Name, size) : malloc(size));

    strcpy(m_Name, szName);
}
/*
CCollection* CCollectionItem::GetCollection() const
{
    return m_pCollection;
}
*/
//////////////////////////////////////////////////////////////////////////////////////
// class CCollection
//////////////////////////////////////////////////////////////////////////////////////

// используем вариант хранения ключей по ссылке
// в данном контексте это безопасно
CCollection::CCollection() : m_ItemsHash(true)
{
}

CCollection::~CCollection()
{
}

int CCollection::GetCount() const
{
    return m_Items.GetSize();
}

int CCollection::GetUpperBound() const
{
    return m_Items.GetUpperBound();
}

void CCollection::Add(CCollectionItem* pItem)
{
	int nHashIndex = m_ItemsHash.Add(pItem->GetName());
	ASSERT( nHashIndex >= 0 );
	int nArrayIndex = m_Items.Add(pItem);
	ASSERT( nHashIndex == nArrayIndex );

	pItem->SetCollection( this );
    OnAdd(pItem);
}

void CCollection::Insert(int nIndex, CCollectionItem* pItem)
{
    ASSERT(nIndex >= 0 && nIndex <= m_Items.GetSize());
    m_Items.InsertAt(nIndex, pItem);
    m_ItemsHash.Insert(nIndex, pItem->GetName());

	pItem->SetCollection( this );
    OnAdd(pItem);
}

void CCollection::Remove(int nIndex)
{
    ASSERT(nIndex >= 0 && nIndex < m_Items.GetSize());
    CCollectionItem* pItem = m_Items[nIndex];
    
	int nHashIndex = m_ItemsHash.Remove(pItem->GetName());

    m_Items.RemoveAt(nIndex);
	pItem->SetCollection( NULL );
    OnRemove(pItem, nIndex);
}

void CCollection::Clear()
{
    for (int i = 0; i < m_Items.GetSize(); i++)
    {
        m_Items[i]->SetCollection( NULL );
        OnRemove(m_Items[i], -1);
    }
    m_ItemsHash.RemoveAll();
    m_Items.RemoveAll();
}

CCollectionItem* CCollection::operator [](int nIndex) const
{
	if( nIndex < 0 ) return NULL;
	if( nIndex > m_Items.GetUpperBound() ) return NULL;
    return m_Items[nIndex];
}

CCollectionItem* CCollection::operator [](LPCSTR szName) const
{
    int nIndex;
    if (m_ItemsHash.Lookup(szName, nIndex))
        return (CCollectionItem*)m_Items[nIndex];
    else
        return NULL;
}

int CCollection::GetIndex(CCollectionItem* pItem) const
{
    for (int i = 0; i < m_Items.GetSize(); i++)
        if (m_Items[i] == pItem)
            return i;
    return -1;
}

int CCollection::GetIndex(LPCSTR szName) const
{
    int nIndex;
    if (m_ItemsHash.Lookup(szName, nIndex))
        return nIndex;
    return -1;
}

CCollectionItem** CCollection::GetData() const
{
    return (CCollectionItem**)m_Items.GetData();
}

void CCollection::Move(int nIndex, int nOffset)
{
    ASSERT(nIndex >= 0 && nIndex < m_Items.GetSize() && nOffset != 0);

    int nNewIndex = nIndex + nOffset;

    ASSERT(nNewIndex >= 0 && nNewIndex <= m_Items.GetSize());
    
    CCollectionItem* pItem = m_Items.GetAt(nIndex);
    m_Items.RemoveAt(nIndex);
    m_Items.InsertAt(nNewIndex, pItem);

	m_ItemsHash.Move(nIndex, nOffset);
}

void CCollection::OnAdd(CCollectionItem* pItem)
{
}

void CCollection::OnRemove(CCollectionItem* pItem, int nIndexWas)
{
}

void CCollection::OnRename(CCollectionItem* pItem)
{
}
