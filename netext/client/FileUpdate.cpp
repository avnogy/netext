#include "FileUpdate.h"
#include "RequestMessage.h"


/// <summary>
/// This function is used to convert a JSON string to a FileUpdate object.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
FileUpdate DeserializeFileUpdate(const string str)
{
	json json = json::parse(str);
	return FileUpdate(
		json["updateCode"],
		json["sender_id"],
		json["editLocation"],
		json["UpdateData"],
		json["timestamp"]
	);
}

/// <summary>
/// This function is used to convert a FileUpdate object to a JSON string.
/// </summary>
/// <returns></returns>
string SerializeFileUpdate(const FileUpdate update)
{
	json json;
	json["updateCode"] = update.updateCode;
	json["sender_id"] = update.senderId;
	json["editLocation"] = update.editLocation;
	json["UpdateData"] = update.updateData;
	json["timestamp"] = update.timestamp;
	return json.dump();
}

/// <summary>
/// an < operator to compare Updates by timestamp
/// </summary>
/// <param name="x"></param>
/// <param name="y"></param>
/// <returns></returns>
bool operator<(const FileUpdate& x, const FileUpdate y)
{
	return x.timestamp < y.timestamp;
}
