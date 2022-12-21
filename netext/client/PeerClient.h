#pragma once


#include "MyException.h"
#include "Helper.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>



using std::string;
using std::cout;
using std::endl;
using std::cin;


class PeerClient
{
	public:
		PeerClient();
		~PeerClient();

		void run();

		

		

	private:
		
		
		void connectToOtherPeer(string ip, int port);
		void createSession();
		void joinSession();



		SOCKET _sock;
};