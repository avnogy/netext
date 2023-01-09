#include "DHkey.h"


DHkey::DHkey()
{
	srand(time(nullptr));
	_kPrivate = rand() % (_p - MINIUM_VALUE) + MINIUM_VALUE;

	_p = generatePrime();
	_g = generatePrimitiveRoot();
}

//this function returns mod((base**exp),mod)
//we cant just calculate it directly because it could overflow the integer limit
//so the solution is to calculat the % every step
int DHkey::modularPow(int base, int exponent, const int modulus) const
{
	//the result if the exponent is zero
	long long result = 1;

	base %= modulus;

	//faster way then just repeatedly performing result*base 
	while (exponent > 0)
	{
		if (exponent % 2 == 1)
		{
			result = (result * base) % modulus;
		}

		exponent /= 2;

		base = (base * base) % modulus;
	}

	return result;
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

int DHkey::generatePrimitiveRoot() const
{
	int candidate = rand();

	while (!isPrimitiveRoot(candidate,_p))
	{
		candidate = rand();
	}

	return candidate;
}

int DHkey::calculateSharedKey(const int otherKey) const
{
	return modular_pow(otherKey, _kPrivate, _p);
}

