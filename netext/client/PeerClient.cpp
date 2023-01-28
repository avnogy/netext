#include "PeerClient.h"

PeerClient::PeerClient()
{
	_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
}

PeerClient::~PeerClient()
{
	closesocket(_sock);
}




/// <summary>
/// running the peer client object by using the helper functions in the class 
/// </summary>
void PeerClient::run()
{
	// TEMPORARLY MENU UNTIL GUI
	cout << "Menu:" << endl << "1. Connect to another peer" << endl << "2. Create Session" << "3. Join Session" << "4. Delete Session (IF ALREADY EXISTED)" << endl;

	int option = 0;
	string ip = "";
	int port = 0;

	cin >> option;

	switch (option)
	{
		case 1:
			

			cout << "Enter ip: ";
			cin >> ip;

			cout << "Enter port: ";
			cin >> port;
			connectToOtherPeer(ip , port);
			break;

		case 2:
			createSession();
			break;
		
		
	}
	
}



/// <summary>
/// connecting to another peer using the socket field
/// </summary>
/// <param name="ip">peer's ip</param>
/// <param name="port">peer's port</param>
void PeerClient::connectToOtherPeer(string ip, int port)
{

	


	sockaddr_in serverAddr;
	memset((char*)&serverAddr, 0 ,sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, ip.c_str(), &serverAddr.sin_addr);
	serverAddr.sin_port = htons(port);

	int status = connect(_sock, (sockaddr*)&serverAddr, sizeof(serverAddr));

	if (status < 0)
	{
		throw MyException("Error: Failed to connect to the socket");
	}
	std::cout << "Connected to the Peer! , ip : " << ip << ", port : " << std::to_string(port) << std::endl;
}






/// <summary>
/// inputing session details and updating central server with request
/// </summary>
void PeerClient::createSession()
{
	json jsonData;

	string name = "";
	string key = "";
	string ip = "";
	int port = 0;

	cout << "Session Name: ";
	cin >> name;

	cout << "Session Key: ";
	cin >> key;

	cout << "Session ip: ";
	cin >> ip;

	cout << "Session port: ";
	cin >> port;

	jsonData["name"] = name;
	jsonData["key"] = key;
	jsonData["ip"] = ip;
	jsonData["port"] = port;

	Buffer buff = Helper::createLoadedBuffer(CREATE_SESSION_REQUEST, jsonData.dump());

	SOCKET centralSock = Helper::createCentralSocket();

	Helper::sendBufferToClient(centralSock, buff);

	int responseId = Helper::getRequestId(Helper::getId(centralSock));
	Buffer responseData = Helper::getDataBufferFromClient(centralSock);

	string data(responseData.begin(), responseData.end());

	json result = json::parse(data);

	cout << "Status: " << result["status"] << endl;

	closesocket(centralSock);
}



/// <summary>
/// inputing session details and checking with central server if information is valid to enter
/// </summary>
void PeerClient::joinSession()
{

	json jsonData;

	string name = "";
	string key = "";
	string ip = "";
	int port = 0;

	cout << "Session Name To Join: ";

	cin >> name;

	cout << "Enter Session Key: ";

	cin >> key;

	jsonData["session"] = name;
	jsonData["key"] = key;

	Buffer buff = Helper::createLoadedBuffer(JOIN_SESSION_REQUEST, jsonData.dump());

	SOCKET centralSock = Helper::createCentralSocket();

	Helper::sendBufferToClient(centralSock, buff);

	int responseId = Helper::getRequestId(Helper::getId(centralSock));
	Buffer responseData = Helper::getDataBufferFromClient(centralSock);

	string data(responseData.begin(), responseData.end());

	json result = json::parse(data);

	int status = result["status"];
	ip = result["ip"];
	port = result["port"];


	cout << "Status: " << status << endl;

	closesocket(centralSock);

	if (status == SUCCESS)
	{
		connectToOtherPeer(ip , port);
	}
}
