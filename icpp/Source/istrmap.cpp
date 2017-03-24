#include "stdafx.h"
#include "istrmap.h"

//////////////////////////////////////////////////////////////////////////////////////
// class CIStringMap
//////////////////////////////////////////////////////////////////////////////////////
UINT CIStringMapCMN::m_CS1251_upper[256] =
{
	0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
	0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
	0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
	0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
	//     @    A
	0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	//														 Z						  _ 
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
	//     `    a
	0x60,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
	//														 z						  ~
	0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x7B,0x7C,0x7D,0x7E,0x7F,
	0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
	0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
	//                                             Ё
	0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
	//											   ё
	0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xA8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
	//	   А
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	//																				  Я
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
	//	   а
	0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
	//																				  я
	0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
};

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
	LPCSTR sHashKey;
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

	LPCSTR sHashKey;
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

	LPCSTR sHashKey;
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
        return m_Items[nIndex];
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
