#include "enemy_game_object.h"
#include <glm/gtc/matrix_transform.hpp> 

namespace game {

	/*
		EnemyGameObject inherits from GameObject
		It overrides GameObject's update method, so that the object can be updated according to its states
	*/

	EnemyGameObject::EnemyGameObject(const glm::vec3& position, GLuint texture, bool collidable, int tiles)
		: GameObject(position, texture, collidable, tiles) {
		state = 0; patrol_point = glm::vec3(position_.x - patrol_radius, position_.y, position_.z); sighting_range_ = 3.5;
	}

	void EnemyGameObject::Update (double delta_time) {
		if (state == 0) {
			internal_timer += delta_time;
			Patrol (delta_time);
			velocity_ = glm::vec3 (0, 0, 0);
		}
		else if (state == 1) {
			Pursue (delta_time);
			position_ += velocity_ * ((float)delta_time);
		}
		transformation_matrix_ = glm::translate(glm::mat4 (1.0f), position_) * glm::scale(glm::mat4 (1.0f), glm::vec3 (scale_, scale_, 1.0)) * glm::rotate(glm::mat4 (1.0f), rotation_, glm::vec3 (0.0f, 0.0f, 1.0f));
	}

	// Updates the object's movement state. 0 = patrolling, 1 = pursuit
	void EnemyGameObject::setState (int state_) {
		state = state_;
		if (state == 0) {
			internal_timer = 0;
			patrol_point = glm::vec3(position_.x - patrol_radius, position_.y, position_.z);
		}
	}

	int EnemyGameObject::getState () {
		return state;
	}

	void EnemyGameObject::setTarget (glm::vec3 target_) {
		target = target_;
	}

	// Object orbits around its patrol point
	void EnemyGameObject::Patrol (double delta_time) {
		position_.x = patrol_radius * cos (internal_timer) + patrol_point.x;
		position_.y = patrol_radius * sin (internal_timer) + patrol_point.y;
		rotation_ = (internal_timer * 57.3);
	}

	// Object pursues a given point that is updated on game tick
	void EnemyGameObject::Pursue (double delta_time) {
		float angle = atan ((abs (target.y - position_.y)) / (abs (target.x - position_.x)));
		if (target.x < position_.x) {
			if (target.y < position_.y) {
				angle += 3.14159;
			}
			else {
				angle = 3.14159 - angle;
			}
		}
		else {
			if (target.y < position_.y) {
				angle = 2*(3.14159) - angle;
			}
		}

		float speed = 2;
		rotation_ = (angle / 3.14159) * 180 - 90;

		velocity_ = (glm::vec3 (
			((speed * cos(angle)) - (speed * sin (angle))),
			((speed * sin(angle)) + (speed * cos (angle))),
			0));
	}

}