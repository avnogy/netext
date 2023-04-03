#include "include/FileHandler.h"

FileHandler::FileHandler()
{
	
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
		filePath = getPath();
			switch (option)
			{
				case 1:
					createFile();
					break;
				case 2:
					deleteFile();
					break;
				case 3:
					insertIntoFile();
					break;
				case 4:
					removeFromFile();
					break;
				default:
					break;
			}
		option = 0;
	}
}


/// <summary>
/// function inserting request into the queue
/// </summary>
/// <param name="request"></param>
void FileHandler::insertRequest(json request)
{
	muRequests.lock();
	editRequests.push(request);
	muRequests.unlock();
}

void FileHandler::test()
{
	std::time_t time = std::time(nullptr);
	json r;
	json r2;
	json r3;

	FileHandler::getInstance().setPath("test.txt");

	r["requestCode"] = FILE_INSERT_REQUEST;
	r["timeStamp"] = time;
	r["data"] = { {"position" , 0} , {"content" , "Hello"}};

	Sleep(2000);

	time = std::time(nullptr);
	r2["requestCode"] = FILE_REMOVE_REQUEST;
	r2["timeStamp"] = time;
	r2["data"] = { {"position" , 1} , {"amount" , 1}};

	Sleep(2000);

	r3["requestCode"] = FILE_INSERT_REQUEST;
	r3["timeStamp"] = time;
	r3["data"] = { {"position" ,4} , {"content" , " My name is yahel. :0"}};

	insertRequest(r3);
	insertRequest(r2);
	insertRequest(r);

	while (!editRequests.empty())
	{
		handleRequests();
	}
}

/// <summary>
/// set function for file path field
/// </summary>
/// <param name="path"></param>
void FileHandler::setPath(string path)
{
	filePath = path;
}


/// <summary>
/// function creates a file 
/// </summary>
/// <param name="path"></param>
void FileHandler::createFile()
{
	if (!boost::filesystem::exists(filePath))
	{
		boost::filesystem::ofstream file(filePath);
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
void FileHandler::deleteFile()
{
	if (boost::filesystem::exists(filePath))
	{
		boost::filesystem::remove(filePath.c_str());
	}
	else
	{
		cout << "File with this path does not exist!";
	}
}





/// <summary>
/// function inserts data into existing file (appending)
/// </summary>
/// <param name="path"></param>
void FileHandler::insertIntoFile()
{
	if (!boost::filesystem::exists(filePath))
	{
		cout << "File does not exist!" << endl;
		return;
	}

	int position = 0;
	string data = "";

	cout << "File Size: " << boost::filesystem::file_size(filePath) << endl;

	cout << "Enter Position: ";
	position = getInt();

	cout << "Enter Data To Insert: ";
	cin >> data;

	insert(position, data);
}


/// <summary>
/// function removing data from a file
/// </summary>
/// <param name="path"></param>
void FileHandler::removeFromFile()
{
	if (!boost::filesystem::exists(filePath))
	{
		cout << "File does not exist!" << endl;
		return;
	}

	bool flag = false;
	int position = 0;
	int amount = 0;
	int fileSize = boost::filesystem::file_size(filePath);
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
	remove(position, amount);
}


/// <summary>
/// functuon handling requests from the queue (thread)
/// </summary>
void FileHandler::handleRequests()
{
	while (true)
	{

		muRequests.lock();
		if (!editRequests.empty())
		{
			json request = editRequests.top();
			editRequests.pop();
			RequestCode id = (RequestCode)request["requestCode"];
			json data = request["data"];
			try
			{
				switch (id)
				{
					case FILE_INSERT_REQUEST:

						insert(data["position"], data["content"]);
						break;

					case FILE_REMOVE_REQUEST:
						remove(data["position"], data["amount"]);
						break;
					
					default:
						break;
				}
			}
			catch (std::exception& e)
			{
				cout << "Error: " << e.what() << endl;
			}
		}
		muRequests.unlock();
	}
		


}

/// <summary>
/// function reads an entire file
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
string FileHandler::readWholeFile()
{
	boost::filesystem::fstream file(filePath);
	string data((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	file.close();
	
	return data;
}


/// <summary>
/// function gets file size
/// </summary>
/// <returns></returns>
int FileHandler::getFileSize()
{
	return boost::filesystem::file_size(filePath);
}



/// <summary>
/// function checking if position of the file is valid
/// </summary>
/// <param name="position"></param>
/// <param name="fileSize"></param>
/// <returns></returns>
bool FileHandler::validPosition(const int position , const int fileSize)
{
	return (position <= fileSize && position >= 0);
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
void FileHandler::insert(const int location , const string content)
{
	if (!boost::filesystem::exists(filePath))
	{
		throw MyException("File does not exist");
	}
	if (!validPosition(location, getFileSize()))
	{
		throw MyException("Position is not valid");
	}
	string fileData = readWholeFile();

	string tmp1 = fileData.substr(0, location);
	string tmp2 = fileData.substr(location, fileData.size() - 1);

	boost::filesystem::ofstream file(filePath, std::ios::out);
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
void FileHandler::remove(const int position, const int removeAmount)
{
	if (!boost::filesystem::exists(filePath))
	{
		throw MyException("File does not exist");
	}
	if (!validPosition(position, getFileSize()))
	{
		throw MyException("Position is not valid");
	}
	if (!validRemoveAmount(position, removeAmount, getFileSize()))
	{
		throw MyException("Remove Amount is not valid");
	}
	string fileData = readWholeFile();
	fileData.erase(position, removeAmount);

	boost::filesystem::ofstream file(filePath);
	file << fileData;
	file.close();
}
