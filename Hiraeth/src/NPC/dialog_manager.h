#pragma once
#include "graphics/label.h"
#include "graphics/layers/layer.h"
#include "game/character.h"
#include "UI/ui_basic/ui_button_basic.h"
#include "UI/ui_quests.h"

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
			//std::vector<std::string> m_Strings;
			size_t m_StringIndex;
			std::vector<SRL::DialogStruct> m_Strings;
		public:
			DialogTree() 
				//: m_Strings{"They tryna find tupac", "Don't let them find tupac", "La da da da"},
				: 
				m_StringIndex(0)
			{
				
			}

			void set_dialog(unsigned int npc_id, unsigned int dialog_index)
			{
				auto npc_data = SRL::deserial<SRL::NpcData>("npc", npc_id);
				m_Strings = npc_data.info.dialog_tree[dialog_index];
				m_StringIndex = 0;
			}
			
			SRL::DialogStruct getNextText()
			{
				if (m_Strings.size() > m_StringIndex)
					return m_Strings[m_StringIndex++];
				return {};
			}
		};

		class DialogManager : public Updatable, public input::MouseEvent
		{
		private:
			graphics::Layer<graphics::Renderable> m_TextBoxes;
			graphics::TGroup<ui::UiButton> m_Buttons;
			graphics::Label m_InText;
			bool m_IsDraw{ false };
			DialogTree m_DialogTree;
			game::Character* m_Character;
			//ui::UiQuests* m_UiQuests;
			network::ClientHandler* m_ClientHandler;
			maths::Rectangle m_Rec{ -140,80,280,80 };
			unsigned int m_CurrentNpc{0}, m_CurrentDialog{0};
			//unsigned int m_CurrentDialog{0};
		public:
			DialogManager(game::Character * character)
				: m_TextBoxes(new graphics::Shader("Assets/shaders/basic.vert", "Assets/shaders/basic.frag"), false),
			//m_InText{nullptr},
				m_InText{ "arial", 16, "", { -130, 130 }, 0xffffffff, graphics::Label::Alignment::LEFT },
			m_Character(character),
			//m_UiQuests(ui_quests),
			m_ClientHandler(character->m_ClientHandler)
			{
				//m_InText = new graphics::Label{ "arial", 16, "", { -130, 130 }, 0xffffffff, graphics::Label::Alignment::LEFT };
				//m_TextBoxes.add(m_InText);
				//m_TextBoxes.add(new graphics::Sprite{ maths::vec2(-140, 80), 280, 80, 0x88331a00 });
				//m_TextBoxes.add_ref(&m_Buttons);

				EventManager *m_EventManager = EventManager::Instance();
				m_EventManager->createEvent<unsigned int, unsigned int>(DialogStart);
				m_EventManager->subscribe(DialogStart,this, &DialogManager::startDialog);
			}

			void createDialog(const std::string& text)
			{
				m_IsDraw = true;
				m_InText.setText(text);
			}

			bool nextDialog()
			{
				m_IsDraw = true;
				const auto next_dialog{ m_DialogTree.getNextText() };
				if (!next_dialog.text.empty())
				{
					m_InText.setText(next_dialog.text);
					m_Character->setStuck(true);
					updateButtons(next_dialog.buttons);
					return true;
				}  
				m_TextBoxes.clear();
				m_Buttons.clear();
				m_Character->setStuck(false);
				return false;
			}

			void updateButtons(const std::vector<char> buttons)
			{
				m_Buttons.clear();
				for (const auto button_type : buttons)
				{
					switch (button_type)
					{
					case SRL::RESPONSE_TYPE_NEXT:
						m_Buttons.add(new ui::UiButtonBasic{ maths::vec2{100,100},"Dialog","BtOK",std::bind(&DialogManager::nextClicked, this) });
						break;
					case SRL::RESPONSE_TYPE_ACCEPT:
						m_Buttons.add(new ui::UiButtonBasic{ maths::vec2{100,100},"Dialog","BtAccept",std::bind(&DialogManager::acceptQuest, this) });
						break;
					case SRL::RESPONSE_TYPE_CANCEL:
						m_Buttons.add(new ui::UiButtonBasic{ maths::vec2{20,100},"Dialog","BtCancel",std::bind(&DialogManager::nextClicked, this) });
						break;
					case SRL::RESPONSE_TYPE_OK:
						m_Buttons.add(new ui::UiButtonBasic{ maths::vec2{100,100},"Dialog","BtOK",std::bind(&DialogManager::nextClicked, this) });
						break;
					default:
						break;
					}
				}
			}

			void nextClicked()
			{
				nextDialog();
			}

			void acceptQuest()
			{
				nextDialog();
				m_ClientHandler->sendQuestAccepted(m_CurrentNpc, m_CurrentDialog);
			}
			
			void startDialog(unsigned int npc_id, unsigned int dialog_index)
			{
				m_TextBoxes.add_ref(&m_InText);
				m_TextBoxes.add(new graphics::Sprite{ maths::vec2(-140, 80), 280, 80, 0x88331a00 });
				m_TextBoxes.add_ref(&m_Buttons);
				m_CurrentNpc = npc_id;
				m_CurrentDialog = dialog_index;
				m_DialogTree.set_dialog(npc_id, dialog_index);
				//m_CurrentDialog = dialog_index;
				nextDialog();
			}

			//void sendStartDialog(unsigned int npc_index)
			//{
			//	m_CurrentNpc = npc_index;
			//	m_ClientHandler->sendNpcClick(npc_index);
			//}

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
				auto mouse_pos = view::Camera::mouse_to_screen_position({mx, my});
				for (auto& button : m_Buttons.m_Renderables)
				{
					if (button != nullptr)
					if (button->isContains(mouse_pos))
					{
						button->onClick(mouse_pos);
						//if (!nextDialog())
						//	m_UiQuests->setQuestAsActive(0);
						//return true;
					}
				}
				//if (!nextDialog())
				//	m_UiQuests->setQuestAsActive(0);
				return true;
			}

			bool rightButtonClicked(float mx, float my) override
			{
				return false;
			}

			bool leftButtonReleased(float mx, float my)  override
			{
				return false;
			}

			bool mouseMove(float pmx, float pmy, float mx, float my)  override
			{
				const auto mouse_pos = view::Camera::mouse_to_screen_position({mx, my});
				//const maths::vec2 rel_mouse_pos{ getRelativeLocation({mx, my}) };
				for (auto & button : m_Buttons.m_Renderables)
				{
					//button->onMove(rel_mouse_pos);
					button->onMove(mouse_pos);
				}
				return false;
			}

		//maths::vec2 getRelativeLocation(maths::vec2 mouse_pos) const
		//{
		//	maths::Rectangle rec = m_Rec;
		//	maths::vec2 relPos = maths::vec2(mouse_pos.x - 800, 450 - mouse_pos.y) - rec.position;
		//	return relPos;
		//}
			bool is_window_contains(maths::vec2 mouse_pos) const override
			{
				return maths::Rectangle{ 650, 280, 290, 80 }.Contains(mouse_pos);
			}
		};
	}
}
