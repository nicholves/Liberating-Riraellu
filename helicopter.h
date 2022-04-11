#ifndef HELICOPTER_H_
#define HELICOPTER_H_

#include "enemy_game_object.h"
#include "bullet_object.h"
#include "helicopter_blades.h"
#include <vector>

namespace game {
	class Helicopter : public EnemyGameObject {
	public:
		Helicopter (const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health);
		~Helicopter ();

		// Update function for moving the enemy object and swapping states
		void Update (double delta_time);

		// Functions for the different states, dictating velocity or position
		void Patrol (double delta_time);
		void Pursue (double delta_time) override;

		// Set up bullets for the whole class
		static void SetupBullets (std::vector<BulletObject*>* ptr, GLuint* bulletTex, GameObject* player);
		inline void setBlades (HelicopterBlades* b) { blades = b; }

		void Render (Shader&, glm::mat4);

	protected:
		double last_bullet_fire_time_; //timer counting up since last bullet was fired
		HelicopterBlades* blades;
		static std::vector<BulletObject*>* bullet_objects_ptr_; //a pointer to game's bullet_objects vector so turrets can create bullets
		static GLuint* bulletTex_; //the texture of the bullet

	}; // Class Helicopter

}


#endif
