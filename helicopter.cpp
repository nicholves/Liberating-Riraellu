#include "helicopter.h"
#include <glm/gtc/matrix_transform.hpp> 

namespace game {

	//initializing static variables to default values
	std::vector<BulletObject*>* Helicopter::bullet_objects_ptr_ = NULL;
	//PlayerGameObject* Helicopter::player_ = NULL;
	GLuint* Helicopter::bulletTex_ = 0;

	Helicopter::Helicopter (const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health) :
		EnemyGameObject (position, texture, collidable, tiles, health) {
		state = 0; 
		patrol_point = glm::vec3 (position_.x - patrol_radius, position_.y, position_.z);
		last_bullet_fire_time_ = 0;
		sighting_range_ = 4.5;
	}

	Helicopter::~Helicopter () { delete blades; }

	void Helicopter::Update (double delta_time) {
		// Same as enemy game object
		if (state == 0) {
			internal_timer += delta_time;
			Patrol (delta_time);
			velocity_ = glm::vec3 (0, 0, 0);
		}
		else if (state == 1) {
			Pursue (delta_time);
			position_ += velocity_ * ((float)delta_time);
		}
		transformation_matrix_ = glm::translate (glm::mat4 (1.0f), position_) * glm::scale (glm::mat4 (1.0f), glm::vec3 (scale_, scale_, 1.0)) * glm::rotate (glm::mat4 (1.0f), rotation_, glm::vec3 (0.0f, 0.0f, 1.0f));

		// Except it incorporates the cooldown
		last_bullet_fire_time_ += delta_time;
		blades->Update (delta_time);
	}

	// Object orbits around its patrol point
	void Helicopter::Patrol (double delta_time) {
		position_.x = patrol_radius * (float)cos (internal_timer) + patrol_point.x;
		position_.y = patrol_radius * (float)sin (internal_timer) + patrol_point.y;
		rotation_ = (float)(internal_timer * 57.3f);
	}

	// Object pursues a given point that is updated on game tick
	void Helicopter::Pursue (double delta_time) {
		float angle = atan ((abs (target.y - position_.y)) / (abs (target.x - position_.x)));
		if (target.x < position_.x) {
			if (target.y < position_.y) {
				angle += 3.14159f;
			}
			else {
				angle = 3.14159f - angle;
			}
		}
		else {
			if (target.y < position_.y) {
				angle = 2.0f * (3.14159f) - angle;
			}
		}

		float speed = 2.0f;
		rotation_ = (angle / 3.14159f) * 180.0f - 90.0f;

		if (last_bullet_fire_time_ > 0.1) {
			BulletObject* bullet = new BulletObject (position_ + glm::vec3 ((-(0.25 * sin ((rotation_ + 45) * 3.14159 / 180))), ((0.25 * cos ((rotation_ + 45) * 3.14159 / 180))), 0), *bulletTex_, true, 1, rotation_ + 0.0f, 5.0f, "enemy");
			bullet->SetScale (0.3f);
			bullet->SetDuration (10.0f); //bullets last 10 seconds
			bullet_objects_ptr_->push_back (bullet);
			bullet = new BulletObject (position_ + glm::vec3 ((-(0.25 * sin ((rotation_ - 45) * 3.14159 / 180))), ((0.25 * cos ((rotation_ - 45) * 3.14159 / 180))), 0), *bulletTex_, true, 1, rotation_ + 0.0f, 5.0f, "enemy");
			bullet->SetScale (0.3f);
			bullet->SetDuration (3.0f); //bullets last 10 seconds
			bullet_objects_ptr_->push_back (bullet);
			last_bullet_fire_time_ = 0.0f;
		}

		velocity_ = (glm::vec3 (
			((speed * cos (angle)) - (speed * sin (angle))),
			(-(speed * sin (angle)) + (speed * cos (angle))),
			0));
	}

	void Helicopter::SetupBullets (std::vector<BulletObject*>* ptr, GLuint* bulletTex, GameObject* player) {
		Helicopter::bullet_objects_ptr_ = ptr;
		Helicopter::bulletTex_ = bulletTex;
		//Helicopter::player_ = (PlayerGameObject*)(player);
	}

	void Helicopter::Render (Shader& shader, glm::mat4 view_matrix) {
		blades->Render (shader, view_matrix);
		GameObject::Render (shader, view_matrix);
	}
}