#include "include/Network.h"

//bool Network::init()
//{
//    io_context context;
//    Network::sock = ip::udp::socket(context, ip::udp::endpoint(ip::udp::v4(), AUTO_SRC_PORT));
//    Network::rendezvous = ip::udp::endpoint(ip::address::from_string(SERVER_IP), SERVER_PORT);
//    Network::senderEndpoint = ip::udp::endpoint();
//    return true;
//}

io_context context;
ip::udp::socket Network::sock = ip::udp::socket(context, ip::udp::endpoint(ip::udp::v4(), AUTO_SRC_PORT));
ip::udp::endpoint Network::rendezvous = ip::udp::endpoint(ip::address::from_string(SERVER_IP), SERVER_PORT);
ip::udp::endpoint Network::senderEndpoint = ip::udp::endpoint();
char Network::buffer[BUFSIZE];

string Network::createSession(const string request)
{
    size_t recv_len = 0;

    sock.send_to(boost::asio::buffer(request), rendezvous);
    do
    {
        recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    } while (senderEndpoint != rendezvous);
    if (recv_len <= 0)
    {
        throw MyException("Error while creating session, No response.");
    }
    buffer[recv_len] = '\0';

    json data = json::parse(string(buffer));
    if (data["code"] != ResponseCode::CREATE_SESSION_RESPONSE)
    {
        throw MyException("Error while creating session, Got wrong code.");
    }

    return data["data"]["key"];
}

json Network::joinSession(const string request)
{
    size_t recv_len = 0; 

    sock.send_to(boost::asio::buffer(request), rendezvous);
    do
    {
        recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    } while (senderEndpoint != rendezvous);
    if (recv_len <= 0)
    {
        throw MyException("Error while joining session, No response.");
    }
    buffer[recv_len] = '\0';

    json data = json::parse(string(buffer));
    if (data["code"] != ResponseCode::JOIN_SESSION_RESPONSE)
    {
        throw MyException("Error while joining session, Got wrong code.");
    }
    return data["data"];
}

void Network::deleteSession(const string request)
{
    size_t recv_len = 0;

    sock.send_to(boost::asio::buffer(request), rendezvous);
    do
    {
        recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    } while (senderEndpoint != rendezvous);
    if (recv_len <= 0)
    {
        throw MyException("Error while deleting session, No response.");
    }
    buffer[recv_len] = '\0';

    json data = json::parse(string(buffer));
    if (data["code"] != ResponseCode::DELETE_SESSION_RESPONSE)
    {
        throw MyException("Error while deleting session, Got wrong code.");
    }
}

json Network::getPeerInfo()
{
    size_t recv_len = 0;
    do
    {
        recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    } while (senderEndpoint != rendezvous);
    if (recv_len <= 0)
    {
        throw MyException("Error while getting peer info.");
        return FAILURE;
    }
    buffer[recv_len] = '\0';

    try
    {
        json peerInfo = json::parse(buffer);

        if (peerInfo["code"] != ResponseCode::PEER_INFO_RESPONSE)
        {
            throw MyException("Error while getting session information, Got wrong code.");
        }
        return peerInfo;
    }
    catch (const std::exception&)
    {
        std::ostringstream message;
        message << "Error while getting session information, Couldnt parse message." << endl << "data: " << buffer << endl;
        throw MyException(message.str());
    }
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
    sock.send_to(boost::asio::buffer("ready"), peer);
    return peer;
}

ip::udp::endpoint Network::acceptFrontend()
{
    ip::udp::endpoint sender_endpoint;
    size_t recv_len = sock.receive_from(boost::asio::buffer(buffer), sender_endpoint);
    return sender_endpoint;
}

/// <summary>
/// test thread to recieve messages, content will be used later for actual recieving.
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

        throw MyException("Connection closed.");
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

string Network::serializeRequest(const int code, const Timestamp time, const json requestData)
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

