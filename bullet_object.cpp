#include "bullet_object.h"

namespace game {

	/*
		BulletObject inherits from GameObject. It uses the default Update function from its parent, after adjusting its timer.
	*/

	BulletObject::BulletObject (const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction)
		: GameObject (position, texture, num_elements, collidable, tiles) {
		rotation_ = direction;
		float currotRadians = rotation_ * 3.14159 / 180;
		float speed = 30;
		velocity_ = glm::vec3 ( // Velocity is constant and set from the start
			(-(speed * sin (currotRadians))),
			((speed * cos (currotRadians))),
			0);
	}

	double BulletObject::getTimer () {
		return duration_timer;
	}

	void BulletObject::Update (double delta_time) {
		duration_timer += delta_time;
		GameObject::Update (delta_time);
	}

}