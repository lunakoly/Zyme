#pragma once

#include <string>


/**
	Initializes the global context
	for any rendering purpose
*/
namespace RenderingContext {
	extern const int & windowWidth;
	extern const int & windowHeight;

	bool initialize();
	bool proceed();
	void terminate();

	std::string getOpenGLVersion();
}