#pragma once

#include <iostream>

#include "entities\Entity.h"

#include "components\Transform.h"


/**
	Manages upadting Transform component
	of each object of the scene
*/
namespace TransformSystem {
	/**
		Called each frame to recalculate
		entity's transform
	*/
	void update(Entity * it);
};