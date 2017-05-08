#pragma once

#include <vector>

#include "monster.h"
#include "map\maplayer.h"
#include "graphics\layers\layer.h"

namespace hiraeth {
	namespace game {

		class MonsterManager : public Updatable
		{
		private:
			//std::vector<Monster*> m_Monsters;
			map::MapLayer* m_MapLayer;
			graphics::Shader m_Shader;
			graphics::Layer<graphics::Renderable2D> m_Layer;
			view::Camera* m_Camera;
		public:
			MonsterManager(map::MapLayer* map_layer);
			void draw();
			void update();

		};
	}
}