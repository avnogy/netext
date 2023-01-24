#pragma once

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
#include "consts.h"
#include "MyException.h"
#include "json.hpp"
#include <boost/filesystem.hpp>
#include <iostream>
#include <format>
#include <sstream>
#include <cstring>

#define CENTRAL_IP "18.196.140.61"
#define CENTRAL_PORT 1234

using json = nlohmann::json;
using std::string;
using std::cout;
using std::endl;
using std::to_string;

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