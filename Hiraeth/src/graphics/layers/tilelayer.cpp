#include "tilelayer.h"

namespace hiraeth {
	namespace graphics {

		TileLayer::TileLayer(Shader* shader)
			: Layer(new BatchRenderer2D(), shader, maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{

		}

		TileLayer::~TileLayer()
		{
		}

		void TileLayer::update()
		{

			for (Renderable2D* rend : m_RefRenderables)
				rend->update();
			for (Renderable2D* rend : m_Renderables)
				rend->update();
		}
	}
}