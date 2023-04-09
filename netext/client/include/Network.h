#pragma once
#include "Utilities.h"
#define PORT_FILE_PATH "port.txt"
class Network {
public:
	static string createSession(const string request);
	static json joinSession(const string request);
	static void deleteSession(const string request);
	static UdpPacket getPeerInfo();
	static void printPeerInfo(const json peerInfo);
	static ip::udp::endpoint punchHole(const json peerInfo);
	static ip::udp::endpoint acceptFrontend();

	static string receiveMessage(ip::udp::socket& sock);
	static void sendMessage(ip::udp::socket& sock, ip::udp::endpoint& peer);

	static bool notify(ip::udp::endpoint recipient, const string message);

	static string serializeRequest(const Code code, const Timestamp time, const json requestData);
	static string getLocalIP();

	static void writePortToFile();

	static char buffer[BUFSIZE];
	static ip::udp::socket sock;
	static ip::udp::endpoint rendezvous;
	static ip::udp::endpoint senderEndpoint;
};
