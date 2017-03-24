/*
proppage.hpp
Класс для создания страницы свойств ВК, загружаемой не по стандарту.
Использование:
 1. В редакторе ресурсов создайте диалог.
	Style - child
	Border - none
	Visible - false
 2. Запустите ClassWizard, выберите "создать новый класс" для диалога
 3. В созданном классе реализуйте функции:
	BOOL IsDirty(); // Должна возвращать TRUE, если свойства изменены
	BOOL OnApply(); // Вызывается при нажатии Apply или Ok. Должна вернуть TRUE, если свойства сохранены
	static LPCSTR GetTitle(){return "Заголовок вкладки";}
 4. При загрузке компоненты вызовите функцию CPropPageCreator<ВашКлассДиалога>::Init()

  При работе страницы свойств, при изменении свойств для активации кнопки "Применить",
  вызовите функцию 	CPropPageCreator<ВашКлассДиалога>::MakeDirty();
*/

#pragma once

#include "traps.hpp"

template<typename T>
class CPropPageCreator
{
public:

	// Тут будет хранится указатель на COlePropertyPageSite
	static COlePropertyPageSite*& GetPageSite(){static COlePropertyPageSite* pSite;return pSite;}
	
	static void MakeDirty()
	{
		if(COlePropertyPageSite* pSite=GetPageSite())
			pSite->m_pageSite.OnStatusChange(PROPPAGESTATUS_DIRTY);
	}

	class CPropPageDoc
	{
	public:
		T* m_pDlg;
		DWORD m_buf[10];

		CPropPageDoc() : m_pDlg(NULL)
		{
			memset(m_buf, 0, sizeof(m_buf));
		}

		void operator = (class CPropPageDoc const & Dest)
		{
			m_pDlg = NULL;
			memcpy(m_buf, Dest.m_buf, sizeof(m_buf));
		};

		virtual void __stdcall vProc00(void){}
		virtual void __stdcall vProc04(void){}
		virtual void __stdcall ClosePropDlg(void)
		{
			delete m_pDlg;
			GetPageSite()=NULL;
			delete this;
		}
		virtual void __stdcall vProc0C(void){}
		virtual void __stdcall CreatePropDlg(HWND hMainWnd, CRect* rect, BOOL bModal)
		{
			m_pDlg = new T(NULL);
			AfxSetResourceHandle(hMyInst);
			m_pDlg->Create(T::IDD, CWnd::FromHandle(hMainWnd));
			AfxSetResourceHandle(h1CResource);
			m_pDlg->MoveWindow(rect);
		}
		virtual void __stdcall vProc14(void){}
		virtual void __stdcall vProc18(void){}
		virtual void __stdcall vProc1C(DWORD dwParam1, DWORD dwParam2){}
		virtual void __stdcall ActivatePropDlg(DWORD dwFlag)
		{
			if(m_pDlg)
				m_pDlg->ShowWindow(dwFlag);
		}
		virtual void __stdcall vProc24(void){}
		virtual BOOL __stdcall IsSaved(void)
		{
			if(m_pDlg && m_pDlg->IsDirty())
				return FALSE;
			return TRUE;
		}
		virtual HRESULT __stdcall Save(void)
		{
			if(m_pDlg)
				return m_pDlg->OnApply() ? S_OK : E_FAIL;
			return S_OK;
		}
	};

	struct SPropPage
	{
	public:
		BOOL m_bHasDialog;
		DWORD m_1;
		DWORD m_2;
		DWORD m_3;
		DWORD m_4;
		DWORD m_5;
		DWORD m_6;
		DWORD m_DlgW;
		DWORD m_DlgH;
		DWORD m_9;
		DWORD m_10;
		WORD m_ResNum;
		WORD m_ResType;
		CPropPageDoc* m_DlgDoc;
		CString m_Path;
		DWORD m_14;
		DWORD m_15;
		DWORD m_16;
		DWORD m_17;
		DWORD m_18;

		SPropPage() : m_DlgDoc(new CPropPageDoc){}
		~SPropPage(){delete m_DlgDoc;}

		void operator = (SPropPage const & Dest)
		{
			memcpy(this, &Dest, sizeof(*this));
			m_DlgDoc = new CPropPageDoc;
			*m_DlgDoc = *Dest.m_DlgDoc;
		};

		SPropPage(struct SPropPage const & Dest)
		{
			*this = Dest;
		};
	};
	
	typedef CArray<struct SPropPage, struct SPropPage> CArraySPropPage;

	static CTrapSwap*& GetTrap(){static CTrapSwap* pTrap; return pTrap;}

	struct COlePropertyPageSiteWrap : COlePropertyPageSite
	{
		int OnInitDialogWrap(void)
		{
			trapswaper tr(*GetTrap());
			
			int ret=COlePropertyPageSite::OnInitDialog();
			m_TabCtrl.InsertItem(m_TabCtrl.GetItemCount(), T::GetTitle(), 0);

			SPropPage newPropPage;
			newPropPage.m_bHasDialog = FALSE;

			CArraySPropPage* PropPages = reinterpret_cast<CArraySPropPage*>(&m_PropPages);
			PropPages->Add(newPropPage);

			GetPageSite()=this;
			return ret;
		}
	};

	static void Init()
	{
		CTrapSwap*& tr=GetTrap();
		tr=new CTrapSwap;
		tr->SetTrapByName("frame.dll", "?OnInitDialog@COlePropertyPageSite@@MAEHXZ",
			&COlePropertyPageSiteWrap::OnInitDialogWrap);
	}
};