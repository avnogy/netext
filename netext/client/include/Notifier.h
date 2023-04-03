#pragma once
#include "Utilities.h"

class Notifier
{
public:
	static Notifier& getInstance() 
	{
		static Notifier instance;
		return instance;
	}

	Notifier(const Notifier&) = delete;
	Notifier& operator=(const Notifier&) = delete;

	bool insert(const json event);
	bool notify();

private:
	Notifier();
	priority_queue<json, vector<json>, CompareJsonByTimestamp> events;
	vector<ip::udp::endpoint> clients;
	mutex muEvents;
};

