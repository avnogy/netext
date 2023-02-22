#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include "Utilities.h"

using namespace boost::asio;

class Network {
public:
    //static bool init();
    static string CreateSession();
    static json getPeerInfo();
    static void printPeerInfo(const json peerInfo);
    static ip::udp::endpoint punchHole(const json peerInfo);

    static void receiveMessage(ip::udp::socket& sock);
    static void sendMessage(ip::udp::socket& sock, ip::udp::endpoint& peer);

    static string getLocalIP();

    static char buffer[BUFSIZE];
    static ip::udp::socket sock;
    static ip::udp::endpoint rendezvous;
    static ip::udp::endpoint senderEndpoint;
};

#endif // !NETWORK_H