#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "MyException.h"
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <format>
#include <sstream>
#include <cstring>
#include <array>
#include <cstdint>
#include <vector>
#include <algorithm>
#include <iostream>
#include <numeric>
#include <cmath>
#include <ctime>
#include <Exception>
#include <boost/thread/thread.hpp>
#include <boost/asio.hpp>



#define CENTRAL_IP "18.196.140.61"
#define CENTRAL_PORT 1234
#define SUCCESS 1
#define FAIL 0
#define DISCONNECT -1
#define TIME_NOW 0

#define BUFSIZE 1024

#define LOWEST_PORT 1024
#define HIGHEST_PORT 40000

using json = nlohmann::json;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::to_string;

using boost::asio::ip::tcp;

typedef std::time_t Timestamp; //used as a timestamp for update messages
typedef unsigned char Byte; // a byte of data
typedef std::vector<Byte> Buffer; // a vector (chunk) of bytes.

enum RequestCode {
	CREATE_SESSION_REQUEST = 1, JOIN_SESSION_REQUEST, DELETE_SESSION_REQUEST, LAST_ENUM
};

enum ResponseCode {
	ERROR_RESPONSE = LAST_ENUM, CREATE_SESSION_RESPONSE, JOIN_SESSION_RESPONSE, DELETE_SESSION_RESPONSE
};

class Helper
{
public:
	static void sendDataToClient(tcp::socket& client_sock, const string& data);
	static string receiveDataFromClient(tcp::socket& client_sock);

	static int generatePort();
	static bool portInUse(int port);

	static tcp::socket createCentralServerSocket();


};

#endif // !UTILITIES_H