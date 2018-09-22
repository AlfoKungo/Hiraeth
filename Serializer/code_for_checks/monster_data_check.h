#pragma once
#include "srl/monster_data.h"
#include <cereal/archives/json.hpp>
#include <fstream>

namespace Checks
{
	static void create_monster_data()
	{

		{
			SRL::CreatureSprites data{{}, maths::vec2{50, 40}};
			data.sprited_data[SRL::MoveState::Stand] = { {
				{{0, 0, 69, 50}, { 25,0 }, 0.2f}, {{69, 0, 69, 50}, { 25, 0}, 0.3f}, 
				{{138, 0, 69, 50}, { 25,0 }, 0.25f}}, false
			};
			//data.sprited_data[SRL::MoveState::Walk] = { {
			//	{{0, 0, 70, 84}, { 28,0 }, 0.2f}, {{70, 0, 70, 84}, { 30, 0}, 0.05f}, 
			//	{{140, 0, 70, 84}, { 28,0 }, 0.15f}, {{210, 0, 70, 84}, { 28,0 }, 0.15f},
			//	{{280, 0, 70, 84}, { 26,2 }, 0.1f}, {{350, 0, 70, 84}, { 26,2 }, 0.1f}, 
			//	{{420, 0, 70, 84}, { 25,2 }, 0.3f}}, false
			//};
			data.sprited_data[SRL::MoveState::Walk] = { {
				{{0, 0, 70, 84}, { 26,0 }, 0.2f}, {{70, 0, 70, 84}, { 27, 0}, 0.05f}, 
				{{140, 0, 70, 84}, { 28,16 }, 0.15f}, {{210, 0, 70, 84}, { 28,17 }, 0.15f},
				{{280, 0, 70, 84}, { 26,10 }, 0.1f}, {{350, 0, 70, 84}, { 26,2 }, 0.1f}, 
				{{420, 0, 70, 84}, { 25,2 }, 0.3f}}, false
			};
			data.sprited_data[SRL::MoveState::Hit] = { {
				{{0, 0, 63, 66}, { 28,0 }, 0.2f}}, false
			};


			std::ofstream map_data_file("data/monster/textures/0/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(map_data_file);
			arout(CEREAL_NVP(data));
		}

		{
			SRL::CreatureSprites data{{}, maths::vec2{40, 40}};
			data.sprited_data[SRL::MoveState::Stand] = { {
				{{0, 0, 56, 52}, { 15,0 }, 0.18f}, {{56, 0, 56, 52}, { 15,0 }, 0.1f}, 
				{{112, 0, 56, 52}, { 15, 0 }, 0.18f}}, false
			};
			data.sprited_data[SRL::MoveState::Walk] = { {
				{{0, 0, 56, 52}, { 15,0 }, 0.15f}, {{56, 0, 56, 52}, { 15,0 }, 0.15f}, 
				{{112, 0, 56, 52}, { 15,0 }, 0.15f}, {{168, 0, 56, 52},  { 15,0 }, 0.15f}}, false
			};
			data.sprited_data[SRL::MoveState::Hit] = { {
				{{0, 0, 56, 56}, { 18,0 }, 0.6f}}, false
			};


			std::ofstream map_data_file("data/monster/textures/1/data.json", std::ios::out);
			cereal::JSONOutputArchive arout(map_data_file);
			arout(CEREAL_NVP(data));
		}

		{
			SRL::MonsterData data{ {"Slime", 5, 4444, 400, 400, 250, 250, 15, 40, 40, 40, 30, 30, 20, 20, 10, 100}, 0 };


			std::ofstream map_data_file("0.json", std::ios::out);
			cereal::JSONOutputArchive arout(map_data_file);
			arout(CEREAL_NVP(data));
		}

	}
}
