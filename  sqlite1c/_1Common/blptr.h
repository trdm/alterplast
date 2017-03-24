
#pragma once

template <class T>
class CBLPtr
{
public:
	typedef T _PtrClass;
	CBLPtr()
	{
		p = NULL;
	}
	CBLPtr(T* lp)
	{
		if ((p = lp) != NULL)
			p->IncrRef();
	}
	CBLPtr(const CBLPtr<T>& lp)
	{
		if ((p = lp.p) != NULL)
			p->IncrRef();
	}
	~CBLPtr()
	{
		if (p)
			p->DecrRef();
	}
	void DecrRef()
	{
		CBLContext* pTemp = p;
		if (pTemp)
		{
			p = NULL;
			pTemp->DecrRef();
		}
	}
	operator T*() const
	{
		return (T*)p;
	}
	T& operator*() const
	{
		ATLASSERT(p!=NULL);
		return *p;
	}
	//The assert on operator& usually indicates a bug.  If this is really
	//what is needed, however, take the address of the p member explicitly.
	T** operator&()
	{
		ATLASSERT(p==NULL);
		return &p;
	}
    
	T* operator->() const
	{
		ATLASSERT(p!=NULL);
		return p;
	}
    
	T* operator=(T* lp)
	{
	    if (lp != NULL)
		    lp->IncrRef();
	    if (p)
		    p->DecrRef();
	    p = lp;
	    return p;
	}
	T* operator=(const CComPtr<T>& lp)
	{
	    if (lp != NULL)
		    lp.p->IncrRef();
	    if (p)
		    p->DecrRef();
	    p = lp.p;
	    return p;
	}
	bool operator!() const
	{
		return (p == NULL);
	}
	bool operator<(T* pT) const
	{
		return p < pT;
	}
	bool operator==(T* pT) const
	{
		return p == pT;
	}
	void Attach(T* p2)
	{
		if (p)
			p->DecrRef();
		p = p2;
	}
	T* Detach()
	{
		T* pt = p;
		p = NULL;
		return pt;
	}
    void Create()
    {
        p = new T();
    };
    void Create(LPCSTR szTypeName)
    {
        p = CBLContext::CreateInstance(szTypeName); //no cast is special: this only for CBLContext instance
    };
	T* p;
};
