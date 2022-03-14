#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

	// Inherits from GameObject
	class EnemyGameObject : public GameObject {

		public:
			EnemyGameObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles);

			// Update function for moving the enemy object and swapping its state
			void Update (double delta_time) override;

			// Setters and getters
			void setState (int state_);
			int getState ();
			void setTarget (glm::vec3 target);

			// Functions for different states of movement
			void Patrol (double delta_time);
			void Pursue (double delta_time);

	protected:
		
		int state;
		float patrol_radius = 2.0;
		double internal_timer = 0;
		glm::vec3 patrol_point;
		glm::vec3 target;
		
	}; // Class EnemyGameObject

}


#endif
