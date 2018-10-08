#pragma once
#include "ui_button.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth {
	namespace ui {

		class UiButtonSideMenu : public UiButton
		{
		private:
			bool m_IsOpen;
			graphics::Sprite m_OpenNormal, m_OpenMouseOver, m_OpenPressed;
			graphics::Sprite m_CloseNormal, m_CloseMouseOver, m_ClosePressed;
		public:
			UiButtonSideMenu(maths::vec2 pos, const std::string& window_name, const std::string& button_name, std::function<void()>&& func_to_call)
				: UiButton(&m_OpenNormal, func_to_call),
				m_IsOpen(false),
				m_OpenNormal(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Open.normal.0.png")),
				m_OpenMouseOver(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Open.mouseOver.0.png")),
				m_OpenPressed(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Open.pressed.0.png")),
				m_CloseNormal(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Close.normal.0.png")),
				m_CloseMouseOver(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Close.mouseOver.0.png")),
				m_ClosePressed(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + "Close.pressed.0.png"))
			{

			}

			void onClick(maths::vec2 mouse_pos) override
			{
				if (isContains(mouse_pos))
				{
					if (m_IsOpen)
						m_Rend = &m_OpenPressed;
					else
						m_Rend = &m_ClosePressed;
					m_IsOpen = !m_IsOpen;
				}
				UiButton::onClick(mouse_pos);
			}

			void onRelease(maths::vec2 mouse_pos) override 
			{
					if (m_IsOpen)
						m_Rend = &m_CloseNormal;
					else
						m_Rend = &m_OpenNormal;
			}

			void onMove(maths::vec2 mouse_pos) override 
			{
				if (isContains(mouse_pos))
					if (m_IsOpen)
						m_Rend = &m_CloseMouseOver;
					else
						m_Rend = &m_OpenMouseOver;
				else
					if (m_IsOpen)
						m_Rend = &m_CloseNormal;
					else
						m_Rend = &m_OpenNormal;
			}
		};
	}
}
