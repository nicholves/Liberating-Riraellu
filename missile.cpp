#include "missile.h"

namespace game {

	/*
		BulletObject inherits from GameObject.
	*/

	MissileObject::MissileObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction, float speed, GameObject* target, std::string from)
		: BulletObject(position, texture, num_elements, collidable, tiles, direction, speed, from) {
		rotation_ = direction;
		float currotRadians = rotation_ * 3.14159f / 180.0f;
		target_ = target;
		acceleration_ = glm::vec3(0.0f);
		velocity_ = glm::normalize(target->GetPosition() - position) * speed;
	}



	void MissileObject::Update(double delta_time) {
		duration_timer += (float)delta_time;
		GameObject::Update(delta_time);
		velocity_ += acceleration_ * (float)delta_time;

		acceleration_ = (8.0f * glm::normalize (target_->GetPosition () - position_)) - velocity_;

		//set rotation of missile in direction of target
		glm::vec3 subtraction = target_->GetPosition() - position_;
		rotation_ = (atan2(subtraction.y, subtraction.x) * 180.0f / 3.14159f) + 180.0f;
	}
	
}