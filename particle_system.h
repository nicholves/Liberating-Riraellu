#ifndef PARTICLE_SYSTEM_H_
#define PARTICLE_SYSTEM_H_

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class ParticleSystem : public GameObject {

        public:
            ParticleSystem(const glm::vec3 &position, GLuint texture, GameObject *parent);

            void Update(double delta_time) override;

            void Render(Shader &shader, glm::mat4 view_matrix, double current_time);

            inline void SetLifetime(float lt) { lifetime = lt; }

            inline float GetLifetime() { return lifetime; }

        private:
            GameObject *parent_;

            float lifetime;

    }; // class ParticleSystem

} // namespace game

#endif // PARTICLE_SYSTEM_H_
