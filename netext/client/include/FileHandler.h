#pragma once

#include <iostream>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <string>

using std::string;
using std::cout;
using std::cin;
using std::endl;

#define SHOW_MENU "Menu:\n1. Create File\n2. Delete File\n3. Insert Into File\n4. Remove From File\n5. Exit"

class FileHandler
{
	public:
		
		void Menu();
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
		
		

		void createFile(string path);
		void deleteFile(string path);
		void insertIntoFile(string path);
		void removeFromFile(string path);

		void insert(const int location , const string content, string path);
		void remove(const int location , const int removeAmount, string path);
		string readWholeFile(string path);
		
		int getInt();
		string getPath();
};