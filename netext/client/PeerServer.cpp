#pragma once

#include "PeerServer.h"

PeerServer::PeerServer(boost::asio::io_context& io_context, int port) :
	_port(port), _ioc(io_context) , _acceptor(_ioc, tcp::endpoint(tcp::v4(), port))
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
		tcp::socket socket(_ioc);
		_acceptor.accept(socket);

		auto client_socket = std::make_shared<tcp::socket>(std::move(socket));

		boost::thread th(boost::bind(&PeerServer::startHandleRequests, client_socket));
	}
}


/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerServer::startHandleRequests(std::shared_ptr<tcp::socket> sock)
{
	tcp::socket& client_sock = *sock;

	if(client_sock.is_open())
		std::cout << "Client accepted!" << std::endl;
	
	
	string data;
	try
	{
		
		Helper::sendDataToClient(client_sock, "Hello!!");
		//data = Helper::receiveDataFromClient(client_sock);

		// TO DO: File Update Requests
	}
	catch (std::exception& e)
	{
		cout << "Error: " << e.what() << endl;
		cout << "Client Disconnected" << endl;
		
	}

}
