#pragma once

#include "../renderable2d.h"

namespace hiraeth {
	namespace graphics {

		class Group : public Renderable2D
		{
		private:
			std::vector<Renderable2D*> m_Renderables;
			maths::mat4 m_TransformationMatrix;
		public:
			Group(const maths::mat4& transform);
			~Group();
			void add(Renderable2D* renderable);
			void draw(Renderer2D* renderer) const override;
			void translate(const maths::vec3& pos);
			void update() override;
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
		};
	}
}