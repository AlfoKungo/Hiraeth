#pragma once

#include "graphics/shader.h"
#include "graphics/Texture.h"
#include "view/camera.h"
#include "tile.h"
#include "physics/foothold.h"
#include "keyboard/keyboard_event.h"
#include <queue>
#include "map_data.h"

namespace hiraeth {
	namespace map {

		class MapLayer 
		{
		private:
			graphics::Shader m_Shader;
			MapRenderer m_Renderer;
		public:
			MapData m_MapData{};
			MapLayer(const std::string& filename);
			~MapLayer();

			MapData& getMapData() { return m_MapData; }
			const std::vector<std::unique_ptr<Portal>>& getPortals() const { return m_MapData.m_Portals; }
			const std::vector<Summon>& getSummons() const { return m_MapData.m_Summons; }
			const std::vector<physics::FootHold>& getFootHolds() const { return m_MapData.m_FootHolds; }


			void draw();
		};
	}
}
