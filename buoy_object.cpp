#include "buoy_object.h"

namespace game {

	BuoyObject::BuoyObject (const glm::vec3& position, GLuint texture, bool collidable, int tiles, double mass)
		: GameObject (position, texture, collidable, tiles) {
		mass_ = mass;
	}


	// The buoy will move with whatever velocity it was given from force, but it will slow down according to its mass 
	// There's no friction modifier to apply, so we have to simulate it.
	void BuoyObject::Update (double delta_time) { 
		GameObject::Update (delta_time);
		double friction = 0.001;
		if (velocity_.x > 0) {
			velocity_.x -= (float)(friction * mass_);
		}
		if (velocity_.y > 0) {
			velocity_.y -= (float)(friction * mass_);
		}
		if (velocity_.x < 0) {
			velocity_.x += (float)(friction * mass_);
		}
		if (velocity_.y < 0) {
			velocity_.y += (float)(friction * mass_);
		}

		// The above code only slows, not stops the buoy
		if (velocity_.x > -0.1 && velocity_.x < 0.1) {
			velocity_.x = 0.0f;
		}
		if (velocity_.y > -0.1f && velocity_.y < 0.1f) {
			velocity_.y = 0.0f;
		}
	}

}