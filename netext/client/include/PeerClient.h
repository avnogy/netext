#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that request connections with other peers
/// </summary>
class PeerClient
{
public:
	static void joinSession();
	static void startHandleRequests(ip::udp::endpoint peer);
	static void session(ip::udp::endpoint peer);
	static void redirect(ip::udp::endpoint peer);
};