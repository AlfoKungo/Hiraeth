#pragma once

#include "utils\timer.h"
#include "renderable2d.h"
#include "maths\maths.h"

namespace hiraeth {
	namespace graphics {

		class SpritedRenderable : public Renderable2D
		{
		private:
			int m_Direction = 1;
			int m_FrameIndex;
			int m_NextMap;
			int m_FramesAmount;
			float m_FrameWidth;
			float m_FrameDelay;
			float m_AnimationTimer;
			bool m_IsLoop;
		protected:
			Timer* m_Time;
		public:
			SpritedRenderable(maths::vec3 position, int frame_amount, float frame_delay, bool  is_loop, graphics::Texture* ptex, Timer* time);
			SpritedRenderable(maths::vec3 position, int frame_amount, float frame_delay, graphics::Texture* ptex, Timer* time) :
				SpritedRenderable(position, frame_amount, frame_delay, true, ptex, time) {}
			virtual ~SpritedRenderable();

			void update() override;

		private:
			std::vector<maths::vec2> create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size);
			void set_new_frame(int index);
		};
	}
}