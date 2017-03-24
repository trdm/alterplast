#include "StdAfx.h"
#include <cppunit/TestAssert.h>

namespace Utils {

void FailIf (bool Condition, char const *pCondition, char const *pFilename,
	int Line)
{
	if (Condition)
	CPPUNIT_NS::Asserter::failIf(true,
		CPPUNIT_NS::Message( "assertion failed", pCondition),
		CPPUNIT_NS::SourceLine (pFilename, Line));
}

}
