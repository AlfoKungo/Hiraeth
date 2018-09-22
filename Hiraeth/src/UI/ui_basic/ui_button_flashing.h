#pragma once
#include "ui_button.h"
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"
#include "graphics/texture_manager.h"

namespace hiraeth {
	namespace ui {

		class UiButtonFlashing : public UiButton
		{
		private:
			bool m_IsEnabled, m_IsClickedOn;
			graphics::SpritedRenderable m_FlashingTexture;
			graphics::Sprite m_MouseOver, m_Disabled, m_Pressed;
		public:
			UiButtonFlashing(maths::vec2 pos, std::string window_name, std::string button_name, std::function<void()>&& func_to_call)
				: UiButton(&m_FlashingTexture,func_to_call),
				//m_FlashingTexture(pos, 4, 0.15f, true, 
				//	graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png"), maths::vec2(0)),
				m_FlashingTexture(pos, 4, 0.15f, true, 
					graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".normal.0.png")),
				m_MouseOver(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".mouseOver.0.png")),
				m_Disabled(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".disabled.0.png")),
				m_Pressed(pos, graphics::TextureManager::Load("Assets/UI/" + window_name + "/" + window_name + ".main." + button_name + ".pressed.0.png")),
				m_IsEnabled(true), m_IsClickedOn(false)
			{

			}

			void onClick(maths::vec2 mouse_pos) override
			{
				if (isContains(mouse_pos) && m_IsEnabled)
				{
					m_IsClickedOn = true;
					m_Rend = &m_Pressed;
				}
			}
			void onRelease(maths::vec2 mouse_pos) override
			{
				if (m_IsClickedOn)
				{
					m_IsClickedOn = false;
					if (isContains(mouse_pos))
					{
						m_IsEnabled = false;
						m_Rend = &m_Disabled;
						UiButton::onClick(mouse_pos);
					}
					else
					{
						m_Rend = &m_FlashingTexture;
					}
				}

			}
			void onMove(maths::vec2 mouse_pos) override
			{
				if (m_IsEnabled && !m_IsClickedOn)
					if (isContains(mouse_pos))
						m_Rend = &m_MouseOver;
					else
						m_Rend = &m_FlashingTexture;
			}
		};
	}
}