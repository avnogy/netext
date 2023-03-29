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
#include <mutex>
#include "include/json.hpp"
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/thread/thread.hpp>

#define BUFSIZE 1024
#define SERVER_IP "18.196.140.61"
#define SERVER_PORT 55555
#define SUCCESS 1
#define FAILURE 0
#define DISCONNECT -1
#define AUTO_SRC_PORT 0
#define TIME_NOW 0

using json = nlohmann::json;
using std::string;
using std::cin;
using std::cout;
using std::cerr;
using std::endl;
using std::to_string;
using std::priority_queue;
using std::mutex;

typedef std::time_t Timestamp; //used as a timestamp for update messages
typedef unsigned char Byte; // a byte of data
typedef std::vector<Byte> Buffer; // a vector (chunk) of bytes.

enum RequestCode {
	CREATE_SESSION_REQUEST = 100, JOIN_SESSION_REQUEST, DELETE_SESSION_REQUEST , FILE_INSERT_REQUEST , FILE_REMOVE_REQUEST
};

enum ResponseCode {
	ERROR_RESPONSE = 200, CREATE_SESSION_RESPONSE, JOIN_SESSION_RESPONSE, DELETE_SESSION_RESPONSE, PEER_INFO_RESPONSE
};


int getInt();
string getPath();


#include "include/FileHandler.h"


#endif // !UTILITIES_H
