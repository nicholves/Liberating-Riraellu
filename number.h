#ifndef NUMBER_H_
#define NUMBER_H_

#include <vector>

#include "ui_element.h"

namespace game {

    // Inherits from UI_Element
    class Number : public UI_Element {

    public:
        Number(const glm::vec3& position, GLuint texture, int tiles, int value);


        void Update(double delta_time) override;

        static void SetupTextures(std::vector<GLuint> , int*); //sets up static variables

        void Render(Shader& shader) override;

    protected:

        void ClearChildren(); // deletes all the children

        static std::vector<GLuint> text_arr_; //a pointer to game's bullet_objects vector so turrets can create bullets

        std::vector<UI_Element*> children_; //all the numbers excluding the first digit are contained in this vector

        int value_; // the value of the number

        static int* size_; //the texture size given by game

    }; // class Number

} // namespace game

#endif // NUMBER_H_