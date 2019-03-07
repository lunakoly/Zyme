#pragma once

#include <sstream>
#include <iostream>


/**
	Object that supports error messages buffering
	and prints them when used in certain cases
*/
static class ErrorMacro {
private:
	std::stringstream buffer;

public:
	/**
		Merely appends message text to the end
	*/
	template <typename T>
	ErrorMacro & operator<<(const T & message) {
		buffer << message;
		return *this;
	}
	
	/**
		Makes this print the message when used
		after `return` statement inside a function
		that returns a boolean or a numeric value
	*/
	operator int() {
		std::cout << "Error > " << buffer.str() << std::endl;
		buffer.str(std::string());
		return 0;
	}

	/**
		Mekes this print the message when used
		after `ErrorMacro |`, so that we can
		create a clever define later
	*/
	void operator| (const ErrorMacro & other) {
		std::cout << "Error > " << buffer.str() << std::endl;
		buffer.str(std::string());
	}
} errorMacroInstance;


#define ERROR errorMacroInstance
#define LOG errorMacroInstance |