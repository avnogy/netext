#pragma once
#include "Utilities.h"

struct UdpPacket {
	ip::udp::endpoint endpoint;
	Timestamp timestamp{};
	Code type{};
	json data;
};

class UdpPacketQueue
{
public:
	static UdpPacketQueue& getInstance()
	{
		static UdpPacketQueue instance;
		return instance;
	}
	void Push(UdpPacket packet);

	UdpPacket PopForRequirements(const Code type);
	UdpPacket PopForRequirements(const Code type, ip::udp::endpoint);

	UdpPacketQueue(const UdpPacketQueue&) = delete;
	UdpPacketQueue& operator=(const UdpPacketQueue&) = delete;
private:
	UdpPacketQueue();
	~UdpPacketQueue();
	queue<UdpPacket> _queue;
	mutex _mutex;
	condition_variable _cv;
};

void UdpReceiverThread();

UdpPacket deserialize(char buffer[], const size_t recv_len);