#pragma once

#include "utils/static_timer.h"
#include "renderable.h"
#include "maths/maths.h"
#include <array>
#include "srl/sprite_data.h"
#include "basic/a_timer.h"
#include "srl/animation_data.h"

namespace hiraeth {
	namespace graphics {

		class SpritedRenderable : public Renderable
		{
		protected:
			unsigned int m_FrameIndex{ 0 };
			bool m_IsOneTime{ false };
		private:
			unsigned int m_FramesAmount;
			float m_FrameWidth;
			
			ATimer m_AnimationTimer;
			int m_LoopDirection{ 1 };
			bool m_IsLoop;
			SRL::AnimationData m_AniData;
			bool m_IsFinished{ false };
		public:
			SpritedRenderable(maths::vec3 position, SRL::AnimationData animation_data, Texture* ptex, bool is_one_time = false);

			SpritedRenderable(maths::vec3 position, const std::string& tex_name,
				SRL::FullAnimationData animation_data, bool is_one_time = false);

			SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float delay, bool is_loop,
				Texture* ptex);

			virtual ~SpritedRenderable() = default;

			void update() override;

			void resetState() override;

			bool hasSpriteFinished() const override
			{
				return m_IsFinished;
			}
		private:
			void update_frame();

			std::vector<maths::vec2> update_uv() const;

			static SRL::AnimationData generate_animation_data(unsigned int frames_amount, float delay,
				bool is_loop, maths::vec2 tex_size)
			{
				const float frame_width = tex_size.x / float(frames_amount);
				SRL::AnimationData ani_data{ {}, is_loop };
				for (unsigned int i = 0; i < frames_amount; ++i)
				{
					ani_data.frames_data.push_back(SRL::SpritedFrameData{ {i*frame_width,0,frame_width,tex_size.y},
						{0, 0}, delay });
				}
				return ani_data;
			}


		};
	}
}
