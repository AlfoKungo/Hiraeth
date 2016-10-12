#pragma once

#include "graphics\shader.h"
#include "graphics\Texture.h"
#include "view\camera.h"
#include "tile.h"

namespace hiraeth {
	namespace map {

		class MapLayer
		{
		private:
			std::vector<Tile*> m_Tiles;
			graphics::Texture m_Tex;
			graphics::Shader m_Shader;
			MapRenderer m_Renderer;
			view::Camera* m_Camera;
		public:
			MapLayer(const std::string& filename, view::Camera* camera);
			~MapLayer();

			void draw();
			void add(Tile::Serializer s);
			void deserialize_tiles(cereal::BinaryInputArchive* iarchive);
			void clear();
		};
	}
}