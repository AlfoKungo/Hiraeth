#pragma once

#include <vector>
#include <queue>
#include <future>
#include <fstream>

#include "monster.h"
#include "map/maplayer.h"
#include "graphics/layers/layer.h"
#include "game/character.h"
#include "map/map_data.h"

namespace hiraeth {
	namespace game {
		struct Summoner
		{
			map::Summon summon;
			float summonTime;
		};

		class MonsterManager : public Updatable
		{
		private:
			std::queue<Summoner> m_SummonQueue;
			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<Monster> m_Layer;
			Character* const m_Char;
			//graphics::Layer<graphics::Renderable> m_Layer;
		public:
			MonsterManager(map::MapLayer* map_layer, game::Character* character);
			void draw();
			void update() override;
			bool checkCollision();
			void mapChanged();
		private:
			static void serialize_data();
			static MonsterData deserialize_monster_data(unsigned int map_index);
			
		};
	}
}
