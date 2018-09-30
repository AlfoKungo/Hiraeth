#pragma once
#include "graphics/renderable.h"
#include "graphics/sprite.h"
#include "graphics/percented_sprite.h"

namespace hiraeth {
	namespace skills {

		class TopRightIcon : public graphics::Renderable
		{
		private:
			graphics::Sprite m_Sprite;
			graphics::PercentedSprite m_PercentedSprite;
			unsigned int m_SkillIndex;
		public:
			TopRightIcon(unsigned int skill_index, maths::vec2 pos, graphics::Texture* ptex, float duration)
				: m_Sprite{ pos, ptex },
				m_PercentedSprite{ pos - maths::vec2{ 1, 0 }, { 32,30 }, duration },
				m_SkillIndex{skill_index}
			{

			}

			void draw(graphics::Renderer* renderer) const override
			{
				m_Sprite.draw(renderer);
				m_PercentedSprite.draw(renderer);
			}

			void update() override
			{
				m_Sprite.update();
				m_PercentedSprite.update();
			}

			void setPosition(const maths::vec2& position) override
			{
				m_Sprite.setPosition(position);
				m_PercentedSprite.setPosition(position);
			}

			unsigned int getSkillIndex() const
			{
				return m_SkillIndex;
			}

			bool hasSpriteFinished() const override
			{
				return m_PercentedSprite.hasSpriteFinished();
			}
		};
	}
}
