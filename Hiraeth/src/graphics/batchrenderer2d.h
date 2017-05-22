#pragma once

#include <cstddef>

#include "renderer2d.h"
#include "renderable2d.h"

#include "buffers/indexbuffer.h"

#if 0
//#include "../../ext/freetype-gl/freetype-gl.h"
#else
//#include "../../ext/freetype-gl_v2/freetype-gl.h"
#endif

namespace hiraeth {
	namespace graphics {

#define RENDERER_MAX_SPRITES    60000
#define RENDERER_VERTEX_SIZE    sizeof(VertexData)
#define RENDERER_SPRITE_SIZE    RENDERER_VERTEX_SIZE * 4
#define RENDERER_BUFFER_SIZE    RENDERER_SPRITE_SIZE * RENDERER_MAX_SPRITES
#define RENDERER_INDICES_SIZE   RENDERER_MAX_SPRITES * 6
#define RENDERER_MAX_TEXTURES   32

#define SHADER_VERTEX_INDEX		0
#define SHADER_UV_INDEX			1
#define SHADER_TID_INDEX		2
#define SHADER_COLOR_INDEX		3
		 
		class BatchRenderer2D : public Renderer2D
		{
		private:
			GLuint m_VAO;
			GLuint m_VBO;
			IndexBuffer* m_IBO;
			GLsizei m_IndexCount;
			VertexData* m_Buffer;

			std::vector<GLuint> m_TextureSlots;
		public:
			BatchRenderer2D();
			~BatchRenderer2D();
			void begin() override;
			void submit(const Renderable2D* renderable) override { submit(renderable, renderable->getColor()); }
			void submit(const Renderable2D* renderable, unsigned int blendColor) override;
			void drawString(const Font& font, const std::string& text, const maths::vec3& position, unsigned int color) override;
			void end() override;
			void flush() override;
		private:
			void init();
		};
	}
}
