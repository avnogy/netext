#include "Update.h"

/// <summary>
/// This function is used to convert a JSON string to an Update object.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
FileUpdate Deserialize(const string str)
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
/// This function is used to convert an Update object to a JSON string.
/// </summary>
/// <returns></returns>
string Serialize(const FileUpdate update)
{
	json json;
	json["updateCode"] = update._updateCode;
	json["sender_id"] = update._sender_id;
	json["editLocation"] = update._editLocation;
	json["UpdateData"] = update._updateData;
	json["timestamp"] = update._timestamp;
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
	return x._timestamp < y._timestamp;
}
