#pragma once

#include "systems\TransformSystem.h"
#include "systems\MeshRendererSystem.h"


/**
	Integrates most commonly used systems
	together so that they can be easily
	enabled/disabled
*/
namespace GameService {
	/**
		Initislizes the underlying systems
	*/
	bool initialize() {
		if (MeshRendererSystem::initialize()) 
			return true;
		return false;
	}

	/**
		Terminates the underlying systems
	*/
	void terminate() {
		MeshRendererSystem::terminate();
	}

	/**
		Used to run the systems pipeline
		starting from a root object
	*/
	void update(Entity * root) {
		TransformSystem::update(root);
		MeshRendererSystem::update(root);
	}
};