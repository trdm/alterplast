// Tests.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include <afxdllx.h>
#include "../Utils/UserInterface.h"
#include <cppunit/extensions/TestFactoryRegistry.h>
#include <cppunit/ui/text/TestRunner.h>
#include <cppunit/TestResult.h>
#include "1CTestResultsCollector.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


static AFX_EXTENSION_MODULE TestsDLL = { NULL, NULL };

extern "C" int APIENTRY
DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID lpReserved)
{
	// Remove this if you use lpReserved
	UNREFERENCED_PARAMETER(lpReserved);

	if (dwReason == DLL_PROCESS_ATTACH)
	{
		TRACE0("Tests.DLL Initializing!\n");
		
		// Extension DLL one-time initialization
		if (!AfxInitExtensionModule(TestsDLL, hInstance))
			return 0;

		// Insert this DLL into the resource chain
		// NOTE: If this Extension DLL is being implicitly linked to by
		//  an MFC Regular DLL (such as an ActiveX Control)
		//  instead of an MFC application, then you will want to
		//  remove this line from DllMain and put it in a separate
		//  function exported from this Extension DLL.  The Regular DLL
		//  that uses this Extension DLL should then explicitly call that
		//  function to initialize this Extension DLL.  Otherwise,
		//  the CDynLinkLibrary object will not be attached to the
		//  Regular DLL's resource chain, and serious problems will
		//  result.

		new CDynLinkLibrary(TestsDLL);

		Utils::CUserInterface::Instance ().AddMessage ("Начало тестирования...");
		CPPUNIT_NS::Test *suite = CPPUNIT_NS::TestFactoryRegistry::getRegistry().makeTest();
		CPPUNIT_NS::TextUi::TestRunner runner;
		runner.addTest( suite );

		CppUnit::TestResult Controller;
		Utils::C1CTestResultsCollector ResultsCollector;
		Controller.addListener (&ResultsCollector);
		//runner.setOutputter( new CPPUNIT_NS::CompilerOutputter( &runner.result(),
														   //CPPUNIT_NS::stdCOut() ) );
		runner.run (Controller);
		Utils::CUserInterface::Instance ().AddMessage ("Тестирование завершено");

	}
	else if (dwReason == DLL_PROCESS_DETACH)
	{
		TRACE0("Tests.DLL Terminating!\n");

		// Terminate the library before destructors are called
		AfxTermExtensionModule(TestsDLL);
	}
	return 1;   // ok
}
