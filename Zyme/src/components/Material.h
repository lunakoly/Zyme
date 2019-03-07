#pragma once

#include <glm\glm.hpp>

#include "wrappers\Texture.h"

#include "Component.h"


/**
	Specifies the properties used by
	MeshRendererSystem to determine
	mesh look and feel
*/
struct Material : public Component {
	Texture * specular = nullptr;
	Texture * diffuse  = nullptr;
	Texture * normal   = nullptr;

	float shininess = 32.0;

	glm::vec4 color = glm::vec4(0.7, 0.7, 0.7, 1.0);
};
