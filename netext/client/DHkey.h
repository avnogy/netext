#pragma once
#include <iostream>
#include <numeric>
#include <cmath>   

#define MINIUM_VALUE 2

class DHkey
{
public:

	DHkey();
	DHkey(int p,int g);
	int calculateSharedKey(const int otherKey) const;

	int modularPow(int base, int exponent, const int mod) const;
	int generatePrime() const;
	int generatePrimitiveRoot(int mod) const;
	bool isPrime(int candidate) const;
	bool isPrimitiveRoot(int candidate, const int mod) const;



private:
	int _p; // modulus (public)
	int _g; // primitive root (public)

	int _kPrivate; // private key (for the specific client)

};