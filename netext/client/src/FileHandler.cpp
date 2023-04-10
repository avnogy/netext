#include "include/FileHandler.h"

FileHandler::FileHandler()
{
}

FileHandler::~FileHandler()
{
}

/// <summary>
/// function inserting request into the queue
/// </summary>
/// <param name="request"></param>
void FileHandler::insertRequest(UdpPacket request)
{
	unique_lock<mutex> lck(_muRequests);
	_editRequests.push(request);
	lck.unlock();
	_cvRequests.notify_all();
}

void FileHandler::writeToFile(string content)
{
	boost::filesystem::ofstream file(_path);
	file << content;
}

/// <summary>
/// set function for file path field
/// </summary>
/// <param name="path"></param>
void FileHandler::setPath(string path)
{
	_path = path;
	this->createFile();
}

/// <summary>
/// function creates a file
/// </summary>
/// <param name="path"></param>
void FileHandler::createFile()
{
	boost::filesystem::ofstream file(_path, std::ofstream::out | std::ofstream::trunc);
	file.close();
}

/// <summary>
/// function deletes a file (if exists)
/// </summary>
/// <param name="path"></param>
void FileHandler::deleteFile()
{
	if (boost::filesystem::exists(_path))
	{
		boost::filesystem::remove(_path.c_str());
	}
	else
	{
		cout << "File with this path does not exist!";
	}
}


/// <summary>
/// functuon handling requests from the queue (thread)
/// </summary>
void FileHandler::handleRequests()
{
	while (true)
	{
		unique_lock<mutex> lck(_muRequests);
		if (_editRequests.empty())
		{
			_cvRequests.wait(lck);
		}

		const UdpPacket request = _editRequests.top();
		Notifier::getInstance().insert(request);
		_editRequests.pop();
		json data = request.data;
		try
		{
			switch (request.type)
			{
			case Code::FILE_INSERT_REQUEST:

				insert(request.data["position"], request.data["content"]);
				break;

			case  Code::FILE_REMOVE_REQUEST:
				remove(request.data["position"], request.data["amount"]);
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
}

/// <summary>
/// function reads an entire file
/// </summary>
/// <param name="path"></param>
/// <returns></returns>
string FileHandler::readWholeFile()
{
	boost::filesystem::fstream file(_path);
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
	return boost::filesystem::file_size(_path);
}

/// <summary>
/// function checking if position of the file is valid
/// </summary>
/// <param name="position"></param>
/// <param name="fileSize"></param>
/// <returns></returns>
bool FileHandler::validPosition(const int position, const int fileSize)
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
bool FileHandler::validRemoveAmount(const int position, const int amount, const int fileSize)
{
	return (amount > 0 && amount <= fileSize - position + 1);
}

/// <summary>
/// insert subfunction
/// </summary>
/// <param name="location"></param>
/// <param name="content"></param>
/// <param name="path"></param>
void FileHandler::insert(const int location, const string content)
{
	if (!boost::filesystem::exists(_path))
	{
		throw runtime_error("File does not exist");
	}
	if (!validPosition(location, getFileSize()))
	{
		throw runtime_error("Position is not valid");
	}
	string fileData = readWholeFile();

	string tmp1 = fileData.substr(0, location);
	string tmp2 = fileData.substr(location, fileData.size() - 1);

	boost::filesystem::ofstream file(_path, std::ios::out);
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
	if (!boost::filesystem::exists(_path))
	{
		throw runtime_error("File does not exist");
	}
	if (!validPosition(position, getFileSize()))
	{
		throw runtime_error("Position is not valid");
	}
	if (!validRemoveAmount(position, removeAmount, getFileSize()))
	{
		throw runtime_error("Remove Amount is not valid");
	}
	string fileData = readWholeFile();
	fileData.erase(position, removeAmount);

	boost::filesystem::ofstream file(_path);
	file << fileData;
	file.close();
}