#include "character_arm.h"

namespace hiraeth {
	namespace character {
		CharacterArm::CharacterArm(unsigned frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, ArmConfigs* arm_configs)
			: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, createOriginsByConfigs(arm_configs, frames_amount))
		{
		}

		CharacterArm::~CharacterArm()
		{
			delete m_WeaponPositions;
		}

		maths::vec2 CharacterArm::getWeaponPosition() const
		{
			return m_WeaponPositions[m_FrameIndex];
		}

		maths::vec2* CharacterArm::createConfigsByOrigins(unsigned frames_amount)
		{
			maths::vec2 * weapon_positions = new maths::vec2 [frames_amount];
			for (auto i = 0; i < frames_amount; i++)
			{
				weapon_positions[i] = maths::vec2(0);
			}
			return weapon_positions;
		}

		maths::vec2* CharacterArm::createOriginsByConfigs(ArmConfigs* arm_configs, unsigned frames_amount)
		{
			m_WeaponPositions = new maths::vec2[frames_amount];
				maths::vec2 * origins_array = new maths::vec2[frames_amount];
				for (auto i = 0; i < frames_amount; i++)
				{
					origins_array[i] = arm_configs[i].org;
					m_WeaponPositions[i] = arm_configs[i].weapon_position;
				}
				delete arm_configs;
				return origins_array;
		}
	}
}