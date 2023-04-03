#include "include/Notifier.h"

bool Notifier::insert(const json event)
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
			const string event = events.top().dump();
			events.pop();
			for (auto& endpoint : clients)
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
		lck.unlock();
		cvEvents.wait(lck);
	}

}
