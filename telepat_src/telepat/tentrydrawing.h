// tentrydrawing.h
#ifndef TENTRYDRAWING_H
#define TENTRYDRAWING_H

enum ImagesNumbers {
	IDI_CMATREXP_,
	IDI_CMATRPRIV_,
	IDI_CMMETEXP_,
	IDI_CMMETPRIV_,
	IDI_DLGFIELD_,
	IDI_GMATREXP_,
	IDI_GMATRPRIV_,
	IDI_GMMETEXP_,
	IDI_GMMETPRIV_,
	IDI_KEYATR_,
	IDI_KEYFUNC_,
	IDI_KEYWORD_,
	IDI_LOCALVAR_,
	IDI_METAATR_,
	IDI_TEMPLATES_,
	IDI_STRINGS_,
};

class CTEntryDrawing{
public:
	static void GetSysColors();
	static void Draw(LPDRAWITEMSTRUCT pds);
	
	static int m_MaxTextWidth;
	static int m_EntryHeight;
	static int m_TextHeight;
	static COLORREF m_BkColor;
	static COLORREF m_ForeColor;
	static COLORREF m_InvertColor;
	static COLORREF m_ToolTipColor;
	static COLORREF m_ToolTipFColor;
	static CPtrArray	m_iconList;
	static void Init();
};

#endif