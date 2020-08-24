#pragma once

#include <vector>
#include <queue>
#include <future>

#include "monster.h"
#include "map/maplayer.h"
#include "graphics/layers/layer.h"
#include "srl/map_data.h"
#include "item/item_manager.h"
#include "monster_data_manager.h"

namespace hiraeth
{
	namespace game
	{
		struct Summoner
		{
			SRL::Summon summon;
			float summonTime;
		};

		class MonsterManager : public Updatable
		{
		private:
			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			std::map<unsigned int, Monster*> m_Monsters;
			graphics::Layer<Monster> m_Layer;
			//item::ItemManager* m_ItemManager;
			//Character* const m_Char;
		public:
			//MonsterManager(map::MapLayer* map_layer, Character* character, item::ItemManager* item_manager);
			MonsterManager(map::MapLayer* map_layer);
			void draw() const;
			void update() override;
			//void addMonster(unsigned int monster_id, unsigned int summon_index);
			void addMonster(unsigned int monster_id, network::MonsterStateUpdate monster_state);
			void updateMonster(unsigned int monster_id, network::MonsterStateUpdate monster_state);
			void killMonster(unsigned int monster_id);
			//void killMonster(unsigned int monster_id, std::vector<unsigned int> & dropped_items);
			//std::vector<Monster*>* getMonsterLayer() { return &m_Layer.m_Renderables; }
			//std::vector<Monster*>* hitMonster(unsigned int monster_id) { return &m_Layer.m_Renderables; }
			std::map<unsigned int, Monster*>* getMonsters() { return &m_Monsters; }
			//void checkCollision();
			void mapChanged();
			void clearMonsters();
		};
	}
}
