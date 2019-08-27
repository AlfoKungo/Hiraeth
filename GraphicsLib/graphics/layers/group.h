#pragma once

#include "../renderable.h"

namespace hiraeth {
	namespace graphics {

		class Group : public Renderable
		{
		private:
			maths::mat4 m_TransformationMatrix;
		public:
			std::vector<std::unique_ptr<Renderable>> m_Renderables;
			std::vector<Renderable*> m_RefRenderables;
			Group(maths::mat4 transform);
			Group(const maths::vec2& transform);
			Group();

			void add_ref(Renderable* renderable);
			void add(Renderable* renderable);
			void add(std::unique_ptr<Renderable> renderable);
			void add(std::unique_ptr<Renderable>& renderable);
			//void add(std::weak_ptr<Renderable> renderable);

			void draw(Renderer* renderer) const override;
			void translate(const maths::vec3& pos);
			void update() override;
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
			void clear();
		};
	}
}