//#include <afx.h>
#include <afxdb.h>
#include <windows.h>
#include <ole2.h>
#include <atlconv.h>
#include <afxtempl.h>

#include <iostream>

using namespace std;

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
// initialize MFC and print and error on failure
  if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
  {
    cerr << _T("Fatal Error: MFC initialization failed") << endl;
    return 1;
  }
  else
  {
    LPCTSTR lpFileName; //имя storage-файла
    CString strStreamPath;//путь к нужному stream внутри storage-файла
    if (argc != 3)// если параметров нет - вывод справки
    {
      cerr << _T("Usage: compound_extr.exe  ") << endl;
      return 1;
    }
    else { //если параметры есть - распределяем их по соответствующим переменным
      lpFileName = argv[1];
      strStreamPath = (LPSTR) argv[2];
    }
    IStorage* pStgRoot = NULL;
    HRESULT hr;
    USES_CONVERSION;
    // get IStorage
    //открытие storage-файла
    hr = ::StgOpenStorage(
                          T2COLE(lpFileName),
                          NULL,
                          STGM_TRANSACTED | STGM_READ | STGM_SHARE_DENY_WRITE,
                          NULL,
                          0,
                          &pStgRoot);
    if (FAILED(hr))
    {	//ошибка
      cerr << _T("Fatal Error: Can't open the STORAGE file: ") << hr << endl;
      return 1;
    }
    //Далее, с помощью функции OpenStream откроем нужный Stream - файл.
    //Формат:
    //	//	HRESULT OpenStream(
    //	const WCHAR *pwcsName, указатель на имя открываемого потка
    //	void *reserved1, зарезирвированно NULL
    //	DWORD grfMode, режим доступа
    //	DWORD reserved2, зарезирвированно = 0
    //	IStream **ppstm указатель на поток
    //);
    CString csStmStgName;
    BOOL	StreamIsFound = FALSE;
    IStream*	pStream = NULL;
    IStorage*	pStgCurrent = pStgRoot;
    CList<IStorage*, IStorage*> stgList;
    int iFrontSlash(0);
    while (!StreamIsFound)
    {
    IStorage*	pStgNext = NULL;
    int iBackSlash = strStreamPath.Find('\\', iFrontSlash+1);
    if (iFrontSlash == strStreamPath.ReverseFind('\\'))
    {
      csStmStgName = strStreamPath.Right(
      strStreamPath.GetLength() - (iFrontSlash+1));
      StreamIsFound = TRUE;
    }
    else
    {
      csStmStgName = strStreamPath.Mid(
      iFrontSlash+1, iBackSlash - (iFrontSlash+1));
      iFrontSlash = iBackSlash;
    }
    cout << (LPCTSTR) csStmStgName;
    // STGTY_STORAGE or STGTY_STREAM
    hr = (StreamIsFound) ?
    pStgCurrent->OpenStream(
                            T2COLE(csStmStgName),
                            0,
                            STGM_READ | STGM_SHARE_EXCLUSIVE,
                            0,
                            &pStream):
                            pStgCurrent->OpenStorage(
                            T2COLE(csStmStgName),
                            NULL,
                            STGM_READ | STGM_SHARE_EXCLUSIVE,
                            NULL,
                            0,
                            &pStgNext);
    
    SUCCEEDED(hr) ? cout << _T(" ...SUCCEEDED") << endl: cout << _T(" ...FAILED") << endl;

    if (!StreamIsFound)
    {
      if (pStgNext != NULL)
      {
        stgList.AddTail(pStgNext);
        pStgCurrent = pStgNext;
      }
      else {
        cerr << _T("pSTG == NULL: now will be break!") << endl;
        return 1;
      }
      }
    }
    // Retrieves the STATSTG structure for this stream
    STATSTG statstg;
    hr = pStream->Stat(&statstg, STATFLAG_NONAME);
    if (FAILED(hr))
    cerr << _T("Fatal Error: Can't get the STAT: ") << hr << endl;
    else
    {
    ULONG cbRead;
    ULONG cbStreamSize = statstg.cbSize.LowPart;
    HGLOBAL hGlobal = ::GlobalAlloc(GMEM_MOVEABLE, cbStreamSize);
    if (hGlobal != NULL)
    {
    LPVOID lpMemStream = ::GlobalLock(hGlobal);
    hr = pStream->Read(lpMemStream, cbStreamSize, &cbRead);
    if (FAILED(hr))
    cerr << _T("Fatal Error: Can't READ the Stream: ") << hr << endl;
    else
    {
    TRY
    {
    CFile fDiskStream(
    csStmStgName,
    CFile::modeCreate | CFile::modeWrite);
    fDiskStream.Write(lpMemStream, cbRead);
    fDiskStream.Close();
    }
    CATCH(CFileException, e)
    {
    #ifdef _DEBUG
    afxDump << "File could not be opened " << e->m_cause << "\n";
    #endif
    }
    END_CATCH
    }
    pStream->Release();
    ::GlobalUnlock(hGlobal);
    ::GlobalFree(hGlobal);
    }
    else
    cerr << "No enough memory, clean up and return NULL" << endl;
    }
    while(!stgList.IsEmpty())
    (stgList.RemoveTail())->Release();
    pStgRoot->Release();
  }
  return 0;
}

