#include "maplayer.h"

namespace hiraeth {
	namespace map {

		MapLayer::MapLayer()
			:
			m_Shader("Assets/shaders/map.vert", "Assets/shaders/map.frag"),
			m_Renderer()
		{

		}

		MapLayer::~MapLayer() { }

		void MapLayer::draw()
		{
			//glActiveTexture(GL_TEXTURE0);
			//glBindTexture(GL_TEXTURE_2D, m_Tex.getID());
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", view::Camera::get_ortho());
			m_Renderer.begin();
			for (auto const& tile : m_Tiles)
				tile.submit(m_Renderer);
			m_Renderer.end();
			m_Renderer.flush();
		}

		void MapLayer::reloadData(unsigned int map_index)
		{
			m_MapData = SRL::deserial<SRL::MapData>("serialized/map.data", map_index);

			m_Tiles.clear();
			for (auto& tile_data : m_MapData.Tiles)
				m_Tiles.push_back(Tile(tile_data));

			//m_Portals.clear();
			//for (auto& portal_data : m_MapData.m_Portals)
			//	m_Portals.push_back(Portal(portal_data));

			m_FootHolds.clear();
			for (auto& foothold_data : m_MapData.FootHolds)
				m_FootHolds.push_back(physics::FootHold(foothold_data));

			m_Renderer.changeTexture(m_MapData.TileTexture);
		}
	}
}
