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

            
            inline void SetMissileCooldown(double c) { missile_cooldown_ = c; }

            inline double getMass () { return mass_; }
            inline double getMissileCooldown(void) { return missile_cooldown_; }

        private:
            // For collision response
            double mass_;

            //for missile timing
            double missile_cooldown_;
    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_