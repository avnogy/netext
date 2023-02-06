#pragma once
#include "Utilities.h"
#include "RequestMessage.h"

/// <summary>
/// class representing the side that request connections with other peers
/// </summary>
class PeerClient
{
public:
	// Ctor
	PeerClient(boost::asio::io_context& io_context);

	// Dtor
	~PeerClient();

	void run();

private:
	// menu functions
	tcp::socket connectToOtherPeer(string ip, int port);
	void createSession();
	void joinSession();

	tcp::socket _sock;
};