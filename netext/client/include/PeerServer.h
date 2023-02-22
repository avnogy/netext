#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that accept other connections from other peers
/// </summary>
class PeerServer
{
public:
	static void createSession();
	static void startHandleRequests(ip::udp::endpoint peer);

private:
	static void acceptClients();
};