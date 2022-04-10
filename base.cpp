#include "base.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "turret.h"

namespace game {
	/*
		Base inherits from GameObject
	*/

	std::vector<EnemyGameObject*>* Base::enemy_objects_ = NULL;
	GLuint* Base::turretTex_ = NULL;

	Base::Base(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int turrets, float scale)
		: GameObject(position, texture, collidable, tiles) {
		srand((unsigned int)time(NULL));
		SetScale(scale);
		active_turrets_ = turrets;
		is_enemy_ = true;
		capturable_ = false;

		//rotation_ = rand() % 360; // for random orientation

		for (int i = 0; i < turrets; ++i) {
			float angle = (float)(rand() % 360);
			float radius = scale_ * 0.75f;

			float x = cos(angle) * radius;
			float y = sin(angle) * radius;

			// Setup enemy objects
			enemy_objects_->push_back(new Turret(glm::vec3(x + position_.x, y + position_.y, 0.0f), *turretTex_, false, 1, 5, this));
		}
	}

	void Base::SetupBases(std::vector<EnemyGameObject*>* enemy_objects, GLuint* turretTex) {
		turretTex_ = turretTex;
		enemy_objects_ = enemy_objects;
	}


	void Base::Update(double delta_time) {
		if (active_turrets_ <= 0 && is_enemy_) { capturable_ = true; }
		GameObject::Update(delta_time);
	}
}