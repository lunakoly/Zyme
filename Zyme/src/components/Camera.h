#pragma once

#include <glm\glm.hpp>

#include "Component.h"


/**
	Allows MeshRendererSystem to use
	an entity as a camera
*/
struct Camera : public Component {
	glm::mat4 projection;
	bool isActive;

	Camera(glm::mat4 projection, bool isActive = false) 
		: projection(projection) 
		, isActive(isActive) {}
};