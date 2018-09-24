#pragma once
#include "graphics/sprite.h"
//#include <graphics/sprite.h>
#include "graphics/texture_manager.h"
#include "srl/skill_data.h"
#include "graphics/label.h"

//#include "srl/skill_data.h"

namespace hiraeth {
	namespace skills {

		class SkillIcon : public graphics::Sprite
		{
		private:
			graphics::Sprite m_Bg;
			graphics::Texture* m_Icon;
			graphics::Texture* m_IconDisabled;
			graphics::Texture* m_IconMouseOver;
			unsigned int m_Level{0};
			graphics::Label m_LevelLabel;
			graphics::Label m_NameLabel;


			SRL::SkillInfo m_Info;
		public:
			SkillIcon(SRL::SkillData data, maths::vec2 pos)
				: Sprite(pos),
				m_Bg(pos + maths::vec2{ -1,-2 }, graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.skill1.png")),
				m_Info(data.skill_info),
				m_Icon(graphics::TextureManager::Load(data.skill_info.name, data.icon_data.icon)),
				m_IconDisabled(graphics::TextureManager::Load(data.skill_info.name + "_disabled", data.icon_data.icon_disabled)),
				m_IconMouseOver(graphics::TextureManager::Load(data.skill_info.name + "_mouse_over", data.icon_data.icon_mouse_over)),
				m_LevelLabel("arial", 12, m_Level, pos + maths::vec2{ 40, 3 }, 0xff000000, graphics::Label::Alignment::LEFT),
				m_NameLabel("arial", 12, data.skill_info.name, pos + maths::vec2{ 40, 21 }, 0xff000000, graphics::Label::Alignment::LEFT)

			{
				updateIconState(false);
			}

			SRL::SkillInfo* get_skill_info()
			{
				return &m_Info;
			}

			void draw(graphics::Renderer* renderer) const override
			{
				m_Bg.draw(renderer);
				m_NameLabel.draw(renderer);
				m_LevelLabel.draw(renderer);
				Sprite::draw(renderer);
				//m_LevelLabel.draw(renderer);
			}

			void inc_level()
			{
				set_level(m_Level + 1);
			}

			void set_level(unsigned int new_level)
			{
				if (new_level <= m_Info.max_level)
				if (!(new_level > m_Info.max_level))
				{
					m_Level = new_level;
					updateIconState(true);
					m_LevelLabel.setText(m_Level);
				}
			}

			maths::Rectangle getSpUpButtonBounds()
			{
				return getBounds();
			}

			void updateIconState(bool is_mouse_on)
			{
				if (m_Level != 0)
				{
					if (is_mouse_on)
						setTexture(m_IconMouseOver);
					else
						setTexture(m_Icon);
				}
				else
					setTexture(m_IconDisabled);
			}

		};

	}
}
