#pragma once


/**
	Deals with any sort of
	a timing question
*/
namespace Time {
	extern const double & deltaTime;
	extern const int & FPS;

	bool initialize();
	void update();

	/**
		Returns the `now` time value
		that can be passed to shaders
		as a uniform
	*/
	float get();
}