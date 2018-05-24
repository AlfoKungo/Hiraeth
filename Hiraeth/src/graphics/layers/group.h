#pragma once

#include "../renderable.h"

namespace hiraeth {
	namespace graphics {

		class Group : public Renderable
		{
		private:
			maths::mat4 m_TransformationMatrix;
		public:
			std::vector<Renderable*> m_Renderables;
			Group(maths::mat4 transform);
			Group(const maths::vec2& transform);
			Group();
			~Group();
			void add(Renderable* renderable);
			void add(Renderable& renderable);
			void draw(Renderer* renderer) const override;
			void translate(const maths::vec3& pos);
			//void set_location(const maths::vec3& pos)
			//{
			//	m_TransformationMatrix = (*new maths::mat4(maths::mat4::Translate(pos)));
			//}
			void update() override;
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
			void clear() { m_Renderables.clear(); }
		};
	}
}