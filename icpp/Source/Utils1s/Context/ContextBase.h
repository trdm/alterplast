#ifndef UTILS1S_CONTEXT_CONTEXT_BASE_H
#define UTILS1S_CONTEXT_CONTEXT_BASE_H

#pragma once

namespace Utils1s {
namespace Context {

class ContextImplBase_t;

class CContextBase
{
protected:
	virtual ContextImplBase_t const &GetImplBase () const = 0;
public:
	CContextBase ();
	virtual ~CContextBase ();

	virtual bool QueryInterface (UINT nID, void *pObject) = 0;
}; // class CContextBase

} // namespace Context
} // namespace Utils1s

#endif // UTILS1S_CONTEXT_CONTEXT_BASE_H
