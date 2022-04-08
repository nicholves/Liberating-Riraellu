#ifndef HELICOPTER_BLADES_H_
#define HELICOPTER_BLADES_H_

#include "game_object.h"
#include "player_game_object.h"

namespace game {

	// Inherits from GameObject
	class HelicopterBlades : public GameObject {

		public:
			HelicopterBlades (const glm::vec3& position, GLuint texture, bool collidable, int tiles);

			// Update function for moving the helicopter blades around
			void Update (double delta_time) override;

			// Setters
			void setParent (GameObject* p);

		protected:
			GameObject* parent_;
			glm::mat4 parent_matrix;
	};

}


#endif 
