#pragma once

#include "MyException.h"
#include <iostream>
#include <string>
#include <WinSock2.h>
#include <WS2tcpip.h>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

using std::string;

class PeerClient
{
	public:
		PeerClient();
		~PeerClient();

		void connectToOtherPeer(string ip, int port);

	private:
		



		SOCKET _sock;
};