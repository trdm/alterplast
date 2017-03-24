#ifndef UTILS_SYSTEM_INTERFACE_H
#define UTILS_SYSTEM_INTERFACE_H

#pragma once

namespace Utils {

class CSystemInterface
{
public:
	CSystemInterface(void);
	virtual ~CSystemInterface(void);

	static CSystemInterface &Instance ();

	virtual void RuntimeError (std::string const &) = 0;
};

} // namespace Utils

#endif // UTILS_SYSTEM_INTERFACE_H
