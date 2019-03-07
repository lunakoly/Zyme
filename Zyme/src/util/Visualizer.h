#pragma once

#include <GL\glew.h>

#include "wrappers\Texture.h"


/**
	Provides some basic common 
	rendring debug functionality
*/
namespace Visualizer {
	bool initialize();
	void terminate();
	void displayTexture(Texture * texture);
	void displayDepth(Texture * texture);
}