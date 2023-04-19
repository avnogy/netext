#include "include/UdpHandler.h"

void UdpPacketQueue::Push(UdpPacket packet)
{
	lock_guard<mutex> lock(_mutex);
	_queue.push(packet);
	_cv.notify_all();
}

UdpPacket UdpPacketQueue::PopForRequirements(Code type)
{
	while (true)
	{
		try
		{
			unique_lock<mutex> lock(_mutex);
			_cv.wait(lock, [this, type] {
				return !_queue.empty() &&
					_queue.front().type == type;
				});
			UdpPacket packet = _queue.front();
			_queue.pop();
			_cv.notify_all();
			return packet;
		}
		catch (const runtime_error& e)
		{
			cerr << "Caught " << e.what() << endl;
		}
	}
}

UdpPacket UdpPacketQueue::PopForRequirements(const Code type1, const Code type2)
{
	while (true)
	{
		try
		{
			unique_lock<mutex> lock(_mutex);
			_cv.wait(lock, [this, type1, type2] {
				return
					!_queue.empty() && (
						_queue.front().type == type1 ||
						_queue.front().type == type2
						);
				});
			UdpPacket packet = _queue.front();
			_queue.pop();
			_cv.notify_all();
			return packet;
		}
		catch (const runtime_error& e)
		{
			cerr << "Caught " << e.what() << endl;
		}
	}
}

UdpPacket UdpPacketQueue::PopForRequirements(const Code type, ip::udp::endpoint endpoint)
{
	while (true)
	{
		try
		{
			unique_lock<mutex> lock(_mutex);
			_cv.wait(lock, [this, type, endpoint] {
				return !_queue.empty() &&
					_queue.front().type == type &&
					_queue.front().endpoint == endpoint; });
			UdpPacket packet = _queue.front();
			_queue.pop();
			_cv.notify_all();
			return packet;
		}
		catch (const runtime_error& e)
		{
			cerr << "Caught " << e.what() << endl;
		}
	}
}

UdpPacket UdpPacketQueue::PopForRequirements(ip::udp::endpoint endpoint)
{
	while (true)
	{
		try
		{
			unique_lock<mutex> lock(_mutex);
			_cv.wait(lock, [this, endpoint] {
				return !_queue.empty() &&
					_queue.front().endpoint == endpoint; });
			UdpPacket packet = _queue.front();
			_queue.pop();
			_cv.notify_all();
			return packet;
		}
		catch (const runtime_error& e)
		{
			cerr << "Caught " << e.what() << endl;
		}
	}
}

UdpPacketQueue::UdpPacketQueue() { }
UdpPacketQueue::~UdpPacketQueue() { }

void UdpReceiverThread()
{
	while (true)
	{
		try
		{
			char buffer[BUFSIZE];
			ip::udp::endpoint sender_endpoint;
			size_t num_bytes_received = Network::sock.receive_from(
				boost::asio::buffer(buffer), sender_endpoint);

			UdpPacket packet = deserialize(buffer, num_bytes_received);
			packet.endpoint = sender_endpoint;
			if (packet.type == Code::FILE_INSERT_REQUEST || packet.type == Code::FILE_REMOVE_REQUEST)
			{
				FileHandler::getInstance().insertRequest(packet);
			}
			else if (packet.type == Code::FILE_OPEN_REQUEST)
			{
				string content = FileHandler::getInstance().openFile(packet.data["path"]);
				UdpPacket response;
				response.endpoint = packet.endpoint;
				response.type = Code::FILE_OPEN_RESPONSE;
				response.data = { "content" , content };
				response.timestamp = time(TIME_NOW);
				Notifier::getInstance().insert(response);
			}
			else
			{
				UdpPacketQueue::getInstance().Push(packet);
			}
		}
		catch (const runtime_error& e)
		{
			cerr << "Caught " << e.what() << endl;
		}
		catch (const std::exception&)
		{
			cerr << endl << "aha" << endl;
		}
	}
}

UdpPacket deserialize(char buffer[], const size_t recv_len)
{
	buffer[recv_len] = '\0';
	try
	{
		cout << string(buffer) << endl;
		if (recv_len <= 0)
		{
			throw runtime_error("Packet is empty");
		}

		json parsed = json::parse(buffer);

		UdpPacket result;

		result.type = parsed["code"];
		result.timestamp = parsed["time"];
		result.data = parsed["data"];
		return result;
	}
	catch (const std::exception&)
	{
		std::ostringstream message;
		message << "Error while handling information, Couldnt parse message." << endl;
		try { message << "data: " << string(buffer) << endl; }
		catch (const std::exception&) { message << "Error while printing buffer." << endl; }
		throw runtime_error(message.str());
	}
}