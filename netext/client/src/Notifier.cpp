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
	//TODO: add stop condition
	while (true)
	{
		unique_lock<mutex> lck(muEvents);
		while (!events.empty())
		{
			const string event = Network::serializeRequest(events.top());
			events.pop();
			for (auto& endpoint : clients)
			{
				if (Network::notify(endpoint, event))
				{
					cout << "notified " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
				}
				{
					cerr << "Error: faild to notify " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
				}
			}
		}
		lck.unlock();
		cvEvents.wait(lck);
	}
}

void Notifier::addClient(ip::udp::endpoint& client)
{
	unique_lock<mutex> lck(muEvents);
	clients.push_back(client);
	lck.unlock();
}