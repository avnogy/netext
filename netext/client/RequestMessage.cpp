
#include "RequestMessage.h"


/// <summary>
///  This function is used to convert a RequestMessage object to a JSON string.
/// </summary>
/// <param name="request"></param>
/// <returns></returns>
string SerializeRequest(const RequestMessage request)
{
	json requestJson;

	requestJson["code"] = request.code;
	requestJson["time"] = request.time;
	requestJson["data"] = request.requestData;

	return requestJson.dump();
}