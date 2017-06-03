#pragma once

#include "graphics/shader.h"
#include "graphics/Texture.h"
#include "view/camera.h"
#include "tile.h"
#include "physics/foothold.h"
#include "keyboard/keyboard_event.h"

namespace hiraeth {
	namespace map {

		class MapLayer 
		{
		private:
			graphics::Shader m_Shader;
			MapRenderer m_Renderer;
		public:
			std::vector<std::unique_ptr<Tile>>* m_Tiles;
			std::vector<physics::FootHold>* m_FootHolds;
		public:
			MapLayer(const std::string& filename);
			~MapLayer();

			void draw();
			void add(Tile::Serializer s);
			void clear();
		};
	}
}
