#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <glm/glm.hpp>
#define GLEW_STATIC
#include <GL/glew.h>

#include <iostream>

#include "shader.h"

namespace game {

    /*
        GameObject is responsible for handling the rendering and updating of objects in the game world
        The update method is virtual, so you can inherit from GameObject and override the update functionality (see PlayerGameObject for reference)
    */
    class GameObject {

        public:
            // Constructor
            GameObject (const glm::vec3& position, GLuint texture, bool collidable, int tiles);

            // Update the GameObject's state. Can be overriden for children
            virtual void Update(double delta_time);

            virtual void Render(Shader &shader, glm::mat4 view_matrix);

            // Getters
            inline glm::vec3& GetPosition(void) { return position_; }
            inline float GetScale(void) { return scale_; }
            inline glm::vec3& GetVelocity(void) { return velocity_; }
            inline bool GetCollidable (void) { return collidable_; }
            inline float GetRotation (void) { return rotation_; }
            inline glm::mat4& GetTransfMatrix (void) { return transformation_matrix_; };

            // Setters
            virtual inline void SetPosition(const glm::vec3& position) { position_ = position; }

            //sepperating scale into different componnents
            void SetScale(float scale) { 
                scale_ = scale; 
                scale_x_ = scale;
                scale_y_ = scale;
            }
            void SetScalex(float scale) { scale_x_ = scale; } //scale just the y
            void SetScaley(float scale) { scale_y_ = scale; } //scale just the x


            inline void SetTexture (GLuint texture) { texture_ = texture; }
            inline void SetRotation (float rotation) { rotation_ = rotation; }
            inline void SetVelocity(const glm::vec3& velocity) { velocity_ = velocity; }
            inline void SetCollidable (bool c) { collidable_ = c; }


        protected:
            // Object's Transform Variables
            // TODO: Add more transformation variables
            glm::vec3 position_;
            
            float scale_; //this variable is deprecated
            float scale_x_; //the scale of the x
            float scale_y_; //the scale of the y

            glm::vec3 velocity_;
            float rotation_;

            // Object's Transformation Matrix
            glm::mat4 transformation_matrix_;

            // Object's details
            GLint num_elements_;

            // Object's texture reference
            GLuint texture_;

            // Object's collidability
            bool collidable_;

            // Number of tiles to represent object with
            int tiles_;

    }; // class GameObject

} // namespace game

#endif // GAME_OBJECT_H_
