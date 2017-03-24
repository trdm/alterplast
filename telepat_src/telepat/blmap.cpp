// blmap.cpp
#include "stdafx.h"
#include "syntax.h"
#include "blmap.h"
// Массив для быстрого перевода строк в нижний регистр
char CBLMap::m_LowerTable[257]=
"                                    $         . 0123456789       abcdefghijklmnopqrstuvwxyz    _ abcdefghijklmnopqrstuvwxyz                                             ё               ё       абвгдежзийклмнопрстуфхцчшщъыьэюяабвгдежзийклмнопрстуфхцчшщъыьэюя";

void CBLMap::InsertKey(LPCTSTR key,void* pos)
{
	if(!key)
		return;
	if(!*key)
		return;
	if(m_Count==m_HashSize)
		Optimize();
	DWORD hash;
	char *buf=new char[lstrlen(key)+1];
	int posInTable=GetHash(key,hash,buf);
	Assoc *pAssoc=m_ppHashTable[posInTable];
	while(pAssoc)
	{
		if(pAssoc->hash==hash)
		{
			if(!lstrcmp(buf,pAssoc->strKey))
			{
				delete [] buf;
				pAssoc->pos=pos;
				return;
			}
		}
		pAssoc=pAssoc->next;
	}
	Assoc *pNew=new Assoc(hash,buf,pos);
	pNew->next=m_ppHashTable[posInTable];
	m_ppHashTable[posInTable]=pNew;
	m_Count++;
}

void CBLMap::InsertKey(SLexem& key,void* pos)
{
	if(!key.pStart)
		return;
	if(!key.len)
		return;
	if(m_Count==m_HashSize)
		Optimize();
	DWORD hash;
	char *buf=new char[key.len+1];
	int posInTable=GetHash(key.pStart,key.len,hash,buf);
	Assoc *pAssoc=m_ppHashTable[posInTable];
	while(pAssoc)
	{
		if(pAssoc->hash==hash)
		{
			if(!lstrcmp(buf,pAssoc->strKey))
			{
				delete [] buf;
				pAssoc->pos=pos;
				return;
			}
		}
		pAssoc=pAssoc->next;
	}
	Assoc *pNew=new Assoc(hash,buf,pos);
	pNew->next=m_ppHashTable[posInTable];
	m_ppHashTable[posInTable]=pNew;
	m_Count++;
}

void* CBLMap::GetKey(LPCTSTR key) const
{
	if(!key)
		return NULL;
	if(!m_ppHashTable)
		return NULL;
	char buf[512];
	DWORD hash;
	Assoc *pAssoc=m_ppHashTable[GetHash(key,hash,buf)];
	while(pAssoc)
	{
		if(pAssoc->hash==hash)
		{
			char *ptr1=buf,*ptr2=pAssoc->strKey;
			while(*ptr2 && *ptr1==*ptr2)
				ptr1++,ptr2++;
			if(*ptr1==*ptr2)
				return pAssoc->pos;
		}
		pAssoc=pAssoc->next;
	}
	return NULL;
}

void* CBLMap::GetKey(SLexem& key) const
{
	if(!key.pStart)
		return NULL;
	if(!m_ppHashTable)
		return NULL;
	char buf[512];
	DWORD hash;
	Assoc *pAssoc=m_ppHashTable[GetHash(key.pStart,key.len,hash,buf)];
	while(pAssoc)
	{
		if(pAssoc->hash==hash)
		{
			char *ptr1=buf,*ptr2=pAssoc->strKey;
			while(*ptr2 && *ptr1==*ptr2)
				ptr1++,ptr2++;
			if(*ptr1==*ptr2)
				return pAssoc->pos;
		}
		pAssoc=pAssoc->next;
	}
	return NULL;
}

void CBLMap::SetHashSize(int size)
{
	Assoc** ppNewHashTable=new Assoc*[size];
	for(int i=0;i<size;i++)
		ppNewHashTable[i]=NULL;
	if(m_ppHashTable)
	{
		for(i=0;i<m_HashSize;i++)
		{
			Assoc* pAssoc=m_ppHashTable[i];
			while(pAssoc)
			{
				int newPos=pAssoc->hash%size;
				Assoc* pNext=pAssoc->next;
				pAssoc->next=ppNewHashTable[newPos];
				ppNewHashTable[newPos]=pAssoc;
				pAssoc=pNext;
			}
		}
		delete [] m_ppHashTable;
	}
	m_HashSize=size;
	m_ppHashTable=ppNewHashTable;
}

void CBLMap::RemoveAll()
{
	for(int i=0;i<m_HashSize && m_Count;i++)
	{
		Assoc* pAssoc=m_ppHashTable[i];
		while(pAssoc)
		{
			delete [] pAssoc->strKey;
			Assoc* pDel=pAssoc;
			pAssoc=pAssoc->next;
			delete pDel;
			m_Count--;
		}
	}
	if(m_ppHashTable)
		delete [] m_ppHashTable;
	m_ppHashTable=NULL;
	m_HashSize=m_Count=0;
}

void CBLMap::Optimize()
{
	// Расчет оптимального размера хэш-таблицы. 
	// Для уменьшения числа коллизий рекомендуется, чтобы размер был простым
	int n=m_Count+m_Count/2;
	if(n<7)
		n=7;
	else if(n%2==0)
		n++;
	for(int i=3;i*i<=n;i+=2)
	{
		if(n%i==0)
		{
			n+=2;
			i=1;
		}
	}
	SetHashSize(n);
}