#ifndef ENEMY_GAME_OBJECT_H_
#define ENEMY_GAME_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"

namespace game {

	// Inherits from GameObject
	class EnemyGameObject : public GameObject {

		public:
			EnemyGameObject(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health);
			virtual ~EnemyGameObject();

			// Update function for moving the enemy object and swapping its state
			virtual void Update (double delta_time) override;

			// Setters and getters
			void setState (int state_);
			int getState ();
			void setTarget (glm::vec3 target);
			inline double getSightingRange() { return sighting_range_; }
			inline int getHealth (void) { return health_; }
			inline void setHealth (int health) { health_ = health; }

			// Functions for different states of movement
			virtual void Patrol (double delta_time);
			virtual void Pursue (double delta_time);

	protected:
		
		int state;
		float patrol_radius = 2.0;
		double internal_timer = 0;
		glm::vec3 patrol_point;
		glm::vec3 target;
		double sighting_range_; //sighting range is the range at which this enemy will notice the player and switch to it's other state (patrol / pursue)
		int health_;
		static PlayerGameObject* player_; //a pointer to the player object for firing purposes
		
	}; // Class EnemyGameObject

}


#endif
