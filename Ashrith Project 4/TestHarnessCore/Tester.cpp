/////////////////////////////////////////////////////////////////////
// Tester.cpp - Prototype for Test Harness child Tester            //
// ver 2.0														   //
// Author : Ashrith Chandramouli								   //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////
/*
Required files - ITests.h, TestUtilities.h, FileSystem.h, FileUtilities.h, StringUtilities.h

Changes :
v2.0 : Added comm to the Tester
v1.0 : Initial working version
*/

#include "Tester.h"
#include "../Dll_Loader_With_Hosted_Resource/ITests.h"
#include "../Utilities/TestUtilities/TestUtilities.h"
#include "../Utilities/FileSystem/FileSystem.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../Utilities/StringUtilities/StringUtilities.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <memory>
#include <vector>

namespace Testing
{
  //----< initialize paths and logger >------------------------------

  Tester::Tester()
  {
    requestPath("../TestRequests");
    libPath("../TestLibraries");
    pLog(Utilities::Logger<0, Utilities::Lock>::getInstance());
    pLog()->setTerminator("");
  }
  //----< deallocate logger >----------------------------------------

  Tester::~Tester()
  {
    delete pLog();
  }
  //----< load Test Requests >-----------------------------------------

  void Tester::loadTestRequests()
  {
    std::vector<std::string> files = FileSystem::Directory::getFiles(requestPath());
    std::ifstream in;
    for (auto file : files)
    {
      if (!Utilities::openReadFile(requestPath() + "\\" + file, in))
        continue;
      std::ostringstream sstr;
      sstr << in.rdbuf();
      TestRequest tr = TestRequest::fromString(sstr.str());
      requests.valueRef().push_back(tr);
    }
  }
  //----< load libraries >---------------------------------------------
  /*
  * - Loads all libraries specified in a TestRequest from libPath.
  * - Uses DllLoader member, dllLoader_, to load dlls and extract
  *   their tests.
  */
  void Tester::loadLibraries(TestRequest& tr,const std::string& loadPath)
  {
    std::string path = FileSystem::Path::getFullFileSpec(loadPath);
	std::cout << "\n  loading from \"" + path + "\"";

    std::vector<std::string> files;
    for (Dll dll : tr.request.valueRef())
    {
		std::cout << dll;
      files.push_back(dll);
    }

    for (auto file : files)
    {
      pLog()->write("\n    " + file);
      std::string fileSpecDll = path + "\\" + file;
      dllLoader_.loadAndExtract(fileSpecDll);

    }
    pLog()->write("\n");
  }
  //----< executes tests >--------------------------
  /*
  * - Executes tests held by DllLoader instance, 
  *   using TestUtilities infrastructure.
  */
  bool Tester::doTests()
  {
    using sPtr = std::shared_ptr<ITest>;
    Utilities::TestExecutive te;
	
    for (ITest* test : dllLoader_.tests()) {
      // Host passes logger resource pointer to test
      // - test will use logger via this pointer
      test->acceptHostedResource(pLog.valueRef());
      sPtr pTest(test);
      te.registerTest(pTest, pTest->name());
    }
	pLog()->write("\nPerforming the tests");
    bool result = te.doTests();
	pLog()->write("\nCompleted the tests");
    return result;
  }
}
ChildTester::ChildTester(EndPoint from, EndPoint to)
	: comm_(from, "tester"), from_(from), to_(to) {
	comm_.start();
	comm_.setSaveFilePath("../ServerSave");
	comm_.setSendFilePath("../Tester1Send");
	MsgPassingCommunication::Message ready;
	ready.from(from_);
	ready.to(to_);
	ready.command("ready");
	std::cout << "\n      Tester sending msg: " + ready.command();
	comm_.postMessage(ready);
}

//----< stop comm >------------------------------------------------

ChildTester::~ChildTester()
{
	std::cout << "\n      ChildTester destroyed";
	comm_.stop();
}
//----< start comm and testing thread >----------------------------

void ChildTester::start()
{
	std::cout << "\n      starting ChildTester thread listening on " + from_.toString();
	recv = std::thread([&]() { doTests(); });
}

void ChildTester::wait()
{
	if (recv.joinable())
		recv.join();
}
//----< deQ request message and do mock test >---------------------
/*
*  - sends ready message when starting and when done with each test
*/

void ChildTester::sendLog(MsgPassingCommunication::EndPoint clnt,const std::string& filename)
{
	MsgPassingCommunication::Message logMsg;
	logMsg.to(clnt);
	logMsg.from(from_);
	logMsg.name("Logfile " + filename);
	logMsg.value("sendingFile");   // required to send file
	logMsg.file(filename);         // defines file to send from sendPath
	std::cout << "\n  " + comm_.name() + " posting: " + logMsg.toString();
	comm_.postMessage(logMsg);
}

void ChildTester::doTests()
{
	while (true)
	{
		MsgPassingCommunication::Message msg = comm_.getMessage();
		std::cout << "\n      Tester " + from_.toString() + " received msg: " + msg.toString();
		
		if (msg.command() == "done")
		{
			msg.from(from_);
			comm_.postMessage(msg);
			std::cout << "\n      ChildTester shutting down";
			std::this_thread::sleep_for(std::chrono::milliseconds(1000));
			break;
		}
		else if (msg.command() == "testReq") {
			std::cout << "\n      --- testing --- ";
			std::this_thread::sleep_for(std::chrono::milliseconds(10000));
			Testing::TestRequest tr = Testing::TestRequest::fromString(msg.name());
			std::string fileSpecLog = comm_.getSendFilePath() + "/" + tr.name() + "_LogFile.txt";
			std::ofstream outfileStrm(fileSpecLog, std::ios::out);
			if (outfileStrm.good()) {
				tester.pLog()->addStream(&outfileStrm);
			}
			tester.loadLibraries(tr,comm_.getSaveFilePath());
			tester.doTests();
			tester.clear();
			std::string logFile = tr.name() + "_LogFile.txt";
			std::cout << "\n Sending log " + logFile + " to client on port ";
			std::cout << msg.from().port;			
			sendLog(msg.from(),logFile);
			tester.pLog()->removeStream(&outfileStrm);
		}
		MsgPassingCommunication::Message ready;
		ready.from(from_);
		ready.to(to_);
		ready.command("ready");
		std::cout << "\n      Tester sending msg: " + ready.command();
		comm_.postMessage(ready);
		
	}
}
using namespace Testing;

int main(int argc,char* argv[])
{
  std::cout << Utilities::sTitle("Created child TestHarness");
  MsgPassingCommunication::EndPoint endpoint1("localhost", std::stoi(argv[0]));
  std::cout << "Endpoint created : " + endpoint1.port;
  MsgPassingCommunication::EndPoint endpoint2("localhost", std::stoi(argv[1]));
  std::cout << "Endpoint created : " + endpoint2.port;
  ChildTester childTest(endpoint1,endpoint2);
  childTest.start();
  childTest.wait();
  Utilities::putline(2);
  return 0;

}
