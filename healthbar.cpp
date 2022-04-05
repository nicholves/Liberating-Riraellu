#include "healthbar.h"
#include <glm/gtc/matrix_transform.hpp>

namespace game {

	Healthbar::Healthbar(const glm::vec3& position, GLuint texture, int tiles)
		: UI_Element(position, texture, tiles) {
		offset_ = position; //the offset defaults to the position
	}


	void Healthbar::Update(double delta_time) {
		position_ = player_->GetPosition() + offset_;

		float correction = (0.15f * scale_y_) - 0.30f; 
		/* 
			y = 0.15x - 0.3 = the position modification I want based on scale for the healthbar, this account for scal_y_ squeezing in both directions
			I want healthbar to only shrink in one way. So we decrease the position of the bar to account for this and give the ilusion it only shrinks in one direction
		*/
		position_.y += correction;
		GameObject::Update(delta_time);
	}

}