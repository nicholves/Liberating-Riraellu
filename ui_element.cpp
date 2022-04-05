#include "ui_element.h"

namespace game {

	/*
		PlayerGameObject inherits from GameObject
		
	*/

	GameObject* UI_Element::player_ = NULL;

	UI_Element::UI_Element(const glm::vec3& position, GLuint texture, int tiles)
		: GameObject(position, texture, false, tiles) {
		offset_ = position; //the offset defaults to the position
		absolute_ = false; //by default ui_elements are relative to the player
	}

	//sets the position based on the offset initially given (this function likely won't do anything, don't use it outside the class)
	inline void UI_Element::SetPosition(const glm::vec3& position) {
		position_ = position + offset_;
	}

	//if the position of the ui_element should be absolute set it's position properly else, it just updates in-place
	void UI_Element::Update(double delta_time) {
		if (!absolute_) {
			position_ = player_->GetPosition() + offset_;
			GameObject::Update(delta_time);
		} else{
			GameObject::Update(delta_time);
		}
	}


	//static function to setup all of the static variables
	void UI_Element::Setup(GameObject* player) {
		UI_Element::player_ = player;
	}
}