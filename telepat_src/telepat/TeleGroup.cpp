// TeleGroup.cpp: implementation of the CTeleGroup class.
#include "stdafx.h"
#include "TeleGroup.h"

CTeleGroup::CTeleGroup()
{
	//m_Entries.SetSize(64,64);
}

CTeleGroup::~CTeleGroup()
{
	ClearEntries();
}

void CTeleGroup::ClearEntries()
{
	int size=m_Entries.GetSize();
	for(int i=0;i<size;i++)
	{
		CTeleEntry* pEntry=(CTeleEntry*)m_Entries[i];
		delete pEntry;
	}
	m_Entries.RemoveAll();
}