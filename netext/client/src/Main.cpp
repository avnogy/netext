#include "include/PeerServer.h"
#include "include/PeerClient.h"


void main()
{
	try
	{
		boost::asio::io_context io_context;

		int serverPort = Helper::generatePort();

		cout << "Server port is: " << serverPort << endl;
		PeerServer server(io_context , serverPort);
		PeerClient client(io_context);

		boost::thread serverThread(&(PeerServer::run), &server);
		boost::thread clientThread(&(PeerClient::run), &client);
		io_context.run();

		serverThread.join();
		clientThread.join();
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}