#ifndef BASE_H_
#define BASE_H_

#include "game_object.h"
#include "enemy_game_object.h"


#include <vector>

#define NUM_TURRETS

namespace game {

    // Inherits from GameObject
    class Base : public GameObject {

    public:
        Base(const glm::vec3& position, GLuint texture, bool collidable, int tiles, int turrets, float scale);

        inline void removeTurret() { --active_turrets_; }
        inline bool IsCapturable() { return capturable_; }
        inline bool GetAllegiance() { return is_enemy_; }

        inline void SetCapturable(bool value) { capturable_ = value; }
        inline void SetAllegiance(bool value) { is_enemy_ = value; }



        void Update(double delta_time) override;

        //setup function
        static void SetupBases(std::vector<EnemyGameObject*>* enemy_objects_, GLuint* turretTex_);

    private:
        // Holds the number of active turrets
        int active_turrets_;

        bool capturable_;

        //whether the base is hostile or not
        bool is_enemy_;

        // enemy game objects list
        static std::vector<EnemyGameObject*>* enemy_objects_;

        static GLuint* turretTex_; //the texture of the turret

    }; // class PlayerGameObject

} // namespace game

#endif // BASE_H_