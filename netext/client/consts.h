#pragma once



#include <vector>
#include <algorithm>


typedef unsigned char Byte;
typedef std::vector<Byte> Buffer;



#define SUCCESS 1
#define FAIL 0

#define DISCONNECT -1

enum RequestId {
	CREATE_SESSION_REQUEST = 1, JOIN_SESSION_REQUEST, DELETE_SESSION_REQUEST, LAST_ENUM
};

enum ResponseId {
	ERROR_RESPONSE = LAST_ENUM, CREATE_SESSION_RESPONSE, JOIN_SESSION_RESPONSE, DELETE_SESSION_RESPONSE
};


