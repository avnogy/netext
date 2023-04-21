#pragma once

#ifndef UTILITIES_H
#define UTILITIES_H
#include <string>
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
#include <queue>
#include <condition_variable>
#include <mutex>
#include <map>

#include <boost/thread/thread.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/asio.hpp>
#include "json.hpp"

#define BUFSIZE 102400
#define SERVER_IP "18.196.140.61"
#define SERVER_PORT 55555
#define SUCCESS 1
#define FAILURE 0
#define DISCONNECT -1
#define AUTO_SRC_PORT 0
#define TIME_NOW 0

using namespace boost::asio;
using json = nlohmann::json;
using thread = boost::thread;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::vector;
using std::queue;
using std::endl;
using std::to_string;
using std::priority_queue;
using std::mutex;
using std::condition_variable;
using std::unique_lock;
using std::lock_guard;
using std::runtime_error;
using std::exception;
typedef std::time_t Timestamp; //used as a timestamp for update messages
typedef unsigned char Byte; // a byte of data
typedef std::vector<Byte> Buffer; // a vector (chunk) of bytes.

enum struct Code {
	//requests
	CREATE_SESSION_REQUEST = 100,
	JOIN_SESSION_REQUEST,
	DELETE_SESSION_REQUEST,
	FILE_INSERT_REQUEST,
	FILE_REMOVE_REQUEST,
	FILE_OPEN_REQUEST,
	FRONTEND_SESSION_JOIN,
	CLIENT_LEAVE_REQUEST,

	//responses
	ERROR_RESPONSE = 200,
	CREATE_SESSION_RESPONSE,
	JOIN_SESSION_RESPONSE,
	DELETE_SESSION_RESPONSE,
	PEER_INFO_RESPONSE,
	FILE_INSERT_RESPONSE,
	FILE_REMOVE_RESPONSE,
	FILE_OPEN_RESPONSE
};

int getInt();
string getPath();

struct UdpPacket {
	ip::udp::endpoint endpoint;
	Timestamp timestamp{};
	Code type{};
	json data;
};

struct CompareUdpPacket
{
	bool operator()(const UdpPacket& left, const UdpPacket& right) const
	{
		Timestamp timeL = left.timestamp;
		Timestamp timeR = right.timestamp;
		return timeL > timeR;
	}
} typedef CompareUdpPacket;

#include "include/UdpHandler.h"
#include "include/FileHandler.h"
#include "Notifier.h"
#include "PeerClient.h"
#include "PeerServer.h"
#include "Network.h"
#include "Menu.h"

#define TRY_CATCH_FUNCTION(return_type, func_name, params, error_msg, body) \
    return_type func_name params { \
        try { \
            body \
        } \
        catch (const runtime_error& e) { \
            cout << error_msg << endl; \
            cout << "Error: " << e.what() << endl; \
        } \
        catch (const exception& e) { \
            throw runtime_error(error_msg); \
        } \
    }

#define TRY_CATCH_LOOP_FUNCTION(return_type, func_name, params, error_msg, body) \
    return_type func_name params { \
		while (true)\
			{\
			try { \
				body \
			}\
			catch (const runtime_error& e) { \
				cout << error_msg << endl; \
				cout << "Error: " << e.what() << endl; \
			} \
			catch (const exception& e) {\
				throw runtime_error(error_msg); \
			} \
			} \
	}

#endif // !UTILITIES_H
