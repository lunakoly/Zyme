#pragma once

#include <glm\glm.hpp>

#include "wrappers\Texture.h"

#include "Component.h"


/**
	Tells MeshRendererSystem that this object
	can cast directional shadows and produce
	directional light
*/
struct DirectionalLight : public Component {
	glm::vec3 color = glm::vec3(1.0, 1.0, 1.0);
	Texture depthTexture = Texture();

	DirectionalLight() {
		depthTexture.bindAs(GL_TEXTURE_2D)
			->prepareSimpleTexture()
			->allocate2D(0, GL_DEPTH_COMPONENT24, 512, 512, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr)
			->unbind();
	}
};