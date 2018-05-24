#include "group.h"

namespace hiraeth {
	namespace graphics {

		Group::Group(maths::mat4 transform)
			: m_TransformationMatrix(transform)
		{

		}

		Group::Group(const maths::vec2& transform)
			: m_TransformationMatrix((*new maths::mat4(maths::mat4::Translate(transform))))
		{

		}

		Group::Group()
			: Group(maths::vec2(0))
		{

		}


		Group::~Group()
		{
			for (int i = 0; i < m_Renderables.size(); ++i)
			{
				delete m_Renderables[i];
			}
			//delete (&m_TransformationMatrix);
		}

		void Group::add(Renderable* renderable)
		{
			m_Renderables.push_back(renderable);
		}
		
		void Group::add(Renderable& renderable)
		{
			m_Renderables.push_back(&renderable);
		}

		void Group::draw(Renderer* renderer) const
		{
			renderer->push(m_TransformationMatrix);

			for (const Renderable* renderable : m_Renderables)
				renderable->draw(renderer);

			renderer->pop();
		}

		void Group::translate(const maths::vec3& pos)
		{
			m_TransformationMatrix *= maths::mat4::Translate(pos);
		}

		void Group::update()
		{
			for (Renderable* rend : m_Renderables)
				rend->update();
		}
	}
}