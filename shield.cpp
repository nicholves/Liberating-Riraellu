#include "shield.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	Shield::Shield (const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, double additional_rotation)
		: GameObject (position, texture, num_elements, collidable, tiles) {
		rotation_ = additional_rotation + 90;
	}

	void Shield::setParent (GameObject* p) {
		parent_ = p;
	}

	void Shield::Update (double delta_time) {
		GameObject::Update (delta_time);
		rotation_ += 4;
		transformation_matrix_ = glm::translate (glm::mat4 (1.0f), parent_->GetPosition()) * glm::rotate (glm::mat4 (1.0f), rotation_, glm::vec3 (0.0f, 0.0f, 1.0f)) * transformation_matrix_ ;
	}

}