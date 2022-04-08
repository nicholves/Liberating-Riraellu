#ifndef BULLET_OBJECT_H_
#define BULLET_OBJECT_H_

#include "game_object.h"
#include "player_game_object.h"
#include <string>
namespace game {

	// Inherits from GameObject
	class BulletObject : public GameObject {

		public:
			BulletObject(const glm::vec3& position, GLuint texture, bool collidable, int tiles, float direction, float speed, std::string from);

			// Getters
			double getTimer ();
			float getDuration() { return duration_; }
			virtual int GetDamage() { return damage_; }
			inline std::string GetFrom(void) { return from_; }
			
			//Setters
			void SetDuration(float duration) { duration_ = duration; }
			static void SetDamageBullet(int dmg);

			// Update function for updating the position of the bullet
			void Update (double delta_time) override;

		protected:
			// Internal clock counts until the bullet needs to despawn
			float duration_timer = 0;

			float speed_; //speed is the magnitude for the velocity of bullets

			float duration_; //the duration that the bullet object lasts

			std::string from_;

			static int damage_;
	};

}


#endif
