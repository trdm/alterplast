
#pragma once

#include <stdio.h>

class CSerialStorage
{
private:
	FILE* File;
	CString String;
	int buf_sz;
	char* buf;
	int buf_offset;
	char* str_ptr;

	void ResizeBuf(int nAdded);

public:
	enum {modeRead, modeWrite} TMode;

public:
	//constructor for writing to string
	CSerialStorage();
	//constructor for reading from string
	CSerialStorage(char const* str);
	//constructor for writing/reading to/from file
	CSerialStorage(FILE* f);

	~CSerialStorage();

	void AddString(const char* format, ...);
	LPCSTR GetResultString();

	char* ReadString();
};