#pragma once

#include "../renderable.h"
#include <algorithm>

namespace hiraeth {
	namespace graphics {

		template <class RendType = Renderable>
		class TGroup : public Renderable
		{
		private:
			maths::mat4& m_TransformationMatrix;
		public:
			std::vector<std::unique_ptr<RendType>> m_Renderables;
			//std::vector<std::weak_ptr<RendType>> m_RefRenderables;

			TGroup(maths::mat4& transform)
			: m_TransformationMatrix(transform)
			{

			}
			TGroup(const maths::vec2& transform)
			: m_TransformationMatrix((*new maths::mat4(maths::mat4::Translate(transform))))
			{

			}
			TGroup()
			: TGroup(maths::vec2(0))
			{

			}
			
			void add(std::unique_ptr<RendType> renderable)
			{
				m_Renderables.emplace_back(std::move(renderable));
			}
			void add(RendType* renderable)
			{
				m_Renderables.emplace_back(renderable);
			}
			//void add(std::weak_ptr<Renderable> renderable)
			//{
			//	m_RefRenderables.emplace_back(renderable);
			//}
			void draw(Renderer* renderer) const override
			{
				renderer->push(m_TransformationMatrix);

				for (const auto& renderable : m_Renderables)
					renderable->draw(renderer);
				//for (const auto & renderable : m_RefRenderables)
				//	if (auto spt = renderable.lock())
				//		spt->draw(renderer);

				renderer->pop();
			}
			void translate(const maths::vec3& pos)
			{
				m_TransformationMatrix *= maths::mat4::Translate(pos);
			}
			void update() override
			{
				for (auto & renderable : m_Renderables)
					renderable->update();
				//for (const auto & renderable : m_RefRenderables)
				//	if (auto spt = renderable.lock())
				//		spt->update();
			}
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
			void clear() { m_Renderables.clear(); }
			void clear_done()
			{
			m_Renderables.erase(
				std::remove_if(begin(m_Renderables), end(m_Renderables),
					[](const auto& sr) {return sr->hasSpriteFinished(); }), end(m_Renderables));
			}
		};
	}
}
