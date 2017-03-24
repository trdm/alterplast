// HackVirtual.cpp: implementation of the CHackVirtual class.
#include "stdafx.h"
#include "HackVirtual.h"

CHackVirtual::CHackVirtual()
{
	m_pVtable=NULL;
	m_pRealAdress=NULL;
}

CHackVirtual::~CHackVirtual()
{
	UnHack();
}

void CHackVirtual::Hack(void* pObject,int offset,AFX_PMSG pFunc)
{
	m_pVtable=*(AFX_PMSG**)pObject;
	m_pVtable+=offset;
	DWORD old;
	VirtualProtect(m_pVtable,4,PAGE_EXECUTE_READWRITE,&old);
	m_pRealAdress=*m_pVtable;
	*m_pVtable=pFunc;
}

void CHackVirtual::UnHack()
{
	if(!m_pVtable)
		return;
	*m_pVtable=m_pRealAdress;
	m_pVtable=NULL;
}

void CHackSwap::Hack(LPCTSTR dll,LPCTSTR func,void* pNewFunc)
{
	HINSTANCE hInst=GetModuleHandle(dll);
	if(!hInst)
		return;
	m_pFuncHacked.pFunc=GetProcAddress(hInst,func);
	if(!m_pFuncHacked.pFunc)
		return;
	DWORD old;
	VirtualProtect(m_pFuncHacked.pFunc,6,PAGE_EXECUTE_READWRITE,&old);
	m_pFuncHack=pNewFunc;
	BYTE* p=m_SwapBuf;
	*p++=0xFF;
	*p++=0x25;
	*(DWORD**)p=(DWORD*)&m_pFuncHack;
	Swap();
}
