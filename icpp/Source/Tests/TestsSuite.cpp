#include "stdafx.h"
#include "CppUnitIncludes.h"
#include "Utils/StringAlgoTest.h"

#define CPPUNITEXT_REGISTRY_ADD_TO_DEFAULT( which )                         \
  static CPPUNIT_NS::AutoRegisterRegistry                                \
             CPPUNIT_MAKE_UNIQUE_NAME( autoRegisterRegistryExt__ )( which )

#define CPPUNITEXT_REGISTER_SUITE(Namespace,ClassName) \
	inline std::string Namespace##_##ClassName##SuiteName () \
{\
	return std::string (#Namespace) + "::" + #ClassName;\
}\
	CPPUNITEXT_REGISTRY_ADD_TO_DEFAULT (Namespace##_##ClassName##SuiteName ());\
	CPPUNIT_TEST_SUITE_NAMED_REGISTRATION (ClassName,\
                                       Namespace##_##ClassName##SuiteName ());

namespace Tests {

} // namespace Tests

namespace UtilsTests {

CPPUNITEXT_REGISTER_SUITE (UtilsTests, StringAlgoTest);

} // namespace UtilsTests
