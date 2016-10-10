#include "tilelayer.h"

namespace hiraeth {
	namespace graphics {

		TileLayer::TileLayer(Shader* shader)
			: Layer(new BatchRenderer2D(), shader, maths::mat4::orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{

		}

		TileLayer::~TileLayer()
		{
		}

		void TileLayer::update()
		{
			for (std::vector<Renderable2D*>::const_iterator it = m_Renderables.begin(); it != m_Renderables.end(); it++)
				(**it).update();
		}
	}
}