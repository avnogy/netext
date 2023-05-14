#include "include/ProxyHandler.h"

/// <summary>
/// functuon handling requests from the peer-host
/// </summary>
TRY_CATCH_LOOP_FUNCTION(void, redirectHost, (ip::udp::endpoint host, ip::udp::endpoint frontend), "Error while redirecting from host",
	{
		const UdpPacket request = UdpPacketQueue::getInstance().PopForRequirements(host);
		if (request.type == Code::FILE_INSERT_RESPONSE || request.type == Code::FILE_REMOVE_RESPONSE)
		{
			Network::notify(frontend, Network::serializeRequest(request));
		}
	}, {
		return;
	});

/// <summary>
/// functuon handling requests from the frontend
/// </summary>
TRY_CATCH_LOOP_FUNCTION(void, redirectFrontend, (ip::udp::endpoint host, ip::udp::endpoint frontend), "Error while redirecting from frontend",
	{
			cout << "got here" << endl;
			cout << frontend.address().to_string() << ":" << frontend.port() << endl;
			const UdpPacket request = UdpPacketQueue::getInstance().PopForRequirements(frontend);
			cout << "packet from frontend:" << request.data << endl;
			if (request.type == Code::FILE_INSERT_REQUEST || request.type == Code::FILE_REMOVE_REQUEST)
			{
				cout << "notified.." << endl;
				Network::notify(host, Network::serializeRequest(request));
			}
	}, {
		return;
	});