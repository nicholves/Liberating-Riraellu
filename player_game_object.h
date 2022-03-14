#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, GLuint texture, GLint num_elements, bool collidable, int tiles, double mass);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            inline double getMass () { return mass_; }

        private:
            // For collision response
            double mass_;
    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_