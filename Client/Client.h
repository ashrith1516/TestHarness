/////////////////////////////////////////////////////////////////////
// Client.h - creates TestRequest and stores in requestPath        //
// ver 2.0                                                         //
// Author : Ashrith Chandramouli								   //
// Source : Jim Fawcett, CSE687 - Object Oriented Design, Fall 2018//
/////////////////////////////////////////////////////////////////////

/*
*  Required Files :
*-------------- -
*	Client.cpp
*  DateTime.h, DateTime.cpp, Message.h, Comm.h, Cpp11-BlockingQueue.h
*
*  Maintenance History :
*--------------------
*  ver 2.0 : 14 Nov 2018
* -Added a receive thread to listen from child tester
*  ver 1.0 : 23 Oct 2018
* -first release
*/

#pragma once
#include <thread>
#include <vector>
#include "../CppCommWithFileXfer/Message/Message.h"
#include "../CppCommWithFileXfer/MsgPassingComm/Comm.h"
#include "../CppCommWithFileXfer/Cpp11-BlockingQueue/Cpp11-BlockingQueue.h"

using namespace MsgPassingCommunication;

class Client
{
public:
	Client(EndPoint from, EndPoint to);
	~Client();
	void start();
	void wait();
	void sendMessages();
	void recvMessages();
private:
	std::thread send;
	std::thread recv;
	Comm comm_;
	EndPoint to_;
	EndPoint from_;
};