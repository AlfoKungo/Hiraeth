#include "hrth_pch.h"
#include "main_ui.h"
#include <functional>

namespace hiraeth {
	namespace ui {

		MainUi::MainUi(input::Keyboard* kb)
			: m_Kb(kb),
		m_CharacterStats(),
			m_StatsStruct(m_CharacterStats.getStatsStruct_()),
			m_Layer(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag")),
		m_GraphicGroup(new graphics::Group(maths::vec2(-555, -450))),
		m_LabelGroup(new graphics::Group(maths::vec2(-555, -450)))
		{
			EventManager *m_EventManager = EventManager::Instance();
			m_EventManager->subscribe(StatsUpdate, this, &MainUi::StatsUpdated);
			fill_stats_group();
			m_Job = new graphics::Label("arial", 11, m_StatsStruct->Job, { -730, -429 }, 0xff70cdd0);
			m_Layer.add(m_Job);
			m_Name = new graphics::Label("arial", 11, m_StatsStruct->Name, { -730, -441 }, 0xffffffff);
			m_Layer.add(m_Name);
			m_TypingLabel = new graphics::Label("arial", 15, m_TextWritten, { -730, -407 }, 0xff000000);
			m_Layer.add(m_TypingLabel);
			m_CursorSprite = new graphics::Sprite({ -730, -407 }, 2, 12, 0xff000000);
			m_Layer.add(m_CursorSprite);

			//m_Layer.add(new graphics::Label("arial", 11, m_StatsStruct->Job, { -730, -429 }, 0xff70cdd0));
			//m_Layer.add(new graphics::Label("arial", 11, m_StatsStruct->Name, { -730, -441 }, 0xffffffff));
			m_Layer.add(m_LabelGroup);
			m_Layer.add(new graphics::Sprite(maths::vec2(-551, -447), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.gaugeCover.png")));
			m_Layer.add(m_GraphicGroup);
			m_Layer.add(new graphics::Sprite(maths::vec2(-580, -448), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.gaugeBackgrd.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-797, -430), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.lvCover.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-799, -448), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.lvBacktrnd.png")));
			m_Layer.add(new graphics::Sprite(maths::vec2(-800, -450), graphics::TextureManager::Load("Assets/UI/MainUi/mainBar.backgrnd.png")));

			setTypingState(false);
		}

			//void MainUi::StatsUpdatedT(int X)
			//{
			//	m_CharacterStats.getStatsStruct_()->Hp = X;
			//	StatsUpdated();
			//}

		void MainUi::update()
		{
			m_Layer.update();
		}

		void MainUi::draw() const
		{
			m_Layer.render();
		}

		void MainUi::StatsUpdated()
		{
			fill_stats_group();
		}

		void MainUi::ButtonClicked(input::Key control)
		{
			if (control == GLFW_KEY_ENTER)
			{
				if (m_IsTyping)
					if (!m_TextWritten.empty())
					{
						NetworkManager::Instance()->sendChatMsg(m_TextWritten);
						CharManager::Instance()->writeSay(m_TextWritten);
					}
				setTypingState(!m_IsTyping);
			}
			else
			{
				if (control >= 32 && control <= 126)
				{
					m_TextWritten.insert(m_TextWritten.begin() + m_TextCursor, char(control));
					m_TypingLabel->setText(m_TextWritten);
					m_TextCursor++;
				}
				else
				{
					switch (control)
					{
					case GLFW_KEY_ESCAPE:
						setTypingState(false);
						break;
					case GLFW_KEY_RIGHT:
						m_TextCursor = std::min<unsigned int>(m_TextWritten.size(), m_TextCursor + 1);
						break;
					case GLFW_KEY_LEFT:
						if (m_TextCursor > 0)
							m_TextCursor -= 1;
						break;
					case GLFW_KEY_BACKSPACE:
						if (m_TextCursor > 0)
						{
							m_TextWritten.erase(m_TextWritten.begin() + m_TextCursor - 1);
							m_TypingLabel->setText(m_TextWritten);
							m_TextCursor--;
						}
						break;
					case GLFW_KEY_DELETE:
						if (m_TextCursor < m_TextWritten.size())
						{
							m_TextWritten.erase(m_TextWritten.begin() + m_TextCursor);
							m_TypingLabel->setText(m_TextWritten);
						}
						break;
					case GLFW_KEY_END:
						m_TextCursor = m_TextWritten.size();
						break;
					case GLFW_KEY_HOME:
						m_TextCursor = 0;
						break;
					default:
						break;
					}
				}
			}
			updateCursorPos();
		}

		void MainUi::setTypingState(bool new_state)
		{
			m_IsTyping = new_state;
			m_Kb->setTyping(this, m_IsTyping);
			if (new_state)
			{
				m_CursorSprite->setColor(0xff000000);
			}
			else
			{
				m_TextWritten.erase();
				m_TypingLabel->setText(m_TextWritten);
				m_IsTyping = false;
				m_TextCursor = 0;
				m_CursorSprite->setColor(0x00000000);
			}
		}

		void MainUi::updateCursorPos()
		{
			auto text_size = m_TypingLabel->getSize(m_TextCursor);
			m_CursorSprite->setPosition(maths::vec2{-730 + text_size.x, -407});
		}

		void MainUi::fill_stats_group()
		{
			m_GraphicGroup->clear();
			m_LabelGroup->clear();
			float relativeHp = float(m_StatsStruct->Hp) / m_StatsStruct->MaxHp;
			m_Hp = new graphics::Sprite(maths::vec2(3, 19), relativeHp * 139, 12, 0xff0000ff);
			m_GraphicGroup->add(m_Hp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Hp, m_StatsStruct->MaxHp), { 140, 21 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
			float relativeMp = float(m_StatsStruct->Mp) / m_StatsStruct->MaxMp;
			m_Mp = new graphics::Sprite(maths::vec2(174, 19), relativeMp * 139, 12, 0xfff5b626);
			m_GraphicGroup->add(m_Mp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Mp, m_StatsStruct->MaxMp), { 311, 21 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
			float relativeExp = float(m_StatsStruct->Exp) / m_StatsStruct->MaxExp;
			m_Exp = new graphics::Sprite(maths::vec2(3, 3), relativeExp * 310, 12, 0xff18dba9);
			m_GraphicGroup->add(m_Exp);
			m_LabelGroup->add(new graphics::Label("arial", 11, create_stats_string(m_StatsStruct->Exp, m_StatsStruct->MaxExp), { 311, 5 }, 0xffffffff, graphics::Label::Alignment::RIGHT));
		}

		std::string MainUi::getJobName(unsigned int job_id)
		{
			switch (job_id)
			{
			case 0:
				return "Novice";
			case 1:
				return "Berserker";
			case 2:
				return "Crusader Knight";
			case 3:
				return "Wizard";
			case 4:
				return "Rogue";
			case 5:
				return "Archer";
			case 6:
				return "Forest Fighter";
			default:
				return "";
			}
		}

		std::string MainUi::create_stats_string(unsigned int value, unsigned int maxValue) const
		{
				return "[" + std::to_string(value) + " / " + std::to_string(maxValue) + "]";
		}
	}
}