#pragma comment (lib , "ws2_32.lib")

#include "WSAInitializer.h"
#include "PeerClient.h"
#include "PeerServer.h"


void main()
{
	try
	{
		WSAInitializer wsa_init;
		
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}


}