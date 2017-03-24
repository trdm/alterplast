// ValueTo.cpp: implementation of the CValueTo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueTo.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//_____________________________________________________________________________
//
CString CValueToDBTYPE::AsSTR(BYTE nParamLen)
{
	CString strVal;
	strVal.Empty();

	switch(m_nTypeCode)
	{
	case REFERENCE_TYPE_1C:
	case DOCUMENT_TYPE_1C:
	case ENUM_TYPE_1C:
	case CALENDAR_TYPE_1C:
	case CALCULATIONKIND_TYPE_1C:
	case ACCOUNT_TYPE_1C:
	case SUBCONTOKIND_TYPE_1C:
	case CHART_OF_ACC_TYPE_1C:
		{
			switch(nParamLen)
			{
			case 9:
				strVal = CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::ShortString);
				break;
			
			case 13:
				strVal = CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::LongString);
				break;
			
			case 23:
				strVal = CMetaDataWork::GetObjDBString(*m_pValue, CMetaDataWork::VeryLongString);
				break;
			
			default:
				strVal = m_pValue->GetString();
				break;
			}
		}
		break;
	
	case STRING_TYPE_1C:
	case NUMBER_TYPE_1C:
	case UNDEFINE_TYPE_1C:
	case DATE_TYPE_1C:
		{
			strVal = m_pValue->GetString();
		}
		break;
	
	default:
		return strVal;
	}
	
	if(strVal.GetLength() < nParamLen)
	{
		// добиваем пробелами справа, если передана строка меньшего размера, 
		// чем заявлено параметром.
		//
		while (strVal.GetLength() < nParamLen)
			strVal += " ";
	}
	else if(strVal.GetLength() > nParamLen)
	{
		// Если же переданная строка больше, чем заявлено для параметра, 
		// то обрезаем по левому краю.
		//
		strVal = strVal.Left(nParamLen);
	}

	return strVal;
}

//_____________________________________________________________________________
//
DBDATE CValueToDBTYPE::AsDBDATE()
{
	DBDATE dbDate = {0, 0, 0};
	
	switch(m_nTypeCode)
	{
	case DATE_TYPE_1C:
		{
			CDate dateVal(m_pValue->GetDate());

			dbDate.year	 = dateVal.GetYear();
			dbDate.month = dateVal.GetMonth();
			dbDate.day	 = dateVal.GetMonthDay();
		}
		break;
	
	default:
		return dbDate;
	}

	return dbDate;

}

//_____________________________________________________________________________
//
DECIMAL CValueToDBTYPE::AsDECIMAL()
{
	DECIMAL Dec;

	DECIMAL_SETZERO(Dec)
	
	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<DECIMAL> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return Dec;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_DECIMAL, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return Dec;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_DECIMAL, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(DECIMAL), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return Dec;

			Dec = *(static_cast<DECIMAL*>(pDataOut));
		}
		break;
	default:
		return Dec;
	}

	return Dec;
}

//_____________________________________________________________________________
//
SHORT CValueToDBTYPE::AsI2()
{
	SHORT nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
LONG CValueToDBTYPE::AsI4()
{
	LONG nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
FLOAT CValueToDBTYPE::AsR4()
{
	FLOAT nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
DOUBLE CValueToDBTYPE::AsR8()
{
	DOUBLE nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
LARGE_INTEGER CValueToDBTYPE::AsCY()
{
	LARGE_INTEGER nRet;
	nRet.QuadPart = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<LARGE_INTEGER> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return nRet;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_CY, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return nRet;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_CY, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(LARGE_INTEGER), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return nRet;

			nRet = *(static_cast<LARGE_INTEGER*>(pDataOut));
		}
		break;
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
DATE CValueToDBTYPE::AsDATE()
{
	DATE nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<DATE> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return nRet;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_DATE, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return nRet;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_DATE, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(DATE), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return nRet;

			nRet = *(static_cast<DATE*>(pDataOut));
		}
		break;
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
VARIANT_BOOL CValueToDBTYPE::AsBOOL()
{
	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			double nVal = m_pValue->GetNumeric().GetDouble();

			if(0 == nVal)
				return VARIANT_FALSE;

			return VARIANT_TRUE;
		}
		break;
	
	default:
		return VARIANT_FALSE;
	}
	
	return VARIANT_FALSE;
}

//_____________________________________________________________________________
//
BYTE CValueToDBTYPE::AsUI1()
{
	BYTE nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
signed char CValueToDBTYPE::AsI1()
{
	signed char nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
USHORT CValueToDBTYPE::AsUI2()
{
	USHORT nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
ULONG CValueToDBTYPE::AsUI4()
{
	ULONG nRet = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		nRet = m_pValue->GetNumeric().GetDouble();
		break;
	
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
LARGE_INTEGER CValueToDBTYPE::AsI8()
{
	LARGE_INTEGER nRet;
	nRet.QuadPart = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<LARGE_INTEGER> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return nRet;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_I8, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return nRet;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_I8, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(LARGE_INTEGER), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return nRet;

			nRet = *(static_cast<LARGE_INTEGER*>(pDataOut));
		}
		break;
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
ULARGE_INTEGER CValueToDBTYPE::AsUI8()
{
	ULARGE_INTEGER nRet;
	nRet.QuadPart = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<ULARGE_INTEGER> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return nRet;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_UI8, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return nRet;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_UI8, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(ULARGE_INTEGER), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return nRet;

			nRet = *(static_cast<ULARGE_INTEGER*>(pDataOut));
		}
		break;
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
GUID CValueToDBTYPE::AsGUID()
{
	GUID guid;
	::UuidCreateNil(&guid);

	switch(m_nTypeCode)
	{
	case STRING_TYPE_1C:
		{
			const char* szGUID = m_pValue->GetString().operator LPCTSTR();
			
			::UuidFromString((unsigned char*)szGUID, &guid);
		}
		break;

	default:
		return guid;
	}

	return guid;
}

//_____________________________________________________________________________
//
DB_NUMERIC CValueToDBTYPE::AsNUMERIC()
{
	DB_NUMERIC nRet;
	nRet.precision	 = 0;
	nRet.scale		 = 0;
	nRet.sign		 = 0;
	nRet.val[0]		 = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:
		{
			HRESULT hr;
			DWORD dwSrcLength = 0;
			DWORD dwDstLength = 0;
			DWORD dwStatus = 0;
			DWORD dwStatusCnv = 0;

			double DataIn = m_pValue->GetNumeric().GetDouble();

			COLEDBMemPtr<DB_NUMERIC> pDataOut;
			pDataOut.Alloc(1);
			if(!pDataOut)
				return nRet;

			hr = m_ConvHlpr.m_spConvert->GetConversionSize(DBTYPE_R8, 
				DBTYPE_NUMERIC, 
				NULL, 
				&dwDstLength, 
				&DataIn);
			if(FAILED(hr))
				return nRet;

			hr = m_ConvHlpr.m_spConvert->DataConvert(DBTYPE_R8, 
				DBTYPE_NUMERIC, 
				dwSrcLength, 
				&dwDstLength, 
				&DataIn, 
				(PVOID)pDataOut, 
				sizeof(DB_NUMERIC), 
				dwStatus, 
				&dwStatusCnv, 
				0, 
				0, 
				DBDATACONVERT_DEFAULT);
			if(FAILED(hr))
				return nRet;

			nRet = *(static_cast<DB_NUMERIC*>(pDataOut));
		}
		break;
	default:
		return nRet;
	}

	return nRet;
}

//_____________________________________________________________________________
//
DBTIMESTAMP CValueToDBTYPE::AsDBTIMESTAMP()
{
	DBTIMESTAMP dbTStamp;
	dbTStamp.day	 = 30;
	dbTStamp.month	 = 12;
	dbTStamp.year	 = 1899;
	dbTStamp.hour	 = 0;
	dbTStamp.minute	 = 0;
	dbTStamp.second	 = 0;
	dbTStamp.fraction = 0;

	switch(m_nTypeCode)
	{
	case DATE_TYPE_1C:
		{
			CDate dateVal(m_pValue->GetDate());

			dbTStamp.year	 = dateVal.GetYear();
			dbTStamp.month	 = dateVal.GetMonth();
			dbTStamp.day	 = dateVal.GetMonthDay();
		}
		break;

	case STRING_TYPE_1C: // yyyymmdd hh:mm:ss.fff, где fff - 0 - 999999999 (до 9 символов)
		{
			const CString strVal = m_pValue->GetString();
			int nLen = strVal.GetLength();

			if(nLen < 8) // 18991230
				return dbTStamp;
			else if(nLen < 9) // yyyymmdd
			{
				dbTStamp.year	 = ::atoi(strVal.Left(2));
				dbTStamp.month	 = ::atoi(strVal.Mid(3, 2));
				dbTStamp.day	 = ::atoi(strVal.Mid(6, 2));
			}
			else if(nLen == 17) // yyyymmdd hh:mm:ss
			{
				dbTStamp.year	 = ::atoi(strVal.Left(2));
				dbTStamp.month	 = ::atoi(strVal.Mid(3, 2));
				dbTStamp.day	 = ::atoi(strVal.Mid(6, 2));
				dbTStamp.hour	 = ::atoi(strVal.Mid(9, 2));
				dbTStamp.minute	 = ::atoi(strVal.Mid(12, 2));
				dbTStamp.second	 = ::atoi(strVal.Right(2));
			}
			else if((nLen > 18) && (nLen < 28)) // yyyymmdd hh:mm:ss.fff
			{
				dbTStamp.year	 = ::atoi(strVal.Left(2));
				dbTStamp.month	 = ::atoi(strVal.Mid(3, 2));
				dbTStamp.day	 = ::atoi(strVal.Mid(6, 2));
				dbTStamp.hour	 = ::atoi(strVal.Mid(9, 2));
				dbTStamp.minute	 = ::atoi(strVal.Mid(12, 2));
				dbTStamp.second	 = ::atoi(strVal.Right(2));
				dbTStamp.fraction= ::atol(strVal.Right(nLen - 18));
			}
			else
				return dbTStamp;
		}
		break;
	
	default:
		return dbTStamp;
	}

	return dbTStamp;
}

//_____________________________________________________________________________
//
DBTIME CValueToDBTYPE::AsDBTIME()
{
	DBTIME dbTime;
	dbTime.hour		 = 0;
	dbTime.minute	 = 0;
	dbTime.second	 = 0;

	switch(m_nTypeCode)
	{
	case NUMBER_TYPE_1C:// секунды
		{
			int nVal = (int)m_pValue->GetNumeric().GetDouble();
			
			dbTime.hour		 = ::ceil(nVal / 3600);
			dbTime.minute	 = ::ceil((nVal % 3600) / 60);
			dbTime.second	 = (nVal % 3600) % 60;
		}
		break;

	case STRING_TYPE_1C: // ЧЧ:ММ:СС
		{
			const CString strVal = m_pValue->GetString();
			
			if(strVal.GetLength() < 8)
				return dbTime;

			dbTime.hour		 = ::atoi(strVal.Left(2));
			dbTime.minute	 = ::atoi(strVal.Mid(3, 2));
			dbTime.second	 = ::atoi(strVal.Right(2));
		}
		break;

	default:
		return dbTime;
	}

	return dbTime;
}