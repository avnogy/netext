#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that accept other connections from other peers
/// </summary>
class PeerServer
{
public:
	// Ctor
	PeerServer(int port, string ip);

	// Dtor
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