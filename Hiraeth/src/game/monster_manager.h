#pragma once

#include <vector>
#include <queue>
#include <future>

#include "monster.h"
#include "map/maplayer.h"
#include "graphics/layers/layer.h"
#include "game/character.h"
#include "srl/map_data.h"
#include "item/item_manager.h"
#include "monster_data_manager.h"

namespace hiraeth {
	namespace game {

		struct Summoner
		{
			SRL::Summon summon;
			float summonTime;
		};
		class MonsterManager : public Updatable
		{
		private:
			std::queue<Summoner> m_SummonQueue;
			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<Monster> m_Layer;
			item::ItemManager* m_ItemManager;
			Character* const m_Char;
		public:
			MonsterManager(map::MapLayer* map_layer, game::Character* character, item::ItemManager* item_manager);
			void draw();
			void update() override;
			bool checkCollision();
			void mapChanged();
		private:
			static void serialize_data();
		};
	}
}
