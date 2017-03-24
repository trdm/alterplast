// TForm.h: interface for the CTForm class.
#ifndef AFX_TFORM_H
#define AFX_TFORM_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "telecont.h"
#include "blmap.h"
struct SLexem;

class CTForm : public CTeleCont
{
public:
	bool IsAttribExist(SLexem& name);
	enum TypesOfControl{
		Edit,
		Masked,
		Static,
		GroupBox,
		Button,
		Table,
		Radio,
		ComboBox,
		CheckBox,
		ListBox,
		lastTypeOfControl,
	};
	static const char *m_TypeNames[];
	static const char *m_1CTypeNames[];
	CTForm(CParser*,CWorkBookDoc* pDoc);
	
	void FillGroup(CPtrArray& arr);
	virtual const int	GetGroupCount() const;
	virtual const int*	GetImagesNum()  const;
	virtual LPCTSTR*	GetGroupNames() const;

	CBLMap m_mNames;
};


class CTFormEntry:public CTeleEntry{
public:
	CTFormEntry(CTForm::TypesOfControl type,LPCTSTR name):
		CTeleEntry(name),m_type(type){}
	void GetHint(CString& buf);
	CTForm::TypesOfControl m_type;
	char m_DataType;
	int m_len;
	int m_prec;
	int m_kind;
};
#endif
