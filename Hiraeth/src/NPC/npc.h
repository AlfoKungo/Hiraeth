#pragma once
#include "game/base_creature.h"
#include "srl/deserial.h"
#include "srl/npc_data.h"
#include "graphics/label.h"

namespace hiraeth {
	namespace npc {
		class Npc : public game::BaseCreature
		{
		private:
			SRL::NpcData m_Data;
			//graphics::Label m_TalkLabel;
			//bool m_IsTalking;
			unsigned int m_NpcIndex;
		public:
			Npc(SRL::NpcData npc_data, map::MapLayer* map_layer, input::Keyboard* kb, unsigned int npc_index)
				: BaseCreature(maths::Rectangle{ maths::vec2{npc_data.info.npc_x_value, 0}, maths::vec2(50, 50) }, map_layer,
					25, 13,
					npc_data.info.npc_foothold),
				m_Data(npc_data),
				m_NpcIndex(npc_index)
				//m_TalkLabel{"arial", 16, "hey im your npc", { 40, -10 }, 0xff000000, graphics::Label::Alignment::LEFT},
				//m_IsTalking{false}
			{
				m_StatesRenderables[game::StcStand].push_back(std::make_unique<graphics::SpritedRenderable>(
					maths::vec2(), m_Data.npc_frames_amount, 0.6f, false,
					graphics::TextureManager::Load(m_Data.info.npc_name + "_stand", m_Data.texture_data)));
			}

			Npc(unsigned int npc_index, map::MapLayer* map_layer, input::Keyboard* kb) :
				Npc(SRL::deserial<SRL::NpcData>("npc", npc_index), map_layer, kb, npc_index)
			{
			}

			void draw(graphics::Renderer* renderer) const override
			{
				BaseCreature::draw(renderer);
				//if (m_IsTalking)
				//{
				//	m_TalkLabel.draw(renderer);
				//}
			}

			void onNpcClick()
			{
				//m_IsTalking = true;
				//move(maths::vec2(10, 0));
			}
			unsigned int getNpcId() const { return m_NpcIndex; }
		};
	}
}
