// TextBox.cpp: implementation of the CTextBox class.
#include "stdafx.h"
#include "telepat.h"
#include "TextBox.h"
#include "syntax.h"
const char CTextBox::m_LowerTable[257]=
"                                                0123456789       abcdefghijklmnopqrstuvwxyz    _ abcdefghijklmnopqrstuvwxyz                                             ¸               ¸       אבגדהוזחטיךכלםמןנסעףפץצקרשתûü‎‏ÿאבגדהוזחטיךכלםמןנסעףפץצקרשתûü‎‏ÿ";

CTextBox::CTextBox()
{
	m_IsSorted=true;
	m_Array=(STextEntry*)VirtualAlloc(NULL,4048,MEM_COMMIT,PAGE_READWRITE);
	m_pLastEntry=((STextEntry*)(((char*)m_Array)+4048));
	m_pEntry=m_Array;

	m_text=(char*)VirtualAlloc(NULL,4048,MEM_COMMIT,PAGE_READWRITE);
	m_pLastText=m_text+4048;
	m_pText=m_text;
	m_count=0;
}

inline static void MyRealloc(char*& pStart,char*& pCurrent,char*& pEnd,int len)
{
	if(pCurrent+len<pEnd)
		return;
	DWORD size=pEnd-pStart;
	char* NewBlock=(char*)VirtualAlloc(NULL,size+4048,MEM_COMMIT,PAGE_READWRITE);
	CopyMemory(NewBlock,pStart,size);
	VirtualFree(pStart,0,MEM_RELEASE);
	pCurrent=NewBlock+size;
	pStart=NewBlock;
	pEnd=pStart+size+4048;
}

CTextBox::~CTextBox()
{
	VirtualFree(m_Array,0,MEM_RELEASE);
	VirtualFree(m_text,0,MEM_RELEASE);
}

void CTextBox::AddString(CString &str,void* param)
{
	AddString((LPCTSTR)str,str.GetLength(),param);
}

void CTextBox::AddString(SLexem &lex,void* param)
{
	AddString(lex.pStart,lex.len,param);
}

int CTextBox::GetCount()
{
	return m_count;
}

CString CTextBox::GetString(int pos)
{
	return m_Array[pos].pStr;
}

void CTextBox::Sort()
{
	if(m_IsSorted)
		return;
}

void CTextBox::AddString(LPCTSTR ptr, int len, void *param)
{
	m_IsSorted=false;
	MyRealloc(m_text,m_pText,m_pLastText,len+1);
	memcpy(m_pText,ptr,len);
	m_pText[len]=0;

	MyRealloc((char*&)m_Array,(char*&)m_pEntry,(char*&)m_pLastEntry,sizeof(STextEntry));
	m_Array[m_count].pStr=m_pText;
	m_Array[m_count].len=len;
	m_Array[m_count].param=param;

	m_pText+=len+1;
	m_count++;
}
