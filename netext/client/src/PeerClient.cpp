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

	json peerInfo = Network::joinSession(Network::serializeRequest(Code::JOIN_SESSION_REQUEST, time(TIME_NOW), jsonData));

	PeerClient::startHandleRequests(Network::punchHole(peerInfo));
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
		//// TODO:
		//	 remove the receiver thread and make the sending work without the menu getting in the way.

		// Creating a sender thread
		boost::thread sender_thread(boost::bind(&Network::sendMessage, boost::ref(Network::sock), peer));

		// Creating a receiver thread
		//boost::thread receiver_thread(boost::bind(&Network::receiveMessage, boost::ref(Network::sock)));

		sender_thread.join();

		// TO DO: File Update Requests
	}
	catch (std::exception& e)
	{
		cerr << "Client Disconnected." << endl;
	}
}