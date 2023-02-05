#pragma once

#include "PeerServer.h"

PeerServer::PeerServer(boost::asio::io_context& io_context, int port) :
	_port(port), _acceptor(io_context , tcp::endpoint(tcp::v4() , port))
{


}
PeerServer::~PeerServer()
{
	
}


/// <summary>
/// running the peer server object by using the helper functions in the class
/// </summary>
void PeerServer::run()
{
	acceptClients();
}



/// <summary>
/// accepting incoming connections and runing handle thread for each connection
/// </summary>
void PeerServer::acceptClients()
{
	while (true)
	{
		tcp::socket socket(_acceptor.get_executor().context());
		_acceptor.accept(socket);

		std::cout << "Connected!" << std::endl;
		boost::thread th(&PeerServer::startHandleRequests, this, std::move(socket));
	}
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerServer::startHandleRequests(tcp::socket client_sock)
{
	std::cout << "Client accepted!" << std::endl;
	string data;
	while (true)
	{
		try
		{
			data = Helper::receiveDataFromClient(client_sock);


		}
		catch (std::exception& e)
		{
			std::cout << "Client Disconnected" << std::endl;
			break;
		}
	}
}
