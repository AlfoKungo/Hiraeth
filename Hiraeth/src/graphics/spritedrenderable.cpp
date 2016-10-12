#include "spritedrenderable.h"

namespace hiraeth {
	namespace graphics {

		SpritedRenderable::SpritedRenderable(maths::vec3 position, int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, Timer* time)
			: Renderable2D(position, maths::vec2(ptex->getWidth() / (float)(frames_amount), ptex->getHeight())),
			m_Time(time), m_FrameIndex(0), m_AnimationTimer(time->elapsed()), m_IsLoop(is_loop),
			m_FrameWidth(ptex->getWidth() / (float)(frames_amount)), m_FrameDelay(frame_delay), m_FramesAmount(frames_amount)
		{
			m_Texture = ptex;
			set_new_frame(m_FrameIndex);
		}

		SpritedRenderable::~SpritedRenderable()
		{
		}

		void SpritedRenderable::update()
		{
			if (m_Time->elapsed() - m_AnimationTimer > m_FrameDelay)
			{
				set_new_frame(m_FrameIndex);
				m_FrameIndex += m_Direction;
				if (m_IsLoop)
				{
					m_FrameIndex %= m_FramesAmount;
				}
				else
				{
					if (m_FrameIndex == 0 || m_FrameIndex == m_FramesAmount - 1)
					{
						m_Direction *= -1;
					}
				}
				m_AnimationTimer = m_Time->elapsed();
			}
		}

		void SpritedRenderable::set_new_frame(int index)
		{
			m_UV = create_uv_by_pos_size(maths::vec2(index * m_FrameWidth, 0) , maths::vec2(m_FrameWidth, m_Texture->getHeight()), maths::vec2(m_Texture->getWidth(), m_Texture->getHeight()));
		}
		
		std::vector<maths::vec2> SpritedRenderable::create_uv_by_pos_size(maths::vec2 pos, maths::vec2 size, maths::vec2 tex_size)
		{
			std::vector<maths::vec2> uv;
			uv.push_back((pos) / tex_size);
			uv.push_back((pos + maths::vec2(0, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, size.y)) / tex_size);
			uv.push_back((pos + maths::vec2(size.x, 0)) / tex_size);
			return uv;
		}
	}
}