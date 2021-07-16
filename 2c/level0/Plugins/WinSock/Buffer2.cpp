// Buffer.cpp: implementation of the CBuffer class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Buffer2.h"
#include "Math.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif



//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	CBuffer
// 
// DESCRIPTION:	Constructs the buffer with a default size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
CBuffer::CBuffer()
{
	// Initial size
	m_nSize = 0;

	m_pPtr = m_pBase = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	~CBuffer
// 
// DESCRIPTION:	Deallocates the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
CBuffer::~CBuffer()
{
	if (m_pBase)
		VirtualFree(m_pBase,0,MEM_RELEASE);
}
	

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Write
// 
// DESCRIPTION:	Writes data into the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CBuffer::Write(PBYTE pData, UINT nSize)
{
	ReAllocateBuffer(nSize + GetBufferLen());

	CopyMemory(m_pPtr,pData,nSize);

	// Advance Pointer
	m_pPtr+=nSize;

	return nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Insert
// 
// DESCRIPTION:	Insert data into the buffer 
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CBuffer::Insert(PBYTE pData, UINT nSize)
{
	ReAllocateBuffer(nSize + GetBufferLen());

	MoveMemory(m_pBase+nSize,m_pBase,GetMemSize() - nSize);
	CopyMemory(m_pBase,pData,nSize);

	// Advance Pointer
	m_pPtr+=nSize;

	return nSize;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Read
// 
// DESCRIPTION:	Reads data from the buffer and deletes what it reads
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::Read(PBYTE pData, UINT nSize)
{
	// Trying to byte off more than ya can chew - eh?
	if (nSize > GetMemSize())
		return 0;

	// all that we have 
	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

		
	if (nSize)
	{
		// Copy over required amount and its not up to us
		// to terminate the buffer - got that!!!
		CopyMemory(pData,m_pBase,nSize);
		
		// Slide the buffer back - like sinking the data
		MoveMemory(m_pBase,m_pBase+nSize,GetMemSize() - nSize);

		m_pPtr -= nSize;
	}
		
	DeAllocateBuffer(GetBufferLen());

	return nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetMemSize
// 
// DESCRIPTION:	Returns the phyical memory allocated to the buffer
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::GetMemSize() 
{
	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBufferLen
// 
// DESCRIPTION:	Get the buffer 'data' length
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::GetBufferLen() 
{
	if (m_pBase == NULL)
		return 0;

	int nSize = 
		m_pPtr - m_pBase;
	return nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ReAllocateBuffer
// 
// DESCRIPTION:	ReAllocateBuffer the Buffer to the requested size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::ReAllocateBuffer(UINT nRequestedSize)
{
	if (nRequestedSize < GetMemSize())
		return 0;

	// Allocate new size
	UINT nNewSize = (UINT) ceil(nRequestedSize / 1024.0) * 1024;

	// New Copy Data Over
	PBYTE pNewBuffer = (PBYTE) VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);

	UINT nBufferLen = GetBufferLen();
	CopyMemory(pNewBuffer,m_pBase,nBufferLen);

	if (m_pBase)
		VirtualFree(m_pBase,0,MEM_RELEASE);


	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	DeAllocateBuffer
// 
// DESCRIPTION:	DeAllocates the Buffer to the requested size
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::DeAllocateBuffer(UINT nRequestedSize)
{
	if (nRequestedSize < GetBufferLen())
		return 0;

	// Allocate new size
	UINT nNewSize = (UINT) ceil(nRequestedSize / 1024.0) * 1024;

	if (nNewSize < GetMemSize())
		return 0;

	// New Copy Data Over
	PBYTE pNewBuffer = (PBYTE) VirtualAlloc(NULL,nNewSize,MEM_COMMIT,PAGE_READWRITE);

	UINT nBufferLen = GetBufferLen();
	CopyMemory(pNewBuffer,m_pBase,nBufferLen);

	VirtualFree(m_pBase,0,MEM_RELEASE);

	// Hand over the pointer
	m_pBase = pNewBuffer;

	// Realign position pointer
	m_pPtr = m_pBase + nBufferLen;

	m_nSize = nNewSize;

	return m_nSize;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Scan
// 
// DESCRIPTION:	Scans the buffer for a given byte sequence
// 
// RETURNS:		Logical offset
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
int CBuffer::Scan(PBYTE pScan,UINT nPos)
{
	if (nPos > GetBufferLen() )
		return -1;

	PBYTE pStr = (PBYTE) strstr((char*)(m_pBase+nPos),(char*)pScan);
	
	int nOffset = 0;

	if (pStr)
		nOffset = (pStr - m_pBase) + strlen((char*)pScan);

	return nOffset;
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	ClearBuffer
// 
// DESCRIPTION:	Clears/Resets the buffer
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CBuffer::ClearBuffer()
{
	// Force the buffer to be empty
	m_pPtr = m_pBase;

	DeAllocateBuffer(1024);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Write
// 
// DESCRIPTION:	Writes a string a the end of the buffer
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CBuffer::Write(CString& strData)
{
	int nSize = strData.GetLength();
	return Write((PBYTE) strData.GetBuffer(nSize), nSize);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Insert
// 
// DESCRIPTION:	Insert a string at the beginning of the buffer
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
BOOL CBuffer::Insert(CString& strData)
{
	int nSize = strData.GetLength();
	return Insert((PBYTE) strData.GetBuffer(nSize), nSize);
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Copy
// 
// DESCRIPTION:	Copy from one buffer object to another...
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
void CBuffer::Copy(CBuffer& buffer)
{
	int nReSize = buffer.GetMemSize();
	int nSize = buffer.GetBufferLen();
	ClearBuffer();
	ReAllocateBuffer(nReSize);

	m_pPtr = m_pBase + nSize;

	CopyMemory(m_pBase,buffer.GetBuffer(),buffer.GetBufferLen());
}

////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBuffer
// 
// DESCRIPTION:	Returns a pointer to the physical memory determined by the offset
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
PBYTE CBuffer::GetBuffer(UINT nPos)
{
	return m_pBase+nPos;
}


////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	GetBuffer
// 
// DESCRIPTION:	Returns a pointer to the physical memory determined by the offset
// 
// RETURNS:	
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
///////////////////////////////////////////////////////////////////////////////
void CBuffer::FileWrite(const CString& strFileName)
{
	CFile file;

	if (file.Open(strFileName, CFile::modeWrite | CFile::modeCreate))
	{
		file.Write(m_pBase,GetBufferLen());
		file.Close();
	}
}



////////////////////////////////////////////////////////////////////////////////
// 
// FUNCTION:	Delete
// 
// DESCRIPTION:	Delete data from the buffer and deletes what it reads
// 
// RETURNS:		
// 
// NOTES:	
// 
// MODIFICATIONS:
// 
// Name				Date		Version		Comments
// N T ALMOND       270400		1.0			Origin
// 
////////////////////////////////////////////////////////////////////////////////
UINT CBuffer::Delete(UINT nSize)
{
	// Trying to byte off more than ya can chew - eh?
	if (nSize > GetMemSize())
		return 0;

	// all that we have 
	if (nSize > GetBufferLen())
		nSize = GetBufferLen();

		
	if (nSize)
	{
		// Slide the buffer back - like sinking the data
		MoveMemory(m_pBase,m_pBase+nSize,GetMemSize() - nSize);

		m_pPtr -= nSize;
	}
		
	DeAllocateBuffer(GetBufferLen());

	return nSize;
}

#define HDR_SIZE sizeof(int)

BOOL CBuffer::IsEndRead()
{
	int nLen=GetBufferLen();
	if( nLen > HDR_SIZE)
	{
		int nSize = 0;
		CopyMemory(&nSize, GetBuffer(), HDR_SIZE);
		char b[1000];
		CopyMemory(b, GetBuffer(), min(1000,nLen));

		nLen-=HDR_SIZE;

		if(nLen>=nSize)
			return TRUE;
	}
	return FALSE;
}

CString CBuffer::GetString()
{
	
/*    CMemFile f(BUF_SIZE);
	while (GetBufferLen() > HDR_SIZE)
	{
		int nSize = 0;
		CopyMemory(&nSize, GetBuffer(), HDR_SIZE);

		if (nSize && GetBufferLen() >= nSize)
		{
			// Read off header
			Read((PBYTE) &nSize, HDR_SIZE);

			////////////////////////////////////////////////////////
			////////////////////////////////////////////////////////
			// SO you would process your data here
			// 
			// I'm just going to post message so we can see the data
			PBYTE pData = new BYTE[nSize];
			Read(pData,nSize);
			f.Write(pData,nSize);
			delete [] pData;
		}
		//else
		//	break;
	}


	CString Str;
	int nLength=f.GetLength();
	if(nLength>0)
	{
		f.SeekToBegin();
		f.ReadHuge(Str.GetBuffer(nLength),nLength);
		Str.ReleaseBuffer();
	}
	//OutToDebug("nLength=%d,Str=%s\n",nLength,Str);
	return Str;
//*/
//*	

	CString Str;
	int nLen=GetBufferLen();

	if(nLen>HDR_SIZE)
	{
		int nSize;
		// Read off header
		Read((PBYTE) &nSize, HDR_SIZE);

		int nTrueSize = nSize+1;
		PBYTE pData = new BYTE[nTrueSize];
		
		Read(pData,nSize);
		pData[nTrueSize-1] =  NULL;
		Str = pData;
		delete [] pData;
	}
	//OutToDebug("nLen=%d,Str=%s\n",nLen,Str);
	return Str;//*/
}
