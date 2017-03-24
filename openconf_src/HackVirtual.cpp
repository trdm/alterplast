// HackVirtual.cpp: implementation of the CTrapVirtual class.
#include "stdafx.h"
#include "HackVirtual.h"

CTrapVirtual::CTrapVirtual()
{
	m_pVtable=NULL;
	m_pRealAdress=NULL;
}

CTrapVirtual::~CTrapVirtual()
{
	UnTrap();
}

void CTrapVirtual::SetTrap(void* pObject,int offset,AFX_PMSG pFunc)
{
	m_pVtable=*(AFX_PMSG**)pObject;
	m_pVtable+=offset;
	DWORD old;
	VirtualProtect(m_pVtable,4,PAGE_EXECUTE_READWRITE,&old);
	m_pRealAdress=*m_pVtable;
	*m_pVtable=pFunc;
}

void CTrapVirtual::UnTrap()
{
	if(!m_pVtable)
		return;
	*m_pVtable=m_pRealAdress;
	m_pVtable=NULL;
}

static void SetTrap(CTrapSwap* pTrap,void* pNewFunc)
{
	if(!pTrap->m_pFuncTrapped.pFunc)
		return;
	DWORD old;
	VirtualProtect(pTrap->m_pFuncTrapped.pFunc,8,PAGE_EXECUTE_READWRITE,&old);
	// Формируем джамп на адрес новой функции
	BYTE* p=(BYTE*)&pTrap->m_SwapBuf;
	*p++=0xE9;
	*(DWORD*)p=((DWORD)pNewFunc)-((DWORD)pTrap->m_pFuncTrapped.pFunc)-5;
	// И вписываем в начало функции сформированный джамп
	pTrap->Swap();
}

void CTrapSwap::SetTrap(LPCTSTR dll,LPCTSTR func,void* pNewFunc)
{
	HINSTANCE hInst=GetModuleHandle(dll);
	if(!hInst)
		return;
	m_pFuncTrapped.pFunc=GetProcAddress(hInst,func);
	::SetTrap(this,pNewFunc);
}

void CTrapSwap::SetTrapOnImportFunc(void *pOldFunc, void *pNewFunc)
{
	char* ptr=(char*)pOldFunc;
	ptr+=2;
	m_pFuncTrapped.pFunc=(void*)**(DWORD**)ptr;
	::SetTrap(this,pNewFunc);
}
