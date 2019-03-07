#include <GL\glew.h>
#include <GLFW\glfw3.h>
#include <glm\vec3.hpp>

#include "auxiliary\Alias.h"

#include "util\Visualizer.h"
#include "util\Time.h"

#include "RenderingContext.h"


///// PROPERTIES /////
static int windowWidth = 0;
static int windowHeight = 0;
const int & RenderingContext::windowWidth = ::windowWidth;
const int & RenderingContext::windowHeight = ::windowHeight;


///// INNER-USE /////
static GLFWwindow * window;


static bool initializeGLFW() {
	if (!glfwInit())
		return ERROR << "Could not initialize GLFW";

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	window = glfwCreateWindow(640, 480, "Hey There!", NULL, NULL);

	if (!window) {
		glfwTerminate();
		return ERROR << "Could not initialize GLFW window";
	}

	glfwMakeContextCurrent(window);
	return true;
}

static bool initializeGLEW() {
	if (glewInit() != GLEW_OK)
		return ERROR << "Could not initialize GLEW";
	return true;
}

static void onWindowResized(GLFWwindow* window, int width, int height) {
	windowHeight = height;
	windowWidth = width;
}

bool RenderingContext::initialize() {	
	if (
		!initializeGLFW() || 
		!initializeGLEW() ||
		!Time::initialize() ||
		!Visualizer::initialize()
	) return false;

	// set up size fields
	glfwGetWindowSize(window, &::windowWidth, &::windowHeight);

	glfwSetWindowSizeCallback(window, onWindowResized);
	glfwSwapInterval(1);
	return true;
}

bool RenderingContext::proceed() {
	if (!glfwWindowShouldClose(window)) {
		Time::update();
		glfwSwapBuffers(window);
		glfwPollEvents();
		return true;
	}

	return false;
}

void RenderingContext::terminate() {
	Visualizer::terminate();
	glfwTerminate();
}

std::string RenderingContext::getOpenGLVersion() {
	return (const char *) glGetString(GL_VERSION);
}