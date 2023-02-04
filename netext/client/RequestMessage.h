#pragma once

#include "Utilities.h"

/// <summary>
/// structured request message between a peer and the centralized server
/// </summary>
struct RequestMessage
{
	const RequestId requestCode;
	const unsigned int senderId;
	json requestData;

} typedef RequestMessage;

string SerializeRequest(const RequestMessage request);
RequestMessage DeserializeRequest(const string str);

