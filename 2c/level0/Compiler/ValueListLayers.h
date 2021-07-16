// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListLayers.h: interface for the CValueListLayers class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ValueListLayers_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
#define AFX_ValueListLayers_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueListControl.h"
class CValueListLayers : public CValueList
{
DECLARE_DYNCREATE(CValueListLayers);
public:
	CValueListLayers();
	virtual ~CValueListLayers();


	virtual int UseLayer(CString csName,int nMode);
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

	void CopyValues();//синхронизация списков (базового класа и списка слоев визуального объекта)

	CString GetTypeString(void)const;

};
#endif // !defined(AFX_ValueListLayers_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
