#ifndef UTILS_ERROR_PROCESSING_H
#define UTILS_ERROR_PROCESSING_H

#pragma once

namespace Utils {

void ProcessError (char const *pText, std::exception const &Reason);
void ProcessMemoryError (char const *pText);
void ProcessErrors (char const *pText);

} // namespace Utils

#endif // UTILS_ERROR_PROCESSING_H
