#pragma once
#include "graphics/sprited_renderable.h"

namespace hiraeth {
	namespace character {
			struct ArmConfigs
			{
				maths::vec2 org, weapon_position;
			};
		class CharacterArm : public graphics::SpritedRenderable
		{
		private:
			maths::vec2* m_WeaponPositions;
		public:
			CharacterArm(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, ArmConfigs* arm_configs);
			CharacterArm(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, maths::vec2* origins)
			: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, origins), m_WeaponPositions(createConfigsByOrigins(frames_amount)) {}
			~CharacterArm();

			maths::vec2 getWeaponPosition() const;
		private:
			maths::vec2* createConfigsByOrigins(unsigned int frames_amount);
			maths::vec2* createOriginsByConfigs(ArmConfigs* arm_configs, unsigned int frames_amount);
		};
	}
}