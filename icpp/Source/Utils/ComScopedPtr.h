#ifndef UTILS_COM_SCOPED_PTR_H
#define UTILS_COM_SCOPED_PTR_H

#pragma once

#include <stdexcept> // runtime_error

namespace Utils {

template<class T> class ComScopedPtr_t : private boost::noncopyable
{
	T *m_p;
	CComPtr<IMalloc> m_pMalloc;

public:
	explicit ComScopedPtr_t (T *p = 0) : m_p (p)
	{
		if (FAILED (::CoGetMalloc (1, &m_pMalloc)))
			throw std::runtime_error ("CoGetMalloc failed");
	}

	~ComScopedPtr_t()
	{
		if (!m_p) return;
		m_pMalloc->Free (m_p);
	}

	T &operator * () const {ASSERT (m_p); return *m_p;}

	T *get () const {return m_p;}

	T *operator-> () const {ASSERT (m_p); return m_p;}

	T **operator & () {return &m_p;}

	T &operator [] (std::size_t Index) {ASSERT (m_p); return *(m_p + Index);}

	T const &operator [] (std::size_t Index) const
	{ASSERT (m_p); return *(m_p + Index);}
};

} // namespace Utils

#endif // UTILS_COM_SCOPED_PTR_H
