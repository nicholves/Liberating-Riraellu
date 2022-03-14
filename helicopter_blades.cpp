#include "helicopter_blades.h"

namespace game {

	/*
	HelicopterBlades inherits from GameObject, not PlayerGameObject.
	There is no specific functionality from PlayerGameObject which would benefit HelicopterBlades
	*/

	HelicopterBlades::HelicopterBlades (const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles)
		: GameObject (position, texture, num_elements, collidable, tiles) {
	}

	// Detemines which object the blades will attach to
	void HelicopterBlades::setParent (GameObject* p) {
		parent_ = p;
	}

	// Update just uses the parent function
	void HelicopterBlades::Update (double delta_time) {
		GameObject::Update (delta_time);
		rotation_ += 16;
		transformation_matrix_ = parent_->GetTransfMatrix () * transformation_matrix_;
		
	}

	

}