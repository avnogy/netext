#include "include/PeerServer.h"

/// <summary>
/// accepting incoming connections and runing handle thread for each connection
/// </summary>
void PeerServer::acceptClients()
{
		json peerInfo =  Network::getPeerInfo();
		ip::udp::endpoint sock = Network::punchHole(peerInfo);

		boost::thread th(boost::bind(&PeerServer::startHandleRequests, sock));
}


bool PeerServer::CreateSession()
{
	string key = Network::CreateSession();
	cout << "key: " << key << endl;
	cout << "waiting for connections.." << endl;
	acceptClients();
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerServer::startHandleRequests(std::shared_ptr<tcp::socket> sock)
{

	std::cout << "Client accepted!" << std::endl;
	
	string data;
	try
	{	
		// Creating a sender thread
		boost::thread sender_thread(boost::bind(&Network::sendMessage, boost::ref(Network::sock), sock));

		// Creating a receiver thread
		boost::thread receiver_thread(boost::bind(&Network::receiveMessage, boost::ref(Network::sock)));
		
		// TO DO: File Update Requests
	}
	catch (std::exception& e)
	{
		cout << "Error: " << e.what() << endl;
		cout << "Client Disconnected." << endl;		
	}
}
