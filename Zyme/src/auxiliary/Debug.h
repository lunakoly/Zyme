#pragma once

#include <GL\glew.h>
#include "Alias.h"

/**
	(Visual C++ only)
	
	USAGE:
	
	Write `CHECK(glCommand());` instead of `glCommand();`
	to check if any errors occured during the `glCommand`.

	The debugger will be stopped at the failed `CHECK()`

	(Taken from `TheChernoProject` YouTube channel)
*/


#define ASSERT(x) if (!(x)) __debugbreak();
#define CHECK(x) clearOpenGLErrors(); x;\
		ASSERT(checkOpenGLError(#x, __FILE__, __LINE__));


static void clearOpenGLErrors() {
	while (glGetError() != GL_NO_ERROR);
}

static const char * parseOpenGLError(GLenum error) {
	if (error == GL_INVALID_ENUM)
		return "GL_INVALID_ENUM";
	if (error == GL_INVALID_VALUE)
		return "GL_INVALID_VALUE";
	if (error == GL_INVALID_OPERATION)
		return "GL_INVALID_OPERATION";
	if (error == GL_INVALID_FRAMEBUFFER_OPERATION)
		return "GL_INVALID_FRAMEBUFFER_OPERATION";
	if (error == GL_OUT_OF_MEMORY)
		return "GL_OUT_OF_MEMORY";
	if (error == GL_STACK_UNDERFLOW)
		return "GL_STACK_UNDERFLOW";
	if (error == GL_STACK_OVERFLOW)
		return "GL_STACK_OVERFLOW";
	return "GL_NO_ERROR";
}

static bool checkOpenGLError(
		const char * code,
		const char * file,
		int line) {
	if (GLenum error = glGetError())
		return ERROR << parseOpenGLError(error) << 
			"\nat line " << line << " in " << file;
	return true;
}