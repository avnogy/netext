#pragma once
#ifndef UTILITIES_H
#define UTILITIES_H

#include <WinSock2.h>
#include <WS2tcpip.h>
#include <string>
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
#include "json.hpp"
#include <boost/asio.hpp>


#define SERVER_IP "18.196.140.61"
#define SERVER_PORT 55555
#define SUCCESS 1
#define FAIL 0
#define DISCONNECT -1
#define AUTO_SRC_PORT 50001

using json = nlohmann::json;
using std::string;
using std::cin;
using std::cout;
using std::endl;
using std::to_string;



#endif // !UTILITIES_H