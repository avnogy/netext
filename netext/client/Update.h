#pragma once
#include "Utilities.h"


/// <summary>
/// class representing a structured message protocol between peers
/// </summary>
class Update
{
private:
	string sender_id;
	string updateCode;
	int editLocation;
	string updateData;
	string timestamp;
public:
	Update(const string sender_id,
		const string updateCode,
		const int editLocation,
		const string updateData,
		const string timestamp);
	static Update Deserialize(const string data);
	string Serialize();

	bool operator<(const Update& other) const;
};
