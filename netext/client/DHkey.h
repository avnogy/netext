#pragma once
#include <iostream>

#define MINIUM_VALUE 2
class DHkey
{
	public:

		DHkey();

		int calculateSharedKey(const int otherKey) const;

		int modular_pow(const int base, const int exponent, const int modulus) const ;
		int generatePrime() const;
		int generatePrimitiveRoot() const;
		bool isPrime(int num) const;
		bool isPrimitiveRoot(int num, int modulo);

		

	private:
		int _p; // modulus (public)
		int _g; // primitive root (public)

		int _kPrivate; // private key (for the specific client)

};