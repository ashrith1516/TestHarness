///////////////////////////////////////////////////////////////////////
// TestHelpers.cpp - Tests for functionalities of TestHelpers.h		 //
// Language:    C++, Visual Studio 2017                              //
// Application: Most Projects, CSE687 - Object Oriented Design       //
// Author:      Ashrith Chandramouli								 //
///////////////////////////////////////////////////////////////////////

/*
* Required Files :
*-------------- -
*	TestHelpers.h
*
* Maintenance History :
*--------------------
* v1.1 : Initial set of tests for TestHelper.h
*/

#include "TestHelpers.h"

#ifdef TESTHELPERS
int main() {
	std::cout << "Testing for the showDirContents functionality\n";
	Path path = "../FileUtilities";
	bool result1 = TestHelpers::showDirContents(path, "");
	std::cout << "Testing for the showFileLines functionality\n";
	Path fileSpec = "../ReqsDemo/ReqsDemo.cpp";
	bool result2 = TestHelpers::showFileLines(fileSpec, 1, 5);
	return result1 && result2;
}
#endif