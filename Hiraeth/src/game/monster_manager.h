#pragma once

#include <vector>

#include "monster.h"
#include "map/maplayer.h"
#include "graphics/layers/layer.h"
#include "game/character.h"

namespace hiraeth {
	namespace game {

		class MonsterManager : public Updatable
		{
		private:
			//std::vector<Monster*> m_Monsters;
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

		};
	}
}