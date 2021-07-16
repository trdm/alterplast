// јвторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListControl.h: interface for the CValueListControl class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUELISTCONTROL_H__4058CA8B_60C3_4930_ABC5_FD2F60A47526__INCLUDED_)
#define AFX_VALUELISTCONTROL_H__4058CA8B_60C3_4930_ABC5_FD2F60A47526__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueList.h"
template <class CTYPE>
class CValueListControl : public CValueList  
{
public:
//	virtual int Check(int nIndex,int bValue=-1);//дл€ CListBox
	virtual void GetText(int nIndex,CString &csPresent){;};

	virtual void LoadItems(void);
	virtual int CurSel(int nCur=-1);
	virtual void SetSize(int nSize);
	virtual void MoveValue(int nCount,int nIndex);
	virtual void RemoveValue(int nIndex, int nCount=-1);
	virtual void FromSeparatedString(CString Str);
	virtual void SortByPresent(int nDirect=-1);
	virtual void Sort(int nDirect=-1);
	virtual void RemoveAll(void);
	virtual int Set(CString Str,CValue Val);
	virtual void SetValue(int nIndex, CValue Val, CString Str="", int nCount=0);
	virtual void InsertValue(int nIndex,CValue Val,CString Str="",int nCount=0);
	virtual void AddValue(CValue Val,CString Str="");


	int GetListSize(void) const;

};
//–≈јЋ»«ј÷»я ЎјЅЋќЌќ¬


template <class CTYPE>
void CValueListControl<CTYPE>::
LoadItems(void)
{
	((CTYPE *)pWnd)->ResetContent();
	for(int i=0;i<aValue.GetSize();i++)
	{
		if (aValue[i].Present.IsEmpty())
			((CTYPE *)pWnd)->InsertString(-1,CString(String(aValue[i].Value)));
		else
			((CTYPE *)pWnd)->InsertString(-1,aValue[i].Present);
	}
}



template <class CTYPE>
void CValueListControl<CTYPE>::
MoveValue(int nCount,int nIndex)
{
	CValueList::MoveValue(nCount,nIndex);

	if(nCount==0)
		return;
	nIndex--;
	int ndelta=0;
	if(nCount<0)
		ndelta=1;
	int nNewIndex=nIndex+nCount+1;
	if(nIndex+ndelta==nNewIndex+1)
		return;

	if(nIndex<0||nIndex>=GetListSize())
		SetError("»ндекс не входит в границы списка значений!");

	CString csPresent;
 	GetText(nIndex,csPresent);

	if(nNewIndex<0)
		nNewIndex=0;
	if(nNewIndex>=((CTYPE *)pWnd)->GetCount())
		nNewIndex=((CTYPE *)pWnd)->GetCount();

	((CTYPE *)pWnd)->InsertString(nNewIndex,csPresent);
	((CTYPE *)pWnd)->DeleteString(nIndex+ndelta);
}

template <class CTYPE>
int CValueListControl<CTYPE>::
GetListSize()const
{
	return ((CTYPE *)pWnd)->GetCount();
}


template <class CTYPE>
void CValueListControl<CTYPE>::
SetSize(int nSize)
{
	CValueList::SetSize(nSize);
	((CTYPE *)pWnd)->InitStorage(nSize,100);
}


template <class CTYPE>
void CValueListControl<CTYPE>::
FromSeparatedString(CString Str)
{
	CValueList::FromSeparatedString(Str);
	LoadItems();
}


template <class CTYPE>
void CValueListControl<CTYPE>::
Sort(int nDirect)
{
	CValueList::Sort(nDirect);
	LoadItems();
}


template <class CTYPE>
void CValueListControl<CTYPE>::
SortByPresent(int nDirect)
{
	CValueList::SortByPresent(nDirect);
	LoadItems();
}


template <class CTYPE>
void CValueListControl<CTYPE>::
RemoveAll(void)
{
	CValueList::RemoveAll();
	((CTYPE *)pWnd)->ResetContent();
	ASSERT(((CTYPE *)pWnd)->GetCount() == 0);
}



template <class CTYPE>
void CValueListControl<CTYPE>::
RemoveValue(int nIndex, int nCount)
{
	if(nCount<=0)
		nCount=1;
	CValueList::RemoveValue(nIndex,nCount);

	nIndex--;
	for(int i=nIndex;i<nIndex+nCount;i++)
	{
		if(i>((CTYPE *)pWnd)->GetCount())
			break;
		((CTYPE *)pWnd)->DeleteString(i);
	}
}

template <class CTYPE>
int CValueListControl<CTYPE>::
CurSel(int nCur)
{
	int nRes=((CTYPE *)pWnd)->GetCurSel()+1;
	if(GetListSize()==0)
		nRes=0;
	if(nCur!=-1)
	{
		if(nCur<1||nCur>((CTYPE *)pWnd)->GetCount())
			SetError("»ндекс выходит за границы списка значений!");
		((CTYPE *)pWnd)->SetCurSel(nCur-1);
		((CTYPE *)pWnd)->SetFocus();
	}
	return nRes;
}


template <class CTYPE>
int CValueListControl<CTYPE>::
Set(CString Str,CValue Val)
{
	int nIndex=CValueList::Set(Str,Val);
	nIndex--;
	if(nIndex>=0)
	{
		((CTYPE *)pWnd)->InsertString(nIndex-1,Val.GetString());
		((CTYPE *)pWnd)->DeleteString (nIndex);
	}
	return nIndex;
}

template <class CTYPE>
void CValueListControl<CTYPE>::
SetValue(int nIndex, CValue Val, CString Str, int nCount)
{
	if(Str=="")
		Str=Val.GetString();
	CValueList::SetValue(nIndex,Val,Str,nCount);

	if(nCount<=0)
		nCount=1;
	nIndex--;
	if(nIndex<0||nIndex>=GetListSize())
		SetError("»ндекс выходит за границы списка значений!");


	for(int i=0;i<nCount;i++)
	{
		((CTYPE *)pWnd)->InsertString(nIndex-1,Str);
		((CTYPE *)pWnd)->DeleteString (nIndex);
	}

}


template <class CTYPE>
void CValueListControl<CTYPE>::
InsertValue(int nIndex, CValue Val, CString Str, int nCount)
{
	if(nCount<=0)
		nCount=1;
	if(Str=="")
		Str=Val.GetString();
//	if(nIndex<1||nIndex>GetListSize())
//		SetError("»ндекс выходит за границы списка значений!");

	CValueList::InsertValue(nIndex,Val,Str,nCount);
	nIndex--;
	if(nIndex>=GetListSize())
		nIndex=GetListSize()-1;
	if(nIndex<0)
		nIndex=0;

	for(int i=0;i<nCount;i++)
		((CTYPE *)pWnd)->InsertString(nIndex,Str);
}


template <class CTYPE>
void CValueListControl<CTYPE>::
AddValue(CValue Val,CString Str)
{
	if(Str=="")
		Str=Val.GetString();
	CValueList::AddValue(Val,Str);
	((CTYPE *)pWnd)->InsertString(-1,Str);
}

#endif // !defined(AFX_VALUELISTCONTROL_H__4058CA8B_60C3_4930_ABC5_FD2F60A47526__INCLUDED_)
