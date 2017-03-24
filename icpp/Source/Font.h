//////////////////////////////////////////////////////////////////////
// Font.h: interface for the CBLFont class.
// Written by Dmitriy Oshchepkov aka DmitrO, mailto: dmitro@russian.ru
// Version: 1.0
//////////////////////////////////////////////////////////////////////

#pragma once

#include "PropDefs.h"
#include "mycontextbase.h"

enum FontType
{
    ftAuto, ftAbsolute,
    ftOEMFixedFont, ftANSIFixedFont, ftANSIVariableFont, ftSystemFont, ftDefaultGUIFont,
    LastFontType
};

class CBLFont : public CMyContextBase
{
    DECLARE_DYNCREATE(CBLFont)
public:
    CBLFont();
    virtual ~CBLFont();

    FontType GetType() const;
    operator CFont* ();

    virtual char const *	GetCode(void)const;

    BOOL GetPropType(CValue& Value) const;
	using CMyContextBase::GetPropName;
    BOOL GetPropName(CValue& Value) const;
    BOOL GetPropSize(CValue& Value) const;
    BOOL GetPropBold(CValue& Value) const;
    BOOL GetPropItalic(CValue& Value) const;
    BOOL GetPropUnderline(CValue& Value) const;
    BOOL GetPropStrikeOut(CValue& Value) const;

    virtual BOOL _Create(CValue** ppValue);
	virtual void Attach(CFont* pFont);

    DECLARE_MY_CONTEXT()
private:
    FontType m_fontType;
    mutable CFont m_font;
    
    mutable WORD m_wFlags;
    mutable WORD m_wSize;
    mutable CString m_strFaceName;
    mutable CString m_strCode;

    void FillProps() const;
    int ToSizeInPixels(int nSizeInPoints);

	// artbear - исключительно для подавления предупреждения о скрытии базовой виртуальной функции !
	virtual char const * GetPropName(int int1, int int2)const { return CMyContextBase::GetPropName(int1, int2); };
};
