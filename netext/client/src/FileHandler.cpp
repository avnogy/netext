#include "include/FileHandler.h"





FileHandler::FileHandler()
{
	Menu();
}

FileHandler::~FileHandler()
{
}

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

void FileHandler::insertIntoFile(string path)
{
	int position = 0;
	string data = "";

	cout << "Position: ";
	cin >> position;

	cout << "Data To Insert: ";
	cin >> data;

	insert(position, data, path);
}

void FileHandler::removeFromFile(string path)
{
	int position = 0;
	int amount = 0;

	cout << "Position: ";
	cin >> position;

	cout << "Amount: ";
	cin >> amount;

	remove(position, amount, path);
}

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

string FileHandler::getPath()
{
	string path;
	cout << "Enter Path: ";
	cin >> path;
	return path;
}



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

void FileHandler::remove(const int location, const int removeAmount, string path)
{
	string fileData = readWholeFile(path);
	fileData.erase(location, removeAmount);

	boost::filesystem::ofstream file(path);
	file << fileData;
}
