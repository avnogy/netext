#include "include/UdpHandler.h"

TRY_CATCH_FUNCTION(void, UdpPacketQueue::Push, (UdpPacket packet), "Error while pushing packet into queue",
	{
	lock_guard<mutex> lock(_mutex);
	_queue.push(packet);
	_cv.notify_all();
	}, {});

TRY_CATCH_LOOP_FUNCTION(UdpPacket, UdpPacketQueue::PopForRequirements, (const Code type), "Error while popping for type",
	{
		unique_lock<mutex> lock(_mutex);
		_cv.wait(lock, [this, type] {
			boost::this_thread::interruption_point();
			return !_queue.empty() &&
				_queue.front().type == type;
			});
		UdpPacket packet = _queue.front();
		_queue.pop();
		_cv.notify_all();
		return packet;
	}, {
		throw boost::thread_interrupted();
	});

TRY_CATCH_LOOP_FUNCTION(UdpPacket, UdpPacketQueue::PopForRequirements, (const Code type1, const Code type2), "Error while popping for type and type",
	{
		unique_lock<mutex> lock(_mutex);
		_cv.wait(lock, [this, type1, type2] {
			boost::this_thread::interruption_point();
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
	}, {
		throw boost::thread_interrupted();
	});

TRY_CATCH_LOOP_FUNCTION(UdpPacket, UdpPacketQueue::PopForRequirements, (const Code type, ip::udp::endpoint endpoint), "Error while popping for endpoint and type",
	{
		unique_lock<mutex> lock(_mutex);
		_cv.wait(lock, [this, type, endpoint] {
			boost::this_thread::interruption_point();
			return !_queue.empty() &&
				_queue.front().type == type &&
				_queue.front().endpoint == endpoint; });
		UdpPacket packet = _queue.front();
		_queue.pop();
		_cv.notify_all();
		return packet;
	}, {
		throw boost::thread_interrupted();
	});

TRY_CATCH_LOOP_FUNCTION(UdpPacket, UdpPacketQueue::PopForRequirements, (ip::udp::endpoint endpoint), "Error while popping for endpoint",
	{
		unique_lock<mutex> lock(_mutex);
		_cv.wait(lock, [this, endpoint] {
			boost::this_thread::interruption_point();
			return !_queue.empty() &&
				_queue.front().endpoint == endpoint &&
				_queue.front().type != Code::CLIENT_LEAVE_REQUEST;
			});
		UdpPacket packet = _queue.front();
		_queue.pop();
		_cv.notify_all();
		return packet;
	}, {
		throw boost::thread_interrupted();
	});

UdpPacketQueue::UdpPacketQueue() { }
UdpPacketQueue::~UdpPacketQueue() { }

TRY_CATCH_LOOP_FUNCTION(void, UdpReceiverThread, (), "error while receiving in udphandler.",
	{
		char buffer[BUFSIZE];
		ip::udp::endpoint sender_endpoint;
		size_t num_bytes_received = Network::sock.receive_from(
			boost::asio::buffer(buffer), sender_endpoint);

		UdpPacket packet = deserialize(buffer, num_bytes_received);
		packet.endpoint = sender_endpoint;
		if (packet.type == Code::FILE_OPEN_REQUEST)
		{
			string content = FileHandler::getInstance().openFile(packet.data["path"]);
			UdpPacket response;
			response.endpoint = packet.endpoint;
			response.type = Code::FILE_OPEN_RESPONSE;
			response.data["content"] = content;
			response.timestamp = time(TIME_NOW);
			Notifier::getInstance().insert(response);
		}
		else if (packet.type != Code::PUNCH_HOLE_PACKET)
		{
			UdpPacketQueue::getInstance().Push(packet);
		}
	}, {});

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