#ifndef UTILS_TESTS_STRING_ALGO_TEST_H
#define UTILS_TESTS_STRING_ALGO_TEST_H

#pragma once

namespace UtilsTests {

class StringAlgoTest : public CPPUNIT_NS::TestFixture
{
	CPPUNIT_TEST_SUITE (StringAlgoTest);
	CPPUNIT_TEST (testUnicodeToStringConverter_t_Convert);
	CPPUNIT_TEST (testStringToUnicodeConverter_t_Convert);
	CPPUNIT_TEST (testStringToUnicodeConverter_t_Convert_CString);
	CPPUNIT_TEST_SUITE_END ();

	void testUnicodeToStringConverter_t_Convert ();
	void testStringToUnicodeConverter_t_Convert ();
	void testStringToUnicodeConverter_t_Convert_CString ();
}; // class StringAlgoTest

} // namespace UtilsTests

#endif // UTILS_TESTS_STRING_ALGO_TEST_H
