#include "Utilities.h"

/// <summary>
/// input int including handling invalid inpu
/// </summary>
/// <returns></returns>
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

/// <summary>
/// function inputing a path
/// </summary>
/// <returns></returns>
string getPath()
{
	string path;
	cout << "Enter Path: ";
	cin >> path;
	return path;
}
