#pragma once

#include "graphics/shader.h"
#include "graphics/Texture.h"
#include "view/camera.h"
#include "tile.h"
#include "physics/foothold.h"
#include "keyboard/keyboard_event.h"
#include <queue>
#include "srl/map_data.h"
#include "portal.h"

namespace hiraeth {
	namespace map {

		class MapLayer 
		{
		private:
			graphics::Shader m_Shader;
			MapRenderer m_Renderer;
			std::vector<Tile> m_Tiles;
			//std::vector<Portal> m_Portals;
			std::vector<physics::FootHold> m_FootHolds;
		public:
			SRL::MapData m_MapData{};
			MapLayer();
			~MapLayer();

			SRL::MapData& getMapData() { return m_MapData; }
			//const std::vector<Portal>& getPortals() const { return m_Portals; }
			const std::vector<SRL::Summon>& getSummons() const { return m_MapData.Summons; }
			const std::vector<physics::FootHold>& getFootHolds() const { return m_FootHolds; }

			void reloadData();

			void draw();
		};
	}
}
