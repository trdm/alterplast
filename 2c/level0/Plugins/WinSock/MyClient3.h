// Авторские права: VTOOLS.RU (info@vtools.ru) 2002-2004г.
// SQLLite.h: interface for the CMyClient3 class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
#define AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "../../COMPILER/Value.h"
#include "SocketClass.h"

class CMyClient3 : public CValue  
{
DECLARE_DYNCREATE(CMyClient3 );
public:
	CMyClient3();
	virtual ~CMyClient3();
	

	//РАБОТА КАК АГРЕГАТНОГО ОБЪЕКТА
	static CMethods Methods;
	//Эти методы нужно переопределить в ваших агрегатных объектах:
	CMethods* GetPMethods(void){return &Methods;};//получить ссылку на класс помощник разбора имен атрибутов и методов
	void PrepareNames(void);//этот метод автоматически вызывается для инициализации имен атрибутов и методов
	CValue Method(int iName,CValue **aParams);//вызов метода
	virtual CString GetString(void);
	virtual CString GetTypeString(void)const;
	//virtual CValue GetAttribute(int iName);//значение атрибута

	//////////////////////////////////////////////////////////////////////
	//Пользовательские методы
	//////////////////////////////////////////////////////////////////////
	typedef BOOL (CMyClient3 ::*MethFunc)(CValue&rez,CValue**ppV);

	BOOL Connect(CValue &rez,CValue**ppV);
	BOOL Send(CValue &rez,CValue**ppV);
	BOOL Receive(CValue &rez,CValue**ppV);
	BOOL Close(CValue &rez,CValue**ppV);
	BOOL LoadToFile(CValue &rez,CValue**ppV);
	BOOL Port(CValue &rez,CValue**ppV);

	//Данные
	CSocketClass	m_socket;
	int				m_nPort;
	//CValue			m_vContext;

	BOOL			bWasDataReceive;
	CString			csData;

//	static CRITICAL_SECTION	m_cs;

};

#endif // !defined(AFX_MyOBJ_H__765DABD5_7BA1_4885_B13B_C36C4781D701__INCLUDED_)
