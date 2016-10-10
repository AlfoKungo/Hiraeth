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
#include "portal.h"
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

			graphics::Shader m_Shader;
			graphics::Shader m_PtShader;
			graphics::Shader m_BgShader;

			graphics::TileLayer m_PtLayer;
			graphics::TileLayer m_BgLayer;

			int m_MapIndex;
			bool m_ChangeMapFlag;

			graphics::Window* m_Wnd;
			view::Camera* m_Camera;
			MapRenderer m_Renderer;
		public:
			Map(const std::string& filename, int map_index, graphics::Window* wind, view::Camera* wnd);
			~Map();

			void draw();
			void update();
			void change_map(int new_index);

		private:
			void serialize();
			void deserialize(int map_index);
			void deserialize_tiles(cereal::BinaryInputArchive* iarchive);
			void deserialize_portals(cereal::BinaryInputArchive* iarchive);
		};

	}
}