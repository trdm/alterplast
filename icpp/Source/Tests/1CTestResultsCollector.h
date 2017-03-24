// 1CTestResultsCollector.h: interface for the C1CTestResultsCollector class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_1CTESTRESULTSCOLLECTOR_H__C09D7176_4B83_4F0D_9E5F_567646DA512C__INCLUDED_)
#define AFX_1CTESTRESULTSCOLLECTOR_H__C09D7176_4B83_4F0D_9E5F_567646DA512C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sstream>
#include <cppunit/TestListener.h>

namespace Utils {
	class CUserInterface;
}

namespace Utils {

class C1CTestResultsCollector : public CppUnit::TestListener  
{
	int m_iNumTests;
	Utils::CUserInterface const &m_UI;
	std::stringstream m_StrStream;
public:
	void addFailure (const CppUnit::TestFailure &Failure);
	void startTest (CppUnit::Test *pTest);
	C1CTestResultsCollector();
	virtual ~C1CTestResultsCollector();
};

} // Utils

#endif // !defined(AFX_1CTESTRESULTSCOLLECTOR_H__C09D7176_4B83_4F0D_9E5F_567646DA512C__INCLUDED_)
