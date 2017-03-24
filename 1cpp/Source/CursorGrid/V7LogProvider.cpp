// V7LogProvider.cpp: implementation of the CV7LogProvider class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "V7LogProvider.h"
#include "..\\SetOfHierarchy.h"
#include "..\\BinaryData.h"
#include "..\\SQL\\MetaDataWork.h"
#include "..\\zlibeng.tlh"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif


//////////////////////////////////////////////////////////////////////////
// class CLogFile
//////////////////////////////////////////////////////////////////////////
CLOG_DP_LogFile::CLOG_DP_LogFile(LOG_DP_LogType LogType):m_hFile(NULL),m_Type(LogType)
{
	m_csStartLogDate.Empty();
	m_nCurrPosition=0;
	Reset();
}


CLOG_DP_LogFile::~CLOG_DP_LogFile()
{
	Detach();
}


BOOL CLOG_DP_LogFile::Attach(const CString& csFileName)
{
	Detach(FALSE);

	DWORD dwDesiredAccess=GENERIC_READ;
	if (m_Type==Temp)
		dwDesiredAccess=dwDesiredAccess|GENERIC_WRITE;
	
	m_hFile = CreateFile(csFileName, dwDesiredAccess, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
	if (m_hFile == INVALID_HANDLE_VALUE)
	{
		m_hFile = NULL;
		return FALSE;
	}
	else
	{
		m_csFileName=csFileName;

		m_nCurrPosition=0;
		DWORD dwRead;
		Read(LOG_DP_DATE_TIME_LENGTH,dwRead);
		
		BOOL bValid;
		if (bValid=IsDateTimeValid(m_csBuffer))
			bValid=m_csBuffer.Compare(_T("19800101;00:00:00;"))>=0;
		if (bValid)
			m_csStartLogDate=m_csBuffer;
		else
			Detach();
		Reset();
	}
	
	return m_hFile ? TRUE : FALSE;
}


void CLOG_DP_LogFile::Detach(BOOL bClearArchiveList)
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile=NULL;
	}

	if (m_Type==Temp)
	{
		if (!m_csFileName.IsEmpty())
			DeleteFile(m_csFileName);
	}
	m_csFileName.Empty();
	if (bClearArchiveList)
		m_FilesList.RemoveAll();
}


BOOL CLOG_DP_LogFile::Read(const DWORD dwBytesToRead, DWORD& dwBytesRead)
{
	BOOL bResult=FALSE;
	dwBytesRead = 0;
	if (m_hFile)
	{
		if (m_Type==Temp)
		{
			POSITION CurrPos=m_FilesList.GetTailPosition();
			while (CurrPos != NULL)
			{
				if (m_nCurrPosition < m_FilesList.GetAt(CurrPos).Position)
					m_FilesList.GetPrev(CurrPos);
				else
					break;
			}
			if (CurrPos == NULL)
				return FALSE; // как такое может быть - хз :)
			else
			{
				DWORD dwBytesToReadFromArchive=dwBytesToRead;
				LONGLONG nDiff=m_FilesList.GetAt(CurrPos).Position+m_FilesList.GetAt(CurrPos).Size-m_nCurrPosition;
				while (dwBytesToReadFromArchive)
				{
					if (ReadArchive(CurrPos))
					{
						if (dwBytesToReadFromArchive<nDiff)
							break;
						else
						{
							dwBytesToReadFromArchive-=nDiff;
							m_FilesList.GetNext(CurrPos);
							if (CurrPos)
								nDiff=m_FilesList.GetAt(CurrPos).Size;
							else
								break;
						}
					}
					else
						return FALSE;
				}
			}
		}
		LPVOID p = m_csBuffer.GetBuffer(dwBytesToRead);
		bResult=ReadFile(m_hFile, p, dwBytesToRead, &dwBytesRead, NULL);
		m_csBuffer.ReleaseBuffer(dwBytesRead);
	}
	return bResult;
}


BOOL CLOG_DP_LogFile::Seek(const LONGLONG nDistToMove, const DWORD dwMoveMethod)
{
	if (m_hFile)
	{
		LARGE_INTEGER liPosition;
		liPosition.QuadPart=nDistToMove;
		liPosition.LowPart=SetFilePointer(m_hFile, liPosition.LowPart, &liPosition.HighPart, dwMoveMethod);
		if (liPosition.LowPart!=0xFFFFFFFF || GetLastError()==NO_ERROR)
		{
			m_nCurrPosition=liPosition.QuadPart;
			return TRUE;
		}
	}
	return FALSE;
}


ULONGLONG CLOG_DP_LogFile::GetFileSize() const
{
	if (m_hFile)
	{
		ULARGE_INTEGER uiFileSize;
		uiFileSize.LowPart=::GetFileSize(m_hFile,&uiFileSize.HighPart);
		return uiFileSize.QuadPart;
	}
	else
		return 0;
}


BOOL CLOG_DP_LogFile::ReadArchive(const POSITION& PositionInList)
{
	if (m_Type==Main)
		return FALSE;
	if (!m_FilesList.GetAt(PositionInList).InArchive)
		return TRUE;
	m_FilesList.GetAt(PositionInList).InArchive=false;

	LOG_DP_FileInArchiveInfo info=m_FilesList.GetAt(PositionInList);

	CComPtr<IStream> FileStream=NULL;
	if (!CFileStream::CreateFileStream(this->GetFileName(), FALSE, CFileStream::Share, GENERIC_WRITE, FileStream))
		return FALSE;

	LARGE_INTEGER Pos;
	Pos.QuadPart=info.Position;
	if (FileStream->Seek(Pos,STREAM_SEEK_SET,NULL) != S_OK)
		return FALSE;

	bool bOK;

	CComPtr<IzlibEngine> Zlib=NULL;
	if (bOK = Zlib.CoCreateInstance(__uuidof(zlibEngine))==S_OK)
	{
		bOK=Zlib->pkReadFile(
			(LPWSTR)info.ArchiveName.operator LPCSTR(),
			(LPWSTR)info.EntryName.operator LPCSTR(),
			FileStream,
			NULL)==S_OK;
	}

	if (bOK)
		return TRUE;
	else
	{
		ULONG nBufferSize=__min(info.Size,1<<18);
		vector<char> pBufVector(nBufferSize); //void* pBuf=malloc(nBufferSize);
		void* pBuf= &pBufVector[0];
// 		if (pBuf==NULL)
// 			return FALSE;
		memset(pBuf,_T(' '),nBufferSize);
		int nSize=info.Size;
		while (nSize)
		{
			ULONG nWritten=__min(nSize,nBufferSize);
			if (FileStream->Write(pBuf,nWritten,&nWritten) != S_OK)
				return FALSE;
			nSize-=nWritten;
		}
		//free(pBuf);
		return TRUE;
	}
}


void CLOG_DP_LogFile::AddArchiveEntry(
									  const CString& csArchiveName,
									  const CString& csEntryName,
									  const int& nSize)
{
	if (m_Type==Main)
		return;

	LOG_DP_FileInArchiveInfo info;
	int nAdd=3;
	POSITION Pos=m_FilesList.GetHeadPosition();
	while (Pos!=NULL)
	{
		info=m_FilesList.GetNext(Pos);
		if (int nCmp=csEntryName.Compare(info.EntryName))
		{
			if (nCmp<0)
			{
				nAdd=1;
				break;
			}
		}
		else
		{
			nAdd=info.InArchive ? 2 : 0;
			break;
		}
	}
	if (nAdd<3)
	{
		if (Pos)
			m_FilesList.GetPrev(Pos);
		else
			Pos=m_FilesList.GetTailPosition();
	}

	if (nAdd)
	{
		info.ArchiveName=csArchiveName;
		info.EntryName=csEntryName;
		info.Size=nSize;
		info.Position=0;
		info.InArchive=true;
		switch (nAdd)
		{
		case 1:
			m_FilesList.InsertBefore(Pos,info);
			break;
		case 2:
			m_FilesList.SetAt(Pos,info);
			break;
		case 3:
			m_FilesList.AddTail(info);
			break;
		}
	}
}


BOOL CLOG_DP_LogFile::PartiallyFilled() const
{
	if (m_Type==Main)
		return TRUE;
	{
		for (POSITION Pos=m_FilesList.GetHeadPosition(); Pos!=NULL; )
		{
			if (!m_FilesList.GetNext(Pos).InArchive)
				return TRUE;
		}
		return FALSE;
	}
}


BOOL CLOG_DP_LogFile::GetEntry(const LOG_DP_SearchDirections Dir, POSITION& PositionInList, LOG_DP_FileInArchiveInfo& EntryInfo) const
{
	if (m_Type==Temp)
	{
		if (m_FilesList.GetCount())
		{
			switch (Dir)
			{
			case Backward:
				if (PositionInList==NULL)
					PositionInList=m_FilesList.GetTailPosition();
				EntryInfo=m_FilesList.GetPrev(PositionInList);
				break;
			case Forward:
				if (PositionInList==NULL)
					PositionInList=m_FilesList.GetHeadPosition();
				EntryInfo=m_FilesList.GetNext(PositionInList);
				break;
			}
			return TRUE;
		}
	}

	return FALSE;
}


BOOL CLOG_DP_LogFile::SetEntryPosition()
{
	if (m_Type==Temp)
	{
		ULONGLONG nPosition=0;
		for (POSITION Pos=m_FilesList.GetHeadPosition(); Pos!=NULL; )
		{
			m_FilesList.GetAt(Pos).Position=nPosition;
			nPosition+=m_FilesList.GetNext(Pos).Size;
		}
	}
	return TRUE;
}


LOG_DP_GetRowStatus CLOG_DP_LogFile::GetRow(
											const BOOL bRandomSeeking,
											const BOOL bConstBuffer,
											LOG_DP_SearchDirections Dir,
											CString& csRow,
											LOG_DP_RowInfo* pRowInfo)
{

	if (pRowInfo)
	{
		if (pRowInfo->Length)
		{
			if (Seek(pRowInfo->Position,FILE_BEGIN))
			{
				DWORD dwRead;
				if (Read(pRowInfo->Length,dwRead))
				{
					csRow=m_csBuffer;
					return grsSuccess;
				}
			}
			return grsFileError;
		}
	}

	csRow.Empty();
	
	if (!bRandomSeeking)
	{
		if ((Dir==Backward)&&(m_nCurrPosition==0))
		{
			if (pRowInfo)
				pRowInfo->Reset();
			return grsBOF;
		}
	}
	
	// если случайный поиск, то сначала всегда ищем с конца буфера
	if (bRandomSeeking)
		Dir=Backward;
	
	BOOL bRememberPosition=TRUE;
	
	ULONGLONG nOldPosition=m_nCurrPosition;
	
	DWORD dwBytesToRead;
	DWORD dwJumpSize;
	
	if (bConstBuffer)
	{
		dwBytesToRead=LOG_DP_MAX_READ_BLOCK_SIZE;
		dwJumpSize=m_nBufferGrowCount?LOG_DP_MAX_READ_BLOCK_SIZE:0;
	}
	else
	{
		dwBytesToRead=LOG_DP_MIN_READ_BLOCK_SIZE;
		switch (m_nBufferGrowCount)
		{
		case 0:
			dwJumpSize=0;
			break;
		case 1:
			dwJumpSize=LOG_DP_MIN_READ_BLOCK_SIZE;
			break;
		default:
			dwJumpSize=LOG_DP_MIN_READ_BLOCK_SIZE<<(m_nBufferGrowCount-1);
		}
	}
	
	int nBufferGrowCount=0;
	
	bool bNearLogBeginning=false;
	BOOL bFullCheck=bRandomSeeking;
	bool bDirChanged=false;
	
	while (true)
	{
		int nPos=Dir==Forward?m_csBuffer.Find(LOG_DP_NEW_ROW_13,0):m_csBuffer.ReverseFind(LOG_DP_NEW_ROW_13);
		
		if (bNearLogBeginning)
		{
			if (nPos == -1)
			{
				if (Dir==Forward)
				{
					if (Seek(0,FILE_BEGIN))
					{
						bNearLogBeginning=false;
						m_csBuffer.Empty();
					}
					else
					{
						if (pRowInfo)
							pRowInfo->Reset();
						csRow.Empty();
						return grsFileError;
					}
				}
				else
				{
					Dir=Forward;
					bFullCheck=FALSE;
					bDirChanged=true;
					continue;
				}
			}
			else
				bNearLogBeginning=false;
		}
		
		if (nPos == -1)
		{
			ULONGLONG nSavedCurrPosition=m_nCurrPosition;

			const CString csBufferCopy=m_csBuffer;

			LONGLONG nDistToMove=0;
			DWORD dwMoveMethod=FILE_CURRENT;
			
			if (Dir==Backward)
			{
				dwJumpSize+=dwBytesToRead;
				
				if (m_nCurrPosition < dwBytesToRead)
					dwMoveMethod=FILE_BEGIN;
				else
				{
					nDistToMove=-static_cast<LONGLONG>(dwJumpSize);
					nBufferGrowCount++;
					dwJumpSize=dwBytesToRead;
				}
			}
			
			if (Seek(nDistToMove,dwMoveMethod))
			{
				if (m_nCurrPosition >= this->GetFileSize())
				{
					if (pRowInfo)
						pRowInfo->Reset();
					csRow.Empty();
					return grsEOF;
				}
			}
			else
			{
				if (pRowInfo)
					pRowInfo->Reset();
				csRow.Empty();
				return grsFileError;
			}
			
			DWORD dwRead;
			if (!Read(dwBytesToRead,dwRead))
			{
				if (pRowInfo)
					pRowInfo->Reset();
				csRow.Empty();
				return grsFileError;
			}

			
			if (Dir==Forward)
			{
				if (int nLen=csBufferCopy.GetLength())
				{
					m_nCurrPosition-=nLen;
					m_csBuffer=csBufferCopy+m_csBuffer;
				}
			}
			else
			{
				if (nSavedCurrPosition < dwBytesToRead)
				{
					bNearLogBeginning=true;
					m_csBuffer=m_csBuffer.Left(nSavedCurrPosition);
				}
				else
					m_csBuffer+=csBufferCopy;
				m_nCurrPosition+=m_csBuffer.GetLength();
			}
			
			if (dwBytesToRead<LOG_DP_MAX_READ_BLOCK_SIZE)
				dwBytesToRead=dwBytesToRead<<1;
		}
		else
		{
			if (Dir==Backward)
			{
				CString strTmp = m_csBuffer.Mid(nPos);
				csRow = strTmp + csRow;
				
				m_csBuffer = m_csBuffer.Left(nPos);
				m_nCurrPosition-=strTmp.GetLength();
				
				bool bCont=true;
				while (bCont)
				{
					TCHAR chSymb=csRow[0];
					switch (chSymb)
					{
					case LOG_DP_NEW_ROW_10:
					case LOG_DP_NEW_ROW_13:
						csRow.TrimLeft(chSymb);
						break;
					default:
						bCont=false;
						break;
					}
				}
				
				if (bRememberPosition)
				{
					if (pRowInfo)
						pRowInfo->Position = m_nCurrPosition+CString(_T("\r\n")).GetLength();
					bRememberPosition=FALSE;
				}
				
			}
			else
			{
				if (bRememberPosition)
				{
					if (pRowInfo)
						pRowInfo->Position=m_nCurrPosition;
					bRememberPosition=FALSE;
				}
				csRow+=m_csBuffer.Left(nPos);
				
				bool bCont=true;
				while (bCont)
				{
					TCHAR chSymb=m_csBuffer[nPos];
					switch (chSymb)
					{
					case LOG_DP_NEW_ROW_10:
					case LOG_DP_NEW_ROW_13:
						csRow+=chSymb;
						nPos++;
						break;
					default:
						bCont=false;
						break;
					}
				}
				
				m_csBuffer=m_csBuffer.Mid(nPos);
				m_nCurrPosition+=nPos;
			}
			
			int nLen=csRow.GetLength();
			if (nLen<LOG_DP_DATE_TIME_LENGTH)
			{
				bRememberPosition=TRUE;
				if (bRandomSeeking)
				{
					csRow.Empty();
					nOldPosition-=nLen;
				}
			}
			else
			{
				BOOL bIsValidRow=FALSE;
				
				int nCount=2;
				
				while (nCount)
				{
					CString strDateTime;
					LOG_DP_SearchDirections WhatCheck;

					if (nCount==2)
						WhatCheck=Dir;
					else
						WhatCheck=Dir==Forward?Backward:Forward;
					
					if (WhatCheck==Forward)
					{
						nLen=m_csBuffer.GetLength();
						if (nLen<LOG_DP_DATE_TIME_LENGTH)
						{
							ULONGLONG nSavedPosition=m_nCurrPosition;
							if (Seek(0,FILE_CURRENT))
							{
								const CString csBufferCopy=m_csBuffer;
								DWORD dwRead;
								Read(LOG_DP_MIN_READ_BLOCK_SIZE,dwRead);
								if (nLen)
									m_csBuffer=csBufferCopy+m_csBuffer;
								m_nCurrPosition=nSavedPosition;
							}
							else
							{
								if (pRowInfo)
									pRowInfo->Reset();
								csRow.Empty();
								return grsFileError;
							}
						}
						
						bool bCont=true;
						while (bCont)
						{
							TCHAR chSymb=m_csBuffer[0];
							switch (chSymb)
							{
							case LOG_DP_NEW_ROW_10:
							case LOG_DP_NEW_ROW_13:
								csRow+=chSymb;
								m_csBuffer.TrimLeft(chSymb);
								m_nCurrPosition+=CString(chSymb).GetLength();
								break;
							default:
								bCont=false;
								break;
							}
						}
						strDateTime=m_csBuffer.Left(LOG_DP_DATE_TIME_LENGTH);
					}
					else
						strDateTime=csRow.Left(LOG_DP_DATE_TIME_LENGTH);
					
					if (strDateTime.GetLength()==LOG_DP_DATE_TIME_LENGTH)
						bIsValidRow=IsDateTimeValid(strDateTime);
					else
						bIsValidRow=TRUE;
					
					if (bIsValidRow)
					{
						if (bRandomSeeking)
						{
							if (bFullCheck)
							{
								bIsValidRow=FALSE;
								bFullCheck=FALSE;
								nCount--;
							}
						}
						break;
					}
					else
						break;
				}
				
				if (bIsValidRow)
					break;
				else
				{
					if (bRandomSeeking)
					{
						if (nCount==1)
						{
							if (Dir==Backward)
							{
								if (Seek(nOldPosition,FILE_BEGIN))
									m_csBuffer.Empty();
								else
								{
									if (pRowInfo)
										pRowInfo->Reset();
									csRow.Empty();
									return grsFileError;
								}
								Dir=Forward;
							}
						}
					}
					else
					{
						if (Dir==Backward)
							bRememberPosition=TRUE;
					}
				}
			}
		}
	}
	
	if (nBufferGrowCount)
		m_nBufferGrowCount=__min(nBufferGrowCount,7);
	
	if (!csRow.IsEmpty())
	{
		bool bCont=true;
		while (bCont)
		{
			TCHAR chSymb=csRow[csRow.GetLength()-1];
			switch (chSymb)
			{
			case LOG_DP_NEW_ROW_10:
			case LOG_DP_NEW_ROW_13:
				csRow.TrimRight(chSymb);
				break;
			default:
				bCont=false;
				break;
			}
		}
		csRow+=LOG_DP_NEW_ROW_13;
		csRow+=LOG_DP_NEW_ROW_10;
	}
	
	if (bDirChanged)
		m_nCurrPosition=0;

	if (pRowInfo)
	{
		pRowInfo->LogType=m_Type;
		pRowInfo->Length=csRow.GetLength();
	}
	
	return grsSuccess;
}



//////////////////////////////////////////////////////////////////////////
// class CLogRow
//////////////////////////////////////////////////////////////////////////
const CValue CLogRow::m_vEmpty=CValue();


CLogRow::CLogRow(const LOG_DP_RowInfo& RowInfo, CString* pRow)
{
	if (pRow)
	{
		for(int nIndex=0; nIndex<LOG_DP_FIELD_COUNT; nIndex++)
		{
			if (pRow[nIndex].IsEmpty())
				m_ppValues[nIndex]=NULL;
			else
			{
				switch (nIndex)
				{
				case 0:
					m_ppValues[0]=new CValue(CDate(atoi(pRow[0].Left(4)),atoi(pRow[0].Mid(4,2)),atoi(pRow[0].Mid(6,2))));
					break;
				case 6:
					m_ppValues[6]=new CValue(CNumeric(atoi(pRow[6])));
					break;
				case 8:
					{
						CValue val;
						if (LogValueFromString(val,pRow[8],1))
							m_ppValues[8]=new CValue(val);
						else
							m_ppValues[8]=NULL;
					}
					break;
				default:
					m_ppValues[nIndex]=new CValue(pRow[nIndex]);
				}
			}
		}
	}
	else
	{
		for(int nIndex=0; nIndex<LOG_DP_FIELD_COUNT; nIndex++)
			m_ppValues[nIndex]=NULL;
	}
	m_RowInfo=RowInfo;

#ifdef _DEBUG_PURE
	m_Position=static_cast<long double>(static_cast<LONGLONG>(RowInfo.Position));
#endif
}


CLogRow::~CLogRow()
{
	for(int nIndex=0; nIndex<LOG_DP_FIELD_COUNT; nIndex++)
	{
		if (m_ppValues[nIndex])
			delete m_ppValues[nIndex];
	}
}


int CLogRow::GetFieldIndexOf(LPCSTR szFieldName)
{
	for (int nIndex=0; nIndex<LOG_DP_FIELD_COUNT; nIndex++)
	{
		if (stricmp(szFieldName, LOG_DP_1C_LogFields[nIndex][0]) == 0)
			return nIndex;
		else
		{
			if (stricmp(szFieldName, LOG_DP_1C_LogFields[nIndex][1]) == 0)
				return nIndex;
		}
	}
	return -1;
}


LPCSTR CLogRow::GetFieldName(int nIndex)
{
	if ((nIndex >= 0) && (nIndex < LOG_DP_FIELD_COUNT))
		return LOG_DP_1C_LogFields[nIndex][GetMetaData()->GetTaskDef()->GetDefaultLanguage()];
	else
		return "";
}


void CLogRow::FormatField(int nIndex, CString& strValue)
{
	if ((nIndex >= 0) && (nIndex < LOG_DP_FIELD_COUNT))
	{
#ifdef _DEBUG_PURE
		if (nIndex==LOG_DP_FIELD_COUNT-1)
		{
			FormatValue(static_cast<long double>(static_cast<LONGLONG>(m_RowInfo.Position)), strValue);
			return;
		}
#endif
		if (m_ppValues[nIndex])
			FormatValue(*m_ppValues[nIndex], strValue);
		else
			FormatValue(m_vEmpty, strValue);
	}
	else
		FormatValue(m_vEmpty, strValue);
}


const CValue& CLogRow::GetValue(int nIndex) const
{
	if ((nIndex >= 0) && (nIndex < LOG_DP_FIELD_COUNT))
	{
#ifdef _DEBUG_PURE
		if (nIndex==LOG_DP_FIELD_COUNT-1)
			return m_Position;
#endif
		if (m_ppValues[nIndex])
			return *m_ppValues[nIndex];
		else
			return m_vEmpty;
	}
    else
		return m_vEmpty;
}


//////////////////////////////////////////////////////////////////////////
// class CV7LogProvider
//////////////////////////////////////////////////////////////////////////
BEGIN_BL_METH_MAP(CV7LogProvider)
	BL_METH("SetPeriod",    "УстановитьПериод",      2, NULL,        SetPeriod,     DefaultPeriod)
	BL_METH("ChangeFilter", "ИзменитьФильтр",        2, NULL,        ChangeFilter,  DefaultFilter)
	BL_METH("SetFilter",    "УстановитьФильтр",      1, NULL,        SetFilter,     DefaultFilter)
	BL_METH("SetScanDeep",  "УстановитьГлубину",     2, NULL,        SetScanDeep,   DefaultScanDeep)
	BL_METH("OpenFile",     "ОткрытьФайл",           0, NULL,        OpenFile,      NULL)
	BL_METH("CloseFile",    "ЗакрытьФайл",           0, CloseFile,   NULL,          NULL)
	BL_METH("SelectLines",  "ВыбратьСтроки",         1, NULL,        SelectLines,   DefaultSelectLines)
	BL_METH("GetLine",      "ПолучитьСтроку",        0, NULL,        GetLine,       NULL)
	BL_METH("SetPeriodEx",  "УстановитьПериодРасш",  4, NULL,        SetPeriodEx,   DefaultPeriodEx)
#ifdef _DEBUG_PURE
	BL_METH("Test",         "Тест",                  1, GoTest,      NULL,          NULL)
#endif
END_BL_METH_MAP()

BEGIN_BL_PROP_MAP(CV7LogProvider)
	BL_PROP("BeginOfPeriod",      "НачалоПериода",              GetBegOfPeriodDate,    NULL)
	BL_PROP("EndOfPeriod",        "КонецПериода",               GetEndOfPeriodDate,    NULL)
	BL_PROP("PeriodBeginingTime", "ВремяНачалаПериода",         GetBegOfPeriodTime,    NULL)
	BL_PROP("PeriodEndingTime",   "ВремяОкончанияПериода",      GetEndOfPeriodTime,    NULL)
	BL_PROP("ScanDeep",           "ГлубинаПросмотра",           GetScanDeep,           NULL)
	BL_PROP("LastScanDate",       "ПоследняяПросмотреннаяДата", GetLastScanDate,       NULL)
	BL_PROP("UsingBuffer",        "ИспользованиеБуфера",        GetUsingBuffer,        NULL)
	BL_PROP("ArchivesView",       "ПросмотрАрхивов",            GetArchivesView,       SetArchivesView)
	BL_PROP("TestBuild",          "ТестоваяСборка",             GetTestBuild,          NULL)
	BL_PROP("FileName",           "ИмяФайла",                   GetLogFileName,        SetLogFileName)
	BL_PROP("EventTypes",         "ТипыСобытий",                GetEventTypes,         NULL)
	BL_PROP("Filter",             "Отбор",                      GetFilter,             NULL)
// свойства записи
	BL_PROP("Position",      "Позиция",                    RowGetPosition,        NULL)
	BL_PROP("Date",          "Дата",                       RowGetDate,            NULL)
	BL_PROP("Time",          "Время",                      RowGetTime,            NULL)
	BL_PROP("User",          "Пользователь",               RowGetUser,            NULL)
	BL_PROP("Mode",          "Режим",                      RowGetMode,            NULL)
	BL_PROP("EventType",     "ТипСобытия",                 RowGetEventType,       NULL)
	BL_PROP("Event",         "Событие",                    RowGetEvent,           NULL)
	BL_PROP("Category",      "Категория",                  RowCategory,           NULL)
	BL_PROP("Comment",       "Комментарий",                RowGetComment,         NULL)
	BL_PROP("Object",        "Объект",                     RowGetObject,          NULL)
	BL_PROP("ObjectPresent", "ПредставлениеОбъекта",       RowGetObjectPresent,   NULL)
END_BL_PROP_MAP()

IMPLEMENT_MY_CONTEXT(CV7LogProvider, "LogProvider", "ПоставщикДанныхЖурналРегистрации", 1, NULL, NULL, -1);


// конструктор / деструктор
CV7LogProvider::CV7LogProvider():m_MainLog(Main),m_TempLog(Temp),m_pEventTypes(NULL)
{
	m_Filter.SetProvider(this);
	m_pEventTypes.Create();
	Init();
	m_csLogFileName.Empty();
}


void CV7LogProvider::Init()
{

	m_TempLog.Detach();

	m_ProviderMode=pmNormalBrowse;

	m_Filter.Reset(true);

	m_nScanDeep=0;

	m_RowPosition=NULL;

	m_bLinesSelectionEnabled=FALSE;

	m_bArchivesView = FALSE;

	m_UpperBorder.Reset();
	m_LowerBorder.Reset();
	m_LastScanRow.Reset();
}


CV7LogProvider::~CV7LogProvider()
{
	CloseLog();
	m_Filter.SetProvider(NULL);
}


// работа с файлом
BOOL CV7LogProvider::OpenLog()
{
	CString csLogFileName=m_csLogFileName;
	if (m_bDefaultLog=csLogFileName.IsEmpty())
		csLogFileName=GetDefaultLogFileName();
	if (m_MainLog.Attach(csLogFileName))
	{
		m_pEventTypes->AddUserEvents(csLogFileName);
		return TRUE;
	}
	else
		return FALSE;
	
}


void CV7LogProvider::CloseLog()
{
	m_MainLog.Detach();
	Init();
}


// свойства и методы
BOOL CV7LogProvider::GetEventTypes(CValue& Value) const
{
	if (m_pEventTypes)
		Value.AssignContext(static_cast<CV7LogEventTypesCollection*>(m_pEventTypes));
	else
		Value.Reset();
	return TRUE;
}


CV7LogEvent* CV7LogProvider::GetEventByName(LPCSTR szName) const
{
	for (int nIndex=0; nIndex<m_pEventTypes->GetSize(); nIndex++)
	{
		if (CV7LogEvent* pEvent=m_pEventTypes->GetAt(nIndex)->FindEvent(szName))
			return pEvent;
	}
	return NULL;
}


CV7LogEventType* CV7LogProvider::GetEventTypeByName(LPCSTR szName) const
{
	return m_pEventTypes->GetAt(szName);
}


BOOL CV7LogProvider::GetFilter(CValue& Value) const
{
	Value.AssignContext(const_cast<CV7LogFilter*>(&m_Filter));
	return TRUE;
}


BOOL CV7LogProvider::OpenFile(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(OpenLog()?1:0);
	return TRUE;
}


BOOL CV7LogProvider::CloseFile(CValue** ppValue)
{
	CloseLog();
	return TRUE;
}


BOOL CV7LogProvider::GetBegOfPeriodDate(CValue& Value) const
{
	m_UpperBorder.GetDate(Value);
	return TRUE;
}


BOOL CV7LogProvider::GetEndOfPeriodDate(CValue& Value) const
{
	m_LowerBorder.GetDate(Value);
	return TRUE;
}


BOOL CV7LogProvider::GetBegOfPeriodTime(CValue& Value) const
{
	m_UpperBorder.GetTime(Value);
	return TRUE;
}


BOOL CV7LogProvider::GetEndOfPeriodTime(CValue& Value) const
{
	m_LowerBorder.GetTime(Value);
	return TRUE;
}


#define LOG_DP_CMP_BEGDATE                  1
#define LOG_DP_CMP_ENDDATE                  2
#define LOG_DP_TEMPLOG_ALREADY_CREATED      (LOG_DP_CMP_BEGDATE|LOG_DP_CMP_ENDDATE)
#define LOG_DP_TEMPLOG_MUST_BE_CREATED      4


void CV7LogProvider::RemoveTempRowsFromList()
{
	m_TempLog.Detach();
	if (BufferMode())
	{
		while (!m_RowsList.IsEmpty())
		{
			if (m_RowsList.GetHead().LogType == Temp)
				m_RowsList.RemoveHead();
			else
				break;
		}
	}
}


BOOL CV7LogProvider::CreateTempLog(const CString& csBegDate, const CString& csEndDate, ULONGLONG& nOffset)
{
	if (GetControl())
		pBkEndUI->DoStatusLine(_T("Поиск и обработка архивов..."));

	nOffset=0;
	
	BOOL bFind=TRUE;
	int nCreate=0;
	CString csOldFirstEntry;
	csOldFirstEntry.Empty();
	
	if (m_TempLog.PartiallyFilled())
	{
		CString csCmpDate=m_UpperBorder.DateTime.Left(8);

		if (csBegDate.Compare(csCmpDate)>=0)
			nCreate=nCreate|LOG_DP_CMP_BEGDATE;
		else
		{
			LOG_DP_FileInArchiveInfo info;
			POSITION Pos=NULL;
			if (m_TempLog.GetEntry(Forward,Pos,info))
			{
				if (csBegDate.Compare(info.EntryName)>=0)
					nCreate=nCreate|LOG_DP_CMP_BEGDATE;
			}
		}

		if ((csBegDate.Compare(csCmpDate)<0)&&(csEndDate.Compare(csCmpDate)<0))
		{
			nCreate=0;
			RemoveTempRowsFromList();
		}
		else
		{
			csCmpDate=m_LowerBorder.DateTime.Left(8);

			if (csEndDate.Compare(csCmpDate)<=0)
				nCreate=nCreate|LOG_DP_CMP_ENDDATE;
			else
			{
				LOG_DP_FileInArchiveInfo info;
				POSITION Pos=NULL;
				if (m_TempLog.GetEntry(Backward,Pos,info))
				{
					if (csEndDate.Compare(info.EntryName)<=0)
						nCreate=nCreate|LOG_DP_CMP_ENDDATE;
				}
			}

			if ((csBegDate.Compare(csCmpDate)>0)&&(csEndDate.Compare(csCmpDate)>0))
			{
				nCreate=0;
				RemoveTempRowsFromList();
			}
			else
			{
				if (bFind=(nCreate&LOG_DP_TEMPLOG_ALREADY_CREATED)!=LOG_DP_TEMPLOG_ALREADY_CREATED)
				{
					LOG_DP_FileInArchiveInfo info;
					POSITION Pos=NULL;
					if (m_TempLog.GetEntry(Forward,Pos,info))
						csOldFirstEntry=info.EntryName;
				}
			}
		}
	}
	else
		RemoveTempRowsFromList();
	
	if (!bFind)
		return TRUE;
	
	CString csPath;
	
	DWORD dwLen=GetFullPathName(m_MainLog.GetFileName(),0,NULL,NULL);
	
	LPTSTR p=NULL;
	
	GetFullPathName(m_MainLog.GetFileName(),dwLen,csPath.GetBuffer(dwLen),&p);
	csPath.ReleaseBuffer();
	csPath.TrimRight(p);
	p=NULL;
	
	WIN32_FIND_DATA fd;
	HANDLE hFind=FindFirstFile(CString(csPath+CString(_T("*.*"))),&fd);
	if (hFind!=INVALID_HANDLE_VALUE)
	{
		CComPtr<IzlibEngine> Zlib;
		if (Zlib.CoCreateInstance(__uuidof(zlibEngine)) != S_OK)
		{
			RuntimeError(_T("Ошибка создания объекта архиватора."));
			return FALSE;
		}
		
		const CString csTime=_T(";00:00:00;");
		
		ULONGLONG qwSize=0;
		while (bFind)
		{
			CString csName=fd.cFileName;
			int nPos=csName.ReverseFind(_T('.'));
			if (nPos!=-1)
				csName=csName.Left(nPos);
			csName+=csTime;
			
			if (IsDateTimeValid(csName))
			{
				csName=csPath+fd.cFileName;
				
				CComPtr<IEnum_zipEntry> zipEntry=NULL;
				
				if (Zlib->pkReadEntry((LPWSTR)csName.operator LPCSTR(), &zipEntry)==S_OK)
				{
					if (GetControl())
					{
						CString csStatus=_T("Обработка архива ");
						csStatus+=fd.cFileName;
						pBkEndUI->DoStatusLine(csStatus);
					}
					
					CComPtr<IzipEntry> zipElem=NULL;
					unsigned long nFetched;
					
					while (zipEntry->Next(1,&zipElem,&nFetched)==S_OK)
					{
						BSTR p;
						zipElem->get_Name(&p);
						CString csDate=p;
						zipElem->get_Size(&nFetched);
						zipElem.Release();
						zipElem=NULL;
						
						if (IsDateTimeValid(CString(csDate+csTime)))
						{
							if ((csDate.Compare(csBegDate)>=0)&&(csDate.Compare(csEndDate)<=0))
							{
								m_TempLog.AddArchiveEntry(csName,csDate,nFetched);
								qwSize+=nFetched;
							}
						}
					}
				}
				if (zipEntry)
				{
					zipEntry.Release();
					zipEntry=NULL;
				}
			}
			bFind=FindNextFile(hFind,&fd);
		}
		FindClose(hFind);
		
		if (!qwSize)
		{
			RemoveTempRowsFromList();
			return TRUE;
		}
		
		csPath.Empty();
		ULARGE_INTEGER qwFree;
		qwFree.QuadPart=GetTempPath(0,NULL);
		GetTempPath(qwFree.LowPart,csPath.GetBuffer(qwFree.LowPart));
		csPath.ReleaseBuffer();
		
		GetDiskFreeSpaceEx(csPath,&qwFree,NULL,NULL);
		
		if (qwFree.QuadPart<qwSize)
		{
			RuntimeError(_T("Недостаточно места: %s"),csPath);
			return FALSE;
		}

		m_TempLog.SetEntryPosition();

/*
#ifdef _DEBUG_PURE
		LOG_DP_FileInArchiveInfo info;
		POSITION Pos=NULL;
		while (m_TempLog.GetEntry(Pos,info))
		{
			Msg(_T("%s:%s(%d) -> %u"),info.ArchiveName,info.EntryName,info.InArchive,info.Position);
			if (Pos==NULL)
				break;
		}
		return FALSE;
#endif
*/
		
		CComPtr<IStream> OldTempLog=NULL;
		CString csName;
		if (nCreate&LOG_DP_CMP_BEGDATE)
			csName=m_TempLog.GetFileName();
		else
		{
			GetTempFileName(csPath,_T("log"),0,csName.GetBuffer(csPath.GetLength()+MAX_PATH));
			csName.ReleaseBuffer();
			if (m_TempLog.Attached())
			{
				if (!CFileStream::CreateFileStream(m_TempLog.GetFileName(), FALSE, CFileStream::Share, GENERIC_READ, OldTempLog))
					return FALSE;
			}
		}

		CComPtr<IStream> NewTempLog=NULL;
		if (!CFileStream::CreateFileStream(csName, nCreate&LOG_DP_CMP_BEGDATE ? FALSE : TRUE, CFileStream::Share, GENERIC_WRITE, NewTempLog))
			return FALSE;
		qwFree.QuadPart=qwSize;
		if (NewTempLog->SetSize(qwFree) != S_OK)
			return FALSE;

		if (OldTempLog)
		{
			LARGE_INTEGER DistToMove;
			DistToMove.QuadPart=0;
			if (NewTempLog->Seek(DistToMove,STREAM_SEEK_SET,NULL) != S_OK)
				return FALSE;

			qwSize=0;
			bool bInArchive;
			bool bGrowUpper;

			LOG_DP_FileInArchiveInfo info;
			POSITION Pos=NULL;

			qwSize=0;
			while (m_TempLog.GetEntry(Forward,Pos,info))
			{
				if (GetControl())
				{
					CString csStatus=_T("Распаковка ")+info.ArchiveName+_T(':')+info.EntryName;
					pBkEndUI->DoStatusLine(csStatus);
				}

				if (qwSize)
				{
					if (bGrowUpper)
					{
						if (csOldFirstEntry.Compare(info.EntryName)==0)
						{
							nOffset=qwSize;
							DistToMove.QuadPart=qwSize;
							if (NewTempLog->Seek(DistToMove,STREAM_SEEK_CUR,NULL) != S_OK)
								return FALSE;
							bGrowUpper=false;
							qwSize=0;
						}
					}

					if (bInArchive!=info.InArchive)
					{
						if (bInArchive)
						{
							if (qwSize)
							{
								DistToMove.QuadPart=qwSize;
								if (NewTempLog->Seek(DistToMove,STREAM_SEEK_CUR,NULL) != S_OK)
									return FALSE;
								if (OldTempLog->Seek(DistToMove,STREAM_SEEK_CUR,NULL) != S_OK)
									return FALSE;
							}
						}
						else
						{
							qwFree.QuadPart=qwSize;
							if (OldTempLog->CopyTo(NewTempLog,qwFree,NULL,NULL) != S_OK)
								return FALSE;
						}
						bInArchive=info.InArchive;
						qwSize=info.Size;
					}
					else
						qwSize+=info.Size;
				}
				else
				{
					bInArchive=info.InArchive;
					qwSize=info.Size;
					bGrowUpper=csOldFirstEntry.Compare(info.EntryName)>0;
				}

				if (Pos==NULL)
				{
					if (!bInArchive)
					{
						qwFree.QuadPart=qwSize;
						if (OldTempLog->CopyTo(NewTempLog,qwFree,NULL,NULL) != S_OK)
							return FALSE;
					}
					break;
				}
			}
			OldTempLog.Release();
			OldTempLog=NULL;
			m_TempLog.Detach(FALSE);
		}

		if (!m_TempLog.Attached())
			m_TempLog.Attach(csName);

	}
	else
		RemoveTempRowsFromList();

	return TRUE;
}


BOOL CV7LogProvider::SetPeriodInternal(CString& csBegDate, CString& csEndDate)
{
	if (csBegDate.Compare(_T("        ;23:59:59;"))<=0)
		csBegDate.Empty();
	
	if (csEndDate.Compare(_T("        ;23:59:59;"))<=0)
		csEndDate.Empty();
	
	const CString csMaxDate=_T("99999999;99:99:99;");

	ProviderMode pmPrevMode=m_ProviderMode;
	
	if (csBegDate.IsEmpty()&&csEndDate.IsEmpty())
	{
		csEndDate=csMaxDate;
		m_ProviderMode=pmNormalBrowse;
	}
	else
	{
		if (csEndDate.IsEmpty())
			csEndDate=csMaxDate;
		if (m_UpperBorder.DateTime.Compare(csBegDate)||m_LowerBorder.DateTime.Compare(csEndDate))
			m_ProviderMode=pmBoundsSearch;
	}

	if (pmPrevMode==m_ProviderMode)
		return TRUE;

	ULONGLONG nOffset=0;
	
	BOOL bResult;
	if (bResult=csBegDate.Compare(csEndDate)<=0)
	{
		int nSearchInArchive=0;
		
		if (m_ProviderMode==pmBoundsSearch)
		{
			CString csDate=m_MainLog.GetStartDate();

			int nCmpDates=csBegDate.Compare(csDate);
			if (nCmpDates<=0)
			{
				if (m_bArchivesView)
					nSearchInArchive=nSearchInArchive|LOG_DP_CMP_BEGDATE;
				else
					csBegDate.Empty();
			}
			
			nCmpDates=csEndDate.Compare(csDate);
			if (nCmpDates<=0)
			{
				if (m_bArchivesView)
				{
					csDate=csEndDate;
					nSearchInArchive=nSearchInArchive|LOG_DP_CMP_ENDDATE;
					if (nCmpDates < 0)
						nSearchInArchive=nSearchInArchive|LOG_DP_TEMPLOG_MUST_BE_CREATED;
				}
				else
				{
					if (bResult=nCmpDates==0)
						csBegDate.Empty();
				}
			}
			
			if (bResult)
			{
				if (m_bArchivesView&&nSearchInArchive)
				{
					if (bResult=CreateTempLog(csBegDate.Left(8),csDate.Left(8),nOffset))
					{
						if (!m_TempLog.Attached())
						{
							if (nSearchInArchive&LOG_DP_TEMPLOG_MUST_BE_CREATED)
								bResult=FALSE;
							else
							{
								csBegDate.Empty();
								nSearchInArchive=0;
							}
						}
					}
				}

				if (bResult&&((nSearchInArchive&LOG_DP_TEMPLOG_MUST_BE_CREATED)==0))
				{
					if (bResult=m_MainLog.Seek(0,FILE_END))
					{
						m_MainLog.Reset();

						if (bResult=m_MainLog.GetRow(FALSE,FALSE,Backward,csDate)==grsSuccess)
						{
							csDate=csDate.Left(LOG_DP_DATE_TIME_LENGTH);
							
							if (bResult=csBegDate.Compare(csDate)<=0)
							{
								if (csEndDate.Compare(csDate)>=0)
									csEndDate=csMaxDate;
							}
						}
					}
				}
			}
		}
		
		if (bResult)
		{
			if (GetControl())
				pBkEndUI->DoStatusLine(_T("Поиск границ..."));

			if (nSearchInArchive&LOG_DP_CMP_BEGDATE)
			{
				m_UpperBorder.LogType=Temp;
				m_pCurLog=&m_TempLog;
			}
			else
			{
				m_UpperBorder.LogType=Main;
				m_pCurLog=&m_MainLog;
			}
			
			if (csBegDate.IsEmpty())
			{
				m_UpperBorder.Position=0;
				m_UpperBorder.DateTime.Empty();
			}
			else
			{
				if (bResult=SearchBound(FALSE,csBegDate,csEndDate,m_UpperBorder.Position))
				{
					if (m_UpperBorder.Position == -1)
					{
						if (m_UpperBorder.LogType == Temp)
						{
							m_UpperBorder.LogType=Main;
							m_pCurLog=&m_MainLog;
							bResult=SearchBound(FALSE,csBegDate,csEndDate,m_UpperBorder.Position);
						}
					}
					if (bResult)
					{
						if (m_UpperBorder.Position == -1)
							m_UpperBorder.Position=0;
						m_UpperBorder.DateTime=csBegDate;
					}
				}
			}
		}
		
		if (bResult)
		{
			if (nSearchInArchive&LOG_DP_CMP_ENDDATE)
			{
				m_LowerBorder.LogType=Temp;
				m_pCurLog=&m_TempLog;
			}
			else
			{
				m_LowerBorder.LogType=Main;
				m_pCurLog=&m_MainLog;
			}
			
			if (csEndDate.Compare(csMaxDate))
			{
				if (bResult=SearchBound(TRUE,csEndDate,csEndDate,m_LowerBorder.Position))
				{
					if (m_LowerBorder.Position == -1)
					{
						if ((nSearchInArchive&LOG_DP_TEMPLOG_MUST_BE_CREATED) == 0)
						{
							if (m_LowerBorder.LogType == Temp)
							{
								m_pCurLog=&m_MainLog;
								if (bResult=SearchBound(TRUE,csEndDate,csEndDate,m_LowerBorder.Position))
								{
									if (m_LowerBorder.Position)
										m_LowerBorder.LogType=Main;
								}
							}
						}
					}
					if (bResult)
					{
						if (m_LowerBorder.Position == -1)
							m_LowerBorder.Position=0;
						m_LowerBorder.DateTime=csEndDate;
					}
				}
			}
			else
			{
				m_LowerBorder.Position=0;
				m_LowerBorder.DateTime.Empty();
			}
		}
	}
	

	GetRowResult res=grrMatch;
	if (bResult)
	{
		if (m_UpperBorder.Position||m_LowerBorder.Position)
			m_ProviderMode=pmPeriodBrowse;
		else
		{
			if (m_UpperBorder.LogType==m_LowerBorder.LogType?m_UpperBorder.LogType==Main:false)
			{
				m_UpperBorder.Reset();
				m_LowerBorder.Reset();
				m_ProviderMode=pmNormalBrowse;
			}
			else
				m_ProviderMode=pmPeriodBrowse;
		}

		if (BufferMode())
		{
			if (m_RowsList.IsEmpty())
				res=FillRowsList();
			else
			{
				if (nOffset) // временный лог с ростом вверх
				{
					for (POSITION Pos=m_RowsList.GetHeadPosition(); Pos!=NULL; )
						m_RowsList.GetNext(Pos).Position+=nOffset;
				}

				ULONGLONG nBytesToRead=0, nBytesRead=0;

				LOG_DP_RowInfo HeadRow=m_RowsList.GetHead();
				LOG_DP_RowInfo TailRow=m_RowsList.GetTail();

				int nHowToAdd=0;

				if (TailRow<m_UpperBorder) // меньше начала периода
					nHowToAdd=nHowToAdd|static_cast<int>(amNormal);
				else
				{

					LOG_DP_RowInfo LowerBorder=m_LowerBorder;
					if (LowerBorder.Position == 0)
						LowerBorder.Position=GetFileSize(LowerBorder.LogType);

					if (HeadRow>m_UpperBorder)
					{
						if (HeadRow>=LowerBorder) // меньше начала периода
							nHowToAdd=nHowToAdd|static_cast<int>(amNormal);
						else // добавить кусок в начало
						{
							if (HeadRow.LogType==m_UpperBorder.LogType)
								nBytesToRead=HeadRow.Position-m_UpperBorder.Position;
							else
								nBytesToRead=m_TempLog.GetFileSize()-m_UpperBorder.Position+HeadRow.Position;

							nHowToAdd=nHowToAdd|static_cast<int>(amToHead);
						}
					}
					else
					{
						while (HeadRow<m_UpperBorder)
						{
							m_RowsList.RemoveHead();
							if (m_RowsList.IsEmpty())
								break;
							else
								HeadRow=m_RowsList.GetHead();
						}
					}

					if (TailRow<LowerBorder) // добавить кусок в конец
					{
						if (TailRow.LogType==LowerBorder.LogType)
							nBytesToRead+=LowerBorder.Position-TailRow.Position;
						else
							nBytesToRead+=m_TempLog.GetFileSize()-TailRow.Position+LowerBorder.Position;

						nHowToAdd=nHowToAdd|static_cast<int>(amToTail);
					}
					else
					{
						while (TailRow>=LowerBorder)
						{
							m_RowsList.RemoveTail();
							if (m_RowsList.IsEmpty())
								break;
							else
								TailRow=m_RowsList.GetTail();
						}
					}
				}

				if (nHowToAdd&static_cast<int>(amNormal))
				{
					m_RowsList.RemoveAll();
					res=FillRowsList();
				}
				else
				{
					if (nHowToAdd&static_cast<int>(amToHead))
						res=AddRowsToList(amToHead,m_UpperBorder,nBytesToRead,nBytesRead);
					if (nHowToAdd&static_cast<int>(amToTail))
					{
						if (res!=grrEscPressed)
							res=AddRowsToList(amToTail,TailRow,nBytesToRead,nBytesRead);
					}
				}
			}
		}
	}
	else
	{
		m_UpperBorder.Reset();
		m_LowerBorder.Reset();
		m_ProviderMode=pmSomeError;
	}

	if (GetControl())
	{
		pBkEndUI->DoStatusLine(_T("Обновление табличного поля..."));
		ResetData();
	}
	
	if (res==grrEscPressed)
		RuntimeError(_T("Log data provider: установка периода прервана по ESC."));

	return bResult;
}


BOOL CV7LogProvider::SetPeriodEx(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);

	if (!m_MainLog.Attached())
		return TRUE;

	if ((ppValue[0]->GetTypeCode() == DATE_TYPE_1C)&&
		(ppValue[1]->GetTypeCode() == STRING_TYPE_1C)&&
		(ppValue[2]->GetTypeCode() == DATE_TYPE_1C)&&
		(ppValue[3]->GetTypeCode() == STRING_TYPE_1C))
	{
		CString csBegDate=ppValue[1]->GetString();
		if (csBegDate.IsEmpty())
			csBegDate=_T("00:00:00");
		csBegDate=ppValue[0]->GetString()+_T(';')+csBegDate+_T(';');

		if (IsDateTimeValid(csBegDate))
		{
			CString csEndDate=ppValue[3]->GetString();
			if (csEndDate.IsEmpty())
				csEndDate=_T("23:59:59");
			csEndDate=ppValue[2]->GetString()+_T(';')+csEndDate+_T(';');

			if (IsDateTimeValid(csEndDate))
			{
				if (SetPeriodInternal(csBegDate,csEndDate))
					Result=CNumeric(1);
			}
		}
	}

	return TRUE;
}


BOOL CV7LogProvider::SetPeriod(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);

	if (!m_MainLog.Attached())
		return TRUE;

	if ((ppValue[0]->GetTypeCode() == DATE_TYPE_1C)&&(ppValue[1]->GetTypeCode() == DATE_TYPE_1C))
	{
		if (SetPeriodInternal(ppValue[0]->GetString()+_T(";00:00:00;"),ppValue[1]->GetString()+_T(";23:59:59;")))
			Result=CNumeric(1);
	}

	return TRUE;
}


BOOL CV7LogProvider::DefaultPeriod(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
	case 1:
		*param = CDate(0,0,0);
		break;
	}
	return TRUE;
}


BOOL CV7LogProvider::DefaultPeriodEx(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
	case 2:
		*param = CDate(0,0,0);
		break;
	case 1:
		*param = CValue(_T("00:00:00"));
		break;
	case 3:
		*param = CValue(_T("23:59:59"));
		break;
	}
	return TRUE;
}


CV7LogProvider::GetRowResult CV7LogProvider::FillRowsList()
{
	ULONGLONG nBytesToRead, nBytesRead=0;

	switch (m_ProviderMode)
	{
	case pmNormalBrowse:
		return AddRowsToList(amNormal,m_UpperBorder,m_MainLog.GetFileSize(),nBytesRead);
		break;
	case pmPeriodBrowse:
		if (m_UpperBorder.LogType==m_LowerBorder.LogType)
		{
			if (m_LowerBorder.Position)
				nBytesToRead=m_LowerBorder.Position;
			else
				nBytesToRead=GetFileSize(m_UpperBorder.LogType);
			nBytesToRead-=m_UpperBorder.Position;
			return AddRowsToList(amNormal,m_UpperBorder,nBytesToRead,nBytesRead);
		}
		else
		{
			if (m_UpperBorder.LogType<m_LowerBorder.LogType)
			{
				nBytesToRead=m_TempLog.GetFileSize()-m_UpperBorder.Position;
				if (m_LowerBorder.Position)
					nBytesToRead+=m_LowerBorder.Position;
				else
					nBytesToRead+=m_MainLog.GetFileSize();
				return AddRowsToList(amNormal,m_UpperBorder,nBytesToRead,nBytesRead);
			}
		}
		break;
	}
	return grrNoMoreRows;
}


CV7LogProvider::GetRowResult CV7LogProvider::AddRowsToList(
														   const AddingMode HowToAdd,
														   const LOG_DP_RowInfo& FromRow,
														   const ULONGLONG& nBytesToRead,
														   ULONGLONG& nBytesRead)
{
	switch (FromRow.LogType)
	{
	case Temp:
		m_pCurLog=&m_TempLog;
		break;
	case Main:
		m_pCurLog=&m_MainLog;
		break;
	}
	if (!m_pCurLog->Seek(FromRow.Position,FILE_BEGIN))
		return grrNoMoreRows;

	m_pCurLog->Reset();


	LOG_DP_RowInfo BorderRow;
	switch (HowToAdd)
	{
	case amToHead:
		if (!m_RowsList.IsEmpty())
			BorderRow=m_RowsList.GetHead();
		break;
	case amToTail:
		if (!m_RowsList.IsEmpty())
			BorderRow=m_RowsList.GetTail();
		break;
	}
	CList<LOG_DP_RowInfo,LOG_DP_RowInfo&> tmpList;

	int nScanDeep=m_nScanDeep;
	int nPercentPrev=-1;
	ULONGLONG nOnePercent=nBytesToRead/100;

	bool bWhile=true;
	while (bWhile)
	{
		LOG_DP_RowInfo NewRow;
		CString mRow[LOG_DP_FIELD_COUNT];

		switch (GetFilteredRow(TRUE,TRUE,Forward,NewRow,mRow))
		{
		case grrMismatch:
			if (m_nScanDeep)
				bWhile = --nScanDeep != 0;
			break;
		case grrMatch:
			{
				switch (HowToAdd)
				{
				case amNormal:
					m_RowsList.AddTail(NewRow);
					break;
				case amToHead:
					if (BorderRow==NewRow)
						bWhile=false;
					else
						tmpList.AddTail(NewRow);
					break;
				case amToTail:
					if (BorderRow!=NewRow)
						tmpList.AddTail(NewRow);
					break;
				}
				nScanDeep=m_nScanDeep;
			}
			break;
		case grrNoMoreRows:
			bWhile=false;
			break;
		case grrEscPressed:
			return grrEscPressed;
			break;
		}

		nBytesRead+=NewRow.Length;

		int nPercentCurr=100;
		if (nOnePercent)
			nPercentCurr=nBytesRead/nOnePercent;

		if (nPercentPrev!=nPercentCurr)
		{
			CString csStatus(_T("Процесс фильтрации: "));

			if (nPercentCurr>100)
				csStatus+=_T("завершение...");
			else
			{
				char buf[5];
				csStatus+=CString(ltoa(nPercentCurr,buf,10))+_T('%');
			}
			pBkEndUI->DoStatusLine(csStatus);
			nPercentPrev=nPercentCurr;
		}

	}

	switch (HowToAdd)
	{
	case amToHead:
		m_RowsList.AddHead(&tmpList);
		break;
	case amToTail:
		m_RowsList.AddTail(&tmpList);
		break;
	}

	int nCount=m_RowsList.GetCount();

	return grrMatch;
}


BOOL CV7LogProvider::SetFilter(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);

	if (!m_MainLog.Attached())
		return TRUE;
	if (m_ProviderMode==pmSomeError)
		return TRUE;

	bool bUseBuffer;
	bool bResult = ppValue[0]->GetTypeCode() == NUMBER_TYPE_1C;

	if (bResult)
	{
		switch (ppValue[0]->GetNumeric())
		{
		case 0:
			bUseBuffer=false;
			break;
		case 1:
			bUseBuffer=true;
			break;
		default:
			bResult=false;
			break;
		}
	}

	if (!bResult)
	{
		RuntimeError(_T("Log data provider: неверное значение параметра 'ИспользоватьБуфер'"));
		return TRUE;
	}

	if (bResult = m_Filter.SetFilter(bUseBuffer))
	{
		ApplyFilter();
		Result=CNumeric(1);
	}

	return TRUE;
}


void CV7LogProvider::ApplyFilter()
{
	if (GetControl())
	{
		GetRowResult res=grrMatch;
		
		m_RowsList.RemoveAll();
		
		if (BufferMode())
			res=FillRowsList();
		
		pBkEndUI->DoStatusLine(_T("Обновление табличного поля..."));
		ResetData();
		
		if (res==grrEscPressed)
			RuntimeError(_T("Log data provider: установка фильтра прервана по ESC."));
	}
}


BOOL CV7LogProvider::ChangeFilter(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);

	if (ppValue[0]->GetTypeCode() != NUMBER_TYPE_1C)
		return TRUE;

	int nIndex=ppValue[0]->GetNumeric();

	if (abs(nIndex) > LOG_DP_EVENTS_COUNT)
		return TRUE;

	if (nIndex == 0)
	{
		m_Filter.Reset(true);

		m_RowsList.RemoveAll();

		m_LastScanRow.Reset();

		Result=CNumeric(1);
		return TRUE;
	}

	const bool bAddFilter=nIndex>0;
	if (!bAddFilter)
		nIndex=-nIndex;

	BYTE nFilterParam;
	for (nFilterParam=0; nFilterParam<LOG_DP_EVENTS_COUNT; nFilterParam++)
	{
		if (LOG_DP_1C_Events[nFilterParam].Index == nIndex)
		{
			nIndex=nFilterParam;
			break;
		}
	}
	nFilterParam=LOG_DP_1C_Events[nIndex].FilterParameter;

	bool bClear=false;
	CStringList* pParamList=new CStringList;

	if (nFilterParam == LOG_DP_FILTER_PARAM_NO)
	{
		CString csParam;
		if (GetResourceString(csParam,LOG_DP_1C_Events[nIndex].ResourceID,erpIdentifier))
			pParamList->AddTail(csParam);
	}
	else
	{
		switch (ppValue[1]->GetTypeCode())
		{
		case UNDEFINE_TYPE_1C:
			if (bAddFilter)
			{
				delete pParamList;
				return TRUE;
			}
			bClear=true;
			break;
		case STRING_TYPE_1C:
			if (nFilterParam & LOG_DP_FILTER_PARAM_STR)
			{
				CString csParam=ppValue[1]->GetString();
				if (!csParam.IsEmpty())
					pParamList->AddTail(csParam);
			}
			else
			{
				delete pParamList;
				return TRUE;
			}
			break;
		case AGREGATE_TYPE_1C:
			if (nFilterParam & LOG_DP_FILTER_PARAM_VL)
			{
				if (CPtrArray* pVL=CValue2VL(*ppValue[1]))
				{
					for (int i=0; i<pVL->GetSize(); i++)
					{
						CValue* pVal=static_cast<CValue*>(pVL->GetAt(i));
						if (pVal->GetTypeCode() == STRING_TYPE_1C)
						{
							if (nFilterParam & LOG_DP_FILTER_PARAM_STR)
							{
								CString csParam=pVal->GetString();
								if (!csParam.IsEmpty())
									pParamList->AddTail(csParam);
							}
						}
						else
						{
							if (nFilterParam & LOG_DP_FILTER_PARAM_OBJ)
							{
								CString csParam;
								if (GetStringPresent(pVal,TRUE,csParam))
									pParamList->AddTail(csParam);
							}
						}
					}
				}
			}
			else
			{
				delete pParamList;
				return TRUE;
			}
			break;
		default:
			if (nFilterParam & LOG_DP_FILTER_PARAM_OBJ)
			{
				CString csParam;
				if (GetStringPresent(ppValue[1],TRUE,csParam))
					pParamList->AddTail(csParam);
			}
			else
			{
				delete pParamList;
				return TRUE;
			}
			break;
		}
	}

	BYTE nFilterType;
	if (SHORT nParent = LOG_DP_1C_Events[nIndex].Parent)
		nFilterType=nParent>0 ? LOG_DP_FILTER_EVENT : -nParent;
	else
		nFilterType=LOG_DP_FILTER_EVENTTYPE;

	if (bClear)
	{
		switch (nFilterType)
		{
		case LOG_DP_FILTER_USER:
			m_Filter.m_mapUser.RemoveAll();
			break;
		case LOG_DP_FILTER_MODE:
			m_Filter.m_strMode.Empty();
			break;
		case LOG_DP_FILTER_EVENTTYPE:
			m_Filter.m_mapEventType.RemoveAll();
			break;
		case LOG_DP_FILTER_EVENT:
			m_Filter.m_mapEvent.RemoveAll();
			break;
		case LOG_DP_FILTER_CATEGORY:
			m_Filter.m_strCategory.Empty();
			break;
		case LOG_DP_FILTER_COMMENT:
			m_Filter.m_strComment.Empty();
		case LOG_DP_FILTER_OBJECT:
		case LOG_DP_FILTER_OBJECTKIND:
			{
				CIStringMapToIndex* pMap;
				if (nFilterType == LOG_DP_FILTER_OBJECT)
					pMap=&m_Filter.m_mapObject;
				else
					pMap=&m_Filter.m_mapObjectKind;
				POSITION Pos=pMap->GetStartPosition();
				while (Pos!=NULL)
				{
					CString csKey;
					int nVal;
					pMap->GetNextAssoc(Pos,csKey,nVal);
					if (LOG_DP_1C_Events[nVal].Parent == -static_cast<SHORT>(nFilterType))
					{
						pMap->RemoveKey(csKey);
						Pos=pMap->GetStartPosition();
					}
				}
			}
			break;
		case LOG_DP_FILTER_PRESENT:
			m_Filter.m_strPresent.Empty();
			break;
		}
	}
	else
	{
		for (POSITION Pos=pParamList->GetHeadPosition(); Pos!=NULL; )
			m_Filter.ChangeFilter(nFilterType,nIndex,pParamList->GetNext(Pos),bAddFilter);
	}
	pParamList->RemoveAll();
	delete pParamList;

	Result=CNumeric(1);
	return TRUE;
}


BOOL CV7LogProvider::DefaultFilter(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		*param = CNumeric(LOG_DP_FILTER_CLEAR);
		break;
	case 1:
		*param = CValue();
		break;
	}
	return TRUE;
}


BOOL CV7LogProvider::GetUsingBuffer(CValue& Value) const
{
	Value=CNumeric(BufferMode()?1:0);
	return TRUE;
}


BOOL CV7LogProvider::BufferMode() const
{
	return m_Filter.UseBuffer() ? TRUE : FALSE;
}


BOOL CV7LogProvider::SetScanDeep(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);
	if ((ppValue[0]->GetTypeCode()==NUMBER_TYPE_1C)&&(ppValue[1]->GetTypeCode()==NUMBER_TYPE_1C))
	{
		long nVal=ppValue[0]->GetNumeric();
		if (nVal>=0)
		{
			if (m_nScanDeep!=nVal)
			{
				m_nScanDeep=nVal;
				if ((ppValue[1]->GetNumeric()==1)&&GetControl())
					ResetData();
			}
			Result=CNumeric(1);
		}
	}
	return TRUE;
}


BOOL CV7LogProvider::GetScanDeep(CValue& Value) const
{
	Value=CNumeric(m_nScanDeep);
	return TRUE;
}


BOOL CV7LogProvider::DefaultScanDeep(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		*param = CValue(CNumeric(0));
		break;
	case 1:
		*param = CValue(CNumeric(1));
		break;
	}
	return TRUE;
}


BOOL CV7LogProvider::GetLastScanDate(CValue& Value) const
{
	m_LastScanRow.GetDate(Value);
	return TRUE;
}


// все остальное
ULONGLONG CV7LogProvider::GetFileSize(const LOG_DP_LogType& LogType) const
{
	switch (LogType)
	{
	case Temp:
		return m_TempLog.GetFileSize();
		break;
	case Main:
		return m_MainLog.GetFileSize();
		break;
	}
	return 0;
}


BOOL CV7LogProvider::SearchBound(const BOOL bLowerBound, const CString& csSearchDate, const CString& csEndDate, ULONGLONG& nBound)
{
	CString csFoundDate, csRow;
	csFoundDate.Empty();
	csRow.Empty();

	LONGLONG nDistToMove=0;
	ULONGLONG qwFileSize=m_pCurLog->GetFileSize();
	ULONGLONG qwJumpSize;
	LOG_DP_SearchDirections dir;

	if (m_pCurLog->GetLogType()==Temp)
	{
		csFoundDate=csSearchDate.Left(8);
		dir = bLowerBound ? Backward : Forward;
		LOG_DP_FileInArchiveInfo info;
		POSITION Pos=NULL;
		BOOL bEntryFound;

		while (bEntryFound=m_pCurLog->GetEntry(dir,Pos,info))
		{
			if (dir==Forward)
			{
				if (csFoundDate.Compare(info.EntryName)<=0)
					break;
			}
			else
			{
				if (csFoundDate.Compare(info.EntryName)>=0)
					break;
			}
			if (Pos==NULL)
				break;
		}
		if (bEntryFound)
		{
			qwJumpSize=info.Size;
			nDistToMove=info.Position;
			csFoundDate.Empty();
		}
		else
			return FALSE;
	}
	else
		qwJumpSize=qwFileSize;

	LOG_DP_RowInfo FoundRow;

	int nLen=(sizeof(qwJumpSize)<<3)-1;
	while (!(qwJumpSize&(static_cast<ULONGLONG>(1)<<nLen)))
		nLen--;
	if ((static_cast<ULONGLONG>(1)<<nLen)<qwJumpSize)
		nLen++;
	int nCount=0;
	if (nLen>10)
		nCount=nLen-10; // размер буфера 2^10=1024 байт
	else
		FoundRow.Position=nDistToMove;

	while (true)
	{
		if (int nCmp=csSearchDate.Compare(csFoundDate))
		{
			if (nCmp<0)
				dir=Backward;
			else
				dir=Forward;
		}
		else
		{
			if (bLowerBound)
				dir=Forward;
			else
				dir=Backward;
		}

		BOOL bRandomSeeking=TRUE;
		if (nCount&&qwJumpSize)
		{
			nLen=csRow.GetLength();
			if ((qwJumpSize>>1)>=(nLen+LOG_DP_MIN_ROW_LENGTH))
				qwJumpSize=qwJumpSize>>1;
			else
			{
				if ((qwJumpSize+LOG_DP_MIN_ROW_LENGTH)<=nLen)
					qwJumpSize=nLen+LOG_DP_MIN_ROW_LENGTH;
			}

			if (dir==Backward)
			{
				if (nDistToMove<=qwJumpSize)
				{
					nDistToMove=0;
					bRandomSeeking=FALSE;
				}
				else
					nDistToMove-=qwJumpSize;
			}
			else
			{
				nDistToMove+=qwJumpSize;
				if (nDistToMove>qwFileSize)
				{
					nDistToMove=qwFileSize;
					bRandomSeeking=FALSE;
				}
			}
		}
		else
			nDistToMove=FoundRow.Position;

		if (m_pCurLog->Seek(nDistToMove,FILE_BEGIN))
		{
			m_pCurLog->Reset();

			if (nCount&&qwJumpSize)
			{
				FoundRow.Reset();
				m_pCurLog->GetRow(bRandomSeeking, TRUE, nDistToMove!=0?Backward:Forward, csRow, &FoundRow);
				csFoundDate=csRow.Left(LOG_DP_DATE_TIME_LENGTH);
				nCount--;
			}
			else
				break;
		}
		else
			return FALSE;
	}

	ULONGLONG nPrevPosition;

	nCount = INT_MAX;
	while (nCount)
	{
		nPrevPosition=FoundRow.Position;
		FoundRow.Reset();
		LOG_DP_GetRowStatus st=m_pCurLog->GetRow(FALSE,TRUE,dir,csRow,&FoundRow);
		csFoundDate=csRow.Left(LOG_DP_DATE_TIME_LENGTH);
		switch (st)
		{
		case grsSuccess:
			if (bLowerBound)
			{
				if (dir==Backward)
				{
					if (csSearchDate.Compare(csFoundDate)>=0)
					{
						nBound=nPrevPosition;
						return TRUE;
					}
				}
				else
				{
					if (csSearchDate.Compare(csFoundDate)<0)
					{
						nBound=FoundRow.Position;
						return TRUE;
					}
				}
			}
			else
			{
				if (dir==Backward)
				{
					if (csSearchDate.Compare(csFoundDate)>0)
					{
						nBound=nPrevPosition;
						return TRUE;
					}
				}
				else
				{
					if (int nCmp=csSearchDate.Compare(csFoundDate))
					{
						if (nCmp<0)
						{
							if (csEndDate.IsEmpty())
							{
								nBound=FoundRow.Position;
								return TRUE;
							}
							else
							{
								if (csFoundDate.Compare(csEndDate)<=0)
								{
									nBound=FoundRow.Position;
									return TRUE;
								}
								else
									return FALSE;
							}
						}
					}
					else
					{
						nBound=FoundRow.Position;
						return TRUE;
					}
				}
			}
			break;
		case grsBOF:
			if (bLowerBound)
				return FALSE;
			else
			{
				nBound=0;
				return TRUE;
			}
			break;
		case grsEOF:
			if (bLowerBound)
			{
				nBound=-1;
				return TRUE;
			}
			else
				return FALSE;
			break;
		case grsFileError:
			return FALSE;
			break;
		}
		nCount--;
	}

	return FALSE;
}


//CDataProvider
void CV7LogProvider::QueryRows(CDataRow* pRowFrom, int nRowsCount, int nPageSizeHint)
{
	if ((nRowsCount == 0) || !GetControl() || !m_MainLog.Attached() || m_Filter.m_bInvalidFilter)
	{
		m_nQueryRowsCount=0;
		return;
	}

    m_nQueryRowsCount = nRowsCount;
	m_pCurLog = NULL;

    if (pRowFrom)
	{
		LOG_DP_RowInfo FromRow=static_cast<CLogRow*>(pRowFrom)->m_RowInfo;

		if (BufferMode())
		{
			if (m_RowsList.IsEmpty())
				m_RowPosition=NULL;
			else
			{
				m_RowPosition=m_RowsList.Find(FromRow);
				if (m_RowPosition)
				{
					if (nRowsCount<0)
						m_RowsList.GetPrev(m_RowPosition);
					else
						m_RowsList.GetNext(m_RowPosition);
				}
				else
				{
					if (FromRow < m_RowsList.GetHead())
					{
						if (nRowsCount<0)
							m_RowPosition=NULL;
						else
							m_RowPosition=m_RowsList.GetHeadPosition();
					}
					else
					{
						if (FromRow > m_RowsList.GetTail())
						{
							if (nRowsCount<0)
								m_RowPosition=m_RowsList.GetTailPosition();
							else
								m_RowPosition=NULL;
						}
						else
						{
							m_RowPosition=NULL;
							for (POSITION Pos=m_RowsList.GetHeadPosition(); Pos!=NULL; )
							{
								if (FromRow < m_RowsList.GetAt(Pos))
								{
									m_RowPosition=Pos;
									break;
								}
								else
									m_RowsList.GetNext(Pos);
							}
						}
					}
				}
			}
			if (m_RowPosition == NULL)
				m_nQueryRowsCount=0;
		}
		else
		{
			BOOL bOK=TRUE;

			DWORD dwMoveMethod=FILE_BEGIN;

			switch (m_ProviderMode)
			{
			case pmNormalBrowse:
				if (nRowsCount < 0)
					bOK=FromRow.Position!=0;
				break;
			case pmPeriodBrowse:
				if (FromRow < m_UpperBorder)
				{
					if (nRowsCount<0)
						bOK=FALSE;
					else
						FromRow=m_UpperBorder;
				}
				else
				{
					LOG_DP_RowInfo LowerRow=m_LowerBorder;
					if (LowerRow.Position == 0)
						LowerRow.Position=GetFileSize(LowerRow.LogType);
					if (FromRow > LowerRow)
					{
						if (nRowsCount<0)
						{
							FromRow=LowerRow;
							dwMoveMethod=FILE_END;
						}
						else
							bOK=FALSE;
					}
				}
				break;
			default:
				bOK=FALSE;
				break;
			}
			if (bOK)
			{
				if (dwMoveMethod == FILE_END)
					FromRow.Position=0;
				if (nRowsCount > 0)
					FromRow.Position+=FromRow.Length;
				CLOG_DP_LogFile* pLog;
				if (FromRow.LogType == Main)
					pLog=&m_MainLog;
				else
					pLog=&m_TempLog;
				if (pLog->Seek(FromRow.Position,dwMoveMethod))
					m_pCurLog=pLog;
				else
				{
					m_nQueryRowsCount=0;
					m_ProviderMode=pmSomeError;
				}
			}
			else
				m_nQueryRowsCount=0;
		}
	}
	else
	{
		if (m_ProviderMode == pmSomeError)
			m_nQueryRowsCount=0;
		{
			if (BufferMode())
			{
				if (nRowsCount < 0)
					m_RowPosition = m_RowsList.GetTailPosition();
				else
					m_RowPosition = m_RowsList.GetHeadPosition();
				if (m_RowPosition == NULL)
					m_nQueryRowsCount=0;
			}
			else
			{
				BOOL bOK=TRUE;

				switch (m_ProviderMode)
				{
				case pmNormalBrowse:
					if (bOK=m_MainLog.Seek(0, nRowsCount < 0 ? FILE_END : FILE_BEGIN))
						m_pCurLog=&m_MainLog;
					break;
				case pmPeriodBrowse:
					if (m_UpperBorder.LogType==m_LowerBorder.LogType)
					{
						ULONGLONG nPosition;
						DWORD dwMoveMethod;

						if (nRowsCount<0)
						{
							nPosition = m_LowerBorder.Position;
							dwMoveMethod = m_LowerBorder.Position ? FILE_BEGIN : FILE_END;
						}
						else
						{
							nPosition = m_UpperBorder.Position;
							dwMoveMethod = FILE_BEGIN;
						}

						CLOG_DP_LogFile* pLog;
						if (m_UpperBorder.LogType == Main)
							pLog=&m_MainLog;
						else
							pLog=&m_TempLog;
						if (bOK=pLog->Seek(nPosition,dwMoveMethod))
							m_pCurLog=pLog;
					}
					else
					{
						if (m_UpperBorder.LogType<m_LowerBorder.LogType)
						{
							if (nRowsCount<0)
							{
								if (bOK=m_MainLog.Seek(m_LowerBorder.Position, m_LowerBorder.Position ? FILE_BEGIN : FILE_END))
									m_pCurLog=&m_MainLog;
							}
							else
							{
								if (bOK=m_TempLog.Seek(m_UpperBorder.Position,FILE_BEGIN))
									m_pCurLog=&m_TempLog;
							}
						}
						else
						{
							m_nQueryRowsCount=0;
							m_ProviderMode=pmSomeError;
						}
					}
					break;
				default:
					m_nQueryRowsCount=0;
					break;
				}

				if (!bOK)
				{
					m_nQueryRowsCount=0;
					m_ProviderMode=pmSomeError;
				}
			}
		}
	}

	if (m_pCurLog)
		m_pCurLog->Reset();
}


CV7LogProvider::GetRowResult CV7LogProvider::GetFilteredRow(
															const BOOL bSeeESC,
															const BOOL bCheckFilter,
															LOG_DP_SearchDirections Dir,
															LOG_DP_RowInfo& RowInfo,
															CString* pRow)
{
	
	CString strRow;
	LOG_DP_GetRowStatus st;
	
	while (true)
	{
		st=m_pCurLog->GetRow(FALSE,bCheckFilter,Dir,strRow,&RowInfo);
		
		if (st==grsSuccess)
			break;
		else
		{
			if ((m_ProviderMode==pmPeriodBrowse)&&(m_UpperBorder.LogType!=m_LowerBorder.LogType))
			{
				switch (Dir)
				{
				case Backward:
					if (st==grsBOF)
					{
						if (m_pCurLog==&m_MainLog)
						{
							if (m_TempLog.Attached())
							{
								if (m_TempLog.Seek(0,FILE_END))
								{
									m_pCurLog=&m_TempLog;
									m_pCurLog->Reset();
									st=grsSuccess;
								}
							}
						}
					}
					break;
				case Forward:
					if (st==grsEOF)
					{
						if (m_pCurLog==&m_TempLog)
						{
							if (m_MainLog.Seek(0,FILE_BEGIN))
							{
								m_pCurLog=&m_MainLog;
								m_pCurLog->Reset();
								st=grsSuccess;
							}
						}
					}
					break;
				}
			}
		
		if (st!=grsSuccess)
			break;
		}
	}
	
	if (st!=grsSuccess)
		return grrNoMoreRows;

	RowInfo.LogType=m_pCurLog->GetLogType();

	if (m_ProviderMode == pmPeriodBrowse)
	{
		if (RowInfo < m_UpperBorder) // если вышли за верхнюю границу
			return grrNoMoreRows;
		if (m_LowerBorder.Position)
		{
			if (RowInfo >= m_LowerBorder) // если вышли за нижнюю границу
				return grrNoMoreRows;
		}

		CString csDate=strRow.Left(LOG_DP_DATE_TIME_LENGTH);

		if (m_UpperBorder.DateTime.Compare(csDate)>0)
			return grrMismatch;

		if (!m_LowerBorder.DateTime.IsEmpty())
		{
			if (m_LowerBorder.DateTime.Compare(csDate)<0)
				return grrMismatch;
		}

	}

	RowInfo.Length=strRow.GetLength();

	strRow.Replace(_T("\"\""),CString(_T('\x1')));
	strRow.TrimLeft();
	strRow.TrimRight();
	
	for(int nCount=0; nCount<LOG_DP_FIELD_COUNT; nCount++)
	{
		pRow[nCount].Empty();
		
		CString csSeparator;
		switch (strRow[0])
		{
		case _T('\x1'):
		case _T('"'):
			csSeparator=_T("\";");
			break;
		default:
			csSeparator=_T(';');
			break;
		}
		int nFind=strRow.Find(csSeparator,0);
		
		CString csTmp;
		if (nFind==-1)
		{
			csTmp=strRow;
			strRow.Empty();
		}
		else
			csTmp=strRow.Left(nFind);
		
		if ((nCount==8)&&(!m_bDefaultLog))
			csTmp.Empty();
		else
		{
			csTmp.Remove(_T('"'));
			if (nCount==4)
			{
				if (csTmp[0] == _T('$'))
					csTmp=csTmp.Mid(1);
			}
		}
		csTmp.Replace(_T('\x1'),_T('"'));
		pRow[nCount]=csTmp;
		strRow=strRow.Mid(nFind+csSeparator.GetLength());
	}
	
	if (bCheckFilter)
	{
		if (bSeeESC)
		{
			MSG msg;
			if (PeekMessage(&msg,NULL,WM_KEYFIRST,WM_KEYLAST,PM_REMOVE))
			{
				//DefWindowProc(msg.hwnd,msg.message,msg.wParam,msg.lParam);
				if (msg.wParam == VK_ESCAPE)
				{
					if (m_nScanDeep)
					{
						m_LastScanRow.Position=RowInfo.Position;
						m_LastScanRow.LogType=RowInfo.LogType;
						m_LastScanRow.DateTime=pRow[0]+_T(';')+pRow[1]+_T(';');
					}
					return grrEscPressed;
				}
			}
		}
		
		if (m_Filter.m_dwFilter) // проверка фильтров
		{
			DWORD dwTest=1<<(LOG_DP_FILTER_USER-1);
			int nIndex=LOG_DP_FILTER_USER;
			while (nIndex<=LOG_DP_FILTER_OBJECTKIND)
			{
				if (m_Filter.m_dwFilter&dwTest)
				{
					BOOL IsRowValid;
					switch (nIndex)
					{
					case LOG_DP_FILTER_USER:
						{
							CString strTmp=pRow[2].IsEmpty()?_T("<>"):pRow[2];
							CObject* p;
							IsRowValid=m_Filter.m_mapUser.Lookup(strTmp,p);
						}
						break;
					case LOG_DP_FILTER_MODE:
						IsRowValid=m_Filter.m_strMode.Find(pRow[3])!=-1;
						break;
					case LOG_DP_FILTER_CATEGORY:
						IsRowValid=m_Filter.m_strCategory.Find(pRow[6])!=-1;
						break;
					case LOG_DP_FILTER_EVENTTYPE:
						if (pRow[4].IsEmpty())
							IsRowValid=FALSE;
						else
						{
							int nIndex;
							IsRowValid=m_Filter.m_mapEventType.Lookup(pRow[4],nIndex);
						}
						break;
					case LOG_DP_FILTER_EVENT:
						if (pRow[5].IsEmpty())
							IsRowValid=FALSE;
						else
						{
							int nIndex;
							IsRowValid=m_Filter.m_mapEvent.Lookup(pRow[5],nIndex);
						}
						break;
					case LOG_DP_FILTER_COMMENT:
						{
							CString strTmp=pRow[7];
							strTmp.MakeLower();
							IsRowValid=strTmp.Find(m_Filter.m_strComment)!=-1;
						}
						break;
					case LOG_DP_FILTER_OBJECT:
					case LOG_DP_FILTER_OBJECTKIND:
						if (pRow[8].IsEmpty())
							IsRowValid=FALSE;
						else
						{
							int nIndex;
							if (m_Filter.m_mapObject.IsEmpty())
								IsRowValid=FALSE;
							else
								IsRowValid=m_Filter.m_mapObject.Lookup(pRow[8],nIndex);
							if (!IsRowValid)
							{
								if (!m_Filter.m_mapObjectKind.IsEmpty())
								{
									if (m_Filter.m_mapObjectKind.Lookup(CString(pRow[8][0]),nIndex))
										IsRowValid=TRUE;
									else
									{
										nIndex=pRow[8].ReverseFind(_T('/'));
										if (nIndex==-1)
											IsRowValid=FALSE;
										else
											IsRowValid=m_Filter.m_mapObjectKind.Lookup(pRow[8].Left(nIndex),nIndex);
									}
								}
							}
						}
						break;
					case LOG_DP_FILTER_PRESENT:
						{
							CString strTmp=pRow[9];
							strTmp.MakeLower();
							IsRowValid=strTmp.Find(m_Filter.m_strPresent)!=-1;
						}
						break;
					}
					
					if (!IsRowValid)
					{
						if (m_nScanDeep)
						{
							m_LastScanRow.Position=RowInfo.Position;
							m_LastScanRow.LogType=RowInfo.LogType;
							m_LastScanRow.DateTime=pRow[0]+_T(';')+pRow[1]+_T(';');
						}
						return grrMismatch;
					}
				}
				dwTest=dwTest<<1;
				nIndex++;
			}
		}
	}
		
	return grrMatch;
}


CDataRow* CV7LogProvider::Fetch()
{
	if ((m_nQueryRowsCount == 0) || !GetControl())
        return NULL;

	LOG_DP_RowInfo NewRow;
	CString mRow[LOG_DP_FIELD_COUNT];

	LOG_DP_SearchDirections Dir = m_nQueryRowsCount < 0 ? Backward : Forward;
	GetRowResult result=grrNoMoreRows;

	if (BufferMode())
	{
		if (m_RowPosition)
		{
			if (Dir == Forward)
				NewRow=m_RowsList.GetNext(m_RowPosition);
			else
				NewRow=m_RowsList.GetPrev(m_RowPosition);
			switch (NewRow.LogType)
			{
			case Temp:
				m_pCurLog=&m_TempLog;
				break;
			case Main:
				m_pCurLog=&m_MainLog;
				break;
			}
			if (m_pCurLog->Seek(NewRow.Position,FILE_BEGIN))
			{
				m_pCurLog->Reset();
				result=GetFilteredRow(TRUE,FALSE,Forward,NewRow,mRow);
			}
		}
	}
	else
	{
		int nScanDeep=m_nScanDeep;

		while (true)
		{
			NewRow.Reset();
			result=GetFilteredRow(TRUE,TRUE,Dir,NewRow,mRow);
			if (result == grrMismatch)
			{
				if (m_nScanDeep)
				{
					if (--nScanDeep == 0)
					{
						result=grrNoMoreRows;
						break;
					}
				}
			}
			else
				break;
		}
	}

	if (result == grrMatch)
	{
		if (BufferMode())
		{
			if (m_RowPosition == NULL)
				m_nQueryRowsCount=0;
		}
		if (m_nQueryRowsCount)
		{
			if (Dir == Forward)
				m_nQueryRowsCount--;
			else
				m_nQueryRowsCount++;
		}
		return new CLogRow(NewRow,mRow);
	}
	else
	{
		m_nQueryRowsCount=0;
		return NULL;
	}
}


BOOL CV7LogProvider::RefreshRow(CDataRow* pRowFrom)
{
	if (GetControl())
	{
		CLogRow* pRow=static_cast<CLogRow*>(pRowFrom);
		if (pRow->m_RowInfo.LogType == Main)
			return pRow->m_RowInfo.Position < m_MainLog.GetFileSize();
		else
			return FALSE;
	}
	else
        return FALSE;
}


BOOL CV7LogProvider::CompareRows(CDataRow* pRow1, CDataRow* pRow2) const
{
	return (static_cast<CLogRow*>(pRow1)->m_RowInfo == static_cast<CLogRow*>(pRow2)->m_RowInfo);
}


void CV7LogProvider::OnAttach()
{
	if (OpenLog())
		ResetData();
}


BOOL CV7LogProvider::SelectLines(CValue& Result, CValue** ppValue)
{
	m_bLinesSelectionEnabled=FALSE;
	Result=CNumeric(0);

	if (GetControl())
		return TRUE;

	if (!m_MainLog.Attached())
		return TRUE;

	if (ppValue[0]->GetTypeCode()!=NUMBER_TYPE_1C)
		return TRUE;

	LOG_DP_RowInfo BuildRow;

	if (ppValue[0]->GetNumeric()<0)
	{
		BuildRow.Position=-ppValue[0]->GetNumeric();
		BuildRow.LogType=Temp;
	}
	else
	{
		BuildRow.Position=ppValue[0]->GetNumeric();
		BuildRow.LogType=Main;
	}

	if (GetRowByRowInfo(BuildRow,TRUE))
	{
		switch (BuildRow.LogType)
		{
		case Temp:
			m_pCurLog=&m_TempLog;
			break;
		case Main:
			m_pCurLog=&m_MainLog;
			break;
		}
		if (m_pCurLog->Seek(BuildRow.Position,FILE_BEGIN))
		{
			m_pCurLog->Reset();

			m_sCurrentRow.RowInfo=BuildRow;
			for (int i=0; i<LOG_DP_FIELD_COUNT; i++)
				m_sCurrentRow.Fields[i].Empty();

			m_bLinesSelectionEnabled=TRUE;

			Result=CNumeric(1);
		}
	}

	return TRUE;
}


BOOL CV7LogProvider::DefaultSelectLines(int nParam, CValue* param) const
{
	switch(nParam)
	{
	case 0:
		*param = CNumeric(0);
		break;
	}
	return TRUE;
}


BOOL CV7LogProvider::GetLine(CValue& Result, CValue** ppValue)
{
	Result=CNumeric(0);

	if (!m_bLinesSelectionEnabled)
		return TRUE;

	if (GetControl())
	{
		m_bLinesSelectionEnabled=FALSE;
		return TRUE;
	}

	if (!m_MainLog.Attached())
	{
		m_bLinesSelectionEnabled=FALSE;
		return TRUE;
	}

	int nScanDeep=m_nScanDeep;
	nScanDeep++;

	GetRowResult result=grrMismatch;
	LOG_DP_RowInfo RowInfo;

	do
	{
		RowInfo.Reset();
		if (m_nScanDeep)
		{
			if (--nScanDeep == 0)
			{
				result=grrNoMoreRows;
				break;
			}
		}

		result=GetFilteredRow(TRUE,TRUE,Forward,RowInfo,m_sCurrentRow.Fields);

	} while (result == grrMismatch);

	if (result==grrMatch)
	{
		m_sCurrentRow.RowInfo=RowInfo;
		Result=CNumeric(1);
	}
	else
	{
		m_sCurrentRow.RowInfo.Reset();
		for (int i=0; i<LOG_DP_FIELD_COUNT; i++)
			m_sCurrentRow.Fields[i].Empty();

		m_bLinesSelectionEnabled=FALSE;

		if (result==grrEscPressed)
			RuntimeError(_T("Log data provider: выборка строк прервана по ESC."));
	}
	return TRUE;
}


BOOL CV7LogProvider::RowGetPosition(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FIELD_POSITION,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetDate(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_DATE,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetTime(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_TIME,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetUser(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_USER,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetMode(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_MODE,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetEventType(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_EVENTTYPE,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetEvent(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_EVENT,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowCategory(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_CATEGORY,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetComment(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_COMMENT,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetObject(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_OBJECT,Value);
	return TRUE;
}


BOOL CV7LogProvider::RowGetObjectPresent(CValue& Value) const
{
	GetRowFieldValue(LOG_DP_FILTER_PRESENT,Value);
	return TRUE;
}


BOOL CV7LogProvider::GetArchivesView(CValue& Value) const
{
	Value=CNumeric(m_bArchivesView?1:0);
	return TRUE;
}


BOOL CV7LogProvider::GetTestBuild(CValue& Value) const
{
#ifdef _DEBUG_PURE
	Value=CNumeric(1);
#else
	Value=CNumeric(0);
#endif
	return TRUE;
}


BOOL CV7LogProvider::SetArchivesView(CValue const& Value)
{
	if (Value.GetTypeCode()==NUMBER_TYPE_1C)
	{
		switch (Value.GetNumeric())
		{
		case 0:
			m_bArchivesView=FALSE;
			break;
		case 1:
			m_bArchivesView=TRUE;
			break;
		}
	}
	return TRUE;
}


BOOL CV7LogProvider::GetLogFileName(CValue& Value) const
{
	Value=m_MainLog.GetFileName();
	return TRUE;
}


BOOL CV7LogProvider::SetLogFileName(CValue const& Value)
{
	if (Value.GetTypeCode()==STRING_TYPE_1C)
	{
		CString csNewFileName=Value.GetString();
		csNewFileName.TrimLeft();
		csNewFileName.TrimRight();

		if (GetDefaultLogFileName().CompareNoCase(csNewFileName)==0)
			csNewFileName.Empty();

		if (m_csLogFileName.CompareNoCase(csNewFileName))
		{
			m_csLogFileName=csNewFileName;
			if (GetControl())
			{
				CloseLog();
				OpenLog();
				ResetData();
			}
		}
	}
	else
		RuntimeError(_T("Имя файла должно иметь тип 'Строка'!"));

	return TRUE;
}


void CV7LogProvider::GetRowFieldValue(const int nFieldIndex, CValue& Value) const
{
	Value.Reset();

	if (m_bLinesSelectionEnabled)
	{
		if (nFieldIndex==LOG_DP_FIELD_POSITION)
		{
			switch (m_sCurrentRow.RowInfo.LogType)
			{
			case Temp:
				Value=-static_cast<long double>(static_cast<LONGLONG>(m_sCurrentRow.RowInfo.Position));
				break;
			case Main:
				Value=static_cast<long double>(static_cast<LONGLONG>(m_sCurrentRow.RowInfo.Position));
				break;
			}
		}
		else
		{
			if (!m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)].IsEmpty())
			{
				switch (nFieldIndex)
				{
				case LOG_DP_FILTER_DATE:
					Value=CValue(CDate(atoi(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)].Left(4)),atoi(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)].Mid(4,2)),atoi(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)].Mid(6,2))));
					break;
				case LOG_DP_FILTER_CATEGORY:
					Value=CValue(CNumeric(atoi(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)])));
					break;
				case LOG_DP_FILTER_OBJECT:
					LogValueFromString(Value,CString(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)]),1);
					break;
				case LOG_DP_FILTER_EVENTTYPE:
					if (CV7LogEventType* pEventType=GetEventTypeByName(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)]))
						Value.AssignContext(pEventType);
					else
						Value=m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)];
					break;
				case LOG_DP_FILTER_EVENT:
					if (CV7LogEvent* pEvent=GetEventByName(m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)]))
						Value.AssignContext(pEvent);
					else
						Value=m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)];
					break;
				case LOG_DP_FILTER_TIME:
				case LOG_DP_FILTER_USER:
				case LOG_DP_FILTER_MODE:
				case LOG_DP_FILTER_COMMENT:
				case LOG_DP_FILTER_PRESENT:
					Value=m_sCurrentRow.Fields[_ROW_FIELD_INDEX_(nFieldIndex)];
					break;
				}
			}
		}
	}
	else
	{
		if (GetControl())
			RuntimeError(_T("Log data provider: доступ к полям запрещен."));
	}
}


BOOL CV7LogProvider::GetRowByRowInfo(LOG_DP_RowInfo& RowInfo, const BOOL bCheckEnd)
{
	BOOL bRandomSeeking=TRUE;
	LOG_DP_SearchDirections Dir=Backward;
	DWORD dwMoveMethod=FILE_BEGIN;

	if (RowInfo <= m_UpperBorder)
	{
		RowInfo=m_UpperBorder;
		bRandomSeeking=FALSE;
		Dir=Forward;
	}

	LOG_DP_RowInfo LowerBorder=m_LowerBorder;
	if (LowerBorder.Position == 0)
		LowerBorder.Position=GetFileSize(LowerBorder.LogType);

	if (RowInfo >= LowerBorder)
	{
		if (bCheckEnd)
			return FALSE;
		RowInfo=LowerBorder;
		bRandomSeeking=FALSE;
		dwMoveMethod=FILE_END;
	}

	CLOG_DP_LogFile* pLog;
	if (RowInfo.LogType == Main)
		pLog=&m_MainLog;
	else
	{
		if (m_ProviderMode == pmPeriodBrowse)
			pLog=&m_TempLog;
		else
			FALSE;
	}
	if (!pLog->Attached())
		return FALSE;

	if (bRandomSeeking)
	{
		RowInfo.Position+=LOG_DP_DATE_TIME_LENGTH;
		if (RowInfo.Position >= pLog->GetFileSize())
		{
			bRandomSeeking=FALSE;
			dwMoveMethod=FILE_END;
		}
	}
	if (dwMoveMethod == FILE_END)
		RowInfo.Position=0;

	if (pLog->Seek(RowInfo.Position,dwMoveMethod))
	{
		pLog->Reset();
		CString csRow;
		return pLog->GetRow(bRandomSeeking,FALSE,Dir,csRow,&RowInfo) == grsSuccess;
	}

	return FALSE;
}


//CV7DataProvider
void CV7LogProvider::GetRowValue(CDataRow* pDataRow, CValue& value) const
{
	CLogRow* pRow=static_cast<CLogRow*>(pDataRow);
	LONGLONG nValue=pRow->m_RowInfo.Position;
	if (pRow->m_RowInfo.LogType == Temp)
		nValue=-nValue;
	value=static_cast<long double>(nValue);
}


CDataRow* CV7LogProvider::BuildRowByValue(const CValue& value)
{
	if (!GetControl())
		return NULL;

	if (value.GetTypeCode()!=NUMBER_TYPE_1C)
		return NULL;

	LOG_DP_RowInfo BuildRow;

	if (value.GetNumeric()<0)
	{
		BuildRow.Position=-value.GetNumeric();
		BuildRow.LogType=Temp;
	}
	else
	{
		BuildRow.Position=value.GetNumeric();
		BuildRow.LogType=Main;
	}

	if (GetRowByRowInfo(BuildRow,FALSE))
		return new CLogRow(BuildRow,NULL);
	else
		return NULL;
}


CV7DataRow* CV7LogProvider::GetV7DataRow(CDataRow* pDataRow) const
{
    return static_cast<CLogRow*>(pDataRow);
}


#ifdef _DEBUG_PURE
BOOL CV7LogProvider::GoTest(CValue** ppValue)
{
	return TRUE;
}
#endif


//////////////////////////////////////////////////////////////////////////
// вспомогательные функции
//////////////////////////////////////////////////////////////////////////
BOOL IsDateTimeValid(const CString& csDateTime)
{
	if (csDateTime.GetLength()!=LOG_DP_DATE_TIME_LENGTH)
		return FALSE;
	
	if (BOOL bIsValidRow=(csDateTime[8]==_T(';')) && (csDateTime[17]==_T(';')) && (csDateTime[11]==_T(':')) && (csDateTime[14]==_T(':')))
	{
		int nNum=csDateTime.GetLength();
		const CString csTemplate=_T("0123456789;:");
		
		for (int i=0;i<nNum;i++)
		{
			if (csTemplate.Find(csDateTime[i])==-1)
				return FALSE;
		}
		
		nNum=atoi(csDateTime.Left(4));
		if (bIsValidRow=((nNum>1979)&&(nNum<=2099)))
		{
			nNum=atoi(csDateTime.Mid(4,2));
			if (bIsValidRow=((nNum>0)&&(nNum<=12)))
			{
				nNum=atoi(csDateTime.Mid(6,2));
				if (bIsValidRow=((nNum>0)&&(nNum<=31)))
				{
					nNum=atoi(csDateTime.Mid(9,2));
					if (bIsValidRow=((nNum>=0)&&(nNum<=23)))
					{
						nNum=atoi(csDateTime.Mid(12,2));
						if (bIsValidRow=((nNum>=0)&&(nNum<=59)))
						{
							nNum=atoi(csDateTime.Mid(15,2));
							bIsValidRow=(nNum>=0)&&(nNum<=59);
						}
					}
				}
			}
		}
		return bIsValidRow;
	}

	return FALSE;
}
