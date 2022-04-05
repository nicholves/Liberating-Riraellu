#ifndef HEALTHBAR_H_
#define HEALTHBAR_H_

#include "ui_element.h"

namespace game {

    // Inherits from GameObject
    class Healthbar : public UI_Element {

    public:
        Healthbar(const glm::vec3& position, GLuint texture, int tiles);

        void Update(double delta_time) override;

    private:

    }; // class healthbar

} // namespace game

#endif // HEALTHBAR_H_