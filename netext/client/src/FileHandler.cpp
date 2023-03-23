#include "include/FileHandler.h"





FileHandler::FileHandler()
{
	Menu();
}

FileHandler::~FileHandler()
{
}


/// <summary>
/// function runs menu to the user
/// </summary>
void FileHandler::Menu()
{
	int option = 0;
	while (option != 5)
	{
		cout << SHOW_MENU << endl;
		while (option < 1 || option > 5)
		{
			cout << "Select Option: ";
			option = getInt();

			if (option < 1 || option > 5)
			{
				cout << "Invalid option chosen , type a existed option!" << endl;
			}
		}
		switch (option)
		{
			case 1:
				createFile(getPath());
				break;
			case 2:
				deleteFile(getPath());
				break;
			case 3:
				insertIntoFile(getPath());
				break;
			case 4:
				removeFromFile(getPath());
				break;
			default:
				break;
		}
		option = 0;
	}
}


/// <summary>
/// function creates a file 
/// </summary>
/// <param name="path"></param>
void FileHandler::createFile(string path)
{
	if (!boost::filesystem::exists(path))
	{
		boost::filesystem::ofstream file(path);
		file << "Hello";
		file.close();
	}
	else
	{
		cout << "File with this path already exists!" << endl;
	}

}

/// <summary>
/// function deletes a file (if exists)
/// </summary>
/// <param name="path"></param>
void FileHandler::deleteFile(string path)
{
	if (boost::filesystem::exists(path))
	{
		boost::filesystem::remove(path.c_str());
	}
	else
	{
		cout << "File with this path does not exist!";
	}
}


/// <summary>
/// input int including handling invalid inpu
/// </summary>
/// <returns></returns>
int FileHandler::getInt()
{

	int num = 0;
	cin >> num;
	while (!cin.good() )
	{
		//reset buffer
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		//trying again
		cout << "Invalid input, Try again." << endl;
		cin >> num;
	}
	return num;

}


/// <summary>
/// function inserts data into existing file (appending)
/// </summary>
/// <param name="path"></param>
void FileHandler::insertIntoFile(string path)
{
	if (!boost::filesystem::exists(path))
	{
		cout << "File does not exist!" << endl;
		return;
	}

	int position = 0;
	string data = "";

	cout << "File Size: " << boost::filesystem::file_size(path) << endl;

	cout << "Enter Position: ";
	position = getInt();

	cout << "Enter Data To Insert: ";
	cin >> data;

	insert(position, data, path);
}


/// <summary>
/// function removing data from a file
/// </summary>
/// <param name="path"></param>
void FileHandler::removeFromFile(string path)
{
	if (!boost::filesystem::exists(path))
	{
		cout << "File does not exist!" << endl;
		return;
	}

	bool flag = false;
	int position = 0;
	int amount = 0;
	int fileSize = boost::filesystem::file_size(path);
	cout << "File Size: " << fileSize << endl;

	while (!flag)
	{
		cout << "Enter Position: ";
		position = getInt();
		if (validPosition(position , fileSize))
		{
			flag = true;
		}
		else
		{
			cout << "Invalid Position!" << endl;
		}
	}
	flag = false;
	
	while (!flag)
	{
		cout << "Enter Amount: ";
		amount = getInt();
		if (validRemoveAmount(position,amount ,fileSize))
		{
			flag = true;
		}
		else
		{
			cout << "Invalid Position!" << endl;
		}
	}
	remove(position, amount, path);
}


/// <summary>
/// function reads an entire file
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
string FileHandler::readWholeFile(string path)
{
	boost::filesystem::ifstream file(path, std::ios::in);
	string data = "";
	string temp;
	while (std::getline(file, temp))
	{
		data += temp;
		data += '\n';
	};
	data[data.size() - 1] = '\0';
	file.close();
	return data;
}

/// <summary>
/// function inputing a path
/// </summary>
/// <returns></returns>
string FileHandler::getPath()
{
	string path;
	cout << "Enter Path: ";
	cin >> path;
	return path;
}

/// <summary>
/// function checking if position of the file is valid
/// </summary>
/// <param name="position"></param>
/// <param name="fileSize"></param>
/// <returns></returns>
bool FileHandler::validPosition(const int position , const int fileSize)
{
	return (position < fileSize&& position >= 0);
}


/// <summary>
/// function checking if amount of characters to remove is valid
/// </summary>
/// <param name="position"></param>
/// <param name="amount"></param>
/// <param name="fileSize"></param>
/// <returns></returns>
bool FileHandler::validRemoveAmount(const int position , const int amount, const int fileSize)
{
	return (amount > 0 && amount <= fileSize - position);
}


/// <summary>
/// insert subfunction
/// </summary>
/// <param name="location"></param>
/// <param name="content"></param>
/// <param name="path"></param>
void FileHandler::insert(const int location , const string content , string path)
{
	string fileData = readWholeFile(path);

	string tmp1 = fileData.substr(0, location);
	string tmp2 = fileData.substr(location, fileData.size() - 1);

	boost::filesystem::ofstream file(path, std::ios::out);
	file << tmp1;
	file << content;
	file << tmp2;

	file.close();
}


/// <summary>
/// remove subfunction
/// </summary>
/// <param name="location"></param>
/// <param name="removeAmount"></param>
/// <param name="path"></param>
void FileHandler::remove(const int location, const int removeAmount, string path)
{
	string fileData = readWholeFile(path);
	fileData.erase(location, removeAmount);

	boost::filesystem::ofstream file(path);
	file << fileData;
}
