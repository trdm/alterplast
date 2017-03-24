//______________________________________________________________________________
//
// Design and implementation by kms, 2006
// Fast string comparator class (NLS version)
//______________________________________________________________________________
//
#ifndef compare_h_included
#define compare_h_included

class CCompare
{
public:
	virtual ~CCompare() {};
	BOOL Init(LCID lcid, UINT mask);

	CCompare()
	{
		if (!Init(LOCALE_USER_DEFAULT, 0xFFFFFFFF))
			RuntimeError("Cannot create fast comparator class!");
	};
	CCompare(UINT mask)
	{
		if (!Init(LOCALE_USER_DEFAULT, mask))
			RuntimeError("Cannot create fast comparator class!");
	};
	CCompare(LCID lcid, UINT mask)
	{
		if (!Init(lcid, mask))
			RuntimeError("Cannot create fast comparator class!");
	};

	int Compare(LPCSTR key1, LPCSTR key2);
	int CompareTrimSpc(LPCSTR key1, LPCSTR key2);
	int CompareIgnoreSpc(LPCSTR key1, LPCSTR key2);

	int Compare(LPCSTR key1, LPCSTR key2, size_t nLen);

	int operator()(LPCSTR key1, LPCSTR key2) { return Compare(key1, key2); }
private:
	UINT GetWeight(LCID lcid, char ch);
	UINT m_tbl[256];
};

// подготовленные компараторы
extern CCompare FastCompare;
extern CCompare FastCompareNoCase;

#endif
