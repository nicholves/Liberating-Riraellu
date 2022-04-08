#ifndef TURRET_H_
#define TURRET_H_

#include "enemy_game_object.h"
#include "bullet_object.h"
#include "missile.h"
#include "player_game_object.h"
#include <vector>
#include "particle_system.h"
#include "game_object.h"
#include "base.h"

namespace game {

	// Inherits from EnemyGameObject
	class Turret : public EnemyGameObject {

	public:
		Turret(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health, Base* parent);
		~Turret();

		// Update function for moving the enemy object and swapping its state
		void Update(double delta_time) override;


		// Functions for different states of movement
		void Patrol(double delta_time) override;
		void Pursue(double delta_time) override;

		//setup function
		static void SetupBullets(std::vector<BulletObject*>* ptr, std::vector<MissileObject*>* misPtr, std::vector<GameObject*>* particlePtr, GLuint* bulletTex, GLuint* missileTex, GLuint* particleTex, GameObject* player);

	protected:

		Base* parent_;

		double last_bullet_fire_time_; //timer counting up since last bullet was fired

		double last_missile_fire_time_; //timer counting up since last missile was fired

		static std::vector<BulletObject*>* bullet_objects_ptr_; //a pointer to game's bullet_objects vector so turrets can create bullets

		static std::vector<MissileObject*>* missile_objects_ptr_; //a pointer to game's missile_objects vector so turrets can create bullets

		static std::vector<GameObject*>* particle_objects_ptr_; //a pointer to the particle system array in game
		
		static GLuint* bulletTex_; //the texture of the bullet

		static GLuint* missileTex_; //the texture of the missile

		static GLuint* particleTex_; //the texture of the particle

	}; // Class EnemyGameObject

}


#endif
