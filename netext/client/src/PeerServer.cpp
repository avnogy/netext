#include "include/PeerServer.h"

/// <summary>
/// accepting incoming connections and runing handle thread for each connection
/// </summary>
void PeerServer::acceptClients()
{
	thread fileHandlerThread = thread(&FileHandler::handleRequests, &FileHandler::getInstance());

	thread frontendThread(boost::bind(&PeerServer::session, Network::acceptFrontend()));

	cout << "waiting for connections.." << endl;
	UdpPacket peerInfo = Network::getPeerInfo();
	ip::udp::endpoint sock = Network::punchHole(peerInfo.data);
	thread clientThread(boost::bind(&PeerServer::session, sock));
	clientThread.join();
	frontendThread.join();
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

	string key = Network::createSession(Network::serializeRequest(Code::CREATE_SESSION_REQUEST, time(TIME_NOW), jsonData));

	cout << "key: " << key << endl;

	acceptClients();
}

/// <summary>
/// deletes session at server
/// </summary>
void PeerServer::deleteSession()
{
	cout << "Are you sure you want to delete the session? (y/n)" << endl;
	char ch;
	cin >> ch;
	cin.clear();
	cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	if (ch == 'Y' || ch == 'y')
	{
		json jsonData;
		string key = "";

		cout << "Session Key: ";
		cin >> key;
		jsonData["key"] = key;
		try
		{
			Network::deleteSession(Network::serializeRequest(Code::DELETE_SESSION_REQUEST, time(TIME_NOW), jsonData));
		}
		catch (const runtime_error& e)
		{
			cerr << "Error while deleting session:" << endl << e.what();
		}
		//TODO: tell threads to finish
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
TRY_CATCH_FUNCTION(void, PeerServer::session, (ip::udp::endpoint peer), "Client Disconnected.", {
		cout << "Client accepted!" << endl;
		Notifier::getInstance().addClient(peer);
		UdpPacketQueue::getInstance().PopForRequirements(Code::CLIENT_LEAVE_REQUEST, peer);
		Notifier::getInstance().removeClient(peer);
		cout << "Client Disconnected." << endl;
	}, {
		return;
	});