#pragma once
#include "Utilities.h"

/// <summary>
/// this class is made to make sure we init the WSA and free it at the end.
/// once the instance's scope end the d-tor will be called and the WSA will be free.
/// </summary>
class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
};

