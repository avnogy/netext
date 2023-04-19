#include "include/Utilities.h"

int main()
{
	try
	{
		Network::writePortToFile();
		FileHandler::getInstance().setPath("test.txt");
		thread notifierTh(&Notifier::notify, &Notifier::getInstance());
		thread receiver_thread(UdpReceiverThread);

		while (true)
		{
			Menu::mainMenu();
		}
		return 0;
	}
	catch (std::exception& e)
	{
		std::cout << "Error: " << e.what() << std::endl;
	}
}