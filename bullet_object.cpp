#include "bullet_object.h"

namespace game {

	/*
		BulletObject inherits from GameObject. It uses the default Update function from its parent, after adjusting its timer.
	*/

	BulletObject::BulletObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction, float speed, std::string from)
		: GameObject (position, texture, num_elements, collidable, tiles) {
		rotation_ = direction;
		duration_ = 0.2f;
		float currotRadians = rotation_ * 3.14159f / 180.0f;
		speed_ = speed; //speed is the magnitude for the velocity of bullets
		velocity_ = glm::vec3 ( // Velocity is constant and set from the start
			(-(speed_ * sin (currotRadians))),
			((speed_ * cos (currotRadians))),
			0);
		from_ = from;
	}

	double BulletObject::getTimer () {
		return duration_timer;
	}

	void BulletObject::Update (double delta_time) {
		duration_timer += (float)delta_time;
		GameObject::Update (delta_time);
	}

}