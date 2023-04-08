#include "include/Utilities.h"

int main()
{
	try
	{
		Network::writePortToFile();
		FileHandler::getInstance().setPath("test.txt");
		boost::thread fileHandlerTh(&FileHandler::handleRequests , &FileHandler::getInstance());

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