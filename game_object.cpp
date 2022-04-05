#include <glm/gtc/matrix_transform.hpp>

#include "game_object.h"

namespace game {

GameObject::GameObject(const glm::vec3 &position, GLuint texture, bool collidable, int tiles) 
{

    // Initialize all attributes
    position_ = position;
    scale_ = 1.0;
    rotation_ = 0.0;
    velocity_ = glm::vec3(0.0f, 0.0f, 0.0f); // Starts out stationary
    texture_ = texture;
    collidable_ = collidable;
    tiles_ = tiles;
}

void GameObject::Update(double delta_time) {

    // Update object position with Euler integration
    position_ += velocity_ * ((float) delta_time);

    transformation_matrix_ = glm::translate (glm::mat4 (1.0f), position_)* glm::scale (glm::mat4 (1.0f), glm::vec3 (scale_, scale_, 1.0))* glm::rotate (glm::mat4 (1.0f), rotation_, glm::vec3 (0.0f, 0.0f, 1.0f));

}


void GameObject::Render(Shader &shader) {

    // Bind the entity's texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    shader.Enable();
    shader.SetSpriteAttributes();

    glm::mat4 transformation_matrix = GetTransfMatrix ();
    // TODO: Add other types of transformations

    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set the number of tiles
    shader.SetUniform1i ("num_tiles", tiles_);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader.GetSpriteSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
