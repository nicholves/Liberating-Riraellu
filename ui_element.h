#ifndef UI_ELEMENT_H_
#define UI_ELEMENT_H_

#include <vector>

#include "game_object.h"

namespace game {

    // Inherits from GameObject
    class UI_Element : public GameObject {

    public:
        UI_Element(const glm::vec3& position, GLuint texture, GLint num_elements, int tiles);

        inline void SetPosition(const glm::vec3& position) override;

        inline void SetAbsolute(bool b) { absolute_ = b; } //set whether the position of the ui element should be relative to the player or fixed in an absolute position

        void Update(double delta_time) override;

        //setup function
        static void Setup(GameObject* player);

        

    protected:
        glm::vec3 offset_; //the offset from the player used if absolute_ == false

        static GameObject* player_; //a pointer to the player object

        bool absolute_; //position should be taken as absolute coordinates rather than relative

        
    }; // class UIElement

} // namespace game

#endif // UI_ELEMENT_H_