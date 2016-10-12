#pragma once

#include "utils\timer.h"
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
#include "graphics\spritedrenderable.h"
#include "view\camera.h"
#include "game\character.h"

#include "maplayer.h"

#include <cereal\archives\binary.hpp>
#include <fstream>
#include <cereal\types\memory.hpp>
#include <cereal\types\vector.hpp>

namespace hiraeth {
	namespace map {

		class Map
		{
		private:

			graphics::Texture m_PtTex;

			graphics::Shader m_PtShader;
			graphics::Shader m_BgShader;
			graphics::Shader m_CrShader;

			graphics::TileLayer m_PtLayer;
			graphics::TileLayer m_BgLayer;
			graphics::TileLayer m_CrLayer;
			MapLayer m_MapLayer;

			int m_MapIndex;
			bool m_ChangeMapFlag;

			graphics::Window* m_Wnd;
			view::Camera* m_Camera;
			game::Character m_Char;
			Timer* m_Time;
		public:
			Map(const std::string& filename, int map_index, graphics::Window* wind, view::Camera* wnd, Timer* time);
			~Map();

			void draw();
			void update();
			void change_map(int new_index);

		private:
			void serialize();
			void deserialize(int map_index);
			void deserialize_portals(cereal::BinaryInputArchive* iarchive);
		};

	}
}