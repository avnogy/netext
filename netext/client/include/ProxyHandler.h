#pragma once
#include "Utilities.h"

void redirectHost(ip::udp::endpoint host, ip::udp::endpoint frontend);
void redirectFrontend(ip::udp::endpoint host, ip::udp::endpoint frontend);
