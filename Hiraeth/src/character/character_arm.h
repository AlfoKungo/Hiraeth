#pragma once
#include "graphics/sprited_renderable.h"

namespace hiraeth {
	namespace character {
		struct ArmConfigs
		{
			maths::vec2 org, weapon_position;
		};
		template <unsigned int N >
		class CharacterArm : public graphics::SpritedRenderable<N>
		{
		private:
			std::array<maths::vec2, N> m_WeaponPositions;
		public:
			CharacterArm(float frame_delay, bool is_loop, graphics::Texture* ptex, std::array<ArmConfigs, N> arm_configs)
				: SpritedRenderable<N>(maths::vec2(0), frame_delay, is_loop, ptex, createOriginsByConfigs(arm_configs, N))
			{
			}
			CharacterArm(float frame_delay, bool is_loop, graphics::Texture* ptex, std::array<maths::vec2, N> origins_array)
				: SpritedRenderable(maths::vec2(0), frame_delay, is_loop, ptex, origins_array),
				m_WeaponPositions(createConfigsByOrigins()) {}
			~CharacterArm() {}

			maths::vec2 getWeaponPosition() const
			{
				return m_WeaponPositions[m_FrameIndex];
			}
		private:
			std::array<maths::vec2, N> createConfigsByOrigins()
			{
				std::array<maths::vec2, N> weapon_positions;
				for (auto & weapon_position : weapon_positions)
				{
					weapon_position = maths::vec2(0);
				}
				return weapon_positions;
			}
			std::array<maths::vec2, N> createOriginsByConfigs(std::array<ArmConfigs, N> arm_configs)
			{
				std::array<maths::vec2, N> origins_array;
				for (auto i = 0; i < N; ++i)
				{
					origins_array[i] = arm_configs[i].org;
					m_WeaponPositions[i] = arm_configs[i].weapon_position;
				}
				return origins_array;
			}
		};
	}
}