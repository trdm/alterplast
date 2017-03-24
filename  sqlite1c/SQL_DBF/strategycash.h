// strategycash.h
#pragma once

class StrategyCash
{
public:
	~StrategyCash();
	BOOL getFromCash(sqlite3_index_info* pIdx, CString& cashKey);
	void addToCash(const CString& cashKey, sqlite3_index_info* pIdx);
protected:
	struct cash_entry
	{
		CString cashKey, idxStr;
		int idxNum, cost;
		BOOL bOrderBy;
		CDWordArray args;
	};
	CPtrList cash;
};
