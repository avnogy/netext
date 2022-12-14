#pragma once


#include <Exception>
#include <iostream>

class MyException : public std::exception
{
public:
	MyException(std::string msg);
	~MyException();
	virtual char const* what() const noexcept;
private:

	std::string _error;
};