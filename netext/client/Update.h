#pragma once
#include "Utilities.h"


/// <summary>
/// class representing a structured message protocol between peers
/// </summary>
class Update
{
private:
	string _sender_id;
	string _updateCode;
	int _editLocation;
	string _updateData;
	string _timestamp;
public:
	Update(const string sender_id,
		const string updateCode,
		const int editLocation,
		const string updateData,
		const string timestamp);
	Update(const string data);
	string Serialize();

	bool operator<(const Update& other) const;
};
