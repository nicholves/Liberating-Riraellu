#ifndef MISSILE_H_
#define MISSILE_H_

#include "bullet_object.h"
#include "game_object.h"
#include "particle_system.h"
#include <vector>


namespace game {

	// Inherits from BulletObject
	class MissileObject : public BulletObject {

	public:
		MissileObject(const glm::vec3& position, GLuint texture, bool collidable, int tiles, float direction, float speed, GameObject* target, std::string from);


		// Update function for updating the position of the missile
		void Update(double delta_time) override;

	private:
		glm::vec3 acceleration_;
		GameObject* target_; //the target the missile flies after

		static std::vector<GameObject*>* particle_objects_ptr_; //a pointer to the particle system array in game

		static GLuint* particleTex_; //the texture of the particle
	};

}


#endif