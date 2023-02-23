#include "include/Menu.h"

void Menu::mainMenu()
{
	cout << WELCOME_SCREEN << endl << "Option: ";
	while (!HandleChoice(getInt()))
	{
		cout << INVALID_CHOICE;
	}
}

bool Menu::HandleChoice(int choice)
{
	switch (choice)
	{
	case Menu::Choices::CREATE_SESSION:
		PeerServer::createSession();
		break;
	case Menu::Choices::JOIN_SESSION:
		PeerClient::joinSession();
		break;
	case Menu::Choices::DELETE_SESSION:
		PeerServer::deleteSession();
		break;
	default:
		return FAILURE;
		break;
	}
	return SUCCESS;
}
