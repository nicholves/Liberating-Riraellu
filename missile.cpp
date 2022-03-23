#include "missile.h"

namespace game {

	/*
		BulletObject inherits from GameObject.
	*/

	MissileObject::MissileObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction, float speed, GameObject* target)
		: BulletObject(position, texture, num_elements, collidable, tiles, direction, speed) {
		rotation_ = direction;
		float currotRadians = rotation_ * 3.14159 / 180;
		target_ = target;
		acceleration_ = glm::vec3(0.0f);
		velocity_ = glm::vec3(0.0f);
	}



	void MissileObject::Update(double delta_time) {
		duration_timer += delta_time;
		GameObject::Update(delta_time);
		velocity_ += acceleration_ * (float)delta_time;

		//set acceleration towards target (smooth turns)
		acceleration_ = ( 8.0f * glm::normalize(target_->GetPosition() - position_) ) - velocity_;

		//set rotation of missile in direction of target
		glm::vec3 subtraction = target_->GetPosition() - position_;
		rotation_ = (atan2(subtraction.y, subtraction.x) * 180/3.14159) + 180.0f;
	}

}