#pragma once
#include "Helper.h"

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
	~Update();
	static Update Deserialize(string data);
	string Serialize();
};
