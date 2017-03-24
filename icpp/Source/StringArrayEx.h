// StringArrayEx.h: interface for the CStringArrayEx class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_)
#define AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

class CSimpleStringArray
{
public:
	CSimpleStringArray() {}
	CSimpleStringArray(LPCTSTR str) { FillSeparateString(str); }
	virtual ~CSimpleStringArray() {};

	void FillSeparateString(LPCTSTR str, LPCTSTR sep = ",", bool bErase = true);
	
	int GetSize() const
	{ return m_vector.size(); }
	
	INT_PTR GetUpperBound() const
	{ return GetSize()-1; }

	CString operator[](int nIndex) const
	{ return m_vector[nIndex]; }

	CString& operator[](int nIndex)
	{ return m_vector[nIndex]; }

	int Add(LPCTSTR newElement)
	{ 
		m_vector.push_back(newElement);
		return GetUpperBound();
	}

	int Add(const CString& newElement)
	{ 
		m_vector.push_back(newElement); 
		return GetUpperBound();
	}

	void RemoveAll()
	{
		m_vector.clear();
	}

private:
	std::vector<CString> m_vector;
};

typedef CSimpleStringArray CStringArrayEx;

#endif // !defined(AFX_STRINGARRAYEX_H__A4CD6DB6_92BA_4EA6_8B27_B5050EF1AA91__INCLUDED_)
