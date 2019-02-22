/////////////////////////////////////////////////////////////////////
// TestHarness.cpp - Process pool with comm which communicates     //
//						with client and child testers			   //
// ver 1.0                                                         //
// Ashrith C, CSE687 - Object Oriented Design, Fall 2018           //
/////////////////////////////////////////////////////////////////////
/*
Required files - Process.h, TestHarness.h

*/
#include "TestHarness.h"
#include "../Process/Process/Process.h"
#include <iostream>
#include <string>

TestHarness::TestHarness(EndPoint from, EndPoint to)
	: comm_(from, "recvr"), to_(to), from_(from)
{
	std::cout << "\n    Creating TestHarness comm";
	comm_.start();
	comm_.setSaveFilePath("../ServerSave");
	comm_.setSendFilePath("../ServerSend");
}
//----< deletes all child threads >--------------------------------

TestHarness::~TestHarness()
{
	std::cout << "\n    TestHarness Comm shutting down";
	std::cout << "\n    requestQ size = " + Utilities::Converter<size_t>::toString(requestQ_.size());
	std::cout << "\n    readyQ size   = " + Utilities::Converter<size_t>::toString(readyQ_.size());
}
//----< starts recvr, dspat, and child threads >-------------------

void TestHarness::start()
{


	std::cout << "\n    starting TestHarness receive thread listening on " << from_.toString();
	recvr = std::thread([&]() { recvMessages(); });
	recvr.detach();


	std::cout << "\n    starting TestHarness dispatch thread";
	dspat = std::thread([&]() { dispatchMessages(); });
	dspat.detach();

}
//----< sends stop message to itself >-----------------------------
/*
*  - this is a place-holder for a more intelligent shutdown
*/
void TestHarness::stop()
{
	Message stop;
	stop.command("TestHarnessStop");
	stop.to(from_);  // send to self
	stop.from(from_);
	comm_.postMessage(stop);
}
//----< waits for TestHarness to shutdown >------------------------
/*
*  - this is a place-holder for a more intellegent shutdown
*  - not used in this demo
*/
void TestHarness::wait()
{
	if (recvr.joinable())
		recvr.join();
	if (dspat.joinable())
		dspat.join();
}
//----< receive and post messages to appropriate queues >----------

void TestHarness::recvMessages()
{
	while (true)
	{
		Message msg = comm_.getMessage();
		std::cout << "\n    TestHnRcv received msg: " + msg.toString();
		if (msg.command() == "ready")
		{
			std::cout << "\n    TestHnRcv posting msg: " + msg.toString();
			readyQ_.enQ(msg);
		}
		else if (msg.command() == "testReq")
		{
			std::cout << "\n    TestHnRcv posting msg: " + msg.toString();
			requestQ_.enQ(msg);
		}
		else if (msg.command() == "exit")
		{
			std::cout << "The client " + msg.from().address + " has exited";
			break;
		}
		else
		{
			std::cout << "\n    TestHarnessR - invalid message";
		}
	}
}

void TestHarness::dispatchMessages()
{
	while (true)
	{
		Message trMsg = requestQ_.deQ();
		std::cout << "\n    TestHnDsp deQ msg: " + trMsg.command();

		Message rdyMsg = readyQ_.deQ();
		std::cout << "\n    TestHnDsp deQ msg: " + rdyMsg.command();

		trMsg.to(rdyMsg.from());  // send request to ready child
		std::cout << "\n    TestHnDsp sending msg: " + trMsg.toString();
		comm_.postMessage(trMsg);
	}
}


int main(int argc, char* argv[]) {
	std::cout << "The number of children to be spawned are " + *argv[1];
	int numChild = std::atoi(argv[1]);
	std::cout << "After conversion value of numChild is " + numChild;
	EndPoint ep1("localhost", 8082);
	EndPoint ep2("localhost", 8081);
	TestHarness testPool(ep1, ep2);
	testPool.start();
	Process childProc;
	int startPort = 8083;
	for (int i = 1; i <= numChild; i++) {
		childProc.title("Child Process " + std::to_string(i));
		std::string appPath = "../Debug/TestHarnessCore.exe";
		childProc.application(appPath);

		std::string cmdLine = std::to_string(startPort) + " 8082";
		childProc.commandLine(cmdLine);
		startPort = startPort + 1;
		std::cout << "\n  starting process: \"" << appPath << "\"";
		std::cout << "\n  with this cmdlne: \"" << cmdLine << "\"";
		childProc.create();
		CBP callback = []() { std::cout << "\n  --- child process exited with this message ---"; };
		childProc.setCallBackProcessing(callback);
		childProc.registerCallback();
	}
	std::cout << "Press any key to stop";
	getchar();
	testPool.wait();
}
