#include "collectible_object.h"

namespace game {
	CollectibleObject::CollectibleObject (const glm::vec3& position, GLuint texture, bool collidable, int tiles, int type)
		: GameObject (position, texture, collidable, tiles) {
		type_ = type;
	}
}
