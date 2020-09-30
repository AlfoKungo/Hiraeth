#include "hrth_pch.h"
#include "group.h"

namespace hiraeth {
	namespace graphics {

		Group::Group(maths::mat4 transform)
			: m_TransformationMatrix(transform)
		{

		}

		Group::Group(const maths::vec2& transform)
			: m_TransformationMatrix((*new maths::mat4(maths::mat4::Translate(transform)))),
			m_Pos(transform)
		{

		}

		Group::Group()
			: Group(maths::vec2(0))
		{

		}

		void Group::add_ref(Renderable* renderable)
		{
			m_RefRenderables.emplace_back(renderable);
		}

		void Group::add(Renderable* renderable)
		{
			m_Renderables.emplace_back(renderable);
			//m_Renderables.push_back(std::make_unique<Renderable>(renderable));
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
			if (is_to_draw)
			{
				renderer->push(m_TransformationMatrix);

				for (const auto & renderable : m_Renderables)
					if (renderable != nullptr)
						renderable->draw(renderer);
				for (const auto & renderable : m_RefRenderables)
					if (renderable != nullptr)
						renderable->draw(renderer);

				renderer->pop();
			}
		}

		void Group::translate(const maths::vec3& pos)
		{
			m_TransformationMatrix *= maths::mat4::Translate(pos);
			m_Pos += pos;
		}

		void Group::update()
		{
			for (auto & renderable : m_Renderables)
				if (renderable != nullptr)
					renderable->update();
			for (const auto & renderable : m_RefRenderables)
				if (renderable != nullptr)
					renderable->update();
		}

		void Group::clear()
		{
			m_Renderables.clear();
			m_RefRenderables.clear();
		}
	}
}