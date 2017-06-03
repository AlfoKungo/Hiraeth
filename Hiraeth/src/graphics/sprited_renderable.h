#pragma once

#include "utils/static_timer.h"
#include "renderable.h"
#include "maths/maths.h"
#include <array>

namespace hiraeth {
	namespace graphics {

		template <unsigned int N >
		class SpritedRenderable : public Renderable
		{
		protected:
			unsigned int m_FrameIndex;
		private:
			int m_Direction = 1;
			float m_FrameWidth;
			float m_FrameDelay;
			float m_AnimationTimer;
			bool m_IsLoop;
			std::array<maths::vec2, N> m_OriginsArray;
		public:
			SpritedRenderable(maths::vec3 position, float frame_delay, bool is_loop, Texture* ptex, std::array<maths::vec2, N> origins_array)
				: Renderable(position, maths::vec2(ptex->getWidth() / (float)(N), ptex->getHeight()), 0xffffffff, origins_array[0]),
				m_FrameIndex(0), m_FrameWidth(ptex->getWidth() / (float)(N)),
				m_FrameDelay(frame_delay), m_AnimationTimer(StaticTimer::timer.elapsed()),
				m_IsLoop(is_loop), m_OriginsArray(origins_array)
			{
				m_Texture = ptex;
				set_new_frame(m_FrameIndex);
			}
			SpritedRenderable<N>(maths::vec3 position, float frame_delay, bool  is_loop, Texture* ptex, maths::vec2 origin = maths::vec2(0)) :
				SpritedRenderable(position, frame_delay, is_loop, ptex, create_origins(origin)) {}
			SpritedRenderable<N>(maths::vec3 position, float frame_delay, Texture* ptex, maths::vec2 origin = maths::vec2(0)) :
				SpritedRenderable(position, frame_delay, true, ptex, origin) {}
			virtual ~SpritedRenderable<N>()
			{
			}

			void update() override
			{
				if (StaticTimer::timer.elapsed() - m_AnimationTimer > m_FrameDelay)
				{
					set_new_frame(m_FrameIndex);
					m_FrameIndex += m_Direction;
					if (m_IsLoop || N == 1)
					{
						m_FrameIndex %= N;
					}
					else
					{
						if (m_FrameIndex == 0 || m_FrameIndex == N - 1)
						{
							m_Direction *= -1;
						}
					}
					m_AnimationTimer = StaticTimer::timer.elapsed();
				}
			}
			//SpritedRenderable(const SpritedRenderable& other) // copy constructor
			//	: m_FrameIndex(other.m_FrameIndex), m_FramesAmount(other.m_FramesAmount), m_FrameWidth(other.m_FrameWidth), 
			//m_FrameDelay(other.m_FrameDelay), m_AnimationTimer(other.m_AnimationTimer), m_IsLoop(other.m_IsLoop)
			//{
			//	m_OriginsArray = new maths::vec2[other.m_FramesAmount];
			//	std::copy(other.m_OriginsArray, other.m_OriginsArray + other.m_FramesAmount, m_OriginsArray);
			//}

			//SpritedRenderable& operator=(const SpritedRenderable& other) // copy assignment
			//{
			//	m_FrameIndex = other.m_FrameIndex;
			//	m_FramesAmount = other.m_FramesAmount;
			//	m_FrameWidth = other.m_FrameWidth;
			//	m_FrameDelay = other.m_FrameDelay;
			//	m_AnimationTimer = other.m_AnimationTimer;
			//	m_IsLoop = other.m_IsLoop;
			//	m_OriginsArray = new maths::vec2[other.m_FramesAmount];
			//	std::copy(other.m_OriginsArray, other.m_OriginsArray + other.m_FramesAmount, m_OriginsArray);
			//	delete[] other.m_OriginsArray;

			//	return *this;
			//}
		private:
			std::array<maths::vec2, N> create_origins(maths::vec2 origin)
			{
				std::array<maths::vec2, N> origins;
				origins.fill(origin);
				return origins;
			}
			void set_new_frame(unsigned int index)
			{
				m_UV = create_uv_by_pos_size(index);
				m_Org = m_OriginsArray[index];
			}
			std::vector<maths::vec2> create_uv_by_pos_size(unsigned int index) const
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
		};
	}
}
