#pragma once
#include "graphics/sprited_renderable.h"
#include "game/monsters/monster_data_manager.h"
#include "game/base_creature.h"

namespace hiraeth {
	namespace editor {

		class MobAnimation : public graphics::Renderable
		{
			game::StanceState m_StanceState{ game::Stand };
			std::map<game::StanceState, std::vector<std::unique_ptr<Renderable>>> m_StatesRenderables;
			maths::mat4 m_TransformationMatrix;
		public:
			MobAnimation(const SRL::MonsterData& monster_data)
				: m_TransformationMatrix(maths::mat4::Translate(maths::vec3{}))
			{
				auto mtd = SRL::deserial<SRL::MonsterTexturesData>("monster",
					(100 + monster_data.TextureIndex));
				m_StatesRenderables[game::Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
					monster_data.StatsStruct.Name + "_stand", mtd.creature_sprites.sprited_data[SRL::MoveState::Stand]));
				m_StatesRenderables[game::Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
					monster_data.StatsStruct.Name + "_walk", mtd.creature_sprites.sprited_data[SRL::MoveState::Walk]));
				m_StatesRenderables[game::Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
					monster_data.StatsStruct.Name + "_hit", mtd.creature_sprites.sprited_data[SRL::MoveState::Hit]));
				m_StatesRenderables[game::Die].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(),
					monster_data.StatsStruct.Name + "_die", mtd.creature_sprites.sprited_data[SRL::MoveState::Die], true));
			}
			MobAnimation(unsigned int mob_id)
				: MobAnimation(game::MonsterDataManager::Get(mob_id))
			{

			}
			void update()
			{
				for (auto& renderable : m_StatesRenderables.at(m_StanceState))
					renderable->update();
			}

			void draw(graphics::Renderer* renderer) const
			{
				//if (m_Direction == Left)
				//	renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)));
				//else
				//	renderer->push(m_TransformationMatrix * maths::mat4::Translate(maths::vec3(m_Org)) * 
				//		maths::mat4::Scale(maths::vec3(-1, 1, 1)) * maths::mat4::Translate(- maths::vec3(2 * m_Org)));

				renderer->push(m_TransformationMatrix * maths::mat4::Translate(-maths::vec3(m_Org)));

				for (auto& renderable : m_StatesRenderables.at(m_StanceState))
					renderable->draw(renderer, m_Color);
				renderer->pop();
			}

			void changeStanceState(game::StanceState new_stance)
			{
				m_StanceState = new_stance;
				for (auto& rend : m_StatesRenderables[new_stance])
					rend->resetState();
			}
		};
	}
}
