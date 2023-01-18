#include "DHkey.h"

/// <summary>
/// this is the initialization for the first person in the exchange.
/// it initiates _p and _g and creates private key
/// </summary>
DHkey::DHkey()
{
	srand(time(nullptr));
	_p = generatePrime();
	_g = generatePrimitiveRoot(_p);

	_kPrivate = rand() % (_p - MINIUM_VALUE) + MINIUM_VALUE;
}

/// <summary>
/// this is the initialization for the second person in the exchange.
/// it initiates _p and _g from given arguments and creates private key
/// </summary>
/// <param name="p"></param>
/// <param name="g"></param>
DHkey::DHkey(int p, int g) :_p(p), _g(g)
{
	srand(time(nullptr));
	_kPrivate = rand() % (_p - MINIUM_VALUE) + MINIUM_VALUE;
}


/// <summary>
/// this function returns modulus((base**exp),mod).
/// we cant just calculate it directly because it could overflow the integer limit,
/// so the solution is to calculate the % every step
/// </summary>
/// <param name="base"></param>
/// <param name="exponent"></param>
/// <param name="mod"></param>
/// <returns></returns>
int DHkey::modularPow(int base, int exponent, const int mod) const
{
	long long result = 1;

	base %= mod;

	while (exponent > 0)
	{
		if (exponent % 2 == 1)
		{
			result = (result * base) % mod;
		}

		exponent /= 2;

		base = (base * base) % mod;
	}

	return result;
}

/// <summary>
/// generates a prime number
/// </summary>
/// <returns>a random prime number</returns>
int DHkey::generatePrime() const
{
	int candidate = rand();

	while (!isPrime(candidate))
	{
		candidate = rand();
	}

	return candidate;
}

/// <summary>
/// generates a primitive root of mod
/// </summary>
/// <param name="mod"></param>
/// <returns></returns>
int DHkey::generatePrimitiveRoot(int mod) const
{
	int candidate = rand();

	while (!isPrimitiveRoot(candidate,mod))
	{
		candidate = rand();
	}

	return candidate;
}

/// <summary>
/// this function checks if a given number is a prime number
/// </summary>
/// <param name="num"></param>
/// <returns></returns>
bool DHkey::isPrime(int candidate) const
{
	bool flag = true;
	for (int i = 2; i < candidate && flag; i++)
	{
		if (candidate % i == 0)
		{
			flag = false;
			break;
		}
	}
	return flag;
}

/// <summary>
/// returns a boolean value indicating if the candidate is a primitive root for the given modulus.
/// </summary>
/// <param name="candidate"></param>
/// <param name="mod"></param>
/// <returns></returns>
bool DHkey::isPrimitiveRoot(int candidate, const int mod) const
{
	if (!isPrime(mod))
	{
		return false;
	}

	int phi = mod - 1;
	for (int i = 2; i <= phi; i++)
	{
		bool isRelativlyPrime = std::gcd(i, mod) == 1;
		bool notPrimitive = pow(candidate, phi / i) == 1;
		if (isRelativlyPrime && notPrimitive)
		{
			return false;
		}
	}
	return true;
}

/// <summary>
/// calculates the shared key from the private key and the other key.
/// </summary>
/// <param name="otherKey"></param>
/// <returns>shared key</returns>
int DHkey::calculateSharedKey(const int otherKey) const
{
	return modularPow(otherKey, _kPrivate, _p);
}

