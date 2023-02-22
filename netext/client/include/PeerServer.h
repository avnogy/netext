#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that accept other connections from other peers
/// </summary>
class PeerServer
{
public:
	static bool CreateSession();
	static void startHandleRequests(std::shared_ptr<tcp::socket> sock);

private:
	static void acceptClients();
};