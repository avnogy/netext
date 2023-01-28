#include "Update.h"

Update::Update(const string sender_id, const string updateCode, const int editLocation, const string updateData, const string timestamp)
	:_sender_id(sender_id), _updateCode(updateCode), _editLocation(editLocation), _updateData(updateData), _timestamp(timestamp)
{
}

/// <summary>
/// This is secondery constructor used to convert a JSON string to the fields of the message.
/// </summary>
/// <param name="data"></param>
/// <returns></returns>
Update::Update(const string data)
{
	json json = json::parse(data);

	_sender_id = json["sender_id"];
	_updateCode = json["updateCode"];
	_editLocation = json["editLocation"];
	_updateData = json["UpdateData"];
	_timestamp = json["timestamp"];
}

/// <summary>
/// This function is used to convert an Update object to a JSON string.
/// </summary>
/// <returns></returns>
string Update::Serialize()
{
	json json;
	json["sender_id"] = _sender_id;
	json["updateCode"] = _updateCode;
	json["editLocation"] = _editLocation;
	json["UpdateData"] = _updateData;
	json["timestamp"] = _timestamp;
	return json.dump();
}

bool Update::operator<(const Update& other) const
{
	return this->_timestamp < other._timestamp;
}
