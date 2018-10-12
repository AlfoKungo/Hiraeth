#pragma once
#include "ui_window.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"
#include "ui_keyconfig_icon.h"
#include "keyboard/key.h"

namespace hiraeth {
	namespace ui {

		enum IconType
		{
			BasicUiIcon,
			SkillUiIcon,
		};
		class UiKeyConfig : public UiWindow
		{
		private:
			std::map<input::KeyCode, maths::Rectangle> m_KeysLocation;
			//std::map<input::KeyCode, KeyConfigIcon * > m_KeysIcons;
			std::vector<KeyConfigIcon *> m_KeysIcons;
			KeyConfigIcon * m_HoldIcon;
			maths::vec2 m_OldItemPos;
			input::Keyboard * m_Keyboard;
		public:
			UiKeyConfig(maths::vec2 pos, UiKey control_key, input::Keyboard* keyboard)
				: UiWindow(maths::Rectangle{ pos, {622,374} }, control_key),
				m_HoldIcon{ nullptr },
				m_Keyboard{ keyboard }
			{

				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(0, 0), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(6, 117), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd2.png")));
				m_BackgroundGroup->add(new graphics::Sprite(maths::vec2(10, 145), graphics::TextureManager::Load("Assets/UI/KeyConfig/KeyConfig.backgrnd3.png")));
				std::vector<unsigned int> l1{ GLFW_KEY_Z, GLFW_KEY_X, GLFW_KEY_C, GLFW_KEY_V, GLFW_KEY_B, GLFW_KEY_N,
					GLFW_KEY_M, GLFW_KEY_COMMA, GLFW_KEY_PERIOD, GLFW_KEY_SLASH };
				std::vector<unsigned int> l2{ GLFW_KEY_A, GLFW_KEY_S, GLFW_KEY_D, GLFW_KEY_F, GLFW_KEY_G, GLFW_KEY_H,
					GLFW_KEY_J, GLFW_KEY_K, GLFW_KEY_L, GLFW_KEY_SEMICOLON, GLFW_KEY_GRAVE_ACCENT };
				std::vector<unsigned int> l3{ GLFW_KEY_Q, GLFW_KEY_W, GLFW_KEY_E, GLFW_KEY_R, GLFW_KEY_T, GLFW_KEY_Y,
					GLFW_KEY_U, GLFW_KEY_I, GLFW_KEY_O, GLFW_KEY_P, GLFW_KEY_LEFT_BRACKET };
				for (int index = 0; index < l1.size(); ++index)
					m_KeysLocation[l1[index]] = { {95 + float(index * 34), 178}, {34} };
				for (int index = 0; index < l2.size(); ++index)
					m_KeysLocation[l2[index]] = { { 78 + float(index * 34), 211 }, {34} };
				for (int index = 0; index < l3.size(); ++index)
					m_KeysLocation[l3[index]] = { {61 + float(index * 34), 244}, {34} };
				addIcon(GLFW_KEY_E, "KeyConfig.icon.0.png");
				addIcon(GLFW_KEY_I, "KeyConfig.icon.1.png");
				addIcon(GLFW_KEY_S, "KeyConfig.icon.2.png");
				addIcon(GLFW_KEY_K, "KeyConfig.icon.3.png");
				addIcon(GLFW_KEY_Q, "KeyConfig.icon.8.png");
				addIcon(GLFW_KEY_Z, "KeyConfig.icon.50.png");
			}
			
			void addIcon(input::KeyCode key_code, const std::string& icon_tex_name)
			{
				m_KeysIcons.push_back(new KeyConfigIcon{ m_KeysLocation[key_code].position,
					icon_tex_name, key_code });
				fillGroup();
			}

			void addSkillIcon(input::KeyCode key_code, input::Key key, const std::string& icon_tex_name,
				input::KeyboardEvent* keyboard_event)
			{
				m_KeysIcons.push_back(new KeyConfigIcon{ m_KeysLocation[key_code].position,
					graphics::TextureManager::Load(icon_tex_name), key_code });
				m_Keyboard->registerToKey(key_code, key, keyboard_event);
				fillGroup();
			}

			void mouse_left_clicked(maths::vec2 mousePos) override
			{
				auto result_item = std::find_if(std::begin(m_KeysIcons),
					std::end(m_KeysIcons), [&](auto const& key_location)
				{ return key_location->getBounds().Contains(mousePos); });
				if (result_item != std::end(m_KeysIcons))
				{
					m_OldItemPos = (*result_item)->getPosition();
					m_HoldIcon = (*result_item);
					m_IsHolding = true;
					return;
				}
			}

			void mouse_left_released(maths::vec2 mousePos) override
			{
				if (m_HoldIcon != nullptr)
				{
					m_IsHolding = false;
					const maths::vec2 item_pos = m_HoldIcon->getBounds().GetMiddle();
					// set new location according to item's center
					auto result_location = std::find_if(std::begin(m_KeysLocation),
						std::end(m_KeysLocation), [&](auto const& element)
					{ return element.second.Contains(item_pos); });
					if (result_location != m_KeysLocation.end())
					{
						const maths::vec2 newPos = result_location->second.position;
						if (newPos != m_OldItemPos)
						{
							const auto result_switch_icon = std::find_if(std::begin(m_KeysIcons),
								std::end(m_KeysIcons),
								[&](auto const& inv_item)
							{ return (inv_item != m_HoldIcon && inv_item->getPosition() == newPos); });
							//KeyConfigIcon * temp_config_icon{ nullptr };
							if (result_switch_icon != std::end(m_KeysIcons))
							{
								(*result_switch_icon)->setPosition(m_OldItemPos);
								(*result_switch_icon)->setKeyCode(m_HoldIcon->getKeyCode());
								//temp_config_icon = (*result_switch_icon);
							}
							//m_KeysIcons[result_location->first] = m_KeysIcons[m_HoldIcon->getKeyCode()];
							//m_KeysIcons[m_HoldIcon->getKeyCode()] = temp_config_icon;
							m_Keyboard->swapKey(m_HoldIcon->getKeyCode(), result_location->first);
							m_HoldIcon->setKeyCode(result_location->first);
						}
						m_HoldIcon->setPosition(newPos);
					}
					else
						m_HoldIcon->setPosition(m_OldItemPos);
					m_HoldIcon = nullptr;
				}
			}

			void mouse_right_clicked(maths::vec2 mousePos) override {}
			void mouse_moved(float mx, float my, maths::vec2 mousePos) override
			{
				if (m_HoldIcon != nullptr)
				{
					m_HoldIcon->setPosition(m_HoldIcon->getBounds().position + maths::vec2(-mx, my));
				}
			}

			void fillGroup()
			{
				m_ForegroundGroup->clear();
				for (auto& icon : m_KeysIcons)
				{
					m_ForegroundGroup->add_ref(icon);
				}
				//m_ForegroundGroup->add(&m_Equips);
			}
		};
	}
}
