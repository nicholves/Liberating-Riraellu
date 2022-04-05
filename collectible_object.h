#ifndef COLLECTIBLE_OBJECT_H_
#define COLLECTIBLE_OBJECT_H_

#include "game_object.h"

namespace game {

	class CollectibleObject : public GameObject {

		public:
			CollectibleObject (const glm::vec3& position, GLuint texture, bool collidable, int tiles, int type_);
			inline int getType () { return type_; }

		private:

			/***************************************************
			* Type determines the effect from collecting it.
			* Legend:
			* 1 = Shield
			***************************************************/

			int type_;
	};

}

#endif
