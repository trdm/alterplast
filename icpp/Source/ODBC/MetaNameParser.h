//////////////////////////////////////////////////////////////////////
// MetaNameParser.h: interface for the CMetaNameParser class.
//
// Разработчик: Дмитрий Ощепков   mailto: dmitro@russian.ru
// Версия: 2.13
//////////////////////////////////////////////////////////////////////

#include "Odbccore.h"
#include "../istrmap.h"
#include <vector>

#if !defined(AFX_METANAMEPARSER_H__6CE71132_76A6_48DC_BE73_802D87E32A37__INCLUDED_)
#define AFX_METANAMEPARSER_H__6CE71132_76A6_48DC_BE73_802D87E32A37__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define DS_NUMERIC  1000
#define DS_DATE     1001

class CRow_allocator
{
public:
	class stRow
	{
	public:
		stRow() : m_pDims(NULL), m_pFigs(NULL), m_nDimsSize(0), m_nFigsSize(0)
		{
		}
		stRow(stRow const& rhs) : m_pDims(NULL), m_pFigs(NULL)
		{
			copy(rhs);
		}
		~stRow()
		{
			cleanup();
		}
		stRow& operator=(stRow const& rhs)
		{
			if (&rhs != this)
			{
				cleanup();
				copy(rhs);
			}
			return *this;
		}
		void resize(int nDimsSize, int nFigsSize)
		{
			cleanup();

			m_nDimsSize = nDimsSize;
			if (m_nDimsSize)
				m_pDims = reinterpret_cast<char*>(::operator new(m_nDimsSize));

			m_nFigsSize = nFigsSize;
			if (m_nFigsSize)
				m_pFigs = reinterpret_cast<double*>(::operator new(m_nFigsSize));
		}
	private:
		void cleanup()
		{
			if (m_pDims)
			{
				::operator delete(m_pDims);
				m_pDims = NULL;
			}

			if (m_pFigs)
			{
				::operator delete(m_pFigs);
				m_pFigs = NULL;
			}
		}
		void copy(stRow const& rhs)
		{
			resize(rhs.m_nDimsSize, rhs.m_nFigsSize);
			memcpy(m_pDims, rhs.m_pDims, m_nDimsSize);
			memcpy(m_pFigs, rhs.m_pFigs, m_nFigsSize);
		}

	public:
		char* m_pDims;
		double* m_pFigs;
	private:
		int m_nDimsSize;
		int m_nFigsSize;
	};

	CRow_allocator() {}
	int m_nDims, m_nFigs;
	enum rtType { eRests, eTurnsPer, eTurnsDoc } m_Type;
	std::vector<int> m_vecDimsSizes;
	int m_DimsSize, m_FigsSize;
	int m_PeriodSize, m_JustDimsSize;
	stRow m_Row;

	void SetSizes(PMetaDataObjArray& Dims)
	{
		//Dims
		m_DimsSize = 0;
		for (int i = 0; i < m_nDims; i++)
		{
			const CType& t = static_cast<CMetaDataTypedObj *>(Dims[i])->GetType();
			int CurSize;
			switch(t.GetTypeCode())
			{
			case 0: CurSize = 23;
				break;
			case 1: CurSize = DS_NUMERIC;
				break;
			case 2: CurSize = t.GetLength();
				break;
			case 3: CurSize = DS_DATE;
				break;
			default:
				CurSize = 9;
				if (!t.GetTypeID()) CurSize += 4;
			}
			m_vecDimsSizes.push_back(CurSize);
			if (CurSize == DS_DATE)
				m_DimsSize += sizeof(DATE);
			else if (CurSize == DS_NUMERIC)
				m_DimsSize += sizeof(double);
			else
				m_DimsSize += CurSize;
		}
		m_JustDimsSize = m_DimsSize;
		//Figs
		if (m_Type == eRests)
			m_FigsSize = m_nFigs<<3;
		else {
			m_FigsSize = m_nFigs<<4;
			if (m_Type == eTurnsPer)
				m_PeriodSize = sizeof(DATE); //for date
			else
				m_PeriodSize = 23 + sizeof(long); //for date_time_iddoc:23 and iddocdef
			m_DimsSize += m_PeriodSize;
		}
	}

	CRow_allocator(PMetaDataObjArray& Dims, int nFigs, rtType nrtType) : m_nDims(Dims.GetSize()), m_nFigs(nFigs), m_Type(nrtType)
	{
		SetSizes(Dims);
		ConstructObj(m_Row);
	}

	CRow_allocator(CRow_allocator const& rhs) : m_nDims(rhs.m_nDims), m_nFigs(rhs.m_nFigs), m_Type(rhs.m_Type),
		m_vecDimsSizes(rhs.m_vecDimsSizes),
		m_DimsSize(rhs.m_DimsSize), m_FigsSize(rhs.m_FigsSize),
		m_PeriodSize(rhs.m_PeriodSize), m_JustDimsSize(rhs.m_JustDimsSize)
	{
		ConstructObj(m_Row);
	}

	~CRow_allocator()
	{
// 		DestroyObj(m_Row);
	}

	void SetRowPeriod(const DATE & Date)
	{
		memcpy(m_Row.m_pDims + m_JustDimsSize, &Date, sizeof(DATE));
	}

	void SetRowPeriod(const char* DocPos, const long DocKind)
	{
		memcpy(m_Row.m_pDims + m_JustDimsSize, DocPos, 23);
		memcpy(m_Row.m_pDims + m_JustDimsSize + 23, &DocKind, sizeof(long));
	}

	int CompareRowPeriod(const stRow* SourceRow) const
	{
		int Size;
		if (m_Type == eTurnsPer)
			Size = sizeof(DATE);
		else
			Size = 23 + sizeof(long);
		return memcmp(m_Row.m_pDims + m_JustDimsSize, SourceRow->m_pDims + m_JustDimsSize, Size);
	}

	void SetRowData(CODBCRecordset & Source)
	{
		int i;
		int Offset = 0;
		std::vector<int>::iterator it = m_vecDimsSizes.begin();
		for (i = 0; i < m_nDims ; i++)
		{
			int CurSize;
			if (*it == DS_NUMERIC)
			{
				CurSize = sizeof(double);
				double d = Source[i].AsDouble();
				memcpy(m_Row.m_pDims + Offset, &d, CurSize);
			}
			else if (*it == DS_DATE)
			{
				CurSize = sizeof(DATE);
				COleDateTime odt =Source[i].AsDate();
				memcpy(m_Row.m_pDims + Offset, &odt.m_dt, CurSize);
			}
			else
			{
				CurSize = *it;
				memcpy(m_Row.m_pDims + Offset, (LPCTSTR)Source[i].AsString(), CurSize);
			}
			Offset += CurSize;
			it++;
		};

		if (m_Type == eRests)
			for (i = 0; i < m_nFigs; i++)
				m_Row.m_pFigs[i] = Source[m_nDims + i].AsDouble();
		else {
			int nFieldOfFirstFig = m_nDims + 1;
			if (m_Type == eTurnsPer)
				SetRowPeriod(Source[m_nDims].AsDate().m_dt);
			else
			{
				SetRowPeriod(Source[m_nDims].AsString(), Source[m_nDims + 1].AsLong());
				nFieldOfFirstFig++;
			}
			for (i = 0; i < m_nFigs; i++) {
				int nByOrd = i<<1;
				m_Row.m_pFigs[nByOrd] = Source[nFieldOfFirstFig + nByOrd].AsDouble();
				m_Row.m_pFigs[nByOrd + 1] = Source[nFieldOfFirstFig + nByOrd + 1].AsDouble();
			}
		}
	}

	void SetDimsToRecordset(CODBCRecordset & Dest, const char* pDims) const
	{
		int Offset = 0;
		std::vector<int>::const_iterator it = m_vecDimsSizes.begin();
		for (int i = 0; i < m_nDims; i++)
		{
			int CurSize;
			if (*it == DS_NUMERIC)
			{
				CurSize = sizeof(double);
				static_cast<CDBField&>(Dest.GetParam(i)) =
					*reinterpret_cast<const double*>(pDims + Offset);
			}
			else if (*it == DS_DATE)
			{
				CurSize = sizeof(DATE);
				static_cast<CDBField&>(Dest.GetParam(i)) =
					COleDateTime(*reinterpret_cast<const DATE*>(pDims + Offset));
			}
			else
			{
				CurSize = *it;
				static_cast<CDBField&>(Dest.GetParam(i)) = CString(pDims + Offset, CurSize);
			}
			Offset += CurSize;
			it++;
		};
	}

	void SetPeroidToRecordset(CODBCRecordset & Dest, const char* pDims) const
	{
		if (m_Type == eTurnsPer)
		{
			static_cast<CDBField&>(Dest.GetParam(m_nDims)) =
				COleDateTime(*reinterpret_cast<const DATE*>(pDims + m_JustDimsSize));
		}
		else if (m_Type == eTurnsDoc)
		{
			static_cast<CDBField&>(Dest.GetParam(m_nDims)) =
				CString(pDims + m_JustDimsSize, 23);
			static_cast<CDBField&>(Dest.GetParam(m_nDims + 1)) =
				*reinterpret_cast<const long*>(pDims + m_JustDimsSize + 23);
		}
	}

	double GetFig(const double* pFigs, const int index, const int debkred = 0) const
	{
		if (m_Type == eRests)
			return pFigs[index];
		else
			return pFigs[(index<<1) + debkred];
	}

	inline int GetJustDimsSize() {return m_JustDimsSize;}

/*
// 	//standard interface
// 	typedef stRow* pointer;
// 	typedef const stRow* const_pointer;
// 	typedef stRow& reference;
// 	typedef const stRow& const_reference;
// 	typedef stRow value_type;
// 	typedef size_t size_type;
// 	typedef ptrdiff_t difference_type;
// 
// 	template <class _Tp1> struct rebind
// 	{
// 		typedef CRow_allocator other;
// 	};
// 
// 	pointer address(reference __x) const {return &__x;}
// 	const_pointer address(const_reference __x) const { return &__x; }
// 
// 	// __n is permitted to be 0.  The C++ standard says nothing about what the return value is when __n == 0.
// 	pointer resize(size_type __n, const void* = 0)
// 	{
// 		Msg("allocate %i", __n);
// 		if (__n > max_size())
// 			throw std::bad_alloc();
// 
// 		if (__n != 0)
// 		{
// 			size_type __buf_size = __n * sizeof(value_type);
// 			pointer __ret = reinterpret_cast<pointer>(::operator new(__buf_size));
// 			return __ret;
// 		}
// 		else
// 			return 0;
// 	}
// 
// 	// __p is permitted to be a null pointer, only if n==0.
// 	void deallocate(pointer __p, size_type __n)
// 	{
// 		Msg("deallocate %i", __n);
// 		assert( (__p == 0) == (__n == 0) );
// 		if (__p != 0)
// 			::operator delete(__p);
// 	}
// 
// 	// backwards compatibility
// 	void deallocate(pointer __p)
// 	{
// 		if (__p)
// 			deallocate(__p, sizeof(value_type));
// 	}
// 	size_type max_size() const throw()
// 	{
// 		return size_t(-1) / sizeof(value_type);
// 	}
// 	void construct(pointer __p, const_reference __val)
// 	{
// 		Msg("construct");
// 		new(__p) stRow(__val);
// 		ConstructObj(*__p);
// 		memcpy(__p->m_pDims, __val.m_pDims, m_DimsSize);
// 		memcpy(__p->m_pFigs, __val.m_pFigs, m_FigsSize);
// 	}
// 	void destroy(pointer __p)
// 	{
// 		Msg("destroy");
// 		__p->~stRow();
// 		DestroyObj(*__p);
// 	}
// 	bool operator==(const CRow_allocator&) const throw() { return true; }
// 	bool operator!=(const CRow_allocator&) const throw() { return false; }
*/

private:
	void ConstructObj(stRow& _X)
	{
		_X.resize(m_DimsSize, m_FigsSize);
	}
/*
// 	void DestroyObj(stRow const& _X)
// 	{
// 	}
*/
};

class CMetaNameParser
{
public:
	CMetaNameParser();
	CMetaNameParser(CODBCDatabase* db);
	~CMetaNameParser();

	enum MetaNameTypes
	{
		ntReference,
		ntDocument,
		ntDocumentStrings,
		ntRegistr,
		ntRegistrTotals,
		ntCalcJournal,

		ntDocumentKind,
		ntConstant,
		ntHistory,
		ntCommonProp,
		ntRegisterFlag,
		ntSequence,
		ntColumnCalcJournal,
		ntDocsJournal,
		ntSelectionColumn,
		ntCalendar,
		ntHolidays,
		ntCalcJournalKind,
		ntAlgorithm,
		ntDocumentKind36,
		ntReferenceKind,
		ntReferenceKind36,
		ntEnumKind36,
		ntEnumeration,
		ntEnumeration23,

		ntRegistrRests,
		ntRegistrTurnovers,
		ntRegistrRestsTurnovers,

		ntLastValue,
		ntBegOfPeriod,

		ntEmptyRef,
		ntEmptyRef13,

		ntEntry, // spock: 2006-01-29
		LastType
	};
	inline static bool IsIdChar(const char Ch);
	void SetParameter(const CString & Name, const CValue & Value);
	void SetQueryText(const char* Text);
	const char* GetQueryText() const;
	void Parse();
	int GetSQLParamNumber(const CValue & nstrParam);
	int GetSQLParamModifikator(const int nParam);
	void SetSQLParamsModifikators(QueryParamArray & ParamArray);
	static bool CValueToDBValue(const CValue & Value, const int Modificator, CString & StrValue, bool & IsStrLiteral, CString& SQLType);
	void OnClose();
	static int DefineList(const char * pStr, CStringList & List, int & CharsProcessed);
	void BackFromTAEnabled(bool fEnabled) {m_BackFromTAEnabled = fEnabled;};
	void SetRPCMode(bool fEnabled) {m_RPCMode = fEnabled;};
	bool GetRPCMode() { return m_RPCMode; };
	void SetParseSQLParam(bool bParse) { m_bParseSQLParam = bParse; };
	//trad
	void SetIBProp(CMetaDataCont *pMDC, CIBSet *pIBSet) {m_pMetaDataCont = pMDC; m_pIBSet = pIBSet;};
	void SetDB(CODBCDatabase* pDB){m_db = pDB;};
	static CDate GetPrevPeriodEx(CDate Date, PeriodType Period);
	static CDate GetNextPeriodEx(CDate Date, PeriodType Period);

private:
	//Attributes
	//Attributes for current parsing process
	CString QueryText;
	int Start;
	int CurIndex;
	struct TableAliases
	{
		MetaNameTypes TableType;
		CMetaDataObj * Table;
	};
	//CMap<CString, LPCTSTR, TableAliases , TableAliases&> Aliases;
	CIStringMap<TableAliases , TableAliases&> Aliases;
	CString m_sTempFieldPrefix;
	//

	CMap<CString, LPCTSTR, CValue, CValue&> Params;
	struct SQLParam
	{
		int Number;
		int Modificator;
	};
	CMap<CString, LPCTSTR, SQLParam , SQLParam &> SQLParams;
	CODBCDatabase * m_db;
	CString m_OnCloseStatments;

	bool m_BackFromTAEnabled;
	bool m_RPCMode;
	bool m_bParseSQLParam;

	struct stRPCParam
	{
		CString VarName;
		CString Value;
		CString Type;
		int Count;
	};
	CArray<stRPCParam, stRPCParam&> m_RPCParams;
	CMapStringToPtr m_UseTextParamsInRPC;

	static CIStringMap<int, int> MetaNames;

	//trad
	CMetaDataCont *m_pMetaDataCont;
	CIBSet* m_pIBSet;
	//
	static void InitParser();
	void ParseMetaNames();
	void ClearParams();
	char GetNextSource(char StopChar = 0);
	bool IsNext();
	void PasteName(CString Name);
	CMetaDataObj * GetMetaDataId(const MetaNameTypes NameType, CString & Kind);
	long GetMetaDataFieldId(const MetaNameTypes NameType, const CMetaDataObj* Table, CString & Field);
	void RaiseException(const char * DescTemplate);
	static int GetIdentificatorFromString(const char * pStr, CString & Id);
	void GetIdentificator(CString & Id);
	BOOL FindeAlias(CString & Alias);
	bool ResolveFirstLevelMetaName(const MetaNameTypes MetaNameType, CString & Name, CMetaDataObj * * ppMetaDataObj);
	void ResolveSecondLevelMetaName(const MetaNameTypes NameType, const CMetaDataObj* Table, CString &Name);
	static CString CDateToPeriod(const CDate & Date);
	static void AddMDObjArray(CString & Dest, const PMetaDataObjArray & MDObjArray,
		const CString & StrTemplate, const CString & StrSep, const int fAddToEnd = 0);
	void CStringToMDObjArray(PMetaDataObjArray & DestMDObjArray, const CString & StrDesc, const CMetaDataObjArray* SourceMDObjArray, int NParam);
	static CString DescParameter(const int NParam);
	struct MDArraysItem
	{
		const CMetaDataObjArray* MDObjArray;
		bool IsUsed;
	};
	void ProcessJoinesConditions(CString & Joines, CString & Conditions, MDArraysItem * pSourceMDObjArray, int ArraySize, int NParam);
	enum RestsQueryType {rqForward, rqBack, rqNoTurnovers};
	int GetBorderParameter(CString & Border, CDate & Date, int NParam);
	static CString CDateToCString(const CDate & Date);
	static CString GetStringDateTimeIddoc(const CDate & Date, const long Time, const CObjID & ObjID);
	int FindModificator();
	CString CValueToSQLLiteral(const CValue & Value, const int Modificator, CString& MSSQLType);
	void BuildLastValue(const CMetaDataTypedObj * MDObj, CString & SubQuery, CStringList & VParams);
	CString GetTABorder(CDate & DateTA);
	static CDate AddDays(const CDate & Date, int Days);
	PeriodType GetPeriodicityFromCString(const CString & StrPeriodicity, int NParam);
	static CString GetPeriodField(const PeriodType & Periodcity, const CString & Source, int SourceType, const CDate & BegLim);
	void BuildBegOfPeriod(CString & SubQuery, const CString & StrPeriodType, CStringList & Params);
	POSITION GetVirtParams(const int ParamsCount, CStringList & Params);
	struct stRestsPlan
	{
		enum RestsQueryType QType;
		CDate RestsPeriod;
		CString TurnBegin;
		CString TurnEnd;
		CString EnlargeStr;
		stRestsPlan(): RestsPeriod(0, 0, 0) {};
		int GetIntervalLength()
		{
			return atoi(TurnEnd.Mid(6, 2)) - atoi(TurnBegin.Mid(6, 2));
		};
	};
	void GetRestsQueryPlan(const CString & Border, const CDate & DateRests, const int fEndOfDay, const CString & PosTA, const CDate & DateTA, const PeriodType RestsSnapShotPeriod, stRestsPlan & RestsPlan);
	void BuildRegRestsSubQuery(CRegDef* RegDef, const PMetaDataObjArray & Dims, const PMetaDataObjArray & Figs, const CString & Joines, const CString & Conditions, const stRestsPlan & RestsPlan, CString & SubQuery);
	void BuildRegRests(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias);
	struct stTurnoversPlan{
		CString BeginBorder;
		CString EndBorder;
		enum PeriodType Periodicity;
		CString EnlargeStr;
		CDate BegLim;
		stTurnoversPlan(): BegLim(0, 0, 0) {};
	};
	void BuildRegTurnoversSubQuery(CRegDef* RegDef, const PMetaDataObjArray & Dims, const PMetaDataObjArray & Figs, const CString & Joines, const CString & Conditions, const stTurnoversPlan TurnoversPlan, CString & SubQuery);
	void BuildRegRestsTurnovers(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias);
	enum RestTurnQueryType {rtqtByBegin, rtqtByEnd};
	void BuildRegTurnovers(CRegDef* RegDef, CString & SubQuery, CStringList & Params, CString & TmpAlias);
	void CheckSQLError(const BOOL & Res, CODBCRecordset & rs);
	enum eMethodRT {eActionsBoundaries, eActions};
	int NumCurRow;
	void CheckSending(CODBCRecordset & Recordset);
	struct stInsPlan
	{
		int DimsCount;
		int FigsCount;
		PeriodType Periodicity;
		RestTurnQueryType QueryType;
		int nFirstFigField;
		int FieldsCount;
	};
	struct stCurBalance
	{
		double FactorOfAccuracy;
		double Balance;
	};
	typedef std::vector<stCurBalance> CvecCurBalances;
	void InsertRow(const CRow_allocator::stRow* pRowSource,
		BOOL IsActions,
		const CRow_allocator & Row_allocator,
		const stInsPlan & InsPlan,
		CODBCRecordset & InsRecordset,
		CvecCurBalances & CurBalances);
	BOOL CurBalancesNotEQ(CvecCurBalances & CurBalances);
	inline double round(const double & x, const double & accuracy);
	inline void SetBalancesFromArray(CvecCurBalances & CurBalances, const double * Array);
	void SetTypeID36Literal(CString & Literal, long TypeID);
	void ProcessForFox();
	void AddSubToken(CString &SubQuery, const char *pSubToken, char ch1, const char **pPrefixSuffix);
	CString GetCPName(const CString& ParamValue, const CString& ParamType);
	CString GetVTStringParam(const CString& ParamValue);
};

class CMNPException : public CException
{
public:
	BOOL GetErrorMessage( LPTSTR lpszError, UINT nMaxError, PUINT pnHelpContext = NULL );
	CMNPException(BOOL b_AutoDelete, CString & aErrorDesc, int CurrentIndex);
	virtual CString GetErrorDescr();
	int GetCurrentIndex();
private:
	CString ErrorDesc;
	int CurIndex;
};

class CIBSet
{
public:
	virtual CDate GetDateTimeTA(CEventTime &TimeTA);
	virtual int GetDataSourceType();
	virtual enum PeriodType GetRestsSnapShotPeriod();
};
class CIBSetSQL : public CIBSet
{
public:
	CIBSetSQL(CODBCDatabase *pDb):m_rs(pDb){};

	CDate GetDateTimeTA(CEventTime &TimeTA);
	int GetDataSourceType();
	enum PeriodType GetRestsSnapShotPeriod();
	CODBCRecordset m_rs;
};
class CIBSetDBF : public CIBSet
{
public:
	CIBSetDBF(CODBCDatabase *pDb):m_rs(pDb){};

	CDate GetDateTimeTA(CEventTime &TimeTA);
	int GetDataSourceType();
	enum PeriodType GetRestsSnapShotPeriod();
	CODBCRecordset m_rs;
};

#endif // !defined(AFX_METANAMEPARSER_H__6CE71132_76A6_48DC_BE73_802D87E32A37__INCLUDED_)
