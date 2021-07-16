// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListBox.h: interface for the CValueListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
#define AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueListControl.h"
#include "../IconListBox.h"
class CValueListBox : public CValueListControl<CIconListBox>
{
DECLARE_DYNCREATE(CValueListBox);
public:
	CValueListBox();
	virtual ~CValueListBox();
	virtual int Check(int nIndex,int bValue=-1)
	{
		if(nIndex<1||nIndex>GetListSize())
			SetError("Индекс выходит за границы списка значений!");
		int nRes=aValue[nIndex-1].bCheck;
		if(bValue!=-1)
		{
			((CIconListBox *)pWnd)->SetItemImage(nIndex-1,bValue+18);
			aValue[nIndex-1].bCheck=bValue;
		}
		return nRes;
	};
	virtual void AddValue(CValue Val,CString Str="")
	{
		CValueList::AddValue(Val,Str);
		if(Str=="")
		Str=CString(String(Val));
		if (((CIconListBox*)pWnd)->GetImageList()!=NULL)
			((CIconListBox*)pWnd)->InsertString(-1,Str,18);
		else
			((CIconListBox*)pWnd)->InsertString(-1,Str);
		
	};
	virtual void LoadItems(void)
	{
		((CIconListBox*)pWnd)->ResetContent();
		for(int i=0;i<aValue.GetSize();i++)
		{
			if (aValue[i].Present.IsEmpty())
				((CIconListBox*)pWnd)->InsertString(-1,CString(String(aValue[i].Value)));
			else
				((CIconListBox*)pWnd)->InsertString(-1,aValue[i].Present);
			if (((CIconListBox*)pWnd)->GetImageList()!=NULL)
				((CIconListBox*)pWnd)->SetItemImage(i,aValue[i].bCheck+18);
		}
	};
	virtual void GetText(int nIndex,CString &csPresent)
	{
		((CListBox *)pWnd)->GetText(nIndex,csPresent);
	};



};
#endif // !defined(AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
