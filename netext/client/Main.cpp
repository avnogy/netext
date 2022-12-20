#pragma comment (lib , "ws2_32.lib")



#include "WSAInitializer.h"
#include "PeerClient.h"
#include "PeerServer.h"


void main()
{
	try
	{
		WSAInitializer wsa_init;
		
		PeerServer server(8080 , "127.0.0.1");
		PeerClient client;

		boost::thread serverThread(&(PeerServer::run), &server);

	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}


}