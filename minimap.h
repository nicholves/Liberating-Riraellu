#ifndef MINIMAP_H_
#define MINIMAP_H_

#include <vector>

#include "ui_element.h"
#include "enemy_game_object.h"
#include "base.h"


#define MINIMAP_RANGE 3.0f

namespace game {

    // Inherits from GameObject
    class Minimap : public UI_Element {

    public:
        Minimap(const glm::vec3& position, GLuint texture, int tiles);

        

        void Update(double delta_time) override;

        void Render(Shader& shader, glm::mat4 view_matrix) override;

        //setup function
        static void Setup(std::vector<EnemyGameObject*>* enemy_vec, std::vector<Base*>* base_vec, GLuint* enemy_tex, GLuint* player_tex);

    protected:

        void ClearChildren(); // deletes all the children

        // enemy game objects list
        static std::vector<EnemyGameObject*>* enemy_objects_;

        // enemy game objects list
        static std::vector<Base*>* bases_;

        //list of ui element children
        std::vector<UI_Element*> children_;


        static GLuint* player_tex_; //the texture of the player
        static GLuint* enemy_tex_; //the texture of an enemy radar return


    }; // class Minimap

} // namespace game

#endif // MINIMAP_H_