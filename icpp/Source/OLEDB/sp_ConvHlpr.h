#if !defined(AFX_SP_CONVHLPR_H__ADC7E0A7_42A4_4825_8CDF_8B123DDB8CF4__INCLUDED_)
#define AFX_SP_CONVHLPR_H__ADC7E0A7_42A4_4825_8CDF_8B123DDB8CF4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __oledb_h__
#include <oledb.h>
#endif // __oledb_h__

#ifndef __msdadc_h__
#include <msdadc.h>
#endif // __msdadc_h__

extern "C" const CLSID CLSID_DataConvert;

//_____________________________________________________________________________
//
class COLEDBConvertHelper
{ // сфижено у microsoft, права ихние.
public:
	HRESULT FinalConstruct()
	{
		HRESULT hr = ::CoCreateInstance(CLSID_DataConvert, NULL, CLSCTX_INPROC_SERVER, IID_IDataConvert, (void**)&m_spConvert);

		if(FAILED(hr))
			return hr;

		DCINFO rgInfo[] = {{DCINFOTYPE_VERSION, {VT_UI4, 0, 0, 0, 0x0}}};
		
		CComPtr<IDCInfo> spIDCInfo;
		
		hr = m_spConvert->QueryInterface(&spIDCInfo);
		if(hr == S_OK)
		{
			V_UI4(&rgInfo->vData) = 0x200;  // OLEDB Version 02.00
			spIDCInfo->SetInfo(1, rgInfo);
		}
		
		return hr;
	}
	
	CComPtr<IDataConvert> m_spConvert;
};
#endif // !defined(AFX_SP_CONVHLPR_H__ADC7E0A7_42A4_4825_8CDF_8B123DDB8CF4__INCLUDED_)