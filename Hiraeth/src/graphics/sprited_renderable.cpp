#include "sprited_renderable.h"


namespace hiraeth {
	namespace graphics {

			SpritedRenderable::SpritedRenderable(maths::vec3 position, unsigned int frames_amount, std::vector<float> frames_delay, bool is_loop, Texture* ptex, std::vector<maths::vec2> origins_array)
				: Renderable(position, maths::vec2(ptex->getWidth() / float(frames_amount), ptex->getHeight()), 0xffffffff, origins_array[0]),
				m_FramesAmount(frames_amount), m_FrameWidth(ptex->getWidth() / float(frames_amount)),
				m_AnimationTimer(StaticTimer::timer.elapsed()), m_IsLoop(is_loop),
				m_OriginsArray(origins_array), m_FramesDelay(frames_delay)
			{
				m_Texture = ptex;
				set_new_frame(m_FrameIndex);
			}

			SpritedRenderable::SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frames_delay, bool  is_loop,
				Texture* ptex, std::vector<maths::vec2> origins_array ) :
				SpritedRenderable(position,frames_amount, std::vector<float>(frames_amount, frames_delay), is_loop, ptex, origins_array) {}
			SpritedRenderable::SpritedRenderable(maths::vec3 position, unsigned int frames_amount, float frames_delay, bool  is_loop,
				Texture* ptex, maths::vec2 origin) :
				SpritedRenderable(position,frames_amount, std::vector<float>(frames_amount, frames_delay), is_loop, ptex, std::vector<maths::vec2>(frames_amount, origin)) {}

			SpritedRenderable::~SpritedRenderable()
			{
			}

			void SpritedRenderable::update()
			{
				if (StaticTimer::timer.elapsed() - m_AnimationTimer > m_FrameDelay)
				{
					set_new_frame(m_FrameIndex);
					m_FrameIndex += m_LoopDirection;
					if (m_IsLoop || m_FramesAmount == 1)
					{
						m_FrameIndex %= m_FramesAmount;
					}
					else
					{
						if (m_FrameIndex == 0 || m_FrameIndex == m_FramesAmount - 1)
						{
							m_LoopDirection *= -1;
						}
					}
					m_AnimationTimer = StaticTimer::timer.elapsed();
				}
			}

			void SpritedRenderable::resetState()
			{
				m_FrameIndex = 0;
				m_LoopDirection = 1;
			}

			std::vector<maths::vec2> SpritedRenderable::create_origins(maths::vec2 origin, unsigned int frames_amount)
			{
				std::vector<maths::vec2> origins;
				for (int i = 0; i < frames_amount; ++i)
					origins.push_back(origin);
				return origins;
			}
			void SpritedRenderable::set_new_frame(unsigned int index)
			{
				m_UV = create_uv_by_pos_size(index);
				m_Org = m_OriginsArray[index];
				m_FrameDelay = m_FramesDelay[index];
			}
			std::vector<maths::vec2> SpritedRenderable::create_uv_by_pos_size(unsigned int index) const
			{
				auto&& tex_size = m_Texture->getSize();
				maths::vec2 pos(index * m_FrameWidth, 0);
				maths::vec2 size(m_FrameWidth, m_Texture->getHeight());
				std::vector<maths::vec2> uv;
				uv.push_back((pos) / tex_size);
				uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
				uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
				uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
				return uv;
			}
	}
}