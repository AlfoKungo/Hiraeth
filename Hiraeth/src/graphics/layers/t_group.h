#pragma once

#include "../renderable.h"

namespace hiraeth {
	namespace graphics {

		template <class RendType = Renderable>
		class TGroup : public Renderable
		{
		private:
			maths::mat4& m_TransformationMatrix;
		public:
			std::vector<RendType*> m_Renderables;

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
			~TGroup()
			{
				for (int i = 0; i < m_Renderables.size(); ++i)
				{
					delete m_Renderables[i];
				}
				delete (&m_TransformationMatrix);
			}
			void add(RendType* renderable)
			{
				m_Renderables.push_back(renderable);
			}
			void add(RendType& renderable)
			{
				m_Renderables.push_back(&renderable);
			}
			void draw(Renderer* renderer) const override
			{
				renderer->push(m_TransformationMatrix);

				for (const Renderable* renderable : m_Renderables)
					renderable->draw(renderer);

				renderer->pop();
			}
			void translate(const maths::vec3& pos)
			{
				m_TransformationMatrix *= maths::mat4::Translate(pos);
			}
			void update() override
			{
				for (Renderable* rend : m_Renderables)
					rend->update();
			}
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
			void clear() { m_Renderables.clear(); }
		};
	}
}
