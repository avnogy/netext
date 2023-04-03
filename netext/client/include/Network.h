#pragma once
#include "Utilities.h"

class Network {
public:
	//static bool init();
	static string createSession(const string request);
	static json joinSession(const string request);
	static void deleteSession(const string request);
	static json getPeerInfo();
	static void printPeerInfo(const json peerInfo);
	static ip::udp::endpoint punchHole(const json peerInfo);
	static ip::udp::endpoint acceptFrontend();

	static string receiveMessage(ip::udp::socket& sock);
	static void sendMessage(ip::udp::socket& sock, ip::udp::endpoint& peer);

	static string serializeRequest(const int code, const Timestamp time, const json requestData);
	static string getLocalIP();

	static char buffer[BUFSIZE];
	static ip::udp::socket sock;
	static ip::udp::endpoint rendezvous;
	static ip::udp::endpoint senderEndpoint;
};
