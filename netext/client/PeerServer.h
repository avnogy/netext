#pragma once

#include "MyException.h"
#include <iostream>
#include <string>
#include <boost/thread/thread.hpp>
#include <WinSock2.h>
#include <ws2tcpip.h>

using std::string;


class PeerServer 
{
	public:
		PeerServer(int port , string ip);
		~PeerServer();

		void run();

		



		static void startHandleRequests(SOCKET client_sock);

	private:

		void bindAndListen();
		void acceptClients();
		
		SOCKET _serverSocket;
		int _port;
		string _ip;
};