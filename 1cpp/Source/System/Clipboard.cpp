/////////////////////////////////////////////////////////////////////////////
// CClipboard.cpp: implementation of the CClipboard class.
// develop by artbear
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "Clipboard.h"

//_____________________________________________________________________________
//
// CClipboard Maps
//_____________________________________________________________________________
//
BEGIN_BL_METH_MAP(CClipboard)
	BL_METH_FUNC("Get", "Получить", 0, funcGet)
	BL_METH_PROC("Set", "Установить", 1, funcSet)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CClipboard)
END_BL_PROP_MAP()

IMPLEMENT_MY_CREATE_CONTEXT(CClipboard, "Clipboard", "БуферОбмена");

const int USED_CLIPBOARD_TEXT_FORMAT = CF_TEXT;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

BOOL CClipboard::funcGet(CValue &RetVal, CValue **ppValue)
{
	RetVal = Get();
	return TRUE;
}

BOOL CClipboard::funcSet(CValue **ppValue)
{
	ValidateParamString(*ppValue[0], "CClipboard::Set - должна быть передана строка");
	Set(ppValue[0]->GetString());

	return TRUE;
}

CString CClipboard::Get()
{
	CString result = "";

	COleDataObject clipboardData;
	clipboardData.AttachClipboard();
	
	if (!clipboardData.IsDataAvailable(USED_CLIPBOARD_TEXT_FORMAT))
		return result;
	
	const HGLOBAL hGlobal = clipboardData.GetGlobalData(USED_CLIPBOARD_TEXT_FORMAT);
	if (!hGlobal)
		RuntimeError("CClipboard::Get - Ошибка работы с буфером обмена");

	const LPVOID lpText = GlobalLock(hGlobal);
	
	result = static_cast<char*>(lpText);

	GlobalUnlock(hGlobal);
	GlobalFree(hGlobal);

	return result;
}

void CClipboard::Set(const CString& str)
{
	// COleDataSource::SetClipboard автоматом удалит выделенный pDataSource и выделенную глобальную память
	COleDataSource* const pDataSource = new COleDataSource;

	const int nTextLen = (str.GetLength()+1)*sizeof(TCHAR);
	const HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, nTextLen);
	if (!hGlobal)
		RuntimeError("CClipboard::Set - Ошибка выделения памяти");

	LPVOID lpText = GlobalLock(hGlobal);
	memcpy(lpText, str, nTextLen);
	GlobalUnlock(hGlobal);

	pDataSource->CacheGlobalData( USED_CLIPBOARD_TEXT_FORMAT, hGlobal );
	
	pDataSource->SetClipboard();
}
