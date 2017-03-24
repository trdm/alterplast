#include "StdAfx.h"
#include "StringAlgo.h"

namespace Utils {

UnicodeToStringConverter_t::UnicodeToStringConverter_t ()
{
}

UnicodeToStringConverter_t::~UnicodeToStringConverter_t ()
{
}

char const *UnicodeToStringConverter_t::Convert (wchar_t *pSource)
{
	std::size_t Length = std::wcslen (pSource);
	if (!Length) return "";

	if (Length + 1 > m_Buffer.size ()) m_Buffer.resize (Length + 1);

	::WideCharToMultiByte (CP_ACP, 0, pSource, Length, &m_Buffer [0], Length + 1,
		NULL, NULL);

	m_Buffer [Length] = 0;
	return &m_Buffer [0];
} // UnicodeToStringConverter_t::Convert


StringToUnicodeConverter_t::StringToUnicodeConverter_t ()
{
}

StringToUnicodeConverter_t::~StringToUnicodeConverter_t ()
{
}

wchar_t const *StringToUnicodeConverter_t::Convert (char const *pSource)
{
	return MakeConversion (pSource, std::strlen (pSource));
} // StringToUnicodeConverter_t::Convert

wchar_t const *StringToUnicodeConverter_t::Convert (CString const &Source)
{
	return MakeConversion ((LPCSTR)Source, Source.GetLength ());
} // StringToUnicodeConverter_t::Convert

wchar_t const *StringToUnicodeConverter_t::MakeConversion (
	char const *pSource, std::size_t Length)
{
	if (!Length) return L"";

	if (Length + 1 > m_Buffer.size ()) m_Buffer.resize (Length + 1);

	::MultiByteToWideChar (CP_ACP, 0, pSource, Length, &m_Buffer [0], Length + 1);
	m_Buffer [Length] = 0;

	return &m_Buffer [0];
} // StringToUnicodeConverter_t::MakeConversion

} // namespace Utils
