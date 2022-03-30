#ifndef MISSILE_H_
#define MISSILE_H_

#include "bullet_object.h"
#include "game_object.h"

namespace game {

	// Inherits from BulletObject
	class MissileObject : public BulletObject {

	public:
		MissileObject(const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, float direction, float speed, GameObject* target, std::string from);


		// Update function for updating the position of the missile
		void Update(double delta_time) override;

	private:
		glm::vec3 acceleration_;
		GameObject* target_; //the target the missile flies after
	};

}


#endif