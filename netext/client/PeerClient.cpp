#include "PeerClient.h"

PeerClient::PeerClient()
{
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

PeerClient::~PeerClient()
{

	closesocket(_sock);
}

void PeerClient::connectToOtherPeer(string ip, int port)
{
	sockaddr_in serverAddr;
	memset((char*)&serverAddr, 0 ,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);

	int status = connect(_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

	if (status < 0)
	{
		throw MyException("Error: Failed to connect to the socket");
	}
	std::cout << "Connected to the server! , ip : " << ip << ", port : " << std::to_string(port) << std::endl;
}
