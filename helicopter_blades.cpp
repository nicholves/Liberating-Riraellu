#include "helicopter_blades.h"

namespace game {

	/*
	HelicopterBlades inherits from GameObject, not PlayerGameObject.
	There is no specific functionality from PlayerGameObject which would benefit HelicopterBlades
	*/

	HelicopterBlades::HelicopterBlades (const glm::vec3& position, GLuint texture, bool collidable, int tiles)
		: GameObject (position, texture, collidable, tiles) {
	}

	HelicopterBlades::~HelicopterBlades () {}

	// Detemines which object the blades will attach to
	void HelicopterBlades::setParent (GameObject* p) {
		parent_ = p;
	}

	// Update just uses the parent function
	void HelicopterBlades::Update (double delta_time) {
		GameObject::Update (delta_time);
		rotation_ += 12;
		transformation_matrix_ = parent_->GetTransfMatrix () * transformation_matrix_;
	}

	

}