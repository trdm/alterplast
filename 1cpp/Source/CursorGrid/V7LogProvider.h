//////////////////////////////////////////////////////////////////////
// V7LogProvider.h: interface for Log data provider for CursorGrid
// control for V7.
// Written by Vasiliy Trapeznikov aka trav, mailto: trav78@yandex.ru
// Version: 0.1
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_V7LOGPROVIDER_H__0C55D3E5_FC82_459F_B02F_C771793674E7__INCLUDED_)
#define AFX_V7LOGPROVIDER_H__0C55D3E5_FC82_459F_B02F_C771793674E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "DataProvider.h"
#include "V7DataProvider.h"
#include "V7LogFilter.h"
#include "../mycontextbase.h"

#ifdef _DEBUG_PURE
#define LOG_DP_FIELD_COUNT             12
#else
#define LOG_DP_FIELD_COUNT             10
#endif

#define LOG_DP_MIN_READ_BLOCK_SIZE     1024
#define LOG_DP_MAX_READ_BLOCK_SIZE     65536
#define LOG_DP_NEW_ROW_10              _T('\n')
#define LOG_DP_NEW_ROW_13              _T('\r')
#define LOG_DP_DATE_TIME_LENGTH        18
#define LOG_DP_MIN_ROW_LENGTH          27

enum LOG_DP_SearchDirections {Backward, Forward};
enum LOG_DP_LogType {Temp=0, Main=1};
enum LOG_DP_GetRowStatus {grsSuccess, grsBOF, grsEOF, grsFileError};


struct LOG_DP_FileInArchiveInfo
{
	CString ArchiveName;
	CString EntryName;
	bool InArchive;
	ULONG Size;
	ULONGLONG Position;
};


struct LOG_DP_BorderInfo
{
	CString DateTime;
	ULONGLONG Position;
	LOG_DP_LogType LogType;
	LOG_DP_BorderInfo()
	{
		Reset();
	}
	void Reset()
	{
		DateTime.Empty();
		Position=0;
		LogType=Main;
	}
	void GetDate(CValue& val) const
	{
		if (DateTime.IsEmpty())
		{
			val=CDate(0,0,0);
			val.m_Date.m_DateNum=0;
		}
		else
		{
			DWORD dwDate=atoi(DateTime.Left(8));
			int nY=dwDate/10000;
			int nM=(dwDate-nY*10000)/100;
			val=CDate(nY,nM,dwDate%100);
		}
	}
	void GetTime(CValue& val) const
	{
		if (DateTime.IsEmpty())
			val=CValue();
		else
			val=DateTime.Mid(9,8);
	}
};

struct LOG_DP_RowInfo
{
	LOG_DP_LogType LogType;
	ULONGLONG Position;
	DWORD Length;
	LOG_DP_RowInfo()
	{
		Reset();
	}
	LOG_DP_RowInfo(const LOG_DP_BorderInfo& Border)
	{
		LogType=Border.LogType;
		Position=Border.Position;
		Length=0;
	}
	void Reset()
	{
		LogType=Main;
		Position=0;
		Length=0;
	}
	bool operator == (const LOG_DP_RowInfo& RowInfo) const
	{
		return (this->LogType==RowInfo.LogType && this->Position==RowInfo.Position);
	}
	bool operator != (const LOG_DP_RowInfo& RowInfo) const
	{
		return !(*this==RowInfo);
	}
	bool operator > (const LOG_DP_RowInfo& RowInfo) const
	{
		if (this->LogType==RowInfo.LogType)
			return this->Position > RowInfo.Position;
		else
			return this->LogType > RowInfo.LogType;
	}
	bool operator < (const LOG_DP_RowInfo& RowInfo) const
	{
		if (this->LogType==RowInfo.LogType)
			return this->Position < RowInfo.Position;
		else
			return this->LogType < RowInfo.LogType;
	}
	bool operator >= (const LOG_DP_RowInfo& RowInfo) const
	{
		if (*this == RowInfo)
			return true;
		else
			return *this > RowInfo;
	}
	bool operator <= (const LOG_DP_RowInfo& RowInfo) const
	{
		if (*this == RowInfo)
			return true;
		else
			return *this < RowInfo;
	}
};


class CLOG_DP_LogFile
{
public:

    CLOG_DP_LogFile(LOG_DP_LogType LogType);
    ~CLOG_DP_LogFile();

	BOOL Attach(const CString& csFileName);
	void Detach(BOOL bClearArchiveList=TRUE);
	BOOL Read(const DWORD dwBytesToRead, DWORD& dwBytesRead);
	BOOL Seek(const LONGLONG nDistToMove, const DWORD dwMoveMethod);
	ULONGLONG GetFileSize() const;

	LOG_DP_GetRowStatus GetRow(
		const BOOL bRandomSeeking,
		const BOOL bConstBuffer,
		LOG_DP_SearchDirections Dir,
		CString& csRow,
		LOG_DP_RowInfo* pRowInfo=NULL);

	void AddArchiveEntry(
		const CString& csArchiveName,
		const CString& csEntryName,
		const int& nSize);
	BOOL PartiallyFilled() const;
	BOOL GetEntry(const LOG_DP_SearchDirections Dir, POSITION& PositionInList, LOG_DP_FileInArchiveInfo& EntryInfo) const;
	BOOL SetEntryPosition();

	BOOL Attached() const
	{
		return m_hFile ? TRUE : FALSE;
	};
	CString GetFileName() const { return m_csFileName; };
	LOG_DP_LogType GetLogType() const { return m_Type; };
	CString GetStartDate() const { return m_csStartLogDate; };
	void Reset()
	{
		m_csBuffer.Empty();
		m_nBufferGrowCount=0;
	};

private:

	LOG_DP_LogType m_Type;

	CList<LOG_DP_FileInArchiveInfo,LOG_DP_FileInArchiveInfo&> m_FilesList;

	BOOL ReadArchive(const POSITION& PositionInList);

	HANDLE m_hFile;
	CString m_csFileName;

	ULONGLONG m_nCurrPosition;
	CString m_csStartLogDate;

	CString m_csBuffer;
	int m_nBufferGrowCount;
};



class CLogRow : public CDataRow, public CV7DataRow
{
    friend class CV7LogProvider;
public:
	CLogRow(const LOG_DP_RowInfo& RowInfo, CString* pRow);
    virtual ~CLogRow();

    //CDataRow
    virtual int GetFieldCount() { return LOG_DP_FIELD_COUNT; };
    virtual int GetFieldIndexOf(LPCSTR szFieldName);
    virtual LPCSTR GetFieldName(int nIndex);
    virtual DataType GetFieldType(int nIndex) {return dtText;};
    virtual void FormatField(int nIndex, CString& strValue);

    //CV7DataRow
    virtual const CValue& GetValue(int nIndex) const;

private:

	LOG_DP_RowInfo m_RowInfo;

	CValue* m_ppValues[LOG_DP_FIELD_COUNT];

	const static CValue m_vEmpty;

#ifdef _DEBUG_PURE
	CValue m_Position;
#endif
};


class CV7LogProvider : 
	public CMyContextBase,
	public CDataProvider, 
	public CV7DataProvider  
{
    DECLARE_DYNCREATE(CV7LogProvider);
public:
	
	CV7LogProvider();
	virtual ~CV7LogProvider();

	BOOL SetPeriod(CValue& Result, CValue** ppValue);
	BOOL DefaultPeriod(int nParam, CValue* param) const;
	BOOL GetBegOfPeriodDate(CValue& Value) const;
	BOOL GetEndOfPeriodDate(CValue& Value) const;
	BOOL GetBegOfPeriodTime(CValue& Value) const;
	BOOL GetEndOfPeriodTime(CValue& Value) const;

	BOOL ChangeFilter(CValue& Result, CValue** ppValue);
	BOOL SetFilter(CValue& Result, CValue** ppValue);
	BOOL DefaultFilter(int nParam, CValue* param) const;

	BOOL SetScanDeep(CValue& Result, CValue** ppValue);
	BOOL GetScanDeep(CValue& Value) const;
	BOOL GetLastScanDate(CValue& Value) const;
	BOOL DefaultScanDeep(int nParam, CValue* param) const;

	BOOL OpenFile(CValue& Result, CValue** ppValue);
	BOOL CloseFile(CValue** ppValue);

	BOOL SelectLines(CValue& Result, CValue** ppValue);
	BOOL DefaultSelectLines(int nParam, CValue* param) const;
	BOOL GetLine(CValue& Result, CValue** ppValue);

	BOOL GetUsingBuffer(CValue& Value) const;

	BOOL RowGetPosition(CValue& Value) const;
	BOOL RowGetDate(CValue& Value) const;
	BOOL RowGetTime(CValue& Value) const;
	BOOL RowGetUser(CValue& Value) const;
	BOOL RowGetMode(CValue& Value) const;
	BOOL RowGetEventType(CValue& Value) const;
	BOOL RowGetEvent(CValue& Value) const;
	BOOL RowCategory(CValue& Value) const;
	BOOL RowGetComment(CValue& Value) const;
	BOOL RowGetObject(CValue& Value) const;
	BOOL RowGetObjectPresent(CValue& Value) const;

	BOOL SetPeriodEx(CValue& Result, CValue** ppValue);
	BOOL DefaultPeriodEx(int nParam, CValue* param) const;

	BOOL GetLogFileName(CValue& Value) const;
	BOOL SetLogFileName(CValue const& Value);

	BOOL GetArchivesView(CValue& Value) const;
	BOOL SetArchivesView(CValue const& Value);

	BOOL GetTestBuild(CValue& Value) const;

	BOOL GetEventTypes(CValue& Value) const;
	BOOL GetFilter(CValue& Value) const;

#ifdef _DEBUG_PURE
	BOOL GoTest(CValue** ppValue);
#endif


	CV7LogEvent* GetEventByName(LPCSTR szName) const;
	CV7LogEventType* GetEventTypeByName(LPCSTR szName) const;

	void ApplyFilter();

    DECLARE_MY_CONTEXT()

    //CDataProvider
public:
    virtual void QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint);
    virtual CDataRow* Fetch();
    virtual BOOL RefreshRow(CDataRow* pRowFrom);
    virtual BOOL CompareRows(CDataRow* pRow1, CDataRow* pRow2) const;
protected:
    virtual void OnAttach();
    virtual void OnDetach() { CloseLog(); };

    //CV7DataProvider
public:
    virtual CV7DataRow* GetV7DataRow(CDataRow* pDataRow) const;
    virtual void GetRowValue(CDataRow* pDataRow, CValue& value) const;
    virtual CDataRow* BuildRowByValue(const CValue& value);

private:

	static CString GetDefaultLogFileName()
	{
		CString csFileName=CSetOfHierarchy::GetHierarchy()->pathBase1C;
		if (csFileName.Right(1)!=_T('\\'))
			csFileName+=_T('\\');
		csFileName+=_T("Syslog\\1cv7.mlg");
		return csFileName;
	}

	enum ProviderMode
	{
		pmNormalBrowse=0,
		pmBoundsSearch,
		pmPeriodBrowse,
		pmSomeError
	} m_ProviderMode;

	enum GetRowResult
	{
		grrMismatch=0,
		grrMatch,
		grrNoMoreRows,
		grrEscPressed
	};

	enum AddingMode
	{
		amNormal=1,
		amToHead=2,
		amToTail=4
	};

	void Init();

	BOOL OpenLog();
	void CloseLog();

	BOOL GetRowByRowInfo(LOG_DP_RowInfo& RowInfo, const BOOL bCheckEnd);

	BOOL SearchBound(const BOOL bLowerBound, const CString& csSearchDate, const CString& csEndDate, ULONGLONG& nBound);
	BOOL SetPeriodInternal(CString& csBegDate, CString& csEndDate);

	GetRowResult GetFilteredRow(
		const BOOL bSeeESC,
		const BOOL bCheckFilter,
		LOG_DP_SearchDirections Dir,
		LOG_DP_RowInfo& RowInfo,
		CString* pRow);

	inline BOOL BufferMode() const;
	inline ULONGLONG GetFileSize(const LOG_DP_LogType& LogType) const;

	GetRowResult AddRowsToList(const AddingMode HowToAdd, const LOG_DP_RowInfo& FromRow, const ULONGLONG &nBytesToRead, ULONGLONG &nBytesRead);

	GetRowResult FillRowsList();
	void RemoveTempRowsFromList();

	void GetRowFieldValue(const int nFieldIndex, CValue& Value) const;

	BOOL CreateTempLog(const CString& csBegDate, const CString& csEndDate, ULONGLONG& nOffset);

	int m_nQueryRowsCount;

	CLOG_DP_LogFile m_MainLog;
	CLOG_DP_LogFile m_TempLog;

	CLOG_DP_LogFile* m_pCurLog;

	BOOL m_bArchivesView;

	BOOL m_bDefaultLog;
	CString m_csLogFileName;

	// для выборки строк
	BOOL m_bLinesSelectionEnabled;
	struct CurrentRow
	{
		LOG_DP_RowInfo RowInfo;
		CString Fields[LOG_DP_FIELD_COUNT];
	} m_sCurrentRow;

	// для периода
	LOG_DP_BorderInfo m_UpperBorder, m_LowerBorder, m_LastScanRow;

	int m_nScanDeep;

	CList<LOG_DP_RowInfo,LOG_DP_RowInfo&> m_RowsList;
	POSITION m_RowPosition;

	CBLPtr<CV7LogEventTypesCollection> m_pEventTypes;

	CV7LogFilter m_Filter;
};


BOOL IsDateTimeValid(const CString& csDateTime);


#endif // !defined(AFX_V7LOGPROVIDER_H__0C55D3E5_FC82_459F_B02F_C771793674E7__INCLUDED_)
