#include "include/PeerClient.h"

/// <summary>
/// inputing session details and checking with central server if information is valid to enter
/// </summary>
void PeerClient::joinSession()
{
	json jsonData;
	string key = "";
	
	cout << "Enter Session key: ";
	cin >> key;
	jsonData["key"] = key;
	jsonData["localIp"] = Network::getLocalIP();
	time_t timeNow = time(TIME_NOW);

	RequestMessage msg = { JOIN_SESSION_REQUEST , timeNow , jsonData };
	json peerInfo = Network::joinSession(SerializeRequest(msg));

	ip::udp::endpoint sock = Network::punchHole(peerInfo["data"]);
	cout << "joined session! " << endl;

	PeerClient::startHandleRequests(sock);
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerClient::startHandleRequests(ip::udp::endpoint peer)
{
	cout << "Host found!" << endl;

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