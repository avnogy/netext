#include "include/Network.h"

io_context context;
ip::udp::socket Network::sock = ip::udp::socket(context, ip::udp::endpoint(ip::udp::v4(), AUTO_SRC_PORT));
ip::udp::endpoint Network::rendezvous = ip::udp::endpoint(ip::address::from_string(SERVER_IP), SERVER_PORT);
ip::udp::endpoint Network::senderEndpoint = ip::udp::endpoint();

string Network::createSession(const string request)
{
	sock.send_to(boost::asio::buffer(request), rendezvous);
	return UdpPacketQueue::getInstance().PopForRequirements(Code::CREATE_SESSION_RESPONSE, rendezvous).data["key"];
}

json Network::joinSession(const string request)
{
	sock.send_to(boost::asio::buffer(request), rendezvous);
	return UdpPacketQueue::getInstance().PopForRequirements(Code::JOIN_SESSION_RESPONSE, rendezvous).data;
}

void Network::deleteSession(const string request)
{
	sock.send_to(boost::asio::buffer(request), rendezvous);
	UdpPacketQueue::getInstance().PopForRequirements(Code::DELETE_SESSION_RESPONSE, rendezvous);
}

UdpPacket Network::getPeerInfo()
{
	return UdpPacketQueue::getInstance().PopForRequirements(Code::PEER_INFO_RESPONSE, rendezvous);
}

void Network::printPeerInfo(const json peerInfo)
{
	cout << "got peer" << endl;
	cout << " ip: " << peerInfo["ip"] << endl;
	cout << " port: " << peerInfo["port"] << endl;
}

ip::udp::endpoint Network::punchHole(const json peerInfo)
{
	ip::udp::endpoint peer(ip::address::from_string(peerInfo["ip"]), peerInfo["port"]);
	sock.send_to(boost::asio::buffer(Network::serializeRequest(Code::PUNCH_HOLE_PACKET, time(TIME_NOW), {})), peer);
	return peer;
}

ip::udp::endpoint Network::acceptFrontend()
{
	json jsonData;

	ip::udp::endpoint frontend = UdpPacketQueue::getInstance().PopForRequirements(Code::FRONTEND_SESSION_JOIN).endpoint;
	jsonData = { frontend.address().to_string(),frontend.port() };
	sock.send_to(boost::asio::buffer(Network::serializeRequest(Code::FRONTEND_SESSION_JOIN, time(TIME_NOW), jsonData)), frontend);
	return frontend;
}

/// <summary>
/// test thread to receive messages, content will be used later for actual recieving.
/// </summary>
/// <param name="sock"></param>
string Network::receiveMessage(ip::udp::socket& sock)
{
	char buffer[BUFSIZE];

	ip::udp::endpoint sender_endpoint;
	size_t recv_len = sock.receive_from(boost::asio::buffer(buffer), sender_endpoint);
	cout << "sender: " << sender_endpoint.address().to_string() << endl;
	if (recv_len > 0)
	{
		buffer[recv_len] = '\0';
		return string(buffer);
	}
	else
	{
		throw runtime_error("Connection closed.");
	}
}

/// <summary>
/// test thread to send messages, content will be used later for actual sending.
/// </summary>
/// <param name="sock"></param>
/// <param name="peer"></param>
void Network::sendMessage(ip::udp::socket& sock, ip::udp::endpoint& peer)
{
	try
	{
		while (true)
		{
			string message;
			cout << "you: ";
			cin >> message;
			sock.send_to(boost::asio::buffer(message), peer);
		}
	}
	catch (const std::exception& e)
	{
		cerr << "Error receiving message: " << e.what() << endl;
	}
}

bool Network::notify(ip::udp::endpoint recipient, const string message)
{
	sock.send_to(boost::asio::buffer(message), recipient);
	return true;
}

string Network::serializeRequest(const UdpPacket packet)
{
	json requestJson;

	requestJson["code"] = packet.type;
	requestJson["time"] = packet.timestamp;
	requestJson["data"] = packet.data;
	if (!requestJson["data"].contains("endpoint"))
	{
		cout << "a";
		requestJson["data"]["endpoint"] = { packet.endpoint.address().to_string(),packet.endpoint.port() };
	}
	cout << "b" << endl;

	return requestJson.dump();
}

string Network::serializeRequest(const Code code, const Timestamp time, const json requestData)
{
	json requestJson;

	requestJson["code"] = code;
	requestJson["time"] = time;
	requestJson["data"] = requestData;

	return requestJson.dump();
}

/// <summary>
/// function to get the local ip address of the machine
/// </summary>
/// <returns></returns>
string Network::getLocalIP()
{
	string local_ip;
	try
	{
		io_context io_context;
		ip::udp::resolver resolver(io_context);
		ip::udp::resolver::query query(ip::host_name(), "");
		ip::udp::resolver::iterator iterator = resolver.resolve(query);

		while (iterator != ip::udp::resolver::iterator())
		{
			ip::address addr = (iterator++)->endpoint().address();
			if (addr.is_v4())
			{
				return addr.to_string();
			}
		}
	}
	catch (std::exception& e)
	{
		cout << "Error: " << e.what() << endl;
	}
	cerr << "Error retrieving local ip address." << endl;
	cout << "If computers are under the same network please write the local ip address." << endl << "Local: ";
	cin >> local_ip;
	return local_ip;
}

void Network::writePortToFile()
{
	FileHandler& handler = FileHandler::getInstance();
	handler.setPath(PORT_FILE_PATH);

	ip::udp::endpoint endpoint = sock.local_endpoint();
	int port = endpoint.port();
	//cout << port << endl;

	handler.writeToFile(std::to_string(port));
}