#pragma once

#include <vector>
#include "tile.h"
#include "graphics\texture.h"
#include "graphics\layers\tilelayer.h"
#include "graphics\shader.h"
#include "maths\maths.h"
#include "graphics\sprite.h"
#include "graphics\window.h"

#include "maprenderer.h"
#include "graphics\batchrenderer2d.h"
#include "View\Camera.h"

#include <cereal\archives\binary.hpp>
#include <fstream>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>

namespace hiraeth {
	namespace map {

		class Map
		{
		private:
			std::vector<Tile> m_Tiles;
			graphics::Texture m_Tex;
			graphics::Texture m_BgTex;
			graphics::Shader m_Shader;
			graphics::TileLayer m_BgLayer;
			view::Camera* m_Camera;
			MapRenderer m_Renderer;
		public:
			Map(const std::string& filename, const std::string& bg_filename, std::vector<Tile> tiles, view::Camera* wnd);
			Map(const std::string& filename, const std::string& bg_filename, int offset, view::Camera* wnd);
			Map(const std::string& filename, const std::string& bg_filename, std::vector<Tile> tiles);
			~Map();

			void draw();
			void serialize_tiles();

		private:
			void deserialize_tiles(int offset);
		};

	}
}