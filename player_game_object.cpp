#include "player_game_object.h"

namespace game {
	

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, GLuint texture, GLint num_elements, bool collidable, int tiles, double mass)
	: GameObject (position, texture, num_elements, collidable, tiles) {
	mass_ = mass;
	missile_cooldown_ = 0;
	num_shield = 0;
	invincible_timer = 2.0f;
	health = MAX_HEALTH;
	cloaked = false;
	cloak_timer = 0.0;
}

void PlayerGameObject::addHealth (int n) {
	health += n;
	if (health >= MAX_HEALTH ) {
		health = MAX_HEALTH;
	}
}

// Update function for moving the player object around
void PlayerGameObject::Update(double delta_time) {

	// The player's velocity is limited, but not in a smart way.
	// The player will go faster diagonally, but still limited.
	if (velocity_.x > 5) {
		velocity_.x = 5;
	}
	else if (velocity_.x < -5) {
		velocity_.x = -5;
	}

	if (velocity_.y > 5) {
		velocity_.y = 5;
	} else if (velocity_.y < -5) {
		velocity_.y = -5;
	}
	if (invincible_timer > 2.0f) {
		collidable_ = true;
	}
	else {
		collidable_ = false;
		invincible_timer += (float)delta_time;
	}

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
	missile_cooldown_ += delta_time;
	cloak_timer += delta_time;
	if (cloak_timer >= CLOAK_LENGTH) {
		uncloak ();
	}
}

} // namespace game
