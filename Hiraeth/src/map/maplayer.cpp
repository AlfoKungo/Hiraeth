#include "maplayer.h"

namespace hiraeth {
	namespace map {

		MapLayer::MapLayer(const std::string& filename, view::Camera* camera)
			: m_Tex(filename), m_Camera(camera),
			m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_Renderer(filename)
		{

		}
		MapLayer::~MapLayer()
		{
			for (int i = 0; i < m_Tiles.size(); i++)
			{
					delete m_Tiles[i];
			}
			m_Tiles.clear();
		}

		void MapLayer::draw()
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_Tex.getID());
			m_Shader.enable();
			m_Shader.setUniformMat4("pr_matrix", m_Camera->get_ortho());
			m_Renderer.begin();
			for (const Tile* tile : m_Tiles)
				tile->submit(&m_Renderer);
			m_Renderer.end();
			m_Renderer.flush();
		}

		void MapLayer::add(Tile::Serializer s)
		{
			m_Tiles.push_back(new Tile(s));
		}
		void MapLayer::deserialize_tiles(cereal::BinaryInputArchive* iarchive)
		{
			Tile::Serializer s;
			int tile_amount;
			(*iarchive)(tile_amount);
			for (int i = 0; i < tile_amount; i++)
			{
				(*iarchive)(s);
				m_Tiles.push_back(new Tile(s));
			}
		}

		void MapLayer::clear()
		{
			for (int i = 0; i < m_Tiles.size(); i++)
			{
					delete m_Tiles[i];
			}
			m_Tiles.clear();
		}
	}
}
