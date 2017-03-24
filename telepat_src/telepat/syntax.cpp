// syntax.cpp
#include "stdafx.h"
#include "syntax.h"
#include "resource.h"
#include "swicttable.inl"

SLexem::SLexem(LPCTSTR pS,LPCTSTR pE)
{
	pStart=pS;
	if(pE)
		len=pE-pS;
	else
	{
		len=0;
		LPCTSTR ptr=pS;
		while(*ptr++)
			len++;
	}
}

bool SLexem::IsStrBegin(LPCTSTR ptr)
{
	LPCTSTR pRead=pStart,pEnd=pStart+len;
	while(*ptr && pRead<=pEnd)
	{
		if(CBLMap::m_LowerTable[(BYTE)*ptr++]!=CBLMap::m_LowerTable[(BYTE)*pRead++])
			return false;
	}
	if(pRead>pEnd)
		return true;
	return false;
}

bool SLexem::IsLexemBegin(LPCTSTR ptr,bool id)
{
	LPCTSTR pRead=pStart,pEnd=pStart+len;
	if(id)
	{
		while(*ptr && pRead<=pEnd)
		{
			if(CBLMap::m_LowerTable[(BYTE)*ptr++]!=CBLMap::m_LowerTable[(BYTE)*pRead++])
				return false;
		}
	}
	else
	{
		while(*ptr && pRead<=pEnd)
		{
			if(*ptr++!=*pRead++)
				return false;
		}
	}
	if(!*ptr)
		return true;
	return false;
}

void CLexer::Init()
{
#ifdef BUILD_RESOURCE
	for(int i=0;i<STATE_LAST;i++)
		m_SwitchTable[i][CLS_LAST]=0;
	for(i=0;i<=CLS_LAST;i++)
		m_SwitchTable[STATE_EOF][i]=-1;
	m_SwitchTable[STATE_NONE][CLS_LAST]=STATE_EOF;

	CString projectdir=__FILE__;
	projectdir=projectdir.Left(projectdir.ReverseFind('\\')+1);
	char fName[100];
	wsprintf(fName,"lexRes%i",IDR_LEXER);
	HANDLE hFile=CreateFile(projectdir+fName,GENERIC_WRITE,0,NULL,CREATE_ALWAYS,0,NULL);
	DWORD rw;
	WriteFile(hFile,m_ClassesOfSymbol,	sizeof(m_ClassesOfSymbol),&rw,NULL);
	WriteFile(hFile,m_SwitchTable,		sizeof(m_SwitchTable),&rw,NULL);
	CloseHandle(hFile);
#else
	char *pData=(char*)GetZipResource(IDR_LEXER),*pRead=pData;
	memcpy(m_ClassesOfSymbol,pRead,sizeof(m_ClassesOfSymbol));
	pRead+=sizeof(m_ClassesOfSymbol);
	memcpy(m_SwitchTable,pRead,sizeof(m_SwitchTable));
	delete [] pData;
#endif
}

void CLexer::SetSource(LPCTSTR pStart,int length,LPCTSTR pCurrent,int StartLine)
{
	m_pStartPos=pStart;
	m_pCurPos=pCurrent;
	if(!m_pCurPos)
		m_pCurPos=pStart;
	m_pEndPos=pStart+length;
	m_curLine=StartLine;
}

#pragma optimize("agt",on)
void CLexer::NextLexem(SLexem& lexem)
{
	/*
	Предполагается, что любая лексема расположена на одной
	строке. Буфер с текстом всегда заканчивается '0'.
	Класс символа '0' - CLS_LAST
	Любое состояние, кроме начального по CLS_LAST переключается
	в нулевое. Нулевое состояние по CLS_LAST переключается в STATE_EOF
	STATE_EOF по любому сиволу переключается в нулевое с возвратом
	на один символ.
	*/
	lexem.pStart=NULL;
	lexem.type=0;
	register int state;
	register int nstate;
	register LPCTSTR pCurPos=m_pCurPos;
	while(!(state=m_SwitchTable[0][m_ClassesOfSymbol[(BYTE)*pCurPos]]))
	{
		if(*pCurPos=='\n')
			m_curLine++;
		pCurPos++;
	}
	lexem.pStart=pCurPos++;
	lexem.line=m_curLine;
	while((nstate=m_SwitchTable[state][m_ClassesOfSymbol[(BYTE)*pCurPos]])>0)
	{
		state=nstate;
		pCurPos++;
	}
	lexem.type=state;
	m_pCurPos=pCurPos+nstate;
	lexem.len=m_pCurPos-lexem.pStart;
}
