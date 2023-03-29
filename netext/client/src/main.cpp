
#include <iostream>
#include <Windows.h>
#include "include/Utilities.h"

int main(void) 
{
	FileHandler& instance = FileHandler::getInstance();
	instance.test();
}