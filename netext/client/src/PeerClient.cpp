#include "include/PeerClient.h"

/// <summary>
/// inputing session details and checking with central server if information is valid to enter
/// </summary>
void PeerClient::joinSession()
{
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

	ip::udp::endpoint frontend = Network::acceptFrontend();
	thread frontendThread(boost::bind(&PeerClient::session, frontend));
	thread redirectThread(boost::bind(&PeerClient::redirect, host, frontend));

	hostThread.join();
	redirectThread.interrupt();
	frontendThread.join();
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