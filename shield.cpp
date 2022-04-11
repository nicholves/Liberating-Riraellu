#include "shield.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	Shield::Shield (const glm::vec3& position, GLuint texture, bool collidable, int tiles, double additional_rotation)
		: GameObject (position, texture, collidable, tiles) {		
		orbit = (float)(additional_rotation + 90);
	}

	void Shield::setParent (GameObject* p) {
		parent_ = p;
	}

	void Shield::Update (double delta_time) {
		GameObject::Update (delta_time);
		orbit += 2;
		transformation_matrix_ = glm::translate (glm::mat4 (1.0f), parent_->GetPosition()) * glm::rotate (glm::mat4 (1.0f), orbit, glm::vec3 (0.0f, 0.0f, 1.0f)) * transformation_matrix_ ;
	}

}