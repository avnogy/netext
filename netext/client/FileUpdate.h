#pragma once
#include "Utilities.h"


/// <summary>
/// structured file updating message protocol between peers
/// </summary>
struct FileUpdate
{
	const unsigned int updateCode;
	const unsigned int senderId;
	const unsigned int editLocation;
	const string updateData;
	const Timestamp timestamp;
} typedef FileUpdate;

string SerializeFileUpdate(const FileUpdate update);
FileUpdate DeserializeFileUpdate(const string str);

bool operator<(const FileUpdate& x, const FileUpdate y);