#include "player_game_object.h"
#include <iostream>

namespace game {
	

/*
	PlayerGameObject inherits from GameObject
	It overrides GameObject's update method, so that you can check for input to change the velocity of the player
*/

PlayerGameObject::PlayerGameObject(const glm::vec3 &position, GLuint texture, bool collidable, int tiles, double mass)
	: GameObject (position, texture, collidable, tiles) {
	mass_ = mass;
	missile_cooldown_ = 0;
	num_shield = 0;
	invincible_timer = 2.0f;
	health = MAX_HEALTH;
	cloaked = false;
	cloak_timer = 0.0;	
	texture_ = texture;
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
	if (velocity_.x > MAX_SPEED) {
		velocity_.x = MAX_SPEED;
	}
	else if (velocity_.x < -MAX_SPEED) {
		velocity_.x = -MAX_SPEED;
	}

	if (velocity_.y > MAX_SPEED) {
		velocity_.y = MAX_SPEED;
	} else if (velocity_.y < -MAX_SPEED) {
		velocity_.y = -MAX_SPEED;
	}
	if (invincible_timer > 2.0f) {
		collidable_ = true;
	}
	else {
		//This is janky, but making sure that the user knows the player is invincible by blinking is important
		if (invincible_timer > 1.5f) {
			SetTexture(1);
		}
		else if (invincible_timer > 1.0f) {
			SetTexture(37);
		}
		else if (invincible_timer > 0.5f) {
			SetTexture(1);
		}
		else {
			SetTexture(37);
		}
		
		std::cout << "Player is invincible. Collidable in: " << 2.0f - invincible_timer << std::endl;
		
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

void PlayerGameObject::Render(Shader &shader, glm::mat4 view_matrix) {


	// Bind the entity's texture
	glBindTexture(GL_TEXTURE_2D, texture_);

	shader.Enable();
	shader.SetSpriteAttributes();

	glm::mat4 transformation_matrix = GetTransfMatrix();
	// TODO: Add other types of transformations

	// Set the transformation matrix in the shader
	shader.SetUniformMat4("transformation_matrix", transformation_matrix);	

	shader.SetUniformMat4("view_matrix", view_matrix);

	// Draw the entity
	glDrawElements(GL_TRIANGLES, shader.GetSpriteSize(), GL_UNSIGNED_INT, 0);


}

} // namespace game
