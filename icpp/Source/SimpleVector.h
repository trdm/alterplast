#ifndef SIMPLE_VECTOR_H_INCLUDED
#define SIMPLE_VECTOR_H_INCLUDED

#include "SimpleCollection.h"

//////////////////////////////////////////////////////////////////////////
// CSimpleVector class (c) kms 2008
//////////////////////////////////////////////////////////////////////////

template <typename ITEM, typename ITEMREF = ITEM>
class CSimpleVector : public std::vector<ITEM>
{
public:
	typedef ITEM ITEM;
	typedef ITEMREF ITEMREF;

	virtual ~CSimpleVector() {}

private:
	void CheckIndex(size_t nIndex, size_t nSize) const
	{
		if (nIndex >= nSize)
			throw sc_exception("Неверное значение индекса!");
	}

public:
	size_t GetSize() const
	{
		return size();
	}

	ITEMREF GetAt(size_t nIndex) const
	{
		CheckIndex(nIndex, GetSize());
		return operator[](nIndex);
	}

	void SetAt(size_t nIndex, ITEMREF val)
	{
		CheckIndex(nIndex, GetSize());
		operator[](nIndex) = val;
	}

	void Add(ITEMREF val)
	{
		push_back(val);
	}

	void Remove(size_t nIndex)
	{
		CheckIndex(nIndex, GetSize());
		erase(begin() + nIndex);
	}

	void Clear()
	{
		clear();
	}

	void Insert(size_t nIndex, ITEMREF val)
	{
		if (nIndex == GetSize())
			Add(val);
		else
		{
			CheckIndex(nIndex, GetSize());
			insert(begin() + nIndex, val);
		}
	}

	void Move(size_t nIndex, int nOffset)
	{
		if (nOffset)
		{
			CheckIndex(nIndex, GetSize());
			int nInsertPos = (int)nIndex + nOffset;
			if (nInsertPos < 0 || nInsertPos >= (int)GetSize())
				throw sc_exception("Неверное смещение!");

			iterator pb = begin();
			if (nOffset > 0)
				std::rotate(pb + nIndex, pb + nIndex + 1, pb + nInsertPos + 1);
			else
				std::rotate(pb + nInsertPos, pb + nIndex, pb + nIndex + 1);
		}
	}
};
#endif
