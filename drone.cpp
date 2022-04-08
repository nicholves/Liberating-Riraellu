#include "drone.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {
	Drone::Drone(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health) 
		: EnemyGameObject(position, texture, collidable, tiles, health) {
		sighting_range_ = DETECTION_RADIUS;
	}


	void Drone::Pursue(double delta_time) {
		glm::vec3 target = player_->GetPosition();
		velocity_ = glm::vec3(target.x - position_.x, target.y - position_.y, 0);
		velocity_ = glm::normalize(velocity_) * (CheckDir() ? PURSUE_SPEED : PURSUE_SPEED / 3.0f);

		glm::vec3 subtraction = target - position_;
		rotation_ = atan2(subtraction.y, subtraction.x);
		rotation_ = (rotation_ / 3.14159f) * 180.0f - 90.0f;

		position_ += velocity_ * ((float)delta_time);
	}



	bool Drone::CheckDir() {
		if (glm::dot(player_->GetVelocity(), position_ - player_->GetPosition()) > 0) {
			return false;
		}

		return true;
	}
}