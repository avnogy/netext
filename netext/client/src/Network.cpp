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

string Network::CreateSession()
{
    sock.send_to(boost::asio::buffer(Network::getLocalIP()), rendezvous);
    size_t recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    if (recv_len <= 0)
    {
        throw MyException("Error in creating session, No response.");
    }
    buffer[recv_len] = '\0';

    json data = json::parse(string(buffer));
    if (data["code"] != ResponseCode::CREATE_SESSION_RESPONSE)
    {
        throw MyException("Error in creating session, Got wrong code.");
    }

    return data["data"]["key"];
}

json Network::getPeerInfo()
{
    json peerInfo;
    size_t recv_len = sock.receive_from(boost::asio::buffer(buffer), senderEndpoint);
    if (recv_len <= 0)
    {
        throw std::exception("error while getting peer info.");
        return FAILURE;
    }
    cout << buffer << endl;
    peerInfo = nlohmann::json::parse(buffer);
    return SUCCESS;
}

void Network::printPeerInfo(const json peerInfo)
{
    cout << "got peer" << endl;
    cout << " source ip: " << peerInfo["source_ip"] << endl;
    cout << " dest ip: " << peerInfo["dest_ip"] << endl;
    cout << " source port: " << peerInfo["source_port"] << endl;
    cout << " dest port: " << peerInfo["dest_port"] << endl;
}

ip::udp::endpoint Network::punchHole(const json peerInfo)
{
    ip::udp::endpoint peer(boost::asio::ip::address::from_string(peerInfo["source_ip"]), peerInfo["source_port"]);
    sock.send_to(boost::asio::buffer("ready"), peer);
    return peer;
}

/// <summary>
/// test thread to recieve messages, content will be used later for actual recieving.
/// </summary>
/// <param name="sock"></param>
void Network::receiveMessage(ip::udp::socket& sock)
{
    char buffer[BUFSIZE];
    try
    {
        boost::asio::ip::udp::endpoint sender_endpoint;

        while (true)
        {
            size_t recv_len = sock.receive_from(boost::asio::buffer(buffer), sender_endpoint);
            if (recv_len > 0)
            {
                buffer[recv_len] = '\0';
                cout << endl << "peer: " << buffer << endl;
            }
            else
            {
                throw std::exception("connection closed.");
            }
        }
    }
    catch (const std::exception& e)
    {
        cerr << "Error receiving message: " << e.what() << endl;
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
            std::getline(std::cin, message);
            sock.send_to(boost::asio::buffer(message), peer);
        }
    }
    catch (const std::exception& e)
    {
        cerr << "Error receiving message: " << e.what() << endl;
    }
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
        io_service io_service;
        ip::tcp::resolver resolver(io_service);
        ip::tcp::resolver::query query(ip::host_name(), "");
        ip::tcp::resolver::iterator iterator = resolver.resolve(query);

        while (iterator != ip::tcp::resolver::iterator())
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
