// HackVirtual.h: interface for the CHackVirtual class.
#if !defined(AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_)
#define AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_

class CHackVirtual  
{
public:
	CHackVirtual();
	~CHackVirtual();
	void Hack(void* pObject,int offset,AFX_PMSG pFunc);
	void UnHack();
	AFX_PMSG* m_pVtable;
	AFX_PMSG  m_pRealAdress;
};

class CHackSwap
{
public:
	void Hack(LPCTSTR dll,LPCTSTR func,void* pNewFunc);
	void Hack(LPCTSTR dll,LPCTSTR func,AFX_PMSG pNewFunc)
	{
		Hack(dll,func,*(DWORD**)&pNewFunc);
	}
	void Swap()
	{
		BYTE t,*ptr1=m_SwapBuf,*ptr2=(BYTE*)m_pFuncHacked.pFunc;
		for(int i=0;i<6;i++)
			t=*ptr1,*ptr1++=*ptr2,*ptr2++=t;
	}
	union PtrConvert{
		AFX_PMSG pClassFunc;
		void*	 pFunc;
	}m_pFuncHacked;
	void* m_pFuncHack;
	BYTE m_SwapBuf[6];
};
#endif // !defined(AFX_HACKVIRTUAL_H__CABBF8FA_D064_4C1F_8167_41926B3E6BC4__INCLUDED_)
