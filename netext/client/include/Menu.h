#pragma once
#include "Utilities.h"

#define WELCOME_SCREEN "Menu:\n1. Create Session\n2. Join Session\n3. Delete Session(IF ALREADY EXISTED)"
#define INVALID_CHOICE "Invalid Choice, Try again: "
class Menu
{
public:
	static void mainMenu();
	static bool HandleChoice(int choice);
	static enum Choices { CREATE_SESSION = 1, JOIN_SESSION, DELETE_SESSION };
};

