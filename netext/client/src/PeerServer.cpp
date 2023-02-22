#include "include/PeerServer.h"

/// <summary>
/// accepting incoming connections and runing handle thread for each connection
/// </summary>
void PeerServer::acceptClients()
{
	json peerInfo = Network::getPeerInfo();
	ip::udp::endpoint sock = Network::punchHole(peerInfo["data"]);

	boost::thread th(boost::bind(&PeerServer::startHandleRequests, sock));
}


bool PeerServer::createSession()
{
	json jsonData;
	string name = "";

	cout << "Session Name: ";
	cin >> name;
	jsonData["name"] = name;
	jsonData["localIp"] = Network::getLocalIP();
	time_t timeNow = time(TIME_NOW);

	RequestMessage msg = { CREATE_SESSION_REQUEST , timeNow , jsonData };
	string key = Network::createSession(SerializeRequest(msg));

	cout << "key: " << key << endl;
	cout << "waiting for connections.." << endl;
	acceptClients();
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerServer::startHandleRequests(ip::udp::endpoint peer)
{
	cout << "Client accepted!" << endl;

	try
	{
		// Creating a sender thread
		boost::thread sender_thread(boost::bind(&Network::sendMessage, boost::ref(Network::sock), peer));

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
