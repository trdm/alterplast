// HackVirtual.h: interface for the CTrapVirtual class.
#if !defined(AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_)
#define AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_

class CTrapVirtual  
{
public:
	CTrapVirtual();
	~CTrapVirtual();
	void SetTrap(void* pObject,int offset,AFX_PMSG pFunc);
	void UnTrap();
	AFX_PMSG* m_pVtable;
	AFX_PMSG  m_pRealAdress;
};

class CTrapSwap
{
public:
	void SetTrapOnImportFunc(void* pOldFunc, void* pNewFunc);
	void SetTrap(LPCTSTR dll,LPCTSTR func,void* pNewFunc);
	void SetTrap(LPCTSTR dll,LPCTSTR func,AFX_PMSG pNewFunc){SetTrap(dll,func,*(DWORD**)&pNewFunc);}
	void Swap()
	{
		__int64 t=*(__int64*)m_pFuncTrapped.pFunc;
		*(__int64*)m_pFuncTrapped.pFunc=m_SwapBuf;
		m_SwapBuf=t;
	}
	union PtrConvert{
		AFX_PMSG pClassFunc;
		void*	 pFunc;
	}m_pFuncTrapped;
	__int64 m_SwapBuf;
};
#endif // !defined(AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_)
