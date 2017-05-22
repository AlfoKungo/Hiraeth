#pragma once

#include <vector>
#include <GL/glew.h>
#include "font.h"
#include "maths/maths.h"


namespace hiraeth {
	namespace graphics {
		
		class Renderable2D;
		class Renderer2D
		{
		protected:
			std::vector<maths::mat4> m_TransformationStack;
			const maths::mat4* m_TransformationBack;
		protected:
			Renderer2D()
			{
				m_TransformationStack.push_back(maths::mat4::Identity());
				m_TransformationBack = &m_TransformationStack.back();
			}
		public:
			virtual ~Renderer2D() = default;

			void push(const maths::mat4& matrix, bool override = false)
			{
				if (override)
					m_TransformationStack.push_back(matrix);
				else
					m_TransformationStack.push_back(m_TransformationStack.back() * matrix);
				m_TransformationBack = &m_TransformationStack.back();
			}
			void pop()
			{
				//TODO: Add to log!
				if (m_TransformationStack.size() > 1)
					m_TransformationStack.pop_back();

				m_TransformationBack = &m_TransformationStack.back();
			}
			virtual void begin() {}
			virtual void submit(const Renderable2D* renderable) = 0;
			virtual void submit(const Renderable2D* renderable, unsigned int color) = 0;
			virtual void end() {}
			virtual void flush() = 0;
			virtual void drawString(const Font& font, const std::string& text, const maths::vec3& position, unsigned int color) = 0;
		};
	}
}