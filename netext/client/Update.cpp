#include "Update.h"


Update::Update(const string sender_id, const string updateCode, const int editLocation, const string updateData, const string timestamp)
	:sender_id(sender_id), updateCode(updateCode), editLocation(editLocation), updateData(updateData), timestamp(timestamp)
{
}

Update::~Update()
{
}

Update Update::Deserialize(string data)
{
	json json = json::parse(data);

	const string sender_id = json["sender_id"];
	const string updateCode = json["updateCode"];
	const int editLocation = json["editLocation"];
	const string updateData = json["UpdateData"];
	const string timestamp = json["timestamp"];
	return Update(sender_id, updateCode, editLocation, updateData, timestamp);
}

string Update::Serialize()
{
	json json;
	json["sender_id"] = sender_id;
	json["updateCode"] = updateCode;
	json["editLocation"] = editLocation;
	json["UpdateData"] = updateData;
	json["timestamp"] = timestamp;
	return json.dump();
}


