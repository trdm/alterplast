// 1CPP_ODBCRecordSet.cpp: implementation of the C1CPP_ODBCRecordSet class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "1CPP_ODBCRecordSet.h"
#include <math.h>
#include "..\\BinaryData.h"
#include "..\\GUID.h"
#include "..\\VTExt\\VTExtended.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

enum {    
  procSetDatabase,
  funcExecute,
  funcGetLastError,
  procClose,
  funcBuildParams,
  funcPrepare,
  funcAddParam,
  funcGetParam,
  procSetParam,
  funcParamCount,
  procClearParams,
  procDeleteParam,
  procSetDatabase1C,
  funcOpen,
  funcIsOpen,
  funcMoveNext,
  funcMovePrevious,
  funcMoveFirst,
  funcMoveLast,
  funcGetFieldCount,
  funcIsBof,
  funcIsEof,
  funcGetRowsAffected,
  funcGetValues,
  funcGetInTablesValues,
  procDebug,
  funcGetFieldIndex,
  funcGetValue,
  procSetRowsetSize,
  procSetPropertyCursor,
  procSetTypeColumn1C,
  funcGetValuesInSV,
  funcExeSQL_FromTV,
  procSetTextParam,
  funcGetFieldDefs,
  procAutoTypification,
  procPutObjectList,
  procSetQueryTimeout,
  funcIsQueryTimeoutExpired,
  procBackFromTAEnabled,
  funcExecuteStatement,
  funcExecuteScalar,
  funcExecuteXML,
  procRPCMode,
  procPutObjectList13,
	lastMethod
	};

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

class CObjID C1CPP_ODBCRecordSet::ObjID;
class CParamDefs C1CPP_ODBCRecordSet::defFnNames;
class CMetaDataWork* C1CPP_ODBCRecordSet::m_pMetaData = NULL;

//class CODBCDatabase C1CPP_ODBCRecordSet::m_db;
int C1CPP_ODBCRecordSet::n_nCountUnlimitPars(0);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(C1CPP_ODBCRecordSet, CBLContext);

char szMNPErrorPrefix[] = "Meta name parser error: ";
char szODBCDatetimeCanon[] = "%04d-%02d-%02d %02d:%02d:%02d.%03d";
char szODBCTimeCanon[] = "%02hu:%02hu:%02hu";

C1CPP_ODBCRecordSet::C1CPP_ODBCRecordSet() :
    CBLContext(), m_pOtherDB(NULL), m_IsDebugMode(FALSE), m_nCurIndex(0),
    m_MetaNameParser(&m_db)
{
  //AFX_MANAGE_STATE(AfxGetStaticModuleState());	
  
  static bool staticBOOLisSQL = "CODBCDB7" == CString(pDataBase7->GetRuntimeClass()->m_lpszClassName);
  if (staticBOOLisSQL)            
  {
    if (!m_db.IsConnected())
    {
      m_db.Attach(*(SQLHDBC*)(long(pDataBase7->CanBeCommited())+0x4c));
    }
    if (m_db.IsConnected())
    {
      AttachRecordset(&m_db);
    }
  }
  
  if (m_pMetaData == NULL)
  {      
    m_pMetaData = new CMetaDataWork;      
    if (m_pMetaData == NULL)      
    {
      AfxMessageBox("Не достаточно памяти для статического объекта CMetaDataWork!");
    }
  }
  
  m_bAutoTypifycation = FALSE;
  

  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastMethod + 1, 0);        
    defFnNames.AddParam("SetDatabase", "УстБД", procSetDatabase, 0, 1);        
    defFnNames.AddParam("Execute", "Выполнить", funcExecute, 1, 2);
    defFnNames.AddParam("GetLastError", "ПолучитьОписаниеОшибки", funcGetLastError, 1, 0);    
    defFnNames.AddParam("Close", "Закрыть", procClose, 0, 1);
    defFnNames.AddParam("BuildParams", "ПостроитьПараметры", funcBuildParams, 1, 0);
    defFnNames.AddParam("Prepare", "Подготовить", funcPrepare, 1, 1);     
    defFnNames.AddParam("AddParam", "ДобПараметр", funcAddParam, 1, 5);    
    defFnNames.AddParam("GetParam", "ПолучПараметр", funcGetParam, 1, 2);    
    defFnNames.AddParam("SetParam", "УстПараметр", procSetParam, 0, 3);    
    defFnNames.AddParam("ParamCount", "КолвоПараметров", funcParamCount, 1, 0);        
    defFnNames.AddParam("ClearParams", "УдалитьПараметры", procClearParams, 0, 0);
    defFnNames.AddParam("DeleteParam", "УдалитьПараметр", procDeleteParam, 0, 1);        
    defFnNames.AddParam("SetDatabase1C", "УстБД1С", procSetDatabase1C, 0, 0);        
    defFnNames.AddParam("Open", "Открыть", funcOpen, 1, 3);
    defFnNames.AddParam("IsOpen", "ВыборкаОткрыта", funcIsOpen, 1, 0);
    defFnNames.AddParam("MoveNext", "След",     funcMoveNext, 1, 0);
    defFnNames.AddParam("MovePrevious", "Пред", funcMovePrevious, 1, 0);
    defFnNames.AddParam("MoveFirst", "ВНачало",  funcMoveFirst, 1, 0);
    defFnNames.AddParam("MoveLast", "ВКонец",  funcMoveLast, 1, 0);    
    defFnNames.AddParam("GetFieldCount", "КолвоПолей",  funcGetFieldCount, 1, 0);
    defFnNames.AddParam("IsBof", "Начало",  funcIsBof, 1, 0);
    defFnNames.AddParam("IsEof", "Конец",  funcIsEof, 1, 0);
    defFnNames.AddParam("GetRowsAffected", "СтрокОбработанно",  funcGetRowsAffected, 1, 0);        
    defFnNames.AddParam("GetValues", "ПолучитьЗначения", funcGetValues, 1, 0);    
    defFnNames.AddParam("GetResultsInVT", "ПолучитьРезультатыВ_ТЗ", funcGetInTablesValues, 1, 2);    
    defFnNames.AddParam("Debug", "Отладка", procDebug, 0, 1);
    defFnNames.AddParam("GetFieldIndex", "ИндексПоля", funcGetFieldIndex, 1, 1);
    defFnNames.AddParam("GetValue", "ПолучитьЗначение", funcGetValue, 1, 1);
    defFnNames.AddParam("SetRowsetSize", "УстРазмерМножестваСтрок", procSetRowsetSize, 0, 1);    
    defFnNames.AddParam("SetPropertyCursor", "УстСвойстваКурсора", procSetPropertyCursor, 0, 4);
    defFnNames.AddParam("SetTypeColumn1C", "УстТипыКолонок1С", procSetTypeColumn1C, 0, 1);            
    defFnNames.AddParam("GetValuesInSV", "ПолучитьЗначенияСЗ", funcGetValuesInSV, 1, 1);
    defFnNames.AddParam("ExeSQL_FromTV","ВыполнитьSQL_ИзТЗ",funcExeSQL_FromTV, 1, 2);
	  defFnNames.AddParam("SetTextParam","УстановитьТекстовыйПараметр", procSetTextParam, 0, 2);
	  defFnNames.AddParam("GetFieldDefs","ПолучитьПараметрыПоля", funcGetFieldDefs, 1, 8);
	  defFnNames.AddParam("AutoTypification","АвтоТипизация", procAutoTypification, 0, 1);
	  defFnNames.AddParam("PutObjectList","УложитьСписокОбъектов", procPutObjectList, 0, 3);
	  defFnNames.AddParam("SetQueryTimeout","УстТаймаутЗапроса", procSetQueryTimeout, 0, 1);
	  defFnNames.AddParam("IsQueryTimeoutExpired","ИстекТаймаут", funcIsQueryTimeoutExpired, 1, 0);
      defFnNames.AddParam("BackFromTAEnabled","ОбратныйРасчетОтТА", procBackFromTAEnabled, 0, 1);
      defFnNames.AddParam("ExecuteStatement","ВыполнитьИнструкцию", funcExecuteStatement, 1, 3);
      defFnNames.AddParam("ExecuteScalar","ВыполнитьСкалярный", funcExecuteScalar, 1, 1);
      defFnNames.AddParam("ExecuteXML","ВыполнитьXML", funcExecuteXML, 1, 2);
      defFnNames.AddParam("RPCMode","РежимRPC", procRPCMode, 0, 1);
      defFnNames.AddParam("RPCMode","РежимRPC", procRPCMode, 0, 1);
	  defFnNames.AddParam("PutObjectList13","УложитьСписокОбъектов13", procPutObjectList13, 0, 2);
  }
#ifdef _DEBUG
  //pBkEndUI->DoMessageLine("Активизация класса C1CPP_ODBCRecordSet!", mmExclamation);	
#endif
}

C1CPP_ODBCRecordSet::~C1CPP_ODBCRecordSet()
{    
  Close(TRUE);
  m_MetaNameParser.OnClose();
  if (m_pOtherDB!=NULL)
    m_pOtherDB->DecrRef();

#ifdef _DEBUG
  //pBkEndUI->DoMessageLine("Деактивизация класса C1CPP_ODBCRecordSet!", mmExclamation);	
#endif
}

int  C1CPP_ODBCRecordSet::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
  int ret = 1;
	switch(iMethNum)
  {    
    case funcExecuteXML:
      {
		  CheckSetDB();
        BOOL bPrep = ppValue[0]->GetString().IsEmpty();
        if (!bPrep)
        {
          m_MetaNameParser.SetQueryText(ppValue[0]->GetString());
          try
          {
            m_MetaNameParser.Parse();
          } catch (CMNPException * MNPException)
          {
            m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
            MNPException->Delete();
            CBLModule::RaiseExtRuntimeError(m_sLastError, mmRedErr);
            ret = 0;
            break;
          };
          if (m_IsDebugMode)
              pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmNone);
        };

        CBinaryData* pBD = NULL;
        if (ppValue[1]->GetTypeCode() == AGREGATE_TYPE_1C)
        {
            if (!strcmp(ppValue[1]->GetContext()->GetRuntimeClass()->m_lpszClassName, "CBinaryData"))
                pBD = (CBinaryData*)ppValue[1]->GetContext();
        };

        BOOL IsOK;
        if (pBD)
        {
            IsOK = ExecuteXML(m_MetaNameParser.GetQueryText(), bPrep, *pBD);
        }
        else
        {
            CString s;
            IsOK = ExecuteXML(m_MetaNameParser.GetQueryText(), bPrep, s);
            rValue = s;
        };
        if (!IsOK)
        {
            CBLModule::RaiseExtRuntimeError(GetLastError(), mmRedErr);
            if (!bPrep) Close();
            ret = 0;
            break;
        };
      }
      break;
    case funcExecuteScalar:
      {
		  CheckSetDB();
        BOOL bPrep = ppValue[0]->GetString().IsEmpty();
        if (!bPrep)
        {
          m_MetaNameParser.SetQueryText(ppValue[0]->GetString());
          try
          {
            m_MetaNameParser.Parse();
          } catch (CMNPException * MNPException)
          {
            m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
            MNPException->Delete();
            CBLModule::RaiseExtRuntimeError(m_sLastError, mmRedErr);
            ret = 0;
            break;
          };
          if (m_IsDebugMode)
              pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmNone);
        };
        
        SetRowsetSize(0);

        BOOL OpenRet = Open(m_MetaNameParser.GetQueryText(), FALSE, bPrep);
        if (!OpenRet)
        {
            const char* szLastError = GetLastError();
            if (strlen(szLastError))
            {
                if (!bPrep) Close();
                CBLModule::RaiseExtRuntimeError(szLastError, mmRedErr);
                ret = 0;
                break;
            };
        }
        
        int FieldCount = GetFieldCount();
        if (FieldCount == 0)
        {//запрос был не DSL
            rValue = GetRowsAffected();
            if (!bPrep) Close();
            break;
        }
        else if (FieldCount == 1)
        {
            if (OpenRet)
                //SetValueOnDBField(*(GetFields()), rValue, true);
                GetFields()->GetValue(rValue);
            else
            {
                rValue.Reset();
                rValue.SetType(GetFields()->Get1CType());
            }
        }
        else
        {
            rValue.CreateObject("Структура");
            CBLContext* pBLContext = rValue.GetContext();
            if (pBLContext)
            {
                CValue vName, vValue, vByRef(1);
                CValue* aParams[] = {&vName, &vValue, &vByRef};
                int nInsertMeth = pBLContext->FindMethod("Вставить");
                CDBField* pDBField = GetFields();
                for (int i = 0; i < FieldCount; i++, pDBField++)
                {
                    vValue.Reset();
                    vName = pDBField->m_strName;
                    if (OpenRet)
                        //SetValueOnDBField(*pDBField, vValue, true);
                        pDBField->GetValue(vValue);
                    else
                        vValue.SetType(pDBField->Get1CType());
                    pBLContext->CallAsProc(nInsertMeth, aParams);
                };
            };
        };

        if (!bPrep) Close();
      }
      break;
    case funcExecuteStatement:
      {
		  CheckSetDB();
        BOOL bPrep = ppValue[0]->GetString().IsEmpty();
        if (!bPrep)
        {
          m_MetaNameParser.SetQueryText(ppValue[0]->GetString());
          try
          {
            m_MetaNameParser.Parse();
          } catch (CMNPException * MNPException)
          {
            m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
            MNPException->Delete();
            CBLModule::RaiseExtRuntimeError(m_sLastError, mmRedErr);
            ret = 0;
            break;
          };
          if (m_IsDebugMode)
              pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmNone);
        };

        if (!Open(m_MetaNameParser.GetQueryText(), FALSE, bPrep))
        {
            const char* szLastError = GetLastError();
            if (strlen(szLastError))
            {
                if (!bPrep) Close();
                CBLModule::RaiseExtRuntimeError(szLastError, mmRedErr);
                ret = 0;
                break;
            }
        }
        
        if (GetFieldCount() == 0)
        {//запрос был не DSL
            rValue = GetRowsAffected();
            if (!bPrep) Close();
            break;
        }

		CValueTable* pVT;
		CPtrArray* pVL;
		CVTExtended* pVTExt;

		if (ppValue[1]->GetTypeCode() == UNDEFINE_TYPE_1C)
		{
			rValue.CreateObject("ТаблицаЗначений");
			pVT = ((CValueTableContextData*)(rValue.GetContext()->GetInternalData()))->GetValueTable();
		}
		else
		{
			if (!(pVT = CValue2VT(*ppValue[1])))
				if (!(pVL = CValue2VL(*ppValue[1])))
					pVTExt = CValue2VTExt(*ppValue[1]);
			rValue = *ppValue[1];
		}

		if(pVT)
		{
			int nColumns;
			if (ppValue[2]->GetNumeric().operator long())
			{
				pVT->Clear(TRUE);
				nColumns = GetFieldCount();
				if (nColumns == 0)
					break;

				CDBField* pField = GetFields();
				CString sFormatStr;
				for (int i = 0; i < nColumns; i++, pField++)
					pVT->AddColumn(pField->m_strName, pField->Get1CType(), pField->m_strName, 0, sFormatStr, 0);
			}
			else
				nColumns = min(GetFieldCount(), pVT->GetColumnCount());

			if (nColumns)
			{
				CValue* pValues = new CValue[pVT->GetColumnCount()];

				CValue** ppValues = new CValue*[pVT->GetColumnCount()];
				CValue* pEndVal = pValues + pVT->GetColumnCount();

				CValue** ppVal = ppValues;
				for (CValue* pVal = pValues; pVal != pEndVal; pVal++, ppVal++)
					*ppVal = pVal;
        
				pEndVal = pValues + nColumns;

				if (m_nRowsetSize > 0)
				{
					int nRowSize = GetRowSize();
					for (; !IsEof(); MoveNext())
					{
						BYTE* Rowset = GetRowset();
						for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); m_nCurIndex++, Rowset += nRowSize)
						{
							CDBField* pField = GetFields();
							int i = 0;
							for (CValue* pVal = pValues; pVal != pEndVal; pVal++, pField++, i++)
								 //SetValueOfRow(Rowset, *pField, i, *pVal, true);
								 pField->GetValue(*pVal, Rowset + GetOffset(i));

							pVT->AddRow(ppValues, -1);
						}
					}
				}
				else
				{
					for (; !IsEof(); MoveNext())
					{
						CDBField* pField = GetFields();
						for (CValue* pVal = pValues; pVal != pEndVal; pVal++, pField++)
							 //SetValueOnDBField(*pField, *pVal, true);
							 pField->GetValue(*pVal);

						pVT->AddRow(ppValues, -1);
					}
				}
            
				delete[] ppValues;
				delete[] pValues;
			}
		}
		else if (pVL)
		{
            //CArray<class CValueItem *,class CValueItem *>* pVL
            if (ppValue[2]->GetNumeric().operator long())
                pVL->RemoveAll();

            if (GetFieldCount())
			{
				CDBField* pField0 = GetFields();
				CDBField* pField1 = NULL;
				if (GetFieldCount()>1)
					if (pField0[1].Get1CType().GetTypeCode()==2)
						pField1 = pField0 + 1;
				CDBField* pField2 = NULL;
				if (GetFieldCount()>2)
					if (pField0[2].Get1CType().GetTypeCode()==1)
						pField2 = pField0 + 2;

				if (m_nRowsetSize > 0)
				{
					int nRowSize = GetRowSize();
					for (; !IsEof(); MoveNext())
					{
						BYTE* Rowset = GetRowset();
						for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); m_nCurIndex++, Rowset += nRowSize)
						{
							CValueItem* pValueItem = new CValueItem;
							pField0->GetValue(pValueItem->m_value, Rowset + GetOffset(0));
							if (pField1)
								pValueItem->m_text = (LPCSTR)(Rowset + GetOffset(1));
							if (pField2)
								pValueItem->m_bCheck = *(BOOL*)(Rowset + GetOffset(2));
							pVL->Add(pValueItem);
						}
					}
				}
				else
				{
					for (; !IsEof(); MoveNext())
					{
						CValueItem* pValueItem = new CValueItem;
						pField0->GetValue(pValueItem->m_value);
						if (pField1)
							pValueItem->m_text = pField1->AsString();
						if (pField2)
							pValueItem->m_bCheck = pField2->AsBool();
						pVL->Add(pValueItem);
					}
				}
			}
		}
		else if (pVTExt)
		{
			int nColumns;
			CValue val;
            if (ppValue[2]->GetNumeric().operator long())
			{
                pVTExt->procCleanup(NULL);

				nColumns = GetFieldCount();
				if (!nColumns)
					break;

				CDBField* pField = GetFields();
				for( int i = 0; i < nColumns; i++, pField++ )
					pVTExt->NewColumn(pField->m_strName);
			}
			else
				nColumns = min(GetFieldCount(), pVTExt->GetColumnsCount());

			if (m_nRowsetSize > 0)
			{
				int nRowSize = GetRowSize();
				for (; !IsEof(); MoveNext())
				{
					BYTE* Rowset = GetRowset();
					for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); m_nCurIndex++, Rowset += nRowSize)
					{
						CVTExtRow* pRow = pVTExt->GetRow(pVTExt->NewRow());
						CDBField* pField = GetFields();
						for( int i = 0; i < nColumns; i++, pField++ )
						{
							pField->GetValue(val, Rowset + GetOffset(i));
							pRow->SetValue(i, val);
						}
					}
				}
			}
			else
			{
				for (; !IsEof(); MoveNext())
				{
					CVTExtRow* pRow = pVTExt->GetRow(pVTExt->NewRow());
					CDBField* pField = GetFields();
					for( int i = 0; i < nColumns; i++, pField++ )
					{
						pField->GetValue(val);
						pRow->SetValue(i, val);
					}
				}
			}
		}
		else
		{
			if (!bPrep) Close();
			RuntimeError("Недопустимое значение второго аргумента метода!");
		}
        if (!bPrep) Close();
      }
      break;
    case funcExeSQL_FromTV:
      {
        CBLContext *pCont = ppValue[0]->GetContext();
        if (pCont == NULL)
          CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода!", 0);
        
        if (m_IsDebugMode)
        {
          CString strClassName(pCont->GetRuntimeClass()->m_lpszClassName);
          if (strClassName != "CValueTableContext")
          {
            CSetOfHierarchy *pH = CSetOfHierarchy::GetHierarchy();
            if (!pH->TheClassIsDerive(pCont->GetTypeString(), "ТаблицаЗначений") && 
                !pH->TheClassIsDerive(pCont->GetTypeString(), "ValueTable"))
            {
              CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода, должна быть таблица значений или производный от нее класс", 0);
            }
          }
        }
        
        CValue par1;        
        CValue *ppPs[1] = {&par1};
        CValue arValue;
        
        CValueTable* pTable = ((CValueTableContextData*)pCont->GetInternalData())->GetValueTable();
        //int staticColumnCount = pCont->FindMethod("ColumnCount");
        //int staticLinesCnt = pCont->FindMethod("LinesCnt");

        /*pCont->CallAsFunc(staticColumnCount,arValue, ppPs);
        int nNumColTV = arValue.GetNumeric();
        pCont->CallAsFunc(staticLinesCnt,arValue, ppPs);
        int nLinesCnt = arValue.GetNumeric();*/
        int nNumColTV = pTable->GetColumnCount();
        int nLinesCnt = pTable->GetRowCount();
        
        if (nNumColTV != 0 && nLinesCnt != 0)
        {          
          int nRowsetSize = ppValue[1]->GetNumeric();
          
          if (nRowsetSize == 0)          
            nRowsetSize = static_cast<int>(nLinesCnt*0.1 == 0 ? nLinesCnt : nLinesCnt*0.1);

          nRowsetSize = nRowsetSize == 0 ? 1 : nRowsetSize;
          
          nRowsetSize = min(nLinesCnt, nRowsetSize);

          SetRowsetSizeForParams(nRowsetSize);
          if (ParamCount() == 0)
            if (BuildParams() == FALSE)
            {
              CString err;
              err.Format("Произошла ошибка при формировании автоматических параметров.\
                \r\nНеобходимо выполнить подготовку запроса или проверить синтаксис! \r\n %s", GetLastError());
              Close();
              CBLModule::RaiseExtRuntimeError(err, 0);
            }
          int nCountParams = m_aParams.GetSize();
          int nColumns = min(nCountParams, nNumColTV);

          /*
          int staticGetColumnParameters = pCont->FindMethod("GetColumnParameters");
          vector<int> vecTypesOfColumn;
          CValue Ident, Type, Lenght, Scale, Caption, Width, Format, Position;
          CValue *Params[8] = {&Ident, &Type, &Lenght, &Scale, &Caption, &Width, &Format, &Position};
          for (int id = 0; id < nColumns; ++id) 
          {            
            Ident = id+1;
            if (pCont->CallAsFunc(staticGetColumnParameters,arValue, Params)==1)
            {
              int type = 0;

              CString nameCol = Caption.GetString();
              nameCol.TrimRight();
              
              int pPosQuest = nameCol.Find("?");
              int res = 0;
              if (pPosQuest != -1)
              {
                res = atoi(nameCol.operator LPCTSTR() + pPosQuest + 1);
              }
              
              switch(res)
              {
               case 1:
                 
                 type = 0;
                 break;
               case 2:                  
                 type = 1;
                 break;
               case 3:                  
                 type = UNDEF_TYPE_MODIFIKATOR;
                 break;
               default:
                 type = 0;
                 break;                  
              }              
              vecTypesOfColumn.push_back(type);
            }            
          }
          */
          //int staticGetValue = pCont->FindMethod("GetValue");          
          //CValue vRetTV;
          //CValue vNumStroke, vNumColumn, vRetTV;
          //CValue *mPars[2] = {&vNumStroke, &vNumColumn};
          int nNumCurRow = 1;
          for (int i=1; i <= nLinesCnt; ++i)
          {        
            for (int j=1; j <= nColumns; ++j)
            {
              //vNumStroke = i;
              //vNumColumn = j;
              //if (pCont->CallAsFunc(staticGetValue,vRetTV, mPars) == 1)
              {

                void *pData = NULL;
                static_cast<CDBField&>(GetParam(j-1)) = pTable->GetValue(j-1, i-1);
                //SetValueOfParam(j-1, vRetTV, vecTypesOfColumn[j-1]);

                int size = 0;
                if (GetParam(j-1).GetBufferAndSize(&pData, size))
                  SetParamOfRow(nNumCurRow, j, size, pData);
              }
            }
            ++nNumCurRow;
            if (nRowsetSize < nNumCurRow || (i) == nLinesCnt)
            {
              if (!Execute(NULL, TRUE))
              {                
                CString err;
                err.Format("Произошла ошибка при выполнении запроса.\
                  \r\n %s", GetLastError());
                Close();
                CBLModule::RaiseExtRuntimeError(err, 0);               
              }
              if ((nLinesCnt - i) < nRowsetSize && nLinesCnt != i)
              {
                nRowsetSize = nLinesCnt - i;
                ResizeRowsetSizeForParams(nRowsetSize);
              }
              nNumCurRow = 1;            
            }
          }  
          //Close();
        }
        else
        {
          rValue = 0L;
        }        
        
      }
    case funcGetValuesInSV:
      {
        int nCount = 0;
        int c = GetFieldCount();        
        if (c == 0)
        {
          rValue = CNumeric(0);
        }
        else
        {
          
          switch(ppValue[0]->GetTypeCode())
          {
            case STRING_TYPE_1C:
              {
                CMapWordToPtr NumOfColumn;

                const CString &str = ppValue[0]->GetString();      
              
                char *token = NULL;
                char *dest = (char*)malloc(str.GetLength()+1); // TODO надежнее использовать vector или смарт-пойнтер
                const char *sep = ",";
                int i = 0;
                if (dest)
                {
                  strcpy(dest, str);
                  
                  CBLContext *pContStruct = NULL;
                  int staticInsert = 0;

                  token = strtok( dest, sep );
                  while( token != NULL  && i < c)
                  {         
                    if (pContStruct == NULL)
                    {
                      rValue.CreateObject("Struct");                      
                      pContStruct = rValue.GetContext();
                      staticInsert = pContStruct->FindMethod("Insert");                  
                    }

                    CString s(token);
                    s.Replace(" ", "");
                    WORD pos = static_cast<WORD>(GetFieldIndex(s));
                    if (pos == -1)
                    {
                      CString err;
                      err.Format("Данной колонки с именем: %s не существует в выворке!", s);
                      free(dest);
                      CBLModule::RaiseExtRuntimeError(err, 0);
                    }
                    
                    CValue par1;
                    CValue par2;
                    CValue par3;
                    par1 = s;
                    par2.CreateObject("ValueList");
                    
                    par3 = CNumeric(1);
                    CValue *params[3] = {&par1, &par2, &par3};
                    NumOfColumn[pos] = (void*)(par2.GetContext());
                    pContStruct->CallAsProc(staticInsert, params);                    
                    ++i;                    
                    token = strtok( NULL, sep );
                  }
                  free(dest);
                  
                  
                  int n = min(GetFieldCount(), i);
                  if (n > 0)
                  {                   
                    CValue ValueList;
                    ValueList.CreateObject("ValueList");

                    int nAddValue = ValueList.GetContext()->FindMethod("AddValue");

                    if (m_nRowsetSize > 0)            
                    {                    
                      for( ; ! IsEof(); MoveNext() )
                      {
                        for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); ++m_nCurIndex)
                        {                      
                          BYTE* Rowset = GetRowset() + GetRowSize()*m_nCurIndex;
                          POSITION pos;
                          WORD key;
                          CBLContext* pa;
                          for( pos = NumOfColumn.GetStartPosition(); pos != NULL; )
                          {
                            NumOfColumn.GetNextAssoc( pos, key, (void*&)pa );                          

                            CValue v;
                            CValue v2;
                            CDBField& f = operator [](key);
                            //SetValueOfRow(Rowset, f, key, v, true);
                            f.GetValue(v, Rowset + GetOffset(key));
                            CValue *params[2] = {&v, &v2};                          
                            pa->CallAsProc(nAddValue, params);                          
                          }
                        }
                      }
                    }
                    else
                    {            
                      for( ; ! IsEof(); MoveNext() )
                      {                                                        
                        POSITION pos;
                        WORD key;
                        CBLContext* pa;
                        for( pos = NumOfColumn.GetStartPosition(); pos != NULL; )
                        {               
                          NumOfColumn.GetNextAssoc( pos, key, (void*&)pa );                          
                
                          CValue v;
                          CValue v2;
                          CDBField& f = operator [](key);
                          //SetValueOnDBField(f, v, true);                                                
                          f.GetValue(v);
                          CValue *params[2] = {&v, &v2};                        
                          pa->CallAsProc(nAddValue, params);                        
                        }                
                      }
                    }
                  }
                  
                }
              }
            break;
            case NUMBER_TYPE_1C:
              {
                int pos = ppValue[0]->GetNumeric();
                if (pos > c || 1 > c)
                  CBLModule::RaiseExtRuntimeError("Передан неверный идентификатор колонки", 0);
                
                --pos;
                rValue.CreateObject("ValueList");
                CBLContext *pCont = rValue.GetContext();
                int staticAddValue = pCont->FindMethod("AddValue");

                if (m_nRowsetSize > 0 && pCont != NULL)            
                {                  
                  for( ; ! IsEof(); MoveNext() )
                  {
                    for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); ++m_nCurIndex)
                    {                      
                      BYTE* Rowset = GetRowset() + GetRowSize()*m_nCurIndex;                                            
                      CValue v;
                      CValue v2;
                      CDBField& f = operator [](pos);
                      //SetValueOfRow(Rowset, f, pos, v, true);
                      f.GetValue(v, Rowset + GetOffset(pos));
                      CValue *params[2] = {&v, &v2};
                      pCont->CallAsProc(staticAddValue, params);                      
                    }
                  }
                }
                else
                {            
                  for( ; ! IsEof(); MoveNext() )
                  {                                                                   
                    CValue v;
                    CValue v2;
                    CDBField& f = operator [](pos);
                    //SetValueOnDBField(f, v, true);
                    f.GetValue(v);
                    CValue *params[2] = {&v, &v2};
                    pCont->CallAsProc(staticAddValue, params);                    
                  }
                }
              }
              break;
            default:
              {
                CBLModule::RaiseExtRuntimeError("Недопустимый тип параметра", 0);
              }
          }          
        }
      } // if (c == 0)
      break;
    case funcGetFieldIndex:
      {
        rValue = GetFieldIndex(ppValue[0]->GetString());
      }
      break;
    case funcGetLastError:
      {
        rValue = GetLastError();
      }
      break;
    case funcPrepare:
      {
		  CheckSetDB();
        if (m_MetaNameParser.GetRPCMode())
        {
            CBLModule::RaiseExtRuntimeError("В RPC режиме параметризированные запросы невозможны.", 0);
            return 0;
        };

        int r = TRUE;
        m_MetaNameParser.SetQueryText(ppValue[0]->GetString());

        try 
        {
          m_MetaNameParser.Parse();
        } 
        catch (CMNPException * MNPException) 
        {
          m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
          MNPException->Delete();
          r = FALSE;
		};
      if (m_IsDebugMode)
      {
        pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmNone);
      }        

		  if (r)
		    rValue = Prepare(m_MetaNameParser.GetQueryText());
          else
            rValue = 0L;
        //pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmExclamation);	
      }
      break;
    case funcAddParam:
      {
		  CheckSetDB();
        long IOType = ppValue[0]->GetNumeric().operator long();        
        if (IOType < 1 && IOType > 3)
          CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: тип IO параметра", 0);
        switch(IOType)
        {        
          case 1:
            IOType = SQL_PARAM_INPUT;
            break;
          case 2:
            IOType = SQL_PARAM_OUTPUT;
            break;
          case 3:
            IOType = SQL_PARAM_INPUT_OUTPUT;
            break;
        }

        SWORD nSQLType = static_cast<SWORD>(ppValue[1]->GetNumeric().operator long());
        switch(nSQLType)
        {          
          case 1:
            nSQLType = SQL_BIT;
            break;
          case 2:
            nSQLType = SQL_TINYINT;
            break;
          case 3:
            nSQLType = SQL_SMALLINT;
            break;
          case 4:
            nSQLType = SQL_INTEGER;
            break;
          case 5:
            nSQLType = SQL_REAL;
            break;
          case 6:
            nSQLType = SQL_FLOAT;
            break;
          case 7:
            nSQLType = SQL_DOUBLE;
            break;
          case 8:
            nSQLType = SQL_TYPE_DATE;
            break;
          case 9:
            nSQLType = SQL_TYPE_TIME;
            break;
          case 10:
            nSQLType = SQL_TYPE_TIMESTAMP;
            break;
          case 11:
            nSQLType = SQL_NUMERIC;
            break;
          case 12:
            nSQLType = SQL_DECIMAL;
            break;
          case 13:
            nSQLType = SQL_BIGINT;
            break;
          case 14:
            nSQLType = SQL_CHAR;
            break;
          case 15:
            nSQLType = SQL_VARCHAR;
            break;          
          case 16:
            nSQLType = SQL_LONGVARCHAR;
            break;
          case 17:
            nSQLType = SQL_BINARY;
            break;
          case 18:
            nSQLType = SQL_VARBINARY;
            break;
          case 19:
            nSQLType = SQL_LONGVARBINARY;
            break;
          default:
            {
              CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: тип SQL параметра", 0);
            }
            break;
        }
        int nLen = ppValue[2]->GetNumeric();
        int nDec = ppValue[3]->GetNumeric();
        CType type(1);
        BOOL ret = AddParam(ppValue[4]->GetString(), IOType, nSQLType, type, nLen, nDec);
        if (ret)
            m_aParams[m_aParams.GetSize() - 1].m_Modificator = m_MetaNameParser.GetSQLParamModifikator(m_aParams.GetSize());
        rValue = ret;
      }
      break;
    case funcGetParam:
      {
        long nPar = m_MetaNameParser.GetSQLParamNumber(*ppValue[0]);
        if (nPar < 1 || nPar > ParamCount())
          CBLModule::RaiseExtRuntimeError("Недопустимое значение индекса параметра", 0);
        CDBField& qp = static_cast<CDBField&>(GetParam(nPar-1));
        
        if(qp.AsChar() || qp.AsString())
        {          
          CString str = ppValue[1]->GetString();
          if (str.IsEmpty())
            rValue = qp.operator CString();
          else
          {
            CType t(0);
            CMetaDataWork::SetTypeAndKind(t, str);
            rValue.SetType(t);
            CMetaDataWork::TypeStringValue eType;            
            if (t.GetTypeCode() == 0)
              eType = CMetaDataWork::VeryLongString;
            else
              eType = t.GetTypeID() == 0 ? CMetaDataWork::LongString: CMetaDataWork::ShortString;

            CMetaDataWork::Make1C_ValueFromString(rValue, t, qp.operator CString(), eType);
          }
        }
        else if(qp.AsDate())
        {
          const COleDateTime &date = qp.operator COleDateTime();          
          rValue = CDate(date.GetYear(),date.GetMonth(),date.GetDay());
        }
        else if (qp.AsFloat())
          rValue = CNumeric(qp.operator float());
        else if (qp.AsInt())
          rValue = CNumeric(qp.operator int());
        else if (qp.AsDouble())
          rValue = CNumeric(qp.operator double());
        else if (qp.AsLong())
          rValue = CNumeric(qp.operator long());
        else if (qp.AsShort())
          rValue = CNumeric(qp.operator short());
        else if (qp.AsBool())
          rValue = CNumeric(qp.operator bool());
      }
      break;
    case funcParamCount:
      {
        rValue = ParamCount();
      }
      break;
    case funcBuildParams:
      {
		  CheckSetDB();
        int ret = BuildParams();
        if (ret)
          m_MetaNameParser.SetSQLParamsModifikators(m_aParams);

        rValue = ret;
      }
      break;
    case funcOpen:
      {
		  CheckSetDB();
        CString strSQL(ppValue[0]->GetString());        
        BOOL bScroll = ppValue[1]->GetNumeric();
        BOOL bPrep   = ppValue[2]->GetNumeric();
        if (strSQL.IsEmpty())
          bPrep = TRUE;
        else {
          m_MetaNameParser.SetQueryText(strSQL);
          try {
            m_MetaNameParser.Parse();
          } catch (CMNPException * MNPException) {
            m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
            MNPException->Delete();
            rValue = 0L;
            break;
          };
          strSQL = m_MetaNameParser.GetQueryText();
        };

        if (bPrep)
          bScroll = FALSE;
        
        if (m_IsDebugMode && !bPrep)
        {
            pBkEndUI->DoMessageLine(strSQL, mmNone);
        }        
        rValue = Open(strSQL, bScroll, bPrep);
        if (m_nRowsetSize > 0)
          m_nCurIndex = 0;      
      }
      break;
    case funcIsOpen:
      rValue = IsOpen();
      break;
    case funcMoveNext:
      {
        if (m_nRowsetSize > 0)
        {
          ++m_nCurIndex;
          if (m_nCurIndex == GetRowFetched())
          {
            m_nCurIndex = 0;
            rValue = MoveNext();
          }
          else
            rValue = TRUE;
        }
        else
          rValue = MoveNext();        
      }
      break;
    case funcMovePrevious:
      rValue = MovePrevious();
      break;
    case funcMoveFirst:
      rValue = MoveFirst();
      break;
    case funcMoveLast:
      rValue = MoveLast();
      break;
    case funcGetFieldCount:
      rValue = GetFieldCount();
      break;
    case funcIsBof:
      rValue = IsBof();
      break;
    case funcIsEof:
      rValue = IsEof();
      break;
    case funcGetRowsAffected:
      rValue = GetRowsAffected();
      break;
    case funcGetValues:
      {        
        int n = min(GetFieldCount(), n_nCountUnlimitPars);
        n_nCountUnlimitPars = 0;
        if (IsBof() || IsEof() || n == 0)
        {
          rValue = CNumeric(0);          
        }
        else
        {
          if (m_nRowsetSize > 0)
          {
            BYTE* Rowset = GetRowset() + GetRowSize()*m_nCurIndex;
            for (int i = 0; i < n; ++i)
            {              
              //SetValueOfRow(Rowset, operator [](i), i, *ppValue[i], true);
                operator [](i).GetValue(*ppValue[i], Rowset + GetOffset(i));
            }
          }
          else
          {
            for (int i = 0; i < n; ++i)
            {                            
              //SetValueOnDBField(operator [](i), *ppValue[i], true);
              operator [](i).GetValue(*ppValue[i]);
            }
          }
          rValue = CNumeric(1);
        }
      }
      break;
    case funcGetValue:
      {
        int i = -1;
        if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
        {
          i = ppValue[0]->GetNumeric();
          if (i > GetFieldCount() || i < 1)
          {
            CString err;
            err.Format("Передано неверное значение индекса (%d)!", i);
            CBLModule::RaiseExtRuntimeError(err, 0);
          }
          --i;
        }
        else if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
        {
          i = GetFieldIndex(ppValue[0]->GetString());
          if (i == -1)
          {
            CString err;
            err.Format("Колонка с именем (%s) отсутсвует в выборке!", ppValue[0]->GetString());
            CBLModule::RaiseExtRuntimeError(err, 0);
          }
        }
        else
          CBLModule::RaiseExtRuntimeError("Недопустимый тип первого аргумента метода!", 0);

        if (m_nRowsetSize > 0)          
        {
          BYTE* Rowset = GetRowset() + GetRowSize()*m_nCurIndex;
          //SetValueOfRow(Rowset, operator [](i), i, rValue, true);
          operator [](i).GetValue(rValue, Rowset + GetOffset(i));
        }          
        else
        {
          //SetValueOnDBField(operator [](i), rValue, true);
          operator [](i).GetValue(rValue);
        }
      }
      break;
    case funcGetInTablesValues:
      {
        bool res = false;
        /*CString field =  "     0   ";
        const CString& st1 = field.Left(6);
        const CString& st2 = field.Mid(6);*/
		    if (GetFieldCount() == 0)
        {
          res = false;
        }
        else
        {
          CBLContext *pCont = ppValue[0]->GetContext();
          if (pCont == NULL)
            CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода!", 0);

#ifndef DMITRO_ENV
          if (m_IsDebugMode)
          {
            CString strClassName(pCont->GetRuntimeClass()->m_lpszClassName);
            if (strClassName != "CValueTableContext")
            {
              CSetOfHierarchy *pH = CSetOfHierarchy::GetHierarchy();
              if (!pH->TheClassIsDerive(pCont->GetTypeString(), "ТаблицаЗначений") && 
                  !pH->TheClassIsDerive(pCont->GetTypeString(), "ValueTable"))
              {
                CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода, должна быть таблица значений или производный от нее класс", 0);
              }
            }
          }
#endif
        
          CValue arValue;        
          CValue par1;        
          CValue *ppPs[1] = {&par1};
          BOOL bCreate = ppValue[1]->GetNumeric();
          int n = 0;
          if (bCreate)
          {
            int staticClear = pCont->FindMethod("Clear");
            int staticNewColumn = pCont->FindMethod("NewColumn");
            pCont->CallAsProc(staticClear, NULL);
            CValue Ident;
					  CValue Type;
					  CValue Length;
					  CValue Scale;
					  CValue Caption;
					  CValue Width;
					  CValue Format;
					  CValue Position;
            CValue *Params[8] = {&Ident, &Type, &Length, &Scale, &Caption, &Width, &Format, &Position};
            n = GetFieldCount();
            for (int i = 0; i < n; ++i) 
            {
              CDBField& f = operator [](i);              
              Ident  = f.m_strName;
              
              int Len, Scal;
              Type = GetField_1CType(f, Len, Scal);
              Length = Len;
              Scale = Scal;
              pCont->CallAsFunc(staticNewColumn,arValue, Params);
            }            
          }
          else
          {
            int staticColumnCount = pCont->FindMethod("ColumnCount");
            pCont->CallAsFunc(staticColumnCount,arValue, ppPs);
            n = min(GetFieldCount(),arValue.GetNumeric());            
          }

          int staticNewLine     = pCont->FindMethod("NewLine");
          //CValueTable * pVT =((CValueTableContextData*)ppValue[0]->GetContext()->GetInternalData())->GetValueTable();

          if (n > 0)
          {
            res = true;            
            if (m_nRowsetSize > 0)            
            {
              for( ; ! IsEof(); MoveNext() )
              {
                for (m_nCurIndex = 0; m_nCurIndex < GetRowFetched(); ++m_nCurIndex)
                {
                  pCont->CallAsFunc(staticNewLine ,arValue, ppPs);                  
                  //pVT->CValueTable::NewRow(-1); 
                  BYTE* Rowset = GetRowset() + GetRowSize()*m_nCurIndex;
                  for (int i = 0; i < n; ++i)                                  
                  {
                    CValue v;                                        
                    CDBField& f = operator [](i);
                    //SetValueOfRow(Rowset, f, i, v, true);
                    f.GetValue(v, Rowset + GetOffset(i));
                    //pVT->CValueTable::SetValue(v,i,ind);
                    pCont->SetPropVal(i+1, v);
                    /*CString str;                    
                    str.Format("SetPropVal = %f", v.GetNumeric().GetDouble());
                    
                    pCont->GetPropVal(i+1, v);                    
                    str.Format("v = %f\r\n====================================", v.GetNumeric().GetDouble());
                    pBkEndUI->DoMessageLine(str, mmInformation);*/
                  }
                }
              }
            }
            else
            {            
              for( ; ! IsEof(); MoveNext() )
              {            
                //pBkEndUI->DoMessageLine("NewLine", mmExclamation);
                pCont->CallAsFunc(staticNewLine ,arValue, ppPs);
                //pVT->CValueTable::NewRow(-1); 
                for (int i = 0; i < n; ++i) 
                {               
                  CValue v;                  
                  CDBField& f = operator [](i);
                  //SetValueOnDBField(f, v, true);
                  f.GetValue(v);
                  pCont->SetPropVal(i+1, v);
                  //pVT->CValueTable::SetValue(v,i,ind);
                  /*
                  CString str;
                  str.Format("SetPropVal = %f", v.GetNumeric().GetDouble());
                  
                  pCont->GetPropVal(i+1, v);                  
                  str.Format("GetPropVal = %f\r\n====================================", v.GetNumeric().GetDouble());
                  pBkEndUI->DoMessageLine(str, mmInformation);*/
                }                
              }
            }
          }
        }
        rValue = res;        
      }
      break;
	case funcGetFieldDefs:
      {
        int i;
        if (ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C)
        {
          i = ppValue[0]->GetNumeric();
          if (i > GetFieldCount() || i < 1)
          {
			rValue = "";
			break;
          }
          --i;
          rValue = operator [](i).m_strName;
        }
        else if(ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
        {
          i = GetFieldIndex(ppValue[0]->GetString());
          rValue = i;
        }
        else
          CBLModule::RaiseExtRuntimeError("Недопустимый тип первого аргумента метода!", 0);
        CDBField& f = operator [](i);

        //1C defs
        int Len, Scal;
        *ppValue[1] = GetField_1CType(f, Len, Scal);
        *ppValue[2] = Len;
        *ppValue[3] = Scal;
        //SQL defs
        *ppValue[4] = f.m_nSQLType;
        *ppValue[5] = f.m_nPrecision;
        *ppValue[6] = f.m_nScale;
        *ppValue[7] = f.m_nNullability;
      }
      break;
    case funcExecute:
      {
		  CheckSetDB();
        int r = true;
        if (ppValue[0]->GetTypeCode() == STRING_TYPE_1C)
		{
          m_MetaNameParser.SetQueryText(ppValue[0]->GetString());
          try {
            m_MetaNameParser.Parse();
          } catch (CMNPException * MNPException) {
            m_sLastError = szMNPErrorPrefix + MNPException->GetErrorDescr();
            MNPException->Delete();
            r = FALSE;
          };
          if (m_IsDebugMode)
          {
            pBkEndUI->DoMessageLine(m_MetaNameParser.GetQueryText(), mmNone);
          };        
		  if (r)
            r = Execute(m_MetaNameParser.GetQueryText(), ppValue[1]->GetNumeric().operator long());
		}
        else
          r = Execute(NULL, TRUE);
        rValue = r;
        //if (r == FALSE)
        //  CBLModule::RaiseExtRuntimeError(GetLastError(), 0);
      }
      break;
    case funcIsQueryTimeoutExpired:
        if (IsQueryTimeoutExpired())
            rValue = 1;
        else
            rValue = 0L;
      break;
    default:
      {
        CString err;
        err.Format("Нет обработчика метода под номером: %d)", iMethNum);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }      
  }
  

  return ret;
}

int  C1CPP_ODBCRecordSet::CallAsProc(int iMethNum,class CValue * * ppValue)
{
  switch(iMethNum)
  {
    case procPutObjectList13:
      {
		  CheckSetDB();
        CString TableName = ppValue[1]->GetString();
        CString Query = "if exists(select * from tempdb..sysobjects where id = object_id('tempdb..%tmp'))\r\n\
            drop table %tmp\r\n\
            create table %tmp (val char(13), primary key clustered (val))";
        Query.Replace("%tmp", TableName);
        Execute(Query);

        Query.Format("insert %s values (?)", TableName);
        Prepare(Query);
        AddParam("", SQL_PARAM_INPUT, SQL_CHAR, CType(0), 13, 0);
        CQueryParam & qp = GetParam(0);

        if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
        {
            CValueListContext* pListCtx=(CValueListContext*)ppValue[0]->GetContext();
            if (pListCtx && !strcmp(pListCtx->GetRuntimeClass()->m_lpszClassName,"CValueListContext"))
            {
                CPtrArray* pVL=*(CPtrArray**) (((char*)pListCtx)+0x30);
                int listSize=pVL->GetSize();
                for(int i=0;i<listSize;i++)
                {
	                CValue* pItem=(CValue*)pVL->GetAt(i);
                    static_cast<CDBField&>(qp) = m_pMetaData->GetObjDBString(*pItem, CMetaDataWork::TypeStringValue::LongString);
                    Execute(NULL, TRUE);
                }
            }
        } 
        else if (ppValue[0]->GetTypeCode() > DATE_TYPE_1C)
        {
            static_cast<CDBField&>(qp) = m_pMetaData->GetObjDBString(*ppValue[0], CMetaDataWork::TypeStringValue::LongString);
            Execute(NULL, TRUE);
        }
        else
        {
            Close();
            CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода!", 0);
            return 0;
        }
        Close();
      }
      break;
    case procSetTypeColumn1C:
    {
      int c = GetFieldCount();
      if (c == 0)
        CBLModule::RaiseExtRuntimeError("Данный метод возможно вызывать только после подготовки запроса или открытия курсора", 0);
      if (ppValue[0]->GetTypeCode() !=  STRING_TYPE_1C)
        CBLModule::RaiseExtRuntimeError("Первый параметр метода должен иметь строковый тип!", 0);
      
      const CString &str = ppValue[0]->GetString();
      
      char *token = NULL;
      char *dest = (char*)malloc(str.GetLength()+1);          
      const char *sep = ",";
      int i = 0;
      if (dest)
      {
        strcpy(dest, str);
        token = strtok( dest, sep );
        while( token != NULL  && i < c)
        {         
          CString s(token);
          s.Replace(" ", "");
          CDBField& f = operator [](i++);          
          CMetaDataWork::SetTypeAndKind(f.m_rt, s);

          token = strtok( NULL, sep );
        }
        free(dest);
      }
    }
    break;
    case procSetPropertyCursor:
    {
      int nCurType = ppValue[0]->GetNumeric();
      int nCurScrollable = ppValue[1]->GetNumeric();
      int nCurSensitivity = ppValue[2]->GetNumeric();
      int nCurConcurrency = ppValue[3]->GetNumeric();
      
      if (nCurType < 0 && nCurType > 4)
        CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: SQL_ATTR_CURSOR_TYPE", 0);
      if (nCurScrollable < 0 && nCurScrollable > 2)
        CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: SQL_ATTR_CURSOR_SCROLLABLE", 0);
      if (nCurSensitivity < 0 && nCurSensitivity > 3)
        CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: SQL_ATTR_CURSOR_SENSITIVITY", 0);
      if (nCurConcurrency < 0 && nCurConcurrency > 4)
        CBLModule::RaiseExtRuntimeError("Недопустимое значение параметра: SQL_ATTR_CONCURRENCY", 0);

      BOOL ret = FALSE;
      if (nCurType > 0)
      {
        switch(nCurType)
        {
          case 1:
            nCurType = SQL_CURSOR_FORWARD_ONLY;
            break;
          case 2:
            nCurType = SQL_CURSOR_STATIC;
            break;
          case 3:
            nCurType = SQL_CURSOR_KEYSET_DRIVEN;
            break;
          case 4:
            nCurType = SQL_CURSOR_DYNAMIC;
            break;
        }
        SetCursorsTypes(nCurType);
      }
      if (nCurScrollable > 0)
      {
        switch(nCurScrollable)
        {
          case 1:
            nCurScrollable = SQL_NONSCROLLABLE;
            break;
          case 2:
            nCurScrollable = SQL_SCROLLABLE;
            break;          
        }
        SetCursorsScrollable(nCurScrollable);
      }
      if (nCurSensitivity > 0)
      {
        switch(nCurSensitivity)
        {
          case 1:
            nCurSensitivity = SQL_UNSPECIFIED;
            break;
          case 2:
            nCurSensitivity = SQL_INSENSITIVE;
            break;
          case 3:
            nCurSensitivity = SQL_SENSITIVE;
            break;
        }
        SetCursorsSensitivity(nCurSensitivity);
      }      
      if (nCurConcurrency > 0)
      {
        switch(nCurConcurrency)
        {
          case 1:
            nCurConcurrency = SQL_CONCUR_READ_ONLY;
            break;
          case 2:
            nCurConcurrency = SQL_CONCUR_LOCK;
            break;
          case 3:
            nCurConcurrency = SQL_CONCUR_ROWVER;
            break;
          case 4:
            nCurConcurrency = SQL_CONCUR_VALUES;          
            break;          
        }
        SetCursorConcurrency(nCurConcurrency);
      }
    }
    break;
    case procSetRowsetSize:
      {
        SetRowsetSize(ppValue[0]->GetNumeric());
      }
      break;
    case procSetDatabase:
      {
        CBLContext* pCont = ppValue[0]->GetContext();
        bool err = true;
        if (pCont != NULL)
        {
          if (pCont->GetRuntimeClass() == RUNTIME_CLASS(C1CPP_ODBCDatabase))
          { 
            err = false;
            C1CPP_ODBCDatabase* db = static_cast<C1CPP_ODBCDatabase*>(pCont);
            if (m_pOtherDB!=NULL)
              m_pOtherDB->DecrRef();
            else
            {
              //if (m_db.IsConnected())
              //  m_db.ResetCurrent();
            }

            m_pOtherDB = db;
            
            SetDatabase(db);
            m_pOtherDB->IncrRef();
            m_MetaNameParser.SetIBProp(db->m_pMetaDataCont, db->m_pCurrentIBSet);
            m_MetaNameParser.SetDB(db);
          }
        }
        if (err)
          CBLModule::RaiseExtRuntimeError("В метод SetDatabase/УстБД передан неверный параметр!", 0);
      }
      break;
    case procClose:
      {
        Close(ppValue[0]->GetNumeric().operator long());
        m_MetaNameParser.OnClose();
      }
      break;
    case procSetParam:
      {
        long nPar = m_MetaNameParser.GetSQLParamNumber(*ppValue[0]);
        
        
        if (nPar < 1 || nPar > ParamCount())
          CBLModule::RaiseExtRuntimeError("Недопустимое значение индекса параметра", 0);
        /*
        int Modifikator;
        if (ppValue[2]->GetTypeCode() == UNDEFINE_TYPE_1C)
          Modifikator = m_aParams[nPar-1].m_Modificator;
        else
          Modifikator = ppValue[2]->GetNumeric();
        */
        static_cast<CDBField&>(GetParam(nPar-1)) = *ppValue[1];
        /*
        if (!SetValueOfParam(nPar-1, *ppValue[1], Modifikator))
        {
          CBLModule::RaiseExtRuntimeError("Данный тип значения параметра не поддерживается!", 0);
        } 
        */
      }
      break;
    case procClearParams:
      {
        ClearParams();
      }
      break;
    // Удаляет ранее установленные параметры по индексу
    case procDeleteParam:
      {
		  CheckSetDB();
        int i = ppValue[0]->GetNumeric();
        if (i > ParamCount())
          CBLModule::RaiseExtRuntimeError("Недопустимое значение индекса параметра", 0);
        DeleteParam(i-1); 
      }
      break;
    case procSetDatabase1C:
      {
        
        Close(TRUE);
        if (m_pOtherDB!=NULL)
        {
          m_pOtherDB->DecrRef();
          m_pOtherDB = NULL;
        }
        AttachRecordset(&m_db);
      }
      break;
    case procDebug:
      {
        m_IsDebugMode = ppValue[0]->GetNumeric();
      }
      break;
	case procSetTextParam:
		m_MetaNameParser.SetParameter(ppValue[0]->GetString(), *ppValue[1]);
    break;
	case procAutoTypification:
        m_bAutoTypifycation = ppValue[0]->GetNumeric().operator long();
      break;
  case procPutObjectList:
      {
		  CheckSetDB();
        CSbCntTypeDef* SbCntTypeDef = NULL;
        CPlanDef* PlanDef = NULL;
        CString RefName = ppValue[2]->GetString();
        if (!RefName.IsEmpty())
        {
            SbCntTypeDef = pMetaDataCont->GetSTypeDef(RefName);
            if (SbCntTypeDef == NULL)
            {
                CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
                if (pBuhDef)
        		    PlanDef = pBuhDef->GetPlanDef(RefName);

            };

            if (SbCntTypeDef == NULL && PlanDef == NULL)
            {
                CBLModule::RaiseExtRuntimeError("Недопустимое значение третьего аргумента метода!", 0);
                return 0;
            };
        };


        const char* sType = ppValue[0]->GetTypeString();
        CString TableName = ppValue[1]->GetString();
        CString Query = "if exists(select * from tempdb..sysobjects where id = object_id('tempdb..%tmp'))\r\n\
            drop table %tmp\r\n\
            create table %tmp (val char(9), isfolder tinyint, primary key clustered (val))";
        Query.Replace("%tmp", TableName);
        Execute(Query);

        Query.Format("insert %s values (?, 2)", TableName);
        Prepare(Query);
        AddParam("", SQL_PARAM_INPUT, SQL_CHAR, CType(0), 9, 0);
        CQueryParam & qp = GetParam(0);

      BOOL bOK = FALSE;
      if (ppValue[0]->GetTypeCode() == AGREGATE_TYPE_1C)
      {
          CBLContext *pCont = ppValue[0]->GetContext();
          if (pCont && !strcmp(pCont->GetRuntimeClass()->m_lpszClassName,"CValueListContext"))
          {
            CPtrArray* pVL=*(CPtrArray**) (((char*)pCont)+0x30);
            int listSize=pVL->GetSize();
            for(int i=0;i<listSize;i++)
            {
	            CValue* pItem=(CValue*)pVL->GetAt(i);
                static_cast<CDBField&>(qp) = m_pMetaData->GetObjDBString(*pItem, CMetaDataWork::TypeStringValue::ShortString);
                Execute(NULL, TRUE);
            }
            bOK = TRUE;
          }
      } 
      else if (ppValue[0]->GetTypeCode() > DATE_TYPE_1C)
      {
        static_cast<CDBField&>(qp) = m_pMetaData->GetObjDBString(*ppValue[0], CMetaDataWork::TypeStringValue::ShortString);
        Execute(NULL, TRUE);
        bOK = TRUE;
      }

      if (!bOK)
      {
          Close();
          CBLModule::RaiseExtRuntimeError("Недопустимое значение первого аргумента метода!", 0);
          return 0;
      }

        if (SbCntTypeDef)
        {
            if (SbCntTypeDef->GetLevelsLimit() > 1)
            {
                CString RefTable;
                RefTable.Format("sc%d", SbCntTypeDef->GetID());
                /*
                Query = "set nocount on\r\n\
                declare @val char(9)\r\n\
                while 1=1 begin\r\n\
                select @val=max(val) from %tmp left join %sc (nolock) on id = val where isfolder = 1\r\n\
                if @val is null break\r\n\
                insert into %tmp select id from %sc (nolock) where parentid=@val and id not in (select val from %tmp)\r\n\
                delete from %tmp where val=@val\r\n\
                end\r\n\
                set nocount off;";
                */

                Query = "set nocount on\r\n\
                update %tmp\
                set\
                  isfolder = 1\
                from %tmp\
                inner join %sc as ref (nolock) on ref.id = %tmp.val\
                where ref.isfolder = 1\
                declare @folders table(val char(9), primary key clustered (val))\
                while 1=1\
                begin\
                  insert into @folders\
                    select val from %tmp where isfolder = 1\
                  if @@rowcount = 0 break\
                  insert into %tmp\
                    select\
                      id,\
                      isfolder\
                    from %sc (nolock)\
                    where parentid in ( select val from @folders )\
                      and id not in ( select val from %tmp )\
                    delete %tmp\
                    where val in ( select val from @folders )\
                    delete @folders\
                end\
                set nocount off";

                Query.Replace("%tmp", TableName);
                Query.Replace("%sc", RefTable);
                if (!Execute(Query))
                {
                    pBkEndUI->DoMessageLine("Internal error:", mmBlackErr);
                    pBkEndUI->DoMessageLine(GetLastError(), mmNone);
                }
            };
        };

        if (PlanDef)
        {
            char sPlanID[20];
            itoa(PlanDef->GetID(), sPlanID, 10);
            Query = "set nocount on\r\n\
            insert %tmp\
            select a2.id, a2.isfolder\
            from %tmp\
            inner join _1saccs a1 (nolock) on a1.id = %tmp.val\
            left join _1saccs a2 (nolock) on left(a2.schkod, len(rtrim(a1.schkod))) = rtrim(a1.schkod)\
            where a2.isfolder = 0 and a2.planid = %pid and a2.id not in (select val from %tmp)\
            \
            delete %tmp\
            from %tmp\
            inner join _1saccs a (nolock) on a.id = %tmp.val\
            where a.isfolder = 1 and a.planid = %pid";
            Query.Replace("%tmp", TableName);
            Query.Replace("%pid", sPlanID);
            if (!Execute(Query))
            {
                pBkEndUI->DoMessageLine("Internal error:", mmBlackErr);
                pBkEndUI->DoMessageLine(GetLastError(), mmNone);
            }
        };
        Close();
      };
      break;
  case procSetQueryTimeout:
      SetQueryTimeout(ppValue[0]->GetNumeric());
      break;
  case procBackFromTAEnabled:
      m_MetaNameParser.BackFromTAEnabled(ppValue[0]->GetNumeric() != 0);
      break;
  case procRPCMode:
      m_MetaNameParser.SetRPCMode(ppValue[0]->GetNumeric() != 0);
      break;
    default:
      {
        CString err;
        err.Format("Нет обработчика метода под номером: %d)", iMethNum);
        CBLModule::RaiseExtRuntimeError(err, 0);
      }
  };
	return 1;
}

int  C1CPP_ODBCRecordSet::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	int ret = 0;
  switch(iMethodNum)
  {
    case funcExecuteXML:
      {
        if (iParamNum == 0)
            *pDefValue = "";
        else
            pDefValue->Reset();
        ret = 1;
      }
      break;
    case funcExecuteScalar:
      {
          if (iParamNum == 0)
          {
              *pDefValue = "";
              ret = 1;
          }
      }
      break;
    case funcExecuteStatement:
      {
        switch (iParamNum)
        {
        case 0:
          *pDefValue = "";
          ret = 1;
          break;
        case 1:
          pDefValue->Reset();
          ret = 1;
          break;
        case 2:
          *pDefValue = 1;
          ret = 1;
          break;
        }
      }
      break;
    case funcExeSQL_FromTV:
      {
        if (iParamNum == 1)
        {
          ret = 1;
          *pDefValue = 0L;
        }
      }
      break;
    case funcGetParam:
      {
        if (iParamNum == 1)
        {
          ret = 1;
          *pDefValue = "";
        }
      }
      break;
    case procSetPropertyCursor:
      {
        ret = 1;
        switch (iParamNum)
        {
          case 0:            
          case 1:
          case 2:
          case 3:          
            *pDefValue = CNumeric(0);
            break;
        }        
      }
      break;
    case funcExecute:
    {
      if (iParamNum == 1)
      {
        ret = 1;
        *pDefValue = CNumeric(0);
      }
      else if (iParamNum == 0)
      {
        ret = 1;
        *pDefValue = CNumeric(0);
      }
    }
    break;
   case procClose:
    {
      if (iParamNum == 0)
      {
         ret = 1;
        *pDefValue = CNumeric(0);
      }
    }
    break;
    case funcAddParam:
      {
        if (iParamNum == 4)
        {
          ret = 1;
          *pDefValue = "";
        }
      }
      break;
    case funcOpen:
      {
        ret = 1;
        switch (iParamNum)
        {
          case 0:
            *pDefValue = "";
            break;
          case 1:
          case 2:
            *pDefValue = CNumeric(0);
            break;
        }        
      }
      break;
    case funcGetInTablesValues:
      {
        if (iParamNum == 1)
        {
          ret = 1;
          *pDefValue = CNumeric(0);
        }
      }
      break;
    case procDebug:
      {
        if (iParamNum == 0)
        {
          ret = 1;
          *pDefValue = CNumeric(1);
        }
      }
      break;
    case procSetParam:
      {
        if (iParamNum == 2)
        {
          ret = 1;
          *pDefValue = CNumeric(0);
        }
      }
      break;
    case funcGetFieldDefs:
      { 
        if (iParamNum >= 1 && iParamNum <= 7)
        {
          ret = 1;
          *pDefValue = CNumeric(0);
        }
      }
      break;
    case procAutoTypification:
      { 
        if (iParamNum == 0)
        {
          ret = 1;
          *pDefValue = CNumeric(1);
        }
      }
      break;
    case procPutObjectList:
      {
        if (iParamNum == 2)
        {
          ret = 1;
          *pDefValue = CString("");
        }
      }
  }
	return ret;
}

int  C1CPP_ODBCRecordSet::FindMethod(char const * lpMethodName) const
{	
	return defFnNames.GetIndexByName(lpMethodName);
}

char const *  C1CPP_ODBCRecordSet::GetMethodName(int iMethodNum,int iMethodAlias)const
{
	return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  C1CPP_ODBCRecordSet::GetNMethods(void)const
{
	return defFnNames.Size();
}

int  C1CPP_ODBCRecordSet::HasRetVal(int iMethodNum)const
{	
	return defFnNames[iMethodNum].HasReturnValue ;
}

int C1CPP_ODBCRecordSet::GetNParams(int iMethodNum)const
{	
  int ret;
  if (iMethodNum == funcGetValues)
    ret = n_nCountUnlimitPars;
  else
	  ret = defFnNames[iMethodNum].NumberOfParams;

  return ret;
}





void  C1CPP_ODBCRecordSet::DecrRef(void)
{
	CBLContext::DecrRef();
}



char const *  C1CPP_ODBCRecordSet::GetCode(void)const
{
	return 0;
}

int  C1CPP_ODBCRecordSet::GetDestroyUnRefd(void)const
{
	return 1;
}

void  C1CPP_ODBCRecordSet::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   C1CPP_ODBCRecordSet::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  C1CPP_ODBCRecordSet::GetInternalData(void)
{

	return CBLContext::GetInternalData();

}

long  C1CPP_ODBCRecordSet::GetTypeID(void)const
{
	return 100;
}

char const *  C1CPP_ODBCRecordSet::GetTypeString(void)const
{
	return "ODBCRecordSet";
}

class CType   C1CPP_ODBCRecordSet::GetValueType(void)const
{
	CType tType(100);
	
	return tType;
}


void  C1CPP_ODBCRecordSet::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  C1CPP_ODBCRecordSet::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  C1CPP_ODBCRecordSet::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  C1CPP_ODBCRecordSet::IsExactValue(void)const
{
	return 0;
}

int  C1CPP_ODBCRecordSet::IsOleContext(void)const
{  
  int regEBX = 0;
  __asm {
    mov regEBX, ebx // В этом регистре 1С хранит количество параметров при вызове метода
  }
  n_nCountUnlimitPars= regEBX; // сохраним это значение  
	return 0;
}

int  C1CPP_ODBCRecordSet::IsPropReadable(int iPropNum)const
{
  if (iPropNum == 0)
    return 1;
	return 0;
}

int  C1CPP_ODBCRecordSet::IsPropWritable(int iPropNum)const
{
	return 0;
}

int  C1CPP_ODBCRecordSet::IsSerializable(void)
{
	return 0;
}

int  C1CPP_ODBCRecordSet::SaveToString(class CString & csStr)
{
	csStr = "";
	return 1;
}

void  C1CPP_ODBCRecordSet::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  C1CPP_ODBCRecordSet::GetNProps(void)const
{
	return 1;
}

char const *  C1CPP_ODBCRecordSet::GetPropName(int A,int B)const
{
  if (A == 0)
    return "md";

	return NULL;
}

int  C1CPP_ODBCRecordSet::GetPropVal(int iPropNum,class CValue & rValue)const
{
  if (iPropNum == 0)
  {    
    rValue.AssignContext(m_pMetaData);
    return 1;
  }
	return -1;
}

int  C1CPP_ODBCRecordSet::SetPropVal(int iPropNum,class CValue const & vValue)
{
	return -1;
}

int  C1CPP_ODBCRecordSet::FindProp(char const * Name)const
{
  if (!stricmp(Name,"md"))
    return 0;
  if (!stricmp(Name,"мд"))
    return 0;

	return -1;
}

/*
void C1CPP_ODBCRecordSet::SetValueOnDBField(CDBField &field, CValue &val, bool bAgg)
{
  CType& rt = field.m_rt;  
  switch( field.m_nCType ) 
  {		        
    case	SQL_C_BIT:
			  val= field.AsBool();
			  break;
	  case	SQL_C_UTINYINT:
			  val = field.AsChar();
			  break;
	  case	SQL_C_SSHORT:
			  val = field.AsShort();
			  break;
	  case	SQL_C_SLONG:
          {
            switch(rt.GetTypeCode())
            {
            case CALCULATIONKIND_TYPE_1C:
            case SUBCONTOKIND_TYPE_1C:
            { //Specially for sql database format
              val.SetType(rt);    		
	          val.SetObjID(CObjID(field.AsLong(),CDBSign("   ")));
            }
              break;
            default:
			  val = field.AsLong();
            };
          }
			  break;
	  case	SQL_C_FLOAT:
		  {	
        val.SetType(CType(1));
			  val = CNumeric(field.AsFloat());
			  break;
		  }
	  case	SQL_C_DOUBLE:		  
      {				                
        val.SetType(CType(1));
			  val = CNumeric(field.AsDouble());        
			  break;
		  }
	  case	SQL_C_TIMESTAMP:
		  {				        
            const TIMESTAMP_STRUCT* pdate = field.AsTimeStamp();
            if (rt.GetTypeCode() == STRING_TYPE_1C)
            {
                CString s;
                if (!field.IsNull())
                s.Format(szODBCDatetimeCanon, pdate->year, pdate->month, pdate->day, pdate->hour, pdate->minute, pdate->second, pdate->fraction);
                val = s;
            }
            else
            {
                if (!field.IsNull())
                val = CDate(pdate->year,pdate->month,pdate->day);
                else
                {
                    val.Reset();
                    val.SetType(CType(3));
                }
            }
		  }
      break;
	  case	SQL_C_CHAR:
		  {
              if (field.m_nSQLType == SQL_NUMERIC)
              {
                  val = field.AsNumeric();
                  break;
          };

          if (field.m_AddType == eStandart)
          {
        int nType = rt.GetTypeCode();
				if (nType == WORD(-1))
					nType = 2;

				if (!bAgg || (nType > 0 && nType < 10))
				{
			    val = field.AsString();
                 break;
             };

             if ((nType == CALCULATIONKIND_TYPE_1C || nType == SUBCONTOKIND_TYPE_1C) && field.m_nPrecision == 4)
                { //Specially for dbf database format
                    val.SetType(rt);    		
	             val.SetObjID(CObjID(strtol(field.AsString(), NULL, 36),CDBSign("   ")));
                    break;
                    };

             CMetaDataWork::TypeStringValue eType;
                    switch (nType)
                    {
                    case UNDEFINE_TYPE_1C:
            eType = CMetaDataWork::VeryLongString;
                        break;
                    case SUBCONTOKIND_TYPE_1C:
                    case CHART_OF_ACC_TYPE_1C:
                        eType = CMetaDataWork::ShortString;
                        break;
                    default:
                        if (rt.GetTypeID() == 0)
                            eType = CMetaDataWork::LongString;
          else
                            eType = CMetaDataWork::ShortString;
              };

              CMetaDataWork::Make1C_ValueFromString(val, rt, field.AsString(), eType);
            }
          else
              SpecialTypification(field, field.AsString(), val);
        }
			  break;
	  case	SQL_C_BINARY:
      {
          if (field.m_nSQLType == SQL_LONGVARBINARY)
          {
			  val.CreateObject("BinaryData");
              static_cast<CBinaryData*>(val.GetContext())->AttachToIStream(field.AsIStream());
          }
          else if (field.m_nSQLType == SQL_BINARY)
          {
              if (field.m_nPrecision == sizeof(GUID))
              {
		          val.CreateObject("GUID");
                  CGUID* pGUID = static_cast<CGUID*>(val.GetContext());
                  if (!field.m_bIsNull)
                      pGUID->Set(field.AsGUID());
              }
          }
      }
			  break;
	  case	SQL_C_NUMERIC:
          val = field.AsNumeric();
          break;
      case	SQL_C_TYPE_TIME:
          val = field.AsString();
          break;
	  default:
		  ASSERT( FALSE );
	}            

    if (field.m_BLCTypeName.GetLength())
    {
        CreateBLCValue(field, val);
    }
}

void C1CPP_ODBCRecordSet::SetValueOfRow(BYTE *Rowset, CDBField &dbf, int Index, CValue &rVal, bool bAgg)
{
  BYTE* pData = &Rowset[GetOffset(Index)];    
  BYTE* p = pData + dbf.m_nBufSize;  
  CType& rt = dbf.m_rt;

  if (*(SQLINTEGER*)p != SQL_NULL_DATA)
  {    		
    
		switch( dbf.m_nCType ) {
		case	SQL_C_BIT:
      {      
			  rVal = *(bool*)pData;
      }
			break;
		case	SQL_C_UTINYINT:
      {      
			  rVal = *(UCHAR*)pData;
      }
			break;

		case	SQL_C_SSHORT:
      {      
			  rVal = *(short*)pData;
      }
			break;

		case	SQL_C_SLONG:
      {        
            switch (rt.GetTypeCode())
            {
            case CALCULATIONKIND_TYPE_1C:
            case SUBCONTOKIND_TYPE_1C:
            { //Specially for sql database format
              rVal.SetType(rt);    		
	          rVal.SetObjID(CObjID(*(long*)pData,CDBSign("   ")));
            }
                break;
            default:
			  rVal = *(long*)pData;
            };
      }
			break;
		case	SQL_C_FLOAT:
			{      
			  
        rVal.SetType(CType(1));
			  rVal = CNumeric(*(float*)pData);       
				break;
			}

		case	SQL_C_DOUBLE:
			{                 
        rVal.SetType(CType(1));
	  		rVal = CNumeric(*(long double*)pData);        
				break;
			}

		case	SQL_C_TIMESTAMP:
			{				                             
        TIMESTAMP_STRUCT* pdate = (TIMESTAMP_STRUCT*)pData;                
            if (dbf.m_rt.GetTypeCode() == STRING_TYPE_1C)
            {
                CString s;
                s.Format(szODBCDatetimeCanon, pdate->year, pdate->month, pdate->day, pdate->hour, pdate->minute, pdate->second, pdate->fraction);
                rVal = s;
            }
            else
                rVal = CDate(pdate->year,pdate->month,pdate->day);
			}
      break;

		case	SQL_C_CHAR:
			{			        
              if (dbf.m_nSQLType == SQL_NUMERIC)
              {
                  CNumeric num;
                  rVal = num.FromString((const char*)pData, NULL);
                  break;
          };

          if (dbf.m_AddType == eStandart)
          {
          int nType = rt.GetTypeCode();
				  if (nType == WORD(-1))
					  nType = 2;

				  if (!bAgg || (nType > 0 && nType < 10))
				  {
					  rVal = (const char*)pData;
                break;
            };

             if ((nType == CALCULATIONKIND_TYPE_1C || nType == SUBCONTOKIND_TYPE_1C) && dbf.m_nPrecision == 4)
                { //Specially for dbf database format
                    rVal.SetType(rt);    		
	             rVal.SetObjID(CObjID(strtol((const char*)pData, NULL, 36),CDBSign("   ")));
                    break;
                    };

             CMetaDataWork::TypeStringValue eType;
                    switch (nType)
                    {
                    case UNDEFINE_TYPE_1C:
              eType = CMetaDataWork::VeryLongString;
                        break;
                    case SUBCONTOKIND_TYPE_1C:
                    case CHART_OF_ACC_TYPE_1C:
                        eType = CMetaDataWork::ShortString;
                        break;
                    default:
                        if (rt.GetTypeID() == 0)
                            eType = CMetaDataWork::LongString;
            else
                            eType = CMetaDataWork::ShortString;
              };

              CMetaDataWork::Make1C_ValueFromString(rVal, rt, (const char*)pData, eType);
				  }   
          else
              SpecialTypification(dbf, (const char*)pData, rVal);
        }        
				break;
		case	SQL_C_BINARY:
				rVal.Reset();
				break;
	    case	SQL_C_NUMERIC:
          {
            CSQLNumeric Num;
            Num.Load((SQL_NUMERIC_STRUCT *)pData);
            rVal = Num;
          }
          break;
        case	SQL_C_TYPE_TIME:
          {
            SQL_TIME_STRUCT* ptime = (SQL_TIME_STRUCT*)pData;
            CString s;
            s.Format(szODBCTimeCanon, ptime->hour, ptime->minute, ptime->second);
            rVal = s;
          }
          break;
		default:
			ASSERT( FALSE );
		}
	}
  else
  { 
    rVal.Reset();
    switch( dbf.m_nCType ) 
    {
      case	SQL_C_BIT:
      case	SQL_C_UTINYINT:      
		  case	SQL_C_SSHORT:    
		  case	SQL_C_SLONG:      
		  case	SQL_C_FLOAT:			
		  case	SQL_C_DOUBLE:
			  {                 			
          rVal = 0L; 				
			  }
      break;
      case	SQL_C_TIMESTAMP:
        {
          rVal.SetType(CType(3));
        }
      break;
      case SQL_C_CHAR:
        {
          
          int nType = rt.GetTypeCode();
				  if (nType == WORD(-1))
				  {
					  rVal.SetType(CType(2));
				  }	
          else
            rVal.SetType(dbf.m_rt);
        }      
    }    
  };

  if (dbf.m_BLCTypeName.GetLength())
  {
      CreateBLCValue(dbf, rVal);
  }
}
*/
/*
bool C1CPP_ODBCRecordSet::SetValueOfParam(int nParam, const CValue &vSource, const int Modificator)
{
  CDBField& qp = static_cast<CDBField&>(GetParam(nParam));
  bool ret = true;

  if (Modificator == 0)
  {
  switch(vSource.GetTypeCode())
  {
    case NUMBER_TYPE_1C:
      {        
        qp = (double)vSource.GetNumeric().GetDouble();
      }
      break;
    case DATE_TYPE_1C:
      {
        const CDate &date = vSource.GetDate();
			  qp = COleDateTime(date.GetYear(),date.GetMonth(),date.GetMonthDay(),0,0,0);
      }
      break;
        case STRING_TYPE_1C:
          {
            qp = vSource.GetString();
          }
          break;
    case AGREGATE_TYPE_1C:
      {
          switch (qp.m_nSQLType)
          {
          case SQL_LONGVARBINARY:
            {
              CBinaryData* pBinaryData = NULL;
              CBLContext* pCont = vSource.GetContext();
              CString ClassName(pCont->GetRuntimeClass()->m_lpszClassName);
		      if (ClassName == "CBinaryData")
                  pBinaryData = static_cast<CBinaryData*>(pCont);
              else if (ClassName == "CComponentClass")
		      {
                  CValue ClassName("CBinaryData"), vBD;
                  if (static_cast<CComponentClass*>(pCont)->GetBaseClass(ClassName, vBD))
                      pBinaryData = static_cast<CBinaryData*>(vBD.GetContext());
              }

              if (pBinaryData)
                qp = pBinaryData->GetIStream();
              else
                qp = (IStream*)NULL;
            }
            break;
          case SQL_BINARY:
            {
              if (!strcmp(vSource.GetContext()->GetRuntimeClass()->m_lpszClassName, "CDBGUID"))
              {
                CGUID* pGUID = static_cast<CGUID*>(vSource.GetContext());
                if (qp.m_nPrecision == sizeof(GUID))
                {
                    GUID g;
                    pGUID->Get(&g);
                    qp = &g;
                }
              }
            }
            break;
          }
      }
      break;
    default:
      {
        CString Str;
        bool IsStrLit;
        ret = CMetaNameParser::CValueToDBValue(vSource, Modificator, Str, IsStrLit, CString());
        if (IsStrLit)
          qp = Str;
      }
  }                   
  }
  else
  {
    CString Str;
    bool IsStrLit;
    ret = CMetaNameParser::CValueToDBValue(vSource, Modificator, Str, IsStrLit, CString());
    qp = Str;
  }

  return ret;
}
*/
CString C1CPP_ODBCRecordSet::GetField_1CType(CDBField & Field, int & Length, int & Scale)
{
    Length = 0;
    Scale = 0;
    CType & t = Field.m_rt;
    int TypeCode = t.GetTypeCode();

    if (TypeCode < 0 || TypeCode > 17) {
        switch(Field.m_nSQLType) {
        case SQL_BIT:
            Length = 1;
            return "Число";
            break;

        case SQL_TINYINT:
            Length = 3;
            return "Число";
            break;

        case SQL_SMALLINT:
            Length = 5;
            return "Число";
            break;

        case SQL_INTEGER:
            Length = 10;
            return "Число";
            break;

        case SQL_REAL:
        case SQL_FLOAT:
        case SQL_DOUBLE:
            return "Число";
            break;
        case SQL_NUMERIC:
            if (Field.m_nScale)
                Length = Field.m_nPrecision + 1;
            else
                Length = Field.m_nPrecision;
            Scale = Field.m_nScale;
            return "Число";
            break;

        case SQL_DATE:
        case SQL_TIMESTAMP:
        case SQL_TYPE_DATE:
        case SQL_TYPE_TIMESTAMP:
            return "Дата";
            break;
        case SQL_TIME:
        case SQL_TYPE_TIME:
            return "Строка";
            break;

        case SQL_DECIMAL:
            if (Field.m_nScale)
                Length = Field.m_nPrecision + 1;
            else
                Length = Field.m_nPrecision;
            Scale = Field.m_nScale;
            return "Число";
            break;

        case SQL_BIGINT:
        case SQL_CHAR:
            Length = Field.m_nPrecision;
            return "Строка";
            break;
        case SQL_VARCHAR:
        case SQL_LONGVARCHAR:
        case SQL_WCHAR:
        case SQL_WVARCHAR:
        case SQL_WLONGVARCHAR:
        case SQL_BINARY:
        case SQL_VARBINARY:
        case SQL_LONGVARBINARY:
        default:
            return "Строка";
        }
    }
    else
    {
        CString Type;
        Type = t.GetTypeTitle();
        if (TypeCode >= 10)
        {
          CMetaDataObj* Obj = pMetaDataCont->FindObject(t.GetTypeID());
          if (Obj)
          {
            Type += '.';
            Type += Obj->GetCode();
          }
        }
        //pBkEndUI->DoMessageLine(Type, mmExclamation);	

        Length = t.GetLength();
        Scale = t.GetPrecision();
        return Type;
    }
};

/*
template CTypedFldDefsArray<CSbKindDef>;

void C1CPP_ODBCRecordSet::SpecialTypification(CDBField & field, const char * strObjID, CValue & val)
{
    switch (field.m_AddType)
    {
      case eDocKind:
        if (field.m_AddFieldIndex >= 0)
        {
          CDBField &KindField = operator[](field.m_AddFieldIndex);
          long KindID;
          if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_SQL)
              KindID = KindField.AsLong();
          else
              KindID = strtol(KindField.AsString(), NULL, 36);

          CMetaDataWork::Make1C_ValueFromString(val, CType(DOCUMENT_TYPE_1C, KindID), strObjID, CMetaDataWork::ShortString);
        }
        break;
      case eSbKind:
        if (field.m_AddFieldIndex >= 0)
        {
          CDBField &KindField = operator[](field.m_AddFieldIndex);
          long KindID;
          if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_SQL)
              KindID = KindField.AsLong();
          else
              KindID = strtol(KindField.AsString(), NULL, 36);

          CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
          if (pBuhDef)
          {
              CSbKindDef* pSbKindDef = pBuhDef->GetSbKindDefs()->GetItem(KindID);
              if (pSbKindDef)
              {
                  CMetaDataWork::TypeStringValue eType;
                  if (pSbKindDef->m_Kind == 0)
                      eType = CMetaDataWork::LongString;
                  else
                      eType = CMetaDataWork::ShortString;
                  CMetaDataWork::Make1C_ValueFromString(val, CType(pSbKindDef->m_TypeCode, pSbKindDef->m_Kind), strObjID, eType);
              };
          }
        }
        break;
    };
};

void C1CPP_ODBCRecordSet::CreateBLCValue(CDBField & field, CValue & val)
{
    int err = 0;
    CValue vBLCParam(val);
    if (val.CreateObject(field.m_BLCTypeName))
    {
        CBLContext* pCont = val.GetContext();
        if (pCont)
        {
            int nMeth = pCont->FindMethod("_SQLCreate");
            if (nMeth >= 0)
            {
                if (pCont->GetNParams(nMeth) <= 2)
                {
                    CValue MW;
                    MW.AssignContext(m_pMetaData);
                    CValue* ppVal[2] = {&vBLCParam, &MW};
                    if (!pCont->CallAsProc(nMeth, ppVal))
                        err = 5;
                }
                else
                    err = 4;
            }
            else
                err = 3;
        }
        else
            err = 2;
    }
    else
        err = 1;

    if (err)
    {
        CString sErr;
        sErr.Format("ODBCRecordSet field:%s udf:%s error:", field.m_strName, field.m_BLCTypeName);
        switch(err)
        {
        case 1:
            sErr += "failed to create object";
            break;
        case 2:
            sErr += "wrong class name";
            break;
        case 3:
            sErr += "failed to find v_ctor (_SQLCreate)";
            break;
        case 4:
            sErr += "v_ctor params count must be <= 2";
            break;
        case 5:
            sErr += "failed to execute v_ctor";
            break;
        };
        pBkEndUI->DoMessageLine(sErr, mmBlackErr);
    }
}CAdminService
*/

BOOL C1CPP_ODBCRecordSet::TypifyField(int nIndex, CDBField& field)
{
    if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
        if (field.m_strName.Replace(TypeNamePrefix, "$"))
            field.m_strName.Replace('_', '.');

    if (!CODBCRecordset::TypifyField(nIndex, field))
        return FALSE;

	if (m_bAutoTypifycation && field.m_strName.GetLength() > 2)
	{
		if ((field.m_strName[0] == 'S' && field.m_strName[1] == 'P') || (field.m_strName[0] == 's' && field.m_strName[1] == 'p'))
		{
			CMetaDataObj* Obj = pMetaDataCont->FindObject(atol((LPCTSTR) field.m_strName + 2));
			if (Obj)
			{
				field.m_strName = Obj->GetCode();
				if (Obj->IsTypedObj())
					field.m_rt = static_cast<CMetaDataTypedObj*>(Obj)->GetType();
			}
		}
    }
    return TRUE;
}

CBLContext* C1CPP_ODBCRecordSet::GetVCtorParam()
{
    return m_pMetaData;
}
