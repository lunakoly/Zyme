#pragma once

#include <glm\glm.hpp>
#include <glm\gtc\quaternion.hpp>

#include "util\ReferenceVector.h"
#include "util\ReferenceView.h"

#include "Component.h"


/**
	Allows entity to be presented
	in a scene virtual space
*/
struct Transform : public Component {
	glm::mat4 model       = glm::mat4(1.0);
	glm::mat4 inverse     = glm::mat4(1.0);

	glm::mat4 translation = glm::mat4(1.0);
	glm::mat4 offset      = glm::mat4(1.0);
	glm::mat4 scale       = glm::mat4(1.0);
	glm::quat rotation    = glm::quat();

	ReferenceVector position = {
		translation[3][0],
		translation[3][1],
		translation[3][2]
	};

	ReferenceVector gap = {
		offset[3][0],
		offset[3][1],
		offset[3][2]
	};

	ReferenceVector size = {
		scale[0][0],
		scale[1][1],
		scale[2][2]
	};

	ReferenceView view = { rotation };
};