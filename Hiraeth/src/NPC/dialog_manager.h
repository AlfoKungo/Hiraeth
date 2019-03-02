#pragma once
#include "graphics/label.h"
#include "graphics/layers/layer.h"
#include "game/character.h"

//#include <winsock2.h>
//#include <Ws2tcpip.h>

namespace hiraeth {
	namespace npc {

		class IDialogInitiator
		{
			unsigned int dialogReturnValue();
		};
		
		//struct DialogBit
		//{
		//	std::string Text;
		//	unsigned int NextDialog{0};
		//};

		class DialogTree
		{
		private:
			//std::map<unsigned int, DialogBit> m_Dialogs;
			std::vector<std::string> m_Strings;
			size_t m_StringIndex;
		public:
			DialogTree() 
				: m_Strings{"They tryna find tupac", "Don't let them find tupac", "La da da da"},
				m_StringIndex(0)
			{
				
			}

			std::string getNextText()
			{
				if (m_Strings.size() > m_StringIndex)
					return m_Strings[m_StringIndex++];
				return "";
			}
		};

		class DialogManager : public Updatable, public input::MouseEvent
		{
		private:
			graphics::Layer<graphics::Renderable> m_TextBoxes;
			graphics::Label* m_InText;
			bool m_IsDraw{false};
			DialogTree m_DialogTree;
			game::Character * m_Character;
		public:
			DialogManager(game::Character * character)
				: m_TextBoxes(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), false),
			m_InText{nullptr},
			m_Character(character)
			{
				m_InText = new graphics::Label{ "arial", 16, "", { -130, 130 }, 0xffffffff, graphics::Label::Alignment::LEFT };
				m_TextBoxes.add(m_InText);
				m_TextBoxes.add(new graphics::Sprite{ maths::vec2(-140, 80), 280, 80, 0x88331a00 });
			}

			void createDialog(const std::string& text)
			{
				m_IsDraw = true;
				m_InText->setText(text);
			}

			void nextDialog()
			{
				m_IsDraw = true;
				const std::string next_text{ m_DialogTree.getNextText() };
				if (!next_text.empty())
				{
					m_InText->setText(next_text);
					m_Character->setStuck(true);
				}
				else
				{
					m_TextBoxes.clear();
					m_Character->setStuck(false);
				}
			}

			void draw() const
			{
				if (m_IsDraw)
					m_TextBoxes.render();
			}
			void update() override
			{
				//m_TextBoxes.
			}

			bool leftButtonClicked(float mx, float my) override
			{
				nextDialog();
				return true;
			}

			bool rightButtonClicked(float mx, float my) override
			{
				return false;
			}

			bool leftButtonReleased(float mx, float my) const override
			{
				return false;
			}

			bool mouseMove(float pmx, float pmy, float mx, float my) const override
			{
				return false;
			}

			bool is_window_contains(maths::vec2 mouse_pos) const override
			{
				return maths::Rectangle{ 650, 280, 290, 80 }.Contains(mouse_pos);
			}
		};
	}
}
