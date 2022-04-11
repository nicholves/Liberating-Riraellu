#include "turret.h"
#include <glm/gtc/matrix_transform.hpp> 


namespace game {
	
	//initializing static variables to default values
	std::vector<BulletObject*>* Turret::bullet_objects_ptr_ = NULL;
	std::vector<MissileObject*>* Turret::missile_objects_ptr_ = NULL; 
	std::vector<GameObject*>* Turret::particle_objects_ptr_ = NULL;
	PlayerGameObject* Turret::player_ = NULL;
	GLuint* Turret::bulletTex_ = 0;
	GLuint* Turret::missileTex_ = 0;
	GLuint* Turret::particleTex_ = 0;
	
	/*
		Turret inherits from EnemyGameObject
		It overrides EnemyGameObject's update method, so that the object can be updated according to its states
	*/

	Turret::Turret(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health, Base* parent)
		: EnemyGameObject(position, texture, collidable, tiles, health) {
		state = 0; patrol_point = glm::vec3(position_.x - patrol_radius, position_.y, position_.z);
		//setting up required fields
		last_bullet_fire_time_ = 0;
		last_missile_fire_time_ = 0;
		sighting_range_ = 4.5;
		parent_ = parent;
	}

	Turret::~Turret() {
		parent_->removeTurret();
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
		rotation_ = (float)(internal_timer * 57.3);
	}

	// Object rotates to face towards target and fires on a given interval
	void Turret::Pursue(double delta_time) {
		float angle = atan((abs(target.y - position_.y)) / (abs(target.x - position_.x)));
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

		rotation_ = (angle / 3.14159f) * 180.0f + 180.0f;

		if (last_bullet_fire_time_ > 0.05) {
			BulletObject* bullet = new BulletObject(position_ + glm::vec3((-(0.25 * sin((rotation_ + 90.0f) * 3.14159 / 180))), ((0.25 * cos((rotation_ + 90.0f) * 3.14159 / 180))), 0), *bulletTex_, true, 1, rotation_ + 90.0f, 5.0f, "enemy");
			bullet->SetScale(0.3f);
			bullet->SetDuration(3.0f); //bullets last 3 seconds
			bullet_objects_ptr_->push_back(bullet);
			last_bullet_fire_time_ = 0.0f;
		}
		if (last_missile_fire_time_ > 5.0f) {
			MissileObject* missile = new MissileObject(position_ + glm::vec3((-(0.25 * sin((rotation_ + 90.0f) * 3.14159 / 180))), ((0.25 * cos((rotation_ + 90.0f) * 3.14159 / 180))), 0), *missileTex_, false, 1, rotation_ + 90.0f, 3.0f, player_, "enemy");
			missile->SetScale(0.5f);
			missile->SetDuration(5.0f); //missiles last 5 seconds
			missile_objects_ptr_->push_back(missile);
			last_missile_fire_time_ = 0.0f;

			
			//Attach particle system to missile
			int size = (int)(missile_objects_ptr_->size() - 1);
			//MissileObject* endMissile = &(missile_objects_ptr_[size]);
			//GameObject* particles = new ParticleSystem(glm::vec3(0.0f, -0.5f, 0.0f), *particleTex_, missile_objects_ptr_[size]);
			GameObject* particles = new ParticleSystem(glm::vec3(0.3f, 0.0f, 0.0f), *particleTex_, missile);
			particles->SetScale(0.2f);
			particles->SetRotation(90);
			particle_objects_ptr_->push_back(particles);
			
		}
	}
	
	//static function to setup all of the turret's static variables
	void Turret::SetupBullets(std::vector<BulletObject*>* ptr, std::vector<MissileObject*>* misPtr, std::vector<GameObject*>* particlePtr, GLuint* bulletTex, GLuint* missileTex, GLuint* particleTex, GameObject* player) {
		Turret::bullet_objects_ptr_ = ptr;
		Turret::missile_objects_ptr_ = misPtr;
		Turret::particle_objects_ptr_ = particlePtr;
		Turret::bulletTex_ = bulletTex;
		Turret::missileTex_ = missileTex;
		Turret::particleTex_ = particleTex;
		Turret::player_ = (PlayerGameObject*)(player);
	}
	
}