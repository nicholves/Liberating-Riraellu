#define GLM_FORCE_RADIANS
#include <glm/gtc/matrix_transform.hpp>

#include "particle_system.h"

namespace game {

ParticleSystem::ParticleSystem(const glm::vec3 &position, GLuint texture, GameObject *parent)
	: GameObject(position, texture, false, 1){

    parent_ = parent;
    lifetime = NULL;
}


void ParticleSystem::Update(double delta_time) {

	// Call the parent's update method to move the object in standard way, if desired
	GameObject::Update(delta_time);
    if (lifetime != NULL) {
        lifetime -= (float)delta_time;
    }
}


void ParticleSystem::Render(Shader& shader, glm::mat4 view_matrix, double current_time) {

    // Bind the particle texture
    glBindTexture(GL_TEXTURE_2D, texture_);

    // Set up the shader
    shader.Enable();
    shader.SetParticleAttributes();

    // Setup the scaling matrix for the shader
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0f), glm::vec3(scale_, scale_, 1.0));

    // Setup the rotation matrix for the shader
    glm::mat4 rotation_matrix = glm::rotate(glm::mat4(1.0f), rotation_, glm::vec3(0.0, 0.0, 1.0));

    // Set up the translation matrix for the shader
    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0f), position_);
    float speed;

    glm::mat4 transformation_matrix;
    if (parent_ != NULL) {

        // Set up the parent transformation matrix
        glm::mat4 parent_rotation_matrix = glm::rotate(glm::mat4(1.0f), parent_->GetRotation(), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 parent_translation_matrix = glm::translate(glm::mat4(1.0f), parent_->GetPosition());
        glm::mat4 parent_transformation_matrix = parent_translation_matrix * parent_rotation_matrix;
    
        // Setup the transformation matrix for the shader
        transformation_matrix = parent_transformation_matrix * translation_matrix * rotation_matrix * scaling_matrix;

        speed = glm::length(parent_->GetVelocity());
    }
    else {
        transformation_matrix = translation_matrix * rotation_matrix * scaling_matrix;
    }
    // Set the transformation matrix in the shader
    shader.SetUniformMat4("transformation_matrix", transformation_matrix);

    // Set the time in the shader
    shader.SetUniform1f("time", (float)current_time);

    shader.SetUniformMat4("view_matrix", view_matrix);

    shader.SetUniform1f("speed", speed);

    // Draw the entity
    glDrawElements(GL_TRIANGLES, shader.GetParticleSize(), GL_UNSIGNED_INT, 0);
}

} // namespace game
