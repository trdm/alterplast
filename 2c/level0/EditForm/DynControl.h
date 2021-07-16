// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.

#if !defined(AFX_DYNCONTROL_H__34FE9A1D_7A03_4FCC_BE39_D6BDBC0AF8F9__INCLUDED_)
#define AFX_DYNCONTROL_H__34FE9A1D_7A03_4FCC_BE39_D6BDBC0AF8F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../dialogplugins.h"
/*
enum
{
	STRING_DATA=0,
	INT_DATA,
	BOOL_DATA,
	FUNCTION_DATA,//STRING+INT
	COLOR_DATA,
	LIST_DATA,//выбор строки из заранее определенного списка STRING+INT, где INT - номер списка выбора
	DOUBLE_DATA,
};
*/
class ControlData
{
public:
	CString csName;	//имя свойства
	int nType;		//тип данных
	//Данные
	CString sData;
	union
	{
		int nData;
		bool bData;
		COLORREF Color;
		double fData;
	};
};

class CDynControl  
{
public:
	CDynControl(CWnd* pWnd, int nControlType, BOOL bConfigMode=1,CString csName="");
	virtual ~CDynControl();

// Attributes
public:
	BOOL m_bConfigMode;
	CFont m_fontDefault;
	HWND m_hWnd;
	CRect m_Rect;
	int m_nID;	
	int nCreate;
	CWnd* m_pWnd;
	CWnd* m_pParentWnd;
	int m_nControlType;

	int m_x1;//абс. положение 1
	int m_y1;
	int m_x2;//абс. положение 2
	int m_y2;
	int m_x3;//новое абс. положение при авторесайзе
	int m_y3;


	CString m_csName;//идентификатор
	CString m_csValue;
	//CString m_csFunction;
	bool m_bVisible;
	bool m_bEnable;
	bool m_nTabOrder;
	CString m_csVAllign;
	CString m_csHAllign;
	bool m_bAutoSizeDown;
	bool m_bAutoSizeRight;

	int IsNew;//признак того что нужно обработать массив имен доп. свойств
	CString csPropertyName;
	CString m_csObjName;
	CValue	m_Property;//дополнительные свойства
	CString m_csLayer;
	CString m_csDescription;//описание (для контекстной помощи)


	CArray <ControlData,ControlData> aDataList;
	int nMaxDataList;


	CValue eventVal;//текущий объект - обработчик событий
	int		bWasEdit;//признак сохранения данных (для таблицы)
	int		bWasFill;
	//COLORREF m_Color;
// Operations
public:
	CWnd* GetWnd(){return m_pWnd;};
	void OnUpdate();
	void AddControl(CValue &Val);
	void AddDialog(CWnd *pControl,CPoint* ppoint,int cx,int cy,CString name,int nStyle0=0,int nStyleE=0);
	void SaveToValue(CValue &Val);

	void AddColorProperty(CString csName,int Data=0);
	void AddIntProperty(CString csName,int Data=0);
	void AddDoubleProperty(CString csName,double Data);
	void AddBoolProperty(CString csName,bool Data=0);
	void AddStringProperty(CString csName,int nList,CString sData);
	void AddFuncProperty(CString csName,int Message,CString csFunction);
	void SetFuncProperty(CString csName,CString csFunction);
	void SetBoolProperty(CString csName,bool Data);

	void AddShowProperty(CPropertyBar *p,CString csModuleName,CString csVarValues="...");
	void PrepareDataArray();
	void FillDataArray();
	

	BOOL GetFirstFunctionParams(CString &csName,CString &csFunction);
	

	void EnableWindow(BOOL bEnable);
	void AddControl(CPoint* ppoint);
	void AddControl(CRect r);
	void AddControl();
	int GetControlType() { return m_nControlType; }
	void CreateMicroForm();

	// general control properties
	static void ResetID();

};

#endif // !defined(AFX_DYNCONTROL_H__34FE9A1D_7A03_4FCC_BE39_D6BDBC0AF8F9__INCLUDED_)
