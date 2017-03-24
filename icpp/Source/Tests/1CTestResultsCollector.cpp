// 1CTestResultsCollector.cpp: implementation of the C1CTestResultsCollector class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include <boost/algorithm/string.hpp>
#include <cppunit/Test.h>
#include <cppunit/TestFailure.h>
#include <cppunit/SourceLine.h>
#include <cppunit/Exception.h>
#include <cppunit/Message.h>
//#include <cppunit/TestAssert.h>
//#include <cppunit/config/SourcePrefix.h>
#include "1CTestResultsCollector.h"
#include "../Utils/UserInterface.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

namespace Utils {

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

C1CTestResultsCollector::C1CTestResultsCollector() :
m_UI (Utils::CUserInterface::Instance ()), m_iNumTests (0)
{
}

C1CTestResultsCollector::~C1CTestResultsCollector()
{
	/*m_StrStream.str ("");
	m_StrStream << "Выполнено тестов: " << m_iNumTests;
	m_UI.AddMessage (m_StrStream.str ());*/
}

void C1CTestResultsCollector::startTest(CppUnit::Test *pTest)
{
	m_UI.SetStatusLine (std::string ("Выполняется тест: ") + pTest->getName ());
	m_iNumTests++;
}

void C1CTestResultsCollector::addFailure(const CppUnit::TestFailure &Failure)
{
	std::string sFailedTest = Failure.failedTestName ();
	m_UI.AddMessage (std::string ("Тест провален: ") + sFailedTest);

	CppUnit::Exception *pException = Failure.thrownException ();
	CppUnit::SourceLine Line = pException->sourceLine ();

	m_StrStream.str ("");
	m_StrStream << Line.fileName () << " (" << Line.lineNumber () << ")";
	std::string strMessage = m_StrStream.str ();

	m_UI.AddMessage (strMessage);

	CppUnit::Message ErrMessage = pException->message ();
	m_UI.AddMessage (ErrMessage.shortDescription ());

	strMessage = ErrMessage.details ();
	boost::algorithm::replace_all (strMessage, "\n", "\r\n");

	m_UI.AddMessage (strMessage);
}

} // Utils