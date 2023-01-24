#pragma once
#ifndef UTILITIES_H

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
#include <Exception>
#include <boost/thread/thread.hpp>

#define CENTRAL_IP "18.196.140.61"
#define CENTRAL_PORT 1234
#define SUCCESS 1
#define FAIL 0
#define DISCONNECT -1

using json = nlohmann::json;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::to_string;

typedef unsigned char Byte; // a byte of data
typedef std::vector<Byte> Buffer; // a vector (chunk) of bytes.

enum RequestId {
	CREATE_SESSION_REQUEST = 1, JOIN_SESSION_REQUEST, DELETE_SESSION_REQUEST, LAST_ENUM
};

enum ResponseId {
	ERROR_RESPONSE = LAST_ENUM, CREATE_SESSION_RESPONSE, JOIN_SESSION_RESPONSE, DELETE_SESSION_RESPONSE
};

class Helper
{
public:
	static void sendBufferToClient(SOCKET client_sock, Buffer buff);
	static Buffer getDataBufferFromClient(SOCKET client_sock);
	static Byte* readFromClient(SOCKET client_sock, int numOfBytes);
	static Byte getId(SOCKET client_sock);
	static int getDataLength(SOCKET client_sock);
	static Byte* getData(SOCKET client_sock, int dataLength);
	static RequestId getRequestId(Byte id);

	static Buffer createLoadedBuffer(int id, string data);
	static Buffer intTo4Bytes(int num);

	static SOCKET createCentralSocket();
};

#endif // !UTILITIES_H