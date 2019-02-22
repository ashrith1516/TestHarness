#pragma once
///////////////////////////////////////////////////////////////////////
// ReqsDemo.cpp - Demonstrating Project 4 requirements		         //
// Language:    C++, Visual Studio 2017                              //
// Application: CSE687 - Object Oriented Design						 //
// Author:      Ashrith Chandramouli								 //
///////////////////////////////////////////////////////////////////////
/*
*
* Required Files:
* ---------------
*	ITests.h
*	CodeUtilities.h
*	DateTime.h
*
* Maintenance History:
* --------------------
* v1.1 : Wrote the original set of requirements to the project
*/
#define IN_DLL
#define ARG_SIZE 256


#include "TestHelpers.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Utilities/DateTime/DateTime.h"
#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>

std::ostream& out = std::cout;
using Path = std::string;
using Message = std::string;
using Line = size_t;
using File = std::string;
using Files = std::vector<File>;
using Pattern = std::string;
using Patterns = std::vector<Pattern>;

class TestReq1 : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of C++
	*  Test Procedure:
	*  - Displays C++ files in project directory
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq1() : path_("..") {}
	~TestReq1() { pRes_->write("\n  deleting TestReq1"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #1 - Use C++");

		pRes_->write("\n------------------------------------------------------------------------");

		pRes_->write("\nDemonstrate by viewing files in project directory");

		Path path = path_ + "/CommLibWrapper";
		Patterns pats = { "*.h", "*.cpp" };

		return TestHelpers::showDirContents(path, "");
	}
	DLL_DECL std::string name() {
		return "TestReq1";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}

};

class TestReq2 : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of WPF
	*  Test Procedure:
	*  - Displays XAML files that use WPF.
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq2() : path_("..") {}
	~TestReq2() { pRes_->write("\n  deleting TestReq2"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #2 - use of WPF");

		pRes_->write("\n------------------------------------------------------------------------");

		pRes_->write("\nDemonstrate by viewing xaml and cs file code lines");
		pRes_->write("\nThe GUI can also be viewed while demonstrating the whole project");

		Path fileSpec = path_ + "/PrototypeGui-OOD-Pr4/PrototypeGui-OOD-Pr4/MainWindow.xaml";
		bool result1 = TestHelpers::showFileLines(fileSpec, 1, 7);
		fileSpec = path_ + "/PrototypeGui-OOD-Pr4/PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		bool result2 = TestHelpers::showFileLines(fileSpec, 34, 51);
		return result1 && result2;
	}

	DLL_DECL std::string name() {
		return "TestReq2";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}

};

class TestReq3 : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of Process Pool
	*  Test Procedure:
	*  - Displays lines of project source code that use Process Pool and Blocking queues
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq3() : path_("..") {}
	~TestReq3() { pRes_->write("\n  deleting TestReq3"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #3 - Assembling working parts from Project#1, #2 and #3");

		pRes_->write("\n------------------------------------------------------------------------");

		pRes_->write("\nDemonstrate by viewing project source code");
		pRes_->write("\nTest Executive from Project 1");
		pRes_->write("\n-------------------------------");
		Path fileSpec = path_ + "/Utilities/TestUtilities/TestUtilities.h";
		bool result1 = TestHelpers::showFileLines(fileSpec, 114, 129);

		pRes_->write("\nDllLoader from Project 2");
		pRes_->write("\n-------------------------------");
		fileSpec = path_ + "/Dll_Loader_With_Hosted_Resource/DllLoader/DllLoader.h";
		bool result2 = TestHelpers::showFileLines(fileSpec, 52, 64);

		pRes_->write("\nComm from Project 3");
		pRes_->write("\n-------------------------------");
		fileSpec = path_ + "/CppCommWithFileXfer/MsgPassingComm/Comm.h";
		bool result3 = TestHelpers::showFileLines(fileSpec, 132, 152);

		pRes_->write("\nThe combined working will be shown in the demonstration");
		return result1 && result2 && result3;
	}
	DLL_DECL std::string name() {
		return "TestReq3";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq4 : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of GUI
	*  Test Procedure:
	*  - Show lines of code from XAML files
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq4() : path_("..") {}
	~TestReq4() { pRes_->write("\n  deleting TestReq4"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #4 - Usage of GUI");

		pRes_->write("\n------------------------------------------------------------------------");

		pRes_->write("\nDemonstrate by viewing project source code");

		pRes_->write("\nXAML files have been used in the application which is a part of the GUI");
		pRes_->write("\nThe GUI can also be viewed during the demonstration");
		Path fileSpec = path_ + "/PrototypeGui-OOD-Pr4/PrototypeGui-OOD-Pr4/MainWindow.xaml";
		return TestHelpers::showFileLines(fileSpec, 1, 8);
		
	}
	DLL_DECL std::string name() {
		return "TestReq4";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq5 : public ITest
{
	/*
*  Test Description:
*    Demonstrates usage of Message Designs
*  Test Procedure:
*  - Displays lines of project source code that perform tasks based on message
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq5() : path_("..") {}
	~TestReq5() { pRes_->write("\n  deleting TestReq5"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #5 - Usage of Message Design");

		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nBased on the message received, the process pool performs the corresponding task");
		pRes_->write("\nCreation of messages on client's side is displayed in Req#6");
		Path fileSpec = path_ + "/ProcessPool/TestHarness.cpp";
		return TestHelpers::showFileLines(fileSpec, 74, 100);

	}
	DLL_DECL std::string name() {
		return "TestReq5";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}


};

class TestReq6 : public ITest
{
	/*
*  Test Description:
*    Demonstrates sending of TestRequest from client to server
*  Test Procedure:
*  - Displays lines of code where the test request is sent
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq6() : path_("..") {}
	~TestReq6() { pRes_->write("\n  deleting TestReq6"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #6 - TestRequest between Client and Server");
		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nThe test request is created as a string on the client's side and sent as a message to the server");
		pRes_->write("\nThe below lines of code display the creation of test request message");
		Path fileSpec = path_ + "/PrototypeGui-OOD-Pr4/PrototypeGui-OOD-Pr4/MainWindow.xaml.cs";
		return TestHelpers::showFileLines(fileSpec, 363, 386);
	}
	DLL_DECL std::string name() {
		return "TestReq6";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq7 : public ITest
{
	/*
*  Test Description:
*    Demonstrates execution of sequence of TestRequests
*  Test Procedure:
*  - Displays lines of code for enqueing and dequeing test requests which are executed by child process
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq7() : path_("..") {}
	~TestReq7() { pRes_->write("\n  deleting TestReq7"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #7 - Execution of test requests");
		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nThe test requests are enqueued by the process pool and executed once a child process is available");
		pRes_->write("\nThis is demonstrated in the enQ and deQ function based on the message command as shown below");
		pRes_->write("\nEnqueueing");
		Path fileSpec = path_ + "/ProcessPool/TestHarness.cpp";
		bool result1 = TestHelpers::showFileLines(fileSpec, 85, 89);
		pRes_->write("\n\nDequeueing");
		fileSpec = path_ + "/ProcessPool/TestHarness.cpp";
		bool result2 = TestHelpers::showFileLines(fileSpec, 102, 116);
		pRes_->write("\n\nTest Execution");
		fileSpec = path_ + "/TestHarnessCore/Tester.cpp";
		bool result3 = TestHelpers::showFileLines(fileSpec, 175, 192);

		return result1 && result2 && result3;
	}
	DLL_DECL std::string name() {
		return "TestReq7";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq8 : public ITest
{
	/*
*  Test Description:
*    Demonstrates sending of results from Tester child to Client
*  Test Procedure:
*  - Displays lines of code where the message is sent
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq8() : path_("..") {}
	~TestReq8() { pRes_->write("\n  deleting TestReq8"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #8 - Results message between Client and Server");
		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nThe message along with the file is sent to the client whose address is obtained from the Process Pool");
		Path fileSpec = path_ + "/TestHarnessCore/Tester.cpp";
		return TestHelpers::showFileLines(fileSpec, 148, 158);
	}
	DLL_DECL std::string name() {
		return "TestReq8";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq9 : public ITest
{
	/*
*  Test Description:
*    Demonstrates project operation for two or more concurrent clients
*  Test Procedure:
*  - Displays message to notify the grader
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq9() : path_("..") {}
	~TestReq9() { pRes_->write("\n  deleting TestReq9"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #9 - Operation of two or more concurrent clients");
		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nThe demonstration will show two client windows which can be simultaneously operated. If a client is run as administrator and the window is closed, it will also close the Process Pool/Server.");
		pRes_->write("\nThe client ports should not be anything after 8081. The number of clients and port numbers can be changed in the run.bat file.\nThe initial wait time for each Test Execution is set to 10 seconds"); 
		return true;
	}
	DLL_DECL std::string name() {
		return "TestReq9";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestReq10 : public ITest
{
	/*
*  Test Description:
*    Demonstrates project operation for two or more concurrent clients
*  Test Procedure:
*  - Displays message to notify the grader
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq10() : path_("..") {}
	~TestReq10() { pRes_->write("\n  deleting TestReq10"); }
	DLL_DECL bool test()
	{
		pRes_->write("\nReq #10 - Automated test unit suite");
		pRes_->write("\n------------------------------------------------------------------------");
		pRes_->write("\nAll the above tests have been wrapped into a Dll which is being executed by the test child");
		pRes_->write("\nAll the logs for tests can be found in ProjectDir/SaveLibraries");
		Path fileSpec = path_ + "/ReqsDemo/ReqsDemo.cpp";
		return TestHelpers::showFileLines(fileSpec, 454, 468);
	}
	DLL_DECL std::string name() {
		return "TestReq10";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};

class TestCollection : public ITests {
	DLL_DECL std::vector<ITest*> tests();
};

// this is where each test class is instantiated and added to collection
DLL_DECL std::vector<ITest*> TestCollection::tests()
{
	std::vector<ITest*> tests_vec;
	tests_vec.push_back(new TestReq1);
	tests_vec.push_back(new TestReq2);
	tests_vec.push_back(new TestReq3);
	tests_vec.push_back(new TestReq4);
	tests_vec.push_back(new TestReq5);
	tests_vec.push_back(new TestReq6);
	tests_vec.push_back(new TestReq7);
	tests_vec.push_back(new TestReq8);
	tests_vec.push_back(new TestReq9);
	tests_vec.push_back(new TestReq10);
	return tests_vec;
}

//////////////////////////////////////////////////////////////////////////////
// this section is where each Test Driver DLL completely differs from other
// test drivers. Although the same name can be used in all TestDrivers, the 
// actual instance of TestCollection is different in the way it returns 
// different collection of ITest*.

DLL_DECL ITests* get_ITests()
{
	return new TestCollection;
}
