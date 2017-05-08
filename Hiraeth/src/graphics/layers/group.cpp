#include "group.h"

namespace hiraeth {
	namespace graphics {

		Group::Group(const maths::mat4& transform)
			: m_TransformationMatrix(transform)
		{

		}

		Group::~Group()
		{
			for (int i = 0; i < m_Renderables.size(); i++)
			{
				delete m_Renderables[i];
			}
		}

		void Group::add(Renderable2D* renderable)
		{
			m_Renderables.push_back(renderable);
		}

		void Group::draw(Renderer2D* renderer) const
		{
			renderer->push(m_TransformationMatrix);

			for (const Renderable2D* renderable : m_Renderables)
				renderable->draw(renderer);

			renderer->pop();
		}

		void Group::translate(const maths::vec3& pos)
		{
			m_TransformationMatrix *= maths::mat4::Translate(pos);
		}

		void Group::update()
		{
			for (Renderable2D* rend : m_Renderables)
				rend->update();
		}
	}
}