// MetaDataWork.cpp: implementation of the CMetaDataWork class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
//#include "addin.h"
#include "MetaDataWork.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;
extern CDataBase7 * pDataBase7;
extern CIBSet NativeIBSet;

IMPLEMENT_DYNCREATE(CMetaDataWork, CBLContext);

class CObjID CMetaDataWork::ObjID;
CParamDefs CMetaDataWork::defFnNames;
//CMetaNameParser CMetaDataWork::m_MetaNameParser;
int CMetaDataWork::nmethIsSelected = 0;

void CMetaDataWork::Init()
{
  if (defFnNames.Empty())
  {
    defFnNames.SetPoolSize(lastMethod + 1, 0);
    defFnNames.AddParam("GetConstID", "ИДКонстанты", methGetConstID, 1, 1);
    defFnNames.AddParam("GetRefID","ИДСправочника", methGetRefID, 1, 1);
	  defFnNames.AddParam("GetRefFieldID","ИДРеквизитаСправочника", methGetRefFieldID, 1, 2);
	  defFnNames.AddParam("GetRefTableName","ИмяТаблицыСправочника", methGetRefTableName, 1, 1);
	  defFnNames.AddParam("GetDocID","ИДДокумента", methGetDocID, 1, 1);
	  defFnNames.AddParam("GetDocHeadFieldID","ИДРеквизитаШапки", methGetDocHeadFieldID, 1, 2);
	  defFnNames.AddParam("GetDocHeadTableName","ИмяТаблицыШапки", methGetDocHeadTableName, 1, 1);
	  defFnNames.AddParam("GetDocTblFieldID","ИДРеквизитаТабличнойЧасти", methGetDocTblFieldID, 1, 2);
	  defFnNames.AddParam("GetDocTblTableName","ИмяТаблицыТабличнойЧасти", methGetDocTblTableName, 1, 1);
	  defFnNames.AddParam("GetCJID","ИДЖурналаРасчетов", methGetCJID, 1, 1);
	  defFnNames.AddParam("GetCJFieldID","ИДРеквизитаЖР", methGetCJFieldID, 1, 2);
	  defFnNames.AddParam("GetCJTableName","ИмяТаблицыЖР", methGetCJTableName, 1, 1);
	  defFnNames.AddParam("GetRegID","ИДРегистра", methGetRegID, 1, 1);
	  defFnNames.AddParam("GetRegDimensionID","ИДИзмеренияРегистра", methGetRegDimensionID, 1, 2);
	  defFnNames.AddParam("GetRegResourceID","ИДРесурсаРегистра", methGetRegResourceID, 1, 2);
	  defFnNames.AddParam("GetRegAttributeID","ИДРеквизитаРегистра", methGetRegAttributeID, 1, 2);
	  defFnNames.AddParam("GetRegActsTableName","ИмяТаблицыДвижений", methGetRegActsTableName, 1, 1);
	  defFnNames.AddParam("GetRegTotalsTableName","ИмяТаблицыИтогов", methGetRegTotalsTableName, 1, 1);
	  defFnNames.AddParam("GetMetadataID","ИДОбъекта", methGetMetaDataID, 1, 1);
    defFnNames.AddParam("ValueToDBString","ЗначениеВСтрокуБД", methToDBString, 1, 1);
	  defFnNames.AddParam("ValueToLongDBString","ЗначениеВДлиннуюСтрокуБД", methToDBStringLong, 1, 1);
	  defFnNames.AddParam("ValueFromDBString","ЗначениеИзСтрокиБД", methFromDBString, 1, 3);
	  defFnNames.AddParam("ValueFromLongDBString","ЗначениеИзДлиннойСтрокиБД", methFromDBStringLong, 1, 2);
    defFnNames.AddParam("GetODBCConnectionHandle","GetODBCConnectionHandle", methGetODBCConnectionHandle, 1, 0);
    defFnNames.AddParam("IntToBase","ЧислоВСтроку", methIntToBase, 1, 2);
    defFnNames.AddParam("BaseToInt","СтрокаВЧисло", methBaseToInt, 1, 2);
    defFnNames.AddParam("GetDateTimeIDDOC","ПолучитьДатуВремяИдДок", methGetDateTimeIDDOC, 1, 1);
    defFnNames.AddParam("GetEndOfPeriod","ПолучитьКонПериода", funcGetEndOfPeriod, 1, 1);
    defFnNames.AddParam("GetEndOfPeriodForReg","ПолучитьКонПериодаДляРег", funcGetEndOfPeriodForReg, 1, 2);

    defFnNames.AddParam("GetBegOfPeriod","ПолучитьНачПериода", funcGetBegOfPeriod, 1, 1);
    defFnNames.AddParam("GetBegOfPeriodForReg","ПолучитьНачПериодаДляРег", funcGetBegOfPeriodForReg, 1, 2);
    defFnNames.AddParam("GetStringInVL","ПолучитьСтрокуИзСЗ", funcGetStringInVL, 1, 2);
    defFnNames.AddParam("GetStrFromDate","ПолучитьСтрИзДаты", funcGetStrFromDate, 1, 1);
    defFnNames.AddParam("GetAddPeriod","ПолучитьДобПериод", funcGetAddPeriod, 1, 3);
    defFnNames.AddParam("ProcessMetaSQL","ОбрМетаСКЛ", funcProcessMetaSQL, 1, 1);
    defFnNames.AddParam("SetTextParam","УстановитьТекстовыйПараметр", procSetTextParam, 0, 2);
	  defFnNames.AddParam("ValueToSuperLongDBString","ЗначениеВСамуюДлиннуюСтрокуБД", methToDBStringSuperLong, 1, 1);
	  defFnNames.AddParam("AttachMD","ПрисоединитьМД", procAttachMD, 0, 1);

	  defFnNames.AddParam("GetEnumAttributeID","ИДЗначенияПеречисления", methGetEnumAttributeID, 1, 2);
  }
  m_pMetaDataCont = pMetaDataCont;
}


CMetaDataWork::CMetaDataWork():CBLContext(1)
{
  Init();
}

CMetaDataWork::CMetaDataWork(int param):CBLContext(param)
{
  Init();
}

CMetaDataWork::~CMetaDataWork()
{
    UnloadMD(m_pMetaDataCont);

#ifdef _DEBUG
  pBkEndUI->DoMessageLine("Деактивизация класса CMetaDataWork!", mmExclamation);
#endif

  if (m_pMetadataContext){
	  CMetadataContext* pCont = m_pMetadataContext;
  
	  // сбросим контекст без удаления !
	  pCont->IncrRef();
	  m_pMetadataContext = NULL;

	  // а теперь можно удалять - в итоге все хранимые CValue станут нулевыми и 1С продолжит работу, а не будет падать
	  delete pCont;
	}
}

void CMetaDataWork::UnloadMD(CMetaDataCont* &pMDC)
{
    if (pMDC && pMDC != pMetaDataCont)
        delete pMDC;
    pMDC = pMetaDataCont;
}

/*
seven.dll
0x220E2F08,
0x220E2F18,
0x220E2F28,
0x220E2F38,
0x220E2F48,
0x220E2F58,
0x220E2F68,
0x220E2F78,
0x220E2F88,
0x220E2F98,
0x220E2FA8,
0x220E2FB8,
BE3BC7F4-5119-11D1-96A9004095E1F439
44CD7AF0-5417-11D1-96AC004095E1F439
1D177690-EEE0-11D1-9EA1008048DA120F
0AF60370-754E-11D2-9F21008048DA120F
2B9C6DA0-7888-11D2-9F23008048DA120F
D6FB6880-9FC9-11D2-9F48008048DA120F
9945E8A0-A80A-11D3-B94B008048DA0334
7F338F20-9DD4-11D4-A635004095E0EBD2
2402D05B-C7AC-453B-A2D0B4E5B3F40BF4
BE88D888-BC26-4D5E-A293B46B57F891CF
AF624CD7-C73B-4423-8865658C795F8EB6
084CFF9A-D503-4B64-B22C2D37C66BB89C
*/
_GUID CMetaDataWork::EncryptGUIDArr[12]={
	{0xBE3BC7F4,0x5119,0x11D1,{0x96,0xA9,0x00,0x40,0x95,0xE1,0xF4,0x39}},
	{0x44CD7AF0,0x5417,0x11D1,{0x96,0xAC,0x00,0x40,0x95,0xE1,0xF4,0x39}},
	{0x1D177690,0xEEE0,0x11D1,{0x9E,0xA1,0x00,0x80,0x48,0xDA,0x12,0x0F}},
	{0x0AF60370,0x754E,0x11D2,{0x9F,0x21,0x00,0x80,0x48,0xDA,0x12,0x0F}},
	{0x2B9C6DA0,0x7888,0x11D2,{0x9F,0x23,0x00,0x80,0x48,0xDA,0x12,0x0F}},
	{0xD6FB6880,0x9FC9,0x11D2,{0x9F,0x48,0x00,0x80,0x48,0xDA,0x12,0x0F}},
	{0x9945E8A0,0xA80A,0x11D3,{0xB9,0x4B,0x00,0x80,0x48,0xDA,0x03,0x34}},
	{0x7F338F20,0x9DD4,0x11D4,{0xA6,0x35,0x00,0x40,0x95,0xE0,0xEB,0xD2}},
	{0x2402D05B,0xC7AC,0x453B,{0xA2,0xD0,0xB4,0xE5,0xB3,0xF4,0x0B,0xF4}},
	{0xBE88D888,0xBC26,0x4D5E,{0xA2,0x93,0xB4,0x6B,0x57,0xF8,0x91,0xCF}},
	{0xAF624CD7,0xC73B,0x4423,{0x88,0x65,0x65,0x8C,0x79,0x5F,0x8E,0xB6}},
	{0x084CFF9A,0xD503,0x4B64,{0xB2,0x2C,0x2D,0x37,0xC6,0x6B,0xB8,0x9C}},
};

BOOL CMetaDataWork::LoadMD(CString strFileName, CMetaDataCont* &pMDC)
{
    UnloadMD(pMDC);

    USES_CONVERSION;
    LPSTORAGE pStg = NULL;
    LPSTORAGE pNestedStg = NULL;
	LPSTREAM pStream = NULL;

	BOOL bResult = false;

    HRESULT hr = ::StgOpenStorage(T2COLE(strFileName),
        NULL,
		STGM_READ | STGM_SHARE_DENY_WRITE,
        NULL,
        0,
        &pStg);
	CFileException* fe = NULL;
    if (hr == S_OK)
	{
        hr = pStg->OpenStorage(T2COLE("Metadata"),
            NULL,
            STGM_READ | STGM_SHARE_EXCLUSIVE,
            NULL,
            0,
            &pNestedStg);

		if (hr == S_OK)
		{
			hr = pNestedStg->OpenStream(T2COLE("Main MetaData Stream"),
				0,
				STGM_READ | STGM_SHARE_EXCLUSIVE,
				0,
				&pStream);
		}
		if (hr == S_OK)
		{

			int nEncriptVer = 0;
			BOOL bNeedEncrypt = FALSE;
			STATSTG stat;
			if (pNestedStg->Stat(&stat,STATFLAG_NONAME) == S_OK)
			{
				for (int i=0; i<12; i++)
				{
					if (bNeedEncrypt = EncryptGUIDArr[i] == stat.clsid)
					{
						nEncriptVer = i+1;
						break;
					}
				}
			}

			COleStreamFile OleStreamFile(pStream);
			CArchive ar(&OleStreamFile, CArchive::load);

			CMetaDataStream *pMDS = (CMetaDataStream*)CMetaDataStream::CreateObject();
			pMDC = new CMetaDataCont;
			pMDS->SetWorkMDCont(pMDC);

			CConfigCont* pCC = CConfigCont::GetContainer();
			int nEncriptVerCurrent = pCC->GetEncriptVer();
			BOOL bNeedEncryptCurrent = pCC->IsNeedEncrypt();
			try
			{
				pCC->SetEncriptVer(nEncriptVer);
				pCC->SetNeedEncrypt(bNeedEncrypt);
				pMDS->Serialize(ar);
			}
			catch (CFileException* e)
			{
				fe = e;
			}
			pCC->SetEncriptVer(nEncriptVerCurrent);
			pCC->SetNeedEncrypt(bNeedEncryptCurrent);

			delete pMDS;
			ar.Close();

			pStream->Release();
			pNestedStg->Release();
			bResult=true;
		}
		pStg->Release();
	}
	if (fe)
	{
		CString strErr;
		LPTSTR pErr = strErr.GetBufferSetLength(100);
		fe->GetErrorMessage(pErr,100);
		strErr.ReleaseBuffer();
		RuntimeError(strErr);
	}
	return bResult;
}

int  CMetaDataWork::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
	CBLContext * pBLContext;

	switch (iMethNum)
	{
  case funcProcessMetaSQL:
    {

      if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        m_MetaNameParser.SetQueryText(ppValue[0]->GetString());
        try
        {
          m_MetaNameParser.Parse();
        }
        catch (CMNPException * MNPException)
        {
          CString strLastError = "Meta name parser error: " + MNPException->GetErrorDescr();
          MNPException->Delete();
          CBLModule::RaiseExtRuntimeError(strLastError, 0);
		    };
        rValue = m_MetaNameParser.GetQueryText();
        return 1;
      }
      else
      {
        CBLModule::RaiseExtRuntimeError("Параметром метода должна быть строка!", 0);
      }
    }
    break;
  case funcGetAddPeriod:
    {
      CValue *pPar3 = ppValue[2];
      PeriodType PerType;
      if (pPar3->IsEmpty())
        PerType = GetSysSet()->GetSnapShotPeriod();
      else
      {
        if(pPar3->GetTypeCode() == STRING_TYPE_1C)
        {
          CRegDef *pRegDef = m_pMetaDataCont->GetRegDef(pPar3->GetString());
          if (pRegDef != NULL)
          {
            if (pRegDef->IsTurnovers())
            {
              PerType = pRegDef->GetPeriod();
            }
            else
              return 1;

          }
          else
          {
            CString err;
            err.Format("Не существует регистра с идентификатором: %s", pPar3->GetString());
            CBLModule::RaiseExtRuntimeError(err, 0);
          }
        }
		    else if (pPar3->GetTypeCode() == NUMBER_TYPE_1C)
        {
          CRegDef *pRegDef = m_pMetaDataCont->GetRegDefAt(pPar3->GetNumeric().operator long()-1);
          if (pRegDef != NULL)
          {
            if (pRegDef->IsTurnovers())
            {
              PerType = pRegDef->GetPeriod();
            }
            else
              return 1;
          }
          else
          {
            CString err;
            err.Format("Не существует регистра под номером: %d", pPar3->GetNumeric().operator long());
            CBLModule::RaiseExtRuntimeError(err, 0);
          }
        }
        else
          CBLModule::RaiseExtRuntimeError("Неверный тип третьего параметра", 0);
      }
      long nPar2 = ppValue[1]->GetNumeric().operator long();

      if (PerType == eDay)
        {
          CDate d = ppValue[0]->GetDate();
          rValue = DATEToCDate(CDateToDATE(d) + nPar2);
            }
            else
            {
          CDate d = ppValue[0]->GetDate();
          if (nPar2 < 0)
          {
              long n = -1*nPar2;
              for (int i = 0; i < n; ++i)
              {
                d = GetBegOfPeriod(d, PerType);
                d = DATEToCDate(CDateToDATE(d) - 1);
              }
              d = GetBegOfPeriod(d, PerType);
          }
          else
          {
              for (int i = 0; i < nPar2; ++i)
              {
                d = GetEndOfPeriod(d, PerType);
                d = DATEToCDate(CDateToDATE(d) + 1);
              }
          }
          rValue = d;
        }
    }
    break;
  case funcGetStrFromDate:
    {
      if (ppValue[0]->GetTypeCode() == DATE_TYPE_1C)
      {
        rValue = GetStrFor1C_Date(ppValue[0]->GetDate());
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра, должна быть дата!", 0);
    }
    break;
  case funcGetStringInVL:
    {
      rValue = 0L;
      if((ppValue[0]->GetTypeCode() != AGREGATE_TYPE_1C) && (0!=strcmp(ppValue[0]->GetContext()->GetRuntimeClass()->m_lpszClassName,"CValueListContext")))
      {
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра!", 0);
      }
      if(ppValue[1]->GetTypeCode() != NUMBER_TYPE_1C)
      {
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра!", 0);
      }
      CBLContext* pValListCont = ppValue[0]->GetContext();
      if(pValListCont)
		  {

		// пример работы с объектом "СписокЗначений" как с объектом С++, а не через контекст
        CValueListContextData* pVLCD = ((CValueListContextData*)pValListCont->GetInternalData());
			  PValueItemArrayT *pValItAr = (PValueItemArrayT*)pVLCD->GetValueList();
        int mode = ppValue[1]->GetNumeric().operator long();
        int size = pValItAr->GetSize();
        CString res;
        int type = -1;
        if (size > 0)
        {
          type = ((PValueT)pValItAr->GetAt(0))->GetTypeCode();
        }
        if (type == AGREGATE_TYPE_1C)
          CBLModule::RaiseExtRuntimeError("В списке значений не должно быть значений с агрегатными типами!", 0);
        if (type == UNDEFINE_TYPE_1C)
          CBLModule::RaiseExtRuntimeError("В списке значений не должно быть значений c неопределенными типами!", 0);

        for (int i = 0; i < size; ++i)
        {
          PValueT pVal = (PValueT)pValItAr->GetAt(i);
          if ((type != pVal->GetTypeCode()))
            CBLModule::RaiseExtRuntimeError("Cписок значений обязан содержать в себе однородные типы значений!", 0);

          switch(type)
          {
          case NUMBER_TYPE_1C:
            {
              res += pVal->GetString();
              res += ',';
            }
            break;
          case STRING_TYPE_1C:
            {
              res += "'";
              res += pVal->GetString();
              res += "',";
            }
            break;
          case DATE_TYPE_1C:
            {
              res += "'";
              res += GetStrFor1C_Date(pVal->GetDate());
              res += "',";
            }
            break;
          default:
            {
              res += "'";
              switch(mode)
              {
              case 1:
                {
                  res += GetObjDBString(*pVal, ShortString);
                }
                break;
              case 2:
                {
                  res += GetObjDBString(*pVal, LongString);
                }
                break;
              default:
                {
                  CBLModule::RaiseExtRuntimeError("Неверно указано значение второго параметра!", 0);
                }
              }
              res += "',";
            }
          }

        }
        if (res.GetAt(res.GetLength()-1) == ',')
          res.SetAt(res.GetLength()-1, ' ');
        rValue = res;

      }
    }
    break;
  case funcGetBegOfPeriod:
    {
      if(ppValue[0]->GetTypeCode() == DATE_TYPE_1C)
        rValue = GetBegOfPeriod(ppValue[0]->GetDate(),GetSysSet()->GetSnapShotPeriod());
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
      break;
    }
  case funcGetBegOfPeriodForReg:
    {
      if(ppValue[1]->GetTypeCode() != DATE_TYPE_1C)
        CBLModule::RaiseExtRuntimeError("Во второй параметр метода необходимо передавать дату!", 0);

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CRegDef *pRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pRegDef != NULL)
        {
          if (pRegDef->IsTurnovers())
          {
            rValue = GetBegOfPeriod(ppValue[1]->GetDate(),pRegDef->GetPeriod());
          }
          else
            rValue = 0L;

        }
        else
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CRegDef *pRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pRegDef != NULL)
        {
          if (pRegDef->IsTurnovers())
          {
            rValue = GetBegOfPeriod(ppValue[1]->GetDate(),pRegDef->GetPeriod());
          }
          else
            rValue = 0L;
        }
        else
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

      break;

    }
  case funcGetEndOfPeriod:
    {
      if(ppValue[0]->GetTypeCode() == DATE_TYPE_1C)
        rValue = GetEndOfPeriod(ppValue[0]->GetDate(),GetSysSet()->GetSnapShotPeriod());
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
      break;
    }
  case funcGetEndOfPeriodForReg:
    {
      if(ppValue[1]->GetTypeCode() != DATE_TYPE_1C)
        CBLModule::RaiseExtRuntimeError("Во второй параметр метода необходимо передавать дату!", 0);

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CRegDef *pRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pRegDef != NULL)
        {
          if (pRegDef->IsTurnovers())
          {
            rValue = GetEndOfPeriod(ppValue[1]->GetDate(),pRegDef->GetPeriod());
          }
          else
            rValue = 0L;

        }
        else
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CRegDef *pRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pRegDef != NULL)
        {
          if (pRegDef->IsTurnovers())
          {
            rValue = GetEndOfPeriod(ppValue[1]->GetDate(),pRegDef->GetPeriod());
          }
          else
            rValue = 0L;
        }
        else
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

      break;
    }
  case methGetDateTimeIDDOC:
    {
      CString str = ppValue[0]->GetString();
      if (!str.IsEmpty())
      {
        char result[24];
        result[23] = '\0';
        if (str.GetLength() != 32 || str[0] != '#')
          CBLModule::RaiseExtRuntimeError("В параметр передана строка с неверным форматом позиции!", 0);

        int lastpos = str.Find(" ");
        int nLen = str.GetLength();
        int NumPart = 0;
        int posBegTime = 0;
        int posBegIDDOC = 0;
        while(lastpos != -1)
        {
          ++NumPart;
          if (NumPart == 1) // Date
          {
            char* p = (char*)str.operator LPCTSTR();
            p[lastpos] = '\0';
            strcpy(result, &p[1]);
            p[lastpos] = ' ';
            posBegTime = lastpos+1;

          }
          else if (NumPart == 2) //Time
          {
            char buff[10];
            LPCSTR p = str;

            long res = strtol(&p[posBegTime], NULL, 10);
            //LongToCharID36(res, buff, 6);
            _itoa(res, buff, 36);
            _strupr(buff);
            sprintf(&result[8],"%6s",buff);
            posBegIDDOC = lastpos+1;
          }
          else if (NumPart == 3) //IDDOC
          {
            char buff[10];
            LPCSTR p = str;
            long res = strtol(&p[posBegIDDOC], NULL, 10);
            _itoa(res, buff, 36);
	          _strupr(buff);
            //
	          sprintf(&result[14],"%6s%3s",buff, &p[posBegIDDOC+9]);
            break;
          }
          while(lastpos!=nLen && str[lastpos++]==' ');

          lastpos = str.Find(" ", lastpos+1);
        }
        rValue = result;
      }
      else
        CBLModule::RaiseExtRuntimeError("В параметр передана пустая строка!", 0);
    }
    break;
  case methIntToBase:
    {
      int a,b;
      char buff[200];
      CString cstrbuf;
		  a	= ppValue[0]->GetNumeric();
		  b	= ppValue[1]->GetNumeric();
		  cstrbuf = _itoa( a, buff, b);
		  cstrbuf.MakeUpper();
		  rValue = cstrbuf;
    }
		break;
	case methBaseToInt:
    {
      int b;
      const char * b2 = (ppValue)[0]->GetString().operator LPCTSTR();

		  b	= ppValue[1]->GetNumeric();
		  rValue = strtol( b2,NULL, b);
    }
		break ;
	case methGetConstID:
    {
		  if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObjArray *pMD = m_pMetaDataCont->GetConstDefs();
        CMetaDataObj * pMDO = pMD->GetItem(ppValue[0]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует константы с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetConstDefs()->GetAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pMDO != NULL)
          rValue = pMDO->GetID();
			  else
        {
          CString err;
          err.Format("Не существует константы под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	
	case methGetEnumAttributeID:
		{
			if(ppValue[0]->GetTypeCode() != STRING_TYPE_1C)
				RuntimeError("Неверный тип первого параметра");

			CMetaDataObjArray *pMD = m_pMetaDataCont->GetEnumDefs();
			CMetaDataObj * pMDO = pMD->GetItem(ppValue[0]->GetString());
			if (!pMDO)
				RuntimeError("Не существует перечисления с идентификатором: %s", ppValue[0]->GetString());

			if(ppValue[1]->GetTypeCode() != STRING_TYPE_1C)
				RuntimeError("Неверный тип второго параметра");
			
			int __i = pMDO->GetNChilds();
			for(int __f = 0; __f < __i; __f++)
			{
				CMetaDataObjArray *pMD_ch = pMDO->GetChild(__f,m_pMetaDataCont);
				if(!pMD_ch)
					RuntimeError("У перечисления \"%s\" нет значений", ppValue[0]->GetString());
				
				CMetaDataObj * pMDO_ch = pMD_ch->GetItem(ppValue[1]->GetString());
				if (!pMDO_ch)
					RuntimeError("У перечисления \"%s\" не существует значения с идентификатором: %s", ppValue[0]->GetString(), ppValue[1]->GetString());

				rValue = pMDO_ch->GetID();
			}
		}
		break;

	case methGetRefID:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetSTypeDefs()->GetItem(ppValue[0]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует справочника с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetSTypeDefs()->GetAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует справочника под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetRefFieldID:
    {
      CSbCntTypeDef *pSbCntTypeDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
			  pSbCntTypeDef = m_pMetaDataCont->GetSTypeDef(ppValue[0]->GetString());
        if (pSbCntTypeDef == NULL)
        {
          CString err;
          err.Format("Не существует справочника с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
			  pSbCntTypeDef = m_pMetaDataCont->GetSTypeDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pSbCntTypeDef == NULL)
        {
          CString err;
          err.Format("Не существует справочника под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pSbCntTypeDef->GetParams()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pSbCntTypeDef->GetParams()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetRefTableName:
    {
		  if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CSbCntTypeDef *pSbCntTypeDef = m_pMetaDataCont->GetSTypeDef(ppValue[0]->GetString());
        if (pSbCntTypeDef!=NULL)
			    rValue = pSbCntTypeDef->GetTableName();
        else
        {
          CString err;
          err.Format("Не существует справочника с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CSbCntTypeDef *pSbCntTypeDef = m_pMetaDataCont->GetSTypeDefAt(ppValue[0]->GetNumeric().operator long()-1);
			  if (pSbCntTypeDef!=NULL)
          rValue = pSbCntTypeDef->GetTableName();
        else
        {
          CString err;
          err.Format("Не существует справочника под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetDocID:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetDocDefs()->GetItem(ppValue[0]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует документа с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetDocDefs()->GetAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует документа под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetDocHeadFieldID:
    {
      CDocDef *pDocDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        pDocDef = m_pMetaDataCont->GetDocDef(ppValue[0]->GetString());
        if (pDocDef == NULL)
        {
          CString err;
          err.Format("Не существует документа с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        pDocDef = m_pMetaDataCont->GetDocDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pDocDef == NULL)
        {
          CString err;
          err.Format("Не существует документа под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pDocDef->GetHeads()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pDocDef->GetHeads()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetDocHeadTableName:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CDocDef *pDocDef = m_pMetaDataCont->GetDocDef(ppValue[0]->GetString());
        if (pDocDef != NULL)
			    rValue = pDocDef->GetHeadTableName();
        else
        {
          CString err;
          err.Format("Не существует документа с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CDocDef *pDocDef = m_pMetaDataCont->GetDocDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pDocDef != NULL)
			    rValue = pDocDef->GetHeadTableName();
        else
        {
          CString err;
          err.Format("Не существует документа под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetDocTblFieldID:
    {
      CDocDef *pDocDef = NULL;
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        pDocDef = m_pMetaDataCont->GetDocDef(ppValue[0]->GetString());
        if (pDocDef == NULL)
        {
          CString err;
          err.Format("Не существует документа с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
			{
        pDocDef = m_pMetaDataCont->GetDocDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pDocDef == NULL)
        {
          CString err;
          err.Format("Не существует документа под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

      if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pDocDef->GetTables()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pDocDef->GetTables()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetDocTblTableName:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CDocDef *pDocDef = m_pMetaDataCont->GetDocDef(ppValue[0]->GetString());
        if (pDocDef != NULL)
			    rValue = pDocDef->GetTblTableName();
        else
        {
          CString err;
          err.Format("Не существует документа с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CDocDef *pDocDef = m_pMetaDataCont->GetDocDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pDocDef != NULL)
			    rValue = pDocDef->GetTblTableName();
        else
        {
          CString err;
          err.Format("Не существует документа под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);

    }
	  break;
	case methGetCJID:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetCJDefs()->GetItem(ppValue[0]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует журнала расчетов с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetCJDefs()->GetAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует журнала расчетов под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetCJFieldID:
    {
      CCJDef *pCCJDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
			  pCCJDef = m_pMetaDataCont->GetCJDef(ppValue[0]->GetString());
        if (pCCJDef == NULL)
        {
          CString err;
          err.Format("Не существует журнала расчетов с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
			  pCCJDef = m_pMetaDataCont->GetCJDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCCJDef == NULL)
        {
          CString err;
          err.Format("Не существует журнала расчетов под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCCJDef->GetParams()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCCJDef->GetParams()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetCJTableName:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CCJDef *pCCJDef = m_pMetaDataCont->GetCJDef(ppValue[0]->GetString());
        if (pCCJDef != NULL)
			    rValue = pCCJDef->GetTableName();
        else
        {
          CString err;
          err.Format("Не существует журнала расчетов с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CCJDef *pCCJDef = m_pMetaDataCont->GetCJDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCCJDef != NULL)
			    rValue = pCCJDef->GetTableName();
        else
        {
          CString err;
          err.Format("Не существует журнала расчетов под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetRegID:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetRegDefs()->GetItem(ppValue[0]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = m_pMetaDataCont->GetRegDefs()->GetAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetRegDimensionID:
    {
      CRegDef *pCRegDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetProps()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует изерения: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetProps()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует изерения под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetRegResourceID:
    {
      CRegDef *pCRegDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetFigure()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует ресурса: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetFigure()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует ресурса под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetRegAttributeID:
    {
      CRegDef *pCRegDef = NULL;

      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        pCRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCRegDef == NULL)
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип первого параметра", 0);

		  if(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetFilds()->GetItem(ppValue[1]->GetString());
        if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита: %s", ppValue[1]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }

      }
		  else if (ppValue[1]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CMetaDataObj * pMDO = pCRegDef->GetFilds()->GetAt(ppValue[1]->GetNumeric().operator long()-1);
			  if (pMDO != NULL)
			    rValue = pMDO->GetID();
        else
        {
          CString err;
          err.Format("Не существует реквизита под номером: %d", ppValue[1]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип второго параметра", 0);
    }
		break;
	case methGetRegActsTableName:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CRegDef *pCRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pCRegDef != NULL)
			    rValue = pCRegDef->GetRegActTableName();
        else
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CRegDef *pCRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCRegDef != NULL)
			    rValue = pCRegDef->GetRegActTableName();
        else
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetRegTotalsTableName:
    {
      if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        CRegDef *pCRegDef = m_pMetaDataCont->GetRegDef(ppValue[0]->GetString());
        if (pCRegDef != NULL)
			    rValue = pCRegDef->GetRegTableName();
        else
        {
          CString err;
          err.Format("Не существует регистра с идентификатором: %s", ppValue[0]->GetString());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
		  else if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        CRegDef *pCRegDef = m_pMetaDataCont->GetRegDefAt(ppValue[0]->GetNumeric().operator long()-1);
        if (pCRegDef != NULL)
			    rValue = pCRegDef->GetRegTableName();
        else
        {
          CString err;
          err.Format("Не существует регистра под номером: %d", ppValue[0]->GetNumeric().operator long());
          CBLModule::RaiseExtRuntimeError(err, 0);
        }
      }
      else
        CBLModule::RaiseExtRuntimeError("Неверный тип параметра", 0);
    }
		break;
	case methGetMetaDataID:
	{
		pBLContext		= ppValue[0]->GetContext();
		if (pBLContext == NULL)
			CBLModule::RaiseExtRuntimeError("В метод необходимо передавать объекты типа метаданных", 0);

		CString strClassName(pBLContext->GetRuntimeClass()->m_lpszClassName);
		if (strClassName != "CMetadataContext")
			CBLModule::RaiseExtRuntimeError("В метод необходимо передавать объекты типа метаданных", 0);

		CValue IsSelected;
		if( nmethIsSelected == 0 ) nmethIsSelected = pBLContext->FindMethod("Selected");
		pBLContext->CallAsFunc(nmethIsSelected, IsSelected, NULL);
		if( IsSelected.GetNumeric() == 0L )
			RuntimeError("В метод необходимо передавать непустые объекты типа метаданных");

		rValue = ((CMetaDataObj**)(long(pBLContext)+0x20))[0]->GetID();
	}
		break;
  case methToDBString:
    {
		  rValue = GetObjDBString(*(ppValue)[0], ShortString);
    }
		break;
	case methToDBStringLong:
    {
      rValue = GetObjDBString(*(ppValue)[0], LongString);
    }
		break;
	case methFromDBString:
    {
		// +1.	Первый параметр: Число (Тип, номер типа)
		//		Второй параметр: Число (Вид, внутренний номер объекта Метаданных)
		//		Третий параметр: Строка (идентификатор объекта)
		//
		// +2.	Первый параметр: Число (Тип, номер типа)
		//		Второй параметр: Строка (Вид, идентификатор объекта Метаданных)
		//		Третий параметр: Строка (идентификатор объекта)
		//
		// -3.	Первый параметр: Строка (Тип, строка типа)
		//		Второй параметр: Число (Вид, внутренний номер объекта Метаданных)
		//		Третий параметр: Строка (идентификатор объекта)
		//
		// +4.	Первый параметр: Строка (Тип, строка типа)
		//		Второй параметр: Строка (Вид, идентификатор объекта Метаданных)
		//		Третий параметр: Строка (идентификатор объекта)
		//
		// +5.	Первый параметр: Строка (Тип и Вид, строка типа и вида)
		//		Второй параметр: Строка (идентификатор объекта)
		//
		int nNumErrPar = 0;
		if (NUMBER_TYPE_1C == ppValue[0]->GetTypeCode())
		{
			if (NUMBER_TYPE_1C == ppValue[1]->GetTypeCode()) // #1
			{
				if (ppValue[2]->GetTypeCode() == STRING_TYPE_1C)
				{
					CType t(ppValue[0]->GetNumeric(), ppValue[1]->GetNumeric());
					Make1C_ValueFromString(rValue, t, ppValue[2]->GetString(), ShortString);
				}
				else
					nNumErrPar = 3;
			}
			else if (STRING_TYPE_1C == ppValue[1]->GetTypeCode()) // #2
			{
				CType t(0);
				t.SetTypeCode(ppValue[0]->GetNumeric());
				CString strType = t.GetTypeTitle();
				strType += ".";
				strType += ppValue[1]->GetString();
				CMetaDataWork::SetTypeAndKind(t, strType);
				Make1C_ValueFromString(rValue, t, ppValue[2]->GetString(), ShortString);
			}
			else
				nNumErrPar = 2;
		}
		else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
		{
			if (ppValue[1]->GetTypeCode() == STRING_TYPE_1C) // #5
			{
				CString strType(ppValue[0]->GetString());
				if (strType.CompareNoCase("ЗаписьЖурналаРасчетов") == 0 || strType.CompareNoCase("CalcJournalRecord") == 0)
				{
					CCJDef* pCJDef = pMetaDataCont->GetCJDef(ppValue[1]->GetString());
					if (pCJDef == NULL)
					{
						CBLModule::RaiseExtRuntimeError("Ошибка во втором параметре", 0);
						return 0;
					};

					CCJRecordContext* pCJRecordContext = new CCJRecordContext(pCJDef);
					char Buf[10];
					strncpy(Buf, ppValue[2]->GetString(), 9);

					Buf[9] = 0;
					char ch = Buf[6];

					Buf[6] = 0;
					long id = strtol(Buf, NULL, 36);

					Buf[6] = ch;
					pCJRecordContext->SetObjID(CObjID(id, &Buf[6]));
					rValue.AssignContext(pCJRecordContext);
					pCJRecordContext->DecrRef();
				}
				else if (!ppValue[2]->IsEmpty()) // #4
				{
					CType t(0);
					CString strType = ppValue[0]->GetString();
					strType += ".";
					strType += ppValue[1]->GetString();
					CMetaDataWork::SetTypeAndKind(t, strType);
					Make1C_ValueFromString(rValue, t, ppValue[2]->GetString(), ShortString);
				}
				else
				{
					CType t(0);
					SetTypeAndKind(t, strType);
					Make1C_ValueFromString(rValue, t, ppValue[1]->GetString(), ShortString);
				}
			}
			else
				nNumErrPar = 2;
		}
		else
			nNumErrPar = 1;

		if (nNumErrPar > 0)
		{
			CString err;
			err.Format("Ошибка в типе передаваемого параметра метода (%d)", nNumErrPar);
			CBLModule::RaiseExtRuntimeError(err, 0);
		}
	}
	break;
  case methFromDBStringLong:
    {
      int nNumErrPar = 0;
      CType t(0);
      if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
      {
        if (ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
        {
          t.SetTypeCode((ppValue[0]->GetNumeric()));
        }
        else
          nNumErrPar = 2;
      }
      else if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
      {
        if (ppValue[1]->GetTypeCode() == STRING_TYPE_1C)
        {
          SetTypeAndKind(t, ppValue[0]->GetString());
        }
        else
          nNumErrPar = 2;
      }
      else
        nNumErrPar = 1;

      if (nNumErrPar > 0)
      {
        CString err;
        err.Format("Ошибка в типе передаваемого параметра метода (%d)", nNumErrPar);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }

      Make1C_ValueFromString(rValue, t, ppValue[1]->GetString(), t.GetTypeCode() ? LongString : VeryLongString);
    }
    break;
  case methGetODBCConnectionHandle:
    {
      rValue = *(long*)*(long*)(long(pDataBase7)+0x20);
    }
		break;
  case methToDBStringSuperLong:
    {
      rValue = GetObjDBString(*ppValue[0], VeryLongString);
    }
		break;
	default:
		return -1;
	}
	return 1;
}

int  CMetaDataWork::CallAsProc(int iMethodNum,class CValue * * ppValue)
{
  int res = 0;
  switch (iMethodNum)
  {
  case procSetTextParam:
    {
      m_MetaNameParser.SetParameter(ppValue[0]->GetString(), *ppValue[1]);
      res = 1;
    }
    break;
        case procAttachMD:
            {
                if (ppValue[0]->GetTypeCode() != STRING_TYPE_1C)
					RuntimeErrorRes(4712); //Неверный тип параметра

                if (ppValue[0]->GetString().IsEmpty())
                    UnloadMD(m_pMetaDataCont);
                else if (!LoadMD(ppValue[0]->GetString(), m_pMetaDataCont))
					RuntimeErrorRes(28771); //Ошибка загрузки метаданных.

				if (m_pMetadataContext)
				{
					m_pMetadataContext->m_pTaskDef = m_pMetaDataCont->m_pTaskDef;
				}

                m_MetaNameParser.SetIBProp(m_pMetaDataCont, &NativeIBSet);
            }
            break;
  default:
    res = -1;
  }

	return res;
}

int  CMetaDataWork::FindMethod(char const * lpMethodName)const
{
	return defFnNames.GetIndexByName(lpMethodName);
}

char const *  CMetaDataWork::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CMetaDataWork::GetNMethods(void)const
{
	return defFnNames.Size();
}

int  CMetaDataWork::HasRetVal(int iMethodNum)const
{
	return defFnNames[iMethodNum].HasReturnValue ;
}

int CMetaDataWork::GetNParams(int iMethodNum)const
{
	return defFnNames[iMethodNum].NumberOfParams;
}

int  CMetaDataWork::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{
  if (iMethodNum == methFromDBString)
  {
  	*pDefValue = "";
	  return 1;
  }
  else if ((iMethodNum == funcGetEndOfPeriod)||(iMethodNum == funcGetBegOfPeriod))
  {
    if (iParamNum == 0)
    {
      *pDefValue = GetSysSet()->GetDateTA();
      return 1;
    }
  }
  else if ((iMethodNum == funcGetEndOfPeriodForReg)||(iMethodNum == funcGetBegOfPeriodForReg))
  {
    if (iParamNum == 1)
    {
      *pDefValue = GetSysSet()->GetDateTA();
      return 1;
    }
  }
  else if (iMethodNum == funcGetStringInVL)
  {
    if (iParamNum == 1)
    {
      *pDefValue = 1L;
      return 1;
    }
  }
  else if (iMethodNum == funcGetAddPeriod)
  {
    if (iParamNum == 2)
    {
      pDefValue->Reset();
      return 1;
    }
  }
  else if (iMethodNum == procAttachMD)
  {
    if (iParamNum == 0)
    {
      *pDefValue = "";
      return 1;
    }
  }
	return 0;
}

void  CMetaDataWork::DecrRef(void)
{
	CBLContext::DecrRef();
}


int  CMetaDataWork::FindProp(char const *name)const
{
	if (!stricmp("Метаданные", name))
		return 0;
	if (!stricmp("Metadata", name))
		return 0;
	return -1;
}

char const *  CMetaDataWork::GetCode(void)const
{
	return 0;
}

int  CMetaDataWork::GetDestroyUnRefd(void)const
{
	return CBLContext::GetDestroyUnRefd();
}

void  CMetaDataWork::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CMetaDataWork::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CMetaDataWork::GetInternalData(void)
{
	return NULL;

}



int  CMetaDataWork::GetNProps(void)const
{
	return 1;
}

char const *  CMetaDataWork::GetPropName(int nProp, int nLang)const
{
	if (nProp == 0)
	{
		if (nLang == 0)
			return "Metadata";
		else
			return "Метаданные";
	}
	return NULL;
}

int  CMetaDataWork::GetPropVal(int nProp, class CValue &value)const
{
	if (nProp == 0)
	{
		if (!m_pMetadataContext)
			//m_pMetadataContext.Attach(new CMetadataContext(m_pMetaDataCont->m_pTaskDef));
			m_pMetadataContext = new CMetadataContext(m_pMetaDataCont->m_pTaskDef);
		if (m_pMetadataContext)
			value.AssignContext(m_pMetadataContext);
	}
	return NULL;
}

long  CMetaDataWork::GetTypeID(void)const
{
	return 100;
}

char const *  CMetaDataWork::GetTypeString(void)const
{
	return "MetaDataWork";
}

class CType   CMetaDataWork::GetValueType(void)const
{
	CType tType(100);
	return tType;
}


void  CMetaDataWork::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CMetaDataWork::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CMetaDataWork::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CMetaDataWork::IsExactValue(void)const
{
	return 0;
}

int  CMetaDataWork::IsOleContext(void)const
{
	return 0;
}

int  CMetaDataWork::IsPropReadable(int iPropNum)const
{
	if (iPropNum==0)
	{
		return TRUE;
	}
	return FALSE;
}

int  CMetaDataWork::IsPropWritable(int iPropNum)const
{
	return 0;
}

int  CMetaDataWork::IsSerializable(void)
{
	return 0;
}

int  CMetaDataWork::SaveToString(class CString & csStr)
{
	csStr = "DJK";
	return 1;
}

void  CMetaDataWork::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}

int  CMetaDataWork::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return 0;
}

bool CMetaDataWork::SetTypeAndKind(CType &dest_type, const CString &source)
{
  int state = 0; //now max = 29
  int nType = UNDEFINE_TYPE_1C;
  int nLenght = source.GetLength();

  bool ret = false;
  bool bNotBreak = true;
  int i = 0;
  for(; bNotBreak && i < nLenght; ++i)
  {
    switch(source[i])
    {
      case 'Н':
      case 'н':
      case 'U':
      case 'u':
        {
          if (state == 0)
          {
          nType = UNDEFINE_TYPE_1C;
          bNotBreak = false;
        }
          else if (state == 28)
          {
            nType = SUBCONTOKIND_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'S':
      case 's':
        {
          if (state == 0) state = 28;
        }
        break;
      case 'С': //рус
      case 'с': //рус
        {
          if(state == 0) state = 10;
          else if (state == 27)
          {
            nType = SUBCONTOKIND_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'т': //рус
      case 'Т': //рус
        {
          if(state == 10)
          {
            nType = STRING_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'п': //рус
      case 'П': //рус
        {
          if(state == 10)
          {
            nType = REFERENCE_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 0)
          {
            state = 24;
          }
        }
        break;
      case 'e':
      case 'E':
        {
          if (state == 0)
          {
            nType = ENUM_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 22)
          {
            nType = CALENDAR_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'N':
      case 'n': //lat
      case 'ч':
      case 'Ч':
        {
          if (state == 10)
          {
            nType = ACCOUNT_TYPE_1C;
            bNotBreak = false;
          }
          else
          {
          nType = NUMBER_TYPE_1C;
          bNotBreak = false;
        }
        }
        break;
      case 'Д':
      case 'д':
        {
          if (state == 0) state = 2;
          else if (state == 26) state = 27;
        }
        break;

      case 'D':
      case 'd':
        {
          if (state == 0) state = 2;
        }
        break;
      case 'O': // Lat
      case 'o': // Lat
        {
          if (state == 2)
          {
            nType = DOCUMENT_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'О': // Rus
      case 'о': // Rus
        {
          if (state == 2)
          {
            nType = DOCUMENT_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'a': // Lat
      case 'A': // Lat
        {
          if (state == 2)
          {
            nType = DATE_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 20)
          {
            state = 21;
        }
          else
          {
            state = 23;
          }
        }
        break;
      case 'а': // Rus
      case 'А': // Rus
        {
          if (state == 2)
          {
            nType = DATE_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 29)
          {
            nType = CALENDAR_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'C': //lat
      case 'c': //lat
        {
          if (state == 0)
            state = 20;
          else if (state == 22)
          {
            nType = CALCULATIONKIND_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 23)
          {
            nType = ACCOUNT_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'L': //lat
      case 'l': //lat
        {
          if (state == 21)
          {
              state = 22;
          }
          else if (state == 20)
          {
            nType = AGREGATE_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'К':
      case 'к':
        {
          if (state == 0) state = 29;
        }
        break;
      case 'В': //рус
      case 'в': //рус
        {
          if (state == 0) state = 25;
        }
        break;
      case 'е': //рус
      case 'Е': //рус
        {
          if (state == 24)
          {
            nType = ENUM_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'л':
      case 'Л':
        {
          if (state == 24)
          {
            nType = CHART_OF_ACC_TYPE_1C;
            bNotBreak = false;
          }
          else if (state == 29)
          {
            nType = AGREGATE_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'h':
      case 'H':
        {
          if (state == 20)
          {
            nType = CHART_OF_ACC_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'и':
      case 'И':
        {
          if (state == 25) state = 26;
        }
        break;
      case 'р':
      case 'Р':
        {
          if (state == 27)
        {
          nType = CALCULATIONKIND_TYPE_1C;
          bNotBreak = false;
        }
        }
        break;
      case 't':
      case 'T':
        {
          if (state == 28)
          {
            nType = STRING_TYPE_1C;
            bNotBreak = false;
          }
        }
        break;
      case 'r':
      case 'R':
        {
          nType = REFERENCE_TYPE_1C;
          bNotBreak = false;
        }
        break;
      default:
      {
        bNotBreak = false;
      }
    }
  }

  if (nType != UNDEFINE_TYPE_1C && NUMBER_TYPE_1C != nType && STRING_TYPE_1C != nType && DATE_TYPE_1C != nType && AGREGATE_TYPE_1C != nType)
  {
    CString kind;
    CMetaDataObj * Obj = NULL;

    int nPosOfDot = source.Find('.', i);
    if(nPosOfDot == -1)
    {
      dest_type = CType(nType);
      ret = true;
    }
    else
    {
      kind = source.Mid(nPosOfDot+1);
	    switch (nType)
      {
	      case ENUM_TYPE_1C:
		      Obj = pMetaDataCont->GetEnumDefs()->GetItem(kind);
		      break;
	      case REFERENCE_TYPE_1C:
		      Obj = pMetaDataCont->GetSTypeDefs()->GetItem(kind);
		      break;
	      case DOCUMENT_TYPE_1C:
		      Obj = pMetaDataCont->GetDocDefs()->GetItem(kind);
		      break;
	      case CALENDAR_TYPE_1C:
		      Obj = pMetaDataCont->GetCalendarTypeDefs()->GetItem(kind);
		      break;
	      case CALCULATIONKIND_TYPE_1C:
		      Obj = pMetaDataCont->GetAlgorithmDefs()->GetItem(kind);
		      break;
	      case ACCOUNT_TYPE_1C:
              {
                  CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
                  if (pBuhDef)
        		      Obj = pBuhDef->GetPlanDef(kind);
              }
		      break;
	      default:
		      break;
	    }
	    if(Obj!=NULL)
      {
		    dest_type = CType(nType,Obj->GetID());
        ret = true;
	    }
    }
  }
  else
  {
    dest_type = CType(nType);
    ret = true;
  }
  return ret;
}

void CMetaDataWork::Make1C_ValueFromString(CValue &res, const CType& type, const CString &source, TypeStringValue eType)
{
	switch(eType)
	{
		case ShortString:
			{
				// Lenght = 9
				res.SetType(type);

				if (6 == source.GetLength())
				{
					// значит передали идентификатор, но без кода УРИБ - добиваем последние символы пробелами
					//
					char* s = const_cast<char *>(source.operator LPCTSTR());

					long id = strtol(s, NULL, 36);
					const char* szURIB = "   ";

					res.SetObjID(CObjID(id, CDBSign(szURIB)));
				}
				else if (source.GetLength() < 9)
					res.SetObjID(CObjID());
				else
				{
					char *s = const_cast<char *>(source.operator LPCTSTR());
					const char ch6 = s[6];

					s[6] = '\0';
					long id = strtol(s,NULL,36);
					s[6] = ch6;

					res.SetObjID(CObjID(id,CDBSign(&s[6])));
				}
			}
			break;

		case LongString:
			{
				if (10 == source.GetLength())
				{
					// значит передали идентификатор, но без кода УРИБ - добиваем последние символы пробелами
					//
					char *s = const_cast<char *>(source.operator LPCTSTR());
					char ch = s[4];

					s[4] = '\0';
					long a = strtol(s, NULL, 36);
					s[4] = ch;

					CType copy(type);
					copy.SetTypeID(a);
					res.SetType(copy);

					long id = strtol(&s[4], NULL, 36);
					const char* szURIB = "   ";

					res.SetObjID(CObjID(id, CDBSign(szURIB)));
				}
				else if (source.GetLength() < 13)
					res.SetObjID(CObjID());
				else
				{
					char *s = const_cast<char *>(source.operator LPCTSTR());
					char ch = s[4];
					s[4] = '\0';
					long a = strtol(s,NULL,36);
					s[4] = ch;

					CType copy(type);
					copy.SetTypeID(a);
					res.SetType(copy);

					ch = s[10];
					s[10] = '\0';
					long id = strtol(&s[4],NULL,36);
					s[10] = ch;

					res.SetObjID(CObjID(id,CDBSign(&s[10])));
				}
			}
			break;

		case VeryLongString:
			{
				char *s = const_cast<char *>(source.operator LPCTSTR());
				int C2TypeCode = CType::C2TypeCode(s[0]);
				char ch;

				if (C2TypeCode < 10)
				{
					if (C2TypeCode == 1)
					{
						if (source.GetLength() < 2)
							res = 0L;
						else
						{
							CNumeric Num;
							Num.FromString(s + 1, NULL);
							res = Num;
						}
					}
					else if (C2TypeCode == 2)
					{
						if (source.GetLength() < 2)
							res = "";
						else
						{
							res = s + 1;
						}
					}
					else
					{
						if (source.GetLength() < 9)
							res = CDate(0,0,0);
						else
						{
							ch = s[1 + 4];
							s[1 + 4] = 0;
							int y = atoi(s + 1);
							s[1 + 4] = ch;
							ch = s[1 + 4 + 2];
							s[1 + 4 + 2] = 0;
							int m = atoi(s + 1 + 4);
							s[1 + 4 + 2] = ch;

							ch = s[1 + 4 + 2 + 2];
							s[1 + 4 + 2 + 2] = 0;
							int d = atoi(s + 1 + 4 + 2);
							s[1 + 4 + 2 + 2] = ch;

							CDate dat(y, m, d);
							res = dat;
						}
					};
				}
				else
				{
					if (source.GetLength() < 15)
						res.SetObjID(CObjID());
					else
					{
						ch = s[6];
						s[6] = '\0';
						long a = strtol(&s[2],NULL,36);
						s[6] = ch;

						CType copy(C2TypeCode, a);
						res.SetType(copy);

						ch = s[12];
						s[12] = '\0';
						long id = strtol(&s[6],NULL,36);
						s[12] = ch;

						res.SetObjID(CObjID(id,CDBSign(&s[12])));
					}
				}
			}
			break;
	}
}

/*CString CMetaDataWork::ToDBString(const CValue &val)
{
  int ObjID = val.GetObjID().GetlObjID();
  char buff[200];
  _itoa(ObjID, buff, 36);
	_strupr(buff);
  CString res;
  res.Format("%6s%3s", buff, val.GetObjID().DBSign.Sign);
  return res;
}

CString CMetaDataWork::ToDBStringLong(const CValue &val)
{
  int ObjID = val.GetObjID().GetlObjID();
  char buff[200];
  char buff2[200];
	char buff3[200];

	_itoa(ObjID, buff, 36);
	_strupr(buff);

	sprintf(buff2,"%6s%3s",buff,val.GetObjID().DBSign.Sign);
	int RealTypeID = val.GetRealTypeID();
	_itoa( RealTypeID, buff3, 36);
	_strupr(buff3);
  CString res;
  res.Format("%4s%9s",buff3,buff2);
	return res;
}
*/
CString CMetaDataWork::GetStrFor1C_Date(const CDate &date)
{
  COleDateTime d(date.GetYear(), date.GetMonth(), date.GetMonthDay(), 0, 0, 0);
  return d.Format("%Y%m%d");
}

CString CMetaDataWork::GetObjDBString(const CValue & Value, const TypeStringValue enTypeStr)
{
    CString DBString;
    int TypeCode = Value.GetTypeCode();
    if (TypeCode > CHART_OF_ACC_TYPE_1C)
        return DBString;

    int len;
    if (enTypeStr == ShortString)
        len = 9;
    else if (enTypeStr == LongString)
        len = 13;
    else
        len = 23;

    char* pCurStr = DBString.GetBufferSetLength(len);

    if (TypeCode < ENUM_TYPE_1C)
    {
        if (enTypeStr == VeryLongString)
        {
           switch (TypeCode)
            {
            case UNDEFINE_TYPE_1C:
                *pCurStr++ = 'U';
                memset(pCurStr, ' ', 22);
              break;
            case NUMBER_TYPE_1C:
                sprintf(pCurStr, "N%22s", Value.GetString());
              break;
            case STRING_TYPE_1C:
                sprintf(pCurStr, "S%-22.22s", Value.GetString());
              break;
            case DATE_TYPE_1C:
              {
                CDate dat = Value.GetDate();
                sprintf(pCurStr, "D%04u%02u%02u              ", dat.GetYear(), dat.GetMonth(), dat.GetMonthDay());
              };
              break;
            };
        }
        else
            memset(pCurStr, ' ', len);
    }
    else
    {
        if (enTypeStr == VeryLongString)
        {
            *pCurStr++ = "EBOCATKP"[TypeCode - 10]; //Интересное слово получилось ;)
            *pCurStr++ = '1';
            memset(pCurStr + 13, ' ', 8);
        };
	    if (enTypeStr == LongString || enTypeStr == VeryLongString)
        {
            if (enTypeStr == LongString && (TypeCode == CALENDAR_TYPE_1C || TypeCode == CALCULATIONKIND_TYPE_1C))
                *reinterpret_cast<DWORD*>(pCurStr) = '0   ';
            else
		        LongToCharID36(Value.GetRealTypeID(), pCurStr, 4);
		    pCurStr += 4;
        };

	    LongToCharID36(Value.GetObjID().GetlObjID(), pCurStr, 6);
        pCurStr += 6;

        char* pSign = Value.GetObjID().DBSign.Sign;
        *pCurStr++ = *pSign++;
        *pCurStr++ = *pSign++;
        *pCurStr = *pSign;
    };
    return DBString;
};

DWORD CMetadataContext::m_VFTableAddress = NULL;
