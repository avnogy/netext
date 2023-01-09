#include "DHkey.h"


DHkey::DHkey()
{
	srand(time(nullptr));
	_kPrivate = rand() % (_p - MINIUM_VALUE) + MINIUM_VALUE;

	_p = generatePrime();
	_g = generatePrimitiveRoot();
}

int DHkey::generatePrime() const
{
	srand(time(nullptr));
	int candidate = rand();

	while (!isPrime(candidate))
	{
		candidate = rand();
	}

	return candidate;
}
