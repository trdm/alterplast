//////////////////////////////////////////////////////////////////////
// V7DataProvider.h: interfaice extent data providers for V7.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include "DataProvider.h"

class CV7DataRow
{
public:
    virtual const CValue& GetValue(int nIndex) const = 0;
    static void FormatValue(const CValue& Value, CString& strValue)
    {
        if (Value.GetTypeCode() == NUMBER_TYPE_1C)
        {
            if (Value == 0L)
                strValue.Empty();
            else
            {
                strValue = Value.Format();
                strValue.TrimLeft(' ');
            }
        }
        else
            strValue = Value.Format();
    };
};

class CV7DataProvider
{
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const = 0; //to avoid dynamic cast for each row
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const {};
    virtual CDataRow* BuildRowByValue(const CValue& value) {return NULL;};
};

