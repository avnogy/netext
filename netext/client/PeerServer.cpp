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


/*
	Function: running the peer server object by using the helper functions in the class
	input: none
	output: none
*/
void PeerServer::run()
{
	bindAndListen();
	acceptClients();
}


/*
	Function: binding the peer server socket and start listening to connections.
	input: none
	output: none
*/
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


/*
	Function: accepting incoming connections and runs handle thread for each connection
	input: none
	output: none
*/
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


/*
	Function: handling a client (his requests) 
	input: client socket (SOCKET)
	output: none
*/
void PeerServer::startHandleRequests(SOCKET client_sock)
{
	std::cout << "Client accepted!" << std::endl;

	Byte id;
	RequestId idReq;
	Buffer buff;

	while (true)
	{
		try
		{
			id = Helper::getId(client_sock);
			idReq = Helper::getRequestId(id);
			buff = Helper::getDataBufferFromClient(client_sock);
			
			
		}
		catch (std::exception& e)
		{
			std::cout << "Client Disconnected" << std::endl;
			break;
		}
	}
}
