#include "collectible_object.h"

namespace game {
	CollectibleObject::CollectibleObject (const glm::vec3& position, GLuint texture, GLint num_elements, bool collidable, int tiles, int type)
		: GameObject (position, texture, num_elements, collidable, tiles) {
		type_ = type;
	}
}
