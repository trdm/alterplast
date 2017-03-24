#include "StdAfx.h"
#include "SystemInterface.h"

#include "../Utils1s/SystemInterface.h"

namespace Utils {

CSystemInterface::CSystemInterface(void)
{
}

CSystemInterface::~CSystemInterface(void)
{
}

CSystemInterface & Utils::CSystemInterface::Instance()
{
	static Utils1s::CSystemInterface Inst;
	return Inst;
}

} // namespace Utils