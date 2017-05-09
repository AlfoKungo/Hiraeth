#pragma once

#include <vector>

#include "monster.h"
#include "map\maplayer.h"
#include "graphics\layers\layer.h"
#include "game\character.h"

namespace hiraeth {
	namespace game {

		class MonsterManager : public Updatable
		{
		private:
			//std::vector<Monster*> m_Monsters;
			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<Monster> m_Layer;
			game::Character* const m_Char;
			//graphics::Layer<graphics::Renderable2D> m_Layer;
			view::Camera* m_Camera;
		public:
			MonsterManager(map::MapLayer* map_layer, game::Character* character);
			void draw();
			void update();
			bool checkCollision();

		};
	}
}