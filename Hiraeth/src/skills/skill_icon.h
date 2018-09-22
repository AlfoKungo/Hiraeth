#pragma once
#include "graphics/sprite.h"
//#include <graphics/sprite.h>
#include "graphics/texture_manager.h"
#include "srl/skill_data.h"

//#include "srl/skill_data.h"

namespace hiraeth {
	namespace skills {

		class SkillIcon : public graphics::Sprite
		{
		private:
			graphics::Sprite m_Bg;
			SRL::SkillInfo m_Info;
		public:
			SkillIcon(SRL::SkillData data, maths::vec2 pos)
				: Sprite(pos, graphics::TextureManager::Load(data.skill_info.name, data.texture_data)),
				m_Bg(pos + maths::vec2{-1,-2}, graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.skill1.png")),
			m_Info(data.skill_info)
			{
				
			}

			float get_mp_con() const
			{
				return m_Info.mp_con;
			}

			SRL::SkillInfo* get_skill_info()
			{
				return &m_Info;
			}

			void draw(graphics::Renderer* renderer) const override
			{
				m_Bg.draw(renderer);
				Sprite::draw(renderer);
			}

		};

	}
}
