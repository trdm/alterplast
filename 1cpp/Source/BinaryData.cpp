
#include "stdafx.h"
#include "BinaryData.h"
#include "zlibeng.tlh"

#include "System\sh_ptr.hpp"

extern CDataBase7 * pDataBase7;
extern CMetaDataCont * pMetaDataCont;
extern CBkEndUI * pBkEndUI;

/******************************************************************************************/
// CFileStream class implementation
/******************************************************************************************/

CFileStream::CFileStream(): m_hFile(NULL)
{
}

CFileStream::~CFileStream()
{
	Detach();
}

BOOL CFileStream::CreateFileStream(const char* FileName, BOOL fTruncate, OpenMode Mode, DWORD dwDesiredAccess, CComPtr<IStream> &FileStream)
{
	CFileStream* pFileStream = new CFileStream();
	if (!pFileStream->Attach(FileName, fTruncate, Mode, dwDesiredAccess))
	{
		delete pFileStream;
		return FALSE;
	}

	if (pFileStream->ExternalQueryInterface(&IID_IStream, (void**)&FileStream) != S_OK)
	{
		delete pFileStream;
		return FALSE;
	}
	pFileStream->ExternalRelease();

	return TRUE;
}

int CFileStream::Attach(const char* FileName, BOOL fTruncate, OpenMode Mode, DWORD dwDesiredAccess)
{
	Detach();
    
    DWORD CrDispos;
    if (fTruncate)
         CrDispos = CREATE_ALWAYS;
    else
         CrDispos = OPEN_ALWAYS;

    DWORD ShareMode;
    switch (Mode)
    {
    case Exclusive:
        ShareMode = 0;
        break;
    case Share:
        ShareMode = FILE_SHARE_READ | FILE_SHARE_WRITE;
        break;
    case ShareRead:
        ShareMode = FILE_SHARE_READ;
        break;
    case ShareWrite:
        ShareMode = FILE_SHARE_WRITE;
        break;
    }

    m_hFile = CreateFile(FileName, dwDesiredAccess, ShareMode, NULL, CrDispos, FILE_ATTRIBUTE_NORMAL, 0);
	if (m_hFile == INVALID_HANDLE_VALUE)
    {
        m_hFile = NULL;
	    return 0;
    }
	return 1;
}

void CFileStream::Detach()
{
	if (m_hFile)
	{
		CloseHandle(m_hFile);
		m_hFile = NULL;
	}
}

BEGIN_INTERFACE_MAP(CFileStream, CCmdTarget)
    INTERFACE_PART(CFileStream, IID_IStream, Stream)
END_INTERFACE_MAP()

ULONG FAR EXPORT CFileStream::XStream::AddRef()
{
    METHOD_PROLOGUE(CFileStream, Stream)
    return pThis->ExternalAddRef();
}

ULONG FAR EXPORT CFileStream::XStream::Release()
{
    METHOD_PROLOGUE(CFileStream, Stream)
    return pThis->ExternalRelease();
}

HRESULT FAR EXPORT CFileStream::XStream::QueryInterface(REFIID iid, void FAR* FAR* ppvObj)
{
    METHOD_PROLOGUE(CFileStream, Stream)
    return (HRESULT)pThis->ExternalQueryInterface(&iid, ppvObj);
}

HRESULT FAR EXPORT CFileStream::XStream::Clone(IStream** ppstm)
{
    METHOD_PROLOGUE(CFileStream, Stream)
    return E_FAIL;
}

HRESULT FAR EXPORT CFileStream::XStream::Commit(DWORD grfCommitFlags)
{
    METHOD_PROLOGUE(CFileStream, Stream)

    BOOL res = FlushFileBuffers(pThis->m_hFile);
    if (!res)
    {
        return S_FALSE;
    }
    return S_OK;
}

#define COPYTO_BUFFER_SIZE 0x60000

HRESULT FAR EXPORT CFileStream::XStream::CopyTo(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten)
{
    METHOD_PROLOGUE(CFileStream, Stream)

//trav
	ULONG nBufferSize=__min(cb.QuadPart,COPYTO_BUFFER_SIZE);

	void* pbuf = malloc(nBufferSize);
	//void* pbuf = malloc(COPYTO_BUFFER_SIZE);
//trav
    if (pbuf == NULL)
        return E_FAIL;

	ULONG nCurrentRead, nCurrentWrite;

	if (pcbRead) pcbRead->QuadPart = 0;
	if (pcbWritten) pcbWritten->QuadPart = 0;
    
    HRESULT ret = S_OK;

	while (cb.QuadPart != 0)
	{
//trav
		if (Read(pbuf, nBufferSize, &nCurrentRead) != S_OK)
		//if (Read(pbuf, COPYTO_BUFFER_SIZE, &nCurrentRead) != S_OK)
//trav
        {
		    ret = E_FAIL;
            break;
        }

//trav
        if (nCurrentRead == 0) break;
		if (pcbRead) pcbRead->QuadPart += nCurrentRead;

		//if (pcbRead) pcbRead->QuadPart += nCurrentRead;

        //if (nCurrentRead == 0) break;
//trav
		
		if (pstm->Write(pbuf, nCurrentRead, &nCurrentWrite) != S_OK)
        {
		    ret = E_FAIL;
            break;
        }

		if (pcbWritten) pcbWritten->QuadPart += nCurrentWrite;

		cb.QuadPart -= nCurrentWrite;
	}

    free(pbuf);
    return ret;
}

HRESULT FAR EXPORT CFileStream::XStream::LockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    METHOD_PROLOGUE(CFileStream, Stream)

    if (LockFile(pThis->m_hFile, libOffset.LowPart, libOffset.HighPart, cb.LowPart, cb.HighPart))
        return S_OK;
    else
        return S_FALSE;
}

HRESULT FAR EXPORT CFileStream::XStream::Read(void* pv, ULONG cb, ULONG* pcbRead)
{
    METHOD_PROLOGUE(CFileStream, Stream)

	DWORD cbRead;
    BOOL res = ReadFile(pThis->m_hFile, pv, cb, &cbRead, NULL);
    if (!res)
    {
        return S_FALSE;
    }

	if (pcbRead) *pcbRead = cbRead;

    return S_OK;
}

HRESULT FAR EXPORT CFileStream::XStream::Revert()
{
    METHOD_PROLOGUE(CFileStream, Stream)
    return E_FAIL;
}

HRESULT FAR EXPORT CFileStream::XStream::Seek(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition)
{
    METHOD_PROLOGUE(CFileStream, Stream)

	ULARGE_INTEGER NewPosition;
    NewPosition.LowPart = SetFilePointer(pThis->m_hFile, dlibMove.LowPart, &dlibMove.HighPart, dwOrigin);
    NewPosition.HighPart = dlibMove.HighPart;
    if (NewPosition.LowPart == 0xFFFFFFFF && GetLastError() != NO_ERROR)
    {
        return S_FALSE;
    }
	if (plibNewPosition) plibNewPosition->QuadPart = NewPosition.QuadPart;

    return S_OK;
}

HRESULT FAR EXPORT CFileStream::XStream::SetSize(ULARGE_INTEGER libNewSize)
{
    METHOD_PROLOGUE(CFileStream, Stream)

    LARGE_INTEGER liNewPos;
    liNewPos.QuadPart = libNewSize.QuadPart;

    if (Seek(liNewPos, STREAM_SEEK_SET, NULL) != S_OK)
        return S_FALSE;

    if (!SetEndOfFile(pThis->m_hFile))
        return S_FALSE;

    return S_OK;
}

HRESULT FAR EXPORT CFileStream::XStream::Stat(STATSTG* pstatstg, DWORD grfStatFlag)
{
    METHOD_PROLOGUE(CFileStream, Stream)
    USES_CONVERSION;

    if (!pstatstg)
        return E_FAIL;

    if (grfStatFlag == STATFLAG_DEFAULT)
	{
        //pstatstg->pwcsName = T2OLE(pThis->m_FileName);
	}

    pstatstg->type = STGTY_STREAM;
    if (pThis->m_hFile != INVALID_HANDLE_VALUE)
    {
        pstatstg->cbSize.LowPart = GetFileSize(pThis->m_hFile, &pstatstg->cbSize.HighPart);
        GetFileTime(pThis->m_hFile, &pstatstg->ctime, &pstatstg->atime, &pstatstg->mtime);
    }
    else
    {
        pstatstg->cbSize.QuadPart = 0;
    }
    pstatstg->grfLocksSupported = LOCK_EXCLUSIVE;
    pstatstg->clsid = IID_IStream;
    pstatstg->grfStateBits  = 0;

    return S_OK;
}

HRESULT FAR EXPORT CFileStream::XStream::UnlockRegion(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType)
{
    METHOD_PROLOGUE(CFileStream, Stream)
    if (UnlockFile(pThis->m_hFile, libOffset.LowPart, libOffset.HighPart, cb.LowPart, cb.HighPart))
        return S_OK;
    else
        return S_FALSE;
}

HRESULT FAR EXPORT CFileStream::XStream::Write(void const* pv, ULONG cb,ULONG* pcbWritten)
{
    METHOD_PROLOGUE(CFileStream, Stream)

	DWORD cbWritten;
    BOOL res = WriteFile(pThis->m_hFile, pv, cb, &cbWritten, NULL);
    if (!res)
    {
        return S_FALSE;
    }

	if (pcbWritten) *pcbWritten = cbWritten;

    return S_OK;
}


/******************************************************************************************/
// CBinaryData class implementation
/******************************************************************************************/

enum {
    funcLoadFromFile,
    funcSaveToFile,
    funcAttachToFile,
    procWriteString,
    procWriteData,
    funcReadString,
    funcReadData,
    funcSeek,
    funcSize,
    procWriteBOM,
    procReadBOM,
    funcLock,
    procUnlock,
    procClose,

    procZip,
    procUnzip,
    lastBinaryDataMethod
};

enum {
    propEncoding,
    lastBinaryDataProperty
};

struct stProp
{
    char* names[2];
    bool IsReadable;
    bool IsWritable;
};

stProp PropDefs[] = 
{
    {{"Кодировка", "Encoding"}, true, true},
};

class CObjID CBinaryData::ObjID;
class CParamDefs CBinaryData::defFnNames;

char TextTerminator[] = "\r\n";

BYTE Utf16BOM[] = {0xFF, 0xFE};
BYTE Utf8BOM[] = {0xEF, 0xBB, 0xBF};

IMPLEMENT_DYNCREATE(CBinaryData, CBLContext);

CBinaryData::CBinaryData()
{
  if (defFnNames.Empty())
  {     
    defFnNames.SetPoolSize(lastBinaryDataMethod, 0);        
    defFnNames.AddParam("LoadFromFile", "ЗагрузитьИзФайла", funcLoadFromFile, 1, 2);
    defFnNames.AddParam("SaveToFile", "СохранитьВФайл", funcSaveToFile, 1, 2);
    defFnNames.AddParam("AttachToFile", "ПодключитьсяКФайлу", funcAttachToFile, 1, 3);
    defFnNames.AddParam("WriteString", "ЗаписатьСтроку", procWriteString, 0, 2);
    defFnNames.AddParam("WriteData", "ЗаписатьДанные", procWriteData, 0, 2);
    defFnNames.AddParam("ReadString", "ПрочитатьСтроку", funcReadString, 1, 2);
    defFnNames.AddParam("ReadData", "ПрочитатьДанные", funcReadData, 1, 2);
    defFnNames.AddParam("Seek", "Перейти", funcSeek, 1, 2);
    defFnNames.AddParam("Size", "Размер", funcSize, 1, 1);
    defFnNames.AddParam("WriteBOM", "ЗаписатьМПБ", procWriteBOM, 0, 0);
    defFnNames.AddParam("ReadBOM", "ПрочитатьМПБ", procReadBOM, 0, 0);
    defFnNames.AddParam("Lock", "УстБлокировку", funcLock, 1, 3);
    defFnNames.AddParam("Unlock", "СнятьБлокировку", procUnlock, 0, 2);
    defFnNames.AddParam("Close", "Закрыть", procClose, 0, 0);

    defFnNames.AddParam("Zip", "Zip", procZip, 0, 1);
    defFnNames.AddParam("Unzip", "Unzip", procUnzip, 0, 1);
  }
  m_Encoding = ANSI;
}

CBinaryData::~CBinaryData()
{
}

BOOL CBinaryData::CreateImplicit()
{
    if (m_Stream == NULL)
    {
    	if (CreateStreamOnHGlobal(NULL, TRUE, &m_Stream) != S_OK)
	    	return FALSE;
    }
   	return TRUE;
}

BOOL CBinaryData::LoadFromFile(const char* FileName, int Compress)
{
    DWORD attributes = GetFileAttributes(FileName);
    if (attributes == 0xFFFFFFFF)
        return FALSE;

	CComPtr<IStream> FileStream;
    if (!CFileStream::CreateFileStream(FileName, FALSE, CFileStream::Exclusive, GENERIC_READ, FileStream))
		return FALSE;

//	m_Stream = NULL;

	if (!CreateImplicit())
		return FALSE;

	LARGE_INTEGER Move;
	Move.QuadPart = 0;
	m_Stream->Seek(Move, STREAM_SEEK_SET, NULL);

	ULARGE_INTEGER Size;
	Size.QuadPart =0;
	m_Stream->SetSize(Size);

	if (Compress)
	{
		CComPtr<IzlibEngine> Zlib;
		if (Zlib.CoCreateInstance(__uuidof(zlibEngine)) != S_OK)
			return FALSE;
		
		if (Zlib->pkCompress(FileStream, m_Stream) != S_OK)
			return FALSE;
	}
	else
	{
		ULARGE_INTEGER cb;
		cb.LowPart = cb.HighPart = 0xFFFFFFFF;
		if (FileStream->CopyTo(m_Stream, cb, NULL, NULL) != S_OK)
			return FALSE;
	}

    return TRUE;
}

BOOL CBinaryData::SaveToFile(const char* FileName, int Decompress)
{
	CComPtr<IStream> FileStream;
	if (!CFileStream::CreateFileStream(FileName, TRUE, CFileStream::Exclusive, GENERIC_WRITE, FileStream))
		return FALSE;
	
	if (m_Stream != NULL)
	{
		LARGE_INTEGER Move;
		Move.QuadPart = 0;

		if (Decompress)
		{
    		ULARGE_INTEGER Len;
            m_Stream->Seek(Move, STREAM_SEEK_END, &Len);
            if (Len.QuadPart)
            {
			    CComPtr<IzlibEngine> Zlib;
			    if (Zlib.CoCreateInstance(__uuidof(zlibEngine)) != S_OK)
				    return FALSE;
			    
        		m_Stream->Seek(Move, STREAM_SEEK_SET, NULL);

			    if (Zlib->pkDecompress(m_Stream, FileStream) != S_OK)
				    return FALSE;
            }
		}
		else
		{
    		m_Stream->Seek(Move, STREAM_SEEK_SET, NULL);

			ULARGE_INTEGER cb;
			cb.LowPart = cb.HighPart = 0xFFFFFFFF;
			if (m_Stream->CopyTo(FileStream, cb, NULL, NULL) != S_OK)
				return FALSE;
		}
	}

	return TRUE;
}

BOOL CBinaryData::AttachToFile(const char* FileName, CFileStream::OpenMode Mode, DWORD dwDesiredAccess)
{
	m_Stream = NULL;
	if (!CFileStream::CreateFileStream(FileName, FALSE, Mode, dwDesiredAccess, m_Stream))
		return FALSE;

	return TRUE;
}

IStream* CBinaryData::GetIStream()
{
    CreateImplicit();
	return m_Stream;
}

// CBLContext implemetnation

int  CBinaryData::CallAsFunc(int iMethNum,class CValue & rValue,class CValue * *ppValue)
{
    int ret = 0;
	switch(iMethNum)
  {
    case funcLoadFromFile:
        rValue = LoadFromFile(ppValue[0]->GetString(), ppValue[1]->GetNumeric());
        ret = 1;
      break;
    case funcSaveToFile:
        rValue = SaveToFile(ppValue[0]->GetString(), ppValue[1]->GetNumeric());
        ret = 1;
      break;
    case funcAttachToFile:
        CFileStream::OpenMode Mode;
        switch((int)ppValue[1]->GetNumeric())
        {
        case 0: Mode = CFileStream::Exclusive; break;
        case 1: Mode = CFileStream::Share; break;
        case 2: Mode = CFileStream::ShareRead; break;
        case 3: Mode = CFileStream::ShareWrite; break;
        default:
            CBLModule::RaiseExtRuntimeError("Недопустимое значение второго аргумента.", 0);
            return 0;
        }
        DWORD dwDesiredAccess;
        switch((int)ppValue[2]->GetNumeric())
        {
        case 0: dwDesiredAccess = GENERIC_READ|GENERIC_WRITE; break;
        case 1: dwDesiredAccess = GENERIC_READ; break;
        case 2: dwDesiredAccess = GENERIC_WRITE; break;
        default:
            CBLModule::RaiseExtRuntimeError("Недопустимое значение третьего аргумента.", 0);
            return 0;
        }
        rValue = AttachToFile(ppValue[0]->GetString(), Mode, dwDesiredAccess);
        ret = 1;
      break;
    case funcReadString:
      {
        if (!CreateImplicit())
            break;

        CString sData;
        switch (ReadEncString(sData, ppValue[1]->GetNumeric()))
        {
        case 0:
          {
            *ppValue[0] = sData;
            rValue = 1;
            ret = 1;
          }
          break;
        case 1:
          {
            rValue = 0L;
            ret = 1;
          }
          break;
        case -1:
          {
            CBLModule::RaiseExtRuntimeError("Ошибка чтения.", 0);
            break;
          }
          break;
        case -2:
          {
            CBLModule::RaiseExtRuntimeError("Недопустимая длина.", 0);
            break;
          }
          break;
        }
      }
      break;
    case funcReadData:
      {
        if (!CreateImplicit())
            break;

        long Data = 0;
        int type = ppValue[1]->GetNumeric();
        long len;

        switch(type)
        {
        case 0: len = 1;
        break;
        case 1: len = 2;
        break;
        case 2: len = 4;
        break;
        default:
            CBLModule::RaiseExtRuntimeError("Недопустимое значение второго параметра.", 0);
            break;
        }

        unsigned long ActualRead;
        if (m_Stream->Read(&Data, len, &ActualRead) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Неудалось прочитать данные.", 0);
            break;
        }

        if (ActualRead == 0)
        {
            rValue = 0L;
        }
        else if (ActualRead < len)
        {
            CBLModule::RaiseExtRuntimeError("Невозможно прочитать данные такой длины.", 0);
            break;
        }
        else
        {
            if (Data < 0)
            {
                CNumeric nData(Data & 0x7FFFFFFF), nData2;
                *ppValue[0] = nData + nData2.FromString("2147483648", NULL);
            }
            else
                *ppValue[0] = Data;
            rValue = 1;
        }
        ret = 1;
      }
      break;
    case funcSeek:
      {
        if (!CreateImplicit())
            break;

        LARGE_INTEGER Move;
        Move.QuadPart = ppValue[0]->GetNumeric();
        DWORD Origin = ppValue[1]->GetNumeric();
        ULARGE_INTEGER NewPos;
        if (m_Stream->Seek(Move, Origin, &NewPos) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Неудалось выполнить перемещение.", 0);
            break;
        }
        rValue = NewPos.QuadPart;
        ret = 1;
      }
      break;
    case funcSize:
      {
        if (!CreateImplicit())
            break;
        
        ULARGE_INTEGER NewSize;
        if (ppValue[0]->GetTypeCode())
        {//set size
            NewSize.QuadPart = ppValue[0]->GetNumeric();
            if (m_Stream->SetSize(NewSize) != S_OK)
                break;
        }
        else
        {//get size
            LARGE_INTEGER Move;
			ULARGE_INTEGER OldPos;
			
			Move.QuadPart = 0;
			if (m_Stream->Seek(Move, STREAM_SEEK_CUR, &OldPos) != S_OK)
				break;

            Move.QuadPart = 0;
            if (m_Stream->Seek(Move, STREAM_SEEK_END, &NewSize) != S_OK)
                break;

			Move.QuadPart = OldPos.QuadPart;
			if (m_Stream->Seek(Move, STREAM_SEEK_SET, &OldPos) != S_OK)
				break;
        }
        rValue = NewSize.QuadPart;
        ret = 1;
      }
      break;
    case funcLock:
      {
        CreateImplicit();

        ULARGE_INTEGER offset;
        offset.QuadPart = ppValue[0]->GetNumeric();
        ULARGE_INTEGER len;
        len.QuadPart = ppValue[1]->GetNumeric();

        DWORD timeout = ppValue[2]->GetNumeric();

        HANDLE hEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
        HRESULT hr;
        while ((hr = m_Stream->LockRegion(offset, len, 0)) != S_OK)
        {
            DWORD tmo = min(timeout, 50);
            if (tmo == 0)
                break;
            WaitForSingleObject(hEvent, tmo);
            timeout -= tmo;
        };

        CloseHandle(hEvent);

        if (hr == S_OK)
            rValue = 1;
        else
            rValue = 0L;

        ret = 1;
      }
      break;
    default:
      {
      }
  };
  return ret;
}

int  CBinaryData::CallAsProc(int iMethNum,class CValue * * ppValue)
{
    int ret = 0;
	switch(iMethNum)
  {
    case procWriteString:
      {
          if (!CreateImplicit())
              break;

          CString sData = ppValue[0]->GetString();

          if (ppValue[1]->GetNumeric() == 0)
              sData += TextTerminator;

          if (sData.GetLength() == 0)
          {
              ret = 1;
              break;
          };

          const void* pData;
          ULONG nLength;
          sh_ptr<WCHAR> pWStr;
          sh_ptr<CHAR> pUtfStr;

          switch(m_Encoding)
          {
          case ANSI:
              pData = sData;
              nLength = sData.GetLength();
              break;
          case OEM:
              nLength = sData.GetLength();
              pData = sData;
              CharToOem(sData, (LPSTR)pData);
              break;
          case UTF_16:
              pWStr = sh_ptr<WCHAR>(new WCHAR[sData.GetLength()]);
              MultiByteToWideChar(CP_ACP, 0, sData, sData.GetLength(), pWStr.get(), sData.GetLength());
              pData = pWStr.get();
              nLength = sData.GetLength() * 2;
              break;
          case UTF_8:
              pWStr = sh_ptr<WCHAR>(new WCHAR[sData.GetLength()]);
              MultiByteToWideChar(CP_ACP, 0, sData, sData.GetLength(), pWStr.get(), sData.GetLength());
              nLength = WideCharToMultiByte(CP_UTF8, 0, pWStr.get(), sData.GetLength(), NULL, 0, NULL, NULL);
              pUtfStr = sh_ptr<CHAR>(new char[nLength]);
              WideCharToMultiByte(CP_UTF8, 0, pWStr.get(), sData.GetLength(), pUtfStr.get(), nLength, NULL, NULL);
              pData = pUtfStr.get();
              break;
          default:
              nLength = 0;
          };

          if (m_Stream->Write(pData, nLength, NULL) != S_OK)
              CBLModule::RaiseExtRuntimeError("Неудалось записать данные.", 0);
          else
            ret = 1;
      }
      break;
    case procWriteData:
      {
          if (!CreateImplicit())
              break;

          long Data = ppValue[0]->GetNumeric();
          int type = ppValue[1]->GetNumeric();
          long len;

          switch(type)
          {
          case 0: len = 1;
          break;
          case 1: len = 2;
          break;
          case 2: len = 4;
          break;
          default:
              CBLModule::RaiseExtRuntimeError("Недопустимое значение второго параметра.", 0);
              break;
          }

          if (m_Stream->Write((char*)&Data, len, NULL) != S_OK)
          {
              CBLModule::RaiseExtRuntimeError("Неудалось записать данные.", 0);
              break;
          }
          ret = 1;
      }
      break;
    case procWriteBOM:
      {
        CreateImplicit();

        void* pData;
        ULONG Len;

        switch(m_Encoding)
        {
        case UTF_16:
            pData = Utf16BOM;
            Len = sizeof(Utf16BOM);
            break;
        case UTF_8:
            pData = Utf8BOM;
            Len = sizeof(Utf8BOM);
            break;
        default:
            pData = NULL;
        };
        if (pData)
        {
            if (m_Stream->Write(pData, Len, NULL) != S_OK)
                CBLModule::RaiseExtRuntimeError("Неудалось записать данные.", 0);
            else
                ret = 1;
        }
        else
            ret = 1;
      }
      break;
    case procReadBOM:
      {
        CreateImplicit();
        
        BYTE bufBOM[3];
        ULONG ActualRead;

        if (m_Stream->Read(bufBOM, 3, &ActualRead) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Неудалось прочитать данные.", 0);
        };

        if (ActualRead == sizeof(Utf8BOM))
        {
            if (!memcmp(bufBOM, Utf8BOM, sizeof(Utf8BOM)))
            {
                m_Encoding = UTF_8;
                ret = 1;
                break;
            }
        };

        if (ActualRead >= sizeof(Utf16BOM))
        {
            if (!memcmp(bufBOM, Utf16BOM, sizeof(Utf16BOM)))
            {
                m_Encoding = UTF_16;
                LARGE_INTEGER moveBack;
                moveBack.HighPart = 0;
                moveBack.LowPart = ActualRead - sizeof(Utf16BOM);
                if (moveBack.LowPart)
                {
                    moveBack.QuadPart = - moveBack.QuadPart;
                    m_Stream->Seek(moveBack, STREAM_SEEK_CUR, NULL);
                };
                ret = 1;
                break;
            }
        };

        if (ActualRead)
        {
            LARGE_INTEGER moveBack;
            moveBack.HighPart = 0;
            moveBack.LowPart = ActualRead;
            moveBack.QuadPart = - moveBack.QuadPart;
            m_Stream->Seek(moveBack, STREAM_SEEK_CUR, NULL);
        };

      }
      break;
    case procZip:
      {
        CComPtr<IzlibEngine> Zlib;
		if (Zlib.CoCreateInstance(__uuidof(zlibEngine)) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Ошибка создания объекта архиватора.", 0);
            ret = 0;
            break;
        };
        
        if (Zlib->pkZip((LPWSTR)ppValue[0]->GetString().operator LPCSTR()) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Ошибка в командной строке архиватора.", 0);
            ret = 0;
        };
      }
      break;
    case procUnzip:
      {
        CComPtr<IzlibEngine> Zlib;
		if (Zlib.CoCreateInstance(__uuidof(zlibEngine)) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Ошибка создания объекта архиватора.", 0);
            ret = 0;
            break;
        };

        if (Zlib->pkUnzip((LPWSTR)ppValue[0]->GetString().operator LPCSTR()) != S_OK)
        {
            CBLModule::RaiseExtRuntimeError("Ошибка в командной строке архиватора.", 0);
            ret = 0;
        };
      }
      break;
    case procUnlock:
      {
        CreateImplicit();

        ULARGE_INTEGER offset;
        offset.QuadPart = ppValue[0]->GetNumeric();
        ULARGE_INTEGER len;
        len.QuadPart = ppValue[1]->GetNumeric();
        if (m_Stream->UnlockRegion(offset, len, 0) != S_OK)
            CBLModule::RaiseExtRuntimeError("Ошибка снятия блокировки.", 0);
        else
            ret = 1;
      }
      break;
    case procClose:
      m_Stream = NULL;
      break;
    default:
      {
      }
  };
  return ret;
}


int  CBinaryData::FindMethod(char const * lpMethodName)const
{
	int i;
	for (i = 0;i<lastBinaryDataMethod;i++){
		if (!stricmp(lpMethodName,defFnNames[i].Names[0]))
			return i;
		if (!stricmp(lpMethodName,defFnNames[i].Names[1]))
			return i;
	}	
	return -1;
}

char const *  CBinaryData::GetMethodName(int iMethodNum,int iMethodAlias)const
{
  return defFnNames[iMethodNum].Names[iMethodAlias];
}

int  CBinaryData::GetNMethods(void)const
{
	return lastBinaryDataMethod;
}

int  CBinaryData::HasRetVal(int iMethodNum)const
{
  return defFnNames[iMethodNum].HasReturnValue ;
}

int CBinaryData::GetNParams(int iMethodNum)const
{
  return defFnNames[iMethodNum].NumberOfParams;
}

int  CBinaryData::GetParamDefValue(int iMethodNum,int iParamNum,class CValue * pDefValue)const
{	
	switch(iMethodNum)
  {
    case funcLoadFromFile:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case funcSaveToFile:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case funcAttachToFile:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
        else if (iParamNum == 2)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case procWriteString:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case procWriteData:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case funcReadString:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case funcReadData:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    case funcSeek:
      {       
        if (iParamNum == 1)
        {
            *pDefValue = 1L;
            return 1;
        }
      }
      break;
    case funcSize:
      {
        if (iParamNum == 0)
        {
            pDefValue->Reset();
            return 1;
        }
      }
      break;
    case funcLock:
      {
        if (iParamNum == 2)
        {
            *pDefValue = 0L;
            return 1;
        }
      }
      break;
    default:
      {
      }
  };
  return 0;
}

void  CBinaryData::DecrRef(void)
{
	CBLContext::DecrRef();
}


char const *  CBinaryData::GetCode(void)const
{
	return 0;
}

int  CBinaryData::GetDestroyUnRefd(void)const
{
	return 1;
}

void  CBinaryData::GetExactValue(class CValue & vParam)
{
	CBLContext::GetExactValue(vParam);
}

class CObjID   CBinaryData::GetID(void)const
{
	return ObjID;
}

class CBLContextInternalData *  CBinaryData::GetInternalData(void)
{
	return CBLContext::GetInternalData();
}

long  CBinaryData::GetTypeID(void)const
{
	return 100;
}

char const *  CBinaryData::GetTypeString(void)const
{
	return "BinaryData";
}

class CType   CBinaryData::GetValueType(void)const
{
	CType tType(100);	
	return tType;
}


void  CBinaryData::IncrRef(void)
{
	CBLContext::IncrRef();
}

void  CBinaryData::InitObject(class CType const & tType)
{
	CBLContext::InitObject(tType);
}

void  CBinaryData::InitObject(char const * strName)
{
	CBLContext::InitObject(strName);
}

int  CBinaryData::IsExactValue(void)const
{
	return 0;
}

int  CBinaryData::IsOleContext(void)const
{
	return 0;
}

int  CBinaryData::IsPropReadable(int iPropNum)const
{
	return PropDefs[iPropNum].IsReadable;
}

int  CBinaryData::IsPropWritable(int iPropNum)const
{
	return PropDefs[iPropNum].IsWritable;
}

int  CBinaryData::IsSerializable(void)
{
	return 0;
}

int  CBinaryData::SaveToString(class CString & csStr)
{	
	return 0;
}

void  CBinaryData::SelectByID(class CObjID cID,long lNum)
{
	CBLContext::SelectByID(cID,lNum);
}


int  CBinaryData::GetNProps(void)const
{
	return lastBinaryDataProperty;
}

char const *  CBinaryData::GetPropName(int A,int B)const
{	
    return PropDefs[A].names[B];
}

int  CBinaryData::GetPropVal(int iPropNum,class CValue & rValue)const
{
    rValue = m_Encoding;
	return 1;
}

int  CBinaryData::SetPropVal(int iPropNum,class CValue const & vValue)
{
    UINT n = vValue.GetNumeric();
    if (n > UTF_8)
    {
        CBLModule::RaiseExtRuntimeError("Недопустимая кодировка.", 0);
        return 0;
    }
    m_Encoding = (Encoding)n;
	return 1;
}

int  CBinaryData::FindProp(char const * Name)const
{
    for (int i = 0; i < lastBinaryDataProperty; i++)
        if (!stricmp(PropDefs[i].names[0], Name) || !stricmp(PropDefs[i].names[1], Name))
            return i;
	return -1;
}

#define PART_SIZE 256

int StrLen(LPCSTR pStr)
{
    return strlen(pStr);
};

int StrLen(LPCWSTR pStr)
{
    return wcslen(pStr);
};

LPSTR FindTerm(LPCSTR pStr)
{
    return strstr(pStr, "\r\n");
};

LPWSTR FindTerm(LPCWSTR pStr)
{
    return wcsstr(pStr, L"\r\n");
};

template <class T>
int ReadStr(IStream* pStream, T*& pStr, int Len)
{
    ULONG ActualRead;

    if (Len)
    {
        pStr = (T*)malloc((Len + 1) * sizeof(T));
        pStr[Len] = 0;
        if (pStream->Read(pStr, Len * sizeof(T), &ActualRead) != S_OK)
            return -1;

        if (ActualRead == 0)
            return 1;

        if (StrLen(pStr) != Len)
        {
            LARGE_INTEGER Move;
            Move.HighPart = 0;
            Move.LowPart = ActualRead;
            Move.QuadPart = - Move.QuadPart;
            pStream->Seek(Move, STREAM_SEEK_CUR, NULL);
            return -2;
        };
    }
    else
    {
        T* pTerm = NULL;
        while (!pTerm)
        {
            if (Len)
                pStr = (T*)realloc(pStr, (Len + PART_SIZE + 1) * sizeof(T));
            else
                pStr = (T*)malloc((PART_SIZE + 1) * sizeof(T));

            T* pPartData = pStr + Len;

            if (pStream->Read(pPartData, PART_SIZE * sizeof(T), &ActualRead) != S_OK)
                return -1;

            if (ActualRead == 0)
                return 1;

            int ARInChars = ActualRead / sizeof(T);
            pPartData[ARInChars] = 0;

            pTerm = FindTerm(pPartData);

            int AddLen, TermLen;

            if (pTerm)
            {
                AddLen = pTerm - pPartData;
                *pTerm = 0;
                TermLen = 2;
            }
            else
            {
                AddLen = StrLen(pPartData);
                if (AddLen < ARInChars)
                {
                    pTerm = pPartData;
                    TermLen = 1;
                }
                else
                {
                    TermLen = 0;
                    if (ARInChars == PART_SIZE)
                        AddLen--;
                    else
                        pTerm = pPartData;
                }
            };

            Len += AddLen;

            LARGE_INTEGER Move;
            Move.LowPart = ActualRead - (AddLen + TermLen) * sizeof(T);
            if (Move.LowPart)
            {
                Move.HighPart = 0;
                Move.QuadPart = - Move.QuadPart;
                if (pStream->Seek(Move, STREAM_SEEK_CUR, NULL) != S_OK)
                    return -1;
            }
        };
    };
    return 0;
};

int CBinaryData::ReadEncString(CString &sData, UINT Length)
{
    int ret;
    LPSTR pStr = NULL;
    LPWSTR pWStr = NULL;
    
    pStr = NULL;

    switch(m_Encoding)
    {
    case ANSI:
        ret = ReadStr(m_Stream, pStr, Length);
        if (ret == 0)
            sData = pStr;
        break;
    case OEM:
        ret = ReadStr(m_Stream, pStr, Length);
        if (ret == 0)
        {
            OemToChar(pStr, pStr);
            sData = pStr;
        };
        break;
    case UTF_16:
      {
        ret = ReadStr(m_Stream, pWStr, Length);
        if (ret == 0)
        {
            int Len = wcslen(pWStr);
            WideCharToMultiByte(CP_ACP, 0, pWStr, Len, sData.GetBufferSetLength(Len), Len, NULL, NULL);
        };
      }
      break;
    case UTF_8:
      {
        if (Length)
        {
            ret = -2;
            break;
        };
        ret = ReadStr(m_Stream, pStr, 0);
        if (ret == 0)
        {
            int LenUtf8 = strlen(pStr);
            int Len = MultiByteToWideChar(CP_UTF8, 0, pStr, LenUtf8, NULL, 0);
            pWStr = (LPWSTR)malloc(Len * sizeof(WCHAR));
            MultiByteToWideChar(CP_UTF8, 0, pStr, LenUtf8, pWStr, Len);
            WideCharToMultiByte(CP_ACP, 0, pWStr, Len, sData.GetBufferSetLength(Len), Len, NULL, NULL);
        };
      }
      break;
    };

    if (pStr)
        free(pStr);

    if (pWStr)
        free(pWStr);

    return ret;
}

