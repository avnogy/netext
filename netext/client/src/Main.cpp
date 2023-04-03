#include "include/Utilities.h"

int main()
{
	try
	{
		Network::writePortToFile();
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