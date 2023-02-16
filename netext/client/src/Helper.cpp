#include "include/Utilities.h"

/// <summary>
///  sending data to another client through socket
/// </summary>
/// <param name="client_sock"></param>
/// <param name="data"></param>
void Helper::sendDataToClient(tcp::socket& client_sock, const string& data)
{
	if (!client_sock.is_open()) {
		throw MyException("Write Function: Socket is not open");
	}
	const string msg = data + "\n";
	boost::asio::write(client_sock, boost::asio::buffer(msg));
}



/// <summary>
/// receiving data from a client using socket
/// </summary>
/// <param name="client_sock"></param>
/// <returns></returns>
string Helper::receiveDataFromClient(tcp::socket& client_sock)
{
	if (!client_sock.is_open()) {
		throw MyException("Write Function: Socket is not open");
	}
	string data = "";
	boost::asio::read_until(client_sock, boost::asio::dynamic_buffer(data), "\n");
	return data;
}

/// <summary>
/// generating a port that is not in use
/// </summary>
/// <returns></returns>
int Helper::generatePort()
{
	int port = 0;

	srand(time(nullptr));

	do
	{
		port = (rand() % (HIGHEST_PORT - LOWEST_PORT + 1)) + LOWEST_PORT;
	} while (portInUse(port));
	return port;
}

/// <summary>
/// checking if a port is already in use
/// </summary>
/// <param name="port"></param>
/// <returns></returns>
bool Helper::portInUse(int port)
{
	boost::asio::io_service ioc;
	tcp::acceptor a(ioc);

	boost::system::error_code ec;
	a.open(tcp::v4(), ec) || a.bind({ tcp::v4(), boost::asio::ip::port_type(port) }, ec);

	return (ec == boost::asio::error::address_in_use);
}

/// <summary>
/// creating a socket with the centralized server
/// </summary>
/// <returns></returns>
tcp::socket Helper::createCentralServerSocket()
{
	boost::asio::io_context ioc;
	tcp::socket centralSock(ioc);
	
	tcp::endpoint centralEndpoint(boost::asio::ip::address::from_string(CENTRAL_IP), CENTRAL_PORT);
	centralSock.connect(centralEndpoint);

	return centralSock;
}
