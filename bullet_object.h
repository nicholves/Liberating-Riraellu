#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "game_object.h"

namespace game {

	// Inherits from GameObject
	class BulletObject : public GameObject {

		public:
			BulletObject (const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction, float speed);

			// Getters
			double getTimer ();
			float getDuration() { return duration_; }
			
			//Setters
			void SetDuration(float duration) { duration_ = duration; }

			// Update function for updating the position of the bullet
			void Update (double delta_time) override;

		protected:
			// Internal clock counts until the bullet needs to despawn
			float duration_timer = 0;

			float speed_; //speed is the magnitude for the velocity of bullets

			float duration_; //the duration that the bullet object lasts
	};

}


#endif
