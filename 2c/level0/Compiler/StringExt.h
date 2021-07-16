// StringExt.h: interface for the CStringExt class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGEXT_H__05EC90AD_127B_42A1_9AE4_0CBEE3307689__INCLUDED_)
#define AFX_STRINGEXT_H__05EC90AD_127B_42A1_9AE4_0CBEE3307689__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


inline unsigned hash( const char* v, int M )
{
  static const unsigned a = 7; // 7 соответствуе 127, 5 - 31-у)
  unsigned h = 0;
  for ( ; *v; ++v )
    h = (h<<a) - h + *v;
  return h % M;
}

class CStringExt// : public CString
{
public:
	CString csNormal;
	CString csUpper;
	CStringExt()//:CString()
	{
	};
	CStringExt(const CString &Str)//:CString(Str)
	{
		csNormal=Str;
		csUpper=Str;
		csUpper.MakeUpper();
		csUpper.TrimRight();
		csUpper.TrimLeft();
	};
	operator DWORD ()
	{
		return hash(csUpper.GetBuffer(0),0xFFFFF);
	};

	operator CString()
	{
		return csNormal;
	}
};
inline BOOL CompareElements(CStringExt *pElement1,CStringExt *pElement2)
{
	return !pElement1->csUpper.Compare(pElement2->csUpper);
}
//BOOL CompareElements(CStringExt *pElement1,CStringExt *pElement2);

//unsigned hash( const char* v, int M );
/*
class CStringExt : public CString
{
public:
	CStringExt():CString()
	{
	};
	CStringExt(CString Str):CString(Str)
	{
	};
	operator DWORD ()
	{
		return hash(GetBuffer(0),0xFFFFF);
	};
	operator DWORD ()
	{
		CString Str=(*this);
		Str.MakeUpper();
		Str.TrimRight();
		Str.TrimLeft();
		return hash(Str.GetBuffer(0),0xFFFFF);
	};
};
*/

#endif // !defined(AFX_STRINGEXT_H__05EC90AD_127B_42A1_9AE4_0CBEE3307689__INCLUDED_)
