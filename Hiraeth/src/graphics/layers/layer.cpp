#include "layer.h"

namespace hiraeth {
	namespace graphics {

		Layer::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix)
			: m_Renderer(renderer), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
		{
			m_Shader->enable();
			m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
			GLint texIDs[] =
			{
				0,1,2,3,4,5,6,7,8,9
			};
			m_Shader->setUniform1iv("textures", texIDs, 10);
			m_Shader->disable();
		}

		Layer::Layer(Shader* shader)
			: Layer(new BatchRenderer2D(), shader, maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
		{

		}

		Layer::~Layer()
		{

			delete m_Renderer;

			for (int i = 0; i < m_Renderables.size(); i++)
			{
					delete m_Renderables[i];
			}
		}

		void Layer::add(Renderable2D* renderable)
		{
			m_Renderables.push_back(renderable);
		}

		void Layer::add_ref(Renderable2D* renderable)
		{
			m_RefRenderables.push_back(renderable);
		}

		void Layer::update()
		{

			for (Renderable2D* rend : m_RefRenderables)
				rend->update();
			for (Renderable2D* rend : m_Renderables)
				rend->update();
		}

		void Layer::render()
		{
			m_Shader->enable();
			m_Renderer->begin();

			for (const Renderable2D* renderable : m_Renderables)
					renderable->submit(m_Renderer);
			for (const Renderable2D* renderable : m_RefRenderables)
					renderable->submit(m_Renderer);

			m_Renderer->end();
			m_Renderer->flush();
		}
		void Layer::clear()
		{
			for (int i = 0; i < m_Renderables.size(); i++)
			{
					delete m_Renderables[i];
			}
			m_Renderables.clear();
			m_RefRenderables.clear();
		}
	}
}