#pragma once

#include "utils/static_timer.h"
#include "renderable.h"
#include "maths/maths.h"
#include <array>

namespace hiraeth {
	namespace graphics {

		class SpritedRenderable : public Renderable
		{
		protected:
			unsigned int m_FrameIndex = 0;
		private:
			unsigned int m_FramesAmount;
			float m_FrameWidth, m_FrameDelay, m_AnimationTimer;
			int m_LoopDirection = 1;
			bool m_IsLoop;
			std::vector<maths::vec2> m_OriginsArray;
			std::vector<float> m_FramesDelay;
		public:
			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, std::vector<float> frames_delay, bool is_loop, Texture* ptex, std::vector<maths::vec2> origins_array);

			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frames_delay, bool  is_loop,
				Texture* ptex, std::vector<maths::vec2> origins_array);
			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frames_delay, bool  is_loop,
				Texture* ptex, maths::vec2 origin);
			virtual ~SpritedRenderable();

			void update() override;

			void resetState() override;
		private:
			std::vector<maths::vec2> create_origins(maths::vec2 origin, unsigned int frames_amount);
			void set_new_frame(unsigned int index);
			
			std::vector<maths::vec2> create_uv_by_pos_size(unsigned int index) const;
			
		};
	}
}
