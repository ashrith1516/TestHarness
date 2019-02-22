#pragma once
///////////////////////////////////////////////////////////////////////
// ReqsDemo.h - Demonstrating Project 2 requirements		         //
// Language:    C++, Visual Studio 2017                              //
// Application: CSE687 - Object Oriented Design						 //
// Author:      Ashrith Chandramouli								 //
///////////////////////////////////////////////////////////////////////
/*
*
* Required Files:
* ---------------
*	SingleLogger.h
*	TestExecutor.h
*	DllLoader.h
*	TestHandler.h
*	TestHelpers.h
*
* Maintenance History:
* --------------------
* v1.1 : Wrote the original set of requirements to the project
*/

#include <vector>
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <memory>
#include "TestHelpers.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include "../Utilities/CodeUtilities/CodeUtilities.h"
#include "../Utilities/DateTime/DateTime.h"
#include "../Process/Process/Process.h"

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
	TestReq1(const std::string& path) : path_(path) {}
	~TestReq1() { std::cout << "\n  deleting TestReq1"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #1 - Use C++";
		
		std::cout << "\n------------------------------------------------------------------------";
		
		std::cout << "\nDemonstrate by viewing files in project directory";
		
		Path path = path_ + "/Client";
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

class TestReq2a : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of C++ streams
	*  Test Procedure:
	*  - Displays lines of project source code that use iostream.
	*  - To insure that std streams are used for ALL output
	*    requires code inspection.
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq2a(const std::string& path) : path_(path) {}
	~TestReq2a() { std::cout << "\n  deleting TestReq2a"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #2a - use std::streams";
		
		std::cout << "\n------------------------------------------------------------------------";
		
		std::cout << "\nDemonstrate by viewing code";
		
		Path fileSpec = path_ + "/ReqsDemo/ReqsDemo.cpp";
		return TestHelpers::showFileLines(fileSpec, 20, 25);
	}

	DLL_DECL std::string name() {
		return "TestReq2a";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}

};

class TestReq2b : public ITest
{
	/*
	*  Test Description:
	*    Demonstrates use of C++ new and delete operators
	*  Test Procedure:
	*  - Displays lines of project source code that use new
	*    and delete
	*  - To insure no use of malloc or free requires code
	*    inspection or more advanced test tool with text
	*    finder capability.
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq2b(const std::string& path) : path_(path) {}
	~TestReq2b() { std::cout << "\n  deleting TestReq2b"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #2b - use new and delete";
		
		std::cout << "\n------------------------------------------------------------------------";
		
		std::cout << "\nDemonstrate by viewing project source code";
		
		Path fileSpec = path_ + "/CppCommWithFileXfer/MsgPassingComm/Comm.h";
		return TestHelpers::showFileLines(fileSpec, 383, 397);
	}
	DLL_DECL std::string name() {
		return "TestReq2b";
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
	TestReq3(const std::string& path) : path_(path) {}
	~TestReq3() { std::cout << "\n  deleting TestReq3"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #3 - usage of Process Pools";
		
		std::cout << "\n------------------------------------------------------------------------";
		
		std::cout << "\nDemonstrate by viewing project source code";
		
		Path fileSpec = path_ + "/ProcessPool/TestHarness.cpp";
		std::cout << "\nPart (a) Creating a specified number of Child Processes to execute the tests";
		return TestHelpers::showFileLines(fileSpec, 115, 131);

		std::cout << "\nPart (b) Posting Ready and TestRequest messages on the queue";
		std::cout << "\nPart (b) This also demonstrates that the messages are being received and enqueued from both client and child";
		return TestHelpers::showFileLines(fileSpec, 64, 89);

		std::cout << "\nPart (c) DeQueuing ready and TestRequest messages and also posting TestRequest to client";
		return TestHelpers::showFileLines(fileSpec, 91, 105);
		
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
	*    Demonstrates use of TestHarness and Tester Children
	*  Test Procedure:
	*  - Displays lines of project source code that use the TestHarness and Tester Children
	*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq4(const std::string& path) : path_(path) {}
	~TestReq4() { std::cout << "\n  deleting TestReq4"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #4 - Usage of TestHarness and Tester Children";
		
		std::cout << "\n------------------------------------------------------------------------";
		
		std::cout << "\nDemonstrate by viewing project source code";
		
		std::cout << "\nPart (a) Usage of TestHarness";
		std::cout << "\nThe demonstration of Requirement #3 shows the usage of TestHarness functionalities\n";
		std::cout << "\nPart (b) Usage of Tester Children";
		Path fileSpec = path_ + "/TestHarnessCore/Tester.cpp";
		return TestHelpers::showFileLines(fileSpec, 152, 193);
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
*    Demonstrates usage of Logger in tester children
*  Test Procedure:
*  - Displays lines of project source code that use the Logger in Tester Children
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq5(const std::string& path) : path_(path) {}
	~TestReq5() { std::cout << "\n  deleting TestReq5"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #5 - Usage of Logger in Tester Children";

		std::cout << "\n------------------------------------------------------------------------";
		Path fileSpec = path_ + "/TestHarnessCore/Tester.cpp";
		return TestHelpers::showFileLines(fileSpec, 23, 29);
		
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
*    Demonstrates HTTP style messages between client, server and tester children
*  Test Procedure:
*  - Executes the client, server and tester children as processes and demonstrates the communication
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq6(const std::string& path) : path_(path) {}
	~TestReq6() { std::cout << "\n  deleting TestReq6"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #6 - Communication in HTTP style messages";
		std::cout << "\n------------------------------------------------------------------------";
		std::cout << "\nThis demonstration will execute the Project 3 with the set up as described below";
		std::cout << "\n	CLIENT (ProcessName - Client	SendPath - ProjectDir/TestLibraries		SavePath - ProjectDir/SendLibraries)";
		std::cout << "\n		1. The client contains two test requests 'tr1' and 'tr2' which is sent to the server (Process Pool)";
		std::cout << "\n		2. The client also loads Dlls ito its SendPath and sends them to the Server using Message";
		std::cout << "\n		3. The client sends the test requests and dll files in the form of HTTP style string messages\n\n";
		std::cout << "\n	SERVER (ProcessName - ProcessPool	SendPath - ProjectDir/ServerSend		SavePath - ProjectDir/ServerSave)";
		std::cout << "\n		1. The server receives messages from client and tester children, enqueues and dequeues them as required";
		std::cout << "\n		2. The number of child processes to be created can be passed as the first argument to ProcessPool.exe";
		std::cout << "\n	CHILD (ProcessName - Child Process 1, Child Process 2	SendPath - ProjectDir/Tester1Send		SavePath - ProjectDir/ServerSave)";
		std::cout << "\n		1. Receives messages from Server in the form of a message object";
		std::cout << "\n		2. Converts message object into TestRequest using the fromString functionality and executes the test";
		std::cout << "\n		3. Sends the Logfile from child's SendPath to the client whose address is obtained from the Message";

		std::cout << "\n	";
		
		return true;
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
*    Demonstrates usage of Logger in tester children
*  Test Procedure:
*  - Displays lines of project source code that use the Logger in Tester Children
*/
private:
	const std::string path_;
	ILog* pRes_ = nullptr;
public:
	DLL_DECL void acceptHostedResource(ILog* pRes)
	{
		pRes_ = pRes;
	}
	TestReq7(const std::string& path) : path_(path) {}
	~TestReq7() { std::cout << "\n  deleting TestReq7"; }
	DLL_DECL bool test()
	{
		std::cout << "\nReq #7 - Usage of Logger in Tester Children";

		std::cout << "\n------------------------------------------------------------------------";
		Path fileSpec = path_ + "/TestHarnessCore/Tester.cpp";
		return TestHelpers::showFileLines(fileSpec, 23, 29);

	}
	DLL_DECL std::string name() {
		return "TestReq7";
	}

	DLL_DECL std::string author() {
		return "Ashrith";
	}
};
