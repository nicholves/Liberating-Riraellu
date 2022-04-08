#ifndef DRONE_H_
#define DRONE_H_

#include "enemy_game_object.h"

#define DETECTION_RADIUS 10.0f
#define PURSUE_SPEED 2.5f

namespace game {

	class Drone : public EnemyGameObject {
	public:
		Drone(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int health);

		void Pursue(double delta_time) override;

	protected:
		bool CheckDir();

	};
}

#endif