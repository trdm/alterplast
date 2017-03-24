
#include "stdafx.h"
#include "SerialStorage.h"

CSerialStorage::CSerialStorage()
{
	File = NULL;
	buf_sz = 0;
	buf = NULL;
	buf_offset = 0;
	str_ptr = NULL;
}

CSerialStorage::CSerialStorage(char const* str)
: String(str)
{
	File = NULL;
	buf_sz = 0;
	buf = NULL;
	buf_offset = 0;
	str_ptr = (char*)(LPCSTR)String;
}

CSerialStorage::CSerialStorage(FILE * f)
{
	File = f;
	buf_sz = 10 * 1024;
	buf = new char[buf_sz];
	buf_offset = 0;
	str_ptr = (char*)(LPCSTR)String;
}

CSerialStorage::~CSerialStorage()
{
	if( File )
		fflush(File);
	if( buf )
		delete[] buf;
}

void CSerialStorage::ResizeBuf(int nAdded)
{
	int sz_needed = buf_sz + nAdded + 1;
	int new_sz = buf_sz * 2;
	if( buf_sz == 0 ) new_sz = 10 * 1024;
	while( new_sz < sz_needed ) new_sz *= 2;
	char *new_buf = new char[new_sz];
	if( buf )
	{
		memcpy(new_buf, buf, buf_offset+1);
		delete[] buf;
	}
	buf = new_buf;
	buf_sz = new_sz;
}

void CSerialStorage::AddString(const char* format, ...)
{
	va_list ap;
	va_start(ap, format);

	if( File )
		vfprintf(File, format, ap);
	else
	{
		CString str;
		str.FormatV(format, ap);

		int nAdded = str.GetLength();
		if( (buf_offset + nAdded + 2) > buf_sz )
		{
			ResizeBuf(nAdded+1);
		}
		memcpy(buf+buf_offset, (LPCSTR)str, nAdded);
		buf_offset += nAdded;
		buf[buf_offset] = '\n';
		buf_offset++;
	}

	va_end(ap);
}

LPCSTR CSerialStorage::GetResultString() 
{
	if( buf == NULL ) return "";
	buf[buf_offset] = '\0';
	return buf; 
}

char* CSerialStorage::ReadString()
{
	if( File )
	{
		while( !feof(File) )
		{
			fgets(buf, buf_sz, File);
			int len = strlen(buf);
			if( len > 1 )
			{
				buf[len-1] = '\0';
				break;
			}
		}
		if( feof(File) ) return NULL;
		return buf;
	}
	else
	{
		if( *str_ptr == '\0' ) return NULL;
		int len;
		while( (len = strcspn(str_ptr, "\r\n")) == 0 )
		{
			len = strspn(str_ptr, "\r\n");
			str_ptr += len;
			if( *str_ptr == '\0' ) return NULL;
		}
		char *ptr = str_ptr;
		str_ptr += len;
		if( *str_ptr != '\0' )
		{
			*str_ptr = '\0';
			str_ptr++;
		}

		return ptr;
	}
}