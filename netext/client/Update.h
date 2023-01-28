#pragma once
#include "Utilities.h"


/// <summary>
/// structured message protocol between peers
/// </summary>
struct FileUpdate
{
	const unsigned int _updateCode;
	const unsigned int _sender_id;
	const unsigned int _editLocation;
	const string _updateData;
	const Timestamp _timestamp;
} typedef FileUpdate;

string Serialize(const FileUpdate update);
FileUpdate Deserialize(const string str);

bool operator<(const FileUpdate& x, const FileUpdate y);