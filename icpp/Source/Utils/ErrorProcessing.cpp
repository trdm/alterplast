#include "StdAfx.h"
#include "SystemInterface.h"

namespace Utils {

namespace {

std::string ErrorDescription;

class ErrDescrInitializer_t {
public:
	ErrDescrInitializer_t () {ErrorDescription.reserve (1024);}
};

ErrDescrInitializer_t ErrDescrInitializer;

}

void ProcessError (char const *pText, std::exception const &Reason)
{
	ErrorDescription = pText;
	ErrorDescription += ": ";
	ErrorDescription += Reason.what ();
	Utils::CSystemInterface::Instance ().RuntimeError (ErrorDescription);
} // ProcessError

void ProcessMemoryError (char const *pText)
{
	ErrorDescription = pText;
	ErrorDescription += ": ";
	ErrorDescription += "Ќедостаточно пам€ти дл€ выполнени€ операции";
	Utils::CSystemInterface::Instance ().RuntimeError (ErrorDescription);
} // ProcessMemoryError

void ProcessErrors (char const *pText)
{
	try {
		throw;
	}
	catch (std::bad_alloc const &) {
		ProcessMemoryError (pText);
	}
	catch (std::exception const &Error) {
		ProcessError (pText, Error);
	}
	catch (CMemoryException *) {
		ProcessMemoryError (pText);
	}
} // ProcessErrors

} // namespace Utils
