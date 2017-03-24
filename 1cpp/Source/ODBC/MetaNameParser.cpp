// MetaNameParser.cpp: implementation of the CMetaNameParser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MetaNameParser.h"
#include "Math.h"
#include "../SQL/MetaDataWork.h"
#include "../GUID.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define INS_ROWSET_SIZE 50

const char BeginMetaNameSimbol = '$';
const char BeginParamSimbol = ':';
const char ModificatorSymbol = '~';
const char ModificatorSymbolUndefType = '*';
const char SQLParamSymbol = '?';
const char* NameOfPositionField = "date_time_iddoc";

//////////////////////////////////////////////////////////////////////
// CMetaNameParser

extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;
extern CDataBase7 * pDataBase7;
class CIBSet NativeIBSet;

const char* arJoinesPS[] = {NULL, "\r\n"};
const char* arConditionsPS[] = {"and (", ")\r\n"};
#define Separator 0x01

CMetaNameParser::~CMetaNameParser()
{
  SQLParams.RemoveAll();
  OnClose();
  ClearParams();
};

const enum mnSubTypes {mnTable, mnField, mnId, mnSpecial, mnVirtualTable, mnVirtualValue};

const struct stNames {  
  const char * Names[2];
  mnSubTypes mnSubType;
} TableNames[] = {
  {{"Справочник", "Reference"}, mnTable},
  {{"Документ", "Document"}, mnTable},
  {{"ДокументСтроки", "DocumentStrings"}, mnTable},
  {{"Регистр", "Register"}, mnTable},
  {{"РегистрИтоги", "RegisterTotals"}, mnTable},
  {{"ЖурналРасчетов", "CalcJournal"}, mnTable},
  
  {{"ВидДокумента", "DocumentKind"}, mnId},
  {{"Константа", "Constant"}, mnId},
  {{"ИсторияРеквизита", "PropertyHistory"}, mnSpecial},
  {{"ОбщийРеквизит", "CommonProperty"}, mnField},
  {{"ФлагРегистра", "RegisterFlag"}, mnField},
  {{"Последовательность", "Sequence"}, mnField},
  {{"ГрафаЖурналаРасчетов", "ColumnCalcJournal"}, mnSpecial},
  {{"ЖурналДокументов", "DocsJournal"}, mnId},
  {{"ГрафаОтбора", "SelectionColumn"}, mnId},
  {{"Календарь", "Calendar"}, mnId},
  {{"Праздники", "Holidays"}, mnSpecial},
  {{"ВидЖурналаРасчетов", "CalcJournalKind"}, mnId},
  {{"ВидРасчета", "Algorithm"}, mnId},
  {{"ВидДокумента36", "DocumentKind36"}, mnId},
  {{"ВидСправочника36", "ReferenceKind36"}, mnId},
  {{"ВидСправочника", "ReferenceKind"}, mnId},
  {{"ВидПеречисления36", "EnumKind36"}, mnId},
  {{"Перечисление", "Enumeration"}, mnSpecial},
  {{"Перечисление23", "Enumeration23"}, mnSpecial},
  
  {{"РегистрОстатки", "RegisterRests"}, mnVirtualTable},
  {{"РегистрОбороты", "RegisterTurnovers"}, mnVirtualTable},
  {{"РегистрОстаткиОбороты", "RegisterRestsTurnovers"}, mnVirtualTable},
  
  {{"ПоследнееЗначение", "LastValue"}, mnVirtualValue},
  {{"НачалоПериода", "BeginOfPeriod"}, mnVirtualValue},

  {{"ПустойИД", "EmptyID"}, mnSpecial},
  {{"ПустойИД13", "EmptyID13"}, mnSpecial},

  {{"Проводка", "Entry"}, mnField} // spock: 2006-01-29
};

const char * ExceptStr[] = {
  "незавершённое метаимя \"%s\"",
    "неизвестное метаимя или алиас \"%s\"",
    "не указан параметр \"%s\"",
    "объект не найден \"%s\"",
    "поле таблицы не найдено \"%s\"",
    "недопустимое значение параметра \"%s\" ",
    "слишком много параметров передано в \"%s\"",
    "виртуальная таблица не найдена \"%s\"",
    "модификатор не определен для типа \"%s\"",
    "виртуальное поле не найдено \"%s\"",
    "имя SQL параметра должно быть уникальным или пустым \"%s\"",
    "это метаимя разрешить невозможно \"%s\"",
    "не определены измерения \"%s\"",
    "не определены ресурсы \"%s\"",
};

const enum ExceptId {
  eiUncomplMN,
    eiUnknownMN,
    eiParamNotSpecifed,
    eiObjectNotFound,
    eiFieldOfObjectNotFound,
    eiWrongParam,
    eiTooManyParam,
    eiVirtTableNotFound,
    eiModNotSpecifed,
    eiVirtualFieldNotFound,
    eiWrongSQLParamName,
    eiImpossible,
    eiDimsNotDef,
    eiFigsNotDef,
};

CIStringMap<int, int> CMetaNameParser::MetaNames;
void CMetaNameParser::InitParser()
{
    if (MetaNames.IsEmpty())
    {
        MetaNames.InitHashTable(LastType * 1.2);
        for (int i = 0; i < LastType; i++)
        {
            const stNames& pMetaName = TableNames[i];
            MetaNames[pMetaName.Names[0]] = i;
            MetaNames[pMetaName.Names[1]] = i;
        };
    };
};

CMetaNameParser::CMetaNameParser(): m_db(NULL), m_BackFromTAEnabled(false), m_RPCMode(false), m_bParseSQLParam(true)
{
    InitParser();
    m_pMetaDataCont = pMetaDataCont;
    m_pIBSet = &NativeIBSet;
};

CMetaNameParser::CMetaNameParser(CODBCDatabase* db): m_db(db), m_BackFromTAEnabled(false), m_RPCMode(false), m_bParseSQLParam(true)
{
    InitParser();
    m_pMetaDataCont = db->m_pMetaDataCont;
    m_pIBSet = db->m_pCurrentIBSet;
};

void CMetaNameParser::SetQueryText(const char* Text)
{
  QueryText = Text;
};

const char* CMetaNameParser::GetQueryText() const
{
  return QueryText;
};

int CMetaNameParser::FindModificator()
{
  int Modificator = 0;
  
  bool fComment = false;
  bool fCommentType;
  
  char CurChar;
  const char * pQT = (LPCSTR) QueryText + CurIndex;
  const char * begin = pQT;
  const char * end = pQT;
  
  while (CurChar = *pQT++) 
  {
    if (fComment) {//идет комментарий
      if (fCommentType)
        fComment = (CurChar != '\n');
      else
        fComment = !((CurChar == '*') && (*pQT == '/'));
      continue;
    };
    switch (CurChar) {
      case '-': //комментарий типа --
        if (*pQT == '-') {
          fComment = true;
          fCommentType = true;
        }
        break;
      case '/': //комментарий типа /*
        if (*pQT == '*') 
        {
          fComment = true;
          fCommentType = false;
        }
        break;
      case ModificatorSymbol: 
      {
        Modificator++;
        end = pQT;
        continue;
      };
      break;
    };
    if (CurChar == ModificatorSymbolUndefType) {
      Modificator = UNDEF_TYPE_MODIFIKATOR;
      end = pQT;
      break;
    };
    if (!strchr(" \r\n\t", CurChar))
      break;
  };
  
  CurIndex += end - begin;
  
  return Modificator;
};

bool CMetaNameParser::CValueToDBValue(const CValue & Value, const int Modificator, CString & StrValue, bool & IsStrLiteral, CString& SQLType)
{
    StrValue.Empty();
    IsStrLiteral = true;
    bool ret = true;
    if (Modificator == UNDEF_TYPE_MODIFIKATOR)
        SQLType = "char(23)";

    if (Value.GetTypeCode() >= ENUM_TYPE_1C && Value.GetTypeCode() <= CHART_OF_ACC_TYPE_1C && (Modificator >=0 && Modificator <=2 || Modificator == UNDEF_TYPE_MODIFIKATOR))
    {
        switch (Modificator)
        {
        case 0:
            StrValue = CMetaDataWork::GetObjDBString(Value, CMetaDataWork::ShortString);
            SQLType = "char(9)";
            break;
        case 1:
            StrValue = CMetaDataWork::GetObjDBString(Value, CMetaDataWork::LongString);
            SQLType = "char(13)";
            break;
        case 2:
        case UNDEF_TYPE_MODIFIKATOR:
            StrValue = CMetaDataWork::GetObjDBString(Value, CMetaDataWork::VeryLongString);
            break;
        }
        return true;
    }
    
    switch(MAKELONG(Value.GetTypeCode(), Modificator))
    {
    case MAKELONG(UNDEFINE_TYPE_1C, UNDEF_TYPE_MODIFIKATOR):
        StrValue = "U                      ";
        break;
    case MAKELONG(UNDEFINE_TYPE_1C, 0):
        StrValue = "NULL";
        SQLType = "char(9)"; //???
        IsStrLiteral = false;
        break;

    case MAKELONG(NUMBER_TYPE_1C, UNDEF_TYPE_MODIFIKATOR):
        StrValue.Format("N%22s", Value.GetString());
        break;
    case MAKELONG(NUMBER_TYPE_1C, 0):
        {
            StrValue = Value.GetString();
            int Len = StrValue.GetLength();
            int Prec = 0;
            int DotPos = StrValue.Find('.');
            if (DotPos != -1)
            {
                Len--;
                Prec = Len - DotPos;
            };
            if (Len > 38)
                Len = 38;
            if (Prec > Len)
                Prec = Len;
            SQLType.Format("numeric(%u,%u)", Len, Prec);
            IsStrLiteral = false;
        }
        break;

    case MAKELONG(STRING_TYPE_1C, UNDEF_TYPE_MODIFIKATOR):
        StrValue.Format("S%-22s", Value.GetString());
        break;
    case MAKELONG(STRING_TYPE_1C, 0):
        StrValue = Value.GetString();
        SQLType.Format("char(%u)", max(1, StrValue.GetLength()));
        break;
    case MAKELONG(STRING_TYPE_1C, 1):
        try
        {
            char TimeBuff[7];
            char IdBuff[7];
            CString Pos = Value.GetString();
            LongToCharID36(atol(Pos.Mid(10, 9)), TimeBuff, 6);
            TimeBuff[6] = 0;
            LongToCharID36(atol(Pos.Mid(19, 10)), IdBuff, 6);
            IdBuff[6] = 0;
            StrValue.Format("%8s%6s%6s%3s", Pos.Mid(1, 8), TimeBuff, IdBuff, Pos.Right(3));
            SQLType = "char(23)";
        }
        catch (...)
        {
            StrValue = "NULL";
            IsStrLiteral = false;
        }
        break;

    case MAKELONG(DATE_TYPE_1C, UNDEF_TYPE_MODIFIKATOR):
        {
            CDate dat = Value.GetDate();
            if (dat.GetYear() == 0)
                StrValue = "D                      ";
            else
                StrValue.Format("D%04u%02u%02u              ", dat.GetYear(), dat.GetMonth(), dat.GetMonthDay());
        }
        break;
    case MAKELONG(DATE_TYPE_1C, 0):
        {
            CDate dat = Value.GetDate();
            StrValue.Format("%04u%02u%02u", dat.GetYear(), dat.GetMonth(), dat.GetMonthDay());
            SQLType = "char(8)";
        }
        break;
    case MAKELONG(DATE_TYPE_1C, 1):
        {
            CDate dat = Value.GetDate();
            StrValue.Format("%04u%02u%02uZ", dat.GetYear(), dat.GetMonth(), dat.GetMonthDay());
            SQLType = "char(9)";
        }
        break;
    case MAKELONG(DATE_TYPE_1C, 2):
        {
            CDate dat = Value.GetDate();
            StrValue.Format("{d '%04u-%02u-%02u'}", dat.GetYear(), dat.GetMonth(), dat.GetMonthDay());
            //SQLType = "smalldatetime";
            SQLType = "datetime";
            IsStrLiteral = false;
        }
        break;

    case MAKELONG(CALCULATIONKIND_TYPE_1C, 2):
    case MAKELONG(SUBCONTOKIND_TYPE_1C, 2):
    case MAKELONG(REFERENCE_TYPE_1C, 3):
    case MAKELONG(DOCUMENT_TYPE_1C, 3):
    case MAKELONG(ACCOUNT_TYPE_1C, 3):
        if (pDataBase7->GetDataSourceType() == DATA_SOURCE_TYPE_SQL)
        {
            StrValue.Format("%u", Value.GetRealTypeID());
            SQLType = "int";
            IsStrLiteral = false;
        }
        else
        {
            LongToCharID36(Value.GetRealTypeID(), StrValue.GetBufferSetLength(4), 4);
            StrValue.ReleaseBuffer();
            SQLType = "char(4)";
        }
        break;

    case MAKELONG(DOCUMENT_TYPE_1C, 4):
        {
			CEventTime EventTime;
			CDate Date(0,0,0);
			GetDateTimeFromValue(Value, Date, EventTime);
			StrValue = GetStringDateTimeIddoc(Date, EventTime.GetlTime(), Value.GetObjID());
            SQLType = "char(23)";
        }
        break;

    case MAKELONG(AGREGATE_TYPE_1C, 0):
        {
            CBLContext* pCont = Value.GetContext();
            if (pCont)
            {
                if (pCont->GetRuntimeClass() ==  RUNTIME_CLASS(CGUID))
                {
                    IsStrLiteral = false;
                    static_cast<CGUID*>(pCont)->GetBinaryString(StrValue);
                    SQLType = "binary(16)";
                }
            }
        }
        break;
    default:
        IsStrLiteral = false;
        ret = false;
        break;
    }
  return ret;
};

CString CMetaNameParser::CValueToSQLLiteral(const CValue & Value, const int Modificator, CString& SQLType)
{
  CString ret;
  bool IsStrLiteral;
  if (!CValueToDBValue(Value, Modificator, ret, IsStrLiteral, SQLType))
    RaiseException(ExceptStr[eiModNotSpecifed]);
  if (IsStrLiteral)
  {
    ret.Insert(0, '\'');
    ret += '\'';
  };
  return ret;
};

void CMetaNameParser::Parse()
{
  if (QueryText.IsEmpty())
    return;
  
  OnClose();
  
    Aliases.RemoveAll();

    //Первый проход по текстовым параметрам
    CurIndex = 0;
    CString Param;
    CValue Value;
    while (GetNextSource(BeginParamSimbol))
    {
        Start = CurIndex;
        GetIdentificator(Param);
        if (Param.IsEmpty())
            continue;
        if (Params.Lookup(Param, Value))
        {
            int Modifikator = FindModificator();
            if (m_RPCMode)
            {
                CString varName('@');
                varName += Param;
                varName += '0' + Modifikator;

                PasteName(varName);

                int index;
                if (m_UseTextParamsInRPC.Lookup(varName, (void*&)index))
                    m_RPCParams[index].Count++;
                else
                {
                    stRPCParam RPCParam;
                    RPCParam.VarName = varName;
                    RPCParam.Count = 1;
                    RPCParam.Value = CValueToSQLLiteral(Value, Modifikator, RPCParam.Type);
                    m_RPCParams.Add(RPCParam);
                    m_UseTextParamsInRPC[varName] = (void*)(m_RPCParams.GetSize() - 1);
                };
            }
            else
            {
                PasteName(CValueToSQLLiteral(Value, Modifikator, CString()));
            };
        }
        else
            RaiseException(ExceptStr[eiParamNotSpecifed]);
    };
  
    //Второй и третий
    ParseMetaNames();

   //Четвертый проход по SQL параметрам
    if (m_bParseSQLParam)
    {
        SQLParams.RemoveAll();
        CurIndex = 0;
        int Number = 1;
        int EmptyNumber = 1;
        SQLParam CurParam;
        while (GetNextSource(SQLParamSymbol))
        {
            Start = CurIndex;
            GetIdentificator(Param);
            if (Param.IsEmpty())
                Param.Format("_%d", EmptyNumber++);
            if (SQLParams.Lookup(Param, CurParam))
                RaiseException(ExceptStr[eiWrongSQLParamName]);
            CurParam.Number = Number++;
            CurParam.Modificator = FindModificator();
            SQLParams[Param] = CurParam;
            PasteName(CString(SQLParamSymbol));
        };
    }
  
  if (m_pIBSet->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
    ProcessForFox();

    if (m_RPCMode)
    {
        CurIndex = 0;
        GetNextSource('\'');

        QueryText.Insert(0, "{call sp_executesql(N'");
        QueryText += '\'';
        CString qtTypes, qtValues;
        
        for (int i = 0; i < m_RPCParams.GetSize(); i++)
        {
            stRPCParam& RPCParam = m_RPCParams[i];
            
            if (RPCParam.Count)
            {
                if (!qtTypes.IsEmpty())
                    qtTypes += ',';
                qtTypes += RPCParam.VarName;
                qtTypes += ' ';
                qtTypes += RPCParam.Type;

                qtValues += ',';
                qtValues += RPCParam.Value;
            };
        };
        if (!qtTypes.IsEmpty())
        {
            QueryText += ", N'";
            QueryText += qtTypes;
            QueryText += "'\r\n";
            QueryText += qtValues;
        };
        QueryText += ")}";
    };
  
  Aliases.RemoveAll();
  ClearParams();
};

void CMetaNameParser::ParseMetaNames()
{

 //Проход по метаименам с заполнением таблицы алиасов
  TableAliases TA;
  CurIndex = 0;
  CString Name;
  while (GetNextSource(BeginMetaNameSimbol))
  {
    Start = CurIndex;
    CString TableType;
    GetIdentificator(TableType);
    int MetaNameType;
    if (MetaNames.Lookup(TableType, MetaNameType))
    {
      Name.Empty();
      CMetaDataObj * pMetaDataObj;
      bool DoFindeAlias = ResolveFirstLevelMetaName(MetaNameTypes(MetaNameType), Name, &pMetaDataObj);
      if (!Name.IsEmpty())
      {
        PasteName(Name);
        if (DoFindeAlias)
        {
          CString Alias;
          if (FindeAlias(Alias))
          { //Есть алиас
            TA.TableType = MetaNameTypes(MetaNameType);
            TA.Table = pMetaDataObj;
            Aliases[Alias] = TA;
          }
        }
      }
    }
  };
  
  //Проход по метаименам с использованием таблицы алиасов
  CurIndex = 0;
  CString Alias;
  while (GetNextSource(BeginMetaNameSimbol))
  {
    Start = CurIndex;
    GetIdentificator(Alias);
    if (Aliases.Lookup(Alias, TA)) {
      if (IsNext()) {
        ResolveSecondLevelMetaName(TA.TableType, TA.Table, Name);
        PasteName(Alias + '.' + Name);
      }
      else
        RaiseException(ExceptStr[eiUncomplMN]);
    }
    else
      RaiseException(ExceptStr[eiUnknownMN]);
  };
};

bool CMetaNameParser::ResolveFirstLevelMetaName(const MetaNameTypes MetaNameType, CString & Name, CMetaDataObj * * ppMetaDataObj)
{
  bool OneName = true;
  switch (MetaNameType)
  {
  case ntHolidays:
    Name.Format("%d", m_pMetaDataCont->GetHolidaysDef()->GetID());
    break;
  case ntEmptyRef:
    Name = "'     0   '";
    break;
  case ntEmptyRef13:
    Name = "'   0     0   '";
    break;
  default:
      OneName = false;
  };
  
  if (OneName)
      return false;

  if (!IsNext())
    RaiseException(ExceptStr[eiUncomplMN]);
  
  bool DoFindeAlias = false;
  CString TableKinde;
  GetIdentificator(TableKinde);
  if (TableKinde.IsEmpty())
    RaiseException(ExceptStr[eiUncomplMN]);
  
  switch (TableNames[MetaNameType].mnSubType) {
  case mnTable: {
    const char * Prefix;
    switch (MetaNameType) {
    case ntReference: {
      Prefix = "sc";
      break;
                      }
    case ntDocument: {
      Prefix = "dh";
      break;
                     }
    case ntDocumentStrings: {
      Prefix = "dt";
      break;
                            }
    case ntRegistr: {
      Prefix = "ra";
      break;
                    }
    case ntRegistrTotals: {
      Prefix = "rg";
      break;
                          }
    case ntCalcJournal: {
      Prefix = "cj";
      break;
                        }
    }
    *ppMetaDataObj = GetMetaDataId(MetaNameType, TableKinde);
    if (IsNext())
      ResolveSecondLevelMetaName(MetaNameType, *ppMetaDataObj, Name);
    else {
      DoFindeAlias = true;
      Name.Format("%s%d", Prefix, (*ppMetaDataObj)->GetID());
    }
    break;
                }
  case mnField: {
    const char * Prefix;
    MetaNameTypes ntRes = MetaNameType;
    switch (MetaNameType) {
    case ntCommonProp:
	case ntEntry:// spock: 2006-01-29
		{
			Prefix = "sp";
			break;
		}
    case ntRegisterFlag:
		{
			Prefix = "rf";
			ntRes = ntRegistr;
			break;
		}
	case ntSequence:
		{
			Prefix = "ds";
			break;
		}
	}
    Name.Format("%s%d", Prefix, GetMetaDataId(ntRes, TableKinde)->GetID());
    break;
                }
  case mnId:
    {
      ULONG id;
      bool bAsChar = m_pIBSet->GetDataSourceType() == DATA_SOURCE_TYPE_DBF;
      switch (MetaNameType)
      {
		case ntDocumentKind:
          id = GetMetaDataId(ntDocument, TableKinde)->GetID();
          break;
        case ntAlgorithm:
          id = GetMetaDataId(ntAlgorithm, TableKinde)->GetID();
          break;
        case ntDocumentKind36:
          id = GetMetaDataId(ntDocument, TableKinde)->GetID();
          bAsChar = true;
          break;
        case ntReferenceKind36:
          id = GetMetaDataId(ntReference, TableKinde)->GetID();
          bAsChar = true;
          break;
        case ntReferenceKind:
          id = GetMetaDataId(ntReference, TableKinde)->GetID();
          bAsChar = false;
          break;
        case ntEnumKind36:
          {
            CEnumDef *pEnums = m_pMetaDataCont->GetEnumDef(TableKinde);
            if (!pEnums)
              RaiseException(ExceptStr[eiObjectNotFound]);
            id = pEnums->GetID();
            bAsChar = true;
          }
          break;
        case ntConstant:
        case ntDocsJournal:
        case ntSelectionColumn:
        case ntCalendar:
        case ntCalcJournalKind:
    		id = GetMetaDataId(MetaNameType, TableKinde)->GetID();
            break;
      };
      if (bAsChar)
        SetTypeID36Literal(Name, id);
      else
      {
          ultoa(id, Name.GetBufferSetLength(12), 10);
          Name.ReleaseBuffer();
      };
      if (m_RPCMode)
          Name = GetCPName(Name, bAsChar ? "char(4)" : "int");
    }
    break;
  case mnVirtualTable: {	//ntRegistrRests, ntRegistrTurnovers, ntRegistrRestsTurnovers
    CString TmpAlias, SubQuery;
    CStringList VParams;
    int Processed = 0;
    if (!DefineList((LPCTSTR) QueryText + CurIndex, VParams, Processed))
      RaiseException(ExceptStr[eiUncomplMN]);
    switch (MetaNameType) {
    case ntRegistrRests:
      {
        BuildRegRests(static_cast<CRegDef*>(GetMetaDataId(ntRegistr,TableKinde)),
          SubQuery, VParams, TmpAlias);
      }
      break;
    case ntRegistrTurnovers:
      {
        BuildRegTurnovers(static_cast<CRegDef*>(GetMetaDataId(ntRegistr,TableKinde)),
          SubQuery, VParams, TmpAlias);
      }
      break;
    case ntRegistrRestsTurnovers:
      {
        BuildRegRestsTurnovers(static_cast<CRegDef*>(GetMetaDataId(ntRegistr,TableKinde)),
          SubQuery, VParams, TmpAlias);
      }
    };
    CurIndex += Processed;
    if (TmpAlias.IsEmpty()) {
    		Name = SubQuery;
    } else {
		    int tCurIndex = CurIndex;
        CString Alias;
        if (!FindeAlias(Alias)) {
          Alias = TmpAlias;
          CurIndex = tCurIndex;
        };
        Name.Format("(\r\n%s) as %s", SubQuery, Alias);
    };
                       }
    break;
  case mnVirtualValue: {
    CString SubQuery, PropName;
    
    if (MetaNameType == ntLastValue) {
      if (!IsNext())
        RaiseException(ExceptStr[eiUncomplMN]);
      GetIdentificator(PropName);
      if (PropName.IsEmpty())
        RaiseException(ExceptStr[eiUncomplMN]);
    };
    CStringList VParams;
    int Processed = 0;
    if (!DefineList((LPCTSTR) QueryText + CurIndex, VParams, Processed))
      RaiseException(ExceptStr[eiUncomplMN]);
    
    switch (MetaNameType) {
    case ntLastValue: {
      CMetaDataTypedObj * MetaDataTypedObj;
      if (!TableKinde.CollateNoCase(TableNames[7].Names[0]) || !TableKinde.CollateNoCase(TableNames[7].Names[1]))
        MetaDataTypedObj = m_pMetaDataCont->GetConstDef(PropName);
      else {
        CSbCntTypeDef * SbCntTypeDef = m_pMetaDataCont->GetSTypeDef(TableKinde);
        if (!SbCntTypeDef)
          RaiseException(ExceptStr[eiVirtualFieldNotFound]);
        MetaDataTypedObj = SbCntTypeDef->GetParamDef(PropName);
      };
      if (!MetaDataTypedObj)
        RaiseException(ExceptStr[eiVirtualFieldNotFound]);
      if (!MetaDataTypedObj->IsDateDep())
        RaiseException(ExceptStr[eiVirtualFieldNotFound]);
      BuildLastValue(MetaDataTypedObj, SubQuery, VParams);
                      };
      break;
    case ntBegOfPeriod: BuildBegOfPeriod(SubQuery, TableKinde, VParams); break;
    };
    CurIndex += Processed;
    Name.Format("(\r\n%s)", SubQuery);
                       }
    break;
  default:
    { //mnSpecial
    switch (MetaNameType)
    {
    case ntHistory: {
      *ppMetaDataObj = GetMetaDataId(ntReference, TableKinde);
      if (IsNext()) {
        CString Field;
        GetIdentificator(Field);
        if (!Field.IsEmpty())
        {
          CSbCntParamDef* SbCntParamDef = static_cast<CSbCntTypeDef*>(*ppMetaDataObj)->GetParamDef(Field);
          if (SbCntParamDef)
          {
            if (SbCntParamDef->IsDateDep())
            {
              if (m_pIBSet->GetDataSourceType() == DATA_SOURCE_TYPE_DBF)
                  SetTypeID36Literal(Name, SbCntParamDef->GetID());
              else
              {
                  ultoa(SbCntParamDef->GetID(), Name.GetBufferSetLength(12), 10);
                  Name.ReleaseBuffer();
                  if (m_RPCMode)
                      Name = GetCPName(Name, "int");
              };
            }
            else
              RaiseException(ExceptStr[eiObjectNotFound]);
          }
          else
            RaiseException(ExceptStr[eiObjectNotFound]);
        }
        else 
          RaiseException(ExceptStr[eiUncomplMN]);
      }
      else 
        RaiseException(ExceptStr[eiUncomplMN]);
                    }
      break;
    case ntEnumeration:
    case ntEnumeration23: {
      *ppMetaDataObj = GetMetaDataId(ntEnumeration, TableKinde);
      if (IsNext()) {
		CString EnumVal;
		GetIdentificator(EnumVal);
		if (!EnumVal.IsEmpty())
		{
			CEnumValDef* pEnumValDef = static_cast<CEnumDef*>(*ppMetaDataObj)->GetValDef(EnumVal);
			if (pEnumValDef)
			{
				CString strSQLType;
				char* pBuf;
				int nSpace;
				if (MetaNameType == ntEnumeration23)
				{
					pBuf = Name.GetBufferSetLength(25);
					*pBuf++ = '\'';
					*pBuf++ = 'E';
					*pBuf++ = '1';
					LongToCharID36((*ppMetaDataObj)->GetID(), pBuf, 4);
					pBuf += 4;
					nSpace = 11;
					strSQLType = "char(9)";
				}
				else
				{
					pBuf = Name.GetBufferSetLength(11);
					*pBuf++ = '\'';
					nSpace = 3;
					strSQLType = "char(23)";
				}
				LongToCharID36(pEnumValDef->GetID(), pBuf, 6);
				pBuf += 6;
				memset(pBuf,' ',nSpace);
				pBuf[nSpace] = '\'';

				Name.ReleaseBuffer();
				if (m_RPCMode)
					Name = GetCPName(Name, strSQLType);
          }
          else
            RaiseException(ExceptStr[eiObjectNotFound]);
        }
        else 
          RaiseException(ExceptStr[eiUncomplMN]);
      }
      else 
        RaiseException(ExceptStr[eiUncomplMN]);
                    }
      break;
    case ntColumnCalcJournal: {
      *ppMetaDataObj = GetMetaDataId(ntCalcJournal, TableKinde);
      if (IsNext()) {
        CString Field;
        GetIdentificator(Field);
        if (!Field.IsEmpty()) {
          Name.Format("ff%d", GetMetaDataFieldId(ntColumnCalcJournal, *ppMetaDataObj, Field));
        }
        else 
          RaiseException(ExceptStr[eiUncomplMN]);
      }
      else 
        RaiseException(ExceptStr[eiUncomplMN]);
                              }
    }
    break;
    }
  }
  return DoFindeAlias;
};

int CMetaNameParser::GetIdentificatorFromString(const char * pStr, CString & Id)
{
  const char * pBegin = pStr;
  while (IsIdChar(*pStr++));
  --pStr;
  int Len = pStr - pBegin;
  strncpy(Id.GetBufferSetLength(Len), pBegin, Len);
  return Len;
};

void CMetaNameParser::GetIdentificator(CString & Id)
{
  CurIndex += GetIdentificatorFromString((LPCTSTR) QueryText + CurIndex, Id);
}

char CMetaNameParser::GetNextSource(char StopChar)
{
  bool fStrConst = false;
  bool fComment = false;
  bool fCommentType;
  bool fExtName = false;
  
  char CurChar;
  const char * pQT = (LPCSTR) QueryText;
  
  while (CurChar = pQT[CurIndex++]) {
    if (fComment) {//идет комментарий
      if (fCommentType)
        fComment = (CurChar != '\n');
      else
        fComment = !((CurChar == '*') && (pQT[CurIndex] == '/'));
      continue;
    }
    if (fStrConst) {// идет строковая константа
      fStrConst = CurChar != '\'';
      if (!fStrConst && m_RPCMode && StopChar == '\'')
      {
        QueryText.Insert(CurIndex, '\'');
        pQT = (LPCSTR) QueryText;
        CurIndex++;
      };
      continue;
    }
    if (fExtName) {// идет расширенное имя
      fExtName = CurChar != ']';
      continue;
    }
    switch (CurChar) {
    case '-': //комментарий типа --
      if (pQT[CurIndex] == '-') {
        fComment = true;
        fCommentType = true;
      }
      break;
    case '/': //комментарий типа /*
      if (pQT[CurIndex] == '*') {
        fComment = true;
        fCommentType = false;
      }
      break;
    case '\'': // строковая константа ''
      fStrConst = true;
      if (m_RPCMode && StopChar == '\'')
      {
        QueryText.Insert(CurIndex, '\'');
        pQT = (LPCSTR) QueryText;
        CurIndex++;
      };
      break;
    case '[': // расширенное имя
      fExtName = true;
      break;
    default:
      if (StopChar) {
        if (CurChar == StopChar)
          return CurChar;
      }
      else
        if (!((CurChar == ' ') || (CurChar == '\n') || (CurChar == '\r'))) {
          CurIndex--;
          return CurChar;
        }
        break;
    }
  }
  CurIndex--;
  return CurChar;
}

bool CMetaNameParser::IsNext()
{
  switch (((LPCSTR) QueryText)[CurIndex]) {
  case '.':
    CurIndex++;
    return true;
  case 0:
    return false;
  default:
    return false;
  }
}

void CMetaNameParser::ResolveSecondLevelMetaName(const MetaNameTypes NameType, const CMetaDataObj* Table, CString &Name)
{
  CString Field;
  GetIdentificator(Field);
  if (Field.IsEmpty())
    RaiseException(ExceptStr[eiUncomplMN]);
  else
    Name.Format("sp%d", GetMetaDataFieldId(NameType, Table, Field));
}

void CMetaNameParser::PasteName(CString Name)
{
  Start--;
  QueryText.Delete(Start, CurIndex - Start);
  QueryText.Insert(Start, Name);
  CurIndex = Start + Name.GetLength();
}

BOOL CMetaNameParser::FindeAlias(CString & Alias)
{
  Alias.Empty();
  if (IsIdChar(GetNextSource())) {
    GetIdentificator(Alias);
    if (!Alias.CompareNoCase("as")) {
      // Стоит "as"
      Alias.Empty();
      if (IsIdChar(GetNextSource()))
        GetIdentificator(Alias);
    }
  }
  return !Alias.IsEmpty();
}

bool CMetaNameParser::IsIdChar(const char Ch)
{
  return (Ch =='_' || IsCharAlphaNumeric(Ch));
};

CMetaDataObj * CMetaNameParser::GetMetaDataId(const MetaNameTypes NameType, CString & Kind)
{
  CMetaDataObj * pMetaDataObj = NULL;
  switch (NameType) {
  case ntConstant:
    pMetaDataObj = m_pMetaDataCont->GetConstDef(Kind);
    break;
  case ntReference:
    pMetaDataObj = m_pMetaDataCont->GetSTypeDef(Kind);
    break;
  case ntDocument:
  case ntDocumentStrings:
    pMetaDataObj = m_pMetaDataCont->GetDocDef(Kind);
    break;
  case ntRegistr:
  case ntRegistrTotals:
    pMetaDataObj = m_pMetaDataCont->GetRegDef(Kind);
    break;
  case ntCalcJournalKind:
  case ntCalcJournal:
    pMetaDataObj = m_pMetaDataCont->GetCJDef(Kind);
    break;
  case ntCommonProp:
    pMetaDataObj = m_pMetaDataCont->GetGenJrnlFlds()->GetItem(Kind);
    break;
  case ntSequence:
    pMetaDataObj = m_pMetaDataCont->GetDocStreamDefs()->GetItem(Kind);
    break;
  case ntDocsJournal:
    pMetaDataObj = m_pMetaDataCont->GetJournalDefs()->GetItem(Kind);
    break;
  case ntSelectionColumn:
    pMetaDataObj = m_pMetaDataCont->GenDocSelRefs()->GetItem(Kind);
    break;
  case ntCalendar:
    pMetaDataObj = m_pMetaDataCont->GetCalendarTypeDef(Kind);
    break;
  case ntAlgorithm:
    pMetaDataObj = m_pMetaDataCont->GetAlgorithmDef(Kind);
	break;// spock: 2006-01-29
  case ntEnumeration:
    pMetaDataObj = m_pMetaDataCont->GetEnumDef(Kind);
    break;
  
  case ntEntry:// spock: 2006-01-29
	  {
		  CBuhDef* pBuhDef = pMetaDataCont->GetBuhDef();
		  if(pBuhDef)
		  {
			  CTypedFldDefsArray<CProvParamDef> * pProvParam = pBuhDef->GetProvParamDefs();
			  if(pProvParam)
				  pMetaDataObj = pProvParam->GetItem(Kind);
		  }
	  }
	  break;
  }
  if (!pMetaDataObj)
    RaiseException(ExceptStr[eiObjectNotFound]);
  return pMetaDataObj;
}

long CMetaNameParser::GetMetaDataFieldId(const MetaNameTypes NameType, const CMetaDataObj* Table, CString & Field)
{
  long id = 0;
  switch (NameType) {
  case ntReference:
    {
      const CSbCntParamDef* SbCntParamDef = static_cast<const CSbCntTypeDef*>(Table)->GetParamDef(Field);
      if (!SbCntParamDef)
        break;
      if (SbCntParamDef->IsDateDep())
        break;
      id = SbCntParamDef->GetID();
    }
    break;
  case ntDocument:
    {
      const CDocHeadFldDef* DocHeadFldDef = static_cast<const CDocDef*>(Table)->GetHeadFldDef(Field);
      if (DocHeadFldDef)
        id = DocHeadFldDef->GetID();
      else
      {
        const CDocTblFldDef* DocTblFldDef = static_cast<const CDocDef*>(Table)->GetTblFldDef(Field);
        if (!DocTblFldDef)
          break;
        if (!DocTblFldDef->GetHaveTotal())
          break;
        id = DocTblFldDef->GetID();
      };
    }
    break;
  case ntDocumentStrings:
    {
      const CDocTblFldDef* DocTblFldDef = static_cast<const CDocDef*>(Table)->GetTblFldDef(Field);
      if (!DocTblFldDef)
        break;
      id = DocTblFldDef->GetID();
    }
    break;
  case ntRegistr:
    {
      const CRegDef* RegDef = static_cast<const CRegDef*>(Table);
      const CRegPropDef * RegPropDef = RegDef->GetPropDef(Field);
      if (RegPropDef)
        id = RegPropDef->GetID();
      else
      {
        const CRegFigureDef * RegFigureDef = RegDef->GetFigureDef(Field);
        if (RegFigureDef)
          id = RegFigureDef->GetID();
        else
        {
          const CRegFldDef * RegFldDef = RegDef->GetFlds()->GetItem(Field);
          if (!RegFldDef)
            break;
          id = RegFldDef->GetID();
        }
      }
    }
    break;
  case ntRegistrTotals:
    {
      const CRegDef * RegDef = static_cast<const CRegDef*>(Table);
      const CRegPropDef * RegPropDef = RegDef->GetPropDef(Field);
      if (RegPropDef)
        id = RegPropDef->GetID();
      else
      {
        const CRegFigureDef * RegFigureDef = RegDef->GetFigureDef(Field);
        if (!RegFigureDef)
          break;
        id = RegFigureDef->GetID();
      }
    }
    break;
  case ntCalcJournal:
    {
      const CCJParamDef * CJParamDef = static_cast<const CCJDef*>(Table)->GetCJParamDef(Field);
      if (!CJParamDef)
        break;
      id = CJParamDef->GetID();
    };
    break;
  case ntColumnCalcJournal:
    {
      const CCJDef* CJDef = static_cast<const CCJDef*>(Table);
      const CSbCntParamDef* SbCntParamDef = m_pMetaDataCont->GetSTypeDef(CJDef->GetReferID())->GetParamDef(Field);
      if (!SbCntParamDef)
        break;
      if (CJDef->GetFFList()->Find(SbCntParamDef->GetID()) == -1)
        break;
      id = SbCntParamDef->GetID();
    }
  }
  if (!id)
    RaiseException(ExceptStr[eiFieldOfObjectNotFound]);
  return id;
}

void CMetaNameParser::SetParameter(const CString & Name, const CValue & Value)
{
  Params[Name] = Value;
}

void CMetaNameParser::RaiseException(const char * DescTemplate)
{
  Aliases.RemoveAll();
  ClearParams();
  
  SQLParams.RemoveAll();
  
  CString ErrDesc;
  Start--;
  ErrDesc.Format(DescTemplate, QueryText.Mid(Start, CurIndex - Start));
  QueryText.Empty();
  throw new CMNPException(TRUE, ErrDesc, CurIndex);
};

CString CMetaNameParser::CDateToPeriod(const CDate & Date)
{
  CString s;
  s.Format("{d '%04d-%02d-%02d'}", Date.GetYear(), Date.GetMonth(), Date.GetMonthDay());
  return s;
};

void CMetaNameParser::AddMDObjArray(CString & Dest, const PMetaDataObjArray & MDObjArray, const CString & StrTemplate, const CString & StrSep, const int fAddToEnd)
{
  
  int Count = MDObjArray.GetSize() - 1;
  for (int i = 0; i <= Count; i++) {
    CString StrTmp(StrTemplate), StrId;
    StrId.Format("sp%d", MDObjArray[i]->GetID());
    
    StrTmp.Replace("%cd", MDObjArray[i]->GetCode());
    StrTmp.Replace("%id", StrId);
    
    Dest += StrTmp;
    
    if ((i < Count) || (fAddToEnd))
      Dest += StrSep;
    
    Dest += "\r\n";
  };
};

CString CMetaNameParser::DescParameter(const int NParam)
{
  CString StrParam;
  StrParam.Format("(%d)", NParam);
  return StrParam;
};

void CMetaNameParser::CStringToMDObjArray(PMetaDataObjArray & DestMDObjArray, const CString & StrDesc, const CMetaDataObjArray* SourceMDObjArray, int NParam)
{
  if (StrDesc.IsEmpty()) {
    int Count = SourceMDObjArray->GetNItems();
    DestMDObjArray.SetSize(Count);
    for (int i = 0; i < Count; i++)
      DestMDObjArray[i] = (CMetaDataTypedObj *)SourceMDObjArray->GetAt(i);
  } else {
    CStringList uList;
    if (StrDesc[0] == '(') {
      int Proced;
      DefineList(StrDesc, uList, Proced);
      if (Proced < StrDesc.GetLength())
        RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
    } else
      uList.AddTail(StrDesc);
    
    DestMDObjArray.SetSize(uList.GetCount());
    POSITION Pos = uList.GetHeadPosition();
    int i = 0;
    while (Pos != NULL) {
      CMetaDataObj * Obj = SourceMDObjArray->GetItem(uList.GetNext(Pos));
      if (!Obj)
        RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
      DestMDObjArray[i] = (CMetaDataTypedObj *)Obj;
      i++;
    };
  };
};

void CMetaNameParser::ProcessJoinesConditions(CString & Joines, CString & Conditions, MDArraysItem * pSourceMDObjArray, int ArraySize, int NParam)
{
  if (!Joines.IsEmpty() || !Conditions.IsEmpty())
  {
    //Push current parsing process params
    CString QueryText_copy = QueryText;
	int Start_copy = Start;
	int CurIndex_copy = CurIndex;

    struct stMNTableAliases
    {
        CString Alias;
        TableAliases Table;
    };
    stMNTableAliases* pMNTableAliases = NULL;
    stMNTableAliases* pCurMNTableAliases;
    if (Aliases.GetCount() > 0)
    {
        pMNTableAliases = new stMNTableAliases[Aliases.GetCount()];
        POSITION pos = Aliases.GetStartPosition();
        CString CurAlias;
        TableAliases CurTable;
        pCurMNTableAliases = pMNTableAliases;
        for ( ;pos != NULL; pCurMNTableAliases++)
        {
            Aliases.GetNextAssoc(pos, CurAlias, CurTable);
            pCurMNTableAliases->Alias = CurAlias;
            pCurMNTableAliases->Table = CurTable;
        };
    };
    CString sTempFieldPrefix = m_sTempFieldPrefix;
    
    //Set new current parsing process params
    QueryText = Joines;
    QueryText += Separator;
    QueryText += Conditions;
    Aliases.RemoveAll();

    try
    {
	    ParseMetaNames();
    } catch (CMNPException * MNPException)
    {
      if (MNPException->GetCurrentIndex() > QueryText.Find(Separator))
        NParam++;
      CString Str;
      Str.Format("(%d : %s)", NParam, MNPException->GetErrorDescr());
      MNPException->Delete();
      RaiseException(ExceptStr[eiWrongParam] + Str);
    };
    
    //Get results
    CString JoinesConditions = QueryText;
    
    //Pop current parsing process params
    QueryText = QueryText_copy;
	Start = Start_copy;
	CurIndex = CurIndex_copy;
    Aliases.RemoveAll();
    if (pMNTableAliases)
    {
        stMNTableAliases* pEndMNTableAliases = pCurMNTableAliases;
        for (pCurMNTableAliases = pMNTableAliases; pCurMNTableAliases != pEndMNTableAliases; pCurMNTableAliases++)
            Aliases[pCurMNTableAliases->Alias] = pCurMNTableAliases->Table;
        m_sTempFieldPrefix = sTempFieldPrefix;
        delete[] pMNTableAliases;
    };


    int Index = 0;
    CString Id;
    bool fStrConst = false;
    bool fDotOrDog = false;
    while (Index < JoinesConditions.GetLength()) {
      char CurChar = JoinesConditions[Index];
      if (fStrConst)
      {
        fStrConst = CurChar != '\'';
        Index++;
        continue;
      };
      if (IsIdChar(CurChar))
      {
        int Len = GetIdentificatorFromString((LPCTSTR) JoinesConditions + Index, Id);
        if (!fDotOrDog)
        {
          for (int i = 0; i < ArraySize; i++)
          {
          CMetaDataObj * Obj = pSourceMDObjArray[i].MDObjArray->GetItem(Id);
            if (Obj)
            {
              Id.Format("%s.sp%d", m_sTempFieldPrefix, Obj->GetID());
              JoinesConditions.Delete(Index, Len);
              JoinesConditions.Insert(Index, Id);
              Index += Id.GetLength();
              Len = 0;
              pSourceMDObjArray[i].IsUsed = true;
              break;
            };
          };
        };
        if (Len)
          Index += Len;
      }
      else
      {
        fStrConst = CurChar == '\'';
        fDotOrDog = CurChar == '.' || CurChar == '@';
        Index++;
      };
    };
    int CutIndex = JoinesConditions.Find(Separator);
    if (CutIndex == -1) {
      CString Str;
      Str.Format("(%d или %d)", NParam, NParam + 1);
      RaiseException(ExceptStr[eiWrongParam] + Str);
    };
    Joines = JoinesConditions.Left(CutIndex);
    Conditions = JoinesConditions.Mid(++CutIndex);
  };
};

int CMetaNameParser::GetBorderParameter(CString & Border, CDate & Date, int NParam)
{
    if (m_RPCMode && Border[0] == '@')
    {
        CString VarValue;
        int index;
        if (!m_UseTextParamsInRPC.Lookup(Border, (void*&)index))
            RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
        stRPCParam& RPCParam = m_RPCParams[index];
        Border = RPCParam.Value;
        RPCParam.Count--;
    };

  int Len = Border.GetLength();
  if (Border[0] != '\'' || Border[--Len] != '\'')
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
  Border = Border.Mid(1, --Len);
  if (Len < 8)
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
  if (Len == 8)
    Len = -1;
  else {
    if (Border[8] == 'Z')
      Len = 1;
    else
      Len = 0;
  };
  Date = CDate(atoi(Border.Left(4)), atoi(Border.Mid(4, 2)), atoi(Border.Mid(6, 2)));
  return Len;
};

CString CMetaNameParser::CDateToCString(const CDate & Date)
{
  CString s;
  s.Format("%04d%02d%02d", Date.GetYear(), Date.GetMonth(), Date.GetMonthDay());
  return s;
};

CString CMetaNameParser::GetStringDateTimeIddoc(const CDate & Date, const long Time, const CObjID & ObjID)
{
  char TimeBuff[7];
  char IDBuff[7];
  LongToCharID36(Time, TimeBuff, 6);
  TimeBuff[6] = 0;
  LongToCharID36(ObjID.GetlObjID(), IDBuff, 6);
  IDBuff[6] = 0;
  CString s;
  s.Format("%s%6s%6s%3s", CDateToCString(Date), TimeBuff, IDBuff, ObjID.DBSign.Sign);
  return s;
};

CString CMetaNameParser::GetTABorder(CDate & DateTA)
{
  CEventTime TimeTA_;
  CDate DateTA_ = m_pIBSet->GetDateTimeTA(TimeTA_);
  //CSystemSet* SystemSet = GetSysSet();
  /*
  DateTA = SystemSet->GetDateTA();
  CEventTime EventTimeTA = SystemSet->GetTimeTA();
  CString PosTA(GetStringDateTimeIddoc(DateTA, EventTimeTA.GetlTime(), EventTimeTA.GetEventID()));
  PosTA.SetAt(20, ' ');
  PosTA.SetAt(21, ' ');
  PosTA.SetAt(22, '0');
  */
  CEventTime EventTimeTA;
  NextDateTime(DateTA_, TimeTA_, DateTA, EventTimeTA);
  CString PosTA(GetStringDateTimeIddoc(DateTA, EventTimeTA.GetlTime(), EventTimeTA.GetEventID()));
  return PosTA;
};

int CMetaNameParser::DefineList(const char * pStr, CStringList & List, int & CharsProcessed)
{
  const char * lStart = pStr;
  if (*pStr++ != '(') {
    CharsProcessed = pStr - lStart;
    return 1;
  };
  
  bool fComment = false;
  bool fCommentType;
  bool fStrConst = false;
  int fSubList = 0;
  bool fAddSep;
  char CurChar;
  
  CString CurItem = "";
  fAddSep = false;
  
  while (CurChar = *pStr++) {
    if (fComment) {//идет комментарий
      if (fCommentType)
        fComment = (CurChar != '\n');
      else {
        fComment = !((CurChar == '*') && (*pStr == '/'));
        if (!fComment)
          pStr++;
      }
      continue;
    };
    switch (CurChar) {
    case '-': //комментарий типа --
      if (*pStr == '-') {
        fComment = true;
        fCommentType = true;
        CurChar = ' ';
      }
      break;
    case '/': //комментарий типа /*
      if (*pStr == '*') {
        fComment = true;
        fCommentType = false;
        CurChar = ' ';
      }
      break;
    case '\'': // строковая константа ''
      fStrConst =! fStrConst;
      break;
    };
    if (!fStrConst) {
      if (fSubList) {//идет под список
        switch (CurChar) {
        case '(':
          fSubList++;
          break;
        case ')':
          fSubList--;
          break;
        };
      } else {
        switch (CurChar) {
        case '\n':
          CurChar = ' ';
          break;
        case '\r':
          CurChar = ' ';
          break;
        case '(': //под список
          fSubList++;
          break;
        case ')': //конец списка
          CurItem.TrimLeft();
          CurItem.TrimRight();
          List.AddTail(CurItem);
          CharsProcessed = pStr - lStart;
          return 2;
          break;
        case ',': //следующий элемент
          CurItem.TrimLeft();
          CurItem.TrimRight();
          List.AddTail(CurItem);
          CurItem = "";
          continue;
        };
      };
    };
    if (CurChar == ' ' && !fStrConst) {
      if (fAddSep)
        continue;
      else
        fAddSep = true;
    } else
      fAddSep = false;
    CurItem += CurChar;
  };
  CharsProcessed = pStr - lStart;
  return 0;
};

POSITION CMetaNameParser::GetVirtParams(const int ParamsCount, CStringList & Params)
{
  for (int i = Params.GetCount(); i < ParamsCount; i++)
    Params.AddTail("");
  if (Params.GetCount() > ParamsCount)
    RaiseException(ExceptStr[eiTooManyParam]);
  return Params.GetHeadPosition();
};

CDate CMetaNameParser::GetPrevPeriodEx(CDate Date, PeriodType Period)
{
	  PeriodType CurrentRestsSnapShotPeriod = GetRestsSnapShotPeriod();
	  if (Period != CurrentRestsSnapShotPeriod)
	  {
		  SetRestsSnapShotPeriod(Period);
		  CDate ResultDate = GetPrevPeriod(Date);
		  SetRestsSnapShotPeriod(CurrentRestsSnapShotPeriod);
		  return ResultDate;
	  }
	  else
		  return GetPrevPeriod(Date);;

}

CDate CMetaNameParser::GetNextPeriodEx(CDate Date, PeriodType Period)
{
	  PeriodType CurrentRestsSnapShotPeriod = GetRestsSnapShotPeriod();
	  if (Period != CurrentRestsSnapShotPeriod)
	  {
		  SetRestsSnapShotPeriod(Period);
		  CDate ResultDate = GetNextPeriod(Date);
		  SetRestsSnapShotPeriod(CurrentRestsSnapShotPeriod);
		  return ResultDate;
	  }
	  else
		  return GetNextPeriod(Date);;
}

void CMetaNameParser::GetRestsQueryPlan(const CString & Border, const CDate & DateRests, const int fEndOfDay, const CString & PosTA, const CDate & DateTA, const PeriodType RestsSnapShotPeriod, stRestsPlan & RestsPlan)
{
  RestsPlan.QType = rqForward;
  if (!Border.Compare(PosTA)) { //на ТА
    RestsPlan.QType = rqNoTurnovers;
    RestsPlan.RestsPeriod = GetBegOfPeriod(DateTA, RestsSnapShotPeriod);
  } else if (fEndOfDay == 1) { //на конец дня
    if (DateRests.m_DateNum == GetEndOfPeriod(DateRests, RestsSnapShotPeriod).m_DateNum) {
      RestsPlan.QType = rqNoTurnovers;
      RestsPlan.RestsPeriod = GetBegOfPeriod(DateRests, RestsSnapShotPeriod);
    };
  } else if (fEndOfDay == -1){ //на начало дня
    if (DateRests.m_DateNum == GetBegOfPeriod(DateRests, RestsSnapShotPeriod).m_DateNum) {
      RestsPlan.QType = rqNoTurnovers;
      //RestsPlan.RestsPeriod = GetPrevPeriod(DateRests);
      RestsPlan.RestsPeriod = GetPrevPeriodEx(DateRests, RestsSnapShotPeriod);
    };
  };
  if (RestsPlan.QType != rqNoTurnovers) {
    CDate DateBeg(0, 0, 0);
    CDate DateEnd(0, 0, 0);
    bool BackEnabled;
    if (GetBegOfPeriod(DateRests, RestsSnapShotPeriod).m_DateNum != GetBegOfPeriod(DateTA, RestsSnapShotPeriod).m_DateNum)
    {
      DateBeg = GetBegOfPeriod(DateRests, RestsSnapShotPeriod);
      DateEnd = GetEndOfPeriod(DateRests, RestsSnapShotPeriod);
      BackEnabled = true;
    }
    else
    {
      DateBeg = GetBegOfPeriod(DateTA, RestsSnapShotPeriod);
      DateEnd = DateTA;
      BackEnabled = m_BackFromTAEnabled;
    };

    //if (BackEnabled && DateRests.GetMonthDay() > (DateEnd.GetMonthDay() >> 1))
    //  RestsPlan.QType = rqBack;
    
	if (BackEnabled)
	{
		if (DateRests.GetMonthDay() > ((DateBeg.GetMonthDay() + DateEnd.GetMonthDay()) >> 1))
		{
			RestsPlan.QType = rqBack;
		}
	}
    
    if (RestsPlan.QType == rqForward) {
      //RestsPlan.RestsPeriod = GetPrevPeriod(DateRests);
      RestsPlan.RestsPeriod = GetPrevPeriodEx(DateRests, RestsSnapShotPeriod);
      RestsPlan.TurnEnd = Border;
      RestsPlan.TurnBegin = CDateToCString(GetBegOfPeriod(DateRests, RestsSnapShotPeriod));
    } else {//rqBack
      RestsPlan.RestsPeriod = GetBegOfPeriod(DateRests, RestsSnapShotPeriod);
      if (GetBegOfPeriod(DateRests, RestsSnapShotPeriod).m_DateNum == GetBegOfPeriod(DateTA, RestsSnapShotPeriod).m_DateNum)
        RestsPlan.TurnEnd = PosTA;
      else
        RestsPlan.TurnEnd.Format("%sZ", CDateToCString(GetEndOfPeriod(DateRests, RestsSnapShotPeriod)));
      RestsPlan.TurnBegin = Border;
    };
  };
};

void CMetaNameParser::AddSubToken(CString &SubQuery, const char *pSubToken, char ch1, const char **pPrefixSuffix)
{
    if (strlen(pSubToken) > 0)
    {
        if (pPrefixSuffix[0])
            SubQuery += pPrefixSuffix[0];

        int Len = SubQuery.GetLength();
        SubQuery += pSubToken;
        char *p = SubQuery.LockBuffer() + Len;
        Len = strlen(m_sTempFieldPrefix);
        while (char *pCurPref = strstr(p, m_sTempFieldPrefix))
        {
            pCurPref[1] = ch1;
            p += Len;
        };

        if (pPrefixSuffix[1])
            SubQuery += pPrefixSuffix[1];
    };
};

CString CMetaNameParser::GetCPName(const CString& ParamValue, const CString& ParamType)
{
    CString VarName;
    VarName.Format("@_CP_%u", m_RPCParams.GetSize());
    stRPCParam RPCParam;
    RPCParam.Count = 1;
    RPCParam.Value = ParamValue;
    RPCParam.Type = ParamType;
    RPCParam.VarName = VarName;
    m_RPCParams.Add(RPCParam);
    return VarName;
};

CString CMetaNameParser::GetVTStringParam(const CString& ParamValue)
{
    if (m_RPCMode)
    {
        CString sValue('\'');
        sValue += ParamValue;
        sValue += '\'';

        int Len = ParamValue.GetLength();
        if (Len == 0)
            Len = 1;

        CString sType;
        sType.Format("char(%u)", Len);
        
        return GetCPName(sValue, sType);
    }
    else
    {
        CString sRet('\'');
        sRet += ParamValue;
        sRet += '\'';
        return sRet;
    };
};

void CMetaNameParser::BuildRegRestsSubQuery(CRegDef* RegDef, const PMetaDataObjArray & Dims, const PMetaDataObjArray & Figs, const CString & Joines, const CString & Conditions, const stRestsPlan & RestsPlan, CString & SubQuery)
{
  CString TmpStr;

  CString sTotName(m_sTempFieldPrefix);
  sTotName.SetAt(1,'g');

  CString sRestsPeriod = CDateToPeriod(RestsPlan.RestsPeriod);
  if (m_RPCMode)
      sRestsPeriod = GetCPName(sRestsPeriod, "smalldatetime");

  SubQuery = "select\r\n";
  if (RestsPlan.QType == rqNoTurnovers)
  {
    if (Dims.GetSize() == RegDef->GetProps()->GetNItems())
    {
      AddMDObjArray(SubQuery, Dims, sTotName + ".%id as %cd", ",", 1);
      AddMDObjArray(SubQuery, Figs, RestsPlan.EnlargeStr + sTotName + ".%id as %cdОстаток", ",");
      TmpStr.Format("from rg%d as %s (nolock)\r\n", RegDef->GetID(), sTotName); SubQuery += TmpStr;

      AddSubToken(SubQuery, Joines, 'g', arJoinesPS);

      TmpStr.Format("where %s.period=%s\r\n", sTotName, sRestsPeriod); SubQuery += TmpStr;
      if (RestsPlan.EnlargeStr.IsEmpty())
      {
        SubQuery += "and (\r\n";
        AddMDObjArray(SubQuery, Figs, sTotName + ".%id <> 0", " or");
        SubQuery += ")\r\n";
      };
      AddSubToken(SubQuery, Conditions, 'g', arConditionsPS);
    }
    else
    {
      AddMDObjArray(SubQuery, Dims, sTotName + ".%id as %cd", ",", 1);
      if (RestsPlan.EnlargeStr.IsEmpty())
        AddMDObjArray(SubQuery, Figs, CString("sum(") + sTotName + ".%id) as %cdОстаток", ",");
      else
        AddMDObjArray(SubQuery, Figs, RestsPlan.EnlargeStr + sTotName + ".%id", ",");
      TmpStr.Format("from rg%d as %s (nolock)\r\n", RegDef->GetID(), sTotName); SubQuery += TmpStr;
      AddSubToken(SubQuery, Joines, 'g', arJoinesPS);

      TmpStr.Format("where %s.period=%s\r\n", sTotName, sRestsPeriod); SubQuery += TmpStr;
      AddSubToken(SubQuery, Conditions, 'g', arConditionsPS);
      if (RestsPlan.EnlargeStr.IsEmpty())
      {
        SubQuery += "group by\r\n";
        AddMDObjArray(SubQuery, Dims, sTotName + ".%id", ",");
        SubQuery += "having\r\n";
        AddMDObjArray(SubQuery, Figs, CString("sum(") + sTotName + ".%id) <> 0", " or");
      };
    };
  }
  else
  {
    CString sTurnName(sTotName);
    sTurnName.SetAt(1,'a');
    CString sVTName(sTotName);
    sVTName.SetAt(1,'r');

    if (RestsPlan.EnlargeStr.IsEmpty())
    {
      AddMDObjArray(SubQuery, Dims, sVTName + ".%id as %cd", ",", 1);
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sVTName + ".%id) as %cdОстаток", ",");
      SubQuery += "from (\r\nselect\r\n";
    };
    AddMDObjArray(SubQuery, Dims, sTotName + ".%id", ",", 1);
    AddMDObjArray(SubQuery, Figs, RestsPlan.EnlargeStr + sTotName + ".%id", ",");
    TmpStr.Format("from rg%d as %s (nolock)\r\n", RegDef->GetID(), sTotName); SubQuery += TmpStr;

    AddSubToken(SubQuery, Joines, 'g', arJoinesPS);

    TmpStr.Format("where %s.period=%s\r\n", sTotName, sRestsPeriod); SubQuery += TmpStr;
    AddSubToken(SubQuery, Conditions, 'g', arConditionsPS);

    SubQuery += "union all\r\nselect\r\n";
    AddMDObjArray(SubQuery, Dims, sTurnName + ".%id", ",", 1);

    int nSign = 0;
    if (RestsPlan.QType == rqBack)
      nSign = 1;
    TmpStr.Format("case %s.debkred when %d then %s.%%id else -%s.%%id end", sTurnName, nSign, sTurnName, sTurnName);

    AddMDObjArray(SubQuery, Figs, RestsPlan.EnlargeStr + TmpStr, ",");
    TmpStr.Format("from ra%d as %s (nolock)\r\n", RegDef->GetID(), sTurnName); SubQuery += TmpStr;
    CString sDateTimeName;
    if (!RegDef->GetDateTimeInActs())
    {
      sDateTimeName.Format("j%d_vt", RegDef->GetID());
      TmpStr.Format("inner join _1sjourn as %s (nolock) on %s.iddoc = %s.iddoc\r\n", sDateTimeName, sDateTimeName, sTurnName); SubQuery += TmpStr;
    }
    else
      sDateTimeName = sTurnName;

    AddSubToken(SubQuery, Joines, 'a', arJoinesPS);

    SubQuery += "where ";
    SubQuery += sDateTimeName;
    SubQuery += ".date_time_iddoc >";

    if (RestsPlan.QType == rqBack)
      SubQuery += '=';
    TmpStr.Format(" %s and %s.date_time_iddoc < %s\r\n", GetVTStringParam(RestsPlan.TurnBegin), sDateTimeName, GetVTStringParam(RestsPlan.TurnEnd)); SubQuery += TmpStr;
    if (!RegDef->GetDateTimeInActs())
    {
        TmpStr.Format("and %s.rf%d = 0x1\r\n", sDateTimeName, RegDef->GetID()); SubQuery += TmpStr;
    };
    AddSubToken(SubQuery, Conditions, 'a', arConditionsPS);

    if (RestsPlan.EnlargeStr.IsEmpty())
    {
      TmpStr.Format(") as %s\r\n", sVTName); SubQuery += TmpStr;
      SubQuery += "group by\r\n";
      AddMDObjArray(SubQuery, Dims, sVTName + ".%id", ",");
      SubQuery += "having\r\n";
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sVTName + ".%id) <> 0", " or");
    };
  };
};

void CMetaNameParser::BuildRegRests(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias)
{
  if (!RegDef->IsRests())
    RaiseException(ExceptStr[eiVirtTableNotFound]);
  
  if (!RegDef->GetNPropDefs())
    RaiseException(ExceptStr[eiDimsNotDef]);
  if (!RegDef->GetNFigureDefs())
    RaiseException(ExceptStr[eiFigsNotDef]);
  
  TmpAlias.Format("Rests%d", RegDef->GetID());
  
  POSITION ListPos = GetVirtParams(5, Params);
  CString Border = Params.GetNext(ListPos);
  CString Joines = Params.GetNext(ListPos);
  CString Conditions = Params.GetNext(ListPos);
  CString Dimentions = Params.GetNext(ListPos);
  CString Figure = Params.GetNext(ListPos);
  
  CDate DateTA(0,0,0);
  CString PosTA = GetTABorder(DateTA);
  PeriodType RestsSnapShotPeriod = m_pIBSet->GetRestsSnapShotPeriod();
  
  CDate DateRests(0, 0, 0);
  int fEndOfDay;
  
  if (Border.IsEmpty())
  {
    Border = PosTA;
    DateRests = DateTA;
    fEndOfDay = 1;
  } else 
    fEndOfDay = GetBorderParameter(Border, DateRests, 1);
  
  if (Border.Compare(PosTA) > 0) {
    Border = PosTA;
    DateRests = DateTA;
    fEndOfDay = 1;
  };
  
  m_sTempFieldPrefix.Format("r_%d_vt", RegDef->GetID());
  stRestsPlan RestsPlan;
  GetRestsQueryPlan(Border, DateRests, fEndOfDay, PosTA, DateTA, RestsSnapShotPeriod, RestsPlan);
  
  MDArraysItem MDArray = {RegDef->GetProps(), false};
  ProcessJoinesConditions(Joines, Conditions, &MDArray, 1, 2);
  
  PMetaDataObjArray Dims;
  CStringToMDObjArray(Dims, Dimentions, RegDef->GetProps(), 4);
  PMetaDataObjArray Figs;
  CStringToMDObjArray(Figs, Figure, RegDef->GetFigure(), 5);
  
  BuildRegRestsSubQuery(RegDef, Dims, Figs, Joines, Conditions, RestsPlan, SubQuery);
};

inline CDate CMetaNameParser::AddDays(const CDate & Date, int Days)
{
  return DATEToCDate(CDateToDATE(Date) + Days);
};

PeriodType CMetaNameParser::GetPeriodicityFromCString(const CString & StrPeriodicity, int NParam)
{
  if (StrPeriodicity.IsEmpty())
    return e_NotUsed;
  
  const struct PerDef {
    const char* Names[2];
    PeriodType Period;
  } PeriodicityDef[] = {
    {{"День", "Day"}, eDay},
    {{"Неделя", "Week"}, eWeek},
    {{"Месяц", "Month"}, eMonth},
    {{"Квартал", "Quarter"}, eQuart},
    {{"Год", "Year"}, eYear},
    {{"Период", "Period"}, e_NotUsed},
    {{"Документ", "Document"}, e_Document},
  };
  for (int i = 0; i < sizeof(PeriodicityDef)/sizeof(PerDef); i++)
    if (!StrPeriodicity.CollateNoCase(PeriodicityDef[i].Names[0]) || !StrPeriodicity.CollateNoCase(PeriodicityDef[i].Names[1]))
      return PeriodicityDef[i].Period;
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(NParam));
    return e_NotUsed;
};

CString CMetaNameParser::GetPeriodField(const PeriodType & Periodcity, const CString & Source, int SourceType, const CDate & BegLim)
{
  //SourceType - тип исходного поля : 0 - char(23), 1 - datetime
  CString Str("");
  switch (Periodcity) {
  case eDay:
    if (SourceType)
      Str = "%s";
    else
      Str = "cast(left(%s, 8) as datetime)";
    break;
  case eWeek:
    if (SourceType)
      Str = "dateadd(y, datepart(y, %s) - datepart(dw, %s), dateadd(yy, year(%s) - 2000, {d '2000-01-01'}))";
    else
      Str = "dateadd(y, datepart(y, cast(left(%s, 8) as datetime)) - datepart(dw, cast(left(%s, 8) as datetime)), dateadd(yy, year(cast(left(%s, 8) as datetime)) - 2000, {d '2000-01-01'}))";
    break;
  case eMonth:
    if (SourceType)
      Str = "dateadd(m, month(%s) - 1, dateadd(yy, year(%s) - 2000, {d '2000-01-01'}))";
    else
      Str = "cast(left(%s, 6) + '01' as datetime)";
    break;
  case eQuart:
    if (SourceType)
      Str = "dateadd(qq, datepart(qq, %s) - 1, dateadd(yy, year(%s) - 2000, {d '2000-01-01'}))";
    else
      Str = "dateadd(qq, datepart(qq, cast(left(%s, 8) as datetime)) - 1, dateadd(yy, year(cast(left(%s, 8) as datetime)) - 2000, {d '2000-01-01'}))";
    break;
  case eYear:
    if (SourceType)
      Str = "dateadd(yy, year(%s) - 2000, {d '2000-01-01'})";
    else
      Str = "cast(left(%s, 4) + '0101' as datetime)";
    break;
  };
  Str.Replace("%s", Source);
  if (BegLim.GetYear() && Periodcity != eDay) {
    CString s;
    s.Format("case when %s < %s then %s else %s end", Str, CDateToPeriod(BegLim), CDateToPeriod(BegLim), Str);
    return s;
  };
  return Str;
};

void CMetaNameParser::BuildRegTurnoversSubQuery(CRegDef* RegDef, const PMetaDataObjArray & Dims, const PMetaDataObjArray & Figs, const CString & Joines, const CString & Conditions, const stTurnoversPlan TurnoversPlan, CString & SubQuery)
{
  CString TmpStr;

  CString sTurnName(m_sTempFieldPrefix);
  sTurnName.SetAt(1,'a');

  CString sDateTimeName;
  if (!RegDef->GetDateTimeInActs())
    sDateTimeName.Format("j%d_vt", RegDef->GetID());
  else
    sDateTimeName = sTurnName;

  SubQuery = "select\r\n";
  AddMDObjArray(SubQuery, Dims, sTurnName + ".%id as %cd", ",", 1);
  if (TurnoversPlan.EnlargeStr.IsEmpty())
  {
    if (TurnoversPlan.Periodicity != e_NotUsed)
    {
      if (TurnoversPlan.Periodicity == e_Document)
      {
        SubQuery += sDateTimeName;
        SubQuery += ".date_time_iddoc as ПозицияДокумента, ";
        SubQuery += sDateTimeName;
        SubQuery += ".iddocdef as ВидДокумента,\r\n";
      }
      else
      {
        TmpStr = sDateTimeName;
        TmpStr += '.';
        TmpStr += NameOfPositionField;
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, TmpStr, 0, TurnoversPlan.BegLim);
        SubQuery += " as Период,\r\n";
      };
    };
    if (RegDef->IsTurnovers())
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sTurnName + ".%id) as %cdОборот", ",");
    else
    {
      TmpStr.Format("sum(case when %s.debkred = 0 then %s.%%id else 0 end) as %%cdПриход,\r\nsum(case when %s.debkred = 1 then %s.%%id else 0 end) as %%cdРасход", sTurnName, sTurnName, sTurnName, sTurnName);
      AddMDObjArray(SubQuery, Figs, TmpStr, ",");
    }
  }
  else
  {
    TmpStr.Format("case when %s.debkred = 0 then %s.%%id else 0 end as %%cdПриход,\r\ncase when %s.debkred = 1 then %s.%%id else 0 end as %%cdРасход", sTurnName, sTurnName, sTurnName, sTurnName);
    TmpStr += TurnoversPlan.EnlargeStr;
    AddMDObjArray(SubQuery, Figs, TmpStr, ",");
  };

  TmpStr.Format("from ra%d as %s (nolock)\r\n", RegDef->GetID(), sTurnName); SubQuery += TmpStr;
  if (!RegDef->GetDateTimeInActs())
  {
    TmpStr.Format("inner join _1sjourn as %s (nolock) on %s.iddoc = %s.iddoc\r\n", sDateTimeName, sDateTimeName, sTurnName); SubQuery += TmpStr;
  };

  AddSubToken(SubQuery, Joines, 'a', arJoinesPS);

  TmpStr.Format("where %s.date_time_iddoc > %s and %s.date_time_iddoc < %s\r\n", sDateTimeName, GetVTStringParam(TurnoversPlan.BeginBorder), sDateTimeName, GetVTStringParam(TurnoversPlan.EndBorder)); SubQuery += TmpStr;
  if (!RegDef->GetDateTimeInActs())
  {
      TmpStr.Format("and %s.rf%d = 0x1\r\n", sDateTimeName, RegDef->GetID()); SubQuery += TmpStr;
  };
  AddSubToken(SubQuery, Conditions, 'a', arConditionsPS);

  if (TurnoversPlan.EnlargeStr.IsEmpty())
  {
    SubQuery += "group by\r\n";
    AddMDObjArray(SubQuery, Dims, sTurnName + ".%id", ",", TurnoversPlan.Periodicity != e_NotUsed);
    if (TurnoversPlan.Periodicity != e_NotUsed)
    {
      if (TurnoversPlan.Periodicity == e_Document)
      {
        SubQuery += sDateTimeName;
        SubQuery += ".iddocdef, ";
        SubQuery += sDateTimeName;
        SubQuery += ".date_time_iddoc\r\n";
      }
      else
      {
        TmpStr = sDateTimeName;
        TmpStr += '.';
        TmpStr += NameOfPositionField;
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, TmpStr, 0, TurnoversPlan.BegLim);
        SubQuery += "\r\n";
      };
    };
    SubQuery += "having\r\n";
    if (RegDef->IsTurnovers())
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sTurnName + ".%id) <> 0", " or");
    else
    {
      TmpStr.Format("sum(case when %s.debkred = 0 then %s.%%id else 0 end) <> 0 or\r\nsum(case when %s.debkred = 1 then %s.%%id else 0 end) <> 0", sTurnName, sTurnName, sTurnName, sTurnName);
      AddMDObjArray(SubQuery, Figs, TmpStr, " or");
    };
  };
};

void CMetaNameParser::BuildRegTurnovers(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias)
{
  if (!RegDef->GetNPropDefs())
    RaiseException(ExceptStr[eiDimsNotDef]);
  if (!RegDef->GetNFigureDefs())
    RaiseException(ExceptStr[eiFigsNotDef]);
  
  long RegId = RegDef->GetID();
  TmpAlias.Format("Turnovers%d", RegId);
  
  POSITION ListPos = GetVirtParams(7, Params);
  CString BeginBorder = Params.GetNext(ListPos);
  CString EndBorder = Params.GetNext(ListPos);
  CString StrPeriodicity = Params.GetNext(ListPos);
  CString Joines = Params.GetNext(ListPos);
  CString Conditions = Params.GetNext(ListPos);
  CString Dimentions = Params.GetNext(ListPos);
  CString Figure = Params.GetNext(ListPos);
  
  CDate DateTA(0,0,0), BeginDate(0,0,0), EndDate(0,0,0);
  int fEndDate, fBegDate;
  CString PosTA = GetTABorder(DateTA);
  
  if (EndBorder.IsEmpty()) {
    EndBorder = PosTA;
    EndDate = DateTA;
    fEndDate = 1;
  } else {
    fEndDate = GetBorderParameter(EndBorder, EndDate, 2);
    if (EndBorder > PosTA) {
      EndBorder = PosTA;
      EndDate = DateTA;
      fEndDate = 1;
    };
  };
  
  if (BeginBorder.IsEmpty())
  {
    BeginBorder = "19000101";
    BeginDate = CDate(1900, 1, 1);
    fBegDate = -1;
  }
  else
  {
    fBegDate = GetBorderParameter(BeginBorder, BeginDate, 1);
    if (BeginBorder > EndBorder)
    {
      BeginBorder = EndBorder;
      BeginDate = EndDate;
      fBegDate = fEndDate;
    };
  };
  
  m_sTempFieldPrefix.Format("r_%d_vt", RegId);
  stTurnoversPlan TurnoversPlan;
  TurnoversPlan.Periodicity = GetPeriodicityFromCString(StrPeriodicity, 3);
  
  MDArraysItem MDArrays[2] = {{RegDef->GetProps(), false}, {RegDef->GetFilds(), false}};
  ProcessJoinesConditions(Joines, Conditions, MDArrays, 2, 4);
  
  PMetaDataObjArray Dims;
  CStringToMDObjArray(Dims, Dimentions, RegDef->GetProps(), 6);
  
  PMetaDataObjArray Figs;
  CStringToMDObjArray(Figs, Figure, RegDef->GetFigure(), 7);
  
  bool fOptimization = false;
  CString TmpStr;
  CString StrWhere;
  CDate BegPer(0, 0, 0), EndPer(0, 0, 0);
  PeriodType RegPeriod = RegDef->GetPeriod();

  CString sTurnName(m_sTempFieldPrefix);
  sTurnName.SetAt(1,'a');

  CString sDateTimeName;
  if (!RegDef->GetDateTimeInActs())
  {
    sDateTimeName.Format("j%d_vt", RegId);
  }
  else
    sDateTimeName = sTurnName;

  if (RegDef->IsTurnovers() && 
    !MDArrays[1].IsUsed && 
    TurnoversPlan.Periodicity != e_Document && 
    (TurnoversPlan.Periodicity == e_NotUsed ||
    (RegPeriod != eTenDays && RegPeriod != eWeek && RegPeriod <= TurnoversPlan.Periodicity) ||
    (RegPeriod == TurnoversPlan.Periodicity)
    ))
  {
    if (fBegDate < 0 && BeginDate.m_DateNum == GetBegOfPeriod(BeginDate, RegPeriod).m_DateNum)
    {
      BegPer = BeginDate;
    }
    else
    {
      if (fBegDate > 0 && BeginDate.m_DateNum == GetEndOfPeriod(BeginDate, RegPeriod).m_DateNum)
        BegPer = AddDays(BeginDate, 1);
      else
      {
        BegPer = AddDays(GetEndOfPeriod(BeginDate, RegPeriod), 1);
        StrWhere.Format("%s.date_time_iddoc > %s and %s.date_time_iddoc < %s", sDateTimeName, GetVTStringParam(BeginBorder), sDateTimeName, GetVTStringParam(CDateToCString(BegPer)));
      };
    };
      if (fEndDate > 0 && EndDate.m_DateNum == GetEndOfPeriod(EndDate, RegPeriod).m_DateNum || EndBorder == PosTA)
    {
        EndPer = GetBegOfPeriod(EndDate, RegPeriod);
    }
      else
    {
        if (fEndDate < 0 && EndDate.m_DateNum == GetBegOfPeriod(EndDate, RegPeriod).m_DateNum)
        {
          EndPer = EndDate;
        }
        else
		{
          EndPer = GetBegOfPeriod(AddDays(GetBegOfPeriod(EndDate, RegPeriod), -1), RegPeriod);
          TmpStr.Format("%s.date_time_iddoc > %s and %s.date_time_iddoc < %s", sDateTimeName, GetVTStringParam(CDateToCString(GetBegOfPeriod(EndDate, RegPeriod))), sDateTimeName, GetVTStringParam(EndBorder));
          if (!StrWhere.IsEmpty())
            {
			    StrWhere.Insert(0, '(');
            StrWhere += " or ";
          StrWhere += TmpStr;
                StrWhere += ')';
            }
		    else
            {
			    StrWhere = TmpStr;
            }
        };
        };
        fOptimization = (BegPer.m_DateNum <= EndPer.m_DateNum);
    if (RegPeriod == eQuart || RegPeriod == eYear)
        EndPer.m_DateNum = GetEndOfPeriod(EndPer, RegPeriod).m_DateNum;
  };

  if (fOptimization)
  {
    CString sBegPer = CDateToPeriod(BegPer);
    CString sEndPer = CDateToPeriod(EndPer);
    if (m_RPCMode)
    {
        sBegPer = GetCPName(sBegPer, "smalldatetime");
        sEndPer = GetCPName(sEndPer, "smalldatetime");
    };

    CString sTotName(m_sTempFieldPrefix);
    sTotName.SetAt(1,'g');

    SubQuery = "select\r\n";
    if (StrWhere.IsEmpty())
    {
      AddMDObjArray(SubQuery, Dims, sTotName + ".%id as %cd", ",", 1);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, sTotName + ".period", 1, TurnoversPlan.BegLim);
        SubQuery +=  " as Период,\r\n";
      };
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sTotName + ".%id) as %cdОборот", ",");

      TmpStr.Format("from rg%d as %s (nolock)\r\n", RegDef->GetID(), sTotName); SubQuery += TmpStr;
      AddSubToken(SubQuery, Joines, 'g', arJoinesPS);

      TmpStr.Format("where %s.period between %s and %s\r\n", sTotName, sBegPer, sEndPer); SubQuery += TmpStr;
      switch (RegPeriod)
      {
        case eQuart:
            TmpStr.Format(" and month(%s.period) in (3, 6, 9, 12)\r\n", sTotName); SubQuery += TmpStr;
            break;
        case eYear:
            TmpStr.Format(" and month(%s.period) = 12\r\n", sTotName); SubQuery += TmpStr;
            break;
      };
      AddSubToken(SubQuery, Conditions, 'g', arConditionsPS);

      SubQuery += "group by\r\n";
      AddMDObjArray(SubQuery, Dims, sTotName + ".%id", ",", TurnoversPlan.Periodicity != e_NotUsed);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, sTotName + ".period", 1, TurnoversPlan.BegLim);
        SubQuery += "\r\n";
      };
      SubQuery += "having\r\n";
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sTotName + ".%id) <> 0", " or");
    }
    else
    {
      CString sVTName(m_sTempFieldPrefix);
      sVTName.SetAt(1,'t');

      AddMDObjArray(SubQuery, Dims, sVTName + ".%cd", ",", 1);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        SubQuery += sVTName;
        SubQuery += ".Период,\r\n";
      };
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sVTName + ".%cd) as %cdОборот", ",");
      SubQuery += "from (\r\nselect\r\n";
      AddMDObjArray(SubQuery, Dims, sTotName + ".%id as %cd", ",", 1);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, sTotName + ".period", 1, TurnoversPlan.BegLim);
        SubQuery +=  " as Период,\r\n";
      };
      AddMDObjArray(SubQuery, Figs, sTotName + ".%id as %cd", ",");

      TmpStr.Format("from rg%d as %s (nolock)\r\n", RegDef->GetID(), sTotName); SubQuery += TmpStr;
      AddSubToken(SubQuery, Joines, 'g', arJoinesPS);

      TmpStr.Format("where %s.period between %s and %s\r\n", sTotName, sBegPer, sEndPer); SubQuery += TmpStr;
      switch (RegPeriod)
      {
        case eQuart:
            TmpStr.Format(" and month(%s.period) in (3, 6, 9, 12)\r\n", sTotName); SubQuery += TmpStr;
            break;
        case eYear:
            TmpStr.Format(" and month(%s.period) = 12\r\n", sTotName); SubQuery += TmpStr;
            break;
      };
      AddSubToken(SubQuery, Conditions, 'g', arConditionsPS);

      SubQuery += "union all\r\nselect\r\n";
      AddMDObjArray(SubQuery, Dims, sTurnName + ".%id", ",", 1);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        TmpStr = sDateTimeName;
        TmpStr += '.';
        TmpStr += NameOfPositionField;
        SubQuery += GetPeriodField(TurnoversPlan.Periodicity, TmpStr, 0, TurnoversPlan.BegLim);
        SubQuery +=  ",\r\n";
      };
      AddMDObjArray(SubQuery, Figs, sTurnName + ".%id", ",");

      TmpStr.Format("from ra%d as %s (nolock)\r\n", RegDef->GetID(), sTurnName); SubQuery += TmpStr;
      AddSubToken(SubQuery, Joines, 'a', arJoinesPS);

      if (!RegDef->GetDateTimeInActs())
      {
        TmpStr.Format("inner join _1sjourn as %s (nolock) on %s.iddoc = %s.iddoc\r\n", sDateTimeName, sDateTimeName, sTurnName); SubQuery += TmpStr;
      };
      SubQuery += "where ";
      SubQuery += StrWhere;
      SubQuery += "\r\n";
      if (!RegDef->GetDateTimeInActs())
      {
          TmpStr.Format("and %s.rf%d = 0x1\r\n", sDateTimeName, RegDef->GetID()); SubQuery += TmpStr;
      };
      AddSubToken(SubQuery, Conditions, 'a', arConditionsPS);

      TmpStr.Format(") as %s\r\n", sVTName); SubQuery += TmpStr;
      SubQuery += "group by\r\n";
      AddMDObjArray(SubQuery, Dims, sVTName + ".%cd", ",", TurnoversPlan.Periodicity != e_NotUsed);
      if (TurnoversPlan.Periodicity != e_NotUsed)
      {
        SubQuery += sVTName;
        SubQuery += ".Период\r\n";
      };
      SubQuery += "having\r\n";
      AddMDObjArray(SubQuery, Figs, CString("sum(") + sVTName + ".%cd) <> 0", " or");
    };
  }
  else
  {
    TurnoversPlan.BeginBorder = BeginBorder;
    TurnoversPlan.EndBorder = EndBorder;
    BuildRegTurnoversSubQuery(RegDef, Dims, Figs, Joines, Conditions, TurnoversPlan, SubQuery);
  };
};

void CMetaNameParser::BuildRegRestsTurnovers(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias)
{
  if (!RegDef->IsRests())
    RaiseException(ExceptStr[eiVirtTableNotFound]);
  
  if (!RegDef->GetNPropDefs())
    RaiseException(ExceptStr[eiDimsNotDef]);
  if (!RegDef->GetNFigureDefs())
    RaiseException(ExceptStr[eiFigsNotDef]);
  
  long RegId = RegDef->GetID();
  TmpAlias.Format("RestsTurnovers%d", RegId);
  
  POSITION ListPos = GetVirtParams(8, Params);
  CString BeginBorder = Params.GetNext(ListPos);
  CString EndBorder = Params.GetNext(ListPos);
  CString StrPeriodicity = Params.GetNext(ListPos);
  CString StrMethod = Params.GetNext(ListPos);
  CString Joines = Params.GetNext(ListPos);
  CString Conditions = Params.GetNext(ListPos);
  CString Dimentions = Params.GetNext(ListPos);
  CString Figure = Params.GetNext(ListPos);
  
  CDate DateTA(0,0,0), BeginDate(0,0,0), EndDate(0,0,0);
  int fEndDate, fBegDate;
  CString PosTA = GetTABorder(DateTA);
  PeriodType RestsSnapShotPeriod = m_pIBSet->GetRestsSnapShotPeriod();
  
  if (EndBorder.IsEmpty()) {
    EndBorder = PosTA;
    EndDate = DateTA;
    fEndDate = 1;
  } else {
    fEndDate = GetBorderParameter(EndBorder, EndDate, 2);
    if (EndBorder > PosTA) {
      EndBorder = PosTA;
      EndDate = DateTA;
      fEndDate = 1;
    };
  };
  
  if (BeginBorder.IsEmpty()) {
    BeginBorder = "19000101";
    BeginDate = CDate(1900, 1, 1);
    fBegDate = -1;
  } else {
    fBegDate = GetBorderParameter(BeginBorder, BeginDate, 1);
    if (BeginBorder > EndBorder) {
      BeginBorder = EndBorder;
      BeginDate = EndDate;
      fBegDate = fEndDate;
    };
  };
  
  m_sTempFieldPrefix.Format("r_%d_vt", RegDef->GetID());
  stTurnoversPlan TurnoversPlan;
  TurnoversPlan.Periodicity = GetPeriodicityFromCString(StrPeriodicity, 3);
  //TurnoversPlan.Periodicity = e_NotUsed;
  TurnoversPlan.BegLim = BeginDate;
  
  eMethodRT MethodRT;
  
  if (StrMethod.IsEmpty())
    MethodRT = eActionsBoundaries;
  else if (StrMethod.CollateNoCase("Движения") == 0 || StrMethod.CollateNoCase("Actions") == 0)
    MethodRT = eActions;
  else if (StrMethod.CollateNoCase("ДвиженияИГраницыПериода") == 0 || StrMethod.CollateNoCase("ActionsAndPeriodBoundaries") == 0)
    MethodRT = eActionsBoundaries;
  else
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(4));
  
  MDArraysItem MDArrays[1];
  MDArrays[0].MDObjArray = RegDef->GetProps();
  MDArrays[0].IsUsed = false;
  ProcessJoinesConditions(Joines, Conditions, MDArrays, 1, 5);
  
  PMetaDataObjArray Dims;
  CStringToMDObjArray(Dims, Dimentions, RegDef->GetProps(), 7);
  PMetaDataObjArray Figs;
  CStringToMDObjArray(Figs, Figure, RegDef->GetFigure(), 8);
  
  stRestsPlan RestsPlan;
  RestTurnQueryType QueryType = rtqtByBegin;
  GetRestsQueryPlan(BeginBorder, BeginDate, fBegDate, PosTA, DateTA, RestsSnapShotPeriod, RestsPlan);
  if (RestsPlan.QType != rqNoTurnovers) {
    stRestsPlan RestsPlanEnd;
    GetRestsQueryPlan(EndBorder, EndDate, fEndDate, PosTA, DateTA, RestsSnapShotPeriod, RestsPlanEnd);
    if (RestsPlanEnd.QType == rqNoTurnovers) {
      QueryType = rtqtByEnd;
      RestsPlan = RestsPlanEnd;
    } else {
      if (RestsPlan.GetIntervalLength() > RestsPlanEnd.GetIntervalLength()) {
        QueryType = rtqtByEnd;
        RestsPlan = RestsPlanEnd;
      };
    };
  };
  TurnoversPlan.BeginBorder = BeginBorder;
  TurnoversPlan.EndBorder = EndBorder;
  
  CString TmpStr;
  if (TurnoversPlan.Periodicity == e_NotUsed)
  {
    CString sVTName(m_sTempFieldPrefix);
    sVTName.SetAt(1,'b');

    SubQuery = "select\r\n";
    AddMDObjArray(SubQuery, Dims, sVTName + ".%cd", ",", 1);
    if (QueryType == rtqtByBegin)
    {
      TmpStr = "sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) as %cdНачальныйОстаток,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) as %cdПриход,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) as %cdРасход,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) + sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) - sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) as %cdКонечныйОстаток";
    }
    else
    {
      TmpStr = "sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) - sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) + sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) as %cdНачальныйОстаток,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) as %cdПриход,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) as %cdРасход,\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) as %cdКонечныйОстаток";
    };
    AddMDObjArray(SubQuery, Figs, TmpStr, ",");

    SubQuery += "from (\r\n";
    TurnoversPlan.EnlargeStr = ",\r\n0 as %cdОстаток";
    BuildRegTurnoversSubQuery(RegDef, Dims, Figs, Joines, Conditions, TurnoversPlan, TmpStr);

    SubQuery += TmpStr;
    SubQuery += "union all\r\n";
    RestsPlan.EnlargeStr = "0,\r\n0,\r\n";
    BuildRegRestsSubQuery(RegDef, Dims, Figs, Joines, Conditions, RestsPlan, TmpStr);
    SubQuery += TmpStr;

    TmpStr.Format(") as %s\r\n", sVTName); SubQuery += TmpStr;
    SubQuery += "group by\r\n";
    AddMDObjArray(SubQuery, Dims, sVTName + ".%cd", ",");
    SubQuery += "having\r\n";
    if (QueryType == rtqtByBegin)
    {
      TmpStr = "sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) + sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) - sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) <> 0";
    }
    else
    {
      TmpStr = "sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) - sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) + sum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdПриход) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdРасход) <> 0 or\r\nsum(";
      TmpStr += sVTName;
      TmpStr += ".%cdОстаток) <> 0";
    };
    AddMDObjArray(SubQuery, Figs, TmpStr, " or");
  }
  else
  {
    if (m_db == NULL)
      RaiseException(ExceptStr[eiImpossible]);
    
    TmpAlias.Empty();
    CODBCRecordset Recordset(m_db);
    stInsPlan InsPlan;
    InsPlan.DimsCount = Dims.GetSize();
    InsPlan.FigsCount = Figs.GetSize();
    InsPlan.Periodicity = TurnoversPlan.Periodicity;
    InsPlan.QueryType = QueryType;
    InsPlan.FieldsCount = InsPlan.DimsCount + 1;
    if (TurnoversPlan.Periodicity == e_Document)
      InsPlan.FieldsCount++;
    InsPlan.nFirstFigField = InsPlan.FieldsCount;
    InsPlan.FieldsCount += InsPlan.FigsCount<<2;
    
    //Обороты
    BuildRegTurnoversSubQuery(RegDef, Dims, Figs, Joines, Conditions, TurnoversPlan, SubQuery);
    SubQuery += "order by\r\n";
    AddMDObjArray(SubQuery, Dims, "%cd", ",", 1);
    if (TurnoversPlan.Periodicity == e_Document)
      SubQuery +=  "ПозицияДокумента";
    else
      SubQuery += "Период";
    if (QueryType == rtqtByBegin)
      SubQuery += "\r\n";
    else
      SubQuery += " desc\r\n";
    
    //      pBkEndUI->DoMessageLine(SubQuery, mmNone);
    
    BOOL fTurnIsNext = Recordset.Open(SubQuery);
    CheckSQLError(Recordset.IsOpen(), Recordset);
    
    CRow_allocator TurnRow_allocator_b(Dims, InsPlan.FigsCount,
      TurnoversPlan.Periodicity == e_Document?CRow_allocator::eTurnsDoc:CRow_allocator::eTurnsPer);
    typedef vector<CRow_allocator::stRow, CRow_allocator> Turns_vector;
    Turns_vector vecTurns(TurnRow_allocator_b);
    
    while (fTurnIsNext) {
      TurnRow_allocator_b.SetRowData(Recordset);
      vecTurns.push_back(TurnRow_allocator_b.Row);
      fTurnIsNext = Recordset.MoveNext();
    };
    
    Recordset.Close();
    /*
    pBkEndUI->DoMessageLine("Turns:", mmNone);
    CRow_allocator::stRow* it;
    for (it = vecTurns.begin(); it != vecTurns.end(); it++) {
    pBkEndUI->DoMessageLine(CString(it->m_pDims, TurnRow_allocator_b.GetJustDimsSize()), mmNone);
    }
    */
    //Остатки
    BuildRegRestsSubQuery(RegDef, Dims, Figs, Joines, Conditions, RestsPlan, SubQuery);
    SubQuery += "order by\r\n";
    AddMDObjArray(SubQuery, Dims, "%cd", ",");
    
    //      pBkEndUI->DoMessageLine(SubQuery, mmNone);
    BOOL fRestIsNext = Recordset.Open(SubQuery);
    CheckSQLError(Recordset.IsOpen(), Recordset);
    
    CRow_allocator RestRow_allocator(Dims, InsPlan.FigsCount, CRow_allocator::eRests);
    typedef vector<CRow_allocator::stRow, CRow_allocator> Rests_vector;
    Rests_vector vecRests(RestRow_allocator);
    
    while (fRestIsNext) {
      RestRow_allocator.SetRowData(Recordset);
      vecRests.push_back(RestRow_allocator.Row);
      fRestIsNext = Recordset.MoveNext();
    };
    
    Recordset.Close();
    /*
    pBkEndUI->DoMessageLine("Rests:", mmNone);
    //CRestRow_allocator::stRestRow* it;
    for (it = vecRests.begin(); it != vecRests.end(); it++) {
    pBkEndUI->DoMessageLine(CString(it->m_pDims, RestRow_allocator.GetJustDimsSize()), mmNone);
    }
    */
    //Создание временной таблицы
    _GUID GUID;
    CoCreateGuid(&GUID);
    SubQuery.Format("#T%08lX%04X%04X%02X%02X%02X%02X%02X%02X%02X%02X",
      GUID.Data1, GUID.Data2, GUID.Data3,
      GUID.Data4[0], GUID.Data4[1], GUID.Data4[2], GUID.Data4[3],
      GUID.Data4[4], GUID.Data4[5], GUID.Data4[6], GUID.Data4[7]);
    
    CString CreateQuery;
    CString sql_type;
    TmpStr.Format("create table %s (\r\n", SubQuery); CreateQuery += TmpStr;
    for (int i = 0; i < Dims.GetSize(); i++)
    {
      const CType & t = static_cast<CMetaDataTypedObj *>(Dims[i])->GetType();
      switch(t.GetTypeCode())
      {
      case 0: sql_type = "char(23)";
        break;
      case 1: sql_type.Format("numeric(%d,%d)", t.GetPrecision()>0?t.GetLength()-1:t.GetLength(), t.GetPrecision());
        break;
      case 2: sql_type.Format("char(%d)", t.GetLength());
        break;
      case 3: sql_type = "datetime";
        break;
      default:
        if (!t.GetTypeID())
          sql_type = "char(13)";
        else
          sql_type = "char(9)";
      }
      TmpStr.Format("%s %s,\r\n", Dims[i]->GetCode(), sql_type); CreateQuery += TmpStr;
    }
    if (TurnoversPlan.Periodicity == e_Document)
      CreateQuery +=  "ПозицияДокумента char(23), ВидДокумента int,\r\n";
    else
      CreateQuery += "Период datetime,\r\n";
    
    CvecCurBalances CurBalances;
    stCurBalance CurBalance;
    for (i = 0; i < Figs.GetSize(); i++)
    {
      int Prec = static_cast<CMetaDataTypedObj *>(Figs[i])->GetType().GetPrecision();
      sql_type.Format("numeric(38,%d)", Prec);
      TmpStr = "%cdНачальныйОстаток %ty,\r\n%cdПриход %ty,\r\n%cdРасход %ty,\r\n%cdКонечныйОстаток %ty";
      TmpStr.Replace("%cd", Figs[i]->GetCode());
      TmpStr.Replace("%ty", sql_type);
      
      if (i == Figs.GetSize() - 1)
        CreateQuery += TmpStr + "\r\n";
      else
        CreateQuery += TmpStr + ",\r\n";
      
      CurBalance.FactorOfAccuracy = pow(10, -Prec);
      CurBalance.Balance = 0;
      CurBalances.push_back(CurBalance);
    }
    CreateQuery += ")";
    
    //      pBkEndUI->DoMessageLine(CreateQuery, mmNone);
    CheckSQLError(Recordset.Execute(CreateQuery), Recordset);
    
    m_OnCloseStatments.Format("if exists(select * from tempdb..sysobjects where id = object_id('tempdb..%s'))\r\ndrop table %s\r\n", SubQuery, SubQuery);
    
    //Формирование временной таблицы
    CString InsQuery = "insert " + SubQuery + " values (";
    int Len = InsQuery.GetLength();
    char *ParamsStr = InsQuery.GetBufferSetLength(Len + (InsPlan.FieldsCount<<1));
    ParamsStr += Len;
    for (i = 0; i < InsPlan.FieldsCount; i++) {
      *ParamsStr++ = '?';
      *ParamsStr++ = ',';
    };
    *--ParamsStr = ')';
    InsQuery.ReleaseBuffer();
    
    //      pBkEndUI->DoMessageLine(InsQuery, mmNone);
    CheckSQLError(Recordset.Prepare(InsQuery), Recordset);
    
    Recordset.SetRowsetSizeForParams(INS_ROWSET_SIZE);
    CheckSQLError(Recordset.BuildParams(), Recordset);
    
    NumCurRow = 1;
    int DimsSize = RestRow_allocator.GetJustDimsSize();
    
    CRow_allocator TurnRow_allocator_e(TurnRow_allocator_b);
    if (InsPlan.Periodicity == e_Document)
    {
      CString s;
      s = TurnoversPlan.BeginBorder;
      if (s.GetLength() < 23)
      {
        strnset(s.GetBufferSetLength(23) + TurnoversPlan.BeginBorder.GetLength(), ' ',
          23 - TurnoversPlan.BeginBorder.GetLength());
        s.ReleaseBuffer();
      };
      TurnRow_allocator_b.SetRowPeriod(s, 0);
      
      s = TurnoversPlan.EndBorder;
      if (s.GetLength() < 23)
      {
        strnset(s.GetBufferSetLength(23) + TurnoversPlan.EndBorder.GetLength(), ' ',
          23 - TurnoversPlan.EndBorder.GetLength());
        s.ReleaseBuffer();
      }
      TurnRow_allocator_e.SetRowPeriod(s, 0);
    }
    else
    {
      if (QueryType == rtqtByBegin)
      {
      TurnRow_allocator_b.SetRowPeriod(CDateToDATE(BeginDate));
      TurnRow_allocator_e.SetRowPeriod(CDateToDATE(EndDate));
    }
      else
      {
          TurnRow_allocator_b.SetRowPeriod(CDateToDATE(EndDate));
          TurnRow_allocator_e.SetRowPeriod(CDateToDATE(BeginDate));
      }
    }
    
    Turns_vector::iterator TurnsRow = vecTurns.begin();
    Turns_vector::iterator PrevTurnsRow;
    Rests_vector::iterator RestsRow = vecRests.begin();
    int ResT,ResR;
    
    for (; TurnsRow != vecTurns.end(); TurnsRow++) 
    {
      if (TurnsRow == vecTurns.begin())
        ResT = 1;
      else
        ResT = memcmp(TurnsRow->m_pDims, PrevTurnsRow->m_pDims, DimsSize);
      
      if (ResT)
      {
        if (MethodRT == eActionsBoundaries && TurnsRow != vecTurns.begin())
          if (CurBalancesNotEQ(CurBalances))
            if (TurnRow_allocator_e.CompRowPeriod(PrevTurnsRow))
              InsertRow(PrevTurnsRow, FALSE, TurnRow_allocator_e, InsPlan, Recordset, CurBalances);
            
            ResR = 1;
            for (; RestsRow != vecRests.end(); RestsRow++)
            {
              ResR = memcmp(RestsRow->m_pDims, TurnsRow->m_pDims, DimsSize);
              if (ResR >= 0)
                break;
              if (MethodRT == eActionsBoundaries)
              {
                SetBalancesFromArray(CurBalances, RestsRow->m_pFigs);
                InsertRow(RestsRow, FALSE, TurnRow_allocator_b, InsPlan, Recordset, CurBalances);
              }
            }
            
            if (ResR)
              for (stCurBalance* CurBalance = CurBalances.begin(); CurBalance != CurBalances.end(); CurBalance++)
                CurBalance->Balance = 0;
              else
              {
                SetBalancesFromArray(CurBalances, RestsRow->m_pFigs);
                RestsRow++;
              }
              
              if (MethodRT == eActionsBoundaries && !ResR)
                if (TurnRow_allocator_b.CompRowPeriod(TurnsRow))
                  InsertRow(TurnsRow, FALSE, TurnRow_allocator_b, InsPlan, Recordset, CurBalances);
      }
      InsertRow(TurnsRow, TRUE, TurnRow_allocator_b, InsPlan, Recordset, CurBalances);
      PrevTurnsRow = TurnsRow;
    };
    
    if (MethodRT == eActionsBoundaries)
      for (; RestsRow != vecRests.end(); RestsRow++)
      {
        SetBalancesFromArray(CurBalances, RestsRow->m_pFigs);
        InsertRow(RestsRow, FALSE, TurnRow_allocator_b, InsPlan, Recordset, CurBalances);
      };
      
      CheckSending(Recordset);
    };
};

void CMetaNameParser::SetBalancesFromArray(CvecCurBalances & CurBalances, const double * Array)
{
  const double * pArray = Array;
  for (stCurBalance* CurBalance = CurBalances.begin(); CurBalance != CurBalances.end(); CurBalance++)
    CurBalance->Balance = *pArray++;
};

BOOL CMetaNameParser::CurBalancesNotEQ(CvecCurBalances & CurBalances)
{
  for (stCurBalance* CurBalance = CurBalances.begin(); CurBalance != CurBalances.end(); CurBalance++)
    if (CurBalance->Balance != 0)
      return TRUE;
    return FALSE;
};

void CMetaNameParser::CheckSending(CODBCRecordset & Recordset)
{
  if (NumCurRow > 1) {
    Recordset.ResizeRowsetSizeForParams(--NumCurRow);
    CheckSQLError(Recordset.Execute(NULL, TRUE), Recordset);
  };
};

double CMetaNameParser::round(const double & x, const double & accuracy)
{
  if (accuracy == 1)
    return x;
  else
    return floor(x / accuracy + 0.5) * accuracy;
}

void CMetaNameParser::InsertRow(const CRow_allocator::stRow* pRowSourse,
                                BOOL IsActions,
                                const CRow_allocator & Row_allocator,
                                const stInsPlan & InsPlan,
                                CODBCRecordset & InsRecordset,
                                CvecCurBalances & CurBalances)
{
  Row_allocator.SetDimsToRecordset(InsRecordset, pRowSourse->m_pDims);
  if (IsActions)
  {
    Row_allocator.SetPeroidToRecordset(InsRecordset, pRowSourse->m_pDims);
    
    stCurBalance* CurBalance = CurBalances.begin();
    for (int i = 0; i < InsPlan.FigsCount; i++) 
    {
      int nFigField = InsPlan.nFirstFigField + (i<<2);
      
      double Receipt = Row_allocator.GetFig(pRowSourse->m_pFigs, i, 0);
      double Expense = Row_allocator.GetFig(pRowSourse->m_pFigs, i, 1);
      
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 1)) = Receipt;
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 2)) = Expense;
      
      if (InsPlan.QueryType == rtqtByBegin) {
        static_cast<CDBField&>(InsRecordset.GetParam(nFigField)) = CurBalance->Balance;
        CurBalance->Balance = round(CurBalance->Balance + Receipt - Expense, CurBalance->FactorOfAccuracy);
        static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 3)) = CurBalance->Balance;
      } else {
        static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 3)) = CurBalance->Balance;
        CurBalance->Balance = round(CurBalance->Balance + Expense - Receipt, CurBalance->FactorOfAccuracy);
        static_cast<CDBField&>(InsRecordset.GetParam(nFigField)) = CurBalance->Balance;
      }
      CurBalance++;
    };
  }
  else
  {
    Row_allocator.SetPeroidToRecordset(InsRecordset, Row_allocator.Row.m_pDims);
    
    stCurBalance* CurBalance = CurBalances.begin();
    for (int i = 0; i < InsPlan.FigsCount; i++) 
    {
      int nFigField = InsPlan.nFirstFigField + (i<<2);
      
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField)) = CurBalance->Balance;
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 1)) = 0;
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 2)) = 0;
      static_cast<CDBField&>(InsRecordset.GetParam(nFigField + 3)) = CurBalance->Balance;
      CurBalance++;
    };
  };
  
  void *pData = NULL;
  int size = 0;
  for (int i = 0; i < InsPlan.FieldsCount; i++) {
    if (InsRecordset.GetParam(i).GetBufferAndSize(&pData, size))
      InsRecordset.SetParamOfRow(NumCurRow, i + 1, size, pData);
  };
  ++NumCurRow;
  
  if (INS_ROWSET_SIZE < NumCurRow) {
    CheckSQLError(InsRecordset.Execute(NULL, TRUE), InsRecordset);
    NumCurRow = 1;
  };
  
  //CheckSQLError(InsRecordset.Execute(NULL, TRUE));
};

void CMetaNameParser::BuildLastValue(const CMetaDataTypedObj * MDObj, CString & SubQuery, CStringList & Params)
{
  POSITION ListPos = GetVirtParams(4, Params);
  CString ObjID = Params.GetNext(ListPos);
  CString Date = Params.GetNext(ListPos);
  CString Time = Params.GetNext(ListPos);
  CString DocID = Params.GetNext(ListPos);
  
  if (ObjID.IsEmpty())
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(1));
  if (Date.IsEmpty())
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(2));
  
  CString TmpStr;
  SubQuery = "select top 1\r\n";
  CString sAlias;
  sAlias.Format("c%d_vv", MDObj->GetID());
  switch (MDObj->GetType().GetTypeCode())
  {
  case UNDEFINE_TYPE_1C:
      SubQuery += "left(";
      SubQuery += sAlias;
      SubQuery += ".value, 23)\r\n";
    break;
  case NUMBER_TYPE_1C:
        TmpStr.Format("cast(%s.value as numeric(%d, %d))\r\n", sAlias, MDObj->GetType().GetLength() - !!MDObj->GetType().GetPrecision(), MDObj->GetType().GetPrecision());
        SubQuery += TmpStr;
    break;
  case STRING_TYPE_1C:
      TmpStr.Format("left(%s.value, %d)\r\n", sAlias, MDObj->GetType().GetLength());
      SubQuery += TmpStr;
    break;
  case DATE_TYPE_1C:
    {
      TmpStr = "cast(case when %id.value = '  .  .    '\r\n\
        then '17530101'\r\n\
        else substring(%id.value, 7, 4) + substring(%id.value, 4, 2) + substring(%id.value, 1, 2)\r\n\
        end as datetime)\r\n";
      TmpStr.Replace("%id", sAlias);
      SubQuery += TmpStr;
    }
    break;
  default:
      TmpStr.Format("left(%s.value, %d)\r\n", sAlias, 9 + 4 * !MDObj->GetType().GetTypeID());
      SubQuery += TmpStr;
    break;
  };
  SubQuery += "from\r\n_1sconst as ";
  SubQuery += sAlias;
  SubQuery += " (nolock)\r\nwhere\r\n";
  TmpStr.Format("%s.id = %d and\r\n", sAlias, MDObj->GetID()); SubQuery += TmpStr;
  TmpStr.Format("%s.objid = %s and\r\n", sAlias, ObjID); SubQuery += TmpStr;
  SubQuery += '(';
  SubQuery += sAlias;
  SubQuery += ".date <";
  if (Time.IsEmpty())
    SubQuery += '=';
  SubQuery += ' ';
  SubQuery += Date;
  if (!Time.IsEmpty())
  {
    TmpStr.Format(" or %s.date = %s and %s.time <", sAlias, Date, sAlias); SubQuery += TmpStr;
    if (DocID.IsEmpty())
      SubQuery += '=';
    SubQuery += ' ';
    SubQuery += Time;
    if (!DocID.IsEmpty())
    {
      TmpStr.Format(" or %s.date = %s and %s.time = %s and %s.docid <= %s", sAlias, Date, sAlias, Time, sAlias, DocID); SubQuery += TmpStr;
    };
  };
  SubQuery += ')';
  TmpStr.Format("\r\norder by %s.date desc, %s.time desc, %s.docid desc, %s.row_id desc\r\n", sAlias, sAlias, sAlias, sAlias); SubQuery += TmpStr;
};

void CMetaNameParser::BuildBegOfPeriod(CString & SubQuery, const CString & StrPeriodType, CStringList & Params)
{
  POSITION ListPos = GetVirtParams(2, Params);
  CString Sourse = Params.GetNext(ListPos);
  CString StrSourseType = Params.GetNext(ListPos);
  
  if (Sourse.IsEmpty())
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(1));
  if (StrPeriodType.IsEmpty())
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(2));
  PeriodType Per = GetPeriodicityFromCString(StrPeriodType, 2);
  if (Per == e_NotUsed || Per == e_Document)
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(2));
  if (StrSourseType.IsEmpty())
    StrSourseType = "Строка";
  
  int SourseType;
  if (!StrSourseType.CollateNoCase("Дата") || !StrSourseType.CollateNoCase("Date"))
    SourseType = 1;
  else if (!StrSourseType.CollateNoCase("Строка") || !StrSourseType.CollateNoCase("String"))
    SourseType = 0;
  else
    RaiseException(ExceptStr[eiWrongParam] + DescParameter(2));
  
  SubQuery = GetPeriodField(Per, Sourse, SourseType, CDate(0, 0, 0));
};

void CMetaNameParser::CheckSQLError(const BOOL & Res, CODBCRecordset & rs)
{
  if (!Res)
    RaiseException(CString(ExceptStr[eiImpossible]) + rs.GetLastError());
};

void CMetaNameParser::OnClose()
{
  if (!m_OnCloseStatments.IsEmpty() && m_db != NULL)
  {
    CODBCRecordset Recordset(m_db);
    CheckSQLError(Recordset.Execute(m_OnCloseStatments), Recordset);
    m_OnCloseStatments.Empty();
  };
};

int CMetaNameParser::GetSQLParamNumber(const CValue & nstrParam)
{
  if (nstrParam.GetTypeCode() == NUMBER_TYPE_1C)
  {      
    return nstrParam.GetNumeric().operator long();
  }
  else
  {
    SQLParam SQLPar;
    if (SQLParams.Lookup(nstrParam.GetString(), SQLPar))
      return SQLPar.Number;
    else
      return 0;
  };
};

int CMetaNameParser::GetSQLParamModifikator(const int nParam)
{
  SQLParam Par;
  CString Name;
  POSITION pos = SQLParams.GetStartPosition();
  while (pos != NULL)
  {
    SQLParams.GetNextAssoc(pos, Name, Par);
    if (Par.Number == nParam)
      return Par.Modificator;
  }
  return 0;
};

void CMetaNameParser::SetSQLParamsModifikators(QueryParamArray & ParamArray)
{
  SQLParam Par;
  CString Name;
  POSITION pos = SQLParams.GetStartPosition();
  while (pos != NULL)
  {
    SQLParams.GetNextAssoc(pos, Name, Par);
    if (Par.Number <= ParamArray.GetSize())
      ParamArray[Par.Number - 1].m_Modificator = Par.Modificator;
    else
      break;
  }
};

void CMetaNameParser::ClearParams()
{
  Params.RemoveAll();
  m_RPCParams.RemoveAll();
  m_UseTextParamsInRPC.RemoveAll();
}

void CMetaNameParser::SetTypeID36Literal(CString & Literal, long TypeID)
{
  char* pStr = Literal.GetBufferSetLength(6);
  *pStr++ = '\'';
  LongToCharID36(TypeID, pStr, 4);
  pStr[4] = '\'';
  Literal.ReleaseBuffer();
}

void CMetaNameParser::ProcessForFox()
{
  CurIndex = 0;
  bool fStrConst = false;
  bool fComment = false;
  bool fCommentType;
  bool fExtName = false;
  
  CString EmptyStr;
  char CurChar;
  const char *pQT = (LPCTSTR) QueryText;
  
  while (CurChar = pQT[CurIndex++])
  {
    if (fComment)
    {//идет комментарий
      if (fCommentType)
        fComment = (CurChar != '\n');
      else
        fComment = !((CurChar == '*') && (pQT[CurIndex] == '/'));
      
      if (!fComment)
      {
        if (fCommentType)
          CurIndex -= 2;
        else
          ++CurIndex;
        PasteName(EmptyStr);
        pQT = QueryText;
      };
      continue;
    }
    if (fStrConst)
    {// идет строковая константа
      fStrConst = CurChar != '\'';
      continue;
    }
    if (fExtName)
    {// идет типизирующий алиас
      fExtName = CurChar != ']';
      
      if (!fExtName)
      {
        CString TypingAlias(pQT + Start - 1, CurIndex - Start + 1);
        TypingAlias.Replace(" ", EmptyStr);
        char buf[2] = {BeginMetaNameSimbol, 0};
        TypingAlias.Replace(buf, TypeNamePrefix);
        TypingAlias.Replace('.', '_');
        PasteName(TypingAlias);
        pQT = QueryText;
      };
      continue;
    }
    switch (CurChar) {
    case '-': //комментарий типа --
      if (pQT[CurIndex] == '-') 
      {
        Start = CurIndex;
        fComment = true;
        fCommentType = true;
      }
      break;
    case '/': //комментарий типа /*
      if (pQT[CurIndex] == '*')
      {
        Start = CurIndex;
        fComment = true;
        fCommentType = false;
      }
      break;
    case '\'': // строковая константа ''
      fStrConst = true;
      break;
    case '[': // типизирующий алиас
      Start = CurIndex;
      fExtName = true;
      break;
    }
  }
  
  if (fComment && fCommentType)
  {
    --CurIndex;
    PasteName(EmptyStr);
  };
}

// CMetaNameParser
//////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////
// CMNPException

CMNPException::CMNPException(BOOL b_AutoDelete, CString & aErrorDesc, int CurrentIndex):
CException(b_AutoDelete),
ErrorDesc(aErrorDesc)
{
  CurIndex = CurrentIndex;
};

CString CMNPException::GetErrorDescr()
{
  return ErrorDesc;
};

int CMNPException::GetCurrentIndex()
{
  return CurIndex;
};

BOOL CMNPException::GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext)
{
  pnHelpContext = NULL;
  strncpy(lpszError, ErrorDesc, min(nMaxError, ErrorDesc.GetLength()));
  return TRUE;
};
// CMNPException
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////
// CIBSet clases
//
CDate CIBSet::GetDateTimeTA(CEventTime &TimeTA)
{
    TimeTA = GetSysSet()->GetTimeTA();
    return GetSysSet()->GetDateTA();
}
int CIBSet::GetDataSourceType()
{
    return pDataBase7->GetDataSourceType();
}
enum PeriodType CIBSet::GetRestsSnapShotPeriod()
{
    return ::GetRestsSnapShotPeriod();
}
/////////
CDate CIBSetSQL::GetDateTimeTA(CEventTime &TimeTA)
{
    if (!m_rs.Open("select curdate, curtime from _1ssystem (nolock)"))
    {
        const char* szLastError = m_rs.GetLastError();
        if (strlen(szLastError))
        {
            m_rs.Close();
            CBLModule::RaiseExtRuntimeError(szLastError, mmRedErr);
        }
    }
    CDBField* pDBField = m_rs.GetFields();
    COleDateTime date = pDBField[0].AsDate();
    long time = pDBField[1].AsLong();
    m_rs.Close();

    TimeTA.SetlTime(time);
    return CDate(date.GetYear(),date.GetMonth(),date.GetDay());
}
int CIBSetSQL::GetDataSourceType()
{
    return DATA_SOURCE_TYPE_SQL;
}
enum PeriodType CIBSetSQL::GetRestsSnapShotPeriod()
{
    if (!m_rs.Open("select snapshper from _1ssystem (nolock)"))
    {
        const char* szLastError = m_rs.GetLastError();
        if (strlen(szLastError))
        {
            m_rs.Close();
            CBLModule::RaiseExtRuntimeError(szLastError, mmRedErr);
        }
    }
    CDBField* pDBField = m_rs.GetFields();
    PeriodType snapshper = TypePeriodFromChar(pDBField[0].AsChar());
    m_rs.Close();
    return snapshper;
}

CDate CIBSetDBF::GetDateTimeTA(CEventTime &TimeTA)
{
	return CIBSet::GetDateTimeTA(TimeTA);
}
int CIBSetDBF::GetDataSourceType()
{
    return DATA_SOURCE_TYPE_DBF;
}
enum PeriodType CIBSetDBF::GetRestsSnapShotPeriod()
{
    return CIBSet::GetRestsSnapShotPeriod();
}
// CIBSet clases
/////////////////////////////////////////////////////////////
