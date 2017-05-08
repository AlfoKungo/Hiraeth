//#include "layer.h"
//
//namespace hiraeth {
//	namespace graphics {
//
//		template <typename T = Renderable2D>
//		Layer<T>::Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix)
//			: m_Renderer(renderer), m_Shader(shader), m_ProjectionMatrix(projectionMatrix)
//		{
//			m_Shader->enable();
//			m_Shader->setUniformMat4("pr_matrix", m_ProjectionMatrix);
//			GLint texIDs[] =
//			{
//				0,1,2,3,4,5,6,7,8,9
//			};
//			m_Shader->setUniform1iv("textures", texIDs, 10);
//			m_Shader->disable();
//		}
//
//		template <typename T = Renderable2D>
//		Layer<T>::Layer(Shader* shader)
//			: Layer(new BatchRenderer2D(), shader, maths::mat4::Orthographic(-800.0f, 800.0f, -450.0f, 450.0f, -1.0f, 1.0f))
//		{
//
//		}
//
//		template <typename T = Renderable2D>
//		Layer<T>::~Layer()
//		{
//			delete m_Renderer;
//			for (int i = 0; i < m_Renderables.size(); i++)
//			{
//				delete m_Renderables[i];
//			}
//		}
//
//		template <typename T = Renderable2D>
//		void Layer<T>::add(T* renderable)
//		{
//			m_Renderables.push_back(renderable);
//		}
//
//		template <typename T = Renderable2D>
//		void Layer<T>::add_ref(T* renderable)
//		{
//			m_RefRenderables.push_back(renderable);
//		}
//
//		template <typename T = Renderable2D>
//		void Layer<T>::update()
//		{
//
//			for (T* rend : m_RefRenderables)
//				rend->update();
//			for (T* rend : m_Renderables)
//				rend->update();
//		}
//
//		template <typename T = Renderable2D>
//		void Layer<T>::render()
//		{
//			m_Shader->enable();
//			m_Renderer->begin();
//
//			for (const T* renderable : m_Renderables)
//				if (renderable->is_to_draw)
//					renderable->submit(m_Renderer);
//			for (const T* renderable : m_RefRenderables)
//				if (renderable->is_to_draw)
//					renderable->submit(m_Renderer);
//			//for (auto window = m_Windows.rbegin(); window != m_Windows.rend(); ++window)
//			//{
//			//	if ((*window)->is_to_draw)
//			//		(*window)->draw();
//			//}
//
//			m_Renderer->end();
//			m_Renderer->flush();
//		}
//
//		template <typename T = Renderable2D>
//		void Layer<T>::clear()
//		{
//			for (int i = 0; i < m_Renderables.size(); i++)
//			{
//				delete m_Renderables[i];
//			}
//			m_Renderables.clear();
//			m_RefRenderables.clear();
//		}
//	}
//}