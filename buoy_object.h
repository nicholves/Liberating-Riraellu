#ifndef BUOY_OBJECT_H_
#define BUOY_OBJECT_H_

#include "game_object.h"

namespace game {
	class BuoyObject : public GameObject {
		
		public:
			BuoyObject (const glm::vec3& position, GLuint texture, bool collidable, int tiles, double mass);

			inline double getMass () { return mass_; }
			inline void setMass (double mass) { mass_ = mass; }

			void Update (double delta_time);

		private:
			double mass_;
	};



}


#endif
