#pragma once
#include "Utilities.h"

/// <summary>
/// structured request message 
/// </summary>
typedef struct RequestMessage
{
	const int code;
	const Timestamp time;
	json requestData;

}RequestMessage;

//RequestMessage DeserializeRequest(const string request);
string serializeRequest(const RequestMessage request);