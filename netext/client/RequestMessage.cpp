
#include "RequestMessage.h"

/// <summary>
/// This function is used to convert a JSON string to an RequestMessage object.
/// </summary>
/// <param name="str"></param>
/// <returns></returns>
RequestMessage DeserializeRequest(const string str)
{
	json jsonRequest = json::parse(str);

	return RequestMessage(
		jsonRequest["requestCode"],
		jsonRequest["senderId"] ,
		jsonRequest["requestData"]
	);
}

/// <summary>
///  This function is used to convert a RequestMessage object to a JSON string.
/// </summary>
/// <param name="request"></param>
/// <returns></returns>
string SerializeRequest(const RequestMessage request)
{
	json requestJson;

	requestJson["requestCode"] = request.requestCode;
	requestJson["senderId"] = request.senderId;
	requestJson["requestData"] = request.requestData;

	return requestJson.dump();
}