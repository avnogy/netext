#include "include/PeerClient.h"

/// <summary>
/// inputing session details and checking with central server if information is valid to enter
/// </summary>
void PeerClient::joinSession()
{
	ip::udp::endpoint frontend = Network::acceptFrontend();
	//Notifier::getInstance().addClient(frontend);
	thread frontendThread(boost::bind(&PeerClient::session, frontend));
	//Notifier::getInstance().removeClient(frontend);

	string key;
	cout << "Enter Session key: ";
	cin >> key;

	json jsonData = {
		{"key", key},
		{"localIp", Network::getLocalIP()}
	};

	json peerInfo = Network::joinSession(Network::serializeRequest(Code::JOIN_SESSION_REQUEST, time(TIME_NOW), jsonData));

	ip::udp::endpoint host = Network::punchHole(peerInfo);
	thread hostThread(boost::bind(&PeerClient::session, host));
	thread redirectThread(boost::bind(&PeerClient::redirect, host, frontend));

	hostThread.join();
	redirectThread.interrupt();
	frontendThread.join();
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
void PeerClient::startHandleRequests(ip::udp::endpoint peer)
{
	cout << "Host found!" << endl;
	try
	{
		//// TODO:
		//	 remove the receiver thread and make the sending work without the menu getting in the way.

		// Creating a sender thread
		boost::thread sender_thread(boost::bind(&Network::sendMessage, boost::ref(Network::sock), peer));

		// Creating a receiver thread
		//boost::thread receiver_thread(boost::bind(&Network::receiveMessage, boost::ref(Network::sock)));

		sender_thread.join();

		// TO DO: File Update Requests
	}
	catch (std::exception& e)
	{
		cerr << "Client Disconnected." << endl;
	}
}

/// <summary>
/// handling client requests
/// </summary>
/// <param name="client_sock"></param>
TRY_CATCH_FUNCTION(void, PeerClient::session, (ip::udp::endpoint peer), "Client Disconnected.",
	{
		cout << "Client accepted!" << endl;
		UdpPacketQueue::getInstance().PopForRequirements(Code::CLIENT_LEAVE_REQUEST, peer);
		cout << "Client Disconnected." << endl;
	}, {
		return;
	});

TRY_CATCH_LOOP_FUNCTION(void, PeerClient::redirect, (ip::udp::endpoint host, ip::udp::endpoint frontend), "error while redirecting",
	{
		frontendThread = thread(boost::bind(&redirectFrontend, host, frontend));
		hostThread = thread(boost::bind(&redirectHost, host, frontend));
		boost::this_thread::interruption_point();
		frontendThread.join();
		hostThread.join();
	}, {
		frontendThread.interrupt();
		hostThread.interrupt();
		UdpPacketQueue::getInstance()._cv.notify_all();
		frontendThread.join();
		hostThread.join();
	},
		thread frontendThread; thread hostThread;
	);