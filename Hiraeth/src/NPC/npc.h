#pragma once
#include "game/base_creature.h"
#include "srl/deserial.h"
#include "srl/npc_data.h"
#include "graphics/label.h"
#include "graphics/sprite.h"
#include "graphics/box_textured_full.h"
#include "graphics/text_box.h"
#include <game\base_creature.h>

namespace hiraeth {
	namespace npc {
		class Npc : public game::BaseCreature
		{
		private:
			SRL::NpcData m_Data;
			unsigned int m_NpcIndex;
			std::vector<std::string> m_SayLines;
			ATimer m_SayTimer;
			bool m_IsSay{ true };
			unsigned int m_SayLineNum{0};
			graphics::TextBox<graphics::BoxTexturedFull> m_SayBox;

			//graphics::Label m_TalkLabel;
			//bool m_IsTalking;
		public:
			Npc(SRL::NpcData npc_data, map::MapLayer* map_layer, input::Keyboard* kb, unsigned int npc_index)
				: BaseCreature(maths::Rectangle{ maths::vec2{npc_data.info.npc_x_value, 0}, maths::vec2(50, 50) }, map_layer,
					25, 13,
					npc_data.info.npc_foothold),
				m_Data(npc_data),
				m_NpcIndex(npc_index),
				m_SayLines(npc_data.info.say_lines),
				m_SayBox{ {}, "", 0xffeeeeee, "Assets/UI/ChatBox/pet.39" }
				//m_TalkLabel{"arial", 16, "hey im your npc", { 40, -10 }, 0xff000000, graphics::Label::Alignment::LEFT},
				//m_IsTalking{false}
			{
				m_StatesRenderables[game::StcStand].push_back(std::make_unique<graphics::SpritedRenderable>(
					maths::vec2(), m_Data.npc_frames_amount, 0.6f, false,
					graphics::TextureManager::Load(m_Data.info.npc_name + "_stand", m_Data.texture_data)));
					m_SayTimer.reSet(5.0f);
					m_SayBox.setText(m_SayLines.at(0));
			}

			Npc(unsigned int npc_index, map::MapLayer* map_layer, input::Keyboard* kb) :
				Npc(SRL::deserial<SRL::NpcData>("npc", npc_index), map_layer, kb, npc_index)
			{
			}

			void draw(graphics::Renderer* renderer) const override
			{
				BaseCreature::draw(renderer);
				if (m_IsSay)
				{
					renderer->push(m_TransformationMatrix * maths::mat4::Translate(maths::vec3{ 20, 74 }));
					m_SayBox.draw(renderer);
					renderer->pop();
				}
			}

			void update() override
			{
				BaseCreature::update();
				if (m_SayTimer.hasExpired())
				{
					m_SayTimer.reSet(5.0f);
					//m_IsSay = !m_IsSay;
					if (m_IsSay)
					{
						m_SayLineNum++;
						m_SayLineNum %= m_SayLines.size();
						m_SayBox.setText(m_SayLines.at(m_SayLineNum));
					}
				}
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
