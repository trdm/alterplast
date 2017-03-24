/////////////////////////////////////////////////////////////////////
// V7Control.h : interface of all 1C++ controls.
//
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro-75@mail.ru
// Copyright (c) 2006-2009. All Rights Reserved.
// Version: 1.0
//
//////////////////////////////////////////////////////////////////////

#pragma once

//ƒл€ того чтобы класс был V7 контролом он должен реализовать этот интерфейс
//—оздание Ё” производитс€ методом јтрибут‘ормы::—оздатьЁлемент”правлени€()

class CV7Control
{
public:
    virtual BOOL CreateControlWnd(CWnd* pParent, CGetDoc7* pDoc, CGetField* pGetField, CBLContext* pUDC) = 0;
    //—оздает окно Ё”.
    //ѕараметры:
    // - pParent - родительское окно (форма);
    // - pDoc - документ отображаемый формой;
    // - pGetField - реквизит формы - под которым будет расположен Ё”;
    // - pUDC - контекст  ќѕа-наследника класса Ё”, может быть NULL.
    //¬озвращает: TRUE - удачно, FALSE - неудачно.

	static void BeginDrag(const CValue& vDragValue);
	static BOOL IsDragOn();
	static void GetDragValue(CValue& vDragValue, COleDataObject* pDataObject);
	static void EndDrag();
};

class IV7ControlContext
{
public:
	virtual CBLContext* GetControlContext() const = 0;
};
