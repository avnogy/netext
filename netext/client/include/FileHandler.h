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

#define SHOW_MENU "1. Create File\n2. Delete File\n3. Edit File\n4. Exit"

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
		void editFile(string path);

		void insert(string content);
		string readEntireFile();

		int getInt();
};