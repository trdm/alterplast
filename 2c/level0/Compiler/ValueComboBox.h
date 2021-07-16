// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueComboBox.h: interface for the CValueComboBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUECOMBOBOX_H__7CC18E77_CED0_4E63_8158_3AC31E2204AB__INCLUDED_)
#define AFX_VALUECOMBOBOX_H__7CC18E77_CED0_4E63_8158_3AC31E2204AB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "ValueListControl.h"

class CValueComboBox : public CValueListControl<CComboBox>
{
DECLARE_DYNCREATE(CValueComboBox);
public:
	CValueComboBox();
	virtual ~CValueComboBox();

	virtual void GetText(int nIndex,CString &csPresent)
	{
		((CComboBox *)pWnd)->GetLBText(nIndex,csPresent);
	};
};

#endif // !defined(AFX_VALUECOMBOBOX_H__7CC18E77_CED0_4E63_8158_3AC31E2204AB__INCLUDED_)


