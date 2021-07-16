// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueOLE.h: interface for the CValueOLE class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUEOLE_H__4CB57088_2179_44C4_B6E9_ED7553D91197__INCLUDED_)
#define AFX_VALUEOLE_H__4CB57088_2179_44C4_B6E9_ED7553D91197__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000



#include "ExtOleDispatchDriver.h"
#include "Value.h"
#include <Atlbase.h>

#define TYPE_OLE 300

class CValueOLE : public CValue  
{
public:
	CValueOLE(IDispatch *p=0);
	virtual ~CValueOLE();

    CExtOleDispatchDriver disp;

	BOOL Create(CString csName);

	int  GetType(void)const{return TYPE_OLE;};

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	CValue Method(int iName,CValue **aParams);
	void SetAttribute(int iName,CValue &Val);//установка атрибута
	CValue GetAttribute(int iName);//значение атрибута
	int  FindMethod(CString csName);
	int  FindAttribute(CString csName);

	CString csObjectName;
	CString GetString(void)const{return csObjectName;};
	CString GetTypeString(void)const{return csObjectName;};

};

#endif // !defined(AFX_VALUEOLE_H__4CB57088_2179_44C4_B6E9_ED7553D91197__INCLUDED_)
