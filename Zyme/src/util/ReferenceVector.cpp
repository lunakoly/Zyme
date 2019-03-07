#include "ReferenceVector.h"


ReferenceVector::ReferenceVector(float & x, float & y, float & z)
		: x(x)
		, y(y)
		, z(z) {}

void ReferenceVector::set(float x, float y, float z) {
	this->x = x;
	this->y = y;
	this->z = z;
}

void ReferenceVector::add(float x, float y, float z) {
	set(this->x + x, this->y + y, this->z + z);
}