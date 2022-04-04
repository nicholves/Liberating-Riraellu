#include <string>

#include "number.h"

namespace game {
	//static variables stuff
	std::vector<GLuint> Number::text_arr_; 
	int* Number::size_ = NULL;
	

	Number::Number(const glm::vec3& position, GLuint texture, GLint num_elements, int tiles, int value) : UI_Element(position, texture, num_elements, tiles) {
		offset_ = position; //the offset defaults to the position
		value_ = value; //the value of the number
	}

	//static function to initialize all the static variables a number needs
	void Number::SetupTextures(std::vector<GLuint> vec, int* size) {
		Number::text_arr_ = vec; //the array of textures which holds digit 0-9
		size_ = size;
	}


	void Number::Update(double delta_time) {

		position_ = player_->GetPosition() + offset_;
		GameObject::Update(delta_time);
		
		ClearChildren();


		int num_digits = (int) std::to_string(value_).length(); // gets the number of digits of the integer

		int first_dig = (int)(value_ / pow(10, num_digits - 1)); // get the most significant digit of the number

		SetTexture(text_arr_[first_dig]); // the first number has the texture of the first digit subscript of the array


		/*
			the following algorithm gets each digit in a number
			then creates aditional ui elements with the correct textures to display a number as a whole
		*/
		for (int i = 0; i < num_digits - 1; ++i) {
			int cur_value = value_;
			int digit_val = cur_value % 10;
			cur_value = cur_value / 10;
			for (int j = 0; j < i; ++j) {
				digit_val = cur_value % 10; // calculates the value of a certain digit
				cur_value = cur_value / 10; //gets rid of 1 digit
			}


			float x_posit = position_.x + ((num_digits - i - 1) * 0.3f); // gets the xposition of letters
			UI_Element* new_child = new UI_Element(glm::vec3(x_posit, position_.y, position_.z), text_arr_[digit_val], *size_, 1);


			new_child->SetScale(scale_);
			new_child->SetAbsolute(true); //the position of children numbers should be absolute and not relative to anything


			children_.push_back(new_child);

			new_child->Update(delta_time);
		}
	}


	void Number::ClearChildren() {
		//deletes all children of the number
		for (int i = 0; i < children_.size(); ++i) {
			delete children_[i];
		}
		children_.clear();
	}

	void Number::Render(Shader& shader) {
		GameObject::Render(shader);
		//renders both the number and all it's children
		for (int i = 0; i < children_.size(); ++i) {
			children_[i]->Render(shader);
		}
	}
}