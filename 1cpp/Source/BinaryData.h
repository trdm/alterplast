//////////////////////////////////////////////////////////////////////
// CFileStream реализация IStream на основе файла
// BinaryData объект, наследник CBLContext
//
// Разработчик: Дмитрий Ощепков aka DmitrO, mailto: dmitro@russian.ru
// Версия: 2.1
//////////////////////////////////////////////////////////////////////

#pragma once

class CFileStream: public CCmdTarget
{
public:
    enum OpenMode {Exclusive, Share, ShareRead, ShareWrite};
	static BOOL CreateFileStream(const char* FileName, BOOL fTruncate, OpenMode Mode, DWORD dwDesiredAccess, CComPtr<IStream> &FileStream);
private:
	CFileStream();
	~CFileStream();
	int Attach(const char* FileName, BOOL fTruncate, OpenMode Mode, DWORD dwDesiredAccess);
	void Detach();

// Interface Maps
protected:
    DECLARE_INTERFACE_MAP()

    BEGIN_INTERFACE_PART(Stream, IStream)
        STDMETHOD_(HRESULT, Clone)(IStream** ppstm);
        STDMETHOD_(HRESULT, Commit)(DWORD grfCommitFlags);
        STDMETHOD_(HRESULT, CopyTo)(IStream* pstm, ULARGE_INTEGER cb, ULARGE_INTEGER* pcbRead, ULARGE_INTEGER* pcbWritten);
        STDMETHOD_(HRESULT, LockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
        STDMETHOD_(HRESULT, Read)(void* pv, ULONG cb, ULONG* pcbRead);
        STDMETHOD_(HRESULT, Revert)();
        STDMETHOD_(HRESULT, Seek)(LARGE_INTEGER dlibMove, DWORD dwOrigin, ULARGE_INTEGER* plibNewPosition);
        STDMETHOD_(HRESULT, SetSize)(ULARGE_INTEGER libNewSize);
        STDMETHOD_(HRESULT, Stat)(STATSTG* pstatstg, DWORD grfStatFlag);
        STDMETHOD_(HRESULT, UnlockRegion)(ULARGE_INTEGER libOffset, ULARGE_INTEGER cb, DWORD dwLockType);
        STDMETHOD_(HRESULT, Write)(void const* pv, ULONG cb,ULONG* pcbWritten);
    END_INTERFACE_PART(Stream)
private:
	HANDLE m_hFile;

};

class CBinaryData: public CBLContext
{
DECLARE_DYNCREATE(CBinaryData);
public:
    CBinaryData();
    ~CBinaryData();
    
    BOOL LoadFromFile(const char* FileName, int Compress); //Загружает файл с диска в память, сжимая его
    BOOL SaveToFile(const char* FileName, int Decompress); //Сохраняет из памяти файл, разжимая его

    BOOL AttachToFile(const char* FileName, CFileStream::OpenMode, DWORD dwDesiredAccess = GENERIC_READ|GENERIC_WRITE); //Настраивается на работу с файлом
	IStream* GetIStream();
    void AttachToIStream(IStream* pIStream) {m_Stream = pIStream;};
    enum Encoding {ANSI, OEM, UTF_16, UTF_8} m_Encoding;
private:
	CComPtr<IStream> m_Stream;
    BOOL CreateImplicit();
    //int ReadString(CString &Data, UINT Length);
    //int ReadWString(LPWSTR* pData, int* pLen);
    int ReadEncString(CString &sData, UINT Length);
//CBLContext interface
public:
    virtual int  CallAsFunc(int,class CValue &,class CValue * *);
	virtual int  CallAsProc(int,class CValue * *);
	virtual void  DecrRef(void);
	virtual int  FindMethod(char const *)const;
	virtual int  FindProp(char const *)const;
	virtual char const *  GetCode(void)const;
	virtual int  GetDestroyUnRefd(void)const;
	virtual void  GetExactValue(class CValue &);
	virtual class CObjID   GetID(void)const;
	virtual class CBLContextInternalData *  GetInternalData(void);
	virtual char const *  GetMethodName(int,int)const;
	virtual int  GetNMethods(void)const;
	virtual int  GetNParams(int)const;
	virtual int  GetNProps(void)const;
	virtual int  GetParamDefValue(int,int,class CValue *)const;
	virtual char const *  GetPropName(int,int)const;
	virtual int  GetPropVal(int,class CValue &)const;
	virtual long  GetTypeID(void)const;
	virtual char const *  GetTypeString(void)const;
	virtual class CType   GetValueType(void)const;
	virtual int  HasRetVal(int)const;
	virtual void  IncrRef(void);
	virtual void  InitObject(class CType const &);
	virtual void  InitObject(char const *);
	virtual int  IsExactValue(void)const;
	virtual int  IsOleContext(void)const;
	virtual int  IsPropReadable(int)const;
	virtual int  IsPropWritable(int)const;
	virtual int  IsSerializable(void);
	virtual int  SaveToString(class CString &);
	virtual void  SelectByID(class CObjID,long);
	virtual int  SetPropVal(int,class CValue const &);

	static class CObjID ObjID;
private:
    static CParamDefs defFnNames;
};
