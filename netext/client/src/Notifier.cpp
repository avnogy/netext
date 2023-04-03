#include "include/Notifier.h"

bool Notifier::insert(const json event)
{
	muEvents.lock();
	events.push(event);
	muEvents.unlock();
}

bool Notifier::notify()
{
	muEvents.lock();
	while (!events.empty())
	{
		const string event = events.top().dump();
		events.pop();
		for (auto& endpoint: clients) 
		{
			if (Network::notify(endpoint, event))
			{
				cerr << "notified " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
			}
			{
				cerr << "Error: faild to notify " << endpoint.address().to_string() << ":" << endpoint.port() << endl;
			}
		}
	}
	muEvents.unlock();

}
