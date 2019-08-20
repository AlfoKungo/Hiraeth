#pragma once
#include "graphics/sprite.h"
//#include <graphics/sprite.h>
#include "graphics/texture_manager.h"
#include "graphics/label.h"
#include "srl/skill_data.h"

//#include "srl/skill_data.h"

namespace hiraeth {
	namespace skills {

		class SkillIcon : public graphics::Sprite
		{
		private:
			unsigned int m_SkillId{};
			graphics::Sprite m_Bg;
			graphics::Texture* m_Icon;
			graphics::Texture* m_IconDisabled;
			graphics::Texture* m_IconMouseOver;
			unsigned int m_Level{ 0 };
			graphics::Label m_LevelLabel;
			graphics::Label m_NameLabel;
			//std::map<SRL::SkillAnimationTypes, SRL::FullAnimationData> m_AnimationData;
			SRL::AnimationMap m_AnimationData;;
			SRL::SkillInfo m_Info;

		public:
			SkillIcon(unsigned int skill_id, SRL::SkillData data, maths::vec2 pos)
				: Sprite(pos),
				m_SkillId(skill_id),
				m_Bg(pos + maths::vec2{ -1,-2 }, graphics::TextureManager::Load("Assets/UI/Skills/Skills.main.skill1.png")),
				m_Icon(graphics::TextureManager::Load(data.skill_info.name, data.icon_data.icon)),
				m_IconDisabled(graphics::TextureManager::Load(data.skill_info.name + "_disabled", data.icon_data.icon_disabled)),
				m_IconMouseOver(graphics::TextureManager::Load(data.skill_info.name + "_mouse_over", data.icon_data.icon_mouse_over)),
				m_LevelLabel("arial", 12, m_Level, pos + maths::vec2{ 40, 3 }, 0xff000000, graphics::Label::Alignment::LEFT),
				m_NameLabel("arial", 12, data.skill_info.name, pos + maths::vec2{ 40, 21 }, 0xff000000, graphics::Label::Alignment::LEFT),
				m_AnimationData(data.animation_map),
				m_Info(data.skill_info)
			{
				updateIconState(false);
			}
			SkillIcon(unsigned int skill_id, maths::vec2 pos)
				: SkillIcon(skill_id, SRL::deserial<SRL::SkillData>("skills", skill_id), pos)
			{}

			SRL::SkillInfo* get_skill_info()
			{
				return &m_Info;
			}

			SRL::AnimationMap* get_animation_data()
			{
				return &m_AnimationData;
			}

			void draw(graphics::Renderer* renderer) const override
			{
				m_Bg.draw(renderer);
				m_NameLabel.draw(renderer);
				m_LevelLabel.draw(renderer);
				Sprite::draw(renderer);
			}
			unsigned int getSkillId() const { return m_SkillId; }
			unsigned int getSkillLvl() const
			{
				return m_Level;
			}
			bool inc_level()
			{
				return set_level(m_Level + 1);
			}

			bool set_level(unsigned int new_level)
			{
				if (new_level <= m_Info.max_level)
				{
					m_Level = new_level;
					updateIconState(false);
					m_LevelLabel.setText(m_Level);
					return true;
				}
				return false;
			}

			maths::Rectangle getSpUpButtonBounds() const
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
