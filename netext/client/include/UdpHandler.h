#pragma once
#include "Utilities.h"

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
	UdpPacket PopForRequirements(const Code type1, const Code type2);
	UdpPacket PopForRequirements(const Code type, ip::udp::endpoint endpoint);
	UdpPacket PopForRequirements(ip::udp::endpoint endpoint);

	UdpPacketQueue(const UdpPacketQueue&) = delete;
	UdpPacketQueue& operator=(const UdpPacketQueue&) = delete;
	condition_variable _cv;
private:
	UdpPacketQueue();
	~UdpPacketQueue();
	queue<UdpPacket> _queue;
	mutex _mutex;
};

void UdpReceiverThread();

UdpPacket deserialize(char buffer[], const size_t recv_len);