#include "PeerServer.h"

PeerServer::PeerServer()
{
	_serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

PeerServer::~PeerServer()
{
	closesocket(_serverSocket);
}

void PeerServer::bindAndListen(int port , string serverIp)
{

	SOCKET clientSock;
	struct sockaddr_in server, client;

	memset(&server, 0, sizeof(struct sockaddr_in));

	server.sin_family = AF_INET;
	server.sin_port = htons(port);
	inet_pton(server.sin_family, serverIp.c_str(), &server.sin_addr.s_addr);

	if (bind(_serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");
	
	if (listen(_serverSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");


	while (true)
	{
		int clientlen = sizeof(struct sockaddr_in);

		if ((clientSock = accept(_serverSocket, (struct sockaddr*)&client, &clientlen)) == -1)
			throw std::runtime_error("Server::startListening: Failed to establish connection with client");

		std::cout << inet_ntoa(client.sin_addr) << " connected" << std::endl;
		std::cout << "hello" << std::endl;


		boost::thread th(&startHandleRequests, clientSock);
	}
}

void PeerServer::startHandleRequests(SOCKET client_sock)
{
	// TO DO : SPRINT 2
}
