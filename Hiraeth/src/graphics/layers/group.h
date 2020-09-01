#pragma once

#include "../renderable.h"

namespace hiraeth {
	namespace graphics {

		class Group : public Renderable
		{
		private:
			maths::mat4 m_TransformationMatrix;
			maths::vec2 m_Pos{ 0,0 };
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
			void setPosition(const maths::vec2& new_pos) override
			{
				translate(new_pos - m_Pos);
			}
			inline const maths::vec2& getPosition() const override { return m_Pos; }
			void update() override;
			maths::mat4 getTransform() const { return m_TransformationMatrix; }
			void clear();
		};
	}
}