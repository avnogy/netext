
#include "Helper.h"




void Helper::sendBufferToClient(SOCKET client_sock, Buffer buff)
{
	char* data = new char[buff.size()];
	int i = 0;

	data[0] = buff[0];

	std::cout << (int)data[0] << " ";

	for (i = 1; i < buff.size(); i++)
	{
		data[i] = buff[i];
	}
	std::cout << std::endl;
	if (send(client_sock, data, buff.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
	delete[] data;
}

Buffer Helper::getDataBufferFromClient(SOCKET client_sock)
{
	Buffer buff;
	int dataLength = getDataLength(client_sock);
	Byte* data = getData(client_sock, dataLength);
	int i = 0;
	for (i = 0; i < dataLength; i++)
	{
		buff.push_back(data[i]);
	}
	return buff;
}

Byte* Helper::readFromClient(SOCKET client_sock, int numOfBytes)
{
	char* result = new char[numOfBytes];
	int res = recv(client_sock, result, numOfBytes, 0);
	if (res == INVALID_SOCKET)
	{
		std::string err = "Error while recieving from socket: ";
		err += std::to_string(client_sock);
		throw std::exception(err.c_str());
	}
	return (Byte*)result;
}

Byte Helper::getId(SOCKET client_sock)
{
	Byte* idBuff = (readFromClient(client_sock, 1));
	Byte id = *idBuff;
	return id;
}

int Helper::getDataLength(SOCKET client_sock)
{
	Byte* lengthBuff = readFromClient(client_sock, 4);
	int dataLength = lengthBuff[0] << 24 | lengthBuff[1] << 16 | lengthBuff[2] << 24 | lengthBuff[3]; // using bitwise operators cause length is 4 bytes
	return dataLength;
}

Byte* Helper::getData(SOCKET client_sock, int dataLength)
{
	Byte* data = readFromClient(client_sock, dataLength);
	return data;
}

RequestId Helper::getRequestId(Byte id)
{
	return (RequestId)(id);
}

Buffer Helper::createLoadedBuffer(int id, string data)
{
	Buffer buff;

	int dataLength = data.size();
	buff.push_back((Byte)id); // id - 1 byte
	Buffer dataLenBuff = intToBytes(dataLength + 1);
	std::copy(dataLenBuff.begin(), dataLenBuff.end(), std::back_inserter(buff)); // data length - 4 bytes
	std::copy(data.begin(), data.end(), std::back_inserter(buff)); // data - N bytes
	
	return buff;
}

Buffer Helper::intToBytes(int num)
{
	Buffer bytes(4);
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		bytes[3 - i] = (num << (i * 8)); // using bitwise operator (shift right) to set each byte
	}
	return bytes;
}



SOCKET Helper::createCentralSocket()
{
	SOCKET centralSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	sockaddr_in serverAddr;
	memset((char*)&serverAddr, 0, sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	inet_pton(AF_INET, CENTRAL_IP, &serverAddr.sin_addr);
	serverAddr.sin_port = htons(CENTRAL_PORT);

	int status = connect(centralSock, (sockaddr*)&serverAddr, sizeof(serverAddr));
	if (status < 0)
	{
		throw MyException("Error: Failed to connect to the socket");
	}

	return centralSock;
}
