#pragma once

#include "maths/vec2.h"
#include "keyboard_event.h"

namespace hiraeth {
	namespace input {

		class Key
		{
		private:
			unsigned int m_KeyCode;
			maths::vec2 m_Pos;
			KeyboardEvent *m_KeyEvent;

		public:
			Key(unsigned int key_code, maths::vec2 pos);
			void set_key_event(KeyboardEvent *key_event)
			{
				m_KeyEvent = key_event;
			}
			KeyboardEvent* get_key_event()
			{
				return m_KeyEvent;
			}
		};

	}
}
