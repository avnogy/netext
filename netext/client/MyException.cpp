#include "MyException.h"

MyException::MyException(std::string msg) :
	_error(msg)
{
}

MyException::~MyException()
{
}

char const* MyException::what() const noexcept
{
	return _error.c_str();
}
