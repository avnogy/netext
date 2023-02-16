#include "include/PeerClient.h"

PeerClient::PeerClient(boost::asio::io_context& io_context) :
	_sock(io_context)
{
	
}

PeerClient::~PeerClient()
{
	_sock.close();
}

/// <summary>
/// running the peer client object by using the helper functions in the class 
/// </summary>
void PeerClient::run()
{
	int option = 0;
	string ip = "";
	int port = 0;
	// TEMPORARLY MENU UNTIL GUI
	cout << "Menu:" << endl << "1. Connect to another peer" << endl << "2. Create Session" << endl << "3. Join Session" << endl
		<< "4. Delete Session (IF ALREADY EXISTED)" << endl;

	
	cout << "Option: ";
	

	cin >> option;

	switch (option)
	{
		case 1:
		{
			cout << "Enter ip: ";
			cin >> ip;

			cout << "Enter port: ";
			cin >> port;
			connectToOtherPeer(ip, port);

			string msg = Helper::receiveDataFromClient(_sock);
			cout << msg << endl;
			break;
		}
			
		case 2:
			createSession();
			break;

		case 3:
			joinSession();
			break;
	}
}


/// <summary>
/// connecting to another peer using the socket field
/// </summary>
/// <param name="ip"></param>
/// <param name="port"></param>
/// <returns></returns>
void PeerClient::connectToOtherPeer(string ip, int port)
{
	try
	{
		tcp::endpoint endpoint(boost::asio::ip::address::from_string(ip), port);
		_sock.connect(endpoint);
		std::cout << "Connected to the Peer! , ip : " << ip << ", port : " << std::to_string(port) << std::endl;
	}
	catch (const std::exception& e)
	{
		throw MyException("Error: Failed to connect to the socket");
	}
}

/// <summary>
/// inputing session details and updating central server with request
/// </summary>
void PeerClient::createSession()
{
	json jsonData;

	string name = "";
	string key = "";

	cout << "Session Name: ";
	cin >> name;
	cout << "Session Key: ";
	cin >> key;
	jsonData["name"] = name;
	jsonData["key"] = key;

	time_t timeNow = time(TIME_NOW);
	RequestMessage msg = {CREATE_SESSION_REQUEST , timeNow , jsonData};
	string msgData = SerializeRequest(msg);

	tcp::socket centralSock = Helper::createCentralServerSocket();
	Helper::sendDataToClient(centralSock, msgData);
	string response = Helper::receiveDataFromClient(centralSock);

	json result = json::parse(response);

	cout << "Status: " << result["status"] << endl;

	centralSock.close();
}



/// <summary>
/// inputing session details and checking with central server if information is valid to enter
/// </summary>
void PeerClient::joinSession()
{
	json jsonData;
	string hashName = "";
	string ip = "";
	int port = 0;
	
	cout << "Enter Session Hash Code: ";
	cin >> hashName;
	jsonData["hash"] = hashName;

	tcp::socket centralSock = Helper::createCentralServerSocket();

	time_t timeNow = time(TIME_NOW);
	RequestMessage msg = { JOIN_SESSION_REQUEST , timeNow , jsonData };
	string msgData = SerializeRequest(msg);

	Helper::sendDataToClient(centralSock, msgData);
	string response = Helper::receiveDataFromClient(centralSock);
	json result = json::parse(response);

	int status = result["status"];
	ip = result["ip"];
	port = result["port"];

	cout << "Status: " << status << endl;

	if (status == SUCCESS)
	{
		connectToOtherPeer(ip , port);
	}

	centralSock.close();
}

