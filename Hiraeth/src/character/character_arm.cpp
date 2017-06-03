//#include "character_arm.h"
//
//namespace hiraeth {
//	namespace character {
//		template <unsigned int N >
//		CharacterArm<N>::CharacterArm(unsigned frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, std::array<ArmConfigs, N> arm_configs)
//			: SpritedRenderable<N>(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, createOriginsByConfigs(arm_configs, frames_amount)),
//		m_WeaponPosition(0)
//		{
//		}
//
//		template <unsigned int N >
//		CharacterArm<N>::~CharacterArm()
//		{
//		}
//
//		template <unsigned int N >
//		maths::vec2 CharacterArm<N>::getWeaponPosition() const
//		{
//			return m_WeaponPositions[m_FrameIndex];
//		}
//
//		template <unsigned int N >
//		std::array<maths::vec2, N> CharacterArm<N>::createConfigsByOrigins(unsigned frames_amount)
//		{
//			std::array<maths::vec2, N> weapon_positions;
//			for (auto i = 0; i < frames_amount; i++)
//			{
//				weapon_positions[i] = maths::vec2(0);
//			}
//			return weapon_positions;
//		}
//
//		template <unsigned int N >
//		std::array<maths::vec2, N> CharacterArm<N>::createOriginsByConfigs(std::array<ArmConfigs, N> arm_configs, unsigned frames_amount)
//		{
//				std::array<maths::vec2, N> origins_array;
//				for (auto i = 0; i < frames_amount; i++)
//				{
//					origins_array[i] = arm_configs[i].org;
//					m_WeaponPositions[i] = arm_configs[i].weapon_position;
//				}
//				return origins_array;
//		}
//	}
//}