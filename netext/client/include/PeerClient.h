#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that request connections with other peers
/// </summary>
class PeerClient
{
public:
	static void joinSession();
	static void session(ip::udp::endpoint peer);
	static void redirect(ip::udp::endpoint host, ip::udp::endpoint frontend);
private:
	PeerClient();
};