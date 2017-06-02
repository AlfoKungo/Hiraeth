#pragma once

#include "utils/static_timer.h"
#include "renderable.h"
#include "maths/maths.h"

namespace hiraeth {
	namespace graphics {
		
		class SpritedRenderable : public Renderable
		{
		protected:
			unsigned int m_FrameIndex;
		private:
			int m_Direction = 1;
			unsigned int m_FramesAmount;
			float m_FrameWidth;
			float m_FrameDelay;
			float m_AnimationTimer;
			bool m_IsLoop;
			maths::vec2 *m_OriginsArray;
		public:
			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frame_delay, bool  is_loop, Texture* ptex, maths::vec2* origins_array);
			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frame_delay, bool  is_loop, Texture* ptex, maths::vec2 origin = maths::vec2(0)) :
				SpritedRenderable(position, frames_amount, frame_delay, is_loop, ptex, create_origins(origin, frames_amount)) {}
			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frame_delay, graphics::Texture* ptex, maths::vec2 origin = maths::vec2(0)) :
				SpritedRenderable(position, frames_amount, frame_delay, true, ptex, origin) {}
			virtual ~SpritedRenderable();

			void update() override;
			//void draw() override;

		private:
			maths::vec2* create_origins(maths::vec2 origin, unsigned int frames_amount)
			{

				maths::vec2 * origins_array = new maths::vec2[frames_amount];
				for (auto i = 0; i < frames_amount; i++)
					origins_array[i] = origin;
				return origins_array;
			}
			virtual void set_new_frame(unsigned int index);
			std::vector<maths::vec2> create_uv_by_pos_size(unsigned int index) const;
		};
	}
}