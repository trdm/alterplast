#include "StdAfx.h"
#include "../Utils1s/UserInterface.h"

namespace Utils {

CUserInterface &CUserInterface::Instance ()
{
	static Utils1s::CUserInterface Inst;
	return Inst;
}

} // namespace Utils
