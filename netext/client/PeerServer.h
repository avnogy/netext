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
		PeerServer();
		~PeerServer();

		void bindAndListen(int port, string server_ip);

		static void startHandleRequests(SOCKET client_sock);

	private:

		
		
		SOCKET _serverSocket;
};