#include "include/Menu.h"

void Menu::mainMenu()
{
    cout << WELCOME_SCREEN << endl << "Option: ";
	while (!HandleChoice(Helper::getInt()))
	{
		cout << INVALID_CHOICE;
	}
}

bool Menu::HandleChoice(int choice)
{
	switch (choice)
	{
	case Menu::Choices::CREATE_SESSION:
		//peerserver create
		break;
	case Menu::Choices::JOIN_SESSION:
		//peerclient join
		break;
	case Menu::Choices::DELETE_SESSION:
		//peerserver delete
		break;
	default:
		return FAILURE;
		break;
	}
	return SUCCESS;
}
