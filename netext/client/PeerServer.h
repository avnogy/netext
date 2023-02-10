#pragma once
#include "Utilities.h"

/// <summary>
/// class representing the side that accept other connections from other peers
/// </summary>
class PeerServer
{
public:
	// Ctor
	PeerServer(boost::asio::io_context& io_context ,int port);

	// Dtor
	~PeerServer();

	void run();
	static void startHandleRequests(std::shared_ptr<tcp::socket> sock);

private:
	void acceptClients();

	boost::asio::io_context& _ioc;
	tcp::acceptor _acceptor;
	int _port;
};