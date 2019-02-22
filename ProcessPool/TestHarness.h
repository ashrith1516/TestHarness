/////////////////////////////////////////////////////////////////////
// TestHarness.h : Class for Process Pool with comm				   //
// ver 1.0                                                         //
// Ashrith C, CSE687 - Object Oriented Design, Fall 2018           //
/////////////////////////////////////////////////////////////////////
/*
Required files - Message.h, Comm.h, Cpp11-BlockingQueue.h

*/
#pragma once
#include <thread>
#include <vector>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

using namespace MsgPassingCommunication;

class TestHarness
{
public:
	TestHarness(EndPoint from, EndPoint to);
	~TestHarness();
	void start();
	void wait();
	void stop();
	void recvMessages();
	void dispatchMessages();
private:
	BlockingQueue<Message> readyQ_;
	BlockingQueue<Message> requestQ_;
	std::thread recvr;
	std::thread dspat;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
};