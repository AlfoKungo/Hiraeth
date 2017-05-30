#pragma once

#include "../renderable.h"
#include "../renderer.h"

namespace hiraeth {
	namespace graphics {

		template <typename T = Renderable>
		class Layer
		{
		protected:
			Renderer* m_Renderer;
			Shader* m_Shader;
			maths::mat4 m_ProjectionMatrix;
		public:
			std::vector<T*> m_Renderables;
			std::vector<T*> m_RefRenderables;
			Layer(Renderer* renderer, Shader* shader, maths::mat4 projectionMatrix)
				: m_Renderer(renderer), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
			{
				m_Shader->enable();
				m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
				GLint texIDs[] =
				{
					0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,
					16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31
				};
				m_Shader->setUniform1iv("textures", texIDs, 32);
				m_Shader->disable();
			}
			Layer(Shader* shader)
				: Layer(new Renderer(), shader, maths::mat4::Orthographic(-800, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
			{

			}
		public:
			virtual ~Layer()
			{
				delete m_Renderer;
				for (int i = 0; i < m_Renderables.size(); i++)
				{
					delete m_Renderables[i];
				}
			}
			virtual void add(T* renderable)
			{
				m_Renderables.push_back(renderable);
			}
			virtual void add_ref(T* renderable)
			{
				m_RefRenderables.push_back(renderable);
			}

			virtual void update()
			{

				for (T* rend : m_RefRenderables)
					if (rend->is_to_update)
						rend->update();
				for (T* rend : m_Renderables)
					if (rend->is_to_update)
						rend->update();
			}
			virtual void render() const
			{
				m_Shader->enable();
				m_Renderer->begin();

				for (auto renderable = m_Renderables.rbegin(); renderable != m_Renderables.rend(); ++renderable)
				{
					if ((*renderable)->is_to_draw)
						(*renderable)->draw(m_Renderer);
				}
				//for (const T* renderable : m_Renderables)
				//	if (renderable->is_to_draw)
				//		renderable->draw(m_Renderer);
				for (auto renderable = m_RefRenderables.rbegin(); renderable != m_RefRenderables.rend(); ++renderable)
				{
					if ((*renderable)->is_to_draw)
						(*renderable)->draw(m_Renderer);
				}
				//for (const T* renderable : m_RefRenderables)
				//	if (renderable->is_to_draw)
				//		renderable->draw(m_Renderer);

				m_Renderer->end();
				m_Renderer->flush();
			}
			void clear()
			{
				for (int i = 0; i < m_Renderables.size(); i++)
				{
					delete m_Renderables[i];
				}
				m_Renderables.clear();
				m_RefRenderables.clear();
			}
		};
	}
}
