// Мирошниченко Кирилл aka spock
// #163860836
// e-mail: spock@km.ru
//
#include "stdafx.h"
#include "sp_cooledb.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//_____________________________________________________________________________
//
COLEDBFldsInf::COLEDBFldsInf() :
	m_nType1C(CType(0)),
	m_Index(0),
	m_strName(""),
	m_bIsType1C(FALSE),
	m_eExtType1C(eToOther),
	m_bIsLong(FALSE) {}

extern CDataBase7* pDataBase7;
extern CMetaDataCont* pMetaDataCont;
extern CBkEndUI* pBkEndUI;

extern char szMNPErrorPrefix[];

BEGIN_BL_METH_MAP(COLEDBCommand)
    BL_METH_FUNC_DEF_PARAM("ExecuteStatement",	 "ВыполнитьИнструкцию", 3, funcExecuteStatement, defsExecuteStatement)
	BL_METH_FUNC_DEF_PARAM("Execute",			 "Выполнить", 1, funcExecute, defsExecute)

	BL_METH_PROC_DEF_PARAM("PutObjectList",		 "УложитьСписокОбъектов", 3, procPutObjectList, defsPutObjectList)

	BL_METH_PROC("Close",			 "Закрыть", 0, procClose)
	BL_METH_PROC("Debug",			 "Отладка", 1, procDebug)
	BL_METH_PROC("SetTextParam",	 "УстановитьТекстовыйПараметр", 2, procSetTextParam)
	BL_METH_PROC("SetTempTablesDir", "УстановитьКаталогВремТаблиц", 1, procSetTempTablesDir)
	BL_METH_PROC("SetExecTimeOut",	 "УстановитьТаймаутВыполнения", 1, procSetExecTimeOut)
	BL_METH_PROC("AddParam",		 "ДобПараметр", 4, procAddParam)
	BL_METH_PROC("SetParam",		 "УстановитьПараметр", 2, procSetParam)
	BL_METH_PROC("DeleteParam",		 "УдалитьПараметр", 1, procDeleteParam)
	BL_METH_PROC("DeleteParams",	 "УдалитьПараметры", 0, procDeleteParams)
	BL_METH_PROC("Prepare",			 "Подготовить", 1, procPrepare)
	BL_METH_PROC("Unprepare",		 "ОтменитьПодготовку", 0, procUnprepare)

	BL_METH_FUNC("ParamCnt",		 "КолвоПараметров", 0, funcParamCnt)
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(COLEDBCommand)
END_BL_PROP_MAP()

IMPLEMENT_MY_NONCREATE_CONTEXT(COLEDBCommand, "OLEDBCommand", "OLEDBCommand");

//_____________________________________________________________________________
//
static ULONG AddOffset(ULONG nCurrent, ULONG nAdd)
{
	struct foobar
	{
		char foo;
		long bar;
	};

	ULONG nAlign = offsetof(foobar, bar);

	return nCurrent + nAdd + (nAdd % nAlign);
}

//_____________________________________________________________________________
//
COLEDBCommand::COLEDBCommand(CComPtr<IDBCreateCommand> pIDBCreateCommand) : CMyContextBase(), m_IsDebugMode(FALSE)
{
	HRESULT hr;
	CString strErr;

	m_bCreated	 = TRUE;
	::UuidCreateNil(&m_GUID);
	m_szTempTablesDir.Empty();

	m_ExecTimeout = -1;
	m_bPrepared = FALSE;

	hr = pIDBCreateCommand->CreateCommand(NULL, IID_ICommandText, reinterpret_cast<IUnknown**>(&m_pICommandText));
	if(FAILED(hr))
	{
		strErr = "FAILED! IDBCreateCommand::CreateCommand()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return;
	}

	m_TmpTblsList.RemoveAll();
}

//_____________________________________________________________________________
//
COLEDBCommand::~COLEDBCommand()
{
	RemoveAllTmpTables();

	if(m_pICommandText)
	{
		m_pICommandText.Release();
	}

	m_bCreated = FALSE;
	m_bPrepared = FALSE;

	m_ParamSet.DeleteParams();
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsExecute(int nParam, CValue* param) const
{
	*param = "";
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsExecuteStatement(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0: *param = "";
		break;
	case 1: param->Reset();
		break;
	case 2: *param = CNumeric(1);
	};

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::defsPutObjectList(int nParam, CValue* param) const
{
	if(nParam == 2)
		*param = "";

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcExecuteStatement(CValue& rValue, CValue** ppValue)
{
	CString strErr;
	HRESULT hr;

	CValueTable* pVT	 = NULL;
	CPtrArray* pVL		 = NULL;
	CVTExtended* pVTExt	 = NULL;

	// второй параметр не передан - создаем ТЗ
	//
	if(UNDEFINE_TYPE_1C == ppValue[1]->GetTypeCode())
	{
		rValue.CreateObject("ТаблицаЗначений");
		pVT = CValue2VT(rValue);
	}
	else
	{
		if(!(pVT = CValue2VT(*ppValue[1])))
		{
			//if(!(pVL = CValue2VL(*ppValue[1])))
				//pVTExt = CValue2VTExt(*ppValue[1]);
		}
		rValue = *ppValue[1];
	}

	if(!pVT && !pVL && !pVTExt)
	{
		strErr = "Не удалось получить возвращаемый объект!";
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	if(!m_bCreated)
	{
		strErr = "Использование метода без создания команды!";
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	DBPARAMS* pParams = NULL;
	CString strSQL;

	if(!m_bPrepared)
	{
		strSQL = ppValue[0]->GetString();

		if(!ParseQuery(strSQL))
			return FALSE;
	}
	else
		pParams = m_ParamSet.GetDBPARAMS();

	long nRowsAffected = 0;
	CComPtr<IRowset> pIRowset;

	hr = Execute(strSQL, pParams, &nRowsAffected, reinterpret_cast<IUnknown**>(&pIRowset), FALSE, m_bPrepared);
	if(!hr)
		return FALSE;

	// Если после исполнения запроса получаем пустой rowset, значит дальше нет смысла продолжать
	// возвращаем пустую ТЗ. А вообще это не ошибка. Например, такое может быть, если выполнить
	// этим методом drop table XXX - результатом будет только RowsAffected
	//
	if(pIRowset == NULL)
	{
		// В случае с SQLOLEDB после запроса select * from XXX через переменную
		// nRowsAffected возвращается значение -1.
		// Если же использовать провайдер VFPOLEDB, то после такого же запроса
		// получаем количество выбранных строк.
		//
		return TRUE;
	}

	ULONG cCols					 = 0;
	ULONG cbMaxRowSize			 = 0;
	BOOL bHasKindField			 = FALSE; // флаг - если в выборке есть хоть одно поле вида "ХХХ_вид" или "XXX_kind", то выставляем в значение TRUE

	DBBINDING_ pDBBindings;
	COLEDBFldsInf_ pColumnsInfo;

	if(!Binding(pIRowset, &cCols, pDBBindings, pColumnsInfo, &cbMaxRowSize, bHasKindField))
		return FALSE;

	HACCESSOR hAccessor = DB_NULL_HACCESSOR;
	IAccessor_ pIAccessor;

	if(!CreateAccessor(DBACCESSOR_ROWDATA, 0, pIAccessor, pIRowset, &hAccessor, cCols, pDBBindings))
		return FALSE;

	if(pVT)
	{
		// Нужно очищать ТЗ
		//
		if(ppValue[2]->GetNumeric().operator long())
		{
			pVT->Clear(TRUE);

			// Инфо по колонкам
			//
			CString strFormat;
			for(ULONG nCol = 0; nCol < cCols; nCol++)
			{
				CString szName(pColumnsInfo[nCol].m_strName);
				CType ColType(pColumnsInfo[nCol].m_nType1C);

				// Пока выяснил, что если типизируем поле как Перечисление (без указания вида),
				// то стоит создавать колонку нетипизированную (так делает сама 1с).
				// Про другие типы ничего пока не знаю
				//
				if((ENUM_TYPE_1C == ColType.GetTypeCode()) && (0 == ColType.GetTypeID())
					|| (CALCULATIONKIND_TYPE_1C == ColType.GetTypeCode()))
					pVT->AddColumn(szName, CType(0), szName, 0, strFormat, 0);
				else
					pVT->AddColumn(szName, ColType, szName, 0, strFormat, 0);
			}
		}
		else
		{
			// Выбираем минимальное кол-во колонок в ТЗ - либо из переданной, либо из созданной
			//
			cCols = min(cCols, pVT->GetColumnCount());
		}
	}

	BYTE_ pRowValues;

	pRowValues.Alloc(cbMaxRowSize);
	if(pRowValues == NULL)
	{
		CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов", mmRedErr);
		return FALSE;
	}

	ULONG cRowsObtained = 0;
	HROW m_hRow[NUMROWS_CHUNK];
	HROW* phRow = &m_hRow[0];

	ULONG cRowNum = 0;
	CValue v;
	CMetaDataWork::TypeStringValue eType;

	wchar_t* wstrVal	 = NULL;
	DBDATE* dbdateVal	 = NULL;
	DBTIMESTAMP* dbdtVal = NULL;
	CString strTemp;
	CNumeric strNum;
	ULONG dwStatus;

	ISequentialStream* pISequentialStream = NULL;

	BYTE_ pRBuffer;

	pRBuffer.Alloc(BLOCK_SIZE);
	if(pRBuffer == NULL)
	{
		CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов", mmRedErr);
		return FALSE;
	}

	ULONG cRead = 0;

	for(;;)
	{
		hr = pIRowset->GetNextRows(NULL, 0, NUMROWS_CHUNK, &cRowsObtained, &phRow);
		if(FAILED(hr))
		{
			strErr = "FAILED! IAccessor::GetNextRows()";
			GetErrorDescription(strErr);
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return FALSE;
		}

		if(cRowsObtained == 0)
			break;

		for(ULONG iRow = 0; iRow < cRowsObtained; iRow++)
		{
			hr = pIRowset->GetData(m_hRow[iRow], hAccessor, pRowValues);
			if(FAILED(hr))
			{
				strErr = "FAILED! IRowset::GetData()";
				GetErrorDescription(strErr);
				CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
				return FALSE;
			}

			pVT->NewRow(cRowNum);

			for(ULONG nCol = 0; nCol < cCols; nCol++)
			{
				v.Reset();

				dwStatus = static_cast<ULONG>(pRowValues[pDBBindings[nCol].obStatus]);

				if(pColumnsInfo[nCol].m_bIsLong)
				{
					if(DBSTATUS_S_ISNULL == dwStatus)
					{
						v.SetType(pColumnsInfo[nCol].m_nType1C);
						v.Reset();
						continue;
					}

					pISequentialStream = *(reinterpret_cast<ISequentialStream**>(pRowValues + pDBBindings[nCol].obValue));

					strTemp.Empty();

					do
					{
						// очищаем память от мусора
						//
						memset(pRBuffer, 0, BLOCK_SIZE);

						pISequentialStream->Read(pRBuffer, BLOCK_SIZE, &cRead);

						if(cRead > 0)
							strTemp += reinterpret_cast<char*>((BYTE*)pRBuffer);
					} while(cRead >= BLOCK_SIZE);

					pISequentialStream->Release();

					v.SetType(pColumnsInfo[nCol].m_nType1C);
					v = strTemp;
				}
				else
				{
					CType rt = pColumnsInfo[nCol].m_nType1C;

					switch(pColumnsInfo[nCol].m_wType)
					{
					case DBTYPE_NUMERIC:
					case DBTYPE_I2:
					case DBTYPE_I4:
					case DBTYPE_R4:
					case DBTYPE_R8:
					case DBTYPE_CY:
					case DBTYPE_DECIMAL:
					case DBTYPE_UI1:
					case DBTYPE_I1:
					case DBTYPE_UI2:
					case DBTYPE_UI4:
					case DBTYPE_I8:
					case DBTYPE_UI8:
						{
							// spock: 2005-12-15
							// Про обработку NULL-значений я и забыл :(
							// и про числовые забыл вообще
							// БАГ: если есть значения NULL. то провайдер возвращает предыдущее значение
							// баг с числовыми нашел DmitrO
							// А я растяпа забыл про числовые, когда правил 2005-11-13
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								v.SetType(pColumnsInfo[nCol].m_nType1C);
								v = 0L;
								break;
							}

							v.SetType(pColumnsInfo[nCol].m_nType1C);
							v = strNum.FromString(reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]), NULL);
						}
						break;

					case DBTYPE_BOOL:
						{
							char* strVal = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);

							// spock: 2005-11-13
							// Про обработку NULL-значений я и забыл :(
							// Обрабатываем просто: есть NULL, значит просто прописываем
							// ноль в ТЗ
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								v.SetType(pColumnsInfo[nCol].m_nType1C);
								v = 0L;
								break;
							}

							strTemp.Format("%s", strVal);
							v.SetType(pColumnsInfo[nCol].m_nType1C);

							if(strTemp.CompareNoCase("FALSE") == 0)
								v = 0L;
							else
								v = 1L;
						}
						break;

					case DBTYPE_DBDATE:
						{
							dbdateVal = reinterpret_cast<DBDATE*>(&pRowValues[pDBBindings[nCol].obValue]);

							v.SetType(pColumnsInfo[nCol].m_nType1C);

							// spock: 2005-11-13
							// Про обработку NULL-значений я и забыл :(
							// Обрабатываем просто: есть NULL, значит просто прописываем
							// пустую дату в ТЗ
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
								break;
							}

							if(dbdateVal->year == 1753 &&
								dbdateVal->month == 1 &&
								dbdateVal->day == 1)
							{
								// скваль 2к
								//
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
							}
							else if(dbdateVal->year == 1899 &&
								dbdateVal->month == 12 &&
								dbdateVal->day == 30)
							{
								// фоксовый драйвер/провайдер
								//
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
							}
							else
							{
								CDate dateVal(dbdateVal->year, dbdateVal->month, dbdateVal->day);
								v = dateVal;
							}
						}
						break;

					case DBTYPE_DBTIMESTAMP:
						{
							dbdtVal = reinterpret_cast<DBTIMESTAMP*>(&pRowValues[pDBBindings[nCol].obValue]);

							v.SetType(pColumnsInfo[nCol].m_nType1C);

							// spock: 2005-11-13
							// Про обработку NULL-значений я и забыл :(
							// Обрабатываем просто: есть NULL, значит просто прописываем
							// пустую дату в ТЗ
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
								break;
							}

							if(dbdtVal->year == 1753 &&
								dbdtVal->month == 1 &&
								dbdtVal->day == 1)
							{
								// скваль 2к
								//
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
							}
							else if(dbdtVal->year == 1899 &&
								dbdtVal->month == 12 &&
								dbdtVal->day == 30)
							{
								// фоксовый драйвер
								//
								CDate dateVal(0, 0, 0);
								dateVal.m_DateNum = 0;

								v = dateVal;
							}
							else
							{
								CDate dateVal(dbdtVal->year, dbdtVal->month, dbdtVal->day);
								v = dateVal;
							}
						}
						break;

					case DBTYPE_WSTR:
						{
							wstrVal = reinterpret_cast<wchar_t*>(&pRowValues[pDBBindings[nCol].obValue]);

							// spock: 2005-11-13
							// Про обработку NULL-значений я и забыл :(
							// Обрабатываем просто: есть NULL, значит просто прописываем
							// пустое значение в ТЗ
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								v.SetType(pColumnsInfo[nCol].m_nType1C);
								v = "";
								break;
							}

							char* pTmpStr = NULL;

							try
							{
								pTmpStr = _com_util::ConvertBSTRToString(wstrVal);
							}
							catch(...)
							{
								CBLModule::RaiseExtRuntimeError("Не удалось сконвертировать результат выборки", mmRedErr);
								return FALSE;
							}

							CString strVal(pTmpStr);
							
							if(pTmpStr)
								delete[] pTmpStr;

							v.SetType(pColumnsInfo[nCol].m_nType1C);
							v = strVal;
						}
						break;

					default:
						{
							// spock: 2005-11-13
							// Про обработку NULL-значений я и забыл :(
							// Обрабатываем просто: есть NULL, значит просто прописываем
							// пустое значение в ТЗ
							//
							if(DBSTATUS_S_ISNULL == dwStatus)
							{
								v.Reset();
								break;
							}

							if(TRUE == pColumnsInfo[nCol].m_bIsType1C)
							{
								CString strVal;
								CType LocalType(0);

								if(0 == rt.GetTypeID())
								{
									int nTypeCode = rt.GetTypeCode();

									switch(nTypeCode)
									{
									case ENUM_TYPE_1C:
									case REFERENCE_TYPE_1C:
									case CALENDAR_TYPE_1C:
									case ACCOUNT_TYPE_1C:
										{
											strVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);
											eType	 = CMetaDataWork::LongString;
											LocalType = rt;
										}
										break;

									case CALCULATIONKIND_TYPE_1C:
										{
											strVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);

											int nLen = strVal.GetLength();
											if(13 == nLen)
											{
												CString sLocVal(strVal.Mid(4, 6));
												sLocVal.TrimLeft();
												sLocVal.TrimRight();
												strVal = sLocVal;
											}

											eType = CMetaDataWork::ShortString;

											LocalType = rt;

											v.SetType(rt);
											v.SetObjID(CObjID(strtol(strVal, NULL, 36), CDBSign("   ")));
										}
										break;

									case DOCUMENT_TYPE_1C:
									case UNDEFINE_TYPE_1C:
										{
											ULONG nKindCol = 0;

											ExtTp1C eExtType1C = pColumnsInfo[nCol].m_eExtType1C;

											if(bHasKindField)
												nKindCol = FindKindCol(pColumnsInfo, cCols, pColumnsInfo[nCol].m_strName);

											if(!nKindCol)
											{
												strVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);
												if(UNDEFINE_TYPE_1C == nTypeCode)
													eType= CMetaDataWork::VeryLongString;
												else
													eType= CMetaDataWork::LongString;

												LocalType= rt;
											}
											else
											{
												CString strValLoc	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);
												CString strKindVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nKindCol - 1].obValue]);

												if(eToSb == eExtType1C)
												{
													CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
													if(pBuhDef)
													{
														CString strKindID(strKindVal);
														strKindID.TrimLeft();
														strKindID.TrimRight();

														long nKindID = strtol(strKindID, NULL, 36);

														CSbKindDef* pSbKindDef = pBuhDef->GetSbKindDefs()->GetItem(nKindID);
														if(pSbKindDef)
														{
															LocalType= CType(pSbKindDef->m_TypeCode, pSbKindDef->m_Kind);
															strVal	 = strValLoc;
															eType	 = pSbKindDef->m_Kind ? CMetaDataWork::ShortString : CMetaDataWork::LongString;
														}
													}
												}
												else // eToDoc
												{
													strVal	 = strKindVal + strValLoc;
													eType	 = CMetaDataWork::LongString;
													LocalType= rt;
												}
											}
										}
										break;

									default:
										{
											strVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);
											eType	 = CMetaDataWork::ShortString;

											LocalType = rt;
										}
										break;
									}
								}
								else
								{
									strVal	 = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);
									eType	 = CMetaDataWork::ShortString;

									LocalType = rt;
								}

								if(CALCULATIONKIND_TYPE_1C != rt.GetTypeCode())
									CMetaDataWork::Make1C_ValueFromString(v, LocalType, strVal, eType);
							}
							else
							{
								char* strVal = reinterpret_cast<char*>(&pRowValues[pDBBindings[nCol].obValue]);

								v.SetType(pColumnsInfo[nCol].m_nType1C);
								v = strVal;
							}
							break;
						}
					};
				}
				pVT->SetValue(v, nCol, cRowNum);
			}
			++cRowNum;
		}

		hr = pIRowset->ReleaseRows(cRowsObtained, m_hRow, NULL, NULL, NULL);
		if(FAILED(hr))
		{
			strErr = "FAILED! IAccessor::GetData()";
			GetErrorDescription(strErr);
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return FALSE;
		}
	}

	if(pIAccessor)
		pIAccessor->ReleaseAccessor(hAccessor, NULL);

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcExecute(CValue& rValue, CValue** ppValue)
{
	CString strErr;
	HRESULT hr;

	if(!m_bCreated)
	{
		strErr = "Использование метода без создания команды!";
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	DBPARAMS* pParams = NULL;
	CString strSQL;

	if(!m_bPrepared)
	{
		strSQL = ppValue[0]->GetString();

		if(!ParseQuery(strSQL))
		{
			rValue = 0L;
			return FALSE;
		}
	}
	else
		pParams = m_ParamSet.GetDBPARAMS();

	long nRowsAffected = 0;

	hr = Execute(strSQL, pParams, &nRowsAffected, NULL, FALSE, m_bPrepared);
	if(!hr)
	{
		rValue = 0L;
		return FALSE;
	}

	rValue	 = nRowsAffected;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procPutObjectList(CValue** ppValue)
{
	HRESULT hr = PutObjectsList(ppValue);
	if(!hr)
		return FALSE;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procClose(CValue** ppValue)
{
	RemoveAllTmpTables();

	if(m_pICommandText)
		m_pICommandText.Release();

	m_bCreated = FALSE;
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDebug(CValue** ppValue)
{
	m_IsDebugMode = (0 == ppValue[0]->GetNumeric()) ? FALSE : TRUE;
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetTextParam(CValue** ppValue)
{
	m_MetaNameParser.SetParameter(ppValue[0]->GetString(), *ppValue[1]);
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetTempTablesDir(CValue** ppValue)
{
	CString strErr;

	m_szTempTablesDir.Empty();
	m_szTempTablesDir = ppValue[0]->GetString();

	if(m_szTempTablesDir.IsEmpty())
	{
		strErr.Format("Введена пустая строка!");
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	if(m_szTempTablesDir.Right(1) != "\\")
		m_szTempTablesDir += "\\";

	if(m_szTempTablesDir.Find(' ') > 0)
	{
		strErr.Format("Введена строка с пробелом!");
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetExecTimeOut(CValue** ppValue)
{
	m_ExecTimeout = ppValue[0]->GetNumeric();
	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procAddParam(CValue** ppValue)
{
	CString strErr;

	if(m_bPrepared)
	{
		strErr.Format("Попытка добавить параметр, после того как был подготовлен набор параметров!");
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	DWORD dwParamIO = 0;
	switch(ppValue[0]->GetNumeric())
	{
	case 1: dwParamIO = DBPARAMIO_INPUT;
		break;
	case 2: dwParamIO = DBPARAMIO_OUTPUT;
		break;
	case 3: dwParamIO = DBPARAMIO_INPUT|DBPARAMIO_OUTPUT;
		break;
	default:
		{
			strErr.Format("Неверный параметр (1)!");
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return FALSE;
		}
	}

	WORD nType = ppValue[1]->GetNumeric();
	if(0 == COLEDBParamHlpr::GetTypeSize(nType))
	{
		strErr.Format("Неверный тип параметра (2)");
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
	}

	m_ParamSet.AddParam(dwParamIO, nType, ppValue[2]->GetNumeric(), ppValue[3]->GetNumeric());

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procSetParam(CValue** ppValue)
{
	CString strErr;

	if(!m_ParamSet.SetParamValue(ppValue[0]->GetNumeric(), ppValue[1], strErr))
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDeleteParam(CValue** ppValue)
{
	int nParamNum = ppValue[0]->GetNumeric();

	if(!m_ParamSet.DeleteParam(nParamNum))
	{
		CBLModule::RaiseExtRuntimeError("Неверный номер параметра", mmRedErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procDeleteParams(CValue** ppValue)
{
	m_ParamSet.DeleteParams();

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procPrepare(CValue** ppValue)
{
	CString strErr;

	m_bPrepared = FALSE;

	CString strSQL(ppValue[0]->GetString());

	if(!ParseQuery(strSQL))
		return FALSE;

	if(!Prepare(strSQL, strErr))
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	m_bPrepared = TRUE;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::procUnprepare(CValue** ppValue)
{
	CString strErr;
	HRESULT hr;

	hr = Unprepare(strErr);
	if(!hr)
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	m_bPrepared = FALSE;

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::funcParamCnt(CValue& rValue, CValue** ppValue)
{
	rValue = m_ParamSet.GetCount();
	return TRUE;
}

//_____________________________________________________________________________
//
void COLEDBCommand::GetErrorDescription(CString& strError) const
{
	CComPtr<IErrorInfo> spIErrorInfo;

	try
	{
		if(SUCCEEDED(::GetErrorInfo(0, &spIErrorInfo)))
		{
			BSTR errDescr;
			spIErrorInfo->GetDescription(&errDescr);

			strError += ": ";
			strError += errDescr;

			::SysFreeString(errDescr);
		}
	}
	catch(...)
	{
		strError += ": Undefined critical error!";
	}
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Binding(IRowset_& pIRowset, ULONG* pnCols, DBBINDING_& pDBBindings, COLEDBFldsInf_& pColInfo1C, ULONG* pcMaxRowSize, BOOL& HasKindField) const
{
	HRESULT hr;
	CString strErr;

	CComPtr<IColumnsInfo> spIColumnsInfo;

	CComPtr<IMalloc> spMalloc;
	if(FAILED(CoGetMalloc(MEMCTX_TASK, &spMalloc)))
	{
		CBLModule::RaiseExtRuntimeError("FAILED! IMalloc::CoGetMalloc(): Не удалось выделить память под объект!", mmRedErr);
		return FALSE;
	}

	hr = pIRowset->QueryInterface(IID_IColumnsInfo, reinterpret_cast<void**>(&spIColumnsInfo));
	if(FAILED(hr))
	{
		strErr = "FAILED! IRowset::QueryInterface()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	ULONG nCols = 0;
	BSTR pColumnStrings;
	DBCOLUMNINFO* pColumnsInfo = NULL;

	hr = spIColumnsInfo->GetColumnInfo(&nCols, &pColumnsInfo, &pColumnStrings);
	if(FAILED(hr))
	{
		spMalloc->Free(pColumnsInfo);
		spMalloc->Free(pColumnStrings);

		strErr = "FAILED! IColumnsInfo::GetColumnInfo()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	ULONG nDataOffset = 0;
	ULONG nLengthOffset;
	ULONG nStatusOffset;

	pDBBindings.Alloc(nCols);
	if(pDBBindings == NULL)
	{
		spMalloc->Free(pColumnsInfo);
		spMalloc->Free(pColumnStrings);

		CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов DBBINDING", mmRedErr);
		return FALSE;
	}

	pColInfo1C.Alloc(nCols);
	if(pColInfo1C == NULL)
	{
		spMalloc->Free(pColumnsInfo);
		spMalloc->Free(pColumnStrings);

		CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов COLEDBFldsInf", mmRedErr);
		return FALSE;
	}

	for(ULONG nCol = 0; nCol < nCols; nCol++)
	{
		char* pTmpStr = NULL;

		try
		{
			pTmpStr = _com_util::ConvertBSTRToString(pColumnsInfo[nCol].pwszName);
		}
		catch(...)
		{
			CBLModule::RaiseExtRuntimeError("Не удалось сконвертировать имя колонки", mmRedErr);
			return FALSE;
		}

		CString strName(pTmpStr);

		if(pTmpStr)
			delete[] pTmpStr;

		// заполняем мою структуру
		//
		pColInfo1C[nCol].m_Index	 = nCol;
		pColInfo1C[nCol].m_strName	 = strName;
		pColInfo1C[nCol].m_wType	 = pColumnsInfo[nCol].wType;
		pColInfo1C[nCol].m_nType1C	 = GetType1C(&pColInfo1C[nCol]);

		// Для небольшой оптимизации: чтобы не искать при любом виде запроса колонку с названием "XXX_вид"/"XXX_kind"
		//
		if((pColInfo1C[nCol].m_strName.Find("_kind") >= 0) || (pColInfo1C[nCol].m_strName.Find("_вид") >= 0))
			HasKindField = TRUE;

		//
		// нужно различать BLOB колонки от обычных.
		// MSDN говорит, что "text" and "ntext" (its for mssql) -
		// это DBCOLUMNFLAGS_ISLONG = TRUE or Maximum column size > 4,000 characters
		//

		if(pColumnsInfo[nCol].dwFlags & DBCOLUMNFLAGS_ISLONG)
		{
			pColInfo1C[nCol].m_bIsLong	 = TRUE;

			pDBBindings[nCol].pObject	 = new DBOBJECT;
			DBOBJECT* pLocDBObject		 = pDBBindings[nCol].pObject;

			if(pLocDBObject == NULL)
			{
				spMalloc->Free(pColumnsInfo);
				spMalloc->Free(pColumnStrings);

				CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов DBOBJECT", mmRedErr);
				return FALSE;
			}

			pLocDBObject->dwFlags		 = STGM_READ;
			pLocDBObject->iid			 = IID_ISequentialStream;

			pDBBindings[nCol].wType		 = DBTYPE_IUNKNOWN;
			pDBBindings[nCol].cbMaxLen	 = sizeof(ISequentialStream*);
		}
		else
		{
			pColInfo1C[nCol].m_bIsLong	 = FALSE;

			pDBBindings[nCol].pObject	 = NULL;

			switch(pColumnsInfo[nCol].wType)
			{
				case DBTYPE_STR:
				case DBTYPE_WSTR:
					{
						pDBBindings[nCol].wType		 = pColumnsInfo[nCol].wType;
						pDBBindings[nCol].cbMaxLen	 = pColumnsInfo[nCol].ulColumnSize + sizeof(char);
					}
					break;

				case DBTYPE_DBDATE:
				case DBTYPE_DBTIMESTAMP:
					{
						pDBBindings[nCol].wType		 = pColumnsInfo[nCol].wType;
						pDBBindings[nCol].cbMaxLen	 = pColumnsInfo[nCol].ulColumnSize;
					}
					break;

				default:
					{
						pDBBindings[nCol].wType		 = DBTYPE_STR;
						pDBBindings[nCol].cbMaxLen	 = 100;
					}
					break;
			};
		}

		nLengthOffset	 = AddOffset(nDataOffset, pDBBindings[nCol].cbMaxLen);
		nStatusOffset	 = AddOffset(nLengthOffset, sizeof(ULONG));

		pDBBindings[nCol].iOrdinal	 = pColumnsInfo[nCol].iOrdinal;
		pDBBindings[nCol].obValue	 = nDataOffset;
		pDBBindings[nCol].obLength	 = nLengthOffset;
		pDBBindings[nCol].obStatus	 = nStatusOffset;
		pDBBindings[nCol].pTypeInfo	 = NULL;
		pDBBindings[nCol].pBindExt	 = NULL;
		pDBBindings[nCol].dwPart	 = DBPART_VALUE | DBPART_STATUS;
		pDBBindings[nCol].dwMemOwner = DBMEMOWNER_CLIENTOWNED;
		pDBBindings[nCol].eParamIO	 = DBPARAMIO_NOTPARAM;
		pDBBindings[nCol].dwFlags	 = 0;
		pDBBindings[nCol].bPrecision = pColumnsInfo[nCol].bPrecision;
		pDBBindings[nCol].bScale	 = pColumnsInfo[nCol].bScale;

		nDataOffset = AddOffset(nStatusOffset, sizeof(DBSTATUS));

		int nTypeCode = pColInfo1C[nCol].m_nType1C.GetTypeCode();

		switch(nTypeCode)
		{
			case ENUM_TYPE_1C:
			case REFERENCE_TYPE_1C:
			case CALENDAR_TYPE_1C:
			case CALCULATIONKIND_TYPE_1C:
			case ACCOUNT_TYPE_1C:
			case SUBCONTOKIND_TYPE_1C:
			case CHART_OF_ACC_TYPE_1C:
			case UNDEFINE_TYPE_1C:
			case DOCUMENT_TYPE_1C:
				pColInfo1C[nCol].m_bIsType1C = TRUE;
				break;

			default:
				pColInfo1C[nCol].m_bIsType1C = FALSE;
				break;
		}
	}

	*pnCols			 = nCols;
	*pcMaxRowSize	 = nDataOffset;

	spMalloc->Free(pColumnsInfo);
	spMalloc->Free(pColumnStrings);

	return TRUE;
}

//_____________________________________________________________________________
//
CType COLEDBCommand::GetType1C(COLEDBFldsInf* fInfo) const
{
	if(pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
	{
		int nPos = -1;
		CType rt(0);
		CString strName = fInfo->m_strName;

		nPos = strName.Find(TypeNamePrefix);
		if(nPos >= 0)
		{
			CString Type1C(strName.operator LPCTSTR() + nPos + (sizeof(TypeNamePrefix) - 1));
			Type1C.Replace('_', '.');

			CMetaDataWork::SetTypeAndKind(rt, Type1C);

			strName				 = strName.Left(nPos);
			fInfo->m_strName	 = strName;

			if(!Type1C.CompareNoCase("Субконто") || !Type1C.CompareNoCase("Subconto"))
				fInfo->m_eExtType1C = eToSb;

			else if(!Type1C.CompareNoCase("Документ") || !Type1C.CompareNoCase("Document"))
				fInfo->m_eExtType1C = eToDoc;

			else
				fInfo->m_eExtType1C = eToOther;

			return rt;
		}
	}

	switch(fInfo->m_wType)
	{
		case DBTYPE_STR:
		case DBTYPE_WSTR:
		case DBTYPE_DBTIME:
			return CType(2);
            break;

		case DBTYPE_NUMERIC:
		case DBTYPE_I2:
		case DBTYPE_I4:
		case DBTYPE_R4:
		case DBTYPE_R8:
		case DBTYPE_CY:
		case DBTYPE_DECIMAL:
		case DBTYPE_UI1:
		case DBTYPE_BOOL:
		case DBTYPE_I1:
		case DBTYPE_UI2:
		case DBTYPE_UI4:
		case DBTYPE_I8:
		case DBTYPE_UI8:
			return CType(1); // DmitrO говорит, что тип Число с Длина(0).Точность(0) - нормально
            break;

		case DBTYPE_DBDATE:
		case DBTYPE_DBTIMESTAMP:
			return CType(3);
			break;

		default:
			return CType(0);
			break;
	};
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::ParseQuery(CString& strQuery)
{
	// OUT: [S_OK, S_FALSE]
	//
	CString strErr;

	strQuery.TrimLeft();
	strQuery.TrimRight();

	if(strQuery.IsEmpty())
	{
		strErr = "A SQL statement is empty!";
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

		return FALSE;
	}

	m_MetaNameParser.SetQueryText(strQuery);

	try
	{
		m_MetaNameParser.Parse();
	}
	catch(CMNPException* MNPException)
	{
		strErr = szMNPErrorPrefix + MNPException->GetErrorDescr();
		MNPException->Delete();

		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

		return FALSE;
	}
	catch(...)
	{
		CBLModule::RaiseExtRuntimeError("MetaNameParser critical error!", mmRedErr);

		return FALSE;
	};

	strQuery = m_MetaNameParser.GetQueryText();

	if(m_IsDebugMode)
		pBkEndUI->DoMessageLine(strQuery, mmNone);

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Execute(const CString& strSQL, DBPARAMS* pParams, long* pnRowsAffected, IUnknown** pIRowset, BOOL bQuiet, BOOL bPrepared) const
{
	HRESULT hr;
	CString strErr;

	if(!bPrepared)
	{
		hr = m_pICommandText->SetCommandText(DBGUID_DEFAULT, CComBSTR(strSQL));
		if(FAILED(hr))
		{
			if(bQuiet)
				return FALSE;

			strErr = "FAILED! ICommandText::SetCommandText()";
			GetErrorDescription(strErr);
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return FALSE;
		}
	}

	if(m_ExecTimeout >= 0)
	{
		CComPtr<ICommandProperties> spCommProperties;

		hr = m_pICommandText->QueryInterface(&spCommProperties);
		if(FAILED(hr))
		{
			strErr = "FAILED! ICommandProperties::QueryInterface()";
			GetErrorDescription(strErr);
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
			return FALSE;
		}

		CDBPropSet PropSet(DBPROPSET_ROWSET);
		PropSet.AddProperty(DBPROP_COMMANDTIMEOUT, m_ExecTimeout);

		hr = spCommProperties->SetProperties(1, &PropSet);
		if(FAILED(hr))
		{
			strErr = "FAILED! ICommandProperties::SetProperties(): Не удалось установить property";
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

			return FALSE;
		}
	}

	if(!pIRowset)
		hr = m_pICommandText->Execute(NULL, IID_NULL, pParams, pnRowsAffected, NULL);
	else
		hr = m_pICommandText->Execute(NULL, IID_IRowset, pParams, pnRowsAffected, pIRowset);

	if(FAILED(hr))
	{
		if(bQuiet)
			return FALSE;

		strErr = "FAILED! ICommandText::Execute()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::CreateAccessor(DBACCESSORFLAGS eDBACCFLAGS, ULONG cbRowSize, IAccessor_& pIAccessor, IUnknown* pIUnknown, HACCESSOR* phAccessor, ULONG& cCntItems, DBBINDING_& pDBBindings) const
{
	// OUT: [S_FALSE, S_OK]
	//
	HRESULT hr;
	CString strErr;

	hr = pIUnknown->QueryInterface(IID_IAccessor, reinterpret_cast<void**>(&pIAccessor));
	if(FAILED(hr))
	{
		strErr = "FAILED! IUnknown::QueryInterface()";
		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	DBBINDSTATUS_ pDBBindStatus;

	pDBBindStatus.Alloc(cCntItems);
	if(pDBBindStatus == NULL)
	{
		CBLModule::RaiseExtRuntimeError("Не удалось выделить память под массив объектов DBBINDSTATUS", mmRedErr);
		return FALSE;
	}

	hr = pIAccessor->CreateAccessor(eDBACCFLAGS, cCntItems, pDBBindings, cbRowSize, phAccessor, pDBBindStatus);
	if(FAILED(hr))
	{
		strErr = "FAILED! IAccessor::CreateAccessor()";

		switch(hr)
		{
		case DB_E_BADACCESSORFLAGS:
			strErr += " [One or more accessor flags were invalid]";
			break;

		case DB_E_BYREFACCESSORNOTSUPPORTED:
			strErr += " [Reference accessors are not supported by this provider]";
			break;

		case DB_E_ERRORSOCCURRED:
			strErr += " [Multiple-step OLE DB operation generated errors]";
			break;

		case DB_E_NOTREENTRANT:
			strErr += " [Consumer's event handler called a non-reentrant method in the provider]";
			break;

		case DB_E_NULLACCESSORNOTSUPPORTED:
			strErr += " [Null accessors are not supported by this provider]";
			break;

		default:
			break;
		}

		GetErrorDescription(strErr);
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::PutObjectsList(CValue** ppValue)
{
	CString strErr;
	HRESULT hr;

	// Сразу позаботимся, чтобы параметр был пустым
	//
	CValue* pTableName = ppValue[1];
	pTableName->Reset();

	// Сгенерируем новый GUID
	//
	::UuidCreate(&m_GUID);

	unsigned char* strGUID;
	::UuidToString(&m_GUID, &strGUID);

	// Получим текстовое значение GUID
	//
	CString szTableName = strGUID;
	szTableName = m_szTempTablesDir + szTableName;
	::RpcStringFree(&strGUID);

	CString strSQL;

	// Создаем временную таблицу
	//
	strSQL.Format("CREATE TABLE %s (VAL C(9) NOT NULL, ISFOLDER N(1,0) NOT NULL)", szTableName);

	long nRowsAffected = 0;

	hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
	if(!hr)
	{
		strErr.Format("Не удалось создать временную таблицу!");
		CBLModule::RaiseExtRuntimeError(strErr, 0);

		return FALSE;
	}

	// Заполняем список: временные таблицы
	//
	m_TmpTblsList.AddTail(szTableName);

	CString RefName = ppValue[2]->GetString();

	BOOL ToDeleteFldr = FALSE;

	if(ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
	{
		// Если передан список значений
		//
		CBLContext* pValCont = ppValue[0]->GetContext();

		if(pValCont && !strcmp(pValCont->GetRuntimeClass()->m_lpszClassName, "CValueListContext"))
		{
			CPtrArray* pVL = *(reinterpret_cast<CPtrArray**>((reinterpret_cast<char*>(pValCont)) + 0x30));

			int nlistSize = pVL->GetSize();
			for(int i = 0; i < nlistSize; i++)
			{
				CValue* pValItem = reinterpret_cast<CValue*>(pVL->GetAt(i));

				hr = PutAggregateObjects(pValItem, &szTableName, &RefName);
				if(!hr)
					return FALSE;
			}

			if(nlistSize > 0)
				ToDeleteFldr = TRUE;
		}
		else
		{
			strErr.Format("Недопустимое значение первого аргумента метода!");
			CBLModule::RaiseExtRuntimeError(strErr, 0);
			return FALSE;
		}
	}
	else if(ppValue[0]->GetTypeCode() > DATE_TYPE_1C)
	{
		// Если передан агрегатный объект
		//
		CValue* pAggregate	 = ppValue[0];

		hr = PutAggregateObjects(pAggregate, &szTableName, &RefName);
		if(!hr)
			return FALSE;

		ToDeleteFldr = TRUE;
	}

	if(ToDeleteFldr)
	{
		// Теперь нужно из временной таблицы удалить строки - группы (isfolder=1)
		//
		strSQL = "DELETE FROM %tmptblnm WHERE ISFOLDER = 1";
		strSQL.Replace("%tmptblnm", szTableName);

		nRowsAffected = 0;

		hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
		if(!hr)
		{
			strErr.Format("Не удалось удалить группы из временной таблицы!");
			CBLModule::RaiseExtRuntimeError(strErr, 0);

			return FALSE;
		}

		strSQL = "USE";

		nRowsAffected = 0;

		hr = Execute(strSQL, NULL, &nRowsAffected, NULL);
		if(!hr)
		{
			strErr.Format("Не удалось сменить контекст базы данных!");
			CBLModule::RaiseExtRuntimeError(strErr, 0);

			return FALSE;
		}
	}

	// Все, теперь можно вернуть имя временной таблицы
	//
	*pTableName	 = szTableName.operator LPCTSTR();

	return TRUE;
	}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::PutAggregateObjects(CValue* pValue, const CString* pszTableName, const CString* RefName)
{
	CString strErr, strSQL;

	CSbCntTypeDef* pSbCntTypeDef = NULL;

	if(!RefName->IsEmpty())
	{
		pSbCntTypeDef = pMetaDataCont->GetSTypeDef(*RefName);

		if(!pSbCntTypeDef)
		{
			strErr.Format("Недопустимое значение третьего пераметра метода!");
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

			return FALSE;
		}
	}

	int nLevelsCount = 1;
	int nIsFldr		 = 2;

	if(!pValue->IsEmpty() && !RefName->IsEmpty())
	{
		CValue arValue;
		arValue.Reset();

		pValue->LinkContext(FALSE);
		CBLContext* pValCont = pValue->GetContext();
		pValCont->CallAsFunc(pValCont->FindMethod("IsGroup"), arValue, NULL);
		nIsFldr = 0 == arValue.GetNumeric() ? 2 : 1;

		nLevelsCount = pSbCntTypeDef->GetLevelsLimit();
	}

	CString ObjStr = "'";
	ObjStr += CMetaDataWork::GetObjDBString(*pValue, CMetaDataWork::ShortString);
	ObjStr += "'";

	strSQL.Empty();
	strSQL.Format("INSERT INTO %s (VAL, ISFOLDER) VALUES (%s, %d)", (*pszTableName), ObjStr, nIsFldr);

	long nRowsAffected = 0;

	if(!Execute(strSQL, NULL, &nRowsAffected, NULL))
	{
		strErr.Format("Не удалось добавить корневую строку!");
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

		return FALSE;
	}

	// Если не передали вид справочника, то дальше не выполняем
	//
	if(RefName->IsEmpty() || nLevelsCount < 2)
		return TRUE;

	strSQL.Empty();

	strSQL = "INSERT INTO %tmptblnm (VAL, ISFOLDER)\r\n\
			SELECT SC.ID as VAL, SC.ISFOLDER as ISFOLDER\r\n\
			FROM %sctbl as SC\r\n\
			WHERE\r\n\
			(SC.PARENTID IN (SELECT T.VAL FROM %tmptblnm as T))\r\n\
			AND (SC.ID NOT IN (SELECT TT.VAL FROM %tmptblnm as TT))";

	strSQL.Replace("%tmptblnm", (*pszTableName));

	CString RefTable;
	RefTable.Format("sc%d", pSbCntTypeDef->GetID());
	strSQL.Replace("%sctbl", RefTable);

	if(!Prepare(strSQL, strErr))
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	// Нужно заполнять временную таблицу итеративно, проход - спуск ниже уровнем
	//
	do
	{
		nRowsAffected = 0;

		if(!Execute(strSQL, NULL, &nRowsAffected, NULL, FALSE, TRUE))
		{
			strErr.Format("Не удалось добавить строки!");
			CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);

			return FALSE;
		}
	}while(nRowsAffected > 0);

	if(!Unprepare(strErr))
	{
		CBLModule::RaiseExtRuntimeError(strErr, mmRedErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
void COLEDBCommand::RemoveTmpTable(CString& strTmpTblName)
{
	// Значит уже закрыли команду, вообще такого не должно быть
	// но лучше перебздеть, чем недобздеть
	//
	if(!m_pICommandText)
		return;

	CString strErr;

	CString strSQL("DROP TABLE ");
	strSQL += strTmpTblName;

	long nRowsAffected = 0;

	HRESULT hr;
	hr = Execute(strSQL, NULL, &nRowsAffected, NULL, TRUE); // будет ошибка? это не важно.
}

//_____________________________________________________________________________
//
void COLEDBCommand::RemoveAllTmpTables()
{
	int nCountTmps = m_TmpTblsList.GetCount();
	if(nCountTmps > 0)
	{
		for(int i = 0; i < nCountTmps; ++i)
			RemoveTmpTable(m_TmpTblsList.GetAt(m_TmpTblsList.FindIndex(i)));

		m_TmpTblsList.RemoveAll();
	}
}

//_____________________________________________________________________________
//
ULONG COLEDBCommand::FindKindCol(const COLEDBFldsInf* pColumnsInfo, const ULONG& cMaxCols, const CString& strColName) const
{
	CString tmpNameRus(strColName);
	tmpNameRus += "_вид";

	CString tmpNameEng(strColName);
	tmpNameEng += "_kind";

	for(ULONG cCol = 0; cCol < cMaxCols; cCol++)
	{
		int nRus = tmpNameRus.CompareNoCase(pColumnsInfo[cCol].m_strName);
		int nEng = tmpNameEng.CompareNoCase(pColumnsInfo[cCol].m_strName);

		if((0 == nRus) || (0 == nEng))
			return cCol + 1;
	}

	return 0;
}

//_____________________________________________________________________________
//
LPOLESTR COLEDBCommand::wGetTypeByStr(const WORD DBType) const
{
	switch(DBType)
	{
		case DBTYPE_I1:			 return L"DBTYPE_I1";
		case DBTYPE_I2:			 return L"DBTYPE_I2";
		case DBTYPE_I4:			 return L"DBTYPE_I4";
		case DBTYPE_I8:			 return L"DBTYPE_I8";
		case DBTYPE_UI1:		 return L"DBTYPE_UI1";
		case DBTYPE_UI2:		 return L"DBTYPE_UI2";
		case DBTYPE_UI4:		 return L"DBTYPE_UI4";
		case DBTYPE_UI8:		 return L"DBTYPE_UI8";
		case DBTYPE_R4:			 return L"DBTYPE_R4";
		case DBTYPE_R8:			 return L"DBTYPE_R8";
		case DBTYPE_CY:			 return L"DBTYPE_CY";
		case DBTYPE_DECIMAL:	 return L"DBTYPE_DECIMAL";
		case DBTYPE_NUMERIC:	 return L"DBTYPE_NUMERIC";
		case DBTYPE_BOOL:		 return L"DBTYPE_BOOL";
		case DBTYPE_ERROR:		 return L"DBTYPE_ERROR";
		case DBTYPE_UDT:		 return L"DBTYPE_UDT";
		case DBTYPE_VARIANT:	 return L"DBTYPE_VARIANT";
		case DBTYPE_IDISPATCH:	 return L"DBTYPE_IDISPATCH";
		case DBTYPE_IUNKNOWN:	 return L"DBTYPE_IUNKNOWN";
		case DBTYPE_GUID:		 return L"DBTYPE_GUID";
		case DBTYPE_DATE:		 return L"DBTYPE_DATE";
		case DBTYPE_DBDATE:		 return L"DBTYPE_DBDATE";
		case DBTYPE_DBTIME:		 return L"DBTYPE_DBTIME";
		case DBTYPE_DBTIMESTAMP: return L"DBTYPE_DBTIMESTAMP";
		case DBTYPE_BSTR:		 return L"DBTYPE_BSTR";
		case DBTYPE_WSTR:		 return L"DBTYPE_WCHAR";
		case DBTYPE_BYTES:		 return L"DBTYPE_BINARY";
		case DBTYPE_FILETIME:	 return L"DBTYPE_FILETIME";
		case DBTYPE_VARNUMERIC:	 return L"DBTYPE_VARNUMERIC";
		case DBTYPE_PROPVARIANT: return L"DBTYPE_PROPVARIANT";
		default:				 return L"DBTYPE_CHAR";
	}
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::BindWithParams(CString& strErr, ICommandWithParameters_& pICmdParams)
{
	HRESULT hr;

	ULONG nDataOffset = 0;
	ULONG nLengthOffset = 0;
	ULONG nStatusOffset = 0;

	int nParamCnt = m_ParamSet.GetCount();

	for(int nItem = 0; nItem < nParamCnt; nItem++)
	{
		WORD ParamType		 = 0;
		BYTE ParamLen		 = 0;
		DWORD ParamIO		 = 0;
		BYTE ParamPrec		 = 0;
		ULONG ParamOrdinal	 = 0;
		long nSize			 = 0;
		CValue val;

		if(!m_ParamSet.GetParamProps(nItem, NULL, ParamLen, ParamType, ParamIO, ParamPrec, ParamOrdinal, nSize))
			return FALSE;

		m_ParamSet.m_pParamBindInfo[nItem].pwszDataSourceType	 = wGetTypeByStr(ParamType);
		m_ParamSet.m_pParamBindInfo[nItem].pwszName				 = NULL;
		m_ParamSet.m_pParamBindInfo[nItem].dwFlags				 = ParamIO;
		m_ParamSet.m_pParamBindInfo[nItem].ulParamSize			 = nSize;
		m_ParamSet.m_pParamBindInfo[nItem].bPrecision			 = ParamPrec;
		m_ParamSet.m_pParamBindInfo[nItem].bScale				 = 0;

		m_ParamSet.m_pDBBinding[nItem].wType	 = ParamType;
		m_ParamSet.m_pDBBinding[nItem].cbMaxLen	 = nSize;

		nLengthOffset = nDataOffset + nSize;

		m_ParamSet.m_pDBBinding[nItem].iOrdinal		 = ParamOrdinal;
		m_ParamSet.m_pDBBinding[nItem].obValue		 = nDataOffset;
		m_ParamSet.m_pDBBinding[nItem].obLength		 = nLengthOffset;
		m_ParamSet.m_pDBBinding[nItem].obStatus		 = 0;//nStatusOffset;
		m_ParamSet.m_pDBBinding[nItem].pTypeInfo		 = NULL;
		m_ParamSet.m_pDBBinding[nItem].pObject		 = NULL;
		m_ParamSet.m_pDBBinding[nItem].pBindExt		 = NULL;
		m_ParamSet.m_pDBBinding[nItem].dwPart		 = DBPART_VALUE | DBPART_LENGTH;
		m_ParamSet.m_pDBBinding[nItem].dwMemOwner	 = DBMEMOWNER_CLIENTOWNED;
		m_ParamSet.m_pDBBinding[nItem].dwFlags		 = 0;
		m_ParamSet.m_pDBBinding[nItem].eParamIO		 = ParamIO;
		m_ParamSet.m_pDBBinding[nItem].bPrecision	 = ParamPrec;
		m_ParamSet.m_pDBBinding[nItem].bScale		 = 0;

		nDataOffset = nLengthOffset + sizeof(long);

		m_ParamSet.m_pParamOrdinals[nItem] = ParamOrdinal;
	}

	hr = m_pICommandText->QueryInterface(IID_ICommandWithParameters, reinterpret_cast<void**>(&pICmdParams));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandWithParameters)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	hr = pICmdParams->SetParameterInfo(nParamCnt, m_ParamSet.m_pParamOrdinals, m_ParamSet.m_pParamBindInfo);
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandWithParameters::SetParameterInfo()";

		switch(hr)
		{
			case E_INVALIDARG: strErr += " (INVALIDARG)";
				break;

			case DB_E_BADORDINAL: strErr += " (BADORDINAL)";
				break;

			case DB_E_BADPARAMETERNAME: strErr += " (BADPARAMETERNAME)";
				break;

			case DB_E_BADTYPENAME: strErr += " (BADTYPENAME)";
				break;

			case DB_E_OBJECTOPEN: strErr += " (OBJECTOPEN)";
				break;

			default:
				break;
		}

		GetErrorDescription(strErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Unprepare(CString& strErr)
{
	HRESULT hr;

	m_ParamSet.UnprepareParamsBuf();

	CComPtr<ICommandPrepare> pICommandPrepare;

	hr = m_pICommandText->QueryInterface(IID_ICommandPrepare, reinterpret_cast<void**>(&pICommandPrepare));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandPrepare)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	if(FAILED(pICommandPrepare->Unprepare()))
	{
		strErr = "FAILED! ICommandPrepare::Unprepare(0)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	return TRUE;
}

//_____________________________________________________________________________
//
BOOL COLEDBCommand::Prepare(const CString& strSQL, CString& strErr)
{
	HRESULT hr;

	hr = m_pICommandText->SetCommandText(DBGUID_DBSQL, CComBSTR(strSQL));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::SetCommandText()";
		GetErrorDescription(strErr);
		return FALSE;
	}

	CComPtr<ICommandPrepare> spICommandPrepare;

	hr = m_pICommandText->QueryInterface(IID_ICommandPrepare, reinterpret_cast<void**>(&spICommandPrepare));
	if(FAILED(hr))
	{
		strErr = "FAILED! ICommandText::QueryInterface(ICommandPrepare)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	if(FAILED(spICommandPrepare->Prepare(0)))
	{
		strErr = "FAILED! ICommandPrepare::Prepare(0)";
		GetErrorDescription(strErr);
		return FALSE;
	}

	if(m_ParamSet.PrepareParamsBuf(strErr) > 0)
	{
		if(!BindWithParams(strErr, m_ParamSet.m_pICmdWithParams))
			return FALSE;

		int nParamCnt = m_ParamSet.GetCount();

		if(!CreateAccessor(DBACCESSOR_PARAMETERDATA, 0, m_ParamSet.m_pIAccessor, m_ParamSet.m_pICmdWithParams, &(m_ParamSet.m_hPrmAccessor), reinterpret_cast<ULONG&>(nParamCnt), m_ParamSet.m_pDBBinding))
			return FALSE;
	}

	return TRUE;
}