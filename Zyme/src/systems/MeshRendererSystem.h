#pragma once

#include <iostream>
#include <GL\glew.h>

#include "auxiliary\Debug.h"

#include "wrappers\FrameBuffer.h"
#include "wrappers\ShaderProgram.h"

#include "entities\Entity.h"

#include "components\Material.h"
#include "components\Mesh.h"
#include "components\Camera.h"
#include "components\DirectionalLight.h"


/**
	Manages rendering meshes
*/
namespace MeshRendererSystem {
	/**
		Initializes system dependencies.
		Called once at the very beginning
	*/
	bool initialize();

	/**
		Used to free resources
	*/
	void terminate();

	/**
		Called each frame to perform some
		action to the object tree
	*/
	void update(Entity *);
};