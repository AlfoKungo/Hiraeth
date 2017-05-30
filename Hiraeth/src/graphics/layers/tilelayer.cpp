#include "tilelayer.h"

namespace hiraeth {
	namespace graphics {

		TileLayer::TileLayer(Shader* shader)
			: Layer(new Renderer(), shader, maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{

		}

		TileLayer::~TileLayer()
		{
		}

		void TileLayer::update()
		{

			for (Renderable* rend : m_RefRenderables)
				rend->update();
			for (Renderable* rend : m_Renderables)
				rend->update();
		}
	}
}