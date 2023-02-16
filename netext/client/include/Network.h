#pragma once
#ifndef NETWORK_H
#define NETWORK_H
#include "Utilities.h"

using namespace boost::asio;

class Network {
public:
    //static bool init();
    static bool NotifyServer();
    static bool getPeerInfo();
    static void printPeerInfo();
    static ip::udp::endpoint punchHole();

    static void receiveMessage(ip::udp::socket& sock);
    static void sendMessage(ip::udp::socket& sock, ip::udp::endpoint& peer);

    static string getLocalIP();

    static char buffer[BUFSIZE];
    static ip::udp::socket sock;
    static ip::udp::endpoint rendezvous;
    static ip::udp::endpoint senderEndpoint;

    static json peerInfo;
};

#endif // !NETWORK_H