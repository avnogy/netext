#include "Utilities.h"

/// <summary>
///  sending data to another client through socket
/// </summary>
/// <param name="client_sock"></param>
/// <param name="data"></param>
void Helper::sendDataToClient(tcp::socket& client_sock, string data)
{
	boost::asio::write(client_sock, boost::asio::buffer(data));
}



/// <summary>
/// receiving data from a client using socket
/// </summary>
/// <param name="client_sock"></param>
/// <returns></returns>
string Helper::receiveDataFromClient(tcp::socket& client_sock)
{
	char buff[BUFSIZE];
	size_t dataLength = boost::asio::read(client_sock, boost::asio::buffer(buff));
	buff[dataLength] = '\0';
	return string(buff);
}


string Helper::createDataRequestMessage(int code, json requestData)
{
	time_t timeNow = time(TIME_NOW);
	RequestMessage msg = { code , timeNow , requestData };
	string msgData = SerializeRequest(msg);
	return msgData;
}


tcp::socket Helper::createCentralServerSocket()
{
	boost::asio::io_context ioc;
	tcp::socket centralSock(ioc);
	
	tcp::endpoint centralEndpoint(boost::asio::ip::address::from_string(CENTRAL_IP), CENTRAL_PORT);
	centralSock.connect(centralEndpoint);

	return centralSock;
}
