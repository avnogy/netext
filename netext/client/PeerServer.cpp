#include "PeerServer.h"

PeerServer::PeerServer(int port , string ip) : 
	_port(port) , _ip(ip)
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

PeerServer::~PeerServer()
{
	closesocket(_serverSocket);
}

void PeerServer::run()
{
	bindAndListen();
	acceptClients();
}

void PeerServer::bindAndListen()
{


	struct sockaddr_in server;

	memset(&server, 0, sizeof(struct sockaddr_in));

	server.sin_family = AF_INET;
	server.sin_port = htons(_port);
	inet_pton(AF_INET, _ip.c_str(), &server.sin_addr);

	if (bind(_serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");


	
}

void PeerServer::acceptClients()
{
	SOCKET clientSock;
	struct sockaddr_in client;

	while (true)
	{
		int clientlen = sizeof(struct sockaddr_in);

		if ((clientSock = accept(_serverSocket, (struct sockaddr*)&client, &clientlen)) == -1)
			throw std::runtime_error("Server::startListening: Failed to establish connection with client");

		std::cout << "Connected!" << std::endl;


		boost::thread th(&startHandleRequests, clientSock);
	}
}

void PeerServer::startHandleRequests(SOCKET client_sock)
{
	// TO DO : SPRINT 2
}
