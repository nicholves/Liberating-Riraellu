#include "minimap.h"

namespace game {
	std::vector<EnemyGameObject*>* Minimap::enemy_objects_ = NULL;
	std::vector<Base*>* Minimap::bases_ = NULL;
	GLuint* Minimap::enemy_tex_ = NULL;
	GLuint* Minimap::player_tex_ = NULL;

	Minimap::Minimap(const glm::vec3& position, GLuint texture, int tiles) : UI_Element(position, texture, tiles) {}

	void Minimap::Setup(std::vector<EnemyGameObject*>* enemy_vec, std::vector<Base*>* base_vec, GLuint* enemy_tex, GLuint* player_tex) {
		enemy_objects_ = enemy_vec;
		bases_ = base_vec;
		enemy_tex_ = enemy_tex;
		player_tex_ = player_tex;
	}

	void Minimap::ClearChildren() {
		for (int i = 0; i < children_.size(); ++i) {
			delete children_[i];
		}
		children_.clear();
	}

	void Minimap::Update(double delta_time) {
		ClearChildren();

		float range_modifier = scale_ * MINIMAP_RANGE;

		//adding chevron representing player
		UI_Element* chevron = new UI_Element(glm::vec3(position_.x + 0.25f, position_.y - 0.3f, 0), *player_tex_, 1);
		chevron->SetAbsolute(true);
		chevron->SetScale(0.5f);
		chevron->SetRotation(player_->GetRotation());
		children_.push_back(chevron);

		for (int i = 0; i < enemy_objects_->size(); ++i) {
			EnemyGameObject* enemy = (*enemy_objects_)[i];


			float dist = glm::length(enemy->GetPosition() - player_->GetPosition());

			if (dist <= range_modifier) {
				glm::vec3 relative = enemy->GetPosition() - player_->GetPosition();
				relative = relative * (1/range_modifier);

				UI_Element* enemy = new UI_Element(glm::vec3(position_.x + 0.25f + relative.x, position_.y - 0.3f + relative.y, 0), *enemy_tex_, 1);
				enemy->SetAbsolute(true);
				enemy->SetScale(0.3f);
				children_.push_back(enemy);
			}
		}


		for (int i = 0; i < bases_->size(); ++i) {
			Base* base = (*bases_)[i];


			float dist = glm::length(base->GetPosition() - player_->GetPosition());

			if (dist <= range_modifier) {
				glm::vec3 relative = base->GetPosition() - player_->GetPosition();
				relative = relative * (1 / range_modifier);

				UI_Element* enemy = new UI_Element(glm::vec3(position_.x + 0.25f + relative.x, position_.y - 0.3f + relative.y, 0), *enemy_tex_, 1);
				enemy->SetAbsolute(true);
				enemy->SetScale(0.7f);
				children_.push_back(enemy);
			}
		}
		

		UI_Element::Update(delta_time);


		for (int i = 0; i < children_.size(); ++i) {
			children_[i]->Update(delta_time);
		}
	}

	void Minimap::Render(Shader& shader, glm::mat4 view_matrix) {
		//renders minimap and all it's children
		for (int i = 0; i < children_.size(); ++i) {
			children_[i]->Render(shader, view_matrix);
		}
		GameObject::Render(shader, view_matrix);
	}
}