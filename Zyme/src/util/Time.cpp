#include <GLFW\glfw3.h>
#include <iostream>

#include "Time.h"


///// PROPERTIES /////
static double deltaTime = 0;
const double & Time::deltaTime = ::deltaTime;

static int FPS = 0;
const int & Time::FPS = ::FPS;


///// INNER-USE /////
static int fpsCounter = 0;
static double secondCounter = 0.0;
static double oldTime = 0.0;


bool Time::initialize() {
	oldTime = glfwGetTime();
	return true;
}

void Time::update() {
	// calculate dt
	const double newTime = glfwGetTime();
	::deltaTime = newTime - oldTime;
	oldTime = newTime;

	secondCounter += ::deltaTime;
	fpsCounter++;

	// submit new fps
	if (secondCounter > 1) {
		::FPS = fpsCounter;
		secondCounter = 0.0;
		fpsCounter = 0;
	}
}

float Time::get() {
	return (unsigned int) (glfwGetTime() * 1000) % 60000 / 1000.0f;
}