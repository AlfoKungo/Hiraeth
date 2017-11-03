#include "maplayer.h"

namespace hiraeth {
	namespace map {

		MapLayer::MapLayer(const std::string& filename)
			:
			m_Shader("src/shaders/map.vert", "src/shaders/map.frag"),
			m_Renderer(new graphics::Texture(filename, 1))
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
			for (auto const& tile : m_MapData.m_Tiles)
				tile->submit(m_Renderer);
			m_Renderer.end();
			m_Renderer.flush();
		}

	}
}
