#pragma once
#include "Utilities.h"

/// <summary>
/// class represents a simple file api for file functionalities
/// </summary>
class FileHandler
{
public:

	void Menu();
	void writeToFile(string content);
	void createFile();
	void setPath(string path);
	void handleRequests();
	void insertRequest(UdpPacket request);

	static FileHandler& getInstance()
	{
		static FileHandler instance;
		return instance;
	}
	FileHandler(const FileHandler&) = delete;
	FileHandler& operator=(const FileHandler&) = delete;
private:

	FileHandler();
	~FileHandler();

	// menu functions

	void deleteFile();
	void insertIntoFile();
	void removeFromFile();

	// helper functions
	void insert(const int location, const string content);
	void remove(const int location, const int removeAmount);

	string readWholeFile();
	int getFileSize();

	bool validPosition(const int position, const int fileSize);
	bool validRemoveAmount(const int position, const int amount, const int fileSize);

	// request priority queue (sorted)
	priority_queue<UdpPacket, vector<UdpPacket>, CompareUdpPacket> _editRequests;
	mutex _muRequests;
	condition_variable _cvRequests;
	string _path;
};