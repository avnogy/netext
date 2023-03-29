#include "Utilities.h"

int getInt()
{

	int num = 0;
	cin >> num;
	while (!cin.good())
	{
		//reset buffer
		cin.clear();
		cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

		//trying again
		cout << "Invalid input, Try again." << endl;
		cin >> num;
	}
	return num;

}
