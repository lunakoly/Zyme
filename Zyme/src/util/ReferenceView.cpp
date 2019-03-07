#include <glm\gtc\quaternion.hpp>

#include "ReferenceView.h"


ReferenceView::ReferenceView(glm::quat & target) : target(target) {}

void ReferenceView::set(float yaw, float pitch, float roll) {
	this->pitch = pitch;
	this->roll = roll;
	this->yaw = yaw;

	glm::quat yawQuaternion = glm::quat(cos(yaw / 2), 0.0, sin(yaw / 2), 0.0);
	glm::quat rollQuaternion = glm::quat(cos(roll / 2), 0.0, 0.0, sin(roll / 2));
	glm::quat pitchQuaternion = glm::quat(cos(pitch / 2), sin(pitch / 2), 0.0, 0.0);

	target = yawQuaternion * pitchQuaternion * rollQuaternion;
}

void ReferenceView::add(float yaw, float pitch, float roll) {
	set(this->yaw + yaw, this->pitch + pitch, this->roll + roll);
}