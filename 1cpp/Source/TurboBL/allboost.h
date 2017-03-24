// allboost.h
#ifndef ALLBOOST_H
#define ALLBOOST_H

#include "../blmap.h"
extern HANDLE hProcess;

class CAllBoost
{
public:
#pragma pack(1)
	struct _Trap{
		BYTE mov;		// mov ecx,this
		DWORD pThis;
		BYTE jmp;		// jmp offset
		DWORD offset;
	};
	_Trap m_FindMethThunk;
	_Trap m_FindPropThunk;
	CBLMap m_MethMap;
	CBLMap m_PropMap;
	int FindMeth(const char *name){return m_MethMap.GetKey(name);}
	int FindProp(const char *name){return m_PropMap.GetKey(name);}
	typedef int(CAllBoost::*PF)(const char*);
	CAllBoost(CBLContext* pCont,bool bTrapProp=false)
	{
		if(*(BYTE*)(*(DWORD**)pCont)[26]==0xB9)
		{
			delete this;
			return;
		}
		m_MethMap.MakeMethMapFromContext(pCont);
		m_FindMethThunk.mov=0xB9;
		m_FindMethThunk.pThis=(DWORD)this;
		m_FindMethThunk.jmp=0xE9;
		PF adr=FindMeth;
		DWORD dAdr=*(DWORD*)&adr;
		m_FindMethThunk.offset=dAdr-(DWORD)&m_FindMethThunk.offset-4;
		DWORD* pVtable=*(DWORD**)pCont;
		pVtable+=26;
		DWORD old;
		VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
		*pVtable=(DWORD)this;
		if(bTrapProp)
		{
			m_PropMap.MakePropMapFromContext(pCont);
			m_FindPropThunk.mov=0xB9;
			m_FindPropThunk.pThis=(DWORD)this;
			m_FindPropThunk.jmp=0xE9;
			adr=FindProp;
			dAdr=*(DWORD*)&adr;
			m_FindPropThunk.offset=dAdr-(DWORD)&m_FindPropThunk.offset-4;
			pVtable-=7;
			VirtualProtect(pVtable,4,PAGE_EXECUTE_READWRITE,&old);
			*pVtable=(DWORD)&m_FindPropThunk;
		}
		VirtualProtect(this, sizeof(*this), PAGE_EXECUTE_READWRITE, &old);
		FlushInstructionCache(hProcess, *(DWORD**)pCont, 256);
	}
};

#endif