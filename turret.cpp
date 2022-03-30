#include "turret.h"
#include <glm/gtc/matrix_transform.hpp> 


namespace game {
	
	//initializing static variables to default values
	std::vector<BulletObject*>* Turret::bullet_objects_ptr_ = NULL;
	std::vector<MissileObject*>* Turret::missile_objects_ptr_ = NULL; 
	GameObject* Turret::player_ = NULL;
	GLuint* Turret::bulletTex_ = 0;
	int* Turret::size_ = NULL;
	GLuint* Turret::missileTex_ = 0;
	
	/*
		Turret inherits from EnemyGameObject
		It overrides EnemyGameObject's update method, so that the object can be updated according to its states
	*/

	Turret::Turret(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles)
		: EnemyGameObject(position, texture, num_elements, collidable, tiles) {
		state = 0; patrol_point = glm::vec3(position_.x - patrol_radius, position_.y, position_.z);
		//setting up required fields
		last_bullet_fire_time_ = 0;
		last_missile_fire_time_ = 0;
		sighting_range_ = 2.5;
	}

	void Turret::Update(double delta_time) {
		//is the same as enemy game object
		if (state == 0) {
			internal_timer += delta_time;
			Patrol(delta_time);
			velocity_ = glm::vec3(0, 0, 0);
		}
		else if (state == 1) {
			Pursue(delta_time);
			velocity_ = glm::vec3(0, 0, 0);
		}
		transformation_matrix_ = glm::translate(glm::mat4(1.0f), position_) * glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0)) * glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0f, 0.0f, 1.0f));

		//differs below
		//updates delays to ensure weapons are fired at reasonable intervals
		last_bullet_fire_time_ += delta_time;
		last_missile_fire_time_ += delta_time;
	}

	// turret spins in place
	void Turret::Patrol(double delta_time) {
		rotation_ = (internal_timer * 57.3);
	}

	// Object rotates to face towards target and fires on a given interval
	void Turret::Pursue(double delta_time) {
		float angle = atan((abs(target.y - position_.y)) / (abs(target.x - position_.x)));
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
				angle = 2 * (3.14159) - angle;
			}
		}

		rotation_ = (angle / 3.14159) * 180 + 180;

		if (last_bullet_fire_time_ > 0.05) {
			BulletObject* bullet = new BulletObject(position_ + glm::vec3((-(0.25 * sin((rotation_ + 90.0f) * 3.14159 / 180))), ((0.25 * cos((rotation_ + 90.0f) * 3.14159 / 180))), 0), *bulletTex_, *size_, true, 1, rotation_ + 90.0f, 5.0f, "enemy");
			bullet->SetScale(0.3);
			bullet->SetDuration(10.0f); //bullets last 10 seconds
			bullet_objects_ptr_->push_back(bullet);
			last_bullet_fire_time_ = 0.0f;
		}
		if (last_missile_fire_time_ > 5.0f) {
			MissileObject* missile = new MissileObject(position_ + glm::vec3((-(0.25 * sin((rotation_ + 90.0f) * 3.14159 / 180))), ((0.25 * cos((rotation_ + 90.0f) * 3.14159 / 180))), 0), *missileTex_, *size_, false, 1, rotation_ + 90.0f, 3.0f, player_, "enemy");
			missile->SetScale(0.5f);
			missile->SetDuration(5.0f); //missiles last 5 seconds
			missile_objects_ptr_->push_back(missile);
			last_missile_fire_time_ = 0.0f;
		}
	}
	
	//static function to setup all of the turret's static variables
	void Turret::SetupBullets(std::vector<BulletObject*>* ptr, std::vector<MissileObject*>* misPtr, GLuint* bulletTex, GLuint* missileTex, int* size, GameObject* player) {
		Turret::bullet_objects_ptr_ = ptr;
		Turret::missile_objects_ptr_ = misPtr;
		Turret::bulletTex_ = bulletTex;
		Turret::size_ = size;
		Turret::missileTex_ = missileTex;
		Turret::player_ = player;
	}
	
}