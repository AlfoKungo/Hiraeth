#pragma once

#include "graphics\shader.h"
#include "graphics\Texture.h"
#include "view\camera.h"
#include "tile.h"
#include "physics\foothold.h"

namespace hiraeth {
	namespace map {

		class MapLayer
		{
		private:
			std::vector<Tile*> m_Tiles;
			graphics::Shader m_Shader;
			MapRenderer m_Renderer;
		public:
			std::vector<physics::FootHold> m_FootHolds;
		public:
			MapLayer(const std::string& filename);
			~MapLayer();

			void draw();
			void add(Tile::Serializer s);
			void deserialize_tiles(cereal::BinaryInputArchive* iarchive);
			void clear();
		};
	}
}