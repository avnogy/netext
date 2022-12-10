#pragma once


#include <iostream>
#include <string>
#include <WinSock2.h>

using std::string;

class PeerClient
{
	public:
		PeerClient();
		~PeerClient();

		void connectToOtherPeer(string ip, int port);

	private:
		
		SOCKET client_sock;
};