#ifndef TEST_ASSERT_H
#define TEST_ASSERT_H

namespace Utils {

void FailIf (bool Condition, char const *pCondition, char const *pFilename,
	int Line);

#ifdef CPPUNIT_ASSERT
#undef CPPUNIT_ASSERT
#endif

#define CPPUNIT_ASSERT(condition) ::Utils::FailIf (!(condition),\
	"Expression: " #condition, __FILE__, __LINE__);
} // namespace Utils

#endif // TEST_ASSERT_H
