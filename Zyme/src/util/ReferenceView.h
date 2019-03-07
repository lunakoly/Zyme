#pragma once

#include <glm\glm.hpp>


/**
	Facade for editing quaternion
	in a user-friendly first-person way
*/
class ReferenceView {
private:
	float yaw = 0.0;
	float roll = 0.0;
	float pitch = 0.0;
	glm::quat & target;

public:
	ReferenceView(glm::quat &);
	void set(float, float, float);
	void add(float, float, float);
};