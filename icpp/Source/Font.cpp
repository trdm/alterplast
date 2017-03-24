
#include "StdAfx.h"
#include "Font.h"

BEGIN_BL_METH_MAP(CBLFont)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CBLFont)
    BL_PROP("Type", "Вид", &GetPropType, NULL)
    BL_PROP("Name", "Имя", &GetPropName, NULL)
    BL_PROP("Size", "Размер", &GetPropSize, NULL)
    BL_PROP("Bold", "Жирный", &GetPropBold, NULL)
    BL_PROP("Italic", "Наклонный", &GetPropItalic, NULL)
    BL_PROP("Underline", "Подчеркивание", &GetPropUnderline, NULL)
    BL_PROP("StrikeOut", "Зачеркивание", &GetPropStrikeOut, NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CBLFont, "Font", "Шрифт", 0, NULL, NULL, 7);

#define F_EMPTY     0x8000
#define F_BOLD      0x0001
#define F_ITALIC    0x0002
#define F_UNDERLINE 0x0004
#define F_STRIKEOUT 0x0008

CBLFont::CBLFont()
{
    m_fontType = ftAuto;
    m_wFlags = F_EMPTY;
}

CBLFont::~CBLFont()
{
}

char const* CBLFont::GetCode(void)const
{
    if (m_fontType == ftAuto)
        return "Авто";
    FillProps();
    return m_strCode;
}

BOOL CBLFont::GetPropType(CValue& Value) const
{
    Value = m_fontType;
    return TRUE;
}

BOOL CBLFont::GetPropName(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_strFaceName;
    }
    return TRUE;
}

BOOL CBLFont::GetPropSize(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_wSize;
    }
    return TRUE;
}

BOOL CBLFont::GetPropBold(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_wFlags & F_BOLD ? 1 : 0;
    }
    return TRUE;
}

BOOL CBLFont::GetPropItalic(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_wFlags & F_ITALIC ? 1 : 0;
    }
    return TRUE;
}

BOOL CBLFont::GetPropUnderline(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_wFlags & F_UNDERLINE ? 1 : 0;
    }
    return TRUE;
}

BOOL CBLFont::GetPropStrikeOut(CValue& Value) const
{
    if (m_fontType == ftAuto)
        Value.Reset();
    else
    {
        FillProps();
        Value = m_wFlags & F_STRIKEOUT ? 1 : 0;
    }
    return TRUE;
}

BOOL CBLFont::_Create(CValue** ppValue)
{
    if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
    {
        int nFontType = ppValue[0]->GetNumeric();
        if (nFontType < 0 || nFontType >= LastFontType)
        {
            CBLModule::RaiseExtRuntimeError("Недопустимое значение первого параметра.", 0);
            return FALSE;
        }

        switch (nFontType)
        {
        case ftAuto:
            return TRUE;
            break;
        case ftAbsolute:
            m_font.Detach();
            m_fontType = ftAuto;
            break;
        case ftOEMFixedFont:
            m_font.CreateStockObject(OEM_FIXED_FONT);
            m_fontType = ftOEMFixedFont;
            break;
        case ftANSIFixedFont:
            m_font.CreateStockObject(ANSI_FIXED_FONT);
            m_fontType = ftANSIFixedFont;
        case ftANSIVariableFont:
            m_font.CreateStockObject(ANSI_VAR_FONT);
            m_fontType = ftANSIVariableFont;
            break;
        case ftSystemFont:
            m_font.CreateStockObject(SYSTEM_FONT);
            m_fontType = ftSystemFont;
            break;
        case ftDefaultGUIFont:
            m_font.CreateStockObject(DEFAULT_GUI_FONT);
            m_fontType = ftDefaultGUIFont;
            break;
        }
    }
    else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
    {
        CString strFaceName = ppValue[0]->GetString();
        int nHeight = ToSizeInPixels(ppValue[1]->GetNumeric());
        BOOL bBold = ppValue[2]->GetNumeric();
        BOOL bItalic = ppValue[3]->GetNumeric();
        BOOL bUnderline = ppValue[4]->GetNumeric();
        BOOL bStrikeOut = ppValue[5]->GetNumeric();
        m_font.CreateFont(nHeight, 0, 0, 0,
            bBold ? FW_BOLD : FW_NORMAL, bItalic, bUnderline, bStrikeOut,
            DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH | FF_DONTCARE,
            strFaceName);
        m_fontType = ftAbsolute;
    }
    else if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
    {
        CValue& vFont = *ppValue[0];
        CHECK_BLTYPE(vFont, CBLFont);
        CBLFont* pBLFont = static_cast<CBLFont*>(ppValue[0]->GetContext());
        if (pBLFont->m_fontType == ftAuto)
        {
            CBLModule::RaiseExtRuntimeError("Недопустимое значение первого параметра.", 0);
            return FALSE;
        }
		LOGFONT lf;
		pBLFont->m_font.GetLogFont(&lf);
		if (ppValue[1]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			strncpy(lf.lfFaceName, ppValue[1]->GetString(), LF_FACESIZE - 1);
		if (ppValue[2]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			lf.lfHeight = ToSizeInPixels(ppValue[2]->GetNumeric());
		if (ppValue[3]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			lf.lfWeight = ppValue[3]->GetNumeric().operator long() ? FW_BOLD : FW_NORMAL;
		if (ppValue[4]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			lf.lfItalic = ppValue[4]->GetNumeric();
		if (ppValue[5]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			lf.lfUnderline = ppValue[5]->GetNumeric();
		if (ppValue[6]->GetTypeCode() != UNSPECIFIED_TYPE_1C)
			lf.lfStrikeOut = ppValue[6]->GetNumeric();
		m_font.CreateFontIndirect(&lf);
		m_fontType = ftAbsolute;
    }
    m_wFlags = F_EMPTY;
    return TRUE;
}

void CBLFont::Attach(CFont* pFont)
{
	m_font.DeleteObject();

	LOGFONT lf;
	pFont->GetLogFont(&lf);
	m_font.CreateFontIndirect(&lf);

	m_fontType = ftAbsolute;
	m_wFlags = F_EMPTY;
}

CBLFont::operator CFont* ()
{
    if (m_fontType == ftAuto)
        return NULL;
    else
        return &m_font;
}

void CBLFont::FillProps() const
{
    if (m_wFlags == F_EMPTY && m_fontType != ftAuto)
    {
        m_wFlags = 0;

        LOGFONT lf;
        m_font.GetLogFont(&lf);
        m_strFaceName = lf.lfFaceName;
        HDC hDC = ::GetDC(NULL);
        m_wSize = MulDiv(-lf.lfHeight, 72, GetDeviceCaps(hDC, LOGPIXELSY));
        ::ReleaseDC(NULL, hDC);
        if (lf.lfWeight == FW_BOLD)
            m_wFlags |= F_BOLD;
        if (lf.lfItalic)
            m_wFlags |= F_ITALIC;
        if (lf.lfUnderline)
            m_wFlags |= F_UNDERLINE;
        if (lf.lfStrikeOut)
            m_wFlags |= F_STRIKEOUT;

        m_strCode.Format("%s, %hu", m_strFaceName.operator LPCTSTR(), m_wSize);
    }
}

int CBLFont::ToSizeInPixels(int nSizeInPoints)
{
    HDC hDC = ::GetDC(NULL);
    int nHeight = -MulDiv(nSizeInPoints, GetDeviceCaps(hDC, LOGPIXELSY), 72);
    ::ReleaseDC(NULL, hDC);
    return nHeight;
}
