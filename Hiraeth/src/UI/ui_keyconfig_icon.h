#pragma once
#include "graphics/sprite.h"

namespace hiraeth {
	namespace ui {

		class KeyConfigIcon : public graphics::Sprite
		{
		private:
			input::KeyCode m_KeyCode;
		public:
			KeyConfigIcon(maths::vec2 position, graphics::Texture* texture, input::KeyCode key_code)
				: graphics::Sprite(position, texture),
			m_KeyCode(key_code)
			{
				
			}
			KeyConfigIcon(maths::vec2 position,const std::string& icon_name, input::KeyCode key_code)
				: graphics::Sprite(position, graphics::TextureManager::Load("Assets/UI/KeyConfig/" + icon_name)),
			m_KeyCode(key_code)
			{
				
			}

			input::KeyCode getKeyCode() const
			{
				return m_KeyCode;
			}

			void setKeyCode(input::KeyCode new_key_code) 
			{
				m_KeyCode = new_key_code;
			}
		};
	}
}
