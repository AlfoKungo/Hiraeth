#pragma once
#include "game/base_creature.h"
#include "srl/deserial.h"
#include "srl/npc_data.h"

namespace hiraeth
{
	namespace npc
	{
		class Npc : public game::BaseCreature, public input::MouseEvent
		{
		private:
			SRL::NpcData m_Data;

		public:
			Npc(SRL::NpcData npc_data, map::MapLayer* map_layer, input::Keyboard* kb)
				: BaseCreature(maths::Rectangle{ maths::vec2{npc_data.npc_info.npc_x_value, 0}, maths::vec2(50, 50) }, map_layer,
					std::make_unique<float>(25), std::make_unique<float>(13),
					npc_data.npc_info.npc_foothold),
				m_Data(npc_data)
			{

				kb->registerToMouse(this);

				m_StatesRenderables[game::Stand].push_back(std::make_unique<graphics::SpritedRenderable>(
					maths::vec2(), m_Data.npc_frames_amount, 0.6f, false,
					graphics::TextureManager::Load(m_Data.npc_info.npc_name + "_stand", m_Data.texture_data), 0));
				//m_StatesRenderables[game::Walk].push_back(std::make_unique<graphics::SpritedRenderable>(
				//	maths::vec2(), mtd.frames_amount.walk_frames, 0.2f, true,
				//	graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_walk", mtd.walk_texture), 0));
				//m_StatesRenderables[game::Jump].push_back(std::make_unique<graphics::SpritedRenderable>(
				//	maths::vec2(), mtd.frames_amount.hit_frames, 0.2f, true,
				//	graphics::TextureManager::Load(monster_data.StatsStruct.Name + "_hit", mtd.hit_texture), 0));
			}

			Npc(unsigned int npc_index, map::MapLayer* map_layer, input::Keyboard* kb) :
				Npc(SRL::deserial<SRL::NpcData>("serialized/npc.data", npc_index), map_layer, kb)
			{
			}

			void leftButtonClicked(float mx, float my) override
			{
				
			}
			void rightButtonClicked(float mx, float my) override {}
			void leftButtonReleased(float mx, float my) const override {}
			void mouseMove(float pmx, float pmy, float mx, float my) const override {}
			bool is_window_contains(maths::vec2 mouse_pos) const override
			{
				const maths::vec2 relative_mouse_pos = view::Camera::mouse_to_map_position(mouse_pos);
				return getBounds().Contains(relative_mouse_pos);
			}
		};
	}
}
