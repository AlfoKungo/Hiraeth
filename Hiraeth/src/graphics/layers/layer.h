#pragma once

#include "../renderer2d.h"
#include "../renderable2d.h"
#include "../batchrenderer2d.h"

namespace hiraeth {
	namespace graphics {

		class Layer
		{
		protected:
			Renderer2D* m_Renderer;
			std::vector<Renderable2D*> m_Renderables;
			std::vector<Renderable2D*> m_RefRenderables;
			Shader* m_Shader;
			maths::mat4 m_ProjectionMatrix;
		public:
			Layer(Renderer2D* renderer, Shader* shader, maths::mat4 projectionMatrix);
			Layer(Shader* shader);
		public:
			virtual ~Layer();
			virtual void add(Renderable2D* renderable);
			virtual void add_ref(Renderable2D* renderable);
			void update();
			virtual void render();
			void clear();
		};

	}
}
