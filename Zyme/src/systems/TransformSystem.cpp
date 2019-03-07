#include "TransformSystem.h"


static void recalculate(Entity * it, Transform * transform) {
	transform->model = transform->translation
					 * glm::mat4_cast(transform->rotation)
			         * transform->offset
			         * transform->scale;

	glm::mat4 inverseTranslation = glm::translate(glm::mat4(1.0), glm::vec3(
		-transform->position.x,
		-transform->position.y,
		-transform->position.z
	));

	glm::mat4 inverseOffset = glm::translate(glm::mat4(1.0), glm::vec3(
		-transform->gap.x,
		-transform->gap.y,
		-transform->gap.z
	));

	glm::mat4 inverseScale = glm::scale(glm::mat4(1.0), glm::vec3(
		1.0 / transform->size.x,
		1.0 / transform->size.y,
		1.0 / transform->size.z
	));

	glm::quat inverseRotation(
		transform->rotation.w,
		-transform->rotation.x,
		-transform->rotation.y,
		-transform->rotation.z
	);

	transform->inverse = inverseScale
					   * inverseOffset
					   * glm::mat4_cast(inverseRotation)
					   * inverseTranslation;

	if (it->getParent() != nullptr) {
		Transform * parentTransform = it->getParent()->getComponent<Transform>();

		if (parentTransform != nullptr) {
			transform->model   = parentTransform->model   * transform      ->model;
			transform->inverse = transform      ->inverse * parentTransform->inverse;
		}
	}
}

void TransformSystem::update(Entity * scene) {
	scene->traverseWith(&recalculate);
}