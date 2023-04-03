#pragma once

#include "Utilities.h"

#define SHOW_MENU "Menu:\n1. Create File\n2. Delete File\n3. Insert Into File\n4. Remove From File\n5. Exit"

/// <summary>
/// class represents a simple file api for file functionallities
/// </summary>
class FileHandler
{
public:

	void Menu();
	void test();
	void writeToFile(string content);
	void createFile();
	void setPath(string path);

	void insertRequest(json request);

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

	void handleRequests();

	// helper functions
	void insert(const int location, const string content);
	void remove(const int location, const int removeAmount);

	string readWholeFile();
	int getFileSize();

	bool validPosition(const int position, const int fileSize);
	bool validRemoveAmount(const int position, const int amount, const int fileSize);

	// request priority queue (sorted)
	priority_queue<json, std::vector<json>, CompareJsonByTimestamp> _editRequests;
	mutex _muRequests;
	condition_variable _cvRequests;
	string _path;

	
};