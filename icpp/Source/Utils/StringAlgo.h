#ifndef UTILS_STRING_ALGO_H
#define UTILS_STRING_ALGO_H

#pragma once

namespace Utils {

class UnicodeToStringConverter_t
{
	std::vector<char> m_Buffer;
public:
	UnicodeToStringConverter_t ();
	~UnicodeToStringConverter_t ();
	char const *Convert (wchar_t *pSource);
};

class StringToUnicodeConverter_t
{
	std::vector<wchar_t> m_Buffer;

	wchar_t const *MakeConversion (char const *pSource, std::size_t Length);
public:
	StringToUnicodeConverter_t ();
	~StringToUnicodeConverter_t ();
	wchar_t const *Convert (char const *pSource);
	wchar_t const *Convert (CString const &Source);
};

} // namespace Utils

#endif // UTILS_STRING_ALGO_H
