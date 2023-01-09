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
	int candidate = rand();

	while (!isPrime(candidate))
	{
		candidate = rand();
	}

	return candidate;
}

int DHkey::calculateSharedKey(const int otherKey) const
{
	return modular_pow(otherKey, _kPrivate, _p);
}