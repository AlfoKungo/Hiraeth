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

		void Group::add(Renderable* renderable)
		{
			m_Renderables.emplace_back(renderable);
		}
		
		void Group::add(std::unique_ptr<Renderable> renderable)
		{
			m_Renderables.emplace_back(std::move(renderable));
		}
		void Group::add(std::unique_ptr<Renderable>& renderable)
		{
			m_Renderables.emplace_back(std::move(renderable));
		}
		//void Group::add(std::weak_ptr<Renderable> renderable)
		//{
		//	m_RefRenderables.emplace_back(renderable);
		//}
		void Group::draw(Renderer* renderer) const
		{
			renderer->push(m_TransformationMatrix);

			for (const auto & renderable : m_Renderables)
				if (renderable != nullptr)
					renderable->draw(renderer);
			//for (const auto & renderable : m_RefRenderables)
			//	if (auto spt = renderable.lock())
			//		spt->draw(renderer);

			renderer->pop();
		}

		void Group::translate(const maths::vec3& pos)
		{
			m_TransformationMatrix *= maths::mat4::Translate(pos);
		}

		void Group::update()
		{
			for (auto & renderable : m_Renderables)
				if (renderable != nullptr)
					renderable->update();
			//for (const auto & renderable : m_RefRenderables)
			//	if (auto spt = renderable.lock())
			//		spt->update();
		}
	}
}