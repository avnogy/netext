#include "Utilities.h"

/// <summary>
/// sending a buffer of bytes to another client 
/// </summary>
/// <param name="client_socket"></param>
/// <param name="buffer"></param>
void Helper::sendBufferToClient(SOCKET client_sock, Buffer buff)
{
	char* data = new char[buff.size()];
	int i = 0;
	for (i = 0; i < buff.size(); i++)
	{
		data[i] = buff[i];
	}

	if (send(client_sock, data, buff.size(), 0) == INVALID_SOCKET)
	{
		throw std::exception("Error while sending message to client");
	}
	delete[] data;
}



/// <summary>
/// getting a fully  data from a client 
/// </summary>
/// <param name="client_sock"></param>
/// <returns>the received data</returns>
string Helper::getDataBufferFromClient(SOCKET client_sock)
{
	int dataLength = getDataLength(client_sock);
	char* data = getData(client_sock, dataLength);
	return string(data);
}


/// <summary>
/// receiving data from a client using socket
/// </summary>
/// <param name="client_socket"></param>
/// <param name="numOfBytes">: number of bytes to get</param>
/// <returns></returns>
char* Helper::readFromClient(SOCKET client_sock, int numOfBytes)
{
	char* result = new char[numOfBytes];
	int res = recv(client_sock, result, numOfBytes, 0);
	if (res == INVALID_SOCKET)
	{
		string err = "Error while recieving from socket: ";
		err += std::to_string(client_sock);
		throw MyException(err);
	}
	return result;
}





/// <summary>
/// getting the data length as 4 bytes acording to protocol
/// </summary>
/// <param name="client_sock"></param>
/// <returns>data length</returns>
int Helper::getDataLength(SOCKET client_sock)
{
	Byte* lengthBuff = (Byte*)readFromClient(client_sock, 4);
	int dataLength = lengthBuff[0] << 24 | lengthBuff[1] << 16 | lengthBuff[2] << 24 | lengthBuff[3]; // using bitwise operators cause length is 4 bytes
	return dataLength;
}


/// <summary>
/// getting the data chunk from a client message acording to protocol
/// </summary>
/// <param name="client_sock"></param>
/// <param name="dataLength"></param>
/// <returns>data</returns>
char* Helper::getData(SOCKET client_sock, int dataLength)
{
	char* data = readFromClient(client_sock, dataLength);
	return data;
}



/// <summary>
/// using an id and a data to create a full message buffer acording to protocol
/// </summary>
/// <param name="id"></param>
/// <param name="data"></param>
/// <returns>the whole buffer</returns>
Buffer Helper::createLoadedBuffer(int id, string data)
{
	Buffer buff;
	int dataLength = data.size();
	Buffer dataLenBuff = intTo4Bytes(dataLength + 1);
	std::copy(dataLenBuff.begin(), dataLenBuff.end(), std::back_inserter(buff)); // data length - 4 bytes
	std::copy(data.begin(), data.end(), std::back_inserter(buff)); // data - N bytes
	return buff;
}

/// <summary>
/// converting a number to a 4 byte buffer
/// </summary>
/// <param name="num"></param>
/// <returns>the 4 byte number </returns>
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


/// <summary>
/// creating a socket with the central server
/// </summary>
/// <returns>the binded socket</returns>
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
