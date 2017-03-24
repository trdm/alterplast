
#include "StdAfx.h"
#include "V7Control.h"

#pragma once

BOOL _bDragOn = FALSE;
CValue _vDragValue;

void CV7Control::BeginDrag(const CValue& vDragValue)
{
	_bDragOn = TRUE;
	_vDragValue = vDragValue;
}

BOOL CV7Control::IsDragOn()
{
	return _bDragOn;
}

void CV7Control::GetDragValue(CValue& vDragValue, COleDataObject* pDataObject)
{
	if (_bDragOn)
	{
		vDragValue = _vDragValue;
		return;
	}

	//Общее преобразование значения COleDataObject в CValue
	if (pDataObject->IsDataAvailable(CF_TEXT))
	{
		HGLOBAL hGlobal = pDataObject->GetGlobalData(CF_TEXT);
		vDragValue = (LPCSTR)::GlobalLock(hGlobal);
		::GlobalUnlock(hGlobal);
	}
}

void CV7Control::EndDrag()
{
	_bDragOn = FALSE;
	_vDragValue.Reset();
}
