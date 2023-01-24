
#include "Helper.h"



/*
	Function: sending a buffer of bytes to another client 
	input: client socket (SOCKET) and a buffer (Buffer)
	output: none
*/
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



/*
	Function: getting a fully  data from a client 
	input: client socket (SOCKET)
	output: the data (Buffer)
*/
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


/*
	Function: receiving data from a client using socket
	input: client socket (SOCKET) , number of bytes to get
	output: the data (Byte*)
*/
Byte* Helper::readFromClient(SOCKET client_sock, int numOfBytes)
{
	char* result = new char[numOfBytes];
	int res = recv(client_sock, result, numOfBytes, 0);
	if (res == INVALID_SOCKET)
	{
		std::string err = "Error while recieving from socket: ";
		err += std::to_string(client_sock);
		throw MyException(err);
	}
	return (Byte*)result;
}



/*
	Function: getting the request/response id from a client (acording to protocol)
	input: client socket (SOCKET)
	output: a single byte representing id (Byte)
*/
Byte Helper::getId(SOCKET client_sock)
{
	Byte* idBuff = (readFromClient(client_sock, 1));
	Byte id = *idBuff;
	return id;
}



/*
	Function: getting the data length as 4 bytes (acording to protocol)
	input: client socket (SOCKET)
	output: the length (int)
*/
int Helper::getDataLength(SOCKET client_sock)
{
	Byte* lengthBuff = readFromClient(client_sock, 4);
	int dataLength = lengthBuff[0] << 24 | lengthBuff[1] << 16 | lengthBuff[2] << 24 | lengthBuff[3]; // using bitwise operators cause length is 4 bytes
	return dataLength;
}



/*
	Function: getting the data chunk from a client message (acording to protocol)
	input: client socket (SOCKET) and the data length (int)
	output: the data (Byte*)
*/
Byte* Helper::getData(SOCKET client_sock, int dataLength)
{
	Byte* data = readFromClient(client_sock, dataLength);
	return data;
}


/*
	Function: casting an id to an requestId enum
	input: id (Byte)
	output: request id (enum - RequestId)
*/
RequestId Helper::getRequestId(Byte id)
{
	return (RequestId)(id);
}



/*
	Function: using an id and a data to create a full message buffer (acording to protocol)
	input: the id (int) and the data (string)
	output: the whole buffer (Buffer)
*/
Buffer Helper::createLoadedBuffer(int id, string data)
{
	Buffer buff;

	int dataLength = data.size();
	buff.push_back((Byte)id); // id - 1 byte
	Buffer dataLenBuff = intTo4Bytes(dataLength + 1);
	std::copy(dataLenBuff.begin(), dataLenBuff.end(), std::back_inserter(buff)); // data length - 4 bytes
	std::copy(data.begin(), data.end(), std::back_inserter(buff)); // data - N bytes
	
	return buff;
}



/*
	Function: converting a number to a 4 byte buffer
	input: the number (int)
	output: the 4 byte number (Buffer)
*/
Buffer Helper::intTo4Bytes(int num)
{
	Buffer bytes(4);
	int i = 0;
	for (i = 0; i < 4; i++)
	{
		bytes[3 - i] = (num << (i * 8)); // using bitwise operator (shift right) to set each byte
	}
	return bytes;
}


/*
	Function: creating a socket with the central server
	input: none
	output: the binded socket (SOCKET)
*/
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
