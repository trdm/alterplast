// TeleCont.cpp: implementation of the CTeleCont class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "TeleCont.h"

const int	CTeleCont::GetGroupCount() const	{return 0;}
const int*	CTeleCont::GetImagesNum()  const	{return NULL;}
LPCTSTR*	CTeleCont::GetGroupNames() const	{return NULL;}

void CTeleCont::InitGroups()
{
	const int iGroupCount=GetGroupCount();
	if(!iGroupCount)
		return;
	m_Group=new CTeleGroup*[iGroupCount];
	const int* pImages=GetImagesNum();
	LPCTSTR* pNames=GetGroupNames();
	for(int i=0;i<iGroupCount;i++)
	{
		m_Group[i]	= new CTeleGroup;
		m_Group[i]->m_Image	= *pImages++;
		m_Group[i]->name	= *pNames++;
	}
}

CTeleCont::CTeleCont(CParser* pParser):m_builded(false),m_pParser(pParser),m_Group(NULL)
{
}

CTeleCont::~CTeleCont()
{
	const int iGroupCount=GetGroupCount();
	if(!iGroupCount)
		return;
	Clear();
	for(int i=0;i<iGroupCount;i++)
		delete m_Group[i];
	delete [] m_Group;
}

void CTeleCont::Clear()
{
	m_builded=false;
	const int iGroupCount=GetGroupCount();
	if(!iGroupCount)
		return;
	for(int i=0;i<iGroupCount;i++)
		m_Group[i]->ClearEntries();
}
