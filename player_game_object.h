#ifndef PLAYER_GAME_OBJECT_H_
#define PLAYER_GAME_OBJECT_H_

#include "game_object.h"
#define MAX_HEALTH 10
#define MAX_SHIELD 20
#define CLOAK_LENGTH 10
#define INVINCIBLE_TIME 2
#define MAX_SPEED 5.0f
#define MIN_SPEED 1.0f
#define STANDARD_SPEED 3.0f
namespace game {
    
    // Inherits from GameObject
    class PlayerGameObject : public GameObject {

        public:
            PlayerGameObject(const glm::vec3 &position, GLuint texture, bool collidable, int tiles, double mass);

            // Update function for moving the player object around
            void Update(double delta_time) override;

            inline void addShield (int n) { num_shield += n; if (num_shield >= MAX_SHIELD) { num_shield = MAX_SHIELD; } }
            inline void minusShield (int damage) { num_shield -= damage; if (num_shield < 0) { num_shield = 0; } }
            inline void resetIFrame() { invincible_timer = 0; }
            void addHealth (int);
            
            inline void SetMissileCooldown(double c) { missile_cooldown_ = c; }

            inline void cloak () { cloaked = true; cloak_timer = 0.0; }
            inline void uncloak () { cloaked = false; }
            inline bool getCloaked (void) { return cloaked; }
            inline double getMass () { return mass_; }
            inline double getMissileCooldown(void) { return missile_cooldown_; }
            inline int getNumShield(void) { return num_shield; }
            inline int getHealth (void) { return health; }
            inline bool IsInvincible() { return (invincible_timer < 2); }

        private:
            // For collision response
            double mass_;

            //for missile timing
            double missile_cooldown_;
            
            //Player keeps track of how many shields they have
            int num_shield;

            //Lets player stay invincible for a short period of time
            float invincible_timer;

            // Holds the player's health
            int health;

            // Cloak variables
            float cloak_timer;
            bool cloaked;

    }; // class PlayerGameObject

} // namespace game

#endif // PLAYER_GAME_OBJECT_H_