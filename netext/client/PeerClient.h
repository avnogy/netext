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


/*
	class representing the side that request connections with other peers
*/
class PeerClient
{
	public:

		// Ctor
		PeerClient();

		// Dtor
		~PeerClient();

		void run();

		

		

	private:
		
		// menu functions
		void connectToOtherPeer(string ip, int port);
		void createSession();
		void joinSession();



		SOCKET _sock;
};