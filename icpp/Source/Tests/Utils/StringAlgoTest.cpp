#include "StdAfx.h"
#include "../CppUnitIncludes.h"
#include "StringAlgoTest.h"
#include "../../Utils/StringAlgo.h"

namespace UtilsTests {

using namespace Utils;

void StringAlgoTest::testUnicodeToStringConverter_t_Convert ()
{
	UnicodeToStringConverter_t Converter;

	CPPUNIT_ASSERT (std::string (Converter.Convert (L"11111")) == "11111");
	CPPUNIT_ASSERT (std::string (Converter.Convert (L"dddffff")) == "dddffff");
	CPPUNIT_ASSERT (std::string (Converter.Convert (L"222")) == "222");
} // StringAlgoTest::testBufferedUnicodeToStringConverter_t_Convert

void StringAlgoTest::testStringToUnicodeConverter_t_Convert ()
{
	StringToUnicodeConverter_t Converter;
	CPPUNIT_ASSERT (std::wstring (Converter.Convert ("11111")) == L"11111");
	CPPUNIT_ASSERT (std::wstring (Converter.Convert ("dddffff")) == L"dddffff");
	CPPUNIT_ASSERT (std::wstring (Converter.Convert ("222")) == L"222");
} // StringAlgoTest::testStringToUnicodeConverter_t_Convert

void StringAlgoTest::testStringToUnicodeConverter_t_Convert_CString ()
{
	StringToUnicodeConverter_t Converter;
	CPPUNIT_ASSERT (std::wstring (Converter.Convert (CString ("11111"))) == L"11111");
	CPPUNIT_ASSERT (std::wstring (Converter.Convert (CString ("dddffff"))) == L"dddffff");
	CPPUNIT_ASSERT (std::wstring (Converter.Convert (CString ("222"))) == L"222");
} // StringAlgoTest::testStringToUnicodeConverter_t_Convert_CString

} // namespace UtilsTests
