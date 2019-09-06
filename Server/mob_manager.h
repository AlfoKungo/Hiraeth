#pragma once
#include <map>
#include "net/protocol.h"
#include <cereal/types/memory.hpp>
#include "srl/map_data.h"
#include "srl/monster_data.h"
#include "srl/deserial.h"
#include <random>

namespace hiraeth {
	namespace network {

		struct MoveControl
		{
			bool left, right;
		};
		struct MobMoveCommand
		{
			//float beg_time;
			Direction dir{};
			ATimer timer;
			float duration{};
		};
		class MobManager
		{
			using MonsterType = unsigned int;
			using MobIndex = unsigned int;
		private:
			std::random_device rd;
			std::mt19937 gen;
			std::uniform_int_distribution<> dis;
		public:
			std::map<MobIndex, MonsterStateUpdate> m_Monsters;
			std::map<MobIndex, MonsterType> m_MonsterIdToType;
			std::map<MonsterType, SRL::MonsterData> m_MonsterTypeToData;
			std::vector<SRL::FootHoldData> m_Footholds;
			std::map<MobIndex, MobMoveCommand> m_MoveCmds;
			std::vector<SRL::Summon> m_Summons;

			MobManager(unsigned int map_index):
			gen(rd()),
			dis(0, 8)
			{
				auto map_data = SRL::deserial<SRL::MapData>("map", map_index);
				m_Footholds = map_data.FootHolds;
				unsigned int index = 0;
				m_Summons = map_data.Summons;
				for (const auto& summon : map_data.Summons)
				{
					m_MonsterIdToType[index] = summon.monster_type;
					auto monster_data = SRL::deserial<SRL::MonsterData>("monster", summon.monster_type - 1);
					if (m_MonsterTypeToData.find(summon.monster_type) == m_MonsterTypeToData.end())
						m_MonsterTypeToData[summon.monster_type] = SRL::deserial<SRL::MonsterData>("monster", summon.monster_type - 1);
					m_Monsters[index++] = MonsterStateUpdate{ summon.monster_type, summon.position - maths::vec2{0, 6}, {0, 0}, Stand,
						monster_data.StatsStruct.Hp };
				}
				setNewMoveCommand(0, MobMoveCommand{ Left, ATimer{5.0f}, 5 });
				setNewMoveCommand(1, MobMoveCommand{ Right, ATimer{5.0f}, 5 });
			}
		private:
			int modulu(int a, int b)
			{
				return ((a % b) + b) % b;
			}
			void calculateLocationNow(MobIndex mob_index)
			{
				MobMoveCommand& mmc = m_MoveCmds[mob_index];
				auto& mob = m_Monsters[mob_index];
				if (mmc.dir == Stand)
				{
					printf("the location is %f : %f\n", mob.pos.x, mob.pos.y);
					return;
				}
				const auto mob_speed = m_MonsterTypeToData[m_MonsterIdToType[mob_index]].StatsStruct.Speed / 4;
				const auto x_start = m_Summons[mob_index].XBegin;
				const auto x_end = m_Summons[mob_index].XEnd;
				const auto duration = (mmc.duration) - mmc.timer.timeRemain();
				const auto frames_amount = (duration) * 60;
				mmc.duration = mmc.timer.timeRemain();
				const auto& foothold = m_Footholds[4];
				//const auto p1 = foothold.p1 + maths::vec2{ 320, 0 } +22.5f;
				//const auto p2 = foothold.p2 - maths::vec2{320, 0} - 22.5f;
				const auto length = (x_end - x_start) * 0.6f;
				const auto edge = (x_end - x_start) * 0.2f;
				const auto p1 = maths::vec2{ x_start + edge, foothold.p1.y };
				const auto p2 = maths::vec2{ x_end - edge, foothold.p1.y };
				//const auto length = p2.x - p1.x;
				const auto distance_walked = frames_amount * mob_speed;
				auto rel_loc = mob.pos.x - p1.x;
				mob.pos.y = foothold.p1.y + 10;
				if (mmc.dir == Left)
				{
					rel_loc = length - rel_loc;
				}
				rel_loc = rel_loc + distance_walked;
				rel_loc = modulu(rel_loc, length * 2);
				if (rel_loc > length)
				{
					rel_loc = length * 2 - rel_loc;
					if (mmc.dir == Left)
					{
						mmc.dir = Right;
						mob.dir = Right;
						rel_loc = length - rel_loc;
					}
					else
					{
						mmc.dir = Left;
						mob.dir = Left;
					}
				}
				else
					if (mmc.dir == Left)
					{
						rel_loc = length - rel_loc;
					}
				mob.pos.x = p1.x + rel_loc;
				printf("mob %d location is %f : %f, and (%d) distance %f frames %f speed %f\n", mob_index, mob.pos.x, mob.pos.y, mob.dir, 
					distance_walked, frames_amount, mob_speed);
				//printf("the location is %f : %f, and (%d, %d) distance %f, %f, %f,%f\n", mob.pos.x, mob.pos.y, mob.left, mob.right, 
				//	distance_walked, rel_loc1, rel_loc2);
			}
		public:
			void setNewMoveCommand(MobIndex mob_id, MobMoveCommand mmc)
			{
				if (m_MoveCmds.find(mob_id) != m_MoveCmds.end())
					calculateLocationNow(mob_id);
				m_MoveCmds[mob_id] = mmc;
				m_Monsters[mob_id].dir = mmc.dir;
			}
			void recalculateAllMobs()
			{
				for (auto& [index, mob] : m_Monsters)
				{
					if (m_MoveCmds.find(index) != m_MoveCmds.end())
						calculateLocationNow(index);
				}
			}
			std::vector<unsigned int> update()
			{
				std::vector<unsigned int> cmdd_mob_ids;
				for (auto&[id, move_cmd] : m_MoveCmds)
				{
					if (move_cmd.timer.hasExpired())
					{
						decideNewAction(id);
						cmdd_mob_ids.push_back(id);
					}
				}
				return cmdd_mob_ids;
			}
			void decideNewAction(MobIndex mob_id)
			{
				Direction dir;
				if (dis(gen) < 4)
					dir = Stand;
				else
					if (dis(gen) < 6)
						dir = Left;
					else
						dir = Right;
				setNewMoveCommand(mob_id, MobMoveCommand{ dir, ATimer{10.0f}, 10 });
			}

			bool damageMob(MonsterHit monster_damage)
			//bool damageMob(MonsterDamage monster_damage)
			{
				auto& monster = m_Monsters[monster_damage.monster_id];
				if (monster.hp > monster_damage.damage)
				{
					m_Monsters[monster_damage.monster_id].hp -= monster_damage.damage;
					return false;
				}
				return true;
			}
			maths::vec2 monsterDied(MobIndex mob_id)
			{
				//m_Monsters.erase(mob_id);
				auto& monster = m_Monsters[mob_id];
				setNewMoveCommand(mob_id, MobMoveCommand{ Stand, ATimer{10.0f}, 10 });
				monster.hp = m_MonsterTypeToData[m_MonsterIdToType[mob_id]].StatsStruct.Hp;
				const auto dead_pos = monster.pos;
				monster.pos = m_Summons.at(mob_id).position;
				return dead_pos;
			}
		};
	}
}
