#ifndef UTILS1S_SYSTEN_INTERFACE_H
#define UTILS1S_SYSTEN_INTERFACE_H

#pragma once

#include "../Utils/SystemInterface.h"

namespace Utils1s {

class CSystemInterface : public Utils::CSystemInterface
{
public:
	virtual void RuntimeError (std::string const &strDescription)
	{
		CBLModule::RaiseExtRuntimeError (strDescription.c_str (), mmRedErr);
	}
};

} // namespace Utils1s

#endif // UTILS1S_SYSTEN_INTERFACE_H
