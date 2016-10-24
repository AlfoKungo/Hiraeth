#pragma once

#include "graphics\spritedrenderable.h"
#include "graphics\window.h"

namespace hiraeth {
	namespace game {

		class Character : public graphics::Renderable2D
		{


#define CHARACTER_SPEED 1.5f
#define CHARACTER_GRAVITY 0.5f;
#define FRICTION 0.4f
#define CHARACTER_TIME_BETWEEN_ADDS 0.001f

		private:
			enum StanceState {
				Stand,
				Walk,
			} m_StanceState;
			enum Direction {
				Right,
				Left,
			} m_Direction;
			std::vector<graphics::SpritedRenderable*> m_Renderables;
			std::vector<graphics::SpritedRenderable*> m_WalkRenderables;
			graphics::Window* m_Wnd;
			float m_MovementTimer;
			maths::vec2 m_Force;
			maths::mat4 m_TransformationMatrix;
			Timer* m_Time;
		public:
			Character(maths::vec3 pos, graphics::Texture* ptex, graphics::Texture* hand_tex, graphics::Texture* head_tex, graphics::Texture* body_walk_tex, graphics::Texture* hand_walk_tex, Timer* time, graphics::Window* m_Wnd);
			~Character();

			void update() override;
			void submit(graphics::Renderer2D* renderable) const override;
			//maths::vec3 getPosition() override { return m_TransformationMatrix.; }
		};
	}
}