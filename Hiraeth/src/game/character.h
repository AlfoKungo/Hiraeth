#pragma once

#include "graphics\spritedrenderable.h"
#include "graphics\window.h"

namespace hiraeth {
	namespace game {

		class Character : public graphics::SpritedRenderable
		{
#define CHARACTER_SPEED 2
#define TIME_BETWEEN_ADDS 0.01f
		private:
			graphics::Window* m_Wnd;
			float m_MovementTimer;
		public:
			Character(maths::vec3 pos, graphics::Texture* ptex, Timer* time, graphics::Window* m_Wnd);
			void update() override;

		};
	}
}