#pragma once

#include "utils/static_timer.h"
#include "renderable.h"
#include "maths/maths.h"

namespace hiraeth {
	namespace graphics {

		class SpritedRenderable : public Renderable
		{
		private:
			int m_Direction = 1;
			int m_FrameIndex;
			int m_FramesAmount;
			float m_FrameWidth;
			float m_FrameDelay;
			float m_AnimationTimer;
			bool m_IsLoop;
		protected:
		public:
			SpritedRenderable(maths::vec3 position, int frame_amount, float frame_delay, bool  is_loop, graphics::Texture* ptex);
			SpritedRenderable(maths::vec3 position, int frame_amount, float frame_delay, graphics::Texture* ptex) :
				SpritedRenderable(position, frame_amount, frame_delay, true, ptex) {}
			virtual ~SpritedRenderable();

			void update() override;
			//void draw() override;

		private:
			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
			void set_new_frame(int index);
		};
	}
}