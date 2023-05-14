#include "include/Notifier.h"

Notifier::Notifier()
{
}

void Notifier::insert(const UdpPacket event)
{
	unique_lock<mutex> lck(muEvents);
	events.push(event);
	lck.unlock();
	cvEvents.notify_all();
}

bool Notifier::notify()
{
	//TODO: add stop condition, error handling
	while (true)
	{
		unique_lock<mutex> lck(muEvents);

		if (events.empty())
		{
			cvEvents.wait(lck);
		}

		UdpPacket eventPacket = events.top();
		switch (eventPacket.type)
		{
		case Code::FILE_INSERT_REQUEST:
			eventPacket.type = Code::FILE_INSERT_RESPONSE;
			break;
		case Code::FILE_REMOVE_REQUEST:
			eventPacket.type = Code::FILE_REMOVE_RESPONSE;
			break;
		default:
			break;
		}
		const string event = Network::serializeRequest(eventPacket);
		events.pop();
		for (auto& endpoint : clients)
		{
			if (Network::notify(endpoint, event))
			{
				cout << "notified " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
			}
			else
			{
				cerr << "Error: faild to notify " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
			}
		}
		lck.unlock();
	}
}

void Notifier::addClient(ip::udp::endpoint& client)
{
	lock_guard<mutex> lck(muEvents);
	clients.push_back(client);
}

void Notifier::removeClient(ip::udp::endpoint& client)
{
	lock_guard<mutex> lck(muEvents);
	auto position = std::find(clients.begin(), clients.end(), client);
	if (position != clients.end())
	{
		clients.erase(position);
	}
}