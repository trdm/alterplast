// sp_oledbparamhlpr.cpp: implementation of the COLEDBParamHlpr class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "sp_oledbparamhlpr.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

//_____________________________________________________________________________
//
COLEDBParamHlpr::COLEDBParamHlpr()
{
	m_nFullSize = 0;
}

//_____________________________________________________________________________
//
COLEDBParamHlpr::~COLEDBParamHlpr()
{
	DeleteParams();
}

//_____________________________________________________________________________
//
int COLEDBParamHlpr::GetCount()
{
	return m_ParamList.size();
}

//_____________________________________________________________________________
//
void COLEDBParamHlpr::AddParam(DWORD dwParamIO, WORD nType, BYTE m_Length, BYTE m_Precision)
{
	CString strErr;
	
	int nSize = GetCount();
	
	COLEDBParam Param;
	
	Param.m_Ordinal		= nSize + 1;
	Param.m_ParamIO		= dwParamIO;
	Param.m_Type		= nType;
	Param.m_Length		= m_Length;
	Param.m_Precision	= m_Precision;

	long nTypeSize = GetTypeSize(nType);

	switch(nType)
	{
	case DBTYPE_STR:
	case DBTYPE_WSTR:
		{
			m_nFullSize += m_Length * nTypeSize;
			m_nFullSize += nTypeSize; // под конец строки

			Param.m_MemSize = m_Length * nTypeSize;
			Param.m_MemSize += nTypeSize;
		}
		break;
		
	default:
		{
			m_nFullSize += nTypeSize;

			Param.m_MemSize = nTypeSize;
		}
		break;
	};
	
	m_nFullSize += sizeof(long); // под размер параметра (sizeof(long) = 4)

	m_ParamList.push_back (Param);
}

//_____________________________________________________________________________
//
void COLEDBParamHlpr::DeleteParams()
{
	m_ParamList.clear ();
}

//_____________________________________________________________________________
//
BOOL COLEDBParamHlpr::DeleteParam(int nParamNum)
{
	int nCnt = GetCount();
	
	if((nCnt > 0) && (nCnt >= nParamNum))
	{
		m_ParamList.erase (m_ParamList.begin () + (nParamNum - 1));
	}
	else
		return FALSE;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBParamHlpr::SetParamValue(int nOrdinal, CValue* pValue, CString& strError)
{
	int nSize = GetCount();

	if(!nSize)
	{
		strError.Format("—писок параметров пуст!");
		return FALSE;
	}

	if((nOrdinal > nSize) || (0 == nOrdinal))
	{
		strError.Format("Ќедопустимый индекс параметра!");
		return FALSE;
	}

	m_ParamList [nOrdinal - 1].m_Value = *pValue;

	return TRUE;

}

//_____________________________________________________________________________
//
CValue& COLEDBParamHlpr::GetParamValue(int nIndex)
{
	return m_ParamList [nIndex].m_Value;
}

//_____________________________________________________________________________
//
BYTE COLEDBParamHlpr::GetParamLength(int nIndex)
{
	return m_ParamList [nIndex].m_Length;
}

//_____________________________________________________________________________
//
BYTE COLEDBParamHlpr::GetParamType(int nIndex)
{
	return m_ParamList [nIndex].m_Type;
}

//_____________________________________________________________________________
//
DWORD COLEDBParamHlpr::GetParamIO(int nIndex)
{
	return m_ParamList [nIndex].m_ParamIO;
}

//_____________________________________________________________________________
//
BYTE COLEDBParamHlpr::GetParamPrecision(int nIndex)
{
	return m_ParamList [nIndex].m_Precision;
}

//_____________________________________________________________________________
//
ULONG COLEDBParamHlpr::GetParamOrdinal(int nIndex)
{
	return m_ParamList [nIndex].m_Ordinal;
}

//_____________________________________________________________________________
//
int COLEDBParamHlpr::GetFullSize()
{
	return m_nFullSize;
}

//_____________________________________________________________________________
//
long COLEDBParamHlpr::GetTypeSize(WORD nType)
{
	long nSize;
	
	switch(nType)
	{
		case DBTYPE_I2: nSize = sizeof(SHORT);
			break;
		
		case DBTYPE_I4: nSize = sizeof(LONG);
			break;
		
		case DBTYPE_R4: nSize = sizeof(FLOAT);
			break;
		
		case DBTYPE_R8: nSize = sizeof(DOUBLE);
			break;
		
		case DBTYPE_CY: nSize = sizeof(LARGE_INTEGER);
			break;
		
		case DBTYPE_DATE: nSize = sizeof(DATE);
			break;
		
		case DBTYPE_BOOL: nSize = sizeof(VARIANT_BOOL);
			break;
		
		case DBTYPE_DECIMAL: nSize = sizeof(DECIMAL);
			break;
		
		case DBTYPE_UI1: nSize = sizeof(BYTE);
			break;
		
		case DBTYPE_I1: nSize = sizeof(signed char);
			break;
		
		case DBTYPE_UI2: nSize = sizeof(USHORT);
			break;
		
		case DBTYPE_UI4: nSize = sizeof(ULONG);
			break;
		
		case DBTYPE_I8: nSize = sizeof(LARGE_INTEGER);
			break;
		
		case DBTYPE_UI8: nSize = sizeof(ULARGE_INTEGER);
			break;
		
		case DBTYPE_GUID: nSize = sizeof(GUID);
			break;
		
		case DBTYPE_STR: nSize = sizeof(char);
			break;

		case DBTYPE_DBTIME: nSize = sizeof(DBTIME);
			break;
			
		/*case DBTYPE_WSTR: nSize = sizeof(wchar_t);
			break;*/
		
		case DBTYPE_NUMERIC: nSize = sizeof(DB_NUMERIC);
			break;

		case DBTYPE_DBTIMESTAMP: nSize = sizeof(DBTIMESTAMP);
			break;

		case DBTYPE_DBDATE: nSize = sizeof(DBDATE);
			break;
		
		default: nSize = 0;
			break;
	}

	return nSize;
}

//_____________________________________________________________________________
//
long COLEDBParamHlpr::GetParamSize(int nIndex)
{
	return m_ParamList [nIndex].m_MemSize;
}

//_____________________________________________________________________________
//
int COLEDBParamHlpr::PrepareParamsBuf(CString& strError)
{
	int nParamCnt = GetCount();

	if(nParamCnt > 0)
	{
		m_ParamBindInfo.resize (nParamCnt);
		m_DBBinding.resize (nParamCnt);
		m_ParamOrdinals.resize (nParamCnt);

		long nSize = GetFullSize();
		
		m_ParamsBuffer.resize (nSize);

		return 1;
	}

	return 0;
}

//_____________________________________________________________________________
//
DBPARAMS* COLEDBParamHlpr::GetDBPARAMS()
{
	if(0 == GetCount())
		return NULL;

	if(!FillParamsBuf())
		return NULL;

	m_Params.pData		 = &m_ParamsBuffer [0];
	m_Params.cParamSets	 = 1;
	m_Params.hAccessor	 = m_hPrmAccessor;
	
	return &m_Params;

}

//_____________________________________________________________________________
//
BOOL COLEDBParamHlpr::FillParamsBuf()
{
	long offset = 0;
	for(int nItem = 0; nItem < GetCount(); nItem++)
	{
		CValue& Val = GetParamValue(nItem);
		BYTE Len = GetParamLength(nItem);
		BYTE Type = GetParamType(nItem);

		long nParamLen = 0;
		int nCode = Val.GetTypeCode();

		CValueToDBTYPE ValTo(&Val, nCode);
		
		switch(Type)
		{
			case DBTYPE_STR:
				{
					nParamLen = Len;
					CString strVal = ValTo.AsSTR(nParamLen);

					LPCTSTR szVal = strVal.operator LPCTSTR();
					memcpy(&m_ParamsBuffer[0] + offset, szVal, nParamLen * COLEDBParamHlpr::GetTypeSize(Type));

					m_ParamsBuffer[offset + nParamLen] = '\0';
					offset += (nParamLen + 1); // + \0

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_DECIMAL:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					DECIMAL Dec = ValTo.AsDECIMAL();

					memcpy(&m_ParamsBuffer[0] + offset, &Dec, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;
			
			case DBTYPE_DBDATE:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					DBDATE dbDate = ValTo.AsDBDATE();

					memcpy(&m_ParamsBuffer[0] + offset, &dbDate, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_I2:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					SHORT nVal = ValTo.AsI2();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_I4:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					LONG nVal = ValTo.AsI4();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_R4:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					FLOAT nVal = ValTo.AsR4();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_R8:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					DOUBLE nVal = ValTo.AsR8();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_CY:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					LARGE_INTEGER nVal = ValTo.AsCY();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;
			
			case DBTYPE_DATE:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;
					
					DATE nVal = ValTo.AsDATE();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;
					
					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_BOOL:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					VARIANT_BOOL nVal = ValTo.AsBOOL();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_UI1:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					BYTE nVal = ValTo.AsUI1();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_I1:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					signed char nVal = ValTo.AsI1();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_UI2:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					USHORT nVal = ValTo.AsUI2();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_UI4:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					ULONG nVal = ValTo.AsUI4();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_I8:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					LARGE_INTEGER nVal = ValTo.AsI8();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_UI8:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					ULARGE_INTEGER nVal = ValTo.AsUI8();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_GUID:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					GUID nVal = ValTo.AsGUID();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			/*case DBTYPE_WSTR:
				{
				}
				break;*/

			case DBTYPE_NUMERIC:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					DB_NUMERIC nVal = ValTo.AsNUMERIC();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_DBTIMESTAMP:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					DBTIMESTAMP nVal = ValTo.AsDBTIMESTAMP();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;

			case DBTYPE_DBTIME:
				{
					nParamLen = COLEDBParamHlpr::GetTypeSize(Type);
					if(0 == nParamLen)
						return false;

					DBTIME nVal = ValTo.AsDBTIME();

					memcpy(&m_ParamsBuffer[0] + offset, &nVal, nParamLen);
					offset += nParamLen;

					memcpy(&m_ParamsBuffer[0] + offset, &nParamLen, sizeof(long));
					offset += sizeof(long);
				}
				break;
			
			default:
				return false;
		}
	}

	return true;

}

//_____________________________________________________________________________
//
void COLEDBParamHlpr::UnprepareParamsBuf()
{
	m_ParamsBuffer.clear ();
	m_ParamBindInfo.clear ();
	m_DBBinding.clear ();
	m_ParamOrdinals.clear ();
	m_pICmdWithParams.Release ();
	m_pIAccessor.Release ();

	m_hPrmAccessor = NULL;
	m_nFullSize = 0;

	DeleteParams();

}

//_____________________________________________________________________________
//
BOOL COLEDBParamHlpr::GetParamProps(int nIndex, CValue* pVal, BYTE& pLength, WORD& pType, DWORD& pIO, BYTE& pPrec, ULONG& pOrdinal, long& pSize)
{
	COLEDBParam* pOLEDBParam = NULL;

	pOLEDBParam = &m_ParamList [nIndex];

	pVal	 = &(pOLEDBParam->m_Value);
	pLength	 = (pOLEDBParam->m_Length);
	pType	 = (pOLEDBParam->m_Type);
	pIO		 = (pOLEDBParam->m_ParamIO);
	pPrec	 = (pOLEDBParam->m_Precision);
	pOrdinal = (pOLEDBParam->m_Ordinal);
	pSize	 = (pOLEDBParam->m_MemSize);

	return TRUE;
}