// nocasemap.inl
CNoCaseMapBase::init CNoCaseMapBase::m_lotableinit;
DWORD CNoCaseMapBase::m_lotable[256];

void CNoCaseMapBase::PrepeareHashTable(DWORD nCount)
{
	DWORD newHashSize=CalcHashSize(nCount);
	CAssoc** ppHashTable=new CAssoc*[newHashSize];
	memset(ppHashTable, 0, newHashSize * sizeof(CAssoc*));
	if(DWORD count=m_nCount)
	{
		CAssoc **ppAssoc=m_pHashTable, *pAssoc=*ppAssoc;
		while(!pAssoc)
			pAssoc=*++ppAssoc;
		for(;;)
		{
			DWORD newPos=pAssoc->nHashValue % newHashSize;
			CAssoc* pNext=pAssoc->pNext;
			pAssoc->pNext=ppHashTable[newPos];
			ppHashTable[newPos]=pAssoc;
			if(!--count)
				break;
			while(!pNext)
				pNext=*++ppAssoc;
			pAssoc=pNext;
		}
	}
	delete [] m_pHashTable;
	m_pHashTable=ppHashTable;
	m_nHashTableSize=newHashSize;
}
