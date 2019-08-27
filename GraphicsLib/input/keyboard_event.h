#pragma once

namespace hiraeth {
	namespace input {
		typedef unsigned int Key;

		class KeyboardEvent
		{
		public:
			virtual void ButtonClicked(Key control) = 0;
			virtual void ButtonReleased(Key control) = 0;
			virtual ~KeyboardEvent() = default;
		};
	}
}