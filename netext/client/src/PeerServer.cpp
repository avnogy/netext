#include "include/PeerServer.h"

/// <summary>
/// accepting incoming connections and runing handle thread for each connection
/// </summary>
void PeerServer::acceptClients()
{
	//will be in a loop later for getting multiple clients 

	cout << "waiting for connections.." << endl;
	json peerInfo = Network::getPeerInfo();
	ip::udp::endpoint sock = Network::punchHole(peerInfo["data"]);

	boost::thread th(boost::bind(&PeerServer::startHandleRequests, sock));
}

/// <summary>
/// gets information from user and creates a session. 
/// </summary>
void PeerServer::createSession()
{
	json jsonData;
	string name = "";

	cout << "Session Name: ";
	cin >> name;
	jsonData["name"] = name;
	jsonData["localIp"] = Network::getLocalIP();
	time_t timeNow = time(TIME_NOW);

	RequestMessage msg = { CREATE_SESSION_REQUEST , timeNow , jsonData };
	string key = Network::createSession(serializeRequest(msg));

	cout << "key: " << key << endl;

	acceptClients();
}

/// <summary>
/// deletes session at server
/// </summary>
void PeerServer::deleteSession()
{
	cout << "Are you sure you want to delete the session?" << endl;
	char ch = std::getchar();
	if ( ch == 'Y' && ch == 'y')
	{
		json jsonData;
		string key = "";

		cout << "Session Key: ";
		cin >> key;
		jsonData["key"] = key;
		jsonData["localIp"] = Network::getLocalIP();
		time_t timeNow = time(TIME_NOW);

		RequestMessage msg = { DELETE_SESSION_REQUEST , timeNow , jsonData };
		Network::deleteSession(serializeRequest(msg));
		//tell threads to finish
	}
	else
	{
		cout << "Deletion Cancled." << endl;
	}
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
