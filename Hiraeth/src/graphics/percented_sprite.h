#pragma once
#include "graphics/sprited_renderable.h"
#include "sprite.h"

namespace hiraeth {
	namespace graphics {

		//class PercentedSprite : public SpritedRenderable
		class PercentedSprite : public Sprite
		{
			float m_BaseXSize;
			float m_TimeToWait;
			ATimer m_Timer;
		public:
			PercentedSprite(maths::vec2 pos, maths::vec2 size, float time_to_wait)
				//: SpritedRenderable(position, animation_data, ptex )
				: Sprite(pos, size.x, size.y, 0xa0999999 ),
			m_BaseXSize{size.x},
			m_TimeToWait(time_to_wait),
			m_Timer{time_to_wait}
			{
			}
			void update() override
			{
				float percentage = 1 - m_Timer.timeRemain() / m_TimeToWait;
				setSize(maths::vec2{ m_BaseXSize * percentage, getSize().y });
			}

			bool hasSpriteFinished() const override
			{
				return m_Timer.hasExpired();
			}
		};
	}
}