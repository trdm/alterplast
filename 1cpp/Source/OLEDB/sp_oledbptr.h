#if !defined(AFX_OLEDBMEMPTR_H__81BCC2A9_66D7_46B2_94B7_C70D544EB7FD__INCLUDED_)
#define AFX_OLEDBMEMPTR_H__81BCC2A9_66D7_46B2_94B7_C70D544EB7FD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

template <class T> class COLEDBMemPtr
{
public:
	COLEDBMemPtr() : p(NULL) {}
	
	COLEDBMemPtr(const int nSize)
	{
		try
		{
			p = new T[nSize];
		}
		catch (...)
		{
			p = NULL;
		}
	}
	
	~COLEDBMemPtr()
	{
		if(p)
			delete[] p;

		p = NULL;
	}

	void Alloc(const int nSize)
	{
		try
		{
			p = new T[nSize];
		}
		catch (...)
		{
			p = NULL;
		}
	}

	void Release()
	{
		if(p)
			delete[] p;

		p = NULL;
	}
	
	operator T*() const
	{
		return (T*)p;
	}
	
	T& operator*() const
	{
		return *p;
	}

	T** operator&()
	{
		return &p;
	}

	bool operator!() const
	{
		return (p == NULL);
	}

public:
	T* p;
};

#endif // !defined(AFX_OLEDBMEMPTR_H__81BCC2A9_66D7_46B2_94B7_C70D544EB7FD__INCLUDED_)