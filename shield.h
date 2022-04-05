#ifndef SHIELD_H_
#define SHIELD_H_

#include "game_object.h"

namespace game {
	class Shield : public GameObject {
		
	public:
		Shield (const glm::vec3& position, GLuint texture, bool collidable, int tiles, double);

		// Update function for orbiting the shield orbs
		void Update (double delta_time) override;

		// Setters
		void setParent (GameObject* p);

	protected:	
		GameObject* parent_;
		glm::mat4 parent_matrix;
	};
}

#endif