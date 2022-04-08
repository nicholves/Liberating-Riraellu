#include "helicopter_blades.h"

namespace game {

	/*
	HelicopterBlades inherits from GameObject, not PlayerGameObject.
	There is no specific functionality from PlayerGameObject which would benefit HelicopterBlades
	*/

	HelicopterBlades::HelicopterBlades (const glm::vec3& position, GLuint texture, bool collidable, int tiles)
		: GameObject (position, texture, collidable, tiles) {
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
		float invincible_timer = ((PlayerGameObject*)(parent_))->getInvincibilityTimer();
		if (invincible_timer > 1.5f) {
			SetTexture(7);
		}
		else if (invincible_timer > 1.0f) {
			SetTexture(32);
		}
		else if (invincible_timer > 0.5f) {
			SetTexture(7);
		}
		else {
			SetTexture(32);
		}
		
	}

	

}