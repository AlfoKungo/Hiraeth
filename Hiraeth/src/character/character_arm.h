#pragma once
#include "graphics/sprited_renderable.h"
#include "graphics/sprite.h"

namespace hiraeth {
	namespace character {
		struct ArmConfigs
		{
			maths::vec2 org, weapon_position;
		};
		class CharacterArm : public graphics::SpritedRenderable
		{
		private:
			graphics::Sprite m_Weapon;
			std::vector<maths::vec2> m_WeaponPositions;
		public:
			CharacterArm(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, std::vector<ArmConfigs> arm_configs)
				: SpritedRenderable(maths::vec2(0), frames_amount, frame_delay, is_loop, ptex, createOriginsByConfigs(arm_configs)),
			m_WeaponPositions(fillWeaponPositions(arm_configs)),
			m_Weapon(10, 20,10, 10, 0xe0ffcc66)
			{
			}
			CharacterArm(unsigned int frames_amount, std::vector<float> frames_delay, bool is_loop, graphics::Texture* ptex, std::vector<ArmConfigs> arm_configs)
				: SpritedRenderable(maths::vec2(0), frames_amount, frames_delay, is_loop, ptex, createOriginsByConfigs(arm_configs)),
			m_WeaponPositions(fillWeaponPositions(arm_configs)),
			m_Weapon(10, 20,10, 10, 0xe0ffcc66)
			{
			}
			CharacterArm(unsigned int frames_amount, float frame_delay, bool is_loop, graphics::Texture* ptex, std::vector<maths::vec2> origins_array)
				: CharacterArm(frames_amount, frame_delay, is_loop, ptex, createConfigsByOrigins(origins_array)){}
			CharacterArm(unsigned int frames_amount, std::vector<float> frames_delay, bool is_loop, graphics::Texture* ptex, std::vector<maths::vec2> origins_array)
				: CharacterArm(frames_amount, frames_delay, is_loop, ptex, createConfigsByOrigins(origins_array)){}
				~CharacterArm() {}

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
		private:
			std::vector<ArmConfigs> createConfigsByOrigins(std::vector<maths::vec2> origins_array)
			{
				std::vector<ArmConfigs> arm_configs;
				for (auto & origin : origins_array)
				{
					arm_configs.push_back(ArmConfigs{ origin, maths::vec2(0) });
				}
				return arm_configs;
			}
			std::vector<maths::vec2> createOriginsByConfigs(std::vector<ArmConfigs> arm_configs)
			{
				std::vector<maths::vec2> origins_array;
				for (auto config : arm_configs)
				{
					origins_array.push_back(config.org);
				}
				return origins_array;
			}
			std::vector<maths::vec2> fillWeaponPositions(std::vector<ArmConfigs> arm_configs)
			{
				std::vector<maths::vec2> weapon_positions;
				for (auto config : arm_configs)
				{
					weapon_positions.push_back(config.weapon_position);
				}
				return weapon_positions;
			}
		};
	}
}
