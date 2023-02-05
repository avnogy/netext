#pragma once

#include "Utilities.h"

/// <summary>
/// structured request message 
/// </summary>
struct RequestMessage
{
	const int code;
	const Timestamp time;
	json requestData;

} typedef RequestMessage;

string SerializeRequest(const RequestMessage request);