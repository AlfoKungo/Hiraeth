#pragma once
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace character {

		class CharacterArm : public graphics::SpritedRenderable
		{
		private:
			graphics::Sprite m_Weapon;
			std::vector<maths::vec2> m_WeaponPositions;
		public:
			CharacterArm(const SRL::AnimationData animation_data, graphics::Texture* ptex, std::vector<maths::vec2> weapon_positions)
				: SpritedRenderable(maths::vec2(0), animation_data, ptex),
				m_Weapon(maths::vec2(10, 20), 10, 10, 0xf0ff6347),
				m_WeaponPositions(weapon_positions)
			{
			}
			~CharacterArm() = default;

			maths::vec2 getWeaponPosition() const
			{
				return m_WeaponPositions[m_FrameIndex];
			}

			void draw(graphics::Renderer* renderer, unsigned int color) override
			{
				renderer->submit(this, color);
				m_Weapon.setPosition(m_WeaponPositions[m_FrameIndex]);
				renderer->submit(&m_Weapon, color);
			}
		};
	}
}