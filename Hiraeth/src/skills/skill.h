#pragma once
#include "srl/skill_data.h"
#include "graphics/renderable.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace skills {

		//class Skill : public graphics::Sprite
		//{
		//private:
		//	SRL::SkillInfo m_Data;
		//	graphics::Renderable * m_Rend;
		//public:
		//	Skill(SRL::SkillData data, maths::vec2 pos)
		//		: Sprite(pos, graphics::TextureManager::Load(data.skill_info.name, data.texture_data)),
		//	m_Data(data.skill_info)
		//	{
		//	m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2(), mtd.creature_sprites.sprited_data[SRL::MoveState::Stand], 
		//		graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_stand", mtd.textures_dict[SRL::MoveState::Stand])));
		//		
		//	}

		//	float get_mp_con() const
		//	{
		//		return m_Data.mp_con;
		//	}
		//};

	}
}
