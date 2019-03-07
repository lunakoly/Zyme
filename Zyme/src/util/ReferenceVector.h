#pragma once


/**
	Facade for editing some
	internal valus in a user-friendly
	vec3 way
*/
struct ReferenceVector {
	float & x;
	float & y;
	float & z;

	ReferenceVector(float &, float &, float &);
	void set(float, float, float);
	void add(float, float, float);
};