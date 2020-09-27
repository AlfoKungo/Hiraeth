#pragma once
#include <map>
#include "base_creature.h"
#include "character/character_body.h"
#include "character/character_arm.h"

namespace hiraeth {
	namespace game {

		enum BodyPart { CBody, CArm, CHead };
		const std::map<StanceState, std::map<BodyPart, SRL::AnimationData>> cg_data_map
		{
		{StcStand,	{
			{CBody, SRL::AnimationData{ {{{0, 0, 23, 31}, {15, 0}, 0.4f}, {{23, 0, 23, 31}, {15, 0}, 0.4f}, {{46, 0, 23, 31}, {15, 0}, 0.4f}}, true }},
			{CArm, SRL::AnimationData{ {{{0, 0, 11, 19}, {33, 10}, 0.4f}, {{11, 0, 11, 19}, {33, 10}, 0.4f}, {{22, 0, 11, 19}, {33, 10}, 0.4f}}, true }},
			{CHead, SRL::AnimationData{ {{{0, 0, 45, 35}, {11, 29}, 0.4f}, {{45, 0, 45, 35}, {11, 29}, 0.4f}, {{90, 0, 45, 35}, {11, 29}, 0.4f}}, true }},
			}
		},
		{StcWalk, {
			{CBody, SRL::AnimationData{ {{{0, 0, 35, 32}, {18, 0}, 0.18f}, {{35, 0, 35, 32}, {18, 0}, 0.18f}, {{70, 0, 35, 32}, {18, 0}, 0.18f}, {{105, 0, 35, 32}, {18, 0}, 0.18f}}, false }},
			{CArm, SRL::AnimationData{ {{{0, 0, 30, 17}, {29, 11}, 0.18f}, {{30, 0, 30, 17}, {29, 11}, 0.18f}, {{60, 0, 30, 17}, {29, 11}, 0.18f}, {{90, 0, 30, 17}, {29, 11}, 0.18f}}, false }},
			{CHead, SRL::AnimationData{ {{{0, 0, 45, 35}, {14, 29}, 0.18f}, {{45, 0, 45, 35}, {14, 29}, 0.18f}, {{90, 0, 45, 35}, {14, 29}, 0.18f}}, false }},
		}},
		{StcAttack, {
			{CBody, SRL::AnimationData{ {{{0, 0, 45, 33}, {19, 0}, 0.35f}, {{45, 0, 45, 33}, {19, 0}, 0.45f }}, false }},
			{CArm, SRL::AnimationData{ {{{0, 0, 30, 18}, {49, 10}, 0.35f}, {{30, 0, 30, 18}, {0, 11}, 0.45f}}, false }},
			{CHead, SRL::AnimationData{ {{{0, 0, 45, 35}, {25, 27}, 0.35f}, {{45, 0, 45, 35}, {8, 26}, 0.45f}}, false }},
		}},
		{StcJump, {
			{CBody, SRL::AnimationData{ {{{0, 0, 35, 30}, {17, 0}, 0.4f}}, false }},
			{CArm, SRL::AnimationData{ {{{0, 0, 13, 11}, {34, 17}, 0.4f}}, false }},
			{CHead, SRL::AnimationData{ {{{0, 0, 39, 35}, {18, 28}, 0.4f}}, false }},
		}},
		};
		const std::map<StanceState, std::vector<maths::vec2>> cg_weapons_locs_map
		{
			{StcStand, std::vector<maths::vec2>{maths::vec2(28, 10), maths::vec2(28, 9), maths::vec2(30, 10)}},
			{StcWalk, std::vector<maths::vec2>{ maths::vec2(33, 11), maths::vec2(22, 10), maths::vec2(33, 11), maths::vec2(35, 12) }},
			{StcAttack, std::vector<maths::vec2>{ maths::vec2(47, 10), maths::vec2(-8, 19)}},
			{StcJump, std::vector<maths::vec2>{ maths::vec2{ 28, 18 }}},
		};
		const std::map<StanceState, std::string> cg_stance_to_names{
			{StcStand, "stand"},
			{StcWalk, "walk"},
			{StcAttack, "attack"},
			{StcJump, "jump"},
		};

		inline void FillCharacterTextureData(std::map<StanceState, std::vector<std::unique_ptr<graphics::Renderable>>>& stance_renderables)
		{
			for (const auto& [stance_state, body_map] : cg_data_map)
			{
				const auto& stance_name = cg_stance_to_names.at(stance_state);
				stance_renderables[stance_state].push_back(std::make_unique<character::CharacterBody>(
					body_map.at(CBody), graphics::TextureManager::Load("char_body_" + stance_name + ".png")));
				stance_renderables[stance_state].push_back(std::make_unique<character::CharacterArm>(
					body_map.at(CArm), graphics::TextureManager::Load("char_hand_" + stance_name + ".png"),
					cg_weapons_locs_map.at(stance_state)));
				stance_renderables[stance_state].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
					body_map.at(CHead), graphics::TextureManager::Load("char_head_" + stance_name + ".png")));
			}
			//m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterBody>(
			//	data_map[Stand][CBody], graphics::TextureManager::Load("char_body_stand.png")));
			//m_StatesRenderables[Stand].push_back(std::make_unique<character::CharacterArm>(
			//	data_map[Stand][CArm], graphics::TextureManager::Load("char_hand_stand.png"),
			//	m_WeaponLocsMap[Stand]));
			//m_StatesRenderables[Stand].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
			//	data_map[Stand][CHead], graphics::TextureManager::Load("char_head.png")));

			//m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterBody>(
			//	data_map[Walk][CBody], graphics::TextureManager::Load("char_body_walk.png")));
			//m_StatesRenderables[Walk].push_back(std::make_unique<character::CharacterArm>(
			//	data_map[Walk][CArm],	graphics::TextureManager::Load("char_hand_walk.png"), 
			//	m_WeaponLocsMap[Walk]));
			//m_StatesRenderables[Walk].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
			//	data_map[Walk][CHead],
			//	graphics::TextureManager::Load("char_head.png")));

			//m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterBody>(
			//	data_map[Attack][CBody], graphics::TextureManager::Load("char_body_attack.png")));
			//m_StatesRenderables[Attack].push_back(std::make_unique<character::CharacterArm>(
			//	data_map[Attack][CArm], graphics::TextureManager::Load("char_hand_attack.png"), 
			//	m_WeaponLocsMap[Attack]));
			//m_StatesRenderables[Attack].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
			//	data_map[Attack][CHead], graphics::TextureManager::Load("char_head_attack.png")));

			//m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterBody>(
			//	data_map[Jump][CBody], graphics::TextureManager::Load("char_body_jump.png")));
			//m_StatesRenderables[Jump].push_back(std::make_unique<character::CharacterArm>(
			//	data_map[Jump][CArm],	graphics::TextureManager::Load("char_hand_jump.png"), 
			//	m_WeaponLocsMap[Jump]));
			//m_StatesRenderables[Jump].push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 }, 
			//	data_map[Jump][CHead], graphics::TextureManager::Load("char_head_jump.png")));
		}

		inline void FillCharacterStandData(std::vector<std::unique_ptr<graphics::Renderable>>& stance_renderables)
		{
			auto stance_state = StcStand;
			const auto & body_map = cg_data_map.at(stance_state);
			const auto& stance_name = cg_stance_to_names.at(stance_state);
			stance_renderables.push_back(std::make_unique<character::CharacterBody>(
				body_map.at(CBody), graphics::TextureManager::Load("char_body_" + stance_name + ".png")));
			stance_renderables.push_back(std::make_unique<character::CharacterArm>(
				body_map.at(CArm), graphics::TextureManager::Load("char_hand_" + stance_name + ".png"),
				cg_weapons_locs_map.at(stance_state)));
			stance_renderables.push_back(std::make_unique<graphics::SpritedRenderable>(maths::vec2{ 0 },
				body_map.at(CHead), graphics::TextureManager::Load("char_head_" + stance_name + ".png")));
		}
	}
}
