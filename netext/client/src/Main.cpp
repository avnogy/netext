#include "include/Utilities.h"

int main()
{
	try
	{
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