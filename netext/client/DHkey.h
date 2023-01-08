#pragma once



class DHkey
{
	public:

		DHkey();

		int calculateSharedKey();

		void generatePrimeAndPrimitive();

		int modular_pow(int base, int exponent, int modulus);
		bool isPrime(int num);
		bool isPrimitiveRoot(int num, int modulo);



	private:
		int _p; // modulus (public)
		int _g; // primitive root (public)

		int _kPrivate; // private key (for the specific client)

};