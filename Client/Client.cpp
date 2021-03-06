/////////////////////////////////////////////////////////////////////
// Client.cpp - creates TestRequest and stores in requestPath      //
// ver 2.0                                                         //
// Author : Ashrith Chandramouli								   //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  Client creates test request and stores in requestPath.
*  - This simulates what a Project #4 client does, e.g., creates
*    TestRequests and sends to TestHarness via a Comm channel.
*
*  Required Files:
*  ---------------
*  Client.h, Message.h, Comm.h
*  TestRequest.h, FileUtilities.h
*  DateTime.h, DateTime.cpp
*
*  Maintenance History:
*  --------------------
*  ver 2.0 : 14 Nov 2018
*  - Added a receive thread to listen from child tester
*  ver 1.0 : 23 Oct 2018
*  - first release
*/

#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../TestRequest/TestRequest.h"
#include "../Utilities/FileUtilities/FileUtilities.h"
#include "../CppCommWithFileXfer/Message/Message.h"
#include "Client.h"

#include <iostream>

using namespace MsgPassingCommunication;

Client::Client(EndPoint from, EndPoint to) : comm_(from, "sender"), to_(to), from_(from)
{
	comm_.start();
	std::string savePath = comm_.setSaveFilePath("../SaveLibraries");
	std::cout << "\n  saving files to \"" << savePath << "\"";
	std::string sendPath = comm_.setSendFilePath("../TestLibraries");
	std::cout << "\n  sending files from \"" << sendPath << "\"";
}
//----< stops comm and detaches thread if needed >-----------------

Client::~Client()
{
	std::cout << "\n  Client Comm shutting down";
	comm_.stop();
	if (recv.joinable())
		recv.detach();
	if (send.joinable())
		send.detach();
}
//----< creates thread to send test request messages >-------------

void Client::start()
{
	std::cout << "\n  starting Client's send thread listening on " + from_.toString();
	send = std::thread([&]() { sendMessages(); });
	recv = std::thread([&]() { recvMessages(); });

}
//----< waits for all messages to be sent >------------------------

void Client::wait()
{
	if (recv.joinable())
		recv.join();
	if (send.joinable())
		send.join();
}
//----< send request messages >------------------------------------
/*
*  - send thread will shut down when this method completes
*  - sleeps just for display so everything doesn't happen at once
*/

void Client::sendMessages()
{
	Message fileMsg;
	fileMsg.to(to_);
	fileMsg.from(from_);
	fileMsg.value("sendingFile");   
	fileMsg.name("ClientDll1");         
	fileMsg.file("TestDriver1.dll");        
	std::cout << "\n  " + comm_.name() + " posting: " + fileMsg.name();
	comm_.postMessage(fileMsg);
	fileMsg.name("ClientDll2");
	fileMsg.file("TestDriver2.dll");         
	std::cout << "\n  " + comm_.name() + " posting: " + fileMsg.name();
	comm_.postMessage(fileMsg);
	fileMsg.name("ReqsDemo");
	fileMsg.file("ReqsDemo.dll");
	std::cout << "\n  " + comm_.name() + " posting: " + fileMsg.name();
	comm_.postMessage(fileMsg);
	Testing::TestRequest tr1;
	tr1.name("tr1");
	tr1.author("Fawcett");
	tr1.date(Utilities::DateTime().now());
	tr1.request.valueRef().push_back("TestDriver1.dll");
	tr1.request.valueRef().push_back("TestDriver2.dll");
	Utilities::title("Created test request:");
	std::cout << "\n  " << tr1.toString();
	Message testReq;
	testReq.to(to_);
	testReq.from(from_);
	testReq.name(tr1.toString());
	testReq.command("testReq");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "\n  Client sending msg: " + testReq.toString();
	comm_.postMessage(testReq);

	Testing::TestRequest tr2;
	tr2.name("tr2");
	tr2.author("Ashrith");
	tr2.date(Utilities::DateTime().now());
	tr2.request.valueRef().push_back("ReqsDemo.dll");
	Utilities::title("Created test request:");
	std::cout << "\n  " << tr2.toString();

	Message testReq2;
	testReq2.to(to_);
	testReq2.from(from_);
	testReq2.name(tr2.toString());
	testReq2.command("testReq");
	std::this_thread::sleep_for(std::chrono::milliseconds(500));
	std::cout << "\n  Client sending msg: " + testReq2.toString();
	comm_.postMessage(testReq2);
}

void Client::recvMessages()
{
	int recvCount = 0;
	while (true)
	{
		MsgPassingCommunication::Message msg = comm_.getMessage();
		std::cout << "\n      Client " + from_.toString() + " received msg: " + msg.toString();
		recvCount = recvCount + 1;
		if (recvCount == 2)
		{
			msg.name("exit");
			msg.from(from_);
			msg.command("exit");
			comm_.postMessage(msg);
			std::cout << "\n     Received all log files. Client shutting down";
			break;
		}
	}
}


namespace Testing
{
  //----< mock test >------------------------------------------------
  /*
  * - This function is used build a TestRequest.
  * - It's used to demonstrate TestHarness Core operations.
  */
  void mock_Test()
  {
    TestRequest tr1;
    tr1.name("tr1");
    tr1.author("Fawcett");
    tr1.date(DateTime().now());
    tr1.request.valueRef().push_back("TestDriver1.dll");
   // tr1.request.valueRef().push_back("TestDriver2.dll");

    Utilities::title("Created test request:");
    std::cout << "\n  " << tr1.toString();

    PathStr TrPath = "../TestRequests";
    PathStr fileSpec = TrPath + "/tr1.txt";
    std::wcout << ("\n\n  Writing to \"" + fileSpec + "\"").c_str();

    std::ofstream out;
    if (Utilities::openWriteFile(fileSpec, out))
    {
      out << tr1.toString();
    }
    std::cout << "\n";
  }
}


int main()
{
  Utilities::Title("Demonstrating Client");
  MsgPassingCommunication::EndPoint ep1("localhost", 8081);
  MsgPassingCommunication::EndPoint ep2("localhost", 8082);
  Client client1(ep1, ep2);
  client1.start();
  std::this_thread::sleep_for(std::chrono::milliseconds(4000));
  client1.wait();
  std::cout << "\nPress any key to exit";
  getchar();
  return 0;
}

